//-----------------------------------------------------------------------------
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "ObstructBase.h"

/// <summary>
/// 障害物：停止
/// </summary>
class ObstructStatic : public ObstructBase
{
public:
	ObstructStatic(int sourceModelHandle);	// コンストラクタ
	virtual ~ObstructStatic();				// デストラクタ

	void Update() override;					// 更新
};

