#pragma once

struct Camera
{
	VECTOR drawOffset;
};

void InitCamera(Camera& camera);
void UpdateCamera(Camera& camera);