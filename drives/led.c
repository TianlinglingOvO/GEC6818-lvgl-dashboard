#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define NO 1
#define OFF 0

#define LED1 7
#define LED2 8
#define LED3 9
#define LED4 10

int LED_Mode(char* LED_DRV_PATH, int status, int LED_IO_Number)
{
    /* 缓冲区 */
    char buf[2];
    /* 打开驱动文件位置， 只读写*/
    int fd = open(LED_DRV_PATH, O_RDWR);

    /* -1是读取失败的返回值 */
    if (fd == -1)
    {
        perror("open error");
        return -1;
    }

    buf[0] = status;        /* 状态 */
    buf[1] = LED_IO_Number; /* 灯号 */
    write(fd, buf, sizeof(buf));

    close(fd);
    return 0;
}

void LED_ALL(int status)
{
    LED_Mode("/dev/led_drv", status, 7);
    LED_Mode("/dev/led_drv", status, 8);
    LED_Mode("/dev/led_drv", status, 9);
    LED_Mode("/dev/led_drv", status, 10);
}

void LED_One(int status)
{
    LED_Mode("/dev/led_drv", status, 7);
}

void LED_Two(int status)
{
    LED_Mode("/dev/led_drv", status, 8);
}

void LED_Thr(int status)
{
    LED_Mode("/dev/led_drv", status, 9);
}

void LED_Four(int status)
{
    LED_Mode("/dev/led_drv", status, 10);
}