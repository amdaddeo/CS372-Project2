#ifndef _ULT_H_
#define _ULT_H_
#include <ucontext.h>

typedef int Tid;
#define ULT_MAX_THREADS 1024
#define ULT_MIN_STACK 32768

//This struct need only contain ucontext and thread id
typedef struct ThrdCtlBlk{
  ucontext_t *p;
  Tid tid;
} ThrdCtlBlk;

typedef struct node_t node_t, *node, *queue;
struct node_t { ThrdCtlBlk block; node prev, next; };

/*
 * Tids between 0 and ULT_MAX_THREADS-1 may
 * refer to specific threads and negative Tids
 * are used for error codes or control codes.
 * The first thread to run (before ULT_CreateThread
 * is called for the first time) must be Tid 0.
 */
static const Tid ULT_ANY = -1;
static const Tid ULT_SELF = -2;
static const Tid ULT_INVALID = -3;
static const Tid ULT_NONE = -4;
static const Tid ULT_NOMORE = -5;
static const Tid ULT_NOMEMORY = -6;
static const Tid ULT_FAILED = -7;

static int isInit __attribute__ ((unused))= 0;
static int tidInc __attribute__ ((unused))=1;
static queue Q __attribute__ ((unused))=NULL;
static ThrdCtlBlk *currentBlock __attribute__ ((unused))=NULL;

static inline int ULT_isOKRet(Tid ret){
  return (ret >= 0 ? 1 : 0);
}

Tid ULT_CreateThread(void (*fn)(void *), void *parg);
Tid ULT_Yield(Tid tid);
Tid ULT_DestroyThread(Tid tid);

void stub(void (*root)(void *), void *arg);
queue q_new();
int empty(queue q);
void enqueue(queue q, ThrdCtlBlk n);
int dequeue(queue q, ThrdCtlBlk *val);
int extract(queue q, Tid val, ThrdCtlBlk *retval);


 




#endif



