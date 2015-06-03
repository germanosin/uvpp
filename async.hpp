#pragma once

#include "handle.hpp"
#include "error.hpp"
#include "loop.hpp"

namespace uvpp
{
    class Async : public handle<uv_async_t>
    {
    public:
        Async(loop &l, std::function<void()> callback): handle<uv_async_t>(), loop_(l.get())
        {
            init(callback);
        }
        
        Async(std::function<void()> callback): handle<uv_async_t>(), loop_(uv_default_loop())
        {
            init(callback);
        }
        
        error send() {
            return error(uv_async_send(get()));
        }

    private:
        
        error init(std::function<void()> callback)
        {
            callbacks::store(get()->data, internal::uv_cid_async, callback);

            return error(uv_async_init(loop_, get(), [](uv_async_t* handle){
                callbacks::invoke<decltype(callback)>(handle->data, internal::uv_cid_async);
            }));
        }
        
        uv_loop_t *loop_;   

    };
}