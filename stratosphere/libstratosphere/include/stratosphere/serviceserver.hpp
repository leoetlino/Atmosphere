#pragma once
#include <switch.h>
#include "iserver.hpp"

template <typename T>
class ServiceServer : public IServer<T> {    
    public:
        ServiceServer(const char *service_name, unsigned int max_s, bool s_d = false) : IServer<T>(service_name, max_s, s_d) { 
            if (R_FAILED(smRegisterService(&this->port_handle, service_name, false, this->max_sessions))) {
                /* TODO: Panic. */
            }
        }
        
        std::unique_ptr<ISession<T>> get_new_session(Handle session_h) override {
            return std::make_unique<ServiceSession<T>>(this, session_h, 0);
        }
};
