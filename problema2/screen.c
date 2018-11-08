// gcc screen.c -lpthread -o screen && ./screen
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

void *screen(void *param);
void *student(void *param);

int helping = 0; // 0 = slepping, another is helping the student
int finished = 0;
int nStudents = 0;
int *queue;
pthread_mutex_t mutex;

int main(int argc, char *argv[]) {
    // Init of Mutex
	pthread_mutex_init(&mutex, NULL);
    // Lock Mutex to show first the Screen
	pthread_mutex_lock(&mutex);
	srand(time(NULL));
	// Create the number of Students between 3 and 40
	nStudents = (rand() % 38) + 3;
    nStudents = 5;
	// Create the numbers of chairs on queue
	queue = (int*) calloc(nStudents/2, sizeof(int));
	// Show the number of Students that came
	printf("Number of Students: %d\n", nStudents);
    // Show the initial Queue
    printf("Queue: ");
    for (int i = 0; i < nStudents/2; i++) {
        printf("%d ", queue[i]);
    }
    printf("\n\n");
	int number[nStudents];
	pthread_t tid[1 + nStudents];
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	// Run screen
	pthread_create(&tid[0], &attr, screen, NULL);
	// Run students
	for (int i = 1; i < 1 + nStudents; i++) {
		number[i - 1] = i;
		pthread_create(&tid[i], &attr, student, &number[i - 1]);
	}
	// Join all threads
	for (int i = 1; i < 1 + nStudents; i++) {
		pthread_join(tid[i], NULL);
	}
	finished = 1;
	pthread_join(tid[0], NULL);
	return 0;
}

void *screen(void *param) {
	printf("I am the AE\n");
    // Unlock the Screening Room
	pthread_mutex_unlock(&mutex);
	do {
		int actual = helping;
        // Verify if the screen isn't helping anyone
		if (actual == 0) {
            pthread_mutex_lock(&mutex);
            // Verify if someone is on the queue to try to sleep
            if (queue[0] == 0) {
                printf("I am the AE and I am slepping\n");
            } else {
                printf("I am the AE an I am going to help Student %d\n", queue[0]);
            }
            pthread_mutex_unlock(&mutex);
            // Stop to print that Screen was sleeping
	 		while (helping == actual && finished == 0) {}
        // If someone is on the queue, help
		} else {
			pthread_mutex_lock(&mutex);
			printf("I am the AE and I am helping the Student %d\n", actual);
			pthread_mutex_unlock(&mutex);
            // Stop to print that Screen was helping the same student
			while (helping == actual && finished == 0) {}
		}
	} while (finished == 0);
    printf("I am a AE and I finished my work\n"); //work = screening
	pthread_exit(NULL);
}

void *student(void *param) {
	int *i = (int*) param;
    // Lock to show first the Screen
	pthread_mutex_lock(&mutex);
	printf("I am the Student %d\n", *i);
	pthread_mutex_unlock(&mutex);
	int helps = 0;
	do {
        // Sleep to doesn't help the same student 3 consecutive times
		sleep(rand() % 3);
		// Lock the Mutex
		pthread_mutex_lock(&mutex);
        // Verify if the student is the first on the queue
		if (queue[0] == *i) {
            if (helping == 0) {
                printf("I am Student %d and I woke up the AE\n", *i);
            }
			helping = *i;
            // Show the queue
			printf("\nQueue: ");
			for (int i = 0; i < nStudents/2; i++) {
				printf("%d ", queue[i]);
			}
			printf("\n");
			// Screen helps the Student
			helps ++;
			printf("I am Student %d and the AE is helping me for the %d time.\n", *i, helps);
            // Move the queue
            for (int j = 0; j < (nStudents/2) - 1; j++) {
                queue[j] = queue[j + 1];
            }
            queue[(nStudents/2) - 1] = 0;
            // Show the att queue
            printf("Queue: ");
            for (int i = 0; i < nStudents/2; i++) {
                printf("%d ", queue[i]);
            }
            printf("\n\n");
			helping = queue[0];
		} else {
			// Verify if it is on the queue
			int isOnQueue = 0;
			for (int j = 0; j < nStudents/2; j++) {
				if (queue[j] == *i) {
					isOnQueue = 1;
				}
			}
			if (!isOnQueue) {
				int full = 1;
				// Try to enter on queue
				for (int j = 0; j < nStudents/2; j++) {
                    // Enter on queue
					if (queue[j] == 0) {
						queue[j] = *i;
						printf("I am Student %d and I am sitting on the chair number %d\n", *i, j + 1);
						full = 0;
						break;
					}
				}
                // Verify if the queue was full
				if (full) {
					printf("I am Student %d and I am programming\n", *i);
					sleep(rand() % 3);
				}
			}
		}
		// Unlock the Mutex
		pthread_mutex_unlock(&mutex);
	} while(helps < 3);
	printf("I am Student %d and I finished my programm\n\n", *i);
	pthread_exit(NULL);
}
