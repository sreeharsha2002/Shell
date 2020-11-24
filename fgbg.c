#include "headers.h"
#include "ls.h"
#include "fn.h"
#include "prompt.h"
#include "fgbg.h"
extern int sstdin, sstdout;
extern int error;
typedef struct node
{
    struct node *next;
    int jobno;
    int pid;
    char *command;
} node;
node *head = NULL;
char cw[1024];
void myhand(int signum)
{
    int status;
    //printf("sdvsdv");
    node *cur = head;
    node *pr = cur;
    while (cur != NULL)
    {

        if (waitpid(cur->pid, &status, WNOHANG) == cur->pid)
        {
            if (WIFEXITED(status))
            {
                fflush(stdout);
                fflush(stderr);
                fprintf(stderr, "\n%s with %d exited normally\n ", cur->command, cur->pid);
               // prompt(cw);
                if (cur == head)
                {
                    node *temp = head;
                    head = head->next;
                    cur = head;
                    free(temp);
                }
                else
                {
                    pr->next = cur->next;
                    node *temp = cur;
                    cur = cur->next;
                    free(temp);
                }
            }
            else if (WIFSIGNALED(status))
            {
                fflush(stdout);
                fflush(stderr);
                fprintf(stderr, "\n%s with %d exited abnormally\n ", cur->command, cur->pid);
              //  prompt(cw);
                if (cur == head)
                {
                    node *temp = head;
                    head = head->next;
                    cur = head;
                    free(temp);
                }
                else
                {
                    pr->next = cur->next;
                    node *temp = cur;
                    cur = cur->next;
                    free(temp);
                }
            }
        }
        else
        {
            pr = cur;
            cur = cur->next;
        }
    }
}

void gotobg(char *str)
{
    char *argv[1024];
    char delim[] = "\t ";
    char *token = strtok(str, delim);
    int i = 0;
    while (token != NULL)
    {
        argv[i] = (char *)malloc(sizeof(token) * sizeof(char));
        strcpy(argv[i], token);
        i++;
        token = strtok(NULL, delim);
    }
    argv[i]=NULL;
    int pid,status;
    if ((pid = fork()) < 0)
    {
        error =1;
        perror("program");
        return;
    }
    else if (pid == 0)
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
        setpgid(0, 0);
        //printf("%s\n", argv[0]);
        if (execvp(argv[0], argv) < 0)
        {

            perror("command");
            exit(-1);
        }
    }
    else
    {
        setpgid(pid, pid);
       
        // node *cur = (node *)malloc(sizeof(node));
        // cur->pid = pid;
        // cur->command = (char *)malloc(sizeof(1024));
        // strcpy(cur->command, argv[0]);
        // cur->next = head;
        // head = cur;
        int iii=1;
        // if(head==NULL)
        // {
        //     head = (node *)malloc(sizeof(node));
        //     head->pid = pid;
        //     head->command = (char *)malloc(sizeof(1024));
        //     strcpy(head->command, argv[0]);
        //     head->next=NULL;
        //     iii++;
        // }
        
        node *cur = (node *)malloc(sizeof(node));
        node * last = head;
        cur->pid = pid;
        cur->command = (char *)malloc(sizeof(1024));
        char actarg[1024]="\0";
        for(int kl=0;kl<i;kl++)
        {
            if(strlen(actarg)!=0)
            {
                strcat(actarg," ");
            }
            strcat(actarg,argv[kl]);
        }
        strcpy(cur->command, actarg);
        cur->next=NULL;
        if(head==NULL)
        {
            cur->jobno=1;
            head = cur;
        }
        else
        {      
            iii=2;  
            while(last->next!=NULL)
            {
                last=last->next;
                iii++;
            }
            cur->jobno=last->jobno+1;
            last->next=cur;
        }
        printf("\n[%d] %d\n",cur->jobno, pid);
        signal(SIGCHLD, myhand);
    }

}


void gotofg(char *str)
{
    char *argv[1024];
    char delim[] = "\t ";
    char *token = strtok(str, delim);
    int i = 0;
    while (token != NULL)
    {
        argv[i] = (char *)malloc(sizeof(token) * sizeof(char));
        strcpy(argv[i], token);
        i++;
        token = strtok(NULL, delim);
    }
    argv[i] = NULL;
    int pid;
    int status;

    if ((pid = fork()) < 0)
    {
        perror("program");
       return;
    }
    else if (pid == 0)
    {
        setpgid(0, 0);
        signal(SIGINT,SIG_DFL);
        signal(SIGTSTP,SIG_DFL);
        if (execvp(argv[0], argv) < 0)
        {
            perror("command");
           exit(-1);
        }
    }
    else
    {
        setpgid(pid, pid);
        signal(SIGTTIN, SIG_IGN);
        signal(SIGTTOU, SIG_IGN);
        tcsetpgrp(sstdin, pid);
        waitpid(pid, &status, WUNTRACED);
        if (!WIFEXITED(status) && WIFSIGNALED(status))
        {
            error=1;
        }
        else if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
        {
            error=1;
        }
        if(WIFSTOPPED(status))
        {
            error=1;
            int iii=1;
            node *cur = (node *)malloc(sizeof(node));
            node *last = head;
            cur->pid = pid;
            cur->command = (char *)malloc(sizeof(1024));
            char actarg[1024] = "\0";
            for (int kl = 0; kl < i; kl++)
            {
                if (strlen(actarg) != 0)
                {
                    strcat(actarg, " ");
                }
                strcat(actarg, argv[kl]);
            }
            strcpy(cur->command, actarg);
            cur->next = NULL;
            if (head == NULL)
            {
                cur->jobno = 1;
                head = cur;
            }
            else
            {
                iii = 2;
                while (last->next != NULL)
                {
                    last = last->next;
                    iii++;
                }
                cur->jobno = last->jobno + 1;
                last->next = cur;
            }
            printf("\n[%d] %d\n", cur->jobno, pid);
        }
        signal(SIGCHLD, myhand);
        tcsetpgrp(sstdin, getpgrp());
        signal(SIGTTIN, SIG_DFL);
        signal(SIGTTOU, SIG_DFL);
    }
}


void gotofgbg(char *str,int nofa,char*cwd_1)
{
    char delim[]="&";
    char amp[1024][1024];
    char *ptr=strtok(str,delim);
    int i=0;
    strcpy(cw,cwd_1);
    while(ptr!=NULL)
    {
        strcpy(amp[i],ptr);
        i++;
        ptr=strtok(NULL,delim);
    }
    for(int j=0;j<nofa;j++)
    {
        gotobg(amp[j]);
    }
    for(int j=nofa;j<i;j++)
    {
        gotofg(amp[j]); 
    }
}

void gotojobs(char * cmd)
{
    if(strlen(cmd)>0)
    {
        error=1;
        fprintf(stderr,"Invalid arguments for jobs");
    }
    else
    {
        node * last=head;
        while(last!=NULL)
        {
            char file[400];
            sprintf(file, "/proc/%d/stat", last->pid);
            FILE *fd = fopen(file, "r");
            if (fd == NULL)
            {
                error=1;
                perror("FIle open");
            }
            else
            {
                int pid;
                char exf[1024];
                char st;
                fscanf(fd, "%d %s %c ", &pid, exf, &st);
                fclose(fd);
                char str[100] = "\0";
                if (st == 'T')
                {
                    strcpy(str, "Stopped\0");
                }
                else
                {
                    strcpy(str, "Running\0");
                }
                printf("[%d] %s %s [%d]\n", last->jobno, str, last->command, last->pid);
            }
            last = last->next;
        }
    }
}
void gotokill(char *cmd)
{
    char delim[]="\t ";
    char *ptr=strtok(cmd,delim);
    char arr[20][20];
    int i=0;
    while(ptr!=NULL)
    {
        strcpy(arr[i],ptr);
        i++;
        ptr=strtok(NULL,delim);
    }
    if(i!=2)
    {
        error=1;
        fprintf(stderr,"Invalid arguments\n");
    }   
    else
    {
        int a,b;
        sscanf(arr[0],"%d",&a);
        sscanf(arr[1],"%d",&b);
        node *last=head;
        
        while(last!=NULL)
        {
            if (last->jobno == a)
            {
                break;
            }
            last=last->next;
            
        }
        // for(int i=0;i<a-1;i++)
        // {
        //     if(last!=NULL)
        //     {
        //         last=last->next;
        //     }
        //     else
        //     {
        //         break;
        //     }
            
        // }
        if(last==NULL)
        {
            error=1;
            fprintf(stderr,"Invalid job number\n");
        }
        
        
        else
        {
           if( kill(last->pid,b)<0)
           {
               error=1;
               perror("kill");
           }
        }
        

    }
    
}

void gotocomfg(char *cmd)
{
    char delim[] = "\t ";
    char *ptr = strtok(cmd, delim);
    char arr[20][20];
    int i = 0;
    int pp;
    while (ptr != NULL)
    {
        strcpy(arr[i], ptr);
        i++;
        ptr = strtok(NULL, delim);
    }
    if(i!=1)
    {
        error=1;
        fprintf(stderr,"Invalid arguments\n");
    }
    else
    {
        int a;
        sscanf(arr[0], "%d", &a);
       
        node *last = head;
        // node *pr=last;

        while (last != NULL)
        {
            if (last->jobno == a)
            {
                break;
            }
            // pr=last;
            last = last->next;
        }
        int status;
        if (last == NULL)
        {
            error=1;
            fprintf(stderr,"Invalid job number\n");
        }
        else
        {
            pp=last->pid;
            signal(SIGTTIN, SIG_IGN);
            signal(SIGTTOU, SIG_IGN);
            if (tcsetpgrp(sstdin, last->pid) == -1)
            {
                error=1;
                perror("tcsete0");
            }
            kill(last->pid,SIGCONT);
            waitpid(last->pid, &status, WUNTRACED);
            fflush(stdin);
            fflush(stdout);
            if (!WIFEXITED(status) && WIFSIGNALED(status))
            {
                error=1;
            }
            else if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
            {
                error=1;
            }
            // char comnd[1024]="\0";
            // strcpy(comnd,last->command);
            if (WIFSTOPPED(status))
            {
                // int iii = 1;
                // node *cur = (node *)malloc(sizeof(node));
                // node *la = head;
                // cur->pid = pp;
                // cur->command = (char *)malloc(sizeof(1024));

                // strcpy(cur->command, comnd);
                // cur->next = NULL;
                // if (head == NULL)
                // {
                //     cur->jobno = 1;
                //     head = cur;
                // }
                // else
                // {
                //     iii = 2;
                //     while (la->next != NULL)
                //     {
                //         la = la->next;
                //         iii++;
                //     }
                //     cur->jobno = la->jobno + 1;
                //     la->next = cur;
                // }
                error=1;
                printf("\n[%d] %d\n", last->jobno, pp);
            }
            else
            {
                node *temp = head;
                node *prev;

                if (temp != NULL && temp->pid == pp)
                {
                    head = temp->next;
                    free(temp);
                }
                else
                {
                    while (temp != NULL && temp->pid != pp)
                    {
                        prev = temp;
                        temp = temp->next;
                    }
                    if (temp != NULL)
                        prev->next = temp->next;
                    free(temp);
                }
            }
            
            
            
            
            signal(SIGCHLD, myhand);
            tcsetpgrp(sstdin, getpgrp());
            signal(SIGTTIN, SIG_DFL);
            signal(SIGTTOU, SIG_DFL);
        }
        // if (!WIFSTOPPED(status))
        // {
        //     node *temp = head;
        //     node *prev;

        //     if (temp != NULL && temp->pid == pp)
        //     {
        //         head = temp->next;
        //         free(temp);
        //         return;
        //     }
        //     while (temp != NULL && temp->pid != pp)
        //     {
        //         prev = temp;
        //         temp = temp->next;
        //     }
        //     if (temp == NULL)
        //         return;
        //     prev->next = temp->next;

        //     free(temp);
        // }
    }
    


}
void gotocombg(char *cmd)
{
    char delim[] = "\t ";
    char *ptr = strtok(cmd, delim);
    char arr[20][20];
    int i = 0;
    while (ptr != NULL)
    {
        strcpy(arr[i], ptr);
        i++;
        ptr = strtok(NULL, delim);
    }
    if (i != 1)
    {
        error=1;
        fprintf(stderr,"Invalid arguments");
    }
    else
    {
        int a;
        sscanf(arr[0], "%d", &a);
        node *last = head;

        while (last != NULL)
        {
            if (last->jobno == a)
            {
                
                break;
            }
           // printf("%d,%d\n", last->jobno, a);
            last = last->next;
        }
        int status;
        if (last == NULL)
        {
            error=1;
            fprintf(stderr,"Invalid job number\n");
        }
        else
        {
            if(kill(last->pid, SIGCONT)<0)
            {
                error=1;
                perror("KIll");
            }
            
        }
    }
}
void gotooverkill()
{
    node *last=head;
    int arr[1000];
    int cn=0;
    while(last!=NULL)
    {
        arr[cn]=last->pid;
        cn++;
        last=last->next;
    }
    for(int i=0;i<cn;i++)
    {
        if(kill(arr[i],SIGKILL)<0)
        {
            error=1;
            perror("kill");
        }
    }
}