////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////			mew115				////////////////
////////////////			cs1550				////////////////
////////////////			project 2			////////////////
////////////////			trafficsim.c		////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

#include <linux/unistd.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <time.h>

struct p_node
{
	struct task_struct *process;
	struct p_node * next;
};

struct queue 
{ 
	struct p_node *head;
	struct p_node *tail;

	int size;
};

struct cs1550_sem
{
	int value;
	//queue of processes
	struct queue *plist;
};

void sleep_s(long sec)
{
	struct timespec t1;
	t1.tv_nsec = 0;
	t1.tv_sec = sec;
	nanosleep(&t1, NULL);
}


void down(struct cs1550_sem *sem)
{
	syscall(__NR_cs1550_down, sem);
}

void up(struct cs1550_sem *sem)
{
	syscall(__NR_cs1550_up, sem);
}


static struct cs1550_sem *total_num_cars;
static struct cs1550_sem *south_mutex;
static struct cs1550_sem *north_mutex;

static void *s_ptr;
static void *n_ptr;
static int *s_head;
static int *n_head;
static int *s_tail;
static int *n_tail;
static int *s_in;
static int *s_out;
static int *n_in;
static int *n_out;

int rand_num()
{
	srand(time(NULL));
	int r = rand() % 10;
	return r;
}

int main(int argc, char** argv)
{
	total_num_cars = (struct cs1550_sem*) mmap(NULL, sizeof(struct cs1550_sem), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
	south_mutex = (struct cs1550_sem*) mmap(NULL, sizeof(struct cs1550_sem), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
	north_mutex = (struct cs1550_sem*) mmap(NULL, sizeof(struct cs1550_sem), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
	
	total_num_cars->value = 0;
	south_mutex->value = 1;
	north_mutex->value = 1;




	int total_car = 0;
	s_ptr = mmap(NULL, 1000, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
	n_ptr = mmap(NULL, 1000, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
	s_head = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
	n_head = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
	s_tail = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
	n_tail = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
	*s_head = -1;
	*s_tail = -1;
	*n_head = -1;
	*n_tail = -1;

	int prod_pid;
	prod_pid = fork();
	if(prod_pid < 0)
	{
		printf("Fork ERROR!!\n");
	}
	else if(prod_pid == 0)
	{
		if(fork() == 0)			// south
		{
			int s_car = 1000;
			while (1)
			{
				down(south_mutex);

				//put one car in south queque

				if(-1 == *s_head)	// check if south queue is empty
				{
					*s_head = 0;
					*s_tail = 0;
					s_in = (int*)(s_ptr + *s_tail);
					*s_in = s_car; //
				}
				else
				{
					*s_tail = (*s_tail + 4) % 1000;
					s_in = (int*)(s_ptr + *s_tail);
					*s_in = s_car;
				}

//				printf("\nSouth:\ns_head:%d s_tail:%d\n", *s_head, *s_tail);
				printf("A new Car No.%d is waitting on south.\n", *s_in);
				s_car++;

				up(south_mutex);
				up(total_num_cars);
				int r = rand_num();
//				printf("r:%d\n", r);
				if( r < 8 )
				{
					sleep(1);
					continue;
				}
				else
				{
					//sleep 20s;
					sleep_s(20);
				}
			}
		}
		else					// north
		{
			int n_car = 3000;
			while (1)
			{
				down(north_mutex);

				//put one car in north queque

				if(-1 == *n_head)	// check if south queue is empty
				{
					*n_head = 0;
					*n_tail = 0;
					n_in = n_ptr + *n_tail;
					*n_in = n_car; //
				}
				else
				{
					*n_tail = (*n_tail + 4) % 1000;
					n_in = n_ptr + *n_tail;
					*n_in = n_car;
				}


//				printf("\nNorth:\nn_head:%d n_tail:%d \n", *n_head, *n_tail);
				printf("A new Car No.%d is waitting on north.\n", *n_in);
				n_car++;

				up(north_mutex);
				up(total_num_cars);
				int r = rand_num();
//				printf("r:%d\n", r);
				if( r < 8 )
				{
					sleep_s(1);
					continue;
				}
				else
				{
					//sleep 20s;
					sleep_s(20);
				}
			}
		}
	}
	else						// flagperson
	{
		int cur_car;
		int direction = 0;		// 0 for south, 1 for north
		int s_car_total;
		int n_car_total;
		while (1)
		{
			down(total_num_cars);
			printf("Flagperson:\n");
			//choose direction:
			if(0 > *s_head)
			{
				s_car_total = 0;
			}
			else
			{
				s_car_total = (int)((*s_tail - *s_head)/4) + 1;
			}
			if(0 > *n_head)
			{
				n_car_total = 0;
			}
			else
			{
				n_car_total = (int)((*n_tail - *n_head)/4) + 1;
			}
			printf("#### South: %d cars #### #### North: %d cars ####\n", s_car_total, n_car_total);
			if (0 == direction)
			{			
				if (0 > *s_head || 10 <= n_car_total)
				{
					printf("Direction changed!!From North to South now!!");
//					printf("total car in sourth is: %d\n", s_car_total);
					direction = 1;
				}
			}
			else
			{
				
				if (0 > *n_head || 10 <= s_car_total)
				{
					printf("Direction Changed! From Sourth to North now!!");
//					printf("total car in north is: %d\n", n_car_total);
					direction = 0;
				}
			}
			if (0 == direction)
			{
				down(south_mutex);
				//pass one car in south queue
				s_out = s_ptr + *s_head;
				cur_car = *s_out;
//				printf("s_head:%d s_tail:%d\n", *s_head, *s_tail);
				printf("A car No.%d is passing from south to north!!\n", cur_car);

				if(*s_head == *s_tail)
				{					
					*s_head = -1;
				}
				else
				{
					*s_head = (*s_head + 4) % 1000;
				}
//				printf("s_head:%d s_tail:%d\n", *s_head, *s_tail);
				//sleep 2s;
				sleep_s(2);
				up(south_mutex);
			}
			else
			{
				down(north_mutex);
				//pass one car in south queue
				n_out = n_ptr + *n_head;
				cur_car = *n_out;
//				printf("n_head:%d n_tail:%d\n", *n_head, *n_tail);
				printf("A car No.%d is passing from north to south!!\n", cur_car);				
				
				if(*n_head == *n_tail)
				{					
					*n_head = -1;
				}
				else
				{
					*n_head = (*n_head + 4) % 1000;
				}
//				printf("n_head:%d n_tail:%d\n", *n_head, *n_tail);
				//sleep 2s
				sleep_s(2);
				up(north_mutex);
			}
			
			
		}
	}


}



