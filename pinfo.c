#include "headers.h"
#include "pinfo.h"
extern int error;
void gotopinfo(char *comm)
{
    char delim[]="\t ";
        int lc = 0;
       char * ptr = strtok(comm, delim);
        lc++;

        while (ptr != NULL || lc == 1)
        {
            char spp[10];
            if (ptr == NULL)
            {
                int pp = getpid();
                sprintf(spp, "%d", pp);
            }
            else
            {
                sprintf(spp, "%s", ptr);
            }
            char file[1024];
            sprintf(file, "/proc/%s/stat", spp);

            FILE *fd = fopen(file, "r");
            if (fd==NULL)
            {
                error=1;
                perror("FIle open");
            }
            int pid;
            char exf[1024];
            char st;
            fscanf(fd, "%d %s %c ", &pid, exf, &st);
            fclose(fd);

            sprintf(file, "/proc/%s/statm", spp);
            fd = fopen(file, "r");
            if (fd==NULL)
            {
                error=1;
                perror("file open");
            }
            long long int vmsize;
            fscanf(fd, "%lld ", &vmsize);
            fclose(fd);

            char buff[1024];
            sprintf(file, "/proc/%s/exe", spp);
            if (readlink(file, buff, sizeof(buff)) < 0)
            {
                error=1;
                perror("reading link");
            }
            printf("\npid -- %d\n", pid);
            printf("Process Status -- %c\n", st);
            printf("memory -- %lld\n", vmsize);
            printf("Executable Path -- %s\n", buff);
            ptr = strtok(NULL, delim);
            lc++;
        }
    
}
void gotosetenv(char *cmd)
{
    char delim []="\t ";
    char * ptr = strtok(cmd,delim);
    char arr[20][400];
    int i=0;
    while(ptr!=NULL)
    {
        strcpy(arr[i],ptr);
        i++;
        ptr=strtok(NULL,delim);
    }
    if( i==0 || i>2)
    {
        error=1;
            fprintf(stderr,"INVALID inputs for setenv command\n");
    }
    else if(i==1)
    {
        arr[1][0]='\0';
        if(setenv(arr[0],arr[1],1)<0)
        {
            error=1;
            perror("setenv");
        }
    }
    else
    {
        if (setenv(arr[0], arr[1], 1) < 0)
        {
            error=1;
            perror("setenv");
        }
    }
}

void gotounsetenv(char *cmd)
{
    char delim[] = "\t ";
    char *ptr = strtok(cmd, delim);
    char arr[20][400];
    int i = 0;
    while (ptr != NULL)
    {
        strcpy(arr[i], ptr);
        i++;
        ptr = strtok(NULL, delim);
    }
    if (i == 0 || i > 1)
    {
        error=1;
        fprintf(stderr,"INVALID inputs for unsetenv command\n");
    }
    else
    {
        if(unsetenv(arr[0])<0)
        {
            error=1;
            perror("unsetenv");
        }
    }
    
}