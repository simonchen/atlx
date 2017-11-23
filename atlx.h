#pragma once

#define _CRT_NON_CONFORMING_SWPRINTFS

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

// Standard C headers
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <time.h>
#include <locale.h>

// Windows Header Files:
#include <windows.h>
#include <tchar.h>
#include <psapi.h>
#include <commctrl.h>

#include <Ole2.h>
#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;

#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "gdiplus.lib")

// atlx headers for Utils
#include "atlx_string.h"

// atlx headers for Window controls
#include "WndThunker.h"
#include "WndSuper.h"
#include "DataExchanger.h"
#include "Dialogx.h"
#include "atlx_static_ctrl.h"
#include "atlx_button_ctrl.h"
#include "atlx_edit_ctrl.h"
