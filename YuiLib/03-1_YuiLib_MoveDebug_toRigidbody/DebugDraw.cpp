// 2024 Takeru Yui All Rights Reserved.
#include "DxLib.h"
#include <list>
#include "DebugDraw.h"
using namespace YuiLib;

// ���̒�`
std::list<DebugDraw::LineInfo> DebugDraw::lineInfo;

/// <summary>
/// �N���A
/// </summary>
void DebugDraw::Clear()
{
	lineInfo.clear();
}

/// <summary>
/// �`��
/// </summary>
void DebugDraw::Draw()
{
	for (const auto& item : lineInfo)
	{
		DxLib::DrawLine(static_cast<int>(item.start.x),
			static_cast<int>(item.start.y),
			static_cast<int>(item.end.x),
			static_cast<int>(item.end.y),
			item.color);
	}
}

/// <summary>
/// ���C���`��
/// </summary>
void DebugDraw::DrawLine(const VECTOR& start, const VECTOR& end, int color)
{
	LineInfo newInfo;
	newInfo.start = start;
	newInfo.end = end;
	newInfo.color = color;
	lineInfo.push_back(newInfo);
}