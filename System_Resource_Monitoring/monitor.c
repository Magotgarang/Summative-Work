#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define CPU_USAGE_FILE "/proc/stat"
#define MEM_USAGE_FILE "/proc/meminfo"
#define NET_USAGE_FILE "/proc/net/dev"

// Function to get CPU usage percentage
float get_cpu_usage() {
    FILE *fp;
    char buffer[256];
    long long int user_time, nice_time, system_time, idle_time, iowait_time, irq_time, softirq_time;
    long long int steal_time, guest_time, guest_nice_time, prev_idle_time = 0, prev_total_time = 0;
    long long int total_time, idle_time_cur;

    fp = fopen(CPU_USAGE_FILE, "r");
    if (fp == NULL) {
        perror("Error opening CPU stats file");
        return -1;
    }

    fgets(buffer, sizeof(buffer), fp);
    sscanf(buffer, "cpu  %lld %lld %lld %lld %lld %lld %lld %lld %lld %lld", 
           &user_time, &nice_time, &system_time, &idle_time_cur, 
           &iowait_time, &irq_time, &softirq_time, &steal_time, 
           &guest_time, &guest_nice_time);

    fclose(fp);

    // Calculate total time and idle time
    total_time = user_time + nice_time + system_time + idle_time_cur + iowait_time + irq_time + softirq_time + steal_time + guest_time + guest_nice_time;
    idle_time = idle_time_cur + iowait_time;

    // Calculate CPU usage percentage
    if (prev_total_time != 0) {
        long long int diff_idle = idle_time - prev_idle_time;
        long long int diff_total = total_time - prev_total_time;

        return (100.0 * (diff_total - diff_idle)) / diff_total;
    }

    prev_total_time = total_time;
    prev_idle_time = idle_time;

    return 0;
}

// Function to get memory usage percentage
float get_memory_usage() {
    FILE *fp;
    char buffer[256];
    long long total_memory, free_memory, available_memory;

    fp = fopen(MEM_USAGE_FILE, "r");
    if (fp == NULL) {
        perror("Error opening memory stats file");
        return -1;
    }

    fgets(buffer, sizeof(buffer), fp);
    sscanf(buffer, "MemTotal: %lld kB", &total_memory);
    fgets(buffer, sizeof(buffer), fp);
    sscanf(buffer, "MemFree: %lld kB", &free_memory);
    fgets(buffer, sizeof(buffer), fp);
    sscanf(buffer, "MemAvailable: %lld kB", &available_memory);

    fclose(fp);

    return 100.0 * (total_memory - available_memory) / total_memory;
}

// Function to get network usage (in KB)
long long get_network_usage() {
    FILE *fp;
    char buffer[256];
    long long recv_bytes, sent_bytes;

    fp = fopen(NET_USAGE_FILE, "r");
    if (fp == NULL) {
        perror("Error opening network stats file");
        return -1;
    }

    // Skipping the first 2 lines
    fgets(buffer, sizeof(buffer), fp);
    fgets(buffer, sizeof(buffer), fp);

    // Reading the third line with network data
    fgets(buffer, sizeof(buffer), fp);
    sscanf(buffer, "eth0: %lld %*d %*d %*d %*d %*d %*d %*d %lld", &recv_bytes, &sent_bytes);

    fclose(fp);

    // Return the total data usage in KB (sum of received and sent bytes)
    return (recv_bytes + sent_bytes) / 1024;
}

// Function to list active processes
void list_processes() {
    FILE *fp;
    char buffer[256];
    fp = popen("ps aux", "r");

    if (fp == NULL) {
        perror("Error listing processes");
        return;
    }

    FILE *process_file = fopen("processes.txt", "w");
    if (process_file == NULL) {
        perror("Error opening processes.txt");
        fclose(fp);
        return;
    }

    // Write the header
    fprintf(process_file, "PID\tUSER\t%%CPU\t%%MEM\tCOMMAND\n");

    // Skip the first line (headers)
    fgets(buffer, sizeof(buffer), fp);

    // Write process info to file
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        fprintf(process_file, "%s", buffer);
    }

    fclose(process_file);
    fclose(fp);
}

// Function to write metrics to the usage.txt file
void log_metrics() {
    FILE *usage_file = fopen("usage.txt", "a");
    if (usage_file == NULL) {
        perror("Error opening usage.txt");
        return;
    }

    // Get current timestamp
    time_t rawtime;
    struct tm *timeinfo;
    char timestamp[9];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(timestamp, sizeof(timestamp), "%H:%M:%S", timeinfo);

    // Get CPU, Memory, and Network usage
    float cpu_usage = get_cpu_usage();
    float memory_usage = get_memory_usage();
    long long network_usage = get_network_usage();

    // Write metrics to file
    fprintf(usage_file, "Timestamp: %s, CPU: %.2f%%, Memory: %.2f%%, Network: %lld KB\n", 
            timestamp, cpu_usage, memory_usage, network_usage);

    fclose(usage_file);
}

int main() {
    // Run indefinitely, collecting metrics every 2 seconds
    while (1) {
        log_metrics();
        list_processes();
        sleep(2); // Delay for 2 seconds before next log
    }

    return 0;
}
