#include "lock_pair.h"

lock_pair::lock_pair(lock_type mem1, lock_type mem2) : mems{mem1, mem2} { }

void lock_pair::init() const {
  *mems[0] = 0;
  *mems[1] = 0;
}

bool lock_pair::try_lock(int which) const {
  if(*mems[other(which)] != 0) {
      return false;
  }

  lfence(); // memory write flush command here. (such as lfence on x86)
  *mems[which] = 1;
  lfence(); // memory write flush command here. (such as lfence)

  // read what we've written to avoid async writes
  if(*mems[which] == 0) {
      // write not applied yet, give up
      *mems[which] = 0;
      return false;
  }
  lfence();

  if(*mems[other(which)] != 0) {
      *mems[which] = 0;
      return false;
  }

  return true;
}

void lock_pair::unlock(int which) const {
  *mems[which] = 0;
}

void lock_pair::lock(int which) const {
    unsigned int min_wait_cycles   = 1 * 300;  // 1-2 us - depends on the core
    unsigned int max_jitter_cycles = 4 * 256;  // ~4-8 us, must be binary
  
    while(!try_lock(which)) {
      unsigned int jitter = RSR_CCOUNT() & (max_jitter_cycles - 1);
      sleep(min_wait_cycles + jitter);
  }
}
