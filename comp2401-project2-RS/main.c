#include "defs.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void load_data(Manager *manager);

int main(void) {
    Manager manager;
    pthread_t manager_thread_id;
    pthread_t *system_threads;

    // Initialize the manager
    manager_init(&manager);

    // Load resources and systems
    load_data(&manager);

    // Allocate memory for system threads
    system_threads = malloc(sizeof(pthread_t) * manager.system_array.size);

    // Create the manager thread
    pthread_create(&manager_thread_id, NULL, manager_thread, &manager);

    // Create threads for each system
    for (int i = 0; i < manager.system_array.size; ++i) {
        pthread_create(&system_threads[i], NULL, system_thread, manager.system_array.systems[i]);
    }

    // Wait for the manager thread to finish
    pthread_join(manager_thread_id, NULL);

    // Wait for all system threads to finish
    for (int i = 0; i < manager.system_array.size; ++i) {
        pthread_join(system_threads[i], NULL);
    }

    // Free allocated memory for threads
    free(system_threads);

    // Clean up the manager
    manager_clean(&manager);

    return 0;
}