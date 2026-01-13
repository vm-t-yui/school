// 2025 Takeru Yui All Rights Reserved.
#pragma once

/// <summary>
/// 障害物:動かない
/// </summary>
class ObstacleKinematic : public ObstacleBase
{
public:
	ObstacleKinematic();				// コンストラクタ
	virtual ~ObstacleKinematic();		// デストラクタ

	void Update();					// 更新
};

