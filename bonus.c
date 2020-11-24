#include "headers.h"
#include "bonus.h"
#include "piping.h"
extern int error;
void gotobonus(char *cutt, char *cwd_1)
{
    char arr[1024][1024];
    int ii=0,jj=0;
    int val=0;
    error=0;
    for(int i=0;i<strlen(cutt);i++)
    {
        if(cutt[i]=='@')
        {
            arr[ii][jj]='\0';
            if(val==0 || (val==1 && error==0) || (val ==2 && error==1))
            {
                error=0;
                piping(arr[ii],cwd_1);
            }
            ii++;
            jj=0;
            val=1;

        }
        else if(cutt[i]=='$')
        {
            arr[ii][jj]='\0';
            if (val == 0 || (val == 1 && error == 0) || (val == 2 && error == 1))
            {
                error = 0;
                piping(arr[ii], cwd_1);
            }
            ii++;
            jj=0;
            val=2;
        }
        else
        {
            arr[ii][jj]=cutt[i];
            jj++;
        }
        
    }
    arr[ii][jj]='\0';
    if (val == 0 || (val == 1 && error == 0) || (val == 2 && error == 1))
    {
        error = 0;
        piping(arr[ii], cwd_1);
    }
    ii++;
    jj=0;
    // for(int i=0;i<ii;i++)
    // {
    //     printf("%s ",arr[i]);
    // }    
}