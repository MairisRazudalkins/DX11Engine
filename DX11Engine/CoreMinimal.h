#pragma once

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>

#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdio>
#include <map>

// Logger
#include "Log.h"

// Math
#include "Math.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Rotator.h"

#include "Object.h"

#define Check(x) if(!x) { Logger::ENGINE_LOG(Logger::LogType::Fatal, "Necessary variable was a nullptr (", #x, ")"); PostQuitMessage(0); }
#define Quit(msg) { Logger::ENGINE_LOG(Logger::LogType::Fatal, msg); PostQuitMessage(0); }