#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 3
// Shared variable
float A,B,C,D,E;
float **array
float X,Y


// Mutex and condition variable for synchronization
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// State variable to indicate the current thread number
int current_thread;
int cnt;



// Function for each thread's task
void* thread_task_0(void* arg) {
    pthread_mutex_lock(&mutex);
    while (current_thread != 0) {
        pthread_cond_wait(&cond, &mutex);
    }

    // Task for thread 0
    printf("Thread 0 is executing task: Computing sum of first 10 numbers...\n");
    int sum = 0;
    for (int i = 1; i <= 10; i++) {
        sum += i;
    }
    printf("Thread 0 finished task: Sum = %d\n", sum);

    current_thread++;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void* thread_task_1(void* arg) {
    pthread_mutex_lock(&mutex);
    while (current_thread != 1) {
        pthread_cond_wait(&cond, &mutex);
    }

    // Task for thread 1
    printf("Thread 1 is executing task: Calculating factorial of 5...\n");
    int factorial = 1;
    for (int i = 1; i <= 5; i++) {
        factorial *= i;
    }
    printf("Thread 1 finished task: Factorial = %d\n", factorial);

    current_thread++;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {

    int rows ,cols;
    printf("Enter the number of pairs to be generated \n");
    scanf("%d",&rows);

    cols = 5;


    array = (float**)calloc(rows,sizeof(float*));
    for(int i = 0;i<rows;i++){
        array[i] = (float *)calloc(cols,sizeof(float));
    }

    pthread_t threads[NUM_THREADS];

    ====// Create Threads
    pthread_create(&threads[1], NULL, thread_task_1, NULL);
    pthread_create(&threads[2], NULL, thread_task_2, NULL);
    pthread_create(&threads[3], NULL, thread_task_3, NULL);

    cnt = 0;
while(cnt < rows){

    current_thread = 0;
    pthread_mutex_lock(&mutex);

    while(current_thread != 0){
        pthread_cond_wait(&cond,&mutex);
    }

    int randX = rand() % 10;
    int randY = rand() % 10;

    X = randX;
    Y = randY;

    // Allow the first thread to proceed
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);

}

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
