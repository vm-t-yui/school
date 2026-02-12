#pragma once

/// <summary>
/// ターゲットを見続けるカメラ
/// </summary>
class LookTargetCamera : public VirtualCamera
{
public:
	LookTargetCamera(const VECTOR& pos, const VECTOR& target, float fovDegree, int priority, bool isActive);

	// ターゲット更新
	// このように、派生先には自由に色々入れられるので、
	// 例えば複数の敵のポインタをセットしてUpdateで自由に切り替える等も可能
	void UpdateTarget(const VECTOR& target) { this->target = target; }

	// 処理なし
	virtual void Update() override {}
};

