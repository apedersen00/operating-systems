#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

int main() {
    const char *name = "/tmp/myfifo";
    const char *msg  = "hello";
    int fd;

    mkfifo(name, 0666);

    fd = open(name, O_WRONLY);
    write(fd, msg, strlen(msg) + 1);
    close(fd);

    unlink(name);

    return 0;
}