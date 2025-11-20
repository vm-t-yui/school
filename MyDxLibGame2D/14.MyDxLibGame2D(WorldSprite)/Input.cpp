#include "DxLib.h"
#include "Input.h"

/// <summary>
/// インプットクラス初期化
/// </summary>
void Input::Initialize()
{
	isPressingShotButton = false;	// そのフレームでボタンが押されているかどうか
	isPressedShotButton = false;	// ボタンが押された瞬間を保存するフラグ
	isPrevInputShotButton = false;	// 前のフレームにショットボタンのインプットがあったかどうか
}

/// <summary>
/// インプットクラス初期化
/// </summary>
void Input::Initialize(Input& input)
{
	input.isPressingShotButton = false;	// そのフレームでボタンが押されているかどうか
	input.isPressedShotButton = false;	// ボタンが押された瞬間を保存するフラグ
	input.isPrevInputShotButton = false;	// 前のフレームにショットボタンのインプットがあったかどうか
}

/// <summary>
/// インプットクラス更新
/// </summary>
void Input::Update()
{
	// ボタンが押されているかどうかを保存する
	isPrevInputShotButton = isPressingShotButton;
	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		isPressingShotButton = true;
	}
	else
	{
		isPressingShotButton = false;
	}
	// ボタンが押された瞬間を取得する
	if (isPressingShotButton && !isPrevInputShotButton)
	{
		isPressedShotButton = true;
	}
	else
	{
		isPressedShotButton = false;
	}
}