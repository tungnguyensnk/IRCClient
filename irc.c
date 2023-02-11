#include "irc.h"

int connect_tcp(char *ip, int port) {
    int sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sfd == -1) {
        perror("socket");
        return -1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);

    if (connect(sfd, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
        perror("connect");
        return -1;
    }

    return sfd;
}

char *getCodeResponse(char *data) {
    char *code = malloc(10);
    sscanf(data, "%*s %s", code);
    return code;
}

char *getNick(char *data) {
    char *tmp = strdup(data);
    char *nick_t = strtok(tmp, "!") + 1;
    return nick_t;
}

char *recv_data(int socket) {
    char *data = malloc(1);
    fd_set read_fds;
    struct timeval timeout;
    int bytes_received, total_bytes = 0;

    FD_ZERO(&read_fds);
    FD_SET(socket, &read_fds);

    timeout.tv_sec = TIME_OUT;
    timeout.tv_usec = 0;
    char buffer[1024] = {0};

    int result = select(socket + 1, &read_fds, NULL, NULL, &timeout);
    if (result == -1) {
        perror("select");
        return NULL;
    } else if (result == 0) {
        return NULL;
    }

    if (FD_ISSET(socket, &read_fds)) {
        while ((bytes_received = recv(socket, buffer, sizeof(buffer), 0)) > 0) {
            buffer[bytes_received] = '\0';
            data = realloc(data, total_bytes + bytes_received + 1);
            memcpy(data + total_bytes, buffer, bytes_received);
            total_bytes += bytes_received;
            data[total_bytes] = '\0';
            if (bytes_received < 1024) {
                break;
            }
        }
        return data;
    }

    return NULL;
}

void ping_pong(int sfd, char *data) {
    char *ping = malloc(10);
    char *pong = malloc(10);
    char *tmp = malloc(10);
    sscanf(data, "%s", ping);
    if (strcmp(ping, "PING") == 0) {
        sscanf(data, "%s %s", ping, tmp);
        sprintf(pong, "PONG %s\r\n", tmp);
        send(sfd, pong, strlen(pong), 0);
    }
}