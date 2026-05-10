#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

// Function to read a file and send it over the socket
void send_file_to_client(int client_fd, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("FOPEN");
        char *err_msg = "Error: Could not open file on server.\n";
        send(client_fd, err_msg, strlen(err_msg), 0);
        return;
    }

    char buffer[1024];
    size_t bytes_read;

    // Read the file in chunks and send until End of File (EOF)
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
        send(client_fd, buffer, bytes_read, 0);
    }

    fclose(fp);
}

int main(int argc, char **argv)
{
    struct sockaddr_in server_info = {0};

    server_info.sin_family = AF_INET;
    server_info.sin_port = htons(1337);
    server_info.sin_addr.s_addr = INADDR_ANY; // Listen on all interfaces

    socklen_t server_info_len = sizeof(server_info);

    struct sockaddr client_info = {0};
    socklen_t client_info_len = sizeof(client_info);

    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (0 > sfd) {
        perror("SOCKET");
        return -1;
    }

    if (0 > bind(sfd ,(struct sockaddr*)&server_info, server_info_len)) {
        perror("BIND");
        return -1;
    }

    if (0 > listen(sfd, 0)) {
        perror("LISTEN");
        return -1;
    }
    
    printf("Server listening on port 1337...\n");

    int cfd = accept(sfd, &client_info, &client_info_len);
    if (0 > cfd) {
        perror("ACCEPT");
        return -1;
    }

    printf("Client connected! Sending files...\n");

    // Send the welcome message
    char *hello = "Welcome to an ArchIve TCP Server\n\n--- BEGIN server.c ---\n";
    send(cfd, hello, strlen(hello), 0);

    // Call our function to send server.c
    send_file_to_client(cfd, "server.c");

    // Send a separator between files
    char *separator = "\n\n--- BEGIN client.c ---\n";
    send(cfd, separator, strlen(separator), 0);

    // Call our function to send client.c
    send_file_to_client(cfd, "client.c");
    
    char *footer = "\n\n--- END OF TRANSMISSION ---\n";
    send(cfd, footer, strlen(footer), 0);

    close(cfd);
    close(sfd); // Good practice to close the listening socket too
    return 0;
}