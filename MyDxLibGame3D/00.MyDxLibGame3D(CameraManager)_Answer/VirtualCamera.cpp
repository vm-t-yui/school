#include "DxLib.h"
#include "VirtualCamera.h"

VirtualCamera::VirtualCamera(const VECTOR& pos, const VECTOR& target, float fovDegree, int priority, bool isActive)
	: pos		(pos)
	 ,target	(target)
	 ,fovDegree	(fovDegree)
	 ,priority	(priority)
	 ,isActive	(isActive)
{
	// 処理なし
}