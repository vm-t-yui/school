#include "DxLib.h"
#include "VirtualCamera.h"
#include "LookTargetCamera.h"

LookTargetCamera::LookTargetCamera(const VECTOR& pos, const VECTOR& target, float fovDegree, int priority, bool isActive)
	: VirtualCamera(pos, target, fovDegree, priority, isActive)
{
	// 処理なし
}