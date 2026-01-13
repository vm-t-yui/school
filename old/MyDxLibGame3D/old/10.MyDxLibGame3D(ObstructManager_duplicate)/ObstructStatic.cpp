//-----------------------------------------------------------------------------
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "DxLib.h"
#include "ObstructStatic.h"

/// <summary>
/// コンストラクタ
/// </summary>
ObstructStatic::ObstructStatic(int sourceModelHandle)
	: ObstructBase(sourceModelHandle)
{
	// 処理なし
}

/// <summary>
/// デストラクタ
/// </summary>
ObstructStatic::~ObstructStatic()
{
	// 処理なし
}

/// <summary>
/// 更新
/// </summary>
void ObstructStatic::Update()
{
	// ３Dモデルのポジション設定
	MV1SetPosition(modelHandle, pos);
}
