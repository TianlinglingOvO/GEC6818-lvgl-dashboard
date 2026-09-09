#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define ON   0
#define OFF  1
static int beep_fd = -1;

int beep_init(void)
{
    beep_fd = open("/dev/beep", O_RDWR);
    if (beep_fd == -1)
    {
        perror("open /dev/beep");
        return -1;
    }
    return 0;
}

void beep_on(void)
{
    if (beep_fd < 0)
    {
        fprintf(stderr, "beep not initialized!\n");
        return;
    }
    ioctl(beep_fd, ON, 1);
}

void beep_off(void)
{
    if (beep_fd < 0)
    {
        fprintf(stderr, "beep not initialized!\n");
        return;
    }
    ioctl(beep_fd, OFF, 1);
}

void beep_deinit(void)
{
    if (beep_fd >= 0)
    {
        close(beep_fd);
        beep_fd = -1;
    }
}