#pragma once

struct Camera
{
	VECTOR pos;
};

void InitCamera(Camera& camera);
void UpdateCamera(Camera& camera);