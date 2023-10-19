#include "DxLib.h"
#include "Input.h"

// 更新
void Input::Update()
{
	int Old;

	// ひとつ前のフレームの入力を変数にとっておく
	Old = NowFrameInput;

	// 現在の入力状態を取得
	NowFrameInput = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	// 今のフレームで新たに押されたボタンのビットだけ立っている値を NowFrameNewInput に代入する
	NowFrameNewInput = NowFrameInput & ~Old;
}