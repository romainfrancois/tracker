#include <tracker.h>

using namespace Rcpp ;
using namespace RcppParallel ;
using namespace tracker ;

struct test_tracker_data {
    test_tracker_data( int& target_, int value_ ) : 
        target(target_), value(value_){}
    
    void process(){
        target = value ;    
    }
    
    int& target ;
    int value ;
} ;

// [[Rcpp::export]]
IntegerVector test_ProcessThreads(){
    
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

struct test_tracker_pool_data {
    test_tracker_pool_data( double& target_, double value_ ) : 
        target(target_), value(value_){}
    
    void process(){
        double res =0 ;
        for( int i=0; i<1000000; i++) {
            res += sqrt(value) ;
        }
        target = res ;    
    }
    
    double& target ;
    int value ;
} ;


// [[Rcpp::export]]
NumericVector test_ProcessThreadsPool(){
    
    // allocate some data
    NumericVector res = no_init(200) ;
    
    // only process in 4 threads at a time
    ProcessThreadsPool<test_tracker_pool_data> threads(4)  ;
    for(int i=0; i<200; i++){
        threads.add( new test_tracker_pool_data( res[i], i) ) ;    
    }
    threads.join_all() ;
    
    return res ;
    
}