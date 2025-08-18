# Lock Pair
## A Simple mutual exclusion mechanism to control access to a shared resource.
### Useful when two or more physical cores share memory space.

This concept can be usefull when you have an embedded system with SoC that contains multiple cores that can access to a global shared memory such as SRAM.
Useful when you want two or more cores to share a simple lock mechanism that can be accessed via a shared memory region.
If the physical cores don't have any hardware locking alternative built in the SoC, lock pair can be useful.

Example usage:
```
// on both cores - declare the lock_pair object
static lock_pair pair((volatile uint32_t *)0x20800000, (volatile uint32_t *)0x20800004);

// only on one core - initialize the lock pair object using one core only
pair.init();

// later usage with handles to the lock pair itself:
...
{
  // use in functions, locks on construction
  lock_handle lh(pair, 0); //0 or 1

  // unlock manually
  lh.unlock();

  // or on destruction
}
...
```
