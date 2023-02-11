#include "print.h"

char menu[512] = {0}, line[2][512] = {0};

void print(char *msg) {
    sprintf(menu, "%s", msg);
    printf("%s", msg);
    fflush(stdout);
}

void print_menu() {
    printf("%s", menu);
}
void print_response(char *msg) {
    system("clear");
    if (strlen(line[0]) == 0) {
        sprintf(line[0], "%s", msg);
    } else if (strlen(line[1]) == 0) {
        sprintf(line[1], "%s", msg);
    } else {
        sprintf(line[0], "%s", line[1]);
        sprintf(line[1], "%s", msg);
    }
    printf("%s\n%s\n", line[0], line[1]);
    printf("%s", menu);
    fflush(stdout);
}