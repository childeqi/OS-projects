////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////            mew115              ////////////////
////////////////            project1            ////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

/*
	system calls:
	open, ioctl, mmap
	ioctl
	write
	select, read
	nanosleep

*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <fcntl.h>
#include <termios.h>
#include <time.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include "iso_font.h"

#include <stdio.h>


int fb;
struct fb_var_screeninfo var_info;
struct fb_fix_screeninfo fix_info;
void* memo;
struct termios term;
int err_var;
int err_fix;
int size;
typedef unsigned short color_t;


void init_graphics()
{
	// open the graphics device
	// int open(const char *pathname, int flags)
	 
	fb = open("/dev/fb0", O_RDWR); 

	// memory mapping
	// void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset)
	

	// screen resolution detection
	// int ioctl(int fd, unsigned long request, <device dependent control struct>)
	err_var = ioctl(fb, FBIOGET_VSCREENINFO, &var_info);
	err_fix = ioctl(fb, FBIOGET_FSCREENINFO, &fix_info);
	size = var_info.yres_virtual * fix_info.line_length;
	memo = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fb, 0);

	// disable display from keyboard
	// int ioctl(int fd, unsigned long request, <device dependent control struct>)
	
	int err_get = ioctl(1, TCGETS, &term);
	term.c_lflag &= ~ICANON; // disable canonical mode
	term.c_lflag &= ~ECHO; // disable ECHO
	int err_set = ioctl(1, TCSETS, &term);
}



void exit_graphics()
{
	int err = munmap(memo, size);

	int err_get = ioctl(1, TCGETS, &term);
	term.c_lflag |= ICANON; // enable canonical mode
	term.c_lflag |= ECHO; // enable ECHO
	int err_set = ioctl(1, TCSETS, &term);

	close(fb);
}

void clear_screen()
{
	// ssize_t write(int fd, const void *buf, size_t count);
	write(fb, "\033[2J", 7);
}


char getkey()
{
	// int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)
	// ssize_t read(int fd, void *buf, size_t count)
	char ccc = '\0';
	fd_set rfds;
    struct timeval tv;
    int retval;
    ssize_t ccc_size;

    /* Watch stdin (fd 0) to see when it has input. */
    FD_ZERO(&rfds);
    FD_SET(0, &rfds);
    /* Wait up to five seconds. */
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    
    retval = select((int)(stdin + 1), &rfds, NULL, NULL, &tv);
    /* Don't rely on the value of tv now! */

//	printf("%d\n", retval);

    if (retval == -1)
        perror("select()");
    else if (retval > 0)
    {
		ccc_size = read(0, &ccc, 1);
    }
               /* FD_ISSET(0, &rfds) will be true. */

 //	exit(EXIT_SUCCESS);

	return ccc;
}



void sleep_ms(long ms)
{
	struct timespec t1;
	t1.tv_nsec = ms * 1000000;
	t1.tv_sec = 0;
	nanosleep(&t1, NULL);
}


void draw_pixel(int x, int y, color_t color)
{
	int offset = 2 * x + fix_info.line_length * y;
	*((color_t *)(memo + offset)) = color;
}

void draw_rect(int x1, int y1, int width, int height, color_t c)
{
	int i, j;
	for (i = x1; i < x1 + width; i++)
	{
		draw_pixel(i, y1, c);
	}
	for (i = x1; i < x1 + width; i++)
	{
		draw_pixel(i, y1 + height, c);
	}	
	for (j = y1; j < y1 + height; j++)
	{
		draw_pixel(x1, j, c);
	}
	for (j = y1; j < y1 + height; j++)
	{
		draw_pixel(x1 + width, j, c);
	}
}


void fill_circle(int x, int y, int r, color_t c)
{
	int i, j;
	int s;
	for (j = -r; j < r; j++)
	{
		for (i = -r; i < r; i++)
		{
			s = i * i + j * j;
			if (s < r * r)
			{
				draw_pixel((i + x), (j + y), c);
			}
		}
	}
}

void draw_letter(int x, int y, char ccc, color_t c)
{
	//iso_font
	// shift & mask
	unsigned char aaa[16];
	unsigned char mask = 0x01; // mask: 0x10000000
	int ascii_num = (int) ccc;

	int i, j;

	for (i = 0; i < 16; i++)
	{
		aaa[i] = iso_font[ascii_num * 16 + i];
//		printf("aaa: %02x\n", aaa[i]);
	}

	
	for (i = 0; i < 16; i++)
	{
		for (j = 0; j < 8; j++)
		{
			if((int)(aaa[i] & mask) > 0)
			{
				draw_pixel(x + j, y + i, c);
//				printf("*");
			}
//			else
//			{
//				printf(" ");
//			}				
			mask = mask << 1;
		}
		mask = 0x01;
		printf("\n");
	}

}

void draw_text(int x, int y, const char *text, color_t c)
{
	while (*text != '\0')
	{
//		printf("%c", *text);
		draw_letter(x, y, *text, c);
		x = x + 8;
		text++;
	}


}


