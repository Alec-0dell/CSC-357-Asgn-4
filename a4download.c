#include "a4download.h"

int main(int argc, char *argv[])
{
    FILE *file = stdin;
    int max = 0;
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <download-file> <max-download-processes>\n", argv[0]);
        return EXIT_FAILURE;
    }
    else
    {
        file = fopen(argv[1], "r");
        if (!file)
        {
            printf("Error opening file '%s'\n", argv[1]);
            return EXIT_FAILURE;
        }
        char *cur_num = argv[2];
        while (*cur_num != 0)
        {
            if (*cur_num > 47 && *cur_num < 58)
            {
                max = (max * 10) + (*cur_num - 48);
                cur_num++;
            }
            else
            {
                fprintf(stderr, "Usage: %s <download-file> <max-download-processes>\n", argv[0]);
                return EXIT_FAILURE;
            }
        }
    }

    printf("Max: %d \n", max);

    return (0);
}
