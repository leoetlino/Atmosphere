#include <mutex>
#include <switch.h>
#include <stratosphere.hpp>
#include "fsmitm_worker.hpp"

/* Owned by g_worker_waiter. */
static SystemEvent *g_new_waitable_event = NULL;

static HosMutex g_new_waitable_mutex;
static HosSemaphore g_sema_new_waitable_finish;

static std::unique_ptr<WaitableManager> g_worker_waiter;

Result FsMitMWorker::AddWaitableCallback(void *arg, Handle *handles, size_t num_handles, u64 timeout) {
    (void)arg;
    svcClearEvent(handles[0]);
    g_sema_new_waitable_finish.Signal();
    return 0;
}

void FsMitMWorker::AddWaitable(std::unique_ptr<IWaitable> waitable) {
    g_worker_waiter->add_waitable(std::move(waitable));
    std::scoped_lock lk{g_new_waitable_mutex};
    g_new_waitable_event->signal_event();
    g_sema_new_waitable_finish.Wait();
}

void FsMitMWorker::Main(void *arg) {
    /* Initialize waitable event. */
    auto new_waitable_event = std::make_unique<SystemEvent>(nullptr, &FsMitMWorker::AddWaitableCallback);
    g_new_waitable_event = new_waitable_event.get();

    /* Make a new waitable manager. */
    g_worker_waiter = std::make_unique<WaitableManager>(U64_MAX);
    g_worker_waiter->add_waitable(std::move(new_waitable_event));
    
    /* Service processes. */
    g_worker_waiter->process();
}
