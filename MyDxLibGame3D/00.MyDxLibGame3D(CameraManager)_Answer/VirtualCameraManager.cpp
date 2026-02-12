#include "DxLib.h"
#include <assert.h>
#include <vector>
#include "VirtualCamera.h"
#include "VirtualCameraManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
VirtualCameraManager::VirtualCameraManager()
	: resultPos			(VGet(0,0,0))
	 ,resultTarget		(VGet(0,0,0))
	 ,resultFovDegree	(0.0f)
{
	// 処理なし
}

/// <summary>
/// VirtualCameraの登録
/// </summary>
void VirtualCameraManager::Register(std::shared_ptr<VirtualCamera> camera)
{
	if (virtualCameras.empty())
	{
		resultPos		= camera->GetPos();
		resultTarget	= camera->GetTarget();
		resultFovDegree	= camera->GetFovDegree();
	}
	// 既に登録済ならassert
	if (std::find(virtualCameras.begin(), virtualCameras.end(), camera) != virtualCameras.end())
	{
		assert(!"カメラの二重登録");
	}
	else
	{
		virtualCameras.emplace_back(camera);
	}
}

/// <summary>
/// VirtualCameraの登録解除
/// </summary>
void VirtualCameraManager::Remove(std::shared_ptr<VirtualCamera> camera)
{
	// いないならassert
	if (std::find(virtualCameras.begin(), virtualCameras.end(), camera) == virtualCameras.end())
	{
		assert(!"登録していないカメラを登録解除しようとした");
	}
	else
	{
		// 削除。Erase-removeイディオムを使うこと
		virtualCameras.erase(std::remove(virtualCameras.begin(), virtualCameras.end(), camera), virtualCameras.end());
	}
}

/// <summary>
/// 更新
/// </summary>
void VirtualCameraManager::Update()
{
	// 空ならreturn
	if (virtualCameras.empty())
	{
		return;
	}

	// 計算に最も優先されるカメラの更新
	topPriorityCamera = nullptr;
	int size = static_cast<int>(virtualCameras.size());
	for (int i = 0; i < size; i++)
	{
		if(virtualCameras[i]->IsActive())
		{
			// priorityを比較する
			if (topPriorityCamera == nullptr
				||
				topPriorityCamera->GetPriority() < virtualCameras[i]->GetPriority()
				)
			{
				topPriorityCamera = virtualCameras[i];
			}
		}
	}

	// TODO: easing補完、不要カメラの自動登録解除

	resultPos		= topPriorityCamera->GetPos();
	resultTarget	= topPriorityCamera->GetTarget();
	resultFovDegree	= topPriorityCamera->GetFovDegree();
}