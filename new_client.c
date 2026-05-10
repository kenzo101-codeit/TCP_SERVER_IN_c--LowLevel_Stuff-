#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

// Function to handle receiving data in a loop
void receive_data_from_server(int socket_fd) {
    char buffer[1024];
    ssize_t bytes_received;

    // Loop until recv returns 0 (server closed connection) or < 0 (error)
    while ((bytes_received = recv(socket_fd, buffer, sizeof(buffer) - 1, 0)) > 0) {
        // Null-terminate the buffer so printf knows where the string ends
        buffer[bytes_received] = '\0'; 
        printf("%s", buffer);
    }

    if (bytes_received < 0) {
        perror("RECV");
    }
}

int main(int argc, char **argv)
{
    struct sockaddr_in server_info = {0};

    server_info.sin_family = AF_INET;
    server_info.sin_addr.s_addr = htonl(0x7f000001); // 127.0.0.1
    server_info.sin_port = htons(1337);
    
    socklen_t server_info_len = sizeof(server_info);
    
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (0 > sfd) {
        perror("SOCKET");
        return -1;
    }
    
    if (0 > connect(sfd, (struct sockaddr*)&server_info, server_info_len)) {
        perror("CONNECT");
        return -1;
    }
    
    printf("Connected to server. Receiving data...\n");
    
    // Call our function to handle the incoming stream
    receive_data_from_server(sfd);
    
    close(sfd);
    return 0;
}