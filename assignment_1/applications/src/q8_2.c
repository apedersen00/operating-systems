#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#define BUFSIZE 1024

int main() {
    const char *name = "/tmp/myfifo";
    char recv_buf[BUFSIZE];
    int fd;

    mkfifo(name, 0666);

    sleep(10);

    fd = open(name, O_RDONLY);
    read(fd, recv_buf, BUFSIZE);
    close(fd);

    printf("Received %s \n", recv_buf);

    return 0;
}