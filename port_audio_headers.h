#pragma once

#pragma warning(disable: "C4189")

#ifdef PA_USE_WASAPI
#include <pa_win_wasapi.h>
#endif

#ifdef WIN32
//#ifdef PA_USE_ASIO
#include "portaudio/include/pa_asio.h"
//#endif
#endif

#include "portaudio/include/portaudio.h"
