#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("usage: samples file numberfrags maxfragsize\n");
        return EXIT_FAILURE;
    }
    else
    {
        FILE *file;
        file = fopen(argv[1], "r");

        if (file != NULL)
        {

            // seed a random number
            srand(time(NULL));

            fseek(file, 0, SEEK_END);
            long size = ftell(file);
            rewind(file);

            if (atoi(argv[3]) >= size)
            {
                printf("Error: size of fragment is bigger than the file size\n");
                return EXIT_FAILURE;
            }

            size -= atoi(argv[3]);

            for (int i = 0; i < atoi(argv[2]); i++)
            {
                fseek(file, rand() % size, SEEK_SET);

                printf(">");
                int j = 0;
                while (j < atoi(argv[3]))
                {
                    char ch = getc(file);

                    printf("%c", ch);
                    j++;
                }
                printf("<\n");
            }

            fclose(file);
        }
        else
        {
            perror("Error");
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}