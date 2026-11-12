#include <stdio.h>
#include <unistd.h>
#include <pigpio.h>
#define PIN 17


void display_count(int count, int max_count) {
    printf("Current Occupancy Count: %d\n", count);
    printf("Occupancy Status: %s\n", (count < max_count) ? "Available" : "Full");
    //Prevents the occupancy count from exceeding the maximum count, and activates LED if the room is full
    if (count >= max_count) {
        printf("\nWARNING: The study room is at full capacity!\n");
        gpioWrite(17, 1);
    }
    else {
        gpioWrite(17, 0);
    }
    //Save details to a file for record-keeping
    FILE *file = fopen("occupancy_log.txt", "a");
    if (file) {
        //Tracks count changes and status changes in the occupancy log file
        fprintf(file, "Occupancy Count: %d\n", count);
        fprintf(file, "Occupancy Status: %s\n", (count < max_count) ? "Available" : "Full");
        fclose(file);
    }
}

int main() {
    gpioInitialise();
    gpioSetMode(PIN, PI_OUTPUT);
    //Initialize variables here
    int count = 0;
    int max_count = 0;
    int running = 1;
    

    printf("Welcome to the Study Room Tracker!\n");
    printf("Please enter the maximum occupancy count for the study room: ");
    scanf("%d", &max_count);

    //Main loop to track occupancy
    while (running == 1) {
        //Display current occupancy count
        display_count(count, max_count);

        //Function to allow user to add or remove occupants
        printf("Enter 1 to add an occupant, 2 to remove an occupant, 3 to change room maximum, or 0 to exit: ");
        int choice;
        scanf("%d", &choice);
        if (choice == 1) {
            count++;
        } else if (choice == 2) {
            count--;
        }else if (choice == 3) { 
            printf("Please enter the maximum occupancy count for the study room: ");
            scanf("%d", &max_count);
        }else if (choice == 0) {
            running = 0;
            printf("Exiting the Study Room Tracker. Goodbye!\n");
            break;
        } else {
            printf("Invalid choice. Please try again.\n");
            continue;
        }

        //Prevent occupancy from going below zero
        if (count <= 0) {
            printf("The study room is empty. No occupants can leave.\n");
            count = 0;
        } 
    }

    return 0;
}
