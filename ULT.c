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
 isInit = 1;
}


Tid ULT_CreateThread(void (*fn)(void *), void *parg)
{
  if(isInit == 0) {
   initQueue();
  }

  assert(0); /* TBD */
  return ULT_FAILED;
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
  if(wantTid == ULT_SELF){
    if(empty(Q)) {
     retVal = ULT_NONE;
    } else {
     retVal = wantTid;
    }
  } else {
   // Save current context
   // Push onto queue
   // Give control to wantTid thread
   retVal = wantTid;
  }
  
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




//////////////// QUEUE MANGAMENT ////////////////

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
        if (!HEAD(q)) HEAD(q) = nd;
        nd->prev = TAIL(q);
        if (nd->prev) nd->prev->next = nd;
        TAIL(q) = nd;
        nd->next = 0;
}

int dequeue(queue q, ThrdCtlBlk *val)
{
        node tmp = HEAD(q);
        if (!tmp) return 0;
        *val = tmp->block;

        HEAD(q) = tmp->next;
        if (TAIL(q) == tmp) TAIL(q) = 0;
        free(tmp);

        return 1;
}

int extract(queue q, Tid val, ThrdCtlBlk *retval)
{
        node tmp = HEAD(q);
        while(1)  //!(tmp->block == val) && tmp->next != 0)
        {
          ThrdCtlBlk tmpBlock = tmp->block;
          Tid tidval = tmpBlock.tid;
          if(tidval == val || tmp->next == 0) {
	    break;
	  }
          tmp = tmp->next;
        }

        if(tmp->next != 0)
        {
          node p = tmp->prev;
          node n = tmp->next;
          p->next = n;
          n->prev = p;
	  retval = &tmp->block;
          return 1;
        }
        else
        {
          return 0;
        }
}



