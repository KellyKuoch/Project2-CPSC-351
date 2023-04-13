#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>

// global variable to hold the input phrase
char inputPhrase[1000];

// function to be executed by alpha thread
void* alphaThread(void* arg) {
    char* start = inputPhrase;
    char* end = strpbrk(start, " \t\n\r");
    while (end != NULL) {
        if (isalpha(*start)) {
            printf("alpha: %.*s\n", (int)(end - start), start);
        }
        start = end + 1;
        end = strpbrk(start, " \t\n\r");
    }
    if (*start != '\0' && isalpha(*start)) {
        printf("alpha: %s\n", start);
    }
    pthread_exit(NULL);
}

// function to be executed by numeric thread
void* numericThread(void* arg) {
    char* start = inputPhrase;
    char* end = strpbrk(start, " \t\n\r");
    while (end != NULL) {
        if (isdigit(*start)) {
            printf("numeric: %.*s\n", (int)(end - start), start);
        }
        start = end + 1;
        end = strpbrk(start, " \t\n\r");
    }
    if (*start != '\0' && isdigit(*start)) {
        printf("numeric: %s\n", start);
    }
    pthread_exit(NULL);
}

int main(int argc, char** argv) {

    // read input phrase from command line argument or stdin
    if (argc == 2) {
        strncpy(inputPhrase, argv[1], sizeof(inputPhrase));
    } else {
        printf("Enter string: ");
        fgets(inputPhrase, sizeof(inputPhrase), stdin);
        inputPhrase[strcspn(inputPhrase, "\n")] = '\0';
    }

    // create alpha and numeric thrads
    pthread_t alphaTid, numericTid;
    pthread_create(&alphaTid, NULL, alphaThread, NULL);
    pthread_create(&numericTid, NULL, numericThread, NULL);

    // wait for threads to finish
    pthread_join(alphaTid, NULL);
    pthread_join(numericTid, NULL);

    return 0;
}
