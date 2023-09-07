# Memory Allocator

This is made only for fun and learning purposes

### Compilation

```
$ c++ -o memalloc.so -fPIC -shared mmemory.cpp 
```

### Usage
```
$ export LD_PRELOAD=$PWD/memalloc.so 
```

### Reference Links
+ [Guide](https://arjunsreedharan.org/post/148675821737/memory-allocators-101-write-a-simple-memory)
+ [Wikipedia](https://en.wikipedia.org/wiki/Memory_management)