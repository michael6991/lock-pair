# Lock Pair
## Lock pair for locking shared resources

This concept can be usefull when you have a SoC with multiple cores with access to a global shared memory such as SRAM.
Useful if you want 2 cores to share a simple lock mechanism (mutex) that can be accessed in the shared memory region.

Example usage:
```
// on both cores - declare the lock_pair object
static lock_pair pair((volatile uint32_t *)0x20800000, (volatile uint32_t *)0x20800004);

// only on one core - initialize the lock pair object using one core only
pair.init();

// later usage:
...
{
  // use in functions, locks on construction
  lock_handle hnd(pair, 0); //0 or 1

  // unlock manually
  hnd.unlock();

  // or on destruction
}
...
```
