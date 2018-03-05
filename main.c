// Chad Cooper
// Project 3: Bounded Buffer


#define _REENTRANT
#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 15

// Thread functions
void *producer(void *write);
void *consumer(void *read);


// circular buffer defintion
typedef struct circ_buff{
    char* buffer; // character buffer
    size_t capacity; // the capacity of the buffer, which is 15 in this case
    size_t producerIndex; // the current index that the producer is on in the buffer
    size_t consumerIndex; // the current index that the consumer is on in the buffer
    size_t size;
} circular_buffer;

//%%%%%%%%%%%%%%%%%% Circular Buffer Prototypes %%%%%%%%%%%%%%%%%%

// Circuluar buffer initializer
void circ_buff_init(circular_buffer *charBuff, size_t capacity, size_t size);

// Free buffer space
void free_buffer(circular_buffer *charBuff);

// Add an item into the buffer
void addToBuffer(circular_buffer *charBuff, const char *item);

// Read the next item from the buffer
void readFromBuffer(circular_buffer *charBuff);


// Global variables: Buffer and semaphores
circular_buffer boundedBuffer;
sem_t mutex, empty, full;


int main(){
    
    // Initialize the global bounded buffer.
    circ_buff_init(&boundedBuffer, BUFFER_SIZE, sizeof(char));
    sem_init(&mutex, 1, 1);
    sem_init(&empty, 1, BUFFER_SIZE);
    sem_init(&full, 1, 0);
    
    // Useful commands for threads
    pthread_t tid1[1]; // process id for thread 1
    pthread_t tid2[1]; // process id for thread 2
    pthread_attr_t attr[1]; // attribute pointer array
    
    // Required to schedule thread independently.
    pthread_attr_init(&attr[0]);
    pthread_attr_setscope(&attr[0], PTHREAD_SCOPE_SYSTEM);
    // end to schedule thread independently
    
    // Create the threads
    pthread_create(&tid1[0], &attr[0], producer, NULL);
    pthread_create(&tid2[0], &attr[0], consumer, NULL);
    
    // Wait for the threads to finish
    pthread_join(tid1[0], NULL);
    pthread_join(tid2[0], NULL);
    
    printf("Threads have exited.");
    
    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    
    fflush(stdout);
    
    free_buffer(&boundedBuffer);
    
    return 0;
}

// The producer reads from the file and adds the characters to a buffer
void *producer(void *write){
    
    // Reading from a file 'mytest.dat'
    char newChar;
    FILE* fp;
    fp= fopen("mytest.dat", "r");
    fscanf(fp,"%c",&newChar); // Reads first character into buffer
    
    do {
        
        sem_wait(&empty);
        sem_wait(&mutex);
        
        addToBuffer(&boundedBuffer, &newChar);
        
        sem_post(&mutex);
        sem_post(&full);
        
    } while ((fscanf(fp,"%c",&newChar) != EOF) && (boundedBuffer.producerIndex < 150));
    
    fclose(fp);
    
    //printf("All characters have been written to buffer.\n");
    
    boundedBuffer.producerIndex--; // This will cause the consumer to stop once it "catches up" to the producer.
    
    pthread_exit(NULL);
}

// The consumer reads from the buffer and prints the items to the screen.
void *consumer(void *read){
    do {
    
        sem_wait(&full);
        sem_wait(&mutex);

        sleep(1); // Pause for 1 second
        readFromBuffer(&boundedBuffer);
        
        sem_post(&mutex);
        sem_post(&empty);
        
    } while (boundedBuffer.consumerIndex <= boundedBuffer.producerIndex);
    
    printf("All characters have been read from buffer.\n");
    
    pthread_exit(NULL);
}

void circ_buff_init(circular_buffer *charBuff, size_t capacity, size_t size){
    charBuff->buffer = malloc(capacity * size);
    charBuff->capacity = capacity;
    charBuff->consumerIndex = 0;
    charBuff->producerIndex = 0;
    charBuff->size = size;
}

void free_buffer(circular_buffer *charBuff){
    free(charBuff->buffer);
}

void addToBuffer(circular_buffer *charBuff, const char *item){
    *(charBuff->buffer + (charBuff->producerIndex % charBuff->capacity)) = *item;
    
    charBuff->producerIndex++;
}

void readFromBuffer(circular_buffer *charBuff){
    
    char currentCharacter = *(charBuff->buffer + (charBuff->consumerIndex % charBuff->capacity));
    
    printf("%c\n", currentCharacter);
    
    charBuff->consumerIndex++;
}


