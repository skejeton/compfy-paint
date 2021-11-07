// Callback dispatching
#pragma once
#include <cstddef>
#include <cstdio>
#include <cstdlib>

#define SELFARG void *self______________________
#define SELFBIND auto self = decltype(this)(self______________________)

template <class R, class ...Args>
struct Selfcall
{
    void *subject = nullptr;
    R (*callback)(void *subject, Args...) = nullptr;
    Selfcall(){}
    Selfcall(void *subject, R (*callback)(void *subject, Args...))
    {
        this->subject = subject;
        this->callback = callback;
    }

    inline bool
    valid()
    {
        return callback != nullptr;
    }

    inline R
    operator()(Args... args)
    {
        return callback(subject, args...);
    }
};

template <class ...Args>
struct Dispatch {
    Selfcall<void, Args...> callbacks[16];
    size_t callback_count = 0;


    void subscribe(void *subject, void (*callback)(void *t, Args...))
    {
        if (callback_count == 16)
        {
            fprintf(stderr, "Callback subscription limit exceeded, your program is now ded");
            exit(-1);
        }
        callbacks[callback_count++] = {
            subject,
            callback
        };
    }

    void operator()(Args... args)
    {
        for (size_t i = 0; i < callback_count; i += 1)
        {
            callbacks[i](args...);
        }
    }
};
