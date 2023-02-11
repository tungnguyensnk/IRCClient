#include "recv_thread.h"
#include "print.h"

char nick[512] = {0};

int main() {
    int sfd = connect_tcp(HOST, PORT);
    char buffer[BUF_SIZE] = {0}, input[512] = {0}, *response;

    remove("response.txt");
    recv_data(sfd);
    recv_data(sfd);
    printf("IRC Client\n");
    printf("Input nick: ");
    //fgets(input, 512, stdin);
    if (input[strlen(input) - 1] == '\n') {
        input[strlen(input) - 1] = '\0';
    }

    sscanf(input, "%s", nick);
    sprintf(buffer, "nick %s\r\n", "nick");
    send(sfd, buffer, strlen(buffer), 0);
    printf("Checking nick...\n");
    if (recv_data(sfd) != NULL) {
        printf("Nick is already in use.\n");
        return 0;
    } else {
        printf("Nick is available.\n");
    }

    printf("Input user: ");
    //fgets(input, 512, stdin);
    if (input[strlen(input) - 1] == '\n') {
        input[strlen(input) - 1] = '\0';
    }
    sprintf(buffer, "user %s 0 * :real name\r\n", "input");
    send(sfd, buffer, strlen(buffer), 0);

    response = recv_data(sfd);
    if (strcmp(getCodeResponse(response), "001") == 0) {
        printf("Welcome to the Internet Relay Network \n");

        pthread_t tid;
        pthread_create(&tid, NULL, recv_thread, &sfd);

        while (1) {
            print("Menu: \n"
                  "1. Show list channel\n"
                  "2. Join channel\n"
                  "3. Leave channel\n"
                  "4. Send private message\n"
                  "5. Send message to channel\n"
                  "6. Quit\n"
                  "Input choice:\n");
            fgets(input, 512, stdin);
            int choice = 0;
            sscanf(input, "%d", &choice);

            switch (choice) {
                case 1:
                    send(sfd, "list\r\n", strlen("list\r\n"), 0);
                    break;
                case 2:
                    print("Input channel: ");
                    fgets(input, 512, stdin);
                    sprintf(buffer, "join #%s", input);
                    send(sfd, buffer, strlen(buffer), 0);
                    break;
                case 3:
                    print("Input channel: ");
                    fgets(input, 512, stdin);
                    sprintf(buffer, "part #%s", input);
                    send(sfd, buffer, strlen(buffer), 0);
                    break;
                case 4:
                    print("Input nick: ");
                    fgets(input, 512, stdin);
                    char *nick_t = malloc(20);
                    sscanf(input, "%s", nick_t);
                    print("Input message: ");
                    fgets(input, 512, stdin);
                    sprintf(buffer, "privmsg %s :%s", nick_t, input);
                    send(sfd, buffer, strlen(buffer), 0);
                    system("clear");
                    break;
                case 5:
                    print("Input channel: ");
                    fgets(input, 512, stdin);
                    char *channel = malloc(20);
                    sscanf(input, "%s", channel);
                    print("Input message: ");
                    fgets(input, 512, stdin);
                    sprintf(buffer, "privmsg #%s :%s", channel, input);
                    send(sfd, buffer, strlen(buffer), 0);
                    system("clear");
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