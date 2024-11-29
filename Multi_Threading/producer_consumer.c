#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_QUEUE_SIZE 10

// Global variables
int queue_size = 0;
int producer_delay = 0; // Producer delay in seconds
int consumer_delay = 0; // Consumer delay in seconds

pthread_mutex_t mutex;          // Mutex to protect shared data
pthread_cond_t cond_full;       // Condition variable to signal when the queue is full
pthread_cond_t cond_empty;      // Condition variable to signal when the queue is empty

// Producer thread function
void *producer(void *arg) {
    while (1) {
        sleep(producer_delay);

        pthread_mutex_lock(&mutex);

        // Wait if the queue is full
        while (queue_size == MAX_QUEUE_SIZE) {
            printf("Queue is full. Producer is waiting...\n");
            pthread_cond_wait(&cond_full, &mutex);
        }

        // Produce a bottle and increase the queue size
        queue_size++;
        printf("Produced a bottle. Queue size: %d\n", queue_size);

        // Signal the consumer that there is a bottle to consume
        pthread_cond_signal(&cond_empty);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

// Consumer thread function
void *consumer(void *arg) {
    while (1) {
        sleep(consumer_delay);

        pthread_mutex_lock(&mutex);

        // Wait if the queue is empty
        while (queue_size == 0) {
            printf("Queue is empty. Consumer is waiting...\n");
            pthread_cond_wait(&cond_empty, &mutex);
        }

        // Consume a bottle and decrease the queue size
        queue_size--;
        printf("Consumed a bottle. Queue size: %d\n", queue_size);

        // Signal the producer that there is space in the queue
        pthread_cond_signal(&cond_full);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    // Initialize mutex and condition variables
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_full, NULL);
    pthread_cond_init(&cond_empty, NULL);

    // Take user input for delays
    printf("Enter producer delay (in seconds): ");
    scanf("%d", &producer_delay);
    printf("Enter consumer delay (in seconds): ");
    scanf("%d", &consumer_delay);

    // Create producer and consumer threads
    pthread_t producer_thread, consumer_thread;

    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    // Wait for threads to finish (though the threads will run indefinitely in this example)
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    // Clean up
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_full);
    pthread_cond_destroy(&cond_empty);

    return 0;
}
