#include "DxLib.h"
#include "Camera.h"

void InitCamera(Camera& camera)
{
	camera.pos = VGet(0, 0, 0);
}

void UpdateCamera(Camera& camera)
{
	// xを足すと全部の描画物が左に動くよー
	camera.pos.x += 0.3f;
}