#include "lock_pair.h"

lock_pair::lock_pair(lock_type mem1, lock_type mem2) : mems{mem1, mem2}
{

}

void lock_pair::init() const
{
    *mems[0] = 0;
    *mems[1] = 0;
}

bool lock_pair::try_lock(int which) const
{
    if(*mems[other(which)] != 0)
    {
        return false;
    }

    lfence(); // memory write flush command here. (such as lfence)
    *mems[which] = 1;
    lfence(); // memory write flush command here. (such as lfence)

    //read what we've written to avoid async writes
    if(*mems[which] == 0)
    {
        //write not applied yet, give up
        *mems[which] = 0;
        return false;
    }
    lfence();

    if(*mems[other(which)] != 0)
    {
        *mems[which] = 0;
        return false;
    }

    return true;
}

void lock_pair::unlock(int which) const
{
    *mems[which] = 0;
}

void lock_pair::lock(int which) const
{
    static constexpr unsigned int min_wait_cycles   = 1 * 300; //1-2 micro
    static constexpr unsigned int max_jitter_cycles = 4*256;   //~4-8 micros, must be binary
    while(!try_lock(which))
    {
        unsigned int jitter = XT_RSR_CCOUNT() & (max_jitter_cycles - 1);
        xos_thread_sleep(min_wait_cycles + jitter);
    }
}
