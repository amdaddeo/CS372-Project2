#include <assert.h>
#include <stdio.h>

/* We want the extra information from these definitions */
#ifndef __USE_GNU
#define __USE_GNU
#endif /* __USE_GNU */
#include <stdlib.h>
#include "ULT.h"


 
//create queue out here. in each function, check to see if it
//has been initialized. If it hasn't, initialize it.
//also set running thread during this (thread/tid 0). 

//aside: the running thread does not have to go on the queue.
//it may be easier to implement. 

//possible way to implement this is to keep a static
//boolean in the header file. 

void initQueue() {
 Q = q_new();
 currentBlock=malloc(sizeof(ThrdCtlBlk));
 currentBlock->tid = 0;
 ucontext_t context, *currentContext = &context;
 getcontext(currentContext);
 currentBlock->p=currentContext;
 isInit = 1;
}

Tid ULT_CreateThread(void (*fn)(void *), void *parg)
{
  if(isInit == 0) {
   initQueue();
  }

  ucontext_t context, *newContext = &context;
  getcontext(newContext);
  ucontext_t *currentContext = (ucontext_t *)currentBlock->p;
  // updating program counter
  newContext->uc_mcontext.gregs[REG_EIP] = (unsigned int)&stub;
  // allocate new stacka
  newContext->uc_mcontext.gregs[REG_ESP] = (unsigned int)malloc(ULT_MIN_STACK);
 // push pargs onto stack
  newContext->uc_mcontext.gregs[REG_EBP+8]= (unsigned int)parg;
 // push fn onto stack
  newContext->uc_mcontext.gregs[REG_EBP+12]= (unsigned int)fn;
  // save old frame pointer onto stack
  newContext->uc_mcontext.gregs[REG_EBP+16] = currentContext->uc_mcontext.gregs[REG_EIP];
  // 
  newContext->uc_mcontext.gregs[REG_EBP] = currentContext->uc_mcontext.gregs[REG_EBP];
  // push args onto stack
  //newContext->uc_mcontext.gregs[REG_EBP+8]= (unsigned int)parg;
  ThrdCtlBlk *newBlock = malloc(sizeof(ThrdCtlBlk));
  newBlock->p = newContext;
  newBlock->tid = tidInc++;
  enqueue(Q,*newBlock);
  // create a new context
  // intilize it
  // set to this context
  //assert(0); /* TBD */
  return newBlock->tid;
}


Tid ULT_Yield(Tid wantTid) //give control to wantTid
{
  if(isInit == 0) {
   initQueue();
  }
  /*
   *Possible algorithm discussed in OA:
   *find new thread in the RQ (ready queue)
   *save context of current thread
   *grab context of new thread
   *put current thread in RQ
   *set context to new thread
   */

  Tid retVal = ULT_ANY;
  if(wantTid == ULT_SELF || wantTid == currentBlock->tid){
    if(empty(Q) && (wantTid != currentBlock->tid)) {
     retVal = ULT_NONE;
    } else {
     return  wantTid;
    }
  } else if(wantTid == ULT_ANY) {
   if(empty(Q)) {
     return ULT_NONE;
   } else {
     //ULT_ANY with stuff on queue
     ThrdCtlBlk *tmp=malloc(sizeof(ThrdCtlBlk));
     dequeue(Q,tmp);
     ucontext_t context, *currentContext = &context;

     int flag = 0;
     getcontext(currentContext);
     currentBlock->p = currentContext;
     enqueue(Q,*currentBlock);
     //currentBlock = tmp; //orig
     tmp = currentBlock;
     retVal = tmp->tid;
     ucontext_t *nextContext = (ucontext_t *)tmp->p;
     if (flag == 0){
       flag = 1;
       setcontext(nextContext);
     }
   }
  } else {
   //Doesn't match any of the codes (ULT_ANY, etc)
    ThrdCtlBlk *tmp=malloc(sizeof(ThrdCtlBlk));
    //int queued = dequeue(Q,tmp);
    int queued = extract(Q, wantTid, tmp);
    if(queued) {
     ucontext_t context, *currentContext = &context;
   
     int flag = 0;
     getcontext(currentContext);
     currentBlock->p = currentContext;
     enqueue(Q,*currentBlock);
     //currentBlock = tmp; //orig
     //retVal = currentBlock->tid;
     //tmp = currentBlock;
     retVal = tmp->tid;
     ucontext_t *nextContext = tmp->p;
     if (flag == 0){
       flag = 1;
       setcontext(nextContext);
     }
    }
    else {
      return ULT_INVALID;
    }
  }
  printf("retVal: %d\n", retVal);
  return retVal;
}


Tid ULT_DestroyThread(Tid tid)
{
  if(isInit == 0){
   initQueue();
  }

  assert(0); /* TBD */
  return ULT_FAILED;
}




//////////////// QUEUE MANGAMENT - REMEMBER TO CITE THIS!!!!!!!!!!!!!!!////////////////

#define HEAD(q) q->prev
#define TAIL(q) q->next
queue q_new()
{
        node q = malloc(sizeof(node_t));
        q->next = q->prev = 0;
        return q;
}

int empty(queue q)
{
        return !HEAD(q);
}

void enqueue(queue q, ThrdCtlBlk n)
{
        node nd = malloc(sizeof(node_t));
        nd->block = n;
        if (!HEAD(q)){
          HEAD(q) = nd;
        }
        nd->prev = TAIL(q);
        if (nd->prev){
          nd->prev->next = nd;
        }
        TAIL(q) = nd;
        nd->next = 0;
}

int dequeue(queue q, ThrdCtlBlk *val)
{
        node tmp = HEAD(q);
        if (!tmp){
          return 0;
        }
        *val = tmp->block;

        HEAD(q) = tmp->next;
        if (TAIL(q) == tmp){
          TAIL(q) = 0;
        }
        free(tmp);

        return 1;
}


int extract(queue q, Tid val, ThrdCtlBlk *retval)
{
        if(empty(q)) {
          return 0;
        }

        node tmp = HEAD(q);
        while(!tmp)  //!(tmp->block == val) && tmp->next != 0)
        {
          ThrdCtlBlk tmpBlock = tmp->block;
          Tid tidval = tmpBlock.tid;
          if(tidval == val) {
            break;
          }

          tmp = tmp->next;
        }

        if(tmp)
        {
          if(tmp == HEAD(q)) {
           dequeue(q,retval);
           return 1;
          }
          if (TAIL(q) == tmp) {
            TAIL(q) = 0;
            node p = tmp->prev;
            p->next = TAIL(q);
            *retval = tmp->block;
            return 1;
          }
          else {
            node p = tmp->prev;
            node n = tmp->next;
            p->next = n;
            n->prev = p;
            retval = &tmp->block;
            return 1;
          }
        }
        else
        {
          return 0;
        }
}



void stub(void (*root)(void *), void *arg)
    {
        // thread starts here
        Tid ret;
        root(arg); // call root function
        ret = ULT_DestroyThread(ULT_SELF);
        assert(ret == ULT_NONE); // we should only get here if we are the last thread.
        exit(0); // all threads are done, so process should exit 
    } 
