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



