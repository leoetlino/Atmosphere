#pragma once
#include <memory>
#include <switch.h>
#include <stratosphere.hpp>

class FsMitMWorker {
    private:
        static Result AddWaitableCallback(void *arg, Handle *handles, size_t num_handles, u64 timeout);
    public:
        static void Main(void *arg);
        static void AddWaitable(std::unique_ptr<IWaitable> waitable);
};
