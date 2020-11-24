#include"headers.h"
extern int error;
extern char prevcd[1024];
int max(int num1, int num2)
{
	return (num1 > num2) ? num1 : num2;
}
char *substring(char *destination, const char *source, int beg, int n)
{
	// extracts n characters from source string starting from beg index
	// and copy them into the destination string
	while (n > 0)
	{
		*destination = *(source + beg);

		destination++;
		source++;
		n--;
	}

	// null terminate destination string
	*destination = '\0';

	// return the destination string
	return destination;
}
// void gototilde(char *str)
// {
// 	char c[1024];
// 	getcwd(c, sizeof(c));
// 	c[strlen(c)] = '\0';
// 	// printf("%sbefore~\n",c);
// 	if (strcmp(str, c) == 0)
// 	{

// 	}
// 	else
// 	{
// 		char temp[strlen(str)+5];
// 		for (int i = 0; i < max(strlen(str), strlen(c)); i++)
// 		{
// 			if (str[i] != c[i])
// 			{
// 				int wcn=0;
// 				if(strlen(c)>strlen(str))
// 				{
// 					wcn=1;
// 				}
// 				for (int j=i;j<strlen(c);j++)
// 				{
// 					if(c[j]=='/')
// 					{

// 						wcn++;

// 					}
// 				}
// 				while(wcn--)
// 				{
// 					if(chdir("..")<0)
// 					{
// 						error=1;
// 						perror("chdir");
// 					}
// 				}
// 				getcwd(c, sizeof(c));
// 				c[strlen(c)] = '\0';
// 				// printf("%safr\n",c);
// 				substring(temp,str,strlen(c)+1,strlen(str)-strlen(c));
// 				// printf("%s\n",temp);
// 				chdir(temp);

// 				break;
// 			}
// 		}
// 	}
// }
void gotocd(char *toke, char *str)
{

	char c[1024];
	getcwd(c, sizeof(c));
	c[strlen(c)] = '\0';
	//printf("%s,%d\n",toke,strlen(toke));
	if(toke[0]=='~')
	{
		//char atoke[strlen(toke) + 2];
		// printf("ksks");
		// gototilde(str);
		// char atoke[strlen(toke) + 2];
		// if(strlen(toke)>2)
		// {
		// 	substring(atoke, toke, 2, strlen(toke) - 2);
		// 	if (chdir(atoke) < 0)
		// 	{
		// 		error=1;
		// 		perror("program");
		// 	}
		// }
		char arr[1024];
		arr[0]='\0';
		strcpy(arr,str);
		int l=strlen(arr);
		// for(int i=1;i<strlen(toke);i++)
		// {
		// 	arr[l+i-1]=toke[i];
		// }
		strcat(arr,toke+1);
		//printf("%s\n",arr);
		if (chdir(arr) < 0)
			{
				error=1;
				perror("program");
			}
	}
	else if(strcmp(toke,"-")==0)
	{
		if (chdir(prevcd) < 0)
		{
			error=1;
			perror("program");
		}
		//printf("%d\n",strlen(prevcd));
		char temp[1024];
		if (strcmp(str, prevcd) == 0)
		{
			printf("~\n");
		}
		else
		{
			for (int i = 0; i < max(strlen(str), strlen(prevcd)); i++)
			{
				if (str[i] != prevcd[i])
				{
					if (i < strlen(str))
					{
						substring(temp, prevcd, 0, strlen(prevcd));
						printf("%s\n",temp);
					}
					else
					{
						substring(temp, prevcd, i, strlen(prevcd) - i);
						printf("~%s\n", temp);
					}
					break;
				}
			}
		}
		char c1[1024];
		getcwd(c1, sizeof(c));
		c1[strlen(c1)] = '\0';
		if(error!=1)
		strcpy(prevcd, c1);
		
	}
	else
	{
		if (chdir(toke) < 0)
		{
			error=1;
			perror("program");
		}
	}
	if(error!=1)
	strcpy(prevcd, c);
}


void acd(char *comm,char *cwd_1)
{
		char delim[]="\t ";
		char cmd1[700];
		char *ptr = strtok(comm, delim);
		if (ptr == NULL)
		{
			gotocd("~", cwd_1);
		}
		else
		{
			strcpy(cmd1,ptr);
			ptr=strtok(NULL,delim);
			if(ptr!=NULL)
			{
				error=1;
				fprintf(stderr,"too many argumemts\n");
			}
			else
			{
				gotocd(cmd1, cwd_1);
			}
					
		}
		
		
	
}
/*void gotopwd()
{
	char c[1000];
	getcwd(c,sizeof(c));
	c[strlen(c)]='\0';
	printf("%s\n",c);
}*/
void apwd()
{
	char c[1000];
	getcwd(c, sizeof(c));
	c[strlen(c)] = '\0';
	printf("%s\n", c);
}
void gotoecho(char *comm)
{
	char delim[] = "\t ";
	char *ptr = strtok(comm, delim);
	while (ptr != NULL)
	{
		printf("%s ", ptr);
		ptr = strtok(NULL, delim);
	}
	printf("\n");
	
}