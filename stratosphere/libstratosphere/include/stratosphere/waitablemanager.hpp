#pragma once
#include <switch.h>
#include <algorithm>
#include <memory>
#include <vector>

#include "waitablemanagerbase.hpp"
#include "iwaitable.hpp"
#include "hossynch.hpp"

class IWaitable;

class WaitableManager : public WaitableManagerBase {
    protected:
        std::vector<std::unique_ptr<IWaitable>> to_add_waitables;
        std::vector<std::unique_ptr<IWaitable>> waitables;
        u64 timeout = 0;
        HosMutex lock;
        std::atomic_bool has_new_items = false;
    private:
        void process_internal(bool break_on_timeout);
    public:
        WaitableManager(u64 t) : timeout(t) { }

        virtual void add_waitable(std::unique_ptr<IWaitable> waitable);
        virtual void process();
        virtual void process_until_timeout();
};
