/*********************User space application for char_fops3.c **********************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define DEVICE_FILE "/dev/mydevice"

int main()
{
    int fd;
    int retval;
    int buffer[10];
    printf("Opening file:%s \n", DEVICE_FILE);
    fd = open(DEVICE_FILE, O_RDWR);
    if (fd < 0)
    {
        perror("Open failed");
        exit(1);
    }

    getchar();

    retval = write(fd, "hello", 5);
    printf("Write retval:%d \n", retval);
    getchar();

    retval = read(fd, buffer, 10);
    printf("Read retval:%d \n", retval);
    printf("Buffer: %s \n", buffer);
    getchar();

    printf("closing file \n");
    close(fd);
    getchar();

    return 0;
}