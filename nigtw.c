#include "headers.h"
#include "nigtw.h"
extern int error;
int letter()
{
    int c;
    struct termios oo;
    struct termios nn;
    tcgetattr(STDIN_FILENO, &oo);
    nn = oo;
    nn.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &nn);
    ioctl(0, FIONREAD, &c);
    if (c > 0)
    {
        c = getchar();
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oo);
    return c;
}
void gotonight(int sec,int crc)
{

    int c;
    clock_t tt;
    if(crc==1)
    {
        FILE *fd = fopen("/proc/interrupts", "r");
        if (fd == NULL)
        {
            error = 1;
            perror("FIle open");
        }
        char srr[1024];
        fgets(srr, 400, fd);
        printf("\n%s\n", srr);
        fclose(fd);
    }
    while (c != 'q')
    {
        //printf("sdvsdvdsb\n");
        if (crc == 1)
        {
            FILE *fd = fopen("/proc/interrupts", "r");
            if (fd == NULL)
            {
                error = 1;
                perror("FIle open");
            }
            char str[1024], ss[1024];
            fgets(str, 400, fd);
            int ll=5;
            while(ll--)
            {
                fgets(str, 400, fd);
                int i = 0;
                int fir=0;
                char fnum;
                for (int j = 0; j < strlen(str); j++)
                {
                    if((str[j] >= '0' && str[j] <= '9'))
                    {
                        if(fir==0)
                        {
                            fnum=str[j];
                            ss[j]=' ';
                        }
                        else
                        {
                        ss[j] = str[j];
                        }
                        i++;
                        fir++;
                    }
                    if ( str[j] == ' ')
                    {
                        ss[j] = str[j];
                        i++;
                    }
                }
                ss[i] = '\0';
                if(fnum=='1')
                {
                    printf("\r%s\n", ss);
                    break;
                }
            }
            
            fclose(fd);
        }
        if (crc == 2)
        {
            FILE *fd = fopen("/proc/loadavg", "r");
            if (fd == NULL)
            {
                error = 1;
                perror("FIle open");
            }
            char str[1024], ss[1024];
            float i1, i2, i3;
            int i4, i5, i6;
            fscanf(fd, "%f %f %f %d/%d %d", &i1, &i2, &i3, &i4, &i5, &i6);
            printf("%d\n", i6);
        }

        tt = clock();
        while (clock() < (tt + (sec * CLOCKS_PER_SEC)))
        {
            c = letter();
            if (c == 'q')
            {
                break;
            }
        }
    }
}
void anight(char * cmd)
{
    char delim[]="\t ";
        int flag = 0, crc;
        int sec;
        char *ptr = strtok(cmd, delim);
        if (ptr == NULL || strcmp(ptr, "-n") != 0)
        {
            flag = 1;
        }
        ptr = strtok(NULL, delim);
        if (ptr == NULL)
        {
            flag = 1;
        }
        else
        {
            for (int i = 0; i < strlen(ptr); i++)
            {
                if (ptr[i] < '0' || ptr[i] > '9')
                {
                    flag = 1;
                    break;
                }
            }
        }
        if (flag == 0)
        {
            sscanf(ptr, "%d", &sec);
        }
        ptr = strtok(NULL, delim);
        if (ptr == NULL)
        {
            flag = 1;
        }
        else if (strcmp(ptr, "interrupt") == 0)
        {
            crc = 1;
        }
        else if (strcmp(ptr, "newborn") == 0)
        {
            crc = 2;
        }
        else
        {
            flag = 1;
        }
        if (flag == 0)
        {
            gotonight(sec, crc);
        }
        else if (strcmp(ptr, "@") == 0)
        {
            flag = 4;
        }

        else
        {
            error=1;
            fprintf(stderr,"Invalid arguments\n");
        }
    
}