#pragma once

#include "request.hpp"
#include "error.hpp"
#include "loop.hpp"

namespace uvpp
{
    class Idle : public handle<uv_idle_t>
    {
    public:        
        Idle(std::function<void()> callback):
            handle<uv_idle_t>(), loop_(uv_default_loop())
        {
            init(loop_, callback);
        }
            
        Idle(loop& l, std::function<void()> callback):
            handle<uv_idle_t>(), loop_(l.get())
        {
            init(loop_, callback);
        }
        
        error start()
        {
            return error(uv_idle_start(get(), [](uv_idle_t* req)
            {
                callbacks::invoke<std::function<void()>>(req->data, internal::uv_cid_idle);
            }));
            
        }
        
        error stop()
        {
            return error(uv_idle_stop(get()));
        }

    private:
        
        void init(uv_loop_t *loop, std::function<void()> callback)
        {
            callbacks::store(get()->data, internal::uv_cid_idle, callback);
            uv_idle_init(loop, get());
        }
        
    	uv_loop_t *loop_;  	       	
    };
}



