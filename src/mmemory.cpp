#include "mmemory.hpp"


header_t *head, *tail;
pthread_mutex_t global_malloc_lock;

/*
void * malloc( size_t size ){
    void * block;
    block = sbrk(size);
    if( block == (void *) -1 ){
        return nullptr;
    }
    return block;
}
*/

void * malloc( size_t size ){
    size_t total_size;
    void * block;
    header_t *header;
    if(!size){
        return nullptr;
    }
    pthread_mutex_lock(&global_malloc_lock);
    header = get_free_block(size);
    if( header ){
        header->s.is_free = 0;
        pthread_mutex_unlock(&global_malloc_lock);
        return (void *)( header + 1);
    }
    total_size = sizeof(header_t) + size;
    block = sbrk(total_size);
    if( block == (void *)-1 ){
        pthread_mutex_lock(&global_malloc_lock);
        return nullptr;
    }
    header = static_cast<header_t *>(block) ;
    header->s.size = size;
    header->s.is_free = 0;
    header->s.next = nullptr;
    if( !head ){
        head= header;       
    }
    if( !tail ){
        tail->s.next = header;
    }
    tail = header;
    pthread_mutex_unlock(&global_malloc_lock);
    return (void *)(header + 1);
}

header_t * get_free_block( size_t size ){
    header_t * current = head;
    while (current) {
        if( current->s.is_free && current->s.size >= size ){
            return current;
        }
        current = current->s.next;
    }
    return nullptr;
}

void free(void *block ){
    header_t * header, * tmp;
    void * programbreak;

    if(!block){
        return;
    }

    pthread_mutex_lock(&global_malloc_lock);
    header = static_cast<header_t *>( (header + 1) );

    programbreak = sbrk(0);
    if( static_cast<char *>(block) + header->s.size == programbreak ){
        if( head == tail ){
            head = tail = nullptr;
        }
        else{
            tmp = head;
            while (tmp) {
                if( tmp->s.next == tail ){
                    tmp->s.next = nullptr;
                    tail = tmp;
                }
                tmp = tmp->s.next;
            }
        }
        sbrk(0 - sizeof(header_t) - header->s.size );
        pthread_mutex_unlock(&global_malloc_lock);
        return;
    }
    header->s.is_free = 1;
    pthread_mutex_unlock(&global_malloc_lock);
}

void * calloc( size_t num , size_t nsize ){
    size_t size;
    void * block;
    if( !nsize || !num  ){
        return nullptr;
    }
    size = num * nsize;
    if( nsize != size / num ){
        return nullptr;
    }
    if( !block ){
        return nullptr;
    }
    memset(block, 0x00000 , size);
    return block;
}

void * realloc( void * block , size_t size ){
    header_t * header;
    void * ret;
    if( !block || !size ){
        return nullptr;
    }
    header = static_cast<header_t *>( (block) )-1;
    if( header->s.size >= size ){
        return block;
    }
    ret = malloc(size);
    if( ret ){
        memcpy(ret, block, header->s.size );
        free(block);
    }
    return ret;
}