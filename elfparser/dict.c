// https://gist.github.com/kylef/86784/fe97567ec9baf5c0dce3c7fcbec948e21dfcce09
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct dict_t_struct {
    int key;
    void *value;
    struct dict_t_struct *next;
} dict_t;

dict_t **dictAlloc(void) {
    return malloc(sizeof(dict_t));
}

void dictDealloc(dict_t **dict) {
    free(dict);
}

void *getItem(dict_t *dict, int key) {
    dict_t *ptr;
    for (ptr = dict; ptr != NULL; ptr = ptr->next) {
        if (ptr->key == key) {
            return ptr->value;
        }
    }
    return "Not set";
}

void dict_delItem(dict_t **dict, int key) {
    dict_t *ptr, *prev;
    for (ptr = *dict, prev = NULL; ptr != NULL; prev = ptr, ptr = ptr->next) {
        if (ptr->key == key) {
            if (ptr->next != NULL) {
                if (prev == NULL) {
                    *dict = ptr->next;
                } else {
                    prev->next = ptr->next;
                }
            } else if (prev != NULL) {
                prev->next = NULL;
            } else {
                *dict = NULL;
            }
            free(ptr);
            return;
        }
    }
}

void addItem(dict_t **dict, int key, void *value) {
    dict_delItem(dict, key); /* If we already have a item with this key, delete it. */
    dict_t *d = malloc(sizeof(struct dict_t_struct));
    d->key = key;
    d->value = value;
    d->next = *dict;
    *dict = d;
}
