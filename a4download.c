#include "a4download.h"

typedef struct download
{
    char *filename;
    char *url;
    char *timelimit;
} download;

char* str_dupe(const char* src) {
    if (src == NULL) {
        return NULL;
    }
    char* dest = malloc(strlen(src) + 1);
    strcpy(dest, src);
    return dest;
}

int main(int argc, char *argv[])
{
    FILE *file = stdin;
    int max = 0;   // max number of processes
    if (argc != 3) // check for correct passed arguments
    {
        fprintf(stderr, "Usage: %s <download-file> <max-download-processes>\n", argv[0]);
        return EXIT_FAILURE;
    }
    else
    {
        file = fopen(argv[1], "r");
        if (!file) // opens file
        {
            printf("Error opening file '%s'\n", argv[1]);
            return EXIT_FAILURE;
        }
        char *cnum = argv[2];
        while (*cnum != 0) // converts the string for max into an integer
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

        if (max <= 0) // ensures there aren't any issues with nex lines of code
        {
            fprintf(stderr, "Max number of processes must be at least 1\n");
            return EXIT_FAILURE;
        }
    }

    char cline[200]; // arbitrary max length of a line

    download *files = (download *)malloc(sizeof(download)); // initialise files
    int nlines = 0; // number of lines in the file -> number of files to download

    while ((fgets(cline, sizeof(cline), file)) != NULL) // loop through
    {
        char *url = cline;
        while (*url != '\n' && *url != ' ' && *url != '\0')
        {
            ++url;
        }
        *url++ = 0;
        char *timeout = url;
        while (*timeout != '\n' && *timeout != ' ' && *timeout != '\0')
        {
            ++timeout;
        }
        *timeout++ = 0;
        char *cchar = timeout;
        if (*timeout > '0' && *timeout < '9')
        {
            while (*cchar != '\n' && *cchar != ' ' && *cchar != '\0')
            {
                cchar++;
            }
            *cchar = 0;
        }
        else
        {
            timeout = NULL;
        }
        files = (download *)realloc(files, (sizeof(download) * (nlines + 1)));
        files[nlines].filename = str_dupe(cline);
        files[nlines].url = str_dupe(url);
        files[nlines++].timelimit = str_dupe(timeout);
    }

    fclose(file); //file no longer needed 
    

    int pids[nlines]; //tracks the pid of each created process
    int pid; //pid of current fork
    int completep = 0; //number of completed processes

    for (int i = 0; i < nlines; i++)
    {
        if (max <= 0)
        {
            //if the max processes has been reach the parent process will wait before creating a new one
            int status;
            waitpid(pids[completep], &status, 0);
            if (WEXITSTATUS(status) == 0)
            {
                printf("process %d processing line %d exited normally\n", pids[completep], completep + 1);
            }
            else
            {
                printf("process %d processing line %d terminated with exit status: %d\n", pids[completep], completep + 1, WEXITSTATUS(status));
            }
            completep++;
        }
        if ((pid = fork()) == 0)
        {
            //child component calls curl to download the next file
            if (files[i].timelimit == NULL)
            {
                execlp("curl", "curl", "-o", files[i].filename, "-s", files[i].url, NULL);
            }
            else
            {
                execlp("curl", "curl", "-m", files[i].timelimit, "-o", files[i].filename, "-s", files[i].url, NULL);
            }
            fprintf(stderr, "process %d processing line %d terminated with error: %s\n", getpid(), i + 1, strerror(errno));
            fclose(file);
            return EXIT_SUCCESS;
        }
        else
        {
            //reports processing within the parent process
            pids[i] = pid;
            printf("process %d processing line %d\n", pid, i + 1);
            max--;
        }
    }

    // completes all downloads and returns their statuses
    for (int i = completep; i < nlines; i++)
    {
        int status;
        waitpid(pids[i], &status, 0);
        if (WEXITSTATUS(status) == 0)
        {
            printf("process %d processing line %d exited normally\n", pids[i], i + 1);
        }
        else
        {
            printf("process %d processing line %d terminated with exit status: %d\n", pids[i], i + 1, WEXITSTATUS(status));
        }
    }

    // free all of the data
    for (int i = 0; i < nlines; i++)
    {
        free(files[i].filename);
        free(files[i].url);
        free(files[i].timelimit);
    }
    free(files);

    return (0);
}
