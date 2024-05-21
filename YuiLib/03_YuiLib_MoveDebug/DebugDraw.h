// 2024 Takeru Yui All Rights Reserved.
#pragma once

namespace YuiLib {

/// <summary>
/// デバッグ用の描画情報をまとめ、後で表示するためのクラス
/// </summary>
class DebugDraw
{
public:
	static void Clear();
	static void Draw();

	// ライン描画
	static void DrawLine(const VECTOR& start, const VECTOR& end, int color);

private:
	// ライン情報
	struct LineInfo
	{
		VECTOR start;
		VECTOR end;
		int color;
	};
	static std::list<LineInfo> lineInfo;
};

}