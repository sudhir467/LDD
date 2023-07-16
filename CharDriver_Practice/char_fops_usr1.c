/*----------------user app for character device driver------------*/
/*---------------------for char_fops1.c---------------------------*/

#include<stdlib.h>
#include<fcntl.h>
#include<stdio.h>
#include<unistd.h>

#define DEVICE_FILE  "/dev/mychardev"

int main()
{
    int fd;
    int retval;
    char buffer[10];

    printf("Opening file: %s \n",DEVICE_FILE);
    fd=open(DEVICE_FILE,O_RDWR);

    if(fd<0)
    {
        perror("Open failed");
        exit(1);
    }
    else
    {
        getchar();
        retval= write(fd,"hello",5);
        printf("Write retval:%d \n",retval);
        getchar();

        retval=read(fd,buffer,10);
        printf("Read retval: %d \n",retval);
        getchar();

        printf("closing file \n");
        close(fd);
        getchar();

    }
    return 0;
}