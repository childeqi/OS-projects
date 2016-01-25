#include <stdio.h>
#include <time.h>

//2048*30
//2048*30+32*50
//

#define RED (2048*30)
#define YEL (2048*30+32*50)
#define BLA 0
#define WHI (2048*31+32*63+31)

typedef unsigned short color_t;

int main(int argc, char** argv)
{
	color_t pikachu[24][28] = {	
		{BLA, BLA, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, BLA, BLA},
		{BLA, BLA, BLA, BLA, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, BLA, BLA, BLA, BLA},
		{BLA, BLA, BLA, YEL, BLA, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, BLA, YEL, BLA, BLA, BLA},
		{BLA, BLA, BLA, YEL, YEL, BLA, BLA, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, BLA, BLA, YEL, YEL, BLA, BLA, BLA},
		{WHI, BLA, BLA, YEL, YEL, YEL, YEL, BLA, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, BLA, YEL, YEL, YEL, YEL, BLA, BLA, WHI},
		{WHI, BLA, BLA, YEL, YEL, YEL, YEL, YEL, BLA, BLA, WHI, WHI, BLA, BLA, BLA, BLA, WHI, WHI, BLA, BLA, YEL, YEL, YEL, YEL, YEL, BLA, BLA, WHI},
		{WHI, WHI, BLA, YEL, YEL, YEL, YEL, YEL, YEL, YEL, BLA, BLA, YEL, YEL, YEL, YEL, BLA, BLA, YEL, YEL, YEL, YEL, YEL, YEL, YEL, BLA, WHI, WHI},
		{WHI, WHI, BLA, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, BLA, WHI, WHI},
		{WHI, WHI, WHI, BLA, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, BLA, WHI, WHI, WHI},
		{WHI, WHI, WHI, WHI, BLA, YEL, BLA, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, BLA, YEL, BLA, WHI, WHI, WHI, WHI},
		{WHI, WHI, WHI, WHI, WHI, BLA, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, BLA, WHI, WHI, WHI, WHI, WHI},
		{WHI, WHI, WHI, WHI, WHI, BLA, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, BLA, WHI, WHI, WHI, WHI, WHI},
		{WHI, WHI, WHI, WHI, BLA, YEL, YEL, YEL, BLA, BLA, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, BLA, BLA, YEL, YEL, YEL, BLA, WHI, WHI, WHI, WHI},
		{WHI, WHI, WHI, WHI, BLA, YEL, YEL, BLA, WHI, BLA, BLA, YEL, YEL, YEL, YEL, YEL, YEL, BLA, BLA, WHI, BLA, YEL, YEL, BLA, WHI, WHI, WHI, WHI},
		{WHI, WHI, WHI, WHI, BLA, YEL, YEL, BLA, BLA, BLA, BLA, YEL, YEL, YEL, YEL, YEL, YEL, BLA, BLA, BLA, BLA, YEL, YEL, BLA, WHI, WHI, WHI, WHI},
		{WHI, WHI, WHI, WHI, BLA, YEL, YEL, YEL, BLA, BLA, YEL, YEL, YEL, BLA, BLA, YEL, YEL, YEL, BLA, BLA, YEL, YEL, YEL, BLA, WHI, WHI, WHI, WHI},
		{WHI, WHI, WHI, BLA, YEL, YEL, RED, RED, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, YEL, RED, RED, YEL, YEL, BLA, WHI, WHI, WHI},
		{WHI, WHI, WHI, BLA, YEL, RED, RED, RED, RED, YEL, YEL, BLA, YEL, BLA, BLA, YEL, BLA, YEL, YEL, RED, RED, RED, RED, YEL, BLA, WHI, WHI, WHI},
		{WHI, WHI, WHI, BLA, YEL, RED, RED, RED, RED, YEL, YEL, YEL, BLA, BLA, BLA, BLA, YEL, YEL, YEL, RED, RED, RED, RED, YEL, BLA, WHI, WHI, WHI},
		{WHI, WHI, WHI, WHI, BLA, YEL, RED, RED, YEL, YEL, YEL, YEL, BLA, RED, RED, BLA, YEL, YEL, YEL, YEL, RED, RED, YEL, BLA, WHI, WHI, WHI, WHI},
		{WHI, WHI, WHI, WHI, WHI, BLA, YEL, YEL, YEL, YEL, YEL, YEL, BLA, RED, RED, BLA, YEL, YEL, YEL, YEL, YEL, YEL, BLA, WHI, WHI, WHI, WHI, WHI},
		{WHI, WHI, WHI, WHI, WHI, WHI, BLA, BLA, YEL, YEL, YEL, YEL, YEL, BLA, BLA, YEL, YEL, YEL, YEL, YEL, BLA, BLA, WHI, WHI, WHI, WHI, WHI, WHI},
		{WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, BLA, BLA, BLA, YEL, YEL, YEL, YEL, YEL, YEL, BLA, BLA, BLA, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI},
		{WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, BLA, BLA, BLA, BLA, BLA, BLA, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI, WHI}
	};
	char key;
	char *dance = "Let's dance!!";
	char *exit = "n => EXIT";
	int swip = 0;
	int pika = 0;

	init_graphics();


	sleep_ms(20);

	char* s = "Hello PIKACHU!!!";
	draw_text(50, 30, s, YEL);
	clear_screen();	
	int i, j;
/*
	for (i = 0; i < 24; i++)
	{
		for (j = 0; j < 28; j++)
		{
			draw_rect((50+j*8), (50+i*8), 8, 8, pikachu[i][j]);
			fill_circle((350+j*8), (50+i*8), 4, pikachu[i][j]);
		}
	}
*/
	sleep_ms(200000000);
	char* start = "Want to see pikachu dance? (y => YES; n => EXIT)";
	draw_text(50, 30, s, BLA);
	draw_text(50, 30, start, YEL);
	
	for (i = 0; i < 24; i++)
			{
				for (j = 0; j < 28; j++)
				{
					draw_rect((50+j*8), (50+i*8), 8, 8, pikachu[i][j]);
					fill_circle((350+j*8), (50+i*8), 4, pikachu[i][j]);
				}
			}
	
	do
	{

		key = getkey();
		if(key == 'y')
			pika = 1;
		if(key == 'n')
			pika = 0;

		if (pika == 1)
		{
			clear_screen();
			fill_rect(50, 30, 400, 16, BLA);
			fill_rect(40, 30, 600, 400, BLA);

			if (swip == 0)
			{
				for (i = 0; i < 24; i++)
				{
					for (j = 0; j < 28; j++)
					{
						fill_circle((350+j*8), (50+i*8), 8, pikachu[i][j]);
					}
				}
				swip = 1;
			}
			else
			{
				for (i = 23; i >= 0; i--)
				{
					for (j = 27; j >= 0; j--)
					{
						fill_circle((50+j*8), (50+i*8), 8, pikachu[i][j]);
					}
				}
				swip = 0;
			}
			sleep_ms(900000000000000);

			draw_text(50, 350, exit, YEL);
		}


	} while(key != 'n');

/*
	int i, j;
	for (i = 0; i < 24; i++)
	{
		for (j = 0; j < 28; j++)
		{
			draw_rect((50+j*8), (50+i*8), 8, 8, pikachu[i][j]);
			fill_circle((350+j*8), (50+i*8), 4, pikachu[i][j]);
		}
	}
*/

	char *gb = "Goodbye!! PIKAPIKACHU!!";
//	draw_text(50, 30, start, BLA);
//	draw_text(50, 30, dance, BLA);
	draw_text(50, 350, exit, BLA);
//	fill_rect(50, 30, 400, 16, BLA);
	fill_rect(50, 30, 400, 16, BLA);
	fill_rect(30, 20, 600, 400, BLA);
	draw_text(50, 30, gb, YEL);
	exit_graphics();	

	return 0;
}

// 00000 000000 00000
// 32    64     32
//