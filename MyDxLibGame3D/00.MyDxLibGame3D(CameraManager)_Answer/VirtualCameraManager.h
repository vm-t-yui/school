#pragma once

#include <memory>

class VirtualCamera;

// std::vector<std::shared_ptr<VirtualCamera>>&が長すぎるので、
// VirtualCameraArray型として言い換える
using VirtualCameraArray = std::vector<std::shared_ptr<VirtualCamera>>;

/// <summary>
/// TODO:実装 VirtualCameraの管理者
/// </summary>
class VirtualCameraManager
{
public:
	VirtualCameraManager();

	// VirtualCameraの登録（スマートポインタを渡す）
	void Register(std::shared_ptr<VirtualCamera> camera);

	// VirtualCameraの登録解除（スマートポインタを渡す）
	void Remove(std::shared_ptr<VirtualCamera> camera);

	// 最終計算後の位置とターゲットとfov
	const VECTOR&	GetResultPos()		const{ return resultPos; }
	const VECTOR&	GetResultTarget()	const{ return resultTarget; }
	float			GetResultFovDegree()const{ return resultFovDegree; }

	// Update（計算に最も優先されるカメラの更新） ※あとでeasing補完、不要カメラの自動登録解除
	void Update();

private:
	// 全VirtualCameraのスマートポインタ参照
	VirtualCameraArray virtualCameras;
	
	// 計算に最も優先されるバーチャルカメラ
	std::shared_ptr<VirtualCamera> topPriorityCamera;

	VECTOR	resultPos;
	VECTOR	resultTarget;
	float	resultFovDegree;
};

