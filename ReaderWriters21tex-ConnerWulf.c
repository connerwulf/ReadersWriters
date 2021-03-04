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
struct shared_dat{ int value;  /* shared variable to store result*/};
struct shared_dat  *counter;

int in_cs = 0;
int threadsReading = 0;
sem_t reader, writer, fairShot;


/****************************************************************
* This function reads the value of shared variable "counter"
  by 250 million times
****************************************************************/
void * reader_thread(void *arg)
{
  int reader_id = *((int *) arg);
	int i = 0;
  int value;

    //Reader calls sem_wait to be able to get into the wait queue with the other readers and possibly writer
      sem_wait(&fairShot);
      //Once it is the writer turn they call sem_wait on the reader semaphore to make sure no readers can increment threadsReading
      //This is done so we can keep track of how many readers are in their critical section.
        sem_wait(&reader);

        threadsReading++;

        //When the first thread enters its critical section we make sure the writer thread is not in their critical section
        if(threadsReading == 1)
        {
          sem_wait(&writer);
        }
        //Once Signaled, the reader knows no the writer is not in its critical section
      sem_post(&fairShot);
      //Reader now allows other readers the chance to enter access threadsReading
      sem_post(&reader);

      //Debug Flag to check if writer thread is in its critical section
      if(in_cs == 1)
      {
        printf("ERROR: BOTH READERS AND WRITERS ARE IN CRITICAL SECTION");
      }
      //Critical Section
      for(i = 0; i < 250000000; i++)
      {
        value = counter->value;
      }
      //End of critical section
      //Waits for reader semaphore to decrement threadsReading to signal a reader has left their critical section
      sem_wait(&reader);
        threadsReading--;

      /*When the final Reader thread is leaving their critical section, it signals the writers semaphore to give the writer a chance to
        get into their critical section*/
      if(threadsReading == 0)
      {
        sem_post(&writer);
      }
      //Reader now allows other readers the chance to enter access threadsReading
      sem_post(&reader);





	printf("Reader %d has finished | Counter value = %d\n", reader_id, counter->value);
  return(NULL);
}


/****************************************************************
This function writes to the value of shared variable "counter"
by one 25000 times
****************************************************************/
void * writer_thread(void *arg)
{
    //Writer calls sem_wait to be able to get into the wait queue with the other readers
    sem_wait(&fairShot);
      //Once it is the writer turn they call sem_wait on the writer semaphore to make sure no readers are in its critical section
      sem_wait(&writer);
    //Once Signaled, The writer signals the next reader in the fairShot semaphore ready queue
    sem_post(&fairShot);
    in_cs = 1;
    /* Critical Section */
    for(int w = 0; w < 25000; w++)
    {
	  counter->value = counter->value + 1;
    }
    //End of Critical sectoin
    sem_post(&writer);
    in_cs = 0;

     printf("\nWriter 0 has finished | Counter value = %d\n\n", counter->value);
     return(NULL);
}



/****************************************************************
*                  Main Body                                    *
****************************************************************/
int main()
{
  //Allocation shared counter, variables
  int numOfReaders = 0;
  counter = (struct shared_dat *) malloc(sizeof(struct shared_dat));
	counter->value = 0;

  //Prompt user to set how many threads there should be
  printf("How many readers should there be? (1 <= n < 16):");
  scanf(" %d", &numOfReaders);
  printf("\n");
  //Initial semaphore used to control access to for the readers shared counter (threadsReading)
  sem_init(&reader,0,1);
  //Initial semaphore used to control access to for the writers critical section
  sem_init(&writer,0,1);

 //Initial semaphore used create fairness in the solution.
  sem_init(&fairShot,0,1);





  //Pthread intialization and allocation
   fflush(stdout);
   pthread_t *readers;
   pthread_t writer[1];
   pthread_attr_t	attr[1];
   readers = malloc(sizeof(pthread_t) * numOfReaders);
 /* Required to schedule thread independently.*/
   pthread_attr_init(&attr[0]);
   pthread_attr_setscope(&attr[0], PTHREAD_SCOPE_SYSTEM);

   // find half num of threads
  int k = (int) (numOfReaders/2);
  int i = 0;
  int readNums[numOfReaders];

  //Created array of numbers to pass to pthread.
  //This solved an error where the reader number was not printing out correctly
  for(i=0; i< numOfReaders; i++)
  {
    readNums[i] = i;
  }

  //Create the first half of the readers
  for(i = 0; i < k; i++)
  {
    pthread_create(&readers[i], &attr[0], reader_thread, (void*) &readNums[i]);
  }

/* Create the writer thread*/
  pthread_create(&writer[0], &attr[0], writer_thread, NULL);

  //Create the second half of the readers
  for(i = k ; i < numOfReaders; i++)
  {

    pthread_create(&readers[i], &attr[0], reader_thread, (void*) &readNums[i]);
  }

  //Wait for the writer thread to finish
      pthread_join(writer[0], NULL);

//Wait for the reader threads to finish
  for(int j = 0; j < numOfReaders; j++)
  {

    pthread_join(readers[j], NULL);

  }




	 printf("\n\t\t	End of simulation\n\n");

	exit(0);

}
