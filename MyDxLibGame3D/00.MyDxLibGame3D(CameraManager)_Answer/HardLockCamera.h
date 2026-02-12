#pragma once

/// <summary>
/// 固定位置にいるだけのカメラ
/// </summary>
class HardLockCamera : public VirtualCamera
{
public:
	HardLockCamera(const VECTOR& pos, const VECTOR& target, float fovDegree, int priority, bool isActive);

	// 処理も何もしない。初期値を保存するだけ
	virtual void Update() override {}
};

