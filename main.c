#include "recv_thread.h"

char nick[512] = {0};

int main() {
    int sfd = connect_tcp(HOST, PORT);
    char buffer[BUF_SIZE] = {0}, input[512] = {0}, *response;

    recv_data(sfd);
    recv_data(sfd);
    printf("IRC Client\n");
    printf("Input nick: ");
    fgets(input, 512, stdin);
    if (input[strlen(input) - 1] == '\n') {
        input[strlen(input) - 1] = '\0';
    }

    sscanf(input, "%s", nick);
    sprintf(buffer, "nick %s\r\n", nick);
    send(sfd, buffer, strlen(buffer), 0);
    printf("Checking nick...\n");
    if (recv_data(sfd) != NULL) {
        printf("Nick is already in use.\n");
        return 0;
    } else {
        printf("Nick is available.\n");
    }

    printf("Input user: ");
    fgets(input, 512, stdin);
    if (input[strlen(input) - 1] == '\n') {
        input[strlen(input) - 1] = '\0';
    }
    sprintf(buffer, "user %s 0 * :real name\r\n", input);
    send(sfd, buffer, strlen(buffer), 0);

    response = recv_data(sfd);
    if (strcmp(getCodeResponse(response), "001") == 0) {
        printf("Welcome to the Internet Relay Network \n");

        pthread_t tid;
        pthread_create(&tid, NULL, recv_thread, &sfd);

        while (1) {
            printf("Menu: \n");
            printf("1. Show list channel\n");
            printf("2. Join channel\n");
            printf("3. Leave channel\n");
            printf("4. Send private message\n");
            printf("5. Send message to channel\n");
            printf("6. Quit\n");
            printf("Input choice:\n");
            fgets(input, 512, stdin);
            int choice = 0;
            sscanf(input, "%d", &choice);

            switch (choice) {
                case 1:
                    send(sfd, "list\r\n", strlen("list\r\n"), 0);
                    break;
                case 2:
                    printf("Input channel: ");
                    fgets(input, 512, stdin);
                    sprintf(buffer, "join #%s", input);
                    send(sfd, buffer, strlen(buffer), 0);
                    break;
                case 3:
                    printf("Input channel: ");
                    fgets(input, 512, stdin);
                    sprintf(buffer, "part #%s", input);
                    send(sfd, buffer, strlen(buffer), 0);
                    break;
                case 4:
                    printf("Input nick: ");
                    fgets(input, 512, stdin);
                    char *nick_t = malloc(20);
                    sscanf(input, "%s", nick_t);
                    printf("Input message: ");
                    fgets(input, 512, stdin);
                    sprintf(buffer, "privmsg %s :%s", nick_t, input);
                    send(sfd, buffer, strlen(buffer), 0);
                    break;
                case 5:
                    printf("Input channel: ");
                    fgets(input, 512, stdin);
                    char *channel = malloc(20);
                    sscanf(input, "%s", channel);
                    printf("Input message: ");
                    fgets(input, 512, stdin);
                    sprintf(buffer, "privmsg #%s :%s", channel, input);
                    send(sfd, buffer, strlen(buffer), 0);
                    break;
                case 6:
                    send(sfd, "quit\r\n", strlen("quit\r\n"), 0);
                    return 0;
                default:
                    printf("Invalid choice\n");
                    break;

            }
        }
    } else {
        printf("Error: %s", response);
    }
}