#ifndef INCLUDE_UTIL_H
#define INCLUDE_UTIL_H

#include <stdint.h>
#include <sys/types.h>

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

/**
 * Defines a new list struct for the specified item type
 * @param type Item type
 * @param name Struct's name
 */
#define DEFINE_LIST_STRUCT(type, name)                                                             \
    typedef struct name {                                                                          \
        type* items;                                                                               \
        size_t count;                                                                              \
        size_t alloced;                                                                            \
    } name

/**
 * Initializes a list defined with the DEFINE_LIST_STRUCT() macro
 * @see DEFINE_LIST_STRUCT()
 */
#define LIST_INITIALIZE(type) (type){ .items = NULL, .count = 0, .alloced = 0 }

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef unsigned char uchar;

/**
 * Adds an item to a list defined with the DEFINE_LIST_STRUCT() macro
 * @param list Target list
 * @param item_type The type used by items in the target list
 * @param item The item to be added
 * @param alloc_error_code Value to be returned if the allocation fails
 *
 * @see DEFINE_LIST_STRUCT()
 */
#define list_add(list, item_type, item, alloc_error_code)                                          \
    do {                                                                                           \
        if (list.count >= list.alloced) {                                                          \
            size_t newCapacity = list.alloced == 0 ? 8 : list.alloced * 2;                         \
            item_type* aux = list.items;                                                           \
            list.items = realloc(list.items, newCapacity * sizeof(item_type));                     \
            if (list.items == NULL) {                                                              \
                list.items = aux;                                                                  \
                return alloc_error_code;                                                           \
            }                                                                                      \
            list.alloced = newCapacity;                                                            \
        };                                                                                         \
        list.items[list.count++] = item;                                                           \
    } while (0)

#define list_free(list, type)                                                                      \
    do {                                                                                           \
        if (list.items != NULL)                                                                    \
            free(list.items);                                                                      \
        list = LIST_INITIALIZE(type);                                                              \
    } while (0);

#endif /* ifndef INCLUDE_UTIL_H */
