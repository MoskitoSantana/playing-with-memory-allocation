#include <cstddef>
#include <cstring>
#include <pthread.h>
#include <type_traits>
#include <unistd.h>

typedef char ALIGN[0x10] ;

union header {
    struct {
        size_t size;
        unsigned is_free;
        union header *next;
    } s;

    ALIGN stub;

}; typedef union header header_t ;

void * malloc( size_t );
void free(void *);
void * calloc( size_t  , size_t  );
void * realloc(void *  , size_t );

header_t *get_free_block( size_t size );