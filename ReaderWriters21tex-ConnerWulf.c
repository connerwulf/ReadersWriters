#define _REENTRANT
#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <semaphore.h>


/* Student: Conner Wulf. */


//Global Variables
struct rusage mytiming;
struct timeval mytimeval;

struct shared_dat
   {
   int value;     /* shared variable to store result*/
   };

struct shared_dat  *counter;
int getpid();
//pthread_mutex_t mutex;
sem_t semaphore;
int jumps=0;

/****************************************************************
* This function increases the value of shared variable "counter"
  by one 2750000 times, and by 100 when count-> is a multiple of 100
****************************************************************/
void * reader_thread(void *arg)
{
	int i;
  int value;
  for(i = 0; i < 250000000)
  {
    value = counter->value;
  }
	printf("Reader %d has finished", getpid());
  return(NULL);
}


/****************************************************************
This function increases the value of shared variable "counter"
by one 275000 times
****************************************************************/
void * writer_thread(void *arg)
{
	int line = 0;

	while (line < 25000)
	{
    line++;
    /* Critical Section */
	  counter->value = counter->value + 1;

   }
	   printf("from process2 counter = %d\n", counter->value);
return(NULL);
}



/****************************************************************
*                  Main Body                                    *
****************************************************************/
int main()
{
  int numOfReaders;
  printf("How many readers should there be?\n");
  scanf("%d", numOfReaders);
  for(int i = 0; i<10;i++)
  {

    printf("%d",numOfReaders);
  }
/*
  pthread_t readers[numOfReaders];
  pthread_t writer[1];
  pthread_attr_t	attr[1];
  int k = (int) (numOfReaders/2);
*/
/* Required to schedule thread independently.
 Otherwise use NULL in place of attr.
  pthread_attr_init(&attr[0]);
  pthread_attr_setscope(&attr[0], PTHREAD_SCOPE_SYSTEM);

for(int i = 0; i < k; i++)
{
  pthread_create(&readers[i], &attr[0], reader_thread, (void*) i);
}
*/
/* Create the writer thread
pthread_create(&writer[0], &attr[0], writer_thread, NULL);

for(i = k ; i < numOfReaders ; i++)
{
  pthread_create(&readers[i], &attr[0], reader_thread, (void*) i);
}

for(int j = 0; j < numOfReaders; j++)
{
  if(j = 0)
  {
    pthread_join(writer[j], NULL);

  }
  pthread_join(readers[j], NULL);
}

*/

	// printf("from parent counter  =  %d\n", counter->value);
  // getrusage(RUSAGE_SELF, &mytiming);
  // printf("Time used is sec: %d, usec %d\n",mytiming.ru_utime.tv_sec,mytiming.ru_utime.tv_usec);
  // printf("System Time used is sec: %d, usec %d\n",mytiming.ru_stime.tv_sec,mytiming.ru_stime.tv_usec);
	// printf("---------------------------------------------------------------------------\n");
	// printf("\t\t	End of simulation\n");

	exit(0);

}
