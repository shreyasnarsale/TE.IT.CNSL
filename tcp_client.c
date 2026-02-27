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
    int client_sockfd;
    char command[MAX], response[MAX];
    struct sockaddr_in server_addr;

    // 1. Create socket
    client_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sockfd < 0)
    {
        perror("Socket creation failed");
        exit(1);
    }

    // 2. Initialize server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(PORT);

    // 3. Connect
    if (connect(client_sockfd,
                (struct sockaddr *)&server_addr,
                sizeof(server_addr)) < 0)
    {
        perror("Connection failed");
        close(client_sockfd);
        exit(1);
    }

    printf("Connected to server...\n");

    while (1)
    {
        printf("Client write: ");
        fgets(command, MAX, stdin);
        command[strcspn(command, "\n")] = 0;

        write(client_sockfd, command, strlen(command) + 1);

        if (strcmp(command, "bye") == 0)
            break;

        memset(response, 0, MAX);
        int n = read(client_sockfd, response, MAX);

        if (n <= 0)
        {
            printf("Server disconnected.\n");
            break;
        }

        printf("Client read: %s\n", response);

        if (strcmp(response, "bye") == 0)
            break;
    }

    close(client_sockfd);
    printf("Connection closed.\n");
    return 0;
}


