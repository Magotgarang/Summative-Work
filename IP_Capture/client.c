#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ifaddrs.h>

#define PORT 12345
#define MAX_MESSAGE_LEN 1024

void get_ip_and_hostname(char *ip, char *hostname);

int main() {
    int socket_desc;
    struct sockaddr_in server_addr;
    char message[MAX_MESSAGE_LEN];
    char ip[INET_ADDRSTRLEN];
    char hostname[100];

    // Create socket
    if ((socket_desc = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Localhost

    // Connect to server
    if (connect(socket_desc, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        return -1;
    }

    // Get local IP and Hostname
    get_ip_and_hostname(ip, hostname);

    // Send IP and hostname to server
    snprintf(message, sizeof(message), "%s,%s", ip, hostname);
    write(socket_desc, message, strlen(message));

    printf("Sent IP: %s, Hostname: %s to the server\n", ip, hostname);

    // Close the socket after sending the information
    close(socket_desc);
    return 0;
}

void get_ip_and_hostname(char *ip, char *hostname) {
    struct ifaddrs *addrs, *tmp;
    getifaddrs(&addrs);
    tmp = addrs;
    
    // Loop through all network interfaces
    while (tmp != NULL) {
        if (tmp->ifa_addr->sa_family == AF_INET) {
            struct sockaddr_in *pAddr = (struct sockaddr_in *)tmp->ifa_addr;
            if (inet_ntop(AF_INET, &pAddr->sin_addr, ip, INET_ADDRSTRLEN) != NULL) {
                // Get the hostname for the IP
                gethostname(hostname, 100);
                break;
            }
        }
        tmp = tmp->ifa_next;
    }

    freeifaddrs(addrs);
}

