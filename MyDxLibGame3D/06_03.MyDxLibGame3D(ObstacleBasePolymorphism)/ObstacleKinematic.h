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

	// overrideを明示的に書くことで、
	// ObstacleBaseの関数を上書き(オーバーライド)していることが分かる
	virtual void Update() override;				// 更新
};

