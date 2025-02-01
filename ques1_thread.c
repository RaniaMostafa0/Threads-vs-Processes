#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "scraper.c"

#define NUM_URLS 8
#define MAX_PRODUCTS 16

void * scrape_images(void * arg) {
    int index = *((int *) arg);
    scrap_images(urls[index], folder_names[index]);
    free(arg);
    pthread_exit(NULL);
}

int main_thread() {
    pthread_t threads[NUM_URLS][MAX_PRODUCTS];
    int i, j;
    int * thread_args;
    for (i = 0; i < NUM_URLS; i++) {
        for (j = 0; j < MAX_PRODUCTS; j++) {
            thread_args = malloc(sizeof(int));
            *thread_args = i;
            if (pthread_create(&threads[i][j], NULL, scrape_images, (void *) thread_args) != 0) {
                fprintf(stderr, "Error creating thread %d\n", j);
                exit(EXIT_FAILURE);
            }
        }
    }
    for (i = 0; i < NUM_URLS; i++) {
        for (j = 0; j < MAX_PRODUCTS; j++) {
            pthread_join(threads[i][j], NULL);
        }
    }
    return 0;
}
