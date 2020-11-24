#include "prompt.h"
#include "headers.h"
#include "fn.h"
#include "comm.h"
#include "history.h"
#include "piping.h"
#include "fgbg.h"
#include "bonus.h"
int N=2024;
int sstdin,sstdout;
char prevcd[1024];
int error;
int main()
{
	signal(SIGINT,SIG_IGN);
	signal(SIGTSTP,SIG_IGN);
	char cwd_1[N];
	getcwd(cwd_1,sizeof(cwd_1));
	strcpy(prevcd,cwd_1);
	sstdin=dup(0);
	sstdout=dup(1);
	while (1)
	{

		prompt(cwd_1);
		char sp=' ';
		int i=0;
		char inp[N],dinp[N];
		while(sp!='\n')
		{
			int llk=scanf("%c",&sp);
			if(i==0)
			{
				if(llk==EOF)
				{
					printf("^D\n");
					close(sstdin);
					close(sstdout);
					gotooverkill();
					exit(0);
				}
				if(sp!=' ' && sp!='\n' && sp!='\t')
				{
					inp[i]=sp;
					i++;
				}
			}
			else if(sp!='\n' && llk!=EOF)
			{
				inp[i]=sp;
				i++;
			}

		}
		inp[i]='\0';
		//printf("%s,%d\n",inp,i);
		//printf("%c\n",inp[9]);
		error=0;
		if(strlen(inp)>0)
		{
			gotohistory(inp,cwd_1);
			strcpy(dinp,inp);
			char delim[] = ";";
			char arr[N][N];
			i=0;
			char *ptr = strtok(dinp, delim);
			//printf("IN mainA %sl\n", ptr);
			while (ptr != NULL)
			{
				strcpy(arr[i],ptr);
				i++;
				ptr = strtok(NULL, delim);	
			}
			for(int j=0;j<i;j++)
			{
				gotobonus(arr[j],cwd_1);
			}
		}
		if (error == 0)
		{
			printf(":')");
		}
		else if (error = 1)
		{
			printf(":'(");
		}
	}
}
