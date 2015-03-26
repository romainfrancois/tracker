#include <tracker.h>

using namespace Rcpp ;
using namespace RcppParallel ;
using namespace tracker ;

struct test_tracker_data {
    test_tracker_data( int& target_, int value_ ) : target(target_), value(value_){}
    
    void process(){
        target = value ;    
    }
    
    int& target ;
    int value ;
} ;

// [[Rcpp::export]]
IntegerVector test_tracker(){
    
    // allocate some data
    IntegerVector res = no_init(4) ;
    
    // fill it in 4 threads
    ProcessThreads<test_tracker_data> threads  ;
    for(int i=0; i<4; i++){
        threads.add( new test_tracker_data( res[i], i) ) ;    
    }
    threads.join_all() ;
    
    return res ;
    
}