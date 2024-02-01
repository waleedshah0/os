#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define size 10

int eCount = 0;//for even count
int oCount = 0;//for odd count
int even_numbers[size];//for save even numbers
int odd_numbers[size];//for save odd numbers
// Function for even numbers
void *p1(void *arg) {
    int *num = (int *)arg;

    for (int i = 0; num[i] != -1; i++) {
        if (num[i] % 2 == 0) {
            eCount++;
            even_numbers[eCount] = num[i];// save in even array
        }
    }
    //print even Values
    printf("The even numbers are: ");
    for (int i = 0; i < eCount; i++) {
        printf("%d ", even_numbers[i]);
    }
    printf("\n");
    pthread_exit(NULL);
}

// Function for odd numbers
void *p2(void *arg) {
    int *num1 = (int *)arg;

    for (int i = 0; num1[i] != -1; i++) {
        if (num1[i] % 2 != 0) {
            oCount++;
            odd_numbers[oCount] = num1[i];//save in odd array
        
        }
    }
    // Print odd Values
     printf("The odd numbers are: ");
    for (int i = 0; i < oCount; i++) {
        printf("%d ", odd_numbers[i]);
    }
    printf("\n");
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    pthread_t t1, t2;
    int number[size];
    int i = 0;
    // Check if enough arguments are provided
    if (argc < 2) {
        printf("Usage: %s <list of numbers separated by spaces>\n", argv[0]);
        return 1;
    }
    // Parse numbers from command line arguments
    for (int j = 1; j < argc && i < size - 1; j++) {
        number[i++] = atoi(argv[j]);
    }
    // Mark end of numbers with -1
    number[i] = -1;
    // Create even thread
    pthread_create(&t1, NULL, p1, number);
    // Create odd thread
    pthread_create(&t2, NULL, p2, number);
    // Wait for even thread to finish
    pthread_join(t1, NULL);
    // Wait for odd thread to finish
    pthread_join(t2, NULL);

    

    return 0;
}
