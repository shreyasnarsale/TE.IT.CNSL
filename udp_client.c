#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX 1024
#define PORT 9734

int main()
{
    int sockfd;
    char buffer[MAX];
    struct sockaddr_in server_addr;
    socklen_t len;

    // 1. Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("Socket creation failed");
        exit(1);
    }

    // 2. Initialize server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(PORT);

    len = sizeof(server_addr);

    printf("UDP Client ready...\n");

    while (1)
    {
        printf("Client write: ");
        fgets(buffer, MAX, stdin);
        buffer[strcspn(buffer, "\n")] = 0;

        // Send to server
        sendto(sockfd, buffer, strlen(buffer) + 1, 0,
               (struct sockaddr *)&server_addr, len);

        if (strcmp(buffer, "bye") == 0)
            break;

        memset(buffer, 0, MAX);

        // Receive from server
        recvfrom(sockfd, buffer, MAX, 0,
                 (struct sockaddr *)&server_addr, &len);

        printf("Server: %s\n", buffer);

        if (strcmp(buffer, "bye") == 0)
            break;
    }

    close(sockfd);
    return 0;
}

