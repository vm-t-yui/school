#pragma once

struct Player;
struct Camera
{
	VECTOR pos;				// 実際のカメラのポジション
	VECTOR drawOffset;		// 全てのDrawObjectに足す値
};

void InitCamera(Camera& camera);
void UpdateCamera(Camera& camera, const Player& player);