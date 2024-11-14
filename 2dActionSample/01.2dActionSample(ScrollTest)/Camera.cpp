#include "DxLib.h"
#include "Camera.h"

void InitCamera(Camera& camera)
{
	camera.drawOffset = VGet(0, 0, 0);
}

void UpdateCamera(Camera& camera)
{
	// xを足すと全部の描画物が左に動くよー
	camera.drawOffset.x += 0.3f;
}