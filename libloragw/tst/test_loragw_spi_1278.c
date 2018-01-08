#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "loragw_spi.h"


#define REG_VERSION                 0x42

char str_dev[]="/dev/GET_GPIO";

int get_io()
{
    int fd;
    unsigned int INT_STATUS = 0;
    //------- open driver--------
    fd = open(str_dev, O_RDWR | O_NONBLOCK);
    if (fd < 0)
    {
        printf("can't open %s\n",str_dev);
        return -1;
    }
    //------------- read INT_STATUS -------
    while(1)
    {
        read(fd, &INT_STATUS, sizeof(INT_STATUS));
        if(INT_STATUS==1)
            printf("GPIO Interrupt Triggered!\n");
        usleep(200000);
    }
    close(fd);

    return 0;
}


/* -------------------------------------------------------------------------- */
/* --- MAIN FUNCTION -------------------------------------------------------- */

int main()
{
    void *spi_target = NULL;
    uint8_t spi_mux_mode = LGW_SPI_MUX_MODE0;
    uint8_t data = 0;

    printf("\n zenghi 加载驱动 %s: %u: \"%s\" function.", __FILE__, __LINE__, __FUNCTION__);
    system("insmod spi-gpio-custom bus0=1,20,22,21,0,10000000,18");

    lgw_spi_open(&spi_target);

    lgw_spi_r(spi_target, spi_mux_mode, LGW_SPI_MUX_TARGET_SX1301, REG_VERSION, &data);
    printf("data received (simple read): %d\n",data);
    lgw_spi_close(spi_target);

    get_io();

    system("rmmod spi-gpio-custom");
    printf("End of test for loragw_spi.c\n");

    return 0;
}

/* --- EOF ------------------------------------------------------------------ */
