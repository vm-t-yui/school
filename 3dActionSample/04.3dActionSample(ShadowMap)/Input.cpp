#include "DxLib.h"
#include "Input.h"

/// <summary>
/// コンストラクタ
/// </summary>
Input::Input()
	: nowFrameInput		(0)
	, nowFrameNewInput	(0)
{
	// 処理なし
}

/// <summary>
/// デストラクタ
/// </summary>
Input::~Input()
{
	// 処理なし
}

/// <summary>
/// 更新
/// </summary>
void Input::Update()
{
	// ひとつ前のフレームの入力を変数にとっておく
	int Old = nowFrameInput;

	// 現在の入力状態を取得
	nowFrameInput = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	// 今のフレームで新たに押されたボタンのビットだけ立っている値を nowFrameNewInput に代入する
	nowFrameNewInput = nowFrameInput & ~Old;
}