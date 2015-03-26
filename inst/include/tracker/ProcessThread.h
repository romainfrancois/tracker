#ifndef TRACKER_PROCESSTHREADS_H
#define TRACKER_PROCESSTHREADS_H

namespace tracker {
       
    template <typename T>
    void process_fun( void* void_arg ){
        T* arg = reinterpret_cast<T*>(void_arg) ;
        arg->process() ;
        delete arg ;
    }
    
    template <typename T>
    class ProcessThread {
    public:
        typedef tthread::thread thread ;
        
        ProcessThread( T* arg ) {
            t = new thread( process_fun<T>, reinterpret_cast<void*>(arg) );    
        }
        
        inline void join(){
            t->join() ;    
        }
        
    private:
        thread* t ;
        process_thread( const process_thread& ) ;
    } ;
    
}

#endif