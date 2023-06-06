/*This driver is to set the input and output permission of a particular port*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/io.h>

#define BASEPORT 0x70

int main()
{
    char val=0;

    if(ioperm(BASEPORT,4,1))
    {
        perror("ioperm");
        exit(1);
    }

    val=inb(BASEPORT+1);
    printf("seconds=%x \n",val);

    if(ioperm(BASEPORT,3,0))
    {
        perror("ioperm");
        exit(1);
    }

    return 0;
}