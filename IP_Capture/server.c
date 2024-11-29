#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define MAX_MESSAGE_LEN 1024

void log_ip_hostname(const char *ip, const char *hostname);

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    char buffer[MAX_MESSAGE_LEN];

    // Create server socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) == -1) {
        perror("Listen failed");
        exit(1);
    }

    printf("Server is listening on port %d...\n", PORT);

    while (1) {
        // Accept a client connection
        if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_size)) == -1) {
            perror("Accept failed");
            continue;
        }

        printf("Client connected from %s\n", inet_ntoa(client_addr.sin_addr));

        // Read data from client
        int bytes_received = read(client_socket, buffer, sizeof(buffer));
        if (bytes_received <= 0) {
            perror("Failed to receive data");
            close(client_socket);
            continue;
        }

        buffer[bytes_received] = '\0';  // Null-terminate the string
        printf("Received data: %s\n", buffer);

        // Log the IP and hostname information
        char *ip = strtok(buffer, ",");
        char *hostname = strtok(NULL, ",");
        if (ip && hostname) {
            log_ip_hostname(ip, hostname);
        }

        // Close the connection to the client
        close(client_socket);
    }

    close(server_socket);
    return 0;
}

void log_ip_hostname(const char *ip, const char *hostname) {
    FILE *log_file = fopen("log.txt", "a");
    if (log_file == NULL) {
        perror("Failed to open log file");
        return;
    }

    fprintf(log_file, "IP: %s, Hostname: %s\n", ip, hostname);
    fclose(log_file);
}

