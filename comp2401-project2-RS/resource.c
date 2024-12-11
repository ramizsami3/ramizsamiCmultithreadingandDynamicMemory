#include "defs.h"
#include <stdlib.h>
#include <string.h>

void resource_create(Resource **resource, const char *name, int amount, int max_capacity) {
    *resource = malloc(sizeof(Resource));
    (*resource)->name = strdup(name);
    (*resource)->amount = amount;
    (*resource)->max_capacity = max_capacity;
    sem_init(&(*resource)->mutex, 0, 1);  // Initialize semaphore for resource
}

void resource_destroy(Resource *resource) {
    free(resource->name);
    sem_destroy(&resource->mutex);  // Destroy semaphore
    free(resource);
}

void resource_array_init(ResourceArray *array) {
    array->resources = malloc(sizeof(Resource *) * 1);
    array->size = 0;
    array->capacity = 1;
}

void resource_array_clean(ResourceArray *array) {
    for (int i = 0; i < array->size; ++i) {
        resource_destroy(array->resources[i]);
    }
    free(array->resources);
}

void resource_array_add(ResourceArray *array, Resource *resource) {
    if (array->size == array->capacity) {
        array->capacity *= 2;
        Resource **new_resources = malloc(sizeof(Resource *) * array->capacity);
        for (int i = 0; i < array->size; ++i) {
            new_resources[i] = array->resources[i];
        }
        free(array->resources);
        array->resources = new_resources;
    }
    array->resources[array->size++] = resource;
}

