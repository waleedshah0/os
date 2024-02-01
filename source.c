#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h> 
#define NUM_STUDENTS 5
#define NUM_CHAIRS 3

sem_t ta_sem; // ta semaphore variable 
sem_t student_sem;  //student semaphore variable 
sem_t chair_sem[NUM_CHAIRS]; //for chairs semaphore


int ta_available = 1; //check that ta is avaibale or not
int waiting_student = 0; //student wait variable

void *student_thread(void *arg) {
    int student_id = *(int *)arg;

    while (1) {
     
        printf("Student %d working on assignment\n", student_id);
        sleep(5); // 5 work time
        //acquire a chair
        if (sem_trywait(&chair_sem[0]) == -1) {
            printf("Student %d: No chairs available, leaving\n", student_id);
            continue;
        }
        // Check if TA is available
        if (ta_available) {
            ta_available = 0;
            sem_post(&ta_sem); //TA free
            printf("Student %d: Requesting help from TA\n", student_id);
        } else {
            printf("Student %d: Joining queue, waiting for TA\n", student_id);
        }
        // Release the chair
        sem_post(&chair_sem[0]);
        // Wait for the TA to finish
        sem_wait(&student_sem);
        printf("Student %d: Receiving help from TA\n", student_id);
        sleep(4); //help 
    }

    pthread_exit(NULL);
}

void *ta_thread(void *arg) {
    while (1) {
        sem_wait(&ta_sem);// student wait
        printf("TA: Helping student\n");
        sem_post(&student_sem); // Signal the helped student
        // Check if any other students are waiting
        if (waiting_student > 0) {
            printf("TA: More students waiting\n");
        } else {
            ta_available = 1;
            printf("TA: Going back to sleep\n");
        }
    }

    pthread_exit(NULL);
}

int main() {
    int i;
    pthread_t s_threads[NUM_STUDENTS], ta_thread;
    int student_ids[NUM_STUDENTS];
    // Initialize semaphores
    sem_init(&ta_sem, 0, 0);
    sem_init(&student_sem, 0, 0);
    for (i = 0; i < NUM_CHAIRS; i++) {
        sem_init(&chair_sem[i], 0, 1);
    }
    // Create student threads
    for (i = 0; i < NUM_STUDENTS; i++) {
        student_ids[i] = i;
        pthread_create(&s_threads[i], NULL, student_thread, &student_ids[i]);
    }
    // Create TA thread
    pthread_create(&ta_thread, NULL, ta_thread, NULL);
    // Join all threads (optional)
    for (i = 0; i < NUM_STUDENTS; i++) {
        pthread_join(s_threads[i], NULL);
    }
    // Destroy semaphores
    sem_destroy(&ta_sem);//ta destroy
    sem_destroy(&student_sem);//student sem des
    for (i = 0; i < NUM_CHAIRS; i++) {
        sem_destroy(&chair_sem[i]);//chair destory
    }

    return 0;
} 
