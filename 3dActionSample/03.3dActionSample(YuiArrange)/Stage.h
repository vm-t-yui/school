#pragma once

/// <summary>
/// 背景物の描画と更新
/// </summary>
class Stage
{
public:
	int GetModelHandle() const { return ModelHandle; }

	void Initialize();			// 初期化処理
	void Finalie();				// 後始末処理
	void Draw();

private:
	int		ModelHandle;		// モデルハンドル
};


