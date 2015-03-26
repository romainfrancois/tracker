#ifndef TRACKER_PROCESS_FUN_H
#define TRACKER_PROCESS_FUN_H

namespace tracker {
       
    template <typename T>
    void process_fun( void* void_arg ){
        T* arg = reinterpret_cast<T*>(void_arg) ;
        arg->process() ;
        delete arg ;
    }
    
}

#endif