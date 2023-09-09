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

### Reference Link
+ [Wikipedia](https://en.wikipedia.org/wiki/Memory_management)
