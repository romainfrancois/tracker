#ifndef TRACKER_PROCESSTHREADS_H
#define TRACKER_PROCESSTHREADS_H

namespace tracker {

    template <typename T>
    class ProcessThreads {
    public:
        typedef ProcessThread<T> Thread ;
        
        ProcessThreads(){}
        
        inline void add( T* arg ){
            threads.push_back( new Thread(arg) ) ;    
        }
        
        inline void join_all(){
            for(size_t i=0; i<threads.size() ; i++) {
                threads[i]->join() ;
            }
        }
        
        ~ProcessThreads(){
            for(size_t i=0; i<threads.size() ; i++) {
                delete threads[i] ;
            }
            threads.clear() ;
        }
        
    private:
        std::vector<Thread*> threads ;
        
    } ;

}

#endif