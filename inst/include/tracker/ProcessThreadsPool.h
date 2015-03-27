#ifndef TRACKER_PROCESSTHREADSPOOL_H
#define TRACKER_PROCESSTHREADSPOOL_H

namespace tracker {
    
    template <typename T>
    class ProcessThreadsPool ;
    
    template <typename T>
    struct thread_notify_data {
        thread_notify_data( T* data_, ProcessThreadsPool<T>& pool_ ) : 
            data(data_), pool(pool_) {} 
        
        void process() ; 
    
        ~thread_notify_data(){
            delete data ;    
        }
        
        T* data ;
        ProcessThreadsPool<T>& pool ;
        
    } ;
    
    template <typename T>
    class ProcessThreadsPool {
    public:
        typedef ProcessThread< thread_notify_data<T> > Thread ;
        
        ProcessThreadsPool(int n_threads_) : 
            m(), 
            cond(),
            n_threads(n_threads_), 
            n_threads_processing(0) 
            {}
        
        inline void add( T* arg ){
            thread_notify_data<T>* param = new thread_notify_data<T>( arg, *this );
            threads.push_back( new Thread(param) ) ;
        }
        
        inline void join_all(){
            for(size_t i=0; i<threads.size() ; i++) {
                threads[i]->join() ;
            }        
        }
        
        ~ProcessThreadsPool(){
            for(size_t i=0; i<threads.size() ; i++) {
                delete threads[i] ;
            }
            threads.clear() ;
        }
        
        inline void wait(){
            tthread::lock_guard<tthread::mutex> lock(m) ;
            while( n_threads_processing >= n_threads ) {
                cond.wait(m) ;
            }
            n_threads_processing++ ;
        }
        
        inline void done(){
            tthread::lock_guard<tthread::mutex> lock(m) ;
            n_threads_processing-- ;
            cond.notify_all() ;
        }
        
    private:
        
        tthread::mutex m ;
        tthread::condition_variable cond ;
        
        int n_threads ;
        std::vector<Thread*> threads ;
        int n_threads_processing ;
        
    } ;
    
    template <typename T>
    void thread_notify_data<T>::process(){
        pool.wait() ;
        data->process() ;
        pool.done() ;
    }
    
}

#endif