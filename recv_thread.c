#include "recv_thread.h"

extern char nick[512];

void *recv_thread(void *arg) {
    int sfd = *(int *) arg;
    char *buffer = malloc(BUF_SIZE);
    while (1) {
        int bytes = recv(sfd, buffer, BUF_SIZE, 0);
        buffer[bytes] = '\0';
        ping_pong(sfd, buffer);

        if (strstr(buffer, "Channel :Users Name") != NULL) {
            system("clear");
            printf("List channel: \n");
            char *tmp = strtok(strdup(buffer), "\r\n");
            int i = 0;
            while (tmp != NULL) {
                if (strcmp(getCodeResponse(tmp), "322") == 0) {
                    char *channel = malloc(10);
                    sscanf(tmp, "%*s %*s %*s %s", channel);
                    printf("%2d. %s\n", ++i, channel);
                }
                tmp = strtok(NULL, "\r\n");
            }
            if (i == 0) {
                printf("No channel available\n");
            }

            print_menu();
            continue;
        }

        // check join channel
        if (strcmp(getCodeResponse(buffer), "JOIN") == 0) {
            char *channel = malloc(10);
            sscanf(buffer, "%*s %*s %s", channel++);
            char *nick_t = getNick(buffer);
            if (strcmp(nick_t, nick) != 0) {
                sprintf(buffer, "%s join channel %s", nick_t, channel);
                print_response(buffer);
            } else {
                sprintf(buffer, "Join channel %s success", channel);
                print_response(buffer);
            }
            continue;
        }

        // check leave channel
        if (strcmp(getCodeResponse(buffer), "PART") == 0) {
            char *channel = malloc(10);
            sscanf(buffer, "%*s %*s %s", channel++);
            char *nick_t = getNick(buffer);
            if (strcmp(nick_t, nick) != 0) {
                sprintf(buffer, "%s leave channel %s", nick_t, channel);
                print_response(buffer);
            } else {
                sprintf(buffer, "Leave channel %s success", channel);
                print_response(buffer);
            }
            continue;
        }

        // handle leave channel but not join
        if (strcmp(getCodeResponse(buffer), "You're not on that channel") == 0) {
            print_response("You're not on that channel");
            continue;
        }

        // handle send message to channel but not join
        if (strstr(buffer, "You cannot send external messages") != NULL) {
            print_response("Channel not found");
            continue;
        }

        // handle send private message to nick not available
        if (strstr(buffer, "No such nick") != NULL) {
            print_response("Nick not found");
            continue;
        }

        // check message from channel or private
        if (strcmp(getCodeResponse(buffer), "PRIVMSG") == 0) {
            char *channel = malloc(10), *msg = malloc(512);
            sscanf(buffer, "%*s %*s %s %s", channel, msg++);
            char *nick_t = getNick(buffer);
            if (channel[0] == '#')
                sprintf(buffer, "Message from %s| %s: %s", channel, nick_t, msg);
            else
                sprintf(buffer, "Message from %s: %s", nick_t, msg);
            print_response(buffer);
            continue;
        }

        // check nick quit
        if (strcmp(getCodeResponse(buffer), "QUIT") == 0) {
            char *nick_t = getNick(buffer);
            sprintf(buffer, "%s quit", nick_t);
            print_response(buffer);
            continue;
        }
    }
}
