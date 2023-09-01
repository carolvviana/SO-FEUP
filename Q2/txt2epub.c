#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

int bar(const char *filename)
{
    printf("[%d] converting %s...\n",
           getpid(), filename);
    return 1;
}
char *split(char *filename)
{
    char *res;
    // similar to malloc, initialize all the allocated memory with 0
    res = calloc((sizeof(filename) - sizeof(char)) * 4, sizeof(char)); // space needed is lenght filename - '.txt'
    char *lim = strrchr(filename, '.');                                // searches for the last occurrence of the character in the string
    int n = lim - filename;                                            // lenght of the filename without the extension
    strncpy(res, filename, n);                                         // copy only what we need
    return res;
}

int main(int argc, char **argv)
{
    int n = atoi(argv[1]);
    int i = 0;
    int status = 0;
    char *args_list[argc + 4];                   // since the nr of arguments may vary, it is necessary to create an array with all the arguments to execute in execvp()
    int o_size = sizeof(argv[1] + sizeof(char)); // output filename has one extra char
    for (int i = 0; i < argc + 4; i++)
    {
        args_list[i] = calloc(o_size, sizeof(char)); // allocate memory for each argument
    }
    args_list[0] = "zip";      // name of the command
    args_list[1] = "--quiet";  // flag to quiet warnings
    args_list[2] = "epub.zip"; // name of output file

    if (n == 0)
        n = argc - 1; // we need argc - 1 child processes
    printf("Creating %d children\n", n);

    for (i = 0; i < n; i++)
    {

        char *filename = argv[i + 1]; // get name of file we want to process
        char *name = split(filename);
        char *outname = calloc(o_size, sizeof(char));
        outname = strcat(name, ".epub"); // create output name for said file
        args_list[i + 3] = outname;      // add it to our arguments list for the zip command

        pid_t pid = fork();
        if (pid == 0) /* only execute this if child */
        {
            bar(argv[i + 1]);
            execlp("pandoc", "pandoc", filename, "--quiet", "-o", outname, NULL); // here the number of arguments is fixed

            exit(0);
        }
        if (pid == -1) /* if fork() fails*/
        {
            printf("fork() failed\n");
            return EXIT_FAILURE;
        }
        // wait(&status);  /* only the parent waits */
    }
    while (wait(&status) > 0)
        ;                       // parent waits for all children
    args_list[argc + 3] = NULL; // last argument has to be 'NULL'

    // zip
    execvp("zip", args_list);
    return 0;
}