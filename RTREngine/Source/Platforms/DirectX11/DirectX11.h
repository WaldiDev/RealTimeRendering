#pragma once

#include <d3d11_1.h>
#include <dxgi.h>
#include <dxgi1_6.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define SAFE_RELEASE(p)	{ if (p) { (p)->Release(); (p)=nullptr; } }