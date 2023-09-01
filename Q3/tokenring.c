#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define READ_END 0
#define WRITE_END 1

#define MAX_PIPE_NAME_SIZE 128

int main(int argc, char *argv[])
{

    if (argc != 4)
    {
        printf("usage: tokenring n p t\n\tn - number of processes\n\tp - probability\n\tt - time sleeping\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i <= atoi(argv[1]); i++)
    {
        // alloc memory to create pipeName
        char *pipeName = malloc(MAX_PIPE_NAME_SIZE * sizeof(char));
        sprintf(pipeName, "pipe%ito%i", i, (i % atoi(argv[1])) + 1);

        // check if pipe was created successfully
        if (mkfifo(pipeName, 0666) < 0)
        {
            free(pipeName); // free previous malloc
            perror("Pipe Error");
            exit(EXIT_FAILURE);
        }

        free(pipeName);
    }

    int fifoNumber = 1;

    // create n-1 child processes, father is already a process
    for (int i = 2; i <= atoi(argv[1]); i++)
    {

        pid_t pid = fork();

        if (pid == 0)
        {
            fifoNumber = i;
            break;
        }
        else if (pid < 0)
        {
            perror("Process Error");
            exit(EXIT_FAILURE);
        }
    }

    int openRead;
    int openWrite;

    if (fifoNumber % 2 == 0)
    {

        char *readEnd = malloc(MAX_PIPE_NAME_SIZE * sizeof(char));
        sprintf(readEnd, "pipe%ito%i", ((fifoNumber + atoi(argv[1]) - 2) % atoi(argv[1])) + 1, fifoNumber);

        openRead = open(readEnd, O_RDONLY);

        free(readEnd);

        if (openRead < 0)
        {
            perror("Error");
            exit(EXIT_FAILURE);
        }

        char *writeEnd = malloc(MAX_PIPE_NAME_SIZE * sizeof(char));
        sprintf(writeEnd, "pipe%ito%i", fifoNumber, (fifoNumber % atoi(argv[1])) + 1);

        openWrite = open(writeEnd, O_WRONLY);

        free(writeEnd);

        if (openWrite < 0)
        {
            perror("Error");
            exit(EXIT_FAILURE);
        }
    }
    else // impar
    {

        char *writeEnd = malloc(MAX_PIPE_NAME_SIZE * sizeof(char));
        sprintf(writeEnd, "pipe%ito%i", fifoNumber, (fifoNumber % atoi(argv[1])) + 1);

        openWrite = open(writeEnd, O_WRONLY);

        free(writeEnd);

        if (openWrite < 0)
        {
            perror("Error");
            exit(EXIT_FAILURE);
        }

        char *readEnd = malloc(MAX_PIPE_NAME_SIZE * sizeof(char));
        sprintf(readEnd, "pipe%ito%i", ((fifoNumber + atoi(argv[1]) - 2) % atoi(argv[1])) + 1, fifoNumber);

        openRead = open(readEnd, O_RDONLY);

        free(readEnd);

        if (openRead < 0)
        {
            perror("Error");
            exit(EXIT_FAILURE);
        }
    }

    // write first message to pipe
    if (fifoNumber == atoi(argv[1]))
    {
        write(openWrite, "0\0", 1);
    }

    // seed rand()
    srand(fifoNumber * time(0));

    for (;;)
    {
        // random number between 1 and 100
        float random_number = (float)rand() * 100 / RAND_MAX;

        char token[100] = "\0";
        if (read(openRead, token, 100) < 0)
        {
            printf("Could not read from pipe\n");
            perror("Error");
            exit(EXIT_FAILURE);
        };

        if (random_number < atof(argv[2]))
        {
            printf("[p%i] lock on token (val = %s)\n", fifoNumber, token);
            sleep(atoi(argv[3]));
            printf("[p%i] unlock token\n", fifoNumber);
        }

        int temp = atoi(token) + 1;
        sprintf(token, "%i", temp);

        write(openWrite, token, strlen(token));
    }
}