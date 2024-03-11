#pragma once
#include <limits.h>

#include "puddle.h"

#define UNIT_SIZE CURRENT_SCREEN_WIDTH / 30;

#define UINTMAX UINT_MAX

#define GRAVITY 9.8f

#if !defined(SAFE_DELETE)
#define SAFE_DELETE(x) if(x) delete x; x=NULL;
#endif

#if !defined(SAFE_DELETE_ARRAY)
#define SAFE_DELETE_ARRAY(x) if (x) delete [] x; x=NULL; 
#endif

#if !defined(SAFE_RELEASE)
#define SAFE_RELEASE(x) if(x) x->Release(); x=NULL;
#endif