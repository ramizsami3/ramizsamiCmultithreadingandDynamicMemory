#include "defs.h"
#include <stdlib.h>

void event_queue_init(EventQueue *queue) {
    queue->head = NULL;
    queue->size = 0;
    sem_init(&queue->mutex, 0, 1);  // Initialize semaphore for the event queue
}

void event_queue_clean(EventQueue *queue) {
    EventNode *current = queue->head;
    while (current) {
        EventNode *next = current->next;
        free(current);
        current = next;
    }
    sem_destroy(&queue->mutex);  // Destroy semaphore
}

void event_queue_push(EventQueue *queue, const Event *event) {
    sem_wait(&queue->mutex);  // Lock the queue

    EventNode *new_node = malloc(sizeof(EventNode));
    new_node->event = *event;
    new_node->next = NULL;

    if (!queue->head || queue->head->event.priority < event->priority) {
        new_node->next = queue->head;
        queue->head = new_node;
    } else {
        EventNode *current = queue->head;
        while (current->next && current->next->event.priority >= event->priority) {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }

    queue->size++;
    sem_post(&queue->mutex);  // Unlock the queue
}

int event_queue_pop(EventQueue *queue, Event *event) {
    sem_wait(&queue->mutex);  // Lock the queue

    if (!queue->head) {
        sem_post(&queue->mutex);
        return 0;
    }

    EventNode *node = queue->head;
    *event = node->event;
    queue->head = node->next;

    free(node);
    queue->size--;

    sem_post(&queue->mutex);  // Unlock the queue
    return 1;
}