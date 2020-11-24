#include "headers.h"
#include "comm.h"
extern int error;
void piping(char *com,char *cwd_1)
{
    error=0;
    char delim[]="|";
    char *ptr=strtok(com,delim);
    char app[1024][1024];
    int cnt=0;
    while(ptr!=NULL)
    {
        strcpy(app[cnt],ptr);
        cnt++;
        ptr=strtok(NULL,delim);
    }
   
    int fd[2],in=0;
    for(int i=0;i<cnt;i++)
    {
        error=0;
        int save_stdin = dup(0);
        int save_stdout = dup(1);
        if(i==cnt-1)
        {
            if(dup2(in,0)<0)
            {
                perror("dup2 in");
                dup2(save_stdin,0);
                dup2(save_stdout,1);
                close(save_stdin);
                close(save_stdout);
                close(in);
                error=1;
                return;
            }
            comm(app[i],cwd_1,in,1);
            dup2(save_stdin,0);
            dup2(save_stdout,1);
            close(save_stdin);
            close(save_stdout);
        }
        else
        {
            pipe(fd);
            if (dup2(in, 0) < 0)
            {
                perror("dup2 in");
                dup2(save_stdin, 0);
                dup2(save_stdout, 1);
                close(save_stdin);
                close(save_stdout);
                close(in);
                error=1;
                return;
            }
            if(dup2(fd[1],1)<0)
            {
                perror("dup2 out");
                dup2(save_stdin, 0);
                dup2(save_stdout, 1);
                close(save_stdin);
                close(save_stdout);
                close(fd[1]);
                error=1;
                return;
            }
            comm(app[i],cwd_1,in,fd[1]);
            
            close(fd[1]);
            in=fd[0];
            dup2(save_stdin, 0);
            dup2(save_stdout, 1);
            close(save_stdin);
            close(save_stdout);
        }
        
    }
}