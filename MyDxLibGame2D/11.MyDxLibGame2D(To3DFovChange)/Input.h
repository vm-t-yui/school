#pragma once

/// <summary>
/// キー入力クラス
/// </summary>
class Input
{
public:
	bool isPressingShotButton;	// そのフレームでボタンが押されているかどうか
	bool isPressedShotButton;	// ボタンが押された瞬間を保存するフラグ
	bool isPrevInputShotButton;	// 前のフレームにショットボタンのインプットがあったかどうか

	// クラスでひとつ
	// クラスで共有の関数としてアクセスするので、メンバ変数がない
	// Input& inputで参照を渡す必要がある
	static void Initialize(Input& input);	// インプットクラスの初期化

	// クラスのインスタンスごとに生成
	// クラス内のメンバ変数を呼ぶのでInput& inputで参照型を渡す必要がない
	void Initialize();		// インプットクラスの初期化
	void Update();
};