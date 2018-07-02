#include <memory>
#include <switch.h>
#include <stratosphere.hpp>
#include "pm_process_track.hpp"
#include "pm_registration.hpp"

void ProcessTracking::MainLoop(void *arg) {
    /* Make a new waitable manager. */
    auto process_waiter = std::make_unique<MultiThreadedWaitableManager>(1, U64_MAX);
    process_waiter->add_waitable(std::unique_ptr<IWaitable>{Registration::GetProcessLaunchStartEvent()});
    Registration::SetProcessListManager(process_waiter.get());
    
    /* Service processes. */
    process_waiter->process();
    
    svcExitThread();
}
