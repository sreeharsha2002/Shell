#include "headers.h"
#include "ls.h"
#include "fn.h"
extern int error;
char *perm(struct stat stats)
{
	char *str = (char *)malloc(100);
	strcpy(str, "----------\0");
	if (stats.st_mode & S_IFDIR)
	{
		str[0] = 'd';
	}
	if (stats.st_mode & S_IRUSR)
	{
		str[1] = 'r';
	}
	if (stats.st_mode & S_IWUSR)
	{
		str[2] = 'w';
	}
	if (stats.st_mode & S_IXUSR)
	{
		str[3] = 'x';
	}
	if (stats.st_mode & S_IRGRP)
	{
		str[4] = 'r';
	}
	if (stats.st_mode & S_IWGRP)
	{
		str[5] = 'w';
	}
	if (stats.st_mode & S_IXGRP)
	{
		str[6] = 'x';
	}
	if (stats.st_mode & S_IROTH)
	{
		str[7] = 'r';
	}
	if (stats.st_mode & S_IWOTH)
	{
		str[8] = 'w';
	}
	if (stats.st_mode & S_IXOTH)
	{
		str[9] = 'x';
	}
	//printf("%s ",str);
	return str;
}

void gotols(char *ast,int astl,int flag,char *cwd_1)
{
	struct dirent **filename;
	int n;
	char incase[1024];
	if(ast[0]=='~')
	{
		if(strlen(ast)>1)
		{
		char dp[1024];
		substring(dp,ast,1,strlen(ast)-1);
		sprintf(incase,"%s%s",cwd_1,dp);
		}
		else
		{
			strcpy(incase,cwd_1);
		}
		
	}
	else
	{
		strcpy(incase,ast);
	}
	//printf("%p\n",incase);
	n = scandir(incase, &filename, NULL, alphasort);
	if (n < 0)
	{
		error=1;
		perror("scandir Program");
	}
	else
	{
		if(astl>0)
		{
			printf("\n%s:\n",ast);
		}
		struct stat mystat;
		int pri=0;
		for (int i = 0; i < n; i++)
		{
			char myfilena[1324];
			int ff=0;
			sprintf(myfilena, "%s/%s", incase, filename[i]->d_name);
			if(filename[i]->d_name[0]=='.')
			{
				ff=1;
			}
			if((ff==0 && (flag==1||flag==0)) || flag==2 || flag==3)
			{
				if(flag==1||flag==3)
				{
					stat(myfilena, &mystat);
					printf("%s ", perm(mystat));
					printf("%3ld ", mystat.st_nlink);
					struct passwd *pw = getpwuid(mystat.st_uid);
					struct group *gr = getgrgid(mystat.st_gid);
					printf("%s ", pw->pw_name);
					printf("%s ", gr->gr_name);
					printf("%13ld ", mystat.st_size);
					time_t t = mystat.st_ctime;
					struct tm lt;
					localtime_r(&t, &lt);
					char timbuf[80];
					strftime(timbuf, sizeof(timbuf), " %b %d %H:%M ", &lt);
					printf("%s ", timbuf);
				}
				printf("%s\n", filename[i]->d_name);
				pri=1;
			}
			free(filename[i]);
		}
		if(pri==0)
		{
			printf("\n");
		}
		free(filename);
	}


}
void als(char *comm,char *cwd_1)
{
		char delim[]="\t ";
		int flag = 0, i = 0;
		char ast[1024][1024];
		char *ptr = strtok(comm, delim);
		while (ptr != NULL)
		{
			//printf("%s\n",ptr);
			/*if(strcmp(ptr,"-l")==0)
				{
					flag=flag|1;
				}
				else if(strcmp(ptr,"-a")==0)
				{
					flag=flag|2;
				}
				else if(strcmp(ptr,"-la")==0 || strcmp(ptr,"-al")==0)
				{
					flag=flag|3;
				}*/
			if (ptr[0] == '-')
			{
				for (int jj = 1; jj < strlen(ptr); jj++)
				{
					if (ptr[jj] == 'l')
					{
						flag = flag | 1;
					}
					else if (ptr[jj] == 'a')
					{
						flag = flag | 2;
					}
					else
					{
						flag = 20;
					}
				}
			}
			else
			{
				strcpy(ast[i], ptr);
				i++;
			}

			ptr = strtok(NULL, delim);
		}
		if (flag < 4)
		{
			if (i == 0)
			{
				gotols(".", i, flag, cwd_1);
			}
			else
			{

				for (int j = 0; j < i; j++)
				{

					gotols(ast[j], i, flag, cwd_1);
				}
			}
		}
		else
		{
			error=1;
			fprintf(stderr,"Invalid flags\n");
		}
	
}
