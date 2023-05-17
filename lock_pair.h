#pragma once

class lock_pair
{
public:
    using lock_type = volatile uint32_t *;

    static constexpr int other(int which)
    {
        return which == 0 ? 1 : 0;
    }

    lock_pair(lock_type mem1, lock_type mem2);
    constexpr lock_pair(uintptr_t mem1, uintptr_t mem2) : uint_mems{mem1, mem2}
    {}

    //const so lock_pair can be constexpr
    void init() const;
    bool try_lock(int which) const;
    void unlock(int which) const;
    void lock(int which) const;

private:
    union {
        lock_type mems[2];
        uintptr_t uint_mems[2];
    };
};

template<bool managed>
class lock_handle
{
public:
    lock_handle(const lock_pair & pair, int which) : pair(pair), which(which)
    {
        if (managed)
        {
            lock();
        }
    }
    lock_handle(const lock_handle &) = delete;
    lock_handle(lock_handle && other) : pair(other.pair), which(other.which), locked(other.locked), active(other.active)
    {
        other.active = false;
        other.locked = false;
    }
    void lock()
    {
        if(!locked && active)
        {
            pair.lock(which);
            locked = true;
        }
    }

    bool try_lock()
    {
        if(!locked && active)
        {
            locked = pair.try_lock(which);
        }
        return locked;
    }

    void unlock()
    {
        if(locked && active)
        {
            pair.unlock(which);
            locked = false;
        }
    }
    ~lock_handle()
    {
        if (managed)
        {
            unlock();
        }
    }

private:
    const lock_pair & pair;
    int which;
    bool locked = false;
    bool active = true;
};
