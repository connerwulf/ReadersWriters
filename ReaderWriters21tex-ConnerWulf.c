#define _REENTRANT
#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stdbool.h>


/* Student: Conner Wulf. */

//Global Variables
struct rusage mytiming;
struct timeval mytimeval;
struct shared_dat{ int value;  /* shared variable to store result*/};
struct shared_dat  *counter;

bool in_cs;
int threadsReading;
sem_t reader, writer;
int getpid();


/****************************************************************
* This function reads the value of shared variable "counter"
  by 250 million times
****************************************************************/
void * reader_thread(void *arg)
{
	int i = 0;
  int value;
  while(i < 250000000)
  {
    if(in_cs != true)
    {
      printf("test");
      if(threadsReading == 0)
      {
        sem_wait(&writer);
      }

      sem_wait(&reader);
        threadsReading++;
      sem_post(&reader);

      value = counter->value;

      sem_wait(&reader);
        threadsReading--;
      sem_post(&reader);

      if(threadsReading == 0)
      {
        sem_post(&writer);
      }
      i++;
    }
  }
	printf("Reader %d has finished", getpid());
  return(NULL);
}


/****************************************************************
This function writes to the value of shared variable "counter"
by one 25000 times
****************************************************************/
void * writer_thread(void *arg)
{
	int line = 0;

	while (line < 25000)
	{
    in_cs = true;
    sem_wait(&writer);

    /* Critical Section */
	  counter->value = counter->value + 1;
    in_cs = false;
    sem_post(&reader);
    line++;
   }
     printf("Writer finished: Counter = %d\n", counter->value);
     return(NULL);
}



/****************************************************************
*                  Main Body                                    *
****************************************************************/
int main()
{
  int numOfReaders = 0;
  in_cs = false;
  threadsReading = 0;
  counter = (struct shared_dat *) malloc(sizeof(struct shared_dat));
	counter->value = 0;
  sem_init(&reader,0,1);
  sem_init(&writer,0,1);


      printf("How many readers should there be? (1 <= n <= 16):");
      scanf("%d", &numOfReaders);
      printf("%d", numOfReaders);




   pthread_t readers[numOfReaders];
   pthread_t writer[1];
   pthread_attr_t	attr[1];
 /* Required to schedule thread independently.*/
   pthread_attr_init(&attr[0]);
   pthread_attr_setscope(&attr[0], PTHREAD_SCOPE_SYSTEM);


  int k = (int) (numOfReaders/2);
  int i;
  for(i = 0; i < k; i++)
  {
    pthread_create(&readers[i], &attr[0], reader_thread, (void*) i);
  }

/* Create the writer thread*/
  pthread_create(&writer[0], &attr[0], writer_thread, NULL);

  for(i = k ; i < numOfReaders; i++)
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



	// printf("from parent counter  =  %d\n", counter->value);
  // getrusage(RUSAGE_SELF, &mytiming);
  // printf("Time used is sec: %d, usec %d\n",mytiming.ru_utime.tv_sec,mytiming.ru_utime.tv_usec);
  // printf("System Time used is sec: %d, usec %d\n",mytiming.ru_stime.tv_sec,mytiming.ru_stime.tv_usec);
	// printf("---------------------------------------------------------------------------\n");
	 printf("\t\t	End of simulation\n");

	exit(0);

}
