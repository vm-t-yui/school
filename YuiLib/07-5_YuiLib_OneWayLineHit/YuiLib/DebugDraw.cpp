// 2024 Takeru Yui All Rights Reserved.
#include "DxLib.h"
#include "WorldSprite.h"
#include "DebugDraw.h"
using namespace YuiLib;

// 実体定義
std::vector<DebugDraw::LineInfo>	DebugDraw::lineInfo;
std::vector<DebugDraw::CircleInfo>	DebugDraw::circleInfo;
std::vector<int>					DebugDraw::handles;
std::vector<WorldSprite*>			DebugDraw::spites;

/// <summary>
/// クリア
/// </summary>
void DebugDraw::Initialize()
{
	// Circle
	handles.emplace_back(LoadGraph("YuiLib/Resources/circle128.png"));
	// CircleWire
	handles.emplace_back(LoadGraph("YuiLib/Resources/circleWire128.png"));
	// Square
	handles.emplace_back(LoadGraph("YuiLib/Resources/white.png"));
	// SquareWire
	handles.emplace_back(LoadGraph("YuiLib/Resources/squareWire128.png"));

	for (int i = 0; i < static_cast<int>(HandleKind::Num); i++)
	{
		HandleKind kind = static_cast<HandleKind>(i);
		auto newSprite = new WorldSprite();
		if (kind == HandleKind::Square)
		{
			newSprite->Initialize(handles[i], 32, 0);
		}
		else
		{
			newSprite->Initialize(handles[i], 128, 0);
		}
		spites.emplace_back(newSprite);
	}
}

/// <summary>
/// クリア
/// </summary>
void DebugDraw::Finalize()
{
	for (const auto& item : handles)
	{
		DeleteGraph(item);
	}
	for (int i = 0; i < static_cast<int>(HandleKind::Num); i++)
	{
		delete spites[i];
		spites[i] = nullptr;
	}
	handles.clear();
	spites.clear();
}

/// <summary>
/// クリア
/// </summary>
void DebugDraw::Clear()
{
	lineInfo.clear();
	circleInfo.clear();
}

/// <summary>
/// 描画
/// </summary>
void DebugDraw::Draw3D()
{
	for (const auto& item : lineInfo)
	{
		DxLib::DrawLine3D(item.start,
			item.end,
			item.color);
	}
	for (const auto& item : circleInfo)
	{
		//DxLib::DrawSphere3D(item.center,
		//	item.radius,
		//	6,
		//	item.color,
		//	item.color,
		//	FALSE);
		int intKind = static_cast<int>(HandleKind::Circle);
		if (item.isWire)
		{
			intKind = static_cast<int>(HandleKind::CircleWire);
		}
		spites[intKind]->SetTransform(item.center, item.radius * 2);
		spites[intKind]->SetColor(item.color, item.alpha);
		spites[intKind]->Draw();
	}
}

/// <summary>
/// ライン描画
/// </summary>
void DebugDraw::DrawLine(const VECTOR& start, const VECTOR& end, int color)
{
	LineInfo newInfo;
	newInfo.start = start;
	newInfo.end = end;
	newInfo.color = color;
	lineInfo.emplace_back(newInfo);
}

/// <summary>
/// 円描画
/// </summary>
void DebugDraw::DrawCircle(const VECTOR& center, float radius, int color, float alpha, bool isWire)
{
	CircleInfo newInfo;
	newInfo.center = center;
	newInfo.radius = radius;
	newInfo.color = color;
	newInfo.alpha = alpha;
	newInfo.isWire = isWire;
	circleInfo.emplace_back(newInfo);
}