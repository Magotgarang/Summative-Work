#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define MAX_CLIENTS 4
#define MAX_USERNAME_LEN 50
#define MAX_MESSAGE_LEN 1024

int client_sockets[MAX_CLIENTS];
char client_usernames[MAX_CLIENTS][MAX_USERNAME_LEN];

void *handle_client(void *client_socket);

int main() {
    int server_socket, client_socket, client_addr_size;
    struct sockaddr_in server_addr, client_addr;
    pthread_t thread_id;

    // Create server socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12345);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        return -1;
    }

    // Listen for incoming connections
    if (listen(server_socket, MAX_CLIENTS) == -1) {
        perror("Listen failed");
        return -1;
    }

    printf("Server is listening on port 12345...\n");

    client_addr_size = sizeof(client_addr);
    int client_count = 0;

    while (1) {
        // Accept incoming connection
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, (socklen_t *)&client_addr_size);
        if (client_socket == -1) {
            perror("Accept failed");
            continue;
        }

        if (client_count < MAX_CLIENTS) {
            client_sockets[client_count] = client_socket;
            printf("Client %d connected.\n", client_count + 1);
            
            // Ask for username
            char username[MAX_USERNAME_LEN];
            write(client_socket, "Enter your username: ", 21);
            read(client_socket, username, MAX_USERNAME_LEN);
            username[strcspn(username, "\n")] = 0;  // Remove newline character
            strncpy(client_usernames[client_count], username, MAX_USERNAME_LEN);

            printf("Client %d (%s) authenticated.\n", client_count + 1, username);

            // Start a new thread to handle the client
            if (pthread_create(&thread_id, NULL, handle_client, (void *)&client_sockets[client_count]) != 0) {
                perror("Thread creation failed");
                return -1;
            }

            client_count++;
        } else {
            write(client_socket, "Server is full. Try again later.\n", 33);
            close(client_socket);
        }
    }

    return 0;
}

void *handle_client(void *client_socket) {
    int socket = *(int *)client_socket;
    char buffer[MAX_MESSAGE_LEN];
    char message[MAX_MESSAGE_LEN * 2];  // Increase the message buffer size
    int client_index = -1;

    // Find the client index
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] == socket) {
            client_index = i;
            break;
        }
    }

    while (1) {
        // Read the message from the client
        int bytes_received = read(socket, buffer, MAX_MESSAGE_LEN);
        if (bytes_received <= 0) {
            printf("Client %d disconnected.\n", client_index + 1);
            break;
        }

        buffer[bytes_received] = '\0';  // Null-terminate the received message
        printf("Client %d (%s) says: %s\n", client_index + 1, client_usernames[client_index], buffer);

        // Send the message to all clients except the sender
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (client_sockets[i] != socket && client_sockets[i] != 0) {
                // Format the message to include the username
                snprintf(message, sizeof(message), "%s: %s", client_usernames[client_index], buffer);
                write(client_sockets[i], message, strlen(message));
            }
        }
    }

    close(socket);
    client_sockets[client_index] = 0;  // Mark the client socket as unused
    return NULL;
}
