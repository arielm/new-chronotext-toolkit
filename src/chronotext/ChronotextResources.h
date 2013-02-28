#pragma once

#include "cinder/CinderResources.h"

#ifdef RC_INVOKED
#define QUOTE_PATH(x) #x
#define CHR_RESOURCE(LOCALPREFIX, PATH, ID, TYPE) \
ID TYPE	QUOTE_PATH(LOCALPREFIX##PATH)
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#define CHR_RESOURCE( LOCALPREFIX, PATH, ID, TYPE ) \
#PATH,ID,#TYPE
#else
#define CHR_RESOURCE(LOCALPREFIX, PATH, ID, TYPE) #PATH
#endif
