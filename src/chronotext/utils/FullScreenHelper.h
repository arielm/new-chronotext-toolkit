#pragma once

#include "cinder/Cinder.h"

#if defined(CINDER_MAC)
#include "FullScreenHelperImplMac.h"
typedef FullScreenHelperImplMac FullScreenHelper;
#elif defined(CINDER_MSW)
#include "FullScreenHelperImplMsw.h"
typedef FullScreenHelperImplMsw FullScreenHelper;
#endif
