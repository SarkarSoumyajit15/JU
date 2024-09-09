
/*


Name  :  Soumyajit Sarkar
Roll No. : 002211001106
Section A2


Assignment Details - The objective of this assignment is to create a deadlock. For this purpose define two global variables (Total_1 and Total_2) and initialize both of them to 1000. You should also have two mutexes to protect these two global variables. You need to create two threads also. 

The function of each of the threads is to generate a random quantity (not more than 50) and subtract that quantity from one of the Total and add that quantity to the other Total. While manipulating the Totals, each Thread should lock both the mutex and then unlock it after changing the Totals. 

The order of locking and unlocking the Mutex is very important, because that’s what creates a Deadlock. Once you correctly identify this order, you should upload the program. Also, include that information (as comment in your source file) how to avoid this kind of Deadlock.

If these two Threads, Two Totals and Two Mutex are not good enough for Deadlock;
Then you need to create one more Thread, one more Total and and one more Mutex; and carry on the same experiment.

Make sure there are enough printf in your program so that it can be clearly understood that there is a Deadlock. One way to ensure enough printf is to display the total of Total_1 and Total_2 all the time after every operation on it.





Input Description  - No input
Output Description - Printing the values of Total_1 and Total_2 and the total of (Total_1 + Total_2)  after each thread perform its operations till DEADLOCK is not attained


Sample Output : 

[be22106@localhost Ass5]$ vi 106_Ass5.c 
[be22106@localhost Ass5]$ gcc 106_Ass5.c -pthread
[be22106@localhost Ass5]$ ./a.out
Thread 1: Trying to lock Lock mutex1...
Thread 1 : Locked mutex 1 
Thread 1: Trying to lock mutex2...
Thread 1: Both mutexes locked.
Thread 2: Trying to lock  mutex2...
Thread1 : 
Value of total1 : 1003  ,  value of total2 : 997 and their total is 2000 
Thread 1: Released mutexes.
Thread 1: Trying to lock Lock mutex1...
Thread 1 : Locked mutex 1 
Thread 1: Trying to lock mutex2...
Thread 2 : Locked mutex 2
Thread 2: Trying to lock mutex1...



*/




#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Mutexes and condition variables
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;

// Shared resources
int total1 = 1000;
int total2 = 1000;

void *thread1_func(void *arg) {

  while(1){
    printf("Thread 1: Trying to lock Lock mutex1...\n");
    pthread_mutex_lock(&mutex1);
    printf("Thread 1 : Locked mutex 1 \n");

    printf("Thread 1: Trying to lock mutex2...\n");
    // Simulate work before trying to lock mutex2
    //sleep(1);

    pthread_mutex_lock(&mutex2);
    printf("Thread 1: Both mutexes locked.\n");

    // Simulate work with both resources

    int randVal = rand() % 20;
    total1 += randVal;
    total2 -= randVal;


    pthread_cond_signal(&cond1);  // Notify thread 2

    int total = total1 + total2;
    printf("Thread1 : \n");
    printf("Value of total1 : %d  ,  value of total2 : %d and their total is %d \n",total1,total2,total);
    // Release mutexes
    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);

    printf("Thread 1: Released mutexes.\n");

  }

}

void *thread2_func(void *arg) {

  while(1){
    printf("Thread 2: Trying to lock  mutex2...\n");
    pthread_mutex_lock(&mutex2);
    printf("Thread 2 : Locked mutex 2\n");

    printf("Thread 2: Trying to lock mutex1...\n");
    // Simulate work before trying to lock mutex1
    //sleep(1);

    pthread_mutex_lock(&mutex1);
    printf("Thread 2: Both mutexes locked.\n");

    // Simulate work with both resources

    int randVal = rand()%20;
    total2 += randVal;
    total1 -= randVal;


    pthread_cond_signal(&cond2);  // Notify thread 1

    int total = total1+total2;

    printf("Thread 2 : \n");
    printf("Value of total1 : %d ,  Value of total2  : %d  and their total is %d \n ",total1,total2,total);

    // Release mutexes
    pthread_mutex_unlock(&mutex1);
    pthread_mutex_unlock(&mutex2);

    printf("Thread 2: Released mutexes.\n");
  }

}

int main() {
    pthread_t thread1, thread2;

    // Create two threads
    pthread_create(&thread1, NULL, thread1_func, NULL);
    pthread_create(&thread2, NULL, thread2_func, NULL);

    // Join threads
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Main thread: Done.\n");
    return 0;
}

