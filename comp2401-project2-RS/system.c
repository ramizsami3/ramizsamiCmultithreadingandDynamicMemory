#include "defs.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void system_create(System **system, const char *name, ResourceAmount consumed, ResourceAmount produced, int processing_time, EventQueue *event_queue) {
    *system = malloc(sizeof(System));
    (*system)->name = strdup(name);
    (*system)->consumed = consumed;
    (*system)->produced = produced;
    (*system)->amount_stored = 0;
    (*system)->processing_time = processing_time;
    (*system)->status = STANDARD;
    (*system)->event_queue = event_queue;
}

void system_destroy(System *system) {
    free(system->name);
    free(system);
}

void system_array_init(SystemArray *array) {
    array->systems = malloc(sizeof(System *) * 1);
    array->size = 0;
    array->capacity = 1;
}

void system_array_clean(SystemArray *array) {
    for (int i = 0; i < array->size; ++i) {
        system_destroy(array->systems[i]);
    }
    free(array->systems);
}

void system_array_add(SystemArray *array, System *system) {
    if (array->size == array->capacity) {
        array->capacity *= 2;
        System **new_systems = malloc(sizeof(System *) * array->capacity);
        for (int i = 0; i < array->size; ++i) {
            new_systems[i] = array->systems[i];
        }
        free(array->systems);
        array->systems = new_systems;
    }
    array->systems[array->size++] = system;
}

void *system_thread(void *arg) {
    System *system = (System *)arg;

    while (system->status != TERMINATE) {
        system_run(system);
    }

    return NULL;
}