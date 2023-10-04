#pragma once

// ステージ情報構造体
class Stage
{
public:
	int GetModelHandle()const { return ModelHandle; }

	void Initialize();						// ステージの初期化処理
	void Finalie();							// ステージの後始末処理
	void Draw();

private:
	int		ModelHandle;				// モデルハンドル
};


