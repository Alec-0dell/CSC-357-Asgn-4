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

    char cline[200];

    int nlines = 0;
    char c;
    while (!feof(file))
    {
        c = fgetc(file);
        if (c == '\n')
        {
            nlines++;
        }
    }

    printf("%d\n", nlines);
    fclose(file);
    file = fopen(argv[1], "r");
    int pids[nlines];
    int pid;

    for (int i = 0; i < nlines; i++)
    {
        if ((fgets(cline, sizeof(cline), file)) == NULL)
        {
            break;
        }
        //printf("%d + %s", getpid(), cline);

        char *url = cline;
        int timeout = 0;
        int len = 0;

        while (*url != 10 && *url != 32)
        {
            ++url;
            len++;
        }
        *url++ = 0;
        char *cchar = url;
        while (*cchar != 10 && *cchar != 32)
        {
            ++cchar;
            len++;
        }
        if (*cchar == 32)
        {
            *cchar++ = 0;
            while (*cchar != 10 && *cchar != 32)
            {
                timeout = (timeout * 10) + (*cchar - 48);
                ++cchar;
                len++;
            }
        }
        *cchar++ = 0;

        char command[len + 17];
        if (timeout == 0)
        {
            sprintf(command, "curl -o %s -s %s", cline, url);
        }
        else
        {
            sprintf(command, "curl -m %d -o %s -s %s", timeout, cline, url);
        }

        if (max > 0)
        {
            if ((pid = fork()) == 0)
            {
                
                // system(command);
                sleep(5);
                //printf("%s + %d\n", command, getpid());
                fclose(file);
                return EXIT_SUCCESS;
            } else {
                pids[i] = pid;
                printf("process %d processing line %d\n", pid, i+1);
            }
        }
    }

    for (int i = 0; i < nlines; i++)
    {
        if(waitpid(pids[i],NULL, WCONTINUED) == -1){
            printf("process %d processing line %d terminated with exit status: %d \n", pids[i], i + 1, errno);
        } else {
            printf("process %d processing line %d exited normally \n", pids[i], i + 1);
        }
    }
    

    fclose(file);

    return (0);
}
