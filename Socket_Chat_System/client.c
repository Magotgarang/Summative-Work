#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_MESSAGE_LEN 1024

int main() {
    int socket_desc;
    struct sockaddr_in server_addr;
    char message[MAX_MESSAGE_LEN];

    // Create socket
    if ((socket_desc = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12345);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Localhost

    // Connect to server
    if (connect(socket_desc, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        return -1;
    }

    // Receive the authentication prompt
    read(socket_desc, message, sizeof(message));
    printf("%s", message);

    // Send username
    char username[50];
    printf("Enter your username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;  // Remove newline character
    write(socket_desc, username, strlen(username));

    // Chat with server
    while (1) {
        printf("Enter message: ");
        fgets(message, sizeof(message), stdin);
        message[strcspn(message, "\n")] = 0;  // Remove newline character
        write(socket_desc, message, strlen(message));

        if (strncmp(message, "exit", 4) == 0) {
            break;
        }

        // Read server response
        int bytes_received = read(socket_desc, message, sizeof(message));
        message[bytes_received] = '\0';  // Null-terminate the received message
        printf("Server: %s\n", message);
    }

    close(socket_desc);
    return 0;
}
