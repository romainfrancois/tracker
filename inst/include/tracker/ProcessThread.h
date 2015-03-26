#ifndef TRACKER_PROCESSTHREAD_H
#define TRACKER_PROCESSTHREAD_H

namespace tracker {
       
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
        
        ~ProcessThread(){
            delete t ;    
        }
        
    private:
        thread* t ;
        ProcessThread( const ProcessThread& ) ;
    } ;
    
}

#endif