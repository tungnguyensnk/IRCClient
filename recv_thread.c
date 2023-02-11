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
            continue;
        }

        // check join channel
        if (strcmp(getCodeResponse(buffer), "JOIN") == 0) {
            char *channel = malloc(10);
            sscanf(buffer, "%*s %*s %s", channel++);
            char *nick_t = getNick(buffer);
            if (strcmp(nick_t, nick) != 0) {
                printf("%s join channel %s\n", nick_t, channel);
            } else {
                printf("Join channel %s success\n", channel);
            }
            continue;
        }

        // check leave channel
        if (strcmp(getCodeResponse(buffer), "PART") == 0) {
            char *channel = malloc(10);
            sscanf(buffer, "%*s %*s %s", channel++);
            char *nick_t = getNick(buffer);
            if (strcmp(nick_t, nick) != 0) {
                printf("%s leave channel %s\n", nick_t, channel);
            } else {
                printf("Leave channel %s success\n", channel);
            }
            continue;
        }

        // handle leave channel but not join
        if (strcmp(getCodeResponse(buffer), "You're not on that channel") == 0) {
            printf("You're not on that channel\n");
            continue;
        }

        // handle send message to channel but not join
        if (strstr(buffer, "You cannot send external messages") != NULL) {
            printf("Channel not found\n");
            continue;
        }

        // handle send private message to nick not available
        if (strstr(buffer, "No such nick") != NULL) {
            printf("Nick not found\n");
            continue;
        }

        // check message from channel or private
        if (strcmp(getCodeResponse(buffer), "PRIVMSG") == 0) {
            char *channel = malloc(10), *msg = malloc(512);
            sscanf(buffer, "%*s %*s %s %s", channel, msg++);
            char *nick_t = getNick(buffer);
            if (channel[0] == '#')
                printf("Message from %s| %s: %s\n", channel, nick_t, msg);
            else
                printf("Message from %s: %s\n", nick_t, msg);
            continue;
        }

        // check nick quit
        if (strcmp(getCodeResponse(buffer), "QUIT") == 0) {
            char *nick_t = getNick(buffer);
            printf("%s quit\n", nick_t);
            continue;
        }
    }
}
