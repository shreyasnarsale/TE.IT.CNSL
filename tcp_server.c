#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define MAX 1024
#define PORT 9734

int main()
{
    int server_sockfd, client_sockfd;
    char command[MAX], response[MAX];
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;

    // 1. Create socket
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sockfd < 0)
    {
        perror("Socket creation failed");
        exit(1);
    }

    // 2. Initialize server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(PORT);

    // 3. Bind
    if (bind(server_sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Bind failed");
        close(server_sockfd);
        exit(1);
    }

    // 4. Listen
    listen(server_sockfd, 5);
    printf("Server waiting for client...\n");

    while (1)
    {
        client_len = sizeof(client_addr);

        // 5. Accept
        client_sockfd = accept(server_sockfd,
                               (struct sockaddr *)&client_addr,
                               &client_len);

        if (client_sockfd < 0)
        {
            perror("Accept failed");
            continue;
        }

        printf("Client connected...\n");

        while (1)
        {
            memset(command, 0, MAX);

            int n = read(client_sockfd, command, MAX);
            if (n <= 0)
            {
                printf("Client disconnected.\n");
                break;
            }

            printf("\nServer read: %s\n", command);

            if (strcmp(command, "bye") == 0)
                break;

            printf("Server write: ");
            fgets(response, MAX, stdin);
            response[strcspn(response, "\n")] = 0;

            write(client_sockfd, response, strlen(response) + 1);

            if (strcmp(response, "bye") == 0)
                break;
        }

        close(client_sockfd);
        printf("Connection closed.\n\n");
    }

    close(server_sockfd);
    return 0;
}


