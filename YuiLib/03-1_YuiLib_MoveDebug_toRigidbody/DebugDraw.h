// 2024 Takeru Yui All Rights Reserved.
#pragma once

namespace YuiLib {

/// <summary>
/// �f�o�b�O�p�̕`������܂Ƃ߁A��ŕ\�����邽�߂̃N���X
/// </summary>
class DebugDraw
{
public:
	static void Clear();
	static void Draw();

	// ���C���`��
	static void DrawLine(const VECTOR& start, const VECTOR& end, int color);

private:
	// ���C�����
	struct LineInfo
	{
		VECTOR start;
		VECTOR end;
		int color;
	};
	static std::list<LineInfo> lineInfo;
};

}