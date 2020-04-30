#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"
#include <assert.h>
#include <time.h>

#define DATA_BUFSIZE 100
#define DATA_FILE "init.txt"
#define PLAN_FILE "mcheyne.txt"
#define URL_SIZE 250


void get_day_and_num(int * day, int * num) {
    FILE * fd = fopen(DATA_FILE, "r");
    if (fd == NULL) {
        printf("error opening data file\n");
    }
    char daybuf[DATA_BUFSIZE] = {0};
    char numbuf[DATA_BUFSIZE] = {0};
    fgets(daybuf, sizeof(daybuf), fd);
    fgets(numbuf, sizeof(numbuf), fd);
    fclose(fd);

    *day = atoi(daybuf);
    *num = atoi(numbuf);
}

char * get_line(int day) {
    FILE * file = fopen(PLAN_FILE, "r");
    if (file == NULL) {
        printf("error opening plan file\n");
    }
    char line[DATA_BUFSIZE] = {0};
    while (day > 0 && fgets(line, sizeof(line), file)) {
        day--;
    }
    fclose(file);

    char * newline = malloc(DATA_BUFSIZE);
    strcpy(newline, line);
    return newline;
}

char * get_chap(char * line, int num) {
    char * chap;

    chap = strtok(line, ";\n");
    num--;
    while (num > 0 && (chap = strtok(NULL, ";\n"))) {
        num--;
    }
    return chap;
}

void increment_data() {
    int day, num;
    char * line, * chap;

    get_day_and_num(&day, &num);
    line = get_line(day);
    chap = get_chap(line, num+1);
    if (chap == NULL) {
        FILE * fd = fopen(DATA_FILE, "w+");
        if (fd == NULL) {
            printf("error opening data file\n");
        }
        day++;
        char buffer[100] = {0};
        sprintf(buffer, "%d", day);
        fputs(buffer, fd);
        fputs("\n1", fd);
        fclose(fd);
    } else {
        FILE * fd = fopen(DATA_FILE, "w+");
        if (fd == NULL) {
            printf("error opening data file\n");
        }
        num++;
        char buf1[100] = {0};
        char buf2[100] = {0};
        sprintf(buf1, "%d", day);
        sprintf(buf2, "%d", num);
        fputs(buf1, fd);
        fputs("\n", fd);
        fputs(buf2, fd);
        fclose(fd);
    }
    free(line);
}

void decrement_data() {
    int day, num;
    char * line;
    char * chap;

    get_day_and_num(&day, &num);
    line = get_line(day);
    if (num == 1) {
        char * prev_line;
        char * temp_chap;
        prev_line = get_line(--day);
        do {
            char line_cpy[100] = {0};
            strcpy(line_cpy, prev_line);
            printf("%s\n", line_cpy);
            temp_chap = get_chap(line_cpy, num);
            num++;
        } while (temp_chap != NULL);
        num=num-2;

        FILE * fd = fopen(DATA_FILE, "w+");
        if (fd == NULL) {
            printf("error opening data file\n");
        }
        char buffer[100] = {0};
        char num_buf[100] = {0};
        sprintf(buffer, "%d", day);
        sprintf(num_buf, "\n%d", num);
        fputs(buffer, fd);
        fputs(num_buf, fd);
        fclose(fd);
    } else {
        chap = get_chap(line, --num);

        FILE * fd = fopen(DATA_FILE, "w+");
        if (fd == NULL) {
            printf("error opening data file\n");
        }
        char buf1[100] = {0};
        char buf2[100] = {0};
        sprintf(buf1, "%d", day);
        sprintf(buf2, "%d", num);
        fputs(buf1, fd);
        fputs("\n", fd);
        fputs(buf2, fd);
        fclose(fd);
    }
    free(line);
}

void display_cur() {
    int day, num;
    char * line, * chap;

    get_day_and_num(&day, &num);
    line = get_line(day);

    char line_cpy[100] = {0};
    strcpy(line_cpy, line);
    chap = get_chap(line_cpy, num);

    printf("Day: %d\n", day);
    printf("Chapters Today: %s", line);
    printf("Current Chapter: %s\n", chap);
    free(line);
}

void read_cur() {
    int day, num;
    char * line, * chap;

    get_day_and_num(&day, &num);
    line = get_line(day);

    char line_cpy[100] = {0};
    strcpy(line_cpy, line);
    chap = get_chap(line_cpy, num);

    char cmd[URL_SIZE] = {0};
    strcpy(cmd, "open ");
    strcat(cmd, "https://www.esv.org/");
    strcat(cmd, chap);
    printf("opening %s with system cmd...\n", chap);
    system(cmd);
}

void display_options() {
    printf("----COMMANDS----\n");
    printf("enter c to see cur\n");
    printf("enter r to read\n");
    printf("enter b to back up\n");
    printf("enter n to skip ahead\n");
    printf("enter q to quit\n");
    printf("----------------\n");
}

int home_execute(char ** args) {
    if (args == NULL) {
        return 1;
    }
    if (args[0] == NULL) {
        return 1;
    }
    if (!strcmp(args[0], "r")) {
        read_cur();
        return 1;
    }
    if (!strcmp(args[0], "o")) {
        display_options();
        return 1;
    }
    if (!strcmp(args[0], "q")) {
        return 0;
    }
    if (!strcmp(args[0], "c")) {
        display_cur();
        return 1;
    }
    if (!strcmp(args[0], "n")) {
        increment_data();
        display_cur();
        return 1;
    }
    if (!strcmp(args[0], "b")) {
        decrement_data();
        display_cur();
        return 1;
    }
    return 1;
}

void home_loop(void) {
    char * line;
    char ** args;
    int status;

    do {
        printf("> ");
        line = lsh_read_line();
        args = lsh_split_line(line);
        status = home_execute(args);

        free(line);
        free(args);
    } while (status);
}

int main(int argc, char ** argv) {
    printf("Welcome to Mcheyne Reading Script\n");
    printf("Type o for options\n");

    home_loop();

    return EXIT_SUCCESS;
}
