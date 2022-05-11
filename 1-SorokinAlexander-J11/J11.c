#define _CRT_SECURE_NO_WARNINGS

#include <inttypes.h> // uint32_t, uint8_t
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 500000
#define TRUE 1
#define FALSE 0

typedef struct chains {
    char* value;
    struct chains* next;
} chains;

chains* chain_search(chains* chain, char* str);
chains* chain_remove(chains* chain, char* str);
chains* chain_add(chains* chain, char* str);
void chain_print(chains* chain);
void chain_destroy(chains* chain);
int chain_count(chains* chain);

typedef struct hash_map_items {
    char is_filled;
    chains* chain;
}hash_map_items;

typedef struct hash_map {
    hash_map_items* items;
    uint32_t size;
}hash_map;

hash_map* map_init(hash_map* map);
void map_add(hash_map* map, char* str);
void map_remove(hash_map* map, char* str);
char map_search(hash_map* map, char* str);
void map_destroy(hash_map* map);
void map_print(hash_map* map);
char check_collision(hash_map* map, char* str);
void create_CRC_table(void);
unsigned int get_CRC32(void* buffer, unsigned int length);
int buffer_length(char* buffer);
unsigned int CRC32_table[256];

hash_map* map_init(hash_map* map) {
    if (map == NULL) {
        map = (hash_map*)malloc(sizeof(hash_map));

        if (map == NULL)
            return NULL;

        map->size = SIZE;
        hash_map_items* tmp_items = (hash_map_items*)malloc(map->size * sizeof(hash_map_items));

        if (tmp_items == NULL)
            return NULL;

        map->items = tmp_items;

        for (int i = 0; i < map->size; i++) {
            map->items[i].is_filled = FALSE;
            map->items[i].chain = NULL;
        }

        create_CRC_table();

        return map;
    }

    return NULL;
}

int buffer_length(char* buffer) {
    int len = 0;

    if (buffer == NULL)
        return 0;

    while (buffer[len] != '\0')
        len++;

    return len;
}

void map_add(hash_map* map, char* string) {
    if (map != NULL && map->items != NULL && string != NULL) {
        if (map_search(map, string)) {
            return;
        }

        uint32_t hash = get_CRC32(string, buffer_length(string));
        uint32_t index = hash % SIZE;

        map->items[index].chain = chain_add(map->items[index].chain, string);
        map->items[index].is_filled = TRUE;
    }
}

int chain_count(chains* chain) {
    int count = 0;

    if (chain == NULL)
        return count;

    while (chain != NULL) {
        count++;
        chain = chain->next;
    }

    return count;
}

void map_remove(hash_map* map, char* str) {
    if (map != NULL && map->items != NULL && str != NULL) {
        if (!map_search(map, str))
            return;

        uint32_t hash = get_CRC32(str, buffer_length(str));
        uint32_t index = hash % SIZE;

        map->items[index].chain = chain_remove(map->items[index].chain, str);

        if (chain_count(map->items[index].chain) == 0)
            map->items[index].is_filled = FALSE;
    }
}

char check_collision(hash_map* map, char* str) {
    if (map != NULL && map->items != NULL && str != NULL) {
        uint32_t hash = get_CRC32(str, buffer_length(str));
        uint32_t index = hash % SIZE;

        if (map->items[index].is_filled == FALSE)
            return FALSE;

        if (map->items[index].is_filled && strcmp(map->items[index].chain->value, str) != 0)
            return TRUE;
    }

    return FALSE;
}

char map_search(hash_map* map, char* str) {
    if (map != NULL && map->items != NULL && str != NULL) {
        uint32_t hash = get_CRC32(str, buffer_length(str));
        uint32_t index = hash % SIZE;

        if (map->items[index].is_filled == FALSE)
            return FALSE;

        if (chain_search(map->items[index].chain, str) != NULL)
            return TRUE;
    }

    return FALSE;
}

void map_destroy(hash_map* map) {
    if (map == NULL) {
        return;
    }
    else {
        for (int i = 0; i < map->size; i++)
            if (map->items[i].is_filled)
                chain_destroy(map->items[i].chain);

        free(map->items);
    }

    free(map);
}

void map_print(hash_map* map) {
    if (map != NULL)
        for (int i = 0; i < map->size; i++)
            if (map->items[i].is_filled)
                chain_print(map->items[i].chain);
}

void create_CRC_table(void) {
    uint32_t CRC_32;
    unsigned int index = 0;
    unsigned int bit = 0;

    for (index = 0; index < 256; index++) {
        CRC_32 = index;

        for (bit = 0; bit < 8; bit++)
            if (CRC_32 & 1)
                CRC_32 = (CRC_32 >> 1) ^ (0xEDB88320);
            else
                CRC_32 = CRC_32 >> 1;

        CRC32_table[index] = CRC_32;
    }
}

unsigned int get_CRC32(void* buffer, unsigned int buffer_length) {
    uint32_t CRC32_value = 0xffffffff;
    unsigned char* tmp_buffer = (unsigned char*)buffer;

    while (buffer_length--)
        CRC32_value = CRC32_table[(CRC32_value ^ *tmp_buffer++) & 0xff] ^ (CRC32_value >> 8);

    return (CRC32_value ^ 0xffffffff);
}

chains* chain_search(chains* chain, char* str) {
    if (chain == NULL)
        return NULL;

    while (chain != NULL) {
        if (strcmp(chain->value, str) == 0)
            return chain;

        chain = chain->next;
    }

    return NULL;
}

chains* chain_add(chains* chain, char* str) {
    if (chain == NULL) {
        chains* newSlot = (chains*)malloc(sizeof(chains));

        if (newSlot != NULL)
            chain = newSlot;

        chain->value = (char*)malloc(sizeof(char) * buffer_length(str));

        if (chain->value != NULL)
            strcpy(chain->value, str);

        chain->next = NULL;

        return chain;
    }
    else {
        chains* tmp = chain;

        while (tmp->next != NULL)
            tmp = tmp->next;

        chains* newSlot = (chains*)malloc(sizeof(chains));
        newSlot->value = (char*)malloc(sizeof(char) * buffer_length(str));

        if (newSlot != NULL && newSlot->value != NULL) {
            tmp->next = newSlot;
            newSlot->next = NULL;
            strcpy(newSlot->value, str);
        }

        return chain;
    }
}

chains* chain_remove(chains* chain, char* str) {
    if (chain == NULL)
        return NULL;

    chains* removeSlot = chain_search(chain, str);

    if (chain == removeSlot) {
        chain = chain->next;
        free(removeSlot);

        return chain;
    }
    else {
        chains* tmp = chain;

        while (tmp->next != removeSlot)
            tmp = tmp->next;

        tmp->next = removeSlot->next;
        free(removeSlot->value);
        free(removeSlot);
    }

    return chain;
}

void chain_destroy(chains* chain) {
    chains* remove = chain;

    while (chain != NULL) {
        chain = remove->next;
        free(remove);
        remove = chain;
    }
}

void chain_print(chains* chain) {
    if (chain == NULL)
        return;

    while (chain != NULL) {
        printf("%s ", chain->value);
        chain = chain->next;
    }
}

int main(void) {
    char command;
    hash_map* map = NULL;
    map = map_init(map);
    char value[100] = { '\0' };

    while (scanf("%c", &command) >= 1) {
        if (command != 'p' && command != 'q')
            getchar();

        fgets(value, sizeof(value), stdin);

        switch (command) {
        case 'a':
            map_add(map, value);
            break;

        case 'r':
            map_remove(map, value);
            break;

        case 'f':
            if (map_search(map, value))
                puts("yes");
            else
                puts("no");

            break;

        case 'p':
            map_print(map);
            putchar('\n');
            break;

        case 'q':
            map_destroy(map);
            return 0;
        }
    }
}