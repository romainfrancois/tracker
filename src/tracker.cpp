#include <tracker.h>
#include <csetjmp>

#define JMP_BUF sigjmp_buf

enum {
    CTXT_TOPLEVEL = 0,
    CTXT_NEXT        = 1,
    CTXT_BREAK      = 2,
    CTXT_LOOP        = 3,    /* break OR next target */
    CTXT_FUNCTION = 4,
    CTXT_CCODE      = 8,
    CTXT_RETURN      = 12,
    CTXT_BROWSER  = 16,
    CTXT_GENERIC  = 20,
    CTXT_RESTART  = 32,
    CTXT_BUILTIN  = 64  /* used in profiling */
};

#ifdef BC_INT_STACK
typedef union { void *p; int i; } IStackval;
#endif

typedef struct RPRSTACK {
    SEXP promise;
    struct RPRSTACK *next;
} RPRSTACK;

typedef struct RCNTXT {
    struct RCNTXT *nextcontext;    /* The next context up the chain */
    int callflag;                    /* The context "type" */
    JMP_BUF cjmpbuf;                /* C stack and register information */
    int cstacktop;                  /* Top of the pointer protection stack */
    int evaldepth;                /* evaluation depth at inception */
    SEXP promargs;                /* Promises supplied to closure */
    SEXP callfun;                    /* The closure called */
    SEXP sysparent;                /* environment the closure was called from */
    SEXP call;                        /* The call that effected this context*/
    SEXP cloenv;                    /* The environment */
    SEXP conexit;                    /* Interpreted "on.exit" code */
    void (*cend)(void *);          /* C "on.exit" thunk */
    void *cenddata;                  /* data for C "on.exit" thunk */
    void *vmax;                      /* top of R_alloc stack */
    int intsusp;                /* interrupts are suspended */
    SEXP handlerstack;          /* condition handler stack */
    SEXP restartstack;          /* stack of available restarts */

    struct RPRSTACK *prstack;   /* stack of pending promises */
    SEXP *nodestack;
    #ifdef BC_INT_STACK
        IStackval *intstack;
    #endif
    SEXP srcref;                  /* The source line in effect */
    int browserfinish;          /* should browser finish this context without stopping */
} RCNTXT ;

extern RCNTXT* R_GlobalContext  ;

// [[Rcpp::export]]
void track_here() {
    // skip the .Call and track_here contexts
    RCNTXT* ctx = R_GlobalContext->nextcontext->nextcontext ;
    
    for( ; ctx ; ctx = ctx->nextcontext ){
        if( ctx->callflag & (CTXT_FUNCTION | CTXT_BUILTIN) && TYPEOF(ctx->call) == LANGSXP ){
            SEXP fun = CAR(ctx->call) ;
            std::cout << "\"" ;
            if( TYPEOF(fun)==SYMSXP ){
                std::cout << CHAR(PRINTNAME(fun)) ;    
            } else {
                std::cout << "<Anonymous>" ; 
            }
            std::cout << "\" " ;
        }
    }
    std::cout << std::endl ;
}


