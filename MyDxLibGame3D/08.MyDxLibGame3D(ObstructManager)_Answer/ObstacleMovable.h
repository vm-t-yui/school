// 2025 Takeru Yui All Rights Reserved.
#pragma once

/// <summary>
/// 障害物：移動する
/// </summary>
class ObstacleMovable : public ObstacleBase
{
public:
	ObstacleMovable();				// コンストラクタ
	virtual ~ObstacleMovable();		// デストラクタ

    virtual void Update() override;			// 更新

private:
	float rad;						// sin移動用のラジアン関数
};

