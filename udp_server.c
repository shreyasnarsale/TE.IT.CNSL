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
    struct sockaddr_in server_addr, client_addr;
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
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // 3. Bind
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Bind failed");
        close(sockfd);
        exit(1);
    }

    printf("UDP Server running...\n");

    len = sizeof(client_addr);

    while (1)
    {
        memset(buffer, 0, MAX);

        // Receive from client
        recvfrom(sockfd, buffer, MAX, 0,
                 (struct sockaddr *)&client_addr, &len);

        printf("Client: %s\n", buffer);

        if (strcmp(buffer, "bye") == 0)
            break;

        printf("Server write: ");
        fgets(buffer, MAX, stdin);
        buffer[strcspn(buffer, "\n")] = 0;

        // Send to client
        sendto(sockfd, buffer, strlen(buffer) + 1, 0,
               (struct sockaddr *)&client_addr, len);

        if (strcmp(buffer, "bye") == 0)
            break;
    }

    close(sockfd);
    return 0;
}

