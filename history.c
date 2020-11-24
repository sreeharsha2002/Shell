#include "headers.h"
#include "history.h"
extern int error;
void gotohistory(char *inph, char *cwd_1 )
{
   // printf("%s\n",inph);
    int ihst = 0;
    char hst[20][1024];
    FILE *fp;
    char actpath[1024];
    sprintf(actpath,"%s/history.txt",cwd_1);
    fp = fopen(actpath, "r");
    if (fp == NULL)
    {
        fp = fopen(actpath, "w");
    }
    char c;
    int count = 0;
    for (c = getc(fp); c != EOF; c = getc(fp))
    {
        if (c == '\n')
            count = count + 1;
    }
    /*c=getc(fp);
		while(c!= EOF)
		{
			if (c == '\n')
			{
				count = count + 1;
			}
			c=getc(fp);
		}*/
    fclose(fp);

    if (count < 20)
    {
        fp = fopen(actpath, "r");
        char cp;
        int ii = 0, jj = 0;
        for (cp = getc(fp); cp != EOF; cp = getc(fp))
        {
            if (cp == '\n')
            {
                hst[ii][jj]='\0';
                ii++;
                jj = 0;
            }
            else
            {
                hst[ii][jj] = cp;
                jj++;
            }
        }
        //	printf("%s",inp);
        if (strcmp(hst[ii - 1], inph) != 0)
        {
           // printf("%s\n", inph);
            strcpy(hst[ii], inph);
           // printf("%s\n", inph);
            //printf("%s",hst[ihst]);
            ii++;
            fclose(fp);
            fp = fopen(actpath, "w");
            for (int kk = 0; kk < ii; kk++)
            {
                fprintf(fp, "%s\n", hst[kk]);
                //	printf("->%d %s\n",kk,hst[kk]);
            }
        }
        fclose(fp);
    }
    else
    {
        fp = fopen(actpath, "r");
        char cp;
        int ii = 0, jj = 0;
        for (cp = getc(fp); cp != EOF; cp = getc(fp))
        {
            if (cp == '\n')
            {
                hst[ii][jj] = '\0';
                ii++;
                jj = 0;
            }
            else
            {
                hst[ii][jj] = cp;
                jj++;
            }
        }
        if (strcmp(hst[ii - 1], inph) != 0)
        {
            //printf("%s\n", inph);
            for (int kk = 0; kk < ii - 1; kk++)
            {
                strcpy(hst[kk], hst[kk + 1]);
            }
            //printf("%s\n", inph);
            strcpy(hst[ii - 1], inph);
            //printf("%s\n", inph);
            fclose(fp);
            fp = fopen(actpath, "w");
            for (int kk = 0; kk < ii; kk++)
            {
                fprintf(fp, "%s\n", hst[kk]);
                //	printf("-*****%d %s\n", kk, hst[kk]);
            }
        }
        fclose(fp);
    }
}
void retrivehistory(char *cmd,char *cwd_1)
{
   char delim[]="\t ";
        char *ptr = strtok(cmd, delim);
        int num, flag = 0;
        if (ptr == NULL)
        {
            num = 10;
            flag = 1;
        }
        else
        {
            sscanf(ptr, "%d", &num);
        }
        FILE *fd;
        char actp[1024];
        sprintf(actp, "%s/history.txt", cwd_1);
        fd = fopen(actp, "r");
        if (fd == NULL)
        {
            error = 1;
            perror("FIle open");
        }
        char array[20][1024];
        char c;
        int count = 0;
        for (c = getc(fd); c != EOF; c = getc(fd))
            if (c == '\n') // Increment count if this character is newline
                count = count + 1;

        if (count < num)
        {
            num = 0;
        }
        else
        {
            num = count - num;
            if(num<0)
            {
                num=0;
            }
        }
        fclose(fd);
        fd = fopen(actp, "r");
        if (fd == NULL)
        {
            error = 1;
            perror("FIle open");
        }
        char cp;
        int ii = 0, jj = 0;
        for (cp = getc(fd); cp != EOF; cp = getc(fd))
        {
            if (cp == '\n')
            {
                array[ii][jj] = '\0';
                ii++;
                jj = 0;
            }
            else
            {
                array[ii][jj] = cp;
                jj++;
            }
        }
        fclose(fd);
        for (int kk = num; kk < count; kk++)
        {
            printf("%s\n", array[kk]);
        }
    
}
