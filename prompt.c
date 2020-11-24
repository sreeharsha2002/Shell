#include"prompt.h"
#include"headers.h"
#include"fn.h"
int Np=1024;
void prompt(char *str)
{
	char hostname[Np],c[Np],temp[Np];
	char * user=getenv("USER");
	int he=gethostname(hostname,sizeof(hostname));
	if(he<0)
	{
		perror("program");
	}
	getcwd(c,sizeof(c));
	c[strlen(c)]='\0';
	if(strcmp(str,c)==0)
	{
		printf("%s@%s:~> ", user, hostname);
	}
	else
	{
		for(int i=0;i<max(strlen(str),strlen(c));i++)
		{
			if(str[i]!=c[i])
			{
				if(i<strlen(str))
				{
					substring(temp,c,0,strlen(c));
					printf("%s@%s:%s> ", user, hostname, temp);
				}
				else
				{
					substring(temp,c,i,strlen(c)-i);
					printf("%s@%s:~%s> ", user, hostname, temp);
				}
				break;
			}

		}

	}
}
