#pragma once
// std lib
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <string>
// External Lib
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>

// windows
#include <windows.h>

// Internal Includes
#include "Resource.h"

// Defines
#define WINDOWS
#define WARNING( s )                         \
{                                            \
   std::wostringstream os_;                  \
   os_ << s;                                 \
   OutputDebugStringW( os_.str().c_str() );  \
}

// MACRO for safe release of resources
#define SAFE_RELEASE(x) if(x != nullptr) x->Release(); x = nullptr;

// * To check monster
#define OutputLOG(_ClassName, _FunctionName, _OutputMessage)	   	\
OutputDebugStringA(_ClassName);											              \
OutputDebugStringA(" : In Function : ");								          \
OutputDebugStringA(_FunctionName);										            \
OutputDebugStringA(" : ");												                \
OutputDebugStringA(_OutputMessage);										            \
OutputDebugStringA("\n");