#include "headers.h"
#include "comm.h"
#include "ls.h"
#include "fn.h"
#include "fgbg.h"
#include "nigtw.h"
#include "pinfo.h"
#include  "history.h"
extern int error;
extern int sstdin, sstdout;
void fncalls(char *cmd1,char *cmd,char * in ,char * out,int ff,char *cwd_1)
{
	//fprintf(stderr,"%s,%s,%s,%s,%d,%s\n", cmd1, cmd, in, out,ff,cwd_1);
	int size = 	(sizeof(cmd1)+sizeof(cmd))*4;
	char *str=malloc(size);
	sprintf(str,"%s %s",cmd1,cmd);	
	
	int save_stdin = dup(0);
	int save_stdout = dup(1);
	int fdin,fdout;
	if(strlen(in)==0)
	{
		fdin=dup(0);
	}
	else
	{
		if((fdin = open(in, O_RDONLY))<0)
		{
			fdin=dup(0);
			perror("Input file");
			dup2(save_stdin, 0);
			dup2(save_stdout, 1);
			close(save_stdin);
			close(save_stdout);
			close(fdin);
			error=1;
			return;
		}
	}

	if(dup2(fdin,0)<0)
	{
		perror("dupIn");
		dup2(save_stdin, 0);
		dup2(save_stdout, 1);
		close(save_stdin);
		close(save_stdout);
		close(fdin);
		error=1;
		return;
	}
	if (strlen(out) == 0)
	{
		fdout = dup(1);
	}
	else
	{
		if(ff==0)
		{
			if ((fdout = open(out, O_WRONLY | O_CREAT | O_TRUNC ,0644)) < 0)
			{
				fdout = 1;
				perror("OUTput file");
				dup2(save_stdin, 0);
				dup2(save_stdout, 1);
				close(save_stdin);
				close(save_stdout);
				close(fdin);
				close(fdout);
				error=1;
				return;
			}
		}
		else
		{
			if ((fdout = open(out, O_WRONLY | O_CREAT | O_APPEND,0644)) < 0)
			{
				fdout = 1;
				perror("OUTPUT file");
				dup2(save_stdin, 0);
				dup2(save_stdout, 1);
				close(save_stdin);
				close(save_stdout);
				close(fdin);
				close(fdout);
				error=1;
				return;
			}
		}
		
	}
	if (dup2(fdout, 1) < 0)
	{
		perror("dupout");
		dup2(save_stdin, 0);
		dup2(save_stdout, 1);
		close(save_stdin);
		close(save_stdout);
		close(fdin);
		close(fdout);
		error=1;
		return;
	}
	//fprintf(stderr,"%d,%d\n",fdin,fdout);
	
	//printf("%s\n", str);
	if (strcmp(cmd1, "cd") == 0)
	{
		acd(cmd, cwd_1);
	}
	else if (strcmp(cmd1, "pwd") == 0)
	{
		apwd();
	}
	else if (strcmp(cmd1, "echo") == 0)
	{
		gotoecho(cmd);
	}
	else if (strcmp(cmd1, "ls") == 0)
	{
		als(cmd, cwd_1);
	}
	else if (strcmp(cmd1, "pinfo") == 0)
	{
		gotopinfo(cmd);
	}
	else if (strcmp(cmd1, "history") == 0)
	{
		retrivehistory(cmd, cwd_1);
	}
	else if (strcmp(cmd1, "nightswatch") == 0)
	{
		anight(cmd);
	}
	else if(strcmp(cmd1,"setenv")==0)
	{
		gotosetenv(cmd);
	}
	else if(strcmp(cmd1,"unsetenv")==0)
	{
		gotounsetenv(cmd);
	}
	else if(strcmp(cmd1,"jobs")==0)
	{
		gotojobs(cmd);
	}
	else if(strcmp(cmd1,"kjob")==0)
	{
		gotokill(cmd);
	}
	else if(strcmp(cmd1,"fg")==0)
	{
		gotocomfg(cmd);
	}
	else if(strcmp(cmd1,"bg")==0)
	{
		gotocombg(cmd);
	}
	else if(strcmp(cmd1,"overkill")==0)
	{
		gotooverkill();
	}
	else if(strcmp(cmd1,"quit")==0)
	{
		gotooverkill();
		close(sstdin);
		close(sstdout);
		exit(0);
	}
	else
	{
		int nofa = 0;
		for (int ij = 0; ij < strlen(str); ij++)
		{
			if (str[ij] == '&')
			{

				nofa++;
			}
		}
		gotofgbg(str, nofa, cwd_1);
	}
	
	dup2(save_stdin,0);
	dup2(save_stdout,1);
	close(save_stdin);
	close(save_stdout);
	close(fdin);
	close(fdout);
}




void comm(char * arg,char *cwd_1,int input,int  output)
{
	//fprintf(stderr,"%s,%s,%d,%d\n",arg,cwd_1,input,output);
	// int save_stdin=dup(0);
	// int save_stdout=dup(1);
	// if(dup2(input,0)<0)
	// {
	// 	perror("dup2In");
	// 	dup2(save_stdin,0);
	// 	return;
	// }
	// if(dup2(output,1)<0)
	// {
	// 	perror("dup2OUT");
	// 	dup2(save_stdout,1);
	// 	return;
	// }
	// fprintf(stderr, "%d,%d\n", input, output);
	// close(input);
	// close(output);
	char delim[] = "\t ";
	char *cpargv=(char*)malloc(1024);
	strcpy(cpargv,arg);
	char cmd[1024], in[1024], out[1024], cmd1[40];
	// char *cmd=(char *)malloc(1024*sizeof(char));
	// char *in = (char *)malloc(1024 * sizeof(char));
	// char *out = (char *)malloc(1024 * sizeof(char));
	// char *cmd1 = (char *)malloc(1024 * sizeof(char));
	cmd[0]='\0';
	cmd1[0]='\0';
	in[0]='\0';
	out[0]='\0';
	char *ptr = strtok(arg, delim);
	int flag=0,ff=0;
	while(ptr!=NULL)
	{
		
		if(strcmp(ptr,"<")==0)
		{
			flag=1;
		}
		else if(strcmp(ptr,">")==0)
		{
			flag=2;
		}
		else if(strcmp(ptr,">>")==0)
		{
			flag=2;
			ff=1;
		}	
		else
		{
			if (flag == 1)
			{
				strcpy(in, ptr);
				flag = 0;
			}
			else if (flag == 2)
			{
				strcpy(out, ptr);
				flag = 0;
			}
			else if (flag == 0 )
			{
				if (strlen(cmd1) == 0)
				{
					strcpy(cmd1, ptr);
				}
				else
				{
					if (strlen(cmd) != 0)
					{
						strcat(cmd, " ");
					}
					strcat(cmd, ptr);
				}
			}
		}
		ptr =strtok(NULL,delim);
	}	
//	printf("%s\n",cpargv);
	
	fncalls(cmd1,cmd,in,out,ff,cwd_1);


	// dup2(save_stdin,0);
	// dup2(save_stdout,1);
	
	// if(strcmp(cmd1,"cd")==0)
	// {
	// 	acd(cmd,cwd_1);
	// }
	// else if(strcmp(cmd1,"pwd")==0)
	// {
	// 	apwd();
	// }
	// else if(strcmp(cmd1,"echo")==0)
	// {
	// 	gotoecho(cmd);
	// }
	// else if(strcmp(cmd1,"ls")==0)
	// {
	// 	als(cmd,cwd_1);
	// }
	// else if(strcmp(cmd1,"pinfo")==0)
	// {
	// 	gotopinfo(cmd);
	// }
	// else if(strcmp(cmd1,"history")==0)
	// {
	// 	retrivehistory(cmd,cwd_1);
	// }
	// else if(strcmp(cmd1,"nightswatch")==0)
	// {
	// 	anight(cmd);
	// }
	// else
	// {
	// 	int nofa=0;
	// 	for (int ij = 0; ij < strlen(cpargv); ij++)
	// 	{
	// 		if (cpargv[ij] == '&')
	// 		{

	// 			nofa++;
	// 		}
	// 	}
	// 	gotofgbg(cpargv, nofa, cwd_1);
	// }
	


	
















// 	while (ptr != NULL)
// 	{
// 		if(strcmp(ptr,"cd")==0)
// 		{
// 			ptr = strtok(NULL, delim);
// 			if(ptr==NULL)
// 			{
// 				gotocd("~",cwd_1);
// 			}
// 			else
// 			{
// 				gotocd(ptr,cwd_1);
// 			}
// 		}
// 		else if(strcmp(ptr,"pwd")==0)
// 		{
// 			gotopwd();
// 		}
// 		else if(strcmp(ptr,"echo")==0)
// 		{
// 			ptr = strtok(NULL, delim);
// 			while(ptr!=NULL)
// 			{
// 				printf("%s ", ptr);
// 				ptr = strtok(NULL, delim);
// 			}
// 			printf("\n");
// 		}
// 		else if(strcmp(ptr,"ls")==0)
// 		{
// 			int flag=0,i=0;
// 			char ast[1024][1024];
// 			ptr = strtok(NULL, delim);
// 			while (ptr != NULL)
// 			{
// 				//printf("%s\n",ptr);
// 				/*if(strcmp(ptr,"-l")==0)
// 				{
// 					flag=flag|1;
// 				}
// 				else if(strcmp(ptr,"-a")==0)
// 				{
// 					flag=flag|2;
// 				}
// 				else if(strcmp(ptr,"-la")==0 || strcmp(ptr,"-al")==0)
// 				{
// 					flag=flag|3;
// 				}*/
// 				if(ptr[0]=='-')
// 				{
// 					for(int jj=1;jj<strlen(ptr);jj++)
// 					{
// 						if(ptr[jj]=='l')
// 						{
// 							flag=flag|1;
// 						}
// 						else if(ptr[jj]=='a')
// 						{
// 							flag=flag|2;
// 						}
// 						else
// 						{
// 							flag=20;
// 						}
						
// 					}
// 				}
// 				else
// 				{
// 					strcpy(ast[i],ptr);
// 					i++;
// 				}

// 				ptr = strtok(NULL, delim);
// 			}
// 			if(flag<4)
// 			{
// 				if(i==0)
// 				{
// 					gotols(".",i,flag,cwd_1);
// 				}
// 				else
// 				{

// 					for(int j=0;j<i;j++)
// 					{

// 						gotols(ast[j],i,flag,cwd_1);

// 					}
// 				}
// 			}
// 			else
// 			{
// 				printf("Invalid flags\n");
// 			}
			
// 		}
// 		else if(strcmp(ptr,"pinfo")==0)
// 		{
// 			int lc=0;
// 			ptr=strtok(NULL,delim);
// 			lc++;
			
// 			while(ptr!=NULL || lc==1)
// 			{
// 				char spp[10];
// 				if(ptr==NULL)
// 				{
// 					int pp=getpid();
// 					sprintf(spp,"%d",pp);
// 				}
// 				else
// 				{
// 					sprintf(spp,"%s",ptr);
// 				}
// 				char file[1024];
// 				sprintf(file,"/proc/%s/stat",spp);
				
// 				FILE *fd=fopen(file,"r");
// 				if(fd<0)
// 				{
// 					perror("FIle open");
// 				}
// 				int pid;char exf[1024];char st;
// 				fscanf(fd,"%d %s %c ",&pid,exf,&st);
// 				fclose(fd);

// 				sprintf(file, "/proc/%s/statm", spp);
// 				fd=fopen(file,"r");
// 				if(fd<0)
// 				{
// 					perror("file open");
// 				}
// 				long long int vmsize;
// 				fscanf(fd,"%lld ",&vmsize);
// 				fclose(fd);

// 				char buff[1024];
// 				sprintf(file, "/proc/%s/exe", spp);
// 				if(readlink(file,buff,sizeof(buff))<0)
// 				{
// 					perror("reading link");
// 				}
// 				printf("\npid -- %d\n",pid);
// 				printf("Process Status -- %c\n",st);
// 				printf("memory -- %lld\n",vmsize);
// 				printf("Executable Path -- %s\n",buff);
// 				ptr = strtok(NULL, delim);
// 				lc++;
// 			}
			
			

// 		}
// 		else if(strcmp(ptr,"history")==0)
// 		{
// 			ptr=strtok(NULL,delim);
// 			int num,flag=0;
// 			if(ptr==NULL)
// 			{
// 				num = 10;
// 				flag=1;
// 			}
// 			else
// 			{
// 				sscanf(ptr,"%d",&num);
// 			}
// 			FILE *fd;
// 			char actp[1024];
// 			sprintf(actp,"%s/history.txt",cwd_1);
// 			fd=fopen(actp,"r");
// 			char array[20][1024];
// 			char c;
// 			int count = 0;
// 			for (c = getc(fd); c != EOF; c = getc(fd))
// 				if (c == '\n') // Increment count if this character is newline
// 					count = count + 1;
			
			
// 			if(count<num)
// 			{
// 				num=0;
// 			}
// 			else
// 			{
// 				num=count-num;
// 			}	
// 			fclose(fd);
// 			fd = fopen(actp, "r");
// 			char cp;
// 			int ii = 0, jj = 0;
// 			for (cp = getc(fd); cp != EOF; cp = getc(fd))
// 			{
// 				if (cp == '\n')
// 				{
// 					array[ii][jj]='\0';
// 					ii++;
// 					jj = 0;
// 				}
// 				else
// 				{
// 					array[ii][jj] = cp;
// 					jj++;
// 				}
// 			}
// 			fclose(fd);
// 			for(int kk=num;kk<count;kk++)
// 			{
// 				printf("%s\n",array[kk]);
// 			}
// 		}
// 		else if(strcmp(ptr,"nightswatch")==0)
// 		{
// 			int flag=0,crc;
// 			int sec;
// 			ptr=strtok(NULL,delim);
// 			if(ptr==NULL || strcmp(ptr,"-n")!=0)
// 			{
// 				flag=1;
// 			}
// 			ptr=strtok(NULL,delim);
// 			if(ptr==NULL)
// 			{
// 				flag=1;
// 			}
// 			else
// 			{
// 				for(int i=0;i<strlen(ptr);i++)
// 				{
// 					if(ptr[i]<'0' || ptr[i]>'9')
// 					{
// 						flag=1;
// 						break;
// 					}
// 				}
// 			}
// 			if(flag==0)
// 			{
// 				sscanf(ptr,"%d",&sec);
// 			}
// 			ptr=strtok(NULL,delim);
// 			if(ptr==NULL)
// 			{
// 				flag=1;
// 			}
// 			else if(strcmp(ptr,"interrupt")==0)
// 			{
// 				crc=1;
// 			}
// 			else if(strcmp(ptr,"newborn")==0)
// 			{
// 				crc=2;
// 			}
// 			else
// 			{
// 				flag=1;
// 			}
// 			if(flag==0)
// 			{
// 				gotonight(sec,crc);
// 			}
// 			else
// 			{
// 				printf("Invalid arguments\n");
// 			}
// 		}
// 		else
// 		{
// 			int nofa=0;
// 			while(ptr!=NULL)
// 			{
// 				// if(strlen(ptr)>0)
// 				// {
// 				// 	if(ptr[strlen(ptr)-1]=='&')
// 				// 	{
// 				// 		nofa++;
// 				// 	}
// 				// 	else if(strcmp(ptr,"&")==0)
// 				// 	{
// 				// 		nofa++;
// 				// 	}
// 				// }
// 				ptr=strtok(NULL,delim);
// 			}
			
// 			for(int ij=0;ij<strlen(cpargv);ij++)
// 			{
// 				if(cpargv[ij]=='&')
// 				{
					
// 					nofa++;
// 				}
// 			}
// 			gotofgbg(cpargv,nofa,cwd_1,in,out);
// 		}

// 		ptr = strtok(NULL, delim);
// 	}
}
