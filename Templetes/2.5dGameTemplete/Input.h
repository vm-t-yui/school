#pragma once

/// <summary>
/// キー入力クラス
/// </summary>
class Input
{
public:
	Input();
	virtual ~Input() { /* 処理なし */ }

	// クラスのインスタンスごとに生成
	// クラス内のメンバ変数を呼ぶのでInput& inputで参照型を渡す必要がない
	void Initialize();		// インプットクラスの初期化
	void Update();

	bool IsPressedShotButton() const { return isPressedShotButton; }

private:
	bool isPressingShotButton;	// そのフレームでボタンが押されているかどうか
	bool isPressedShotButton;	// ボタンが押された瞬間を保存するフラグ
	bool isPrevInputShotButton;	// 前のフレームにショットボタンのインプットがあったかどうか
};