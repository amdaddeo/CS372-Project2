#include <assert.h>
#include <stdio.h>

/* We want the extra information from these definitions */
#ifndef __USE_GNU
#define __USE_GNU
#endif /* __USE_GNU */
#include <stdlib.h>

#include "ULT.h"


Tid 
ULT_CreateThread(void (*fn)(void *), void *parg)
{
  assert(0); /* TBD */
  return ULT_FAILED;
}



Tid ULT_Yield(Tid wantTid)
{
  Tid getsControl;

  switch ((int)wantTid)
  {
   case -2 :
    //printf("ULT_SELF\n");
    return ULT_NONE;
    getsControl = wantTid;
   break;
   case -3 :
    printf("-3\n");
   break;
   default :
    //printf("DEFAULT\n");
   break;
  }
  
  return 0;
}


Tid ULT_DestroyThread(Tid tid)
{
  assert(0); /* TBD */
  return ULT_FAILED;
}





