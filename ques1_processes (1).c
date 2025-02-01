#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "scraper.c"

#define NUM_URLS 8

int main_processes() {
    pid_t pid[NUM_URLS];
    int status;
    for (int i = 0; i < NUM_URLS; i++) {
        pid[i] = fork();
        if (pid[i] == -1) {
            printf("Fork Failed");
            exit(EXIT_FAILURE);
        } else if (pid[i] == 0) {
            scrap_images(urls[i], folder_names[i]);
            exit(EXIT_SUCCESS);
        }
    }
    for (int i = 0; i < NUM_URLS; i++) {
        waitpid(pid[i], &status, 0);
    }
    return 0;
}
