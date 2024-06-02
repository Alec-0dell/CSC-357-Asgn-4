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
        char *cnum = argv[2];
        while (*cnum != 0)
        {
            if (*cnum > 47 && *cnum < 58)
            {
                max = (max * 10) + (*cnum - 48);
                cnum++;
            }
            else
            {
                fprintf(stderr, "Usage: %s <download-file> <max-download-processes>\n", argv[0]);
                return EXIT_FAILURE;
            }
        }
    }

    char cline[512];
    char *oname = fgets(cline, sizeof(cline), file);
    char *url = oname;
    int timeout;

    while (*url != 10 && *url != 32)
    {
        ++url;
    }
    *url++ = 0;
    char *cchar = url;
    while (*cchar != 10 && *cchar != 32)
    {
        ++cchar;
    }
    *cchar++ = 0;
    while (*cchar != 10 && *cchar != 32)
    {
        timeout = (timeout * 10) + (*cchar - 48);
        ++cchar;
    }
    *cchar++ = 0;

    printf("Name: %s , URL: %s , Timeout: %d , Max: %d\n", oname, url, timeout, max);

    return (0);
}
