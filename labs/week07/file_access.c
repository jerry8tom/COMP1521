#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int fd = open("xyz.txt", O_RDONLY);
    if (fd<0)
    {
        printf("entered\n");
        perror(NULL);
        exit(1);
    }
    return 0;
}