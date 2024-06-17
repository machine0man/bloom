#pragma once
#include <DirectXMath.h>
using namespace DirectX;

#define SCREEN_WIDTH  1920
#define SCREEN_HEIGHT 1080

#define SHADERS_PATH "res/shaders/"

enum ESHADER_TYPE
{
	NONE = 0,
	VERTEX = 1,
	PIXEL = 2,
	COUNT
};
struct Vertex
{
	XMFLOAT3 Position;
	XMFLOAT4 Color;
};
