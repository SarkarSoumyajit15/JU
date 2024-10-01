#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 3

// Shared variable
float A, B, C, D, E;
float **array;
float X, Y;
int rows, cols;

// Mutex and condition variable for synchronization
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// State variables to control the synchronization and execution
int current_thread;
int cnt;

// Function for each thread's task


void *thread_1(void *arg)
{
    while (1)
    {
        // Functioning of Task 1 :

        pthread_mutex_lock(&mutex);
        while (current_thread != 1)
        {
            pthread_cond_wait(&cond, &mutex);
        }


        // Task for thread 1
        printf("Thread 1 is executing task 1 :  (X * Y) \n");
        A = X * Y;
        printf("Thread 1 finished task 1  \n");


        // Incrementing current_thread to give control to the next task(i.e. task 2)
        current_thread++;
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);

        // Functioning of Task 5 :

        pthread_mutex_lock(&mutex);
        while (current_thread != 5)
        {
            pthread_cond_wait(&cond, &mutex);
        }

        // Task for thread 1
        printf("Thread 1 is executing task 5 :   (X + Y) * (X - Y) \n");
        E = (X + Y) * (X - Y);
        printf("Thread 1 finished task 5 \n");

        // Incrementing current_thread to give control to perfrom the next task (i.e. task of the main thread)

        current_thread = (current_thread + 1) % 6;
        cnt++;
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);

        // If all the (X,Y)  pairs are generated then just break from the infinite while loop
        if(cnt >= rows-1) break;
    }

    return NULL;
}

void *thread_2(void *arg)
{
    while (1)
    {
        // Functioning of Task 2 starts here

        pthread_mutex_lock(&mutex);
        while (current_thread != 2)
        {
            pthread_cond_wait(&cond, &mutex);
        }

        // Task for thread 2
        printf("Thread 2 is executing task  2 : (X * Y)/2 \n");
        B = (X * Y) / 2;
        printf("Thread 2 finished task 2 \n");

        // Incrementing current_thread to pass the control to perform the next task (i.e. task 3)
        current_thread++;
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);

        // Funcitoning of Task 4 starts here

        pthread_mutex_lock(&mutex);
        while (current_thread != 4)
        {
            pthread_cond_wait(&cond, &mutex);
        }

        // Task for thread 2
        printf("Thread 1 is executing task 4 : (X * Y) / (X + Y) \n");

        D = (X * Y) / (X + Y);
        printf("Thread 1 finished task 4  \n");


        // Incrementing current_thread to pass the control to perform next task (i.e. task 5);
        current_thread++;
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);

        // If all (X,Y) pairs are generated then just break from the infinite while loop
        if (cnt >= rows - 2)
            break;
    }
    return NULL;
}

void *thread_3(void *arg)
{
    while (1)
    {
        // Functioning for Task 3 starts here

        pthread_mutex_lock(&mutex);
        while (current_thread != 3)
        {
            pthread_cond_wait(&cond, &mutex);
        }

        // Task for thread 3
        printf("Thread 3 is executing task 3 :   (X + Y)  \n");

        C = (X + Y);
        printf("Thread 3 finished task 3 \n");

        // Incrementing current_thread to pass the control to perform next task (i.e. task 4);
        current_thread++;
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);

        // If all (X,Y) pairs are generated then just break from the infinite while loop
        if (cnt >= rows - 2)
            break;
    }
    return NULL;
}

int main()
{

    // Taking the no. of pairs to be generated from the user
    printf("Enter the number of pairs to be generated \n");
    scanf("%d", &rows);

    cols = 7;

    // Dynamically initializing the array with row = rows and cols  = 7
    array = (float **)calloc(rows, sizeof(float *));
    for (int i = 0; i < rows; i++)
    {
        array[i] = (float *)calloc(cols, sizeof(float));
    }

    // Initializing 3 thread pointers :
    pthread_t threads[NUM_THREADS];

    // Create Threads

    printf("Creating 3 threads  \n\n");


    pthread_create(&threads[0], NULL, thread_1, NULL);
    sleep(1);
    pthread_create(&threads[1], NULL, thread_2, NULL);
    sleep(1);
    pthread_create(&threads[2], NULL, thread_3, NULL);
    sleep(1);

    printf("Threads created\n\n");

    // Initializing cnt with -1 and incrementing it after task 5 is completed
    cnt = -1;

    while (cnt < rows)
    {

        // The next 4 lines are just for synchronizing the main thread
        pthread_mutex_lock(&mutex);
        while (current_thread != 0)
        {
            pthread_cond_wait(&cond, &mutex);
        }

        // Task for thread main
        printf("Main Thread is executing its task  \n");

        printf("Inserting all these generated nos in an array  \n\n");
        if (cnt != -1)
        {
            array[cnt][2] = A;
            array[cnt][3] = B;
            array[cnt][4] = C;
            array[cnt][5] = D;
            array[cnt][6] = E;
        }


        if (cnt == rows - 1)
        {
            break;
            pthread_mutex_unlock(&mutex);
        }

        printf("Generating new no.s \n");
        sleep(1);
        int randX = rand() % 10;
        int randY = rand() % 10;

        X = randX;
        Y = randY;

        array[cnt+1][0] = X;
        array[cnt+1][1] = Y;

        printf("Main thread task : New nos generated  - > finished \n\n\n");
        printf("Now we will work with new (X,Y) pairs : %f & %f \n\n",X,Y);

        current_thread++;

        // Allow the first thread to proceed
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("\n\n");

    printf("  ( X     ,      Y ) :                A                 B              C               D               E    \n\n\n");

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if(j < 2){
                if( j == 0 ) printf("( ");
                printf(" %f ",array[i][j]);

                if( j == 1) printf(" )  :");
                if(j == 0) printf(" , ");
                continue;
            }
            printf("    %f    ", array[i][j]);
        }
        printf("\n\n");
    }

    return 0;
}
