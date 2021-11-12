#pragma once

// TODO: pch needs fixing!!!!!

#include <windows.h>
#include <d3d11_1.h>
#include <directxmath.h>
#include <d3dcompiler.h>
#include <directxcolors.h>

#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdio>

// Logger
#include "Log.h"

// Math
#include "Vector2.h"

#define Check(x) if(!x) { Logger::LOG_FATAL_ERROR("Necessary variable was a nullptr (", #x, ")"); PostQuitMessage(0); }