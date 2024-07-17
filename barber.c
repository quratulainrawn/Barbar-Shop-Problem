#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

void barber();
void customer();

sem_t seatMutex;
sem_t customers;
sem_t smfBarber;

int runningTime;
int numberOfFreeSeats;
int customersCount;
int customerWait;
int getHCn;
int customerNumber = 1;

pthread_t barberThread;
pthread_t customersThreads[20];

int main() {
    printf("Enter the running time of the program: ");
    scanf("%d", &runningTime);
    printf("Enter the number of free seats: ");
    scanf("%d", &numberOfFreeSeats);
    printf("Enter the customers count: ");
    scanf("%d", &customersCount);
    printf("Enter the maximum waiting time for the customer to come again: ");
    scanf("%d", &customerWait);

    getHCn = 0;

    printf("\nProgram is beginning\n\n");

    sem_init(&seatMutex, 0, 1);
    sem_init(&customers, 0, 0);
    sem_init(&smfBarber, 0, 0);

    pthread_create(&barberThread, NULL, barber, NULL);
    printf("Barber has been created.\n");

    for (int i = 0; i < customersCount; i++) {
        pthread_create(&customersThreads[i], NULL, customer, NULL);
        customerNumber++;
    }

    sleep(runningTime);

    printf("\n\nEnd of the day :)\n");
    printf("%d out of %d customers get a haircut.", getHCn, customersCount);
    exit(0);
}

void barber() {
    int workingTime;

    while (1) {
        sem_wait(&customers);
        sem_wait(&seatMutex);

        numberOfFreeSeats += 1;

        workingTime = (rand() % 5) + 1;
        printf("Barber took a new customer, and he will take %d seconds for a haircut.\n", workingTime);

        printf("\tNumber of free seats: %d\n", numberOfFreeSeats);

        sem_post(&smfBarber);
        sem_post(&seatMutex);
        sleep(workingTime);
    }
}

void customer() {
    int waitingTime;
    int notEnd = 1;
    int myNumber = customerNumber;

    while (notEnd == 1) {
        sem_wait(&seatMutex);

        if (numberOfFreeSeats <= 0) {
            waitingTime = (rand() % customerWait) + 1;
            printf("Customer %d left without a haircut and will come back after %d seconds to try again.\n", myNumber, waitingTime);
            sem_post(&seatMutex);

            sleep(waitingTime);
        } else {
            numberOfFreeSeats -= 1;
            printf("Customer %d is waiting.\n", myNumber);
            printf("\tNumber of free seats: %d\n", numberOfFreeSeats);

            sem_post(&customers);
            sem_post(&seatMutex);
            sem_wait(&smfBarber);
            printf("Customer %d got a haircut :)\n", myNumber);
            notEnd = 0;
            getHCn += 1;
        }
    }
}