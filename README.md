# Lock Pair
## Lock pair for locking shared resources

Example usage:
//on both cores
static lock_pair pair((volatile uint32_t *)0x20800000, (volatile uint32_t *)0x20800004);
//only on one core
pair.init();

{
//use in functions, locks on construction
lock_handle hnd(pair, 0); //0 or 1

//unlock manually
hnd.unlock();

//or on destruction
}
