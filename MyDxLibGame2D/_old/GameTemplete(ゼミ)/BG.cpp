//-----------------------------------------------------------------------------
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "GameObject.h"
#include "BG.h"
#include "DxLib.h"

// 初期化.
void InitBG(BG& bg, int graph, int index)
{
	bg.obj.graph = graph;
	GetGraphSize(bg.obj.graph, &bg.obj.w, &bg.obj.h);
	bg.obj.pos.x = bg.obj.w * 0.5f;
	bg.obj.pos.y = (float)(index * bg.obj.h);
}

// アップデート.
void UpdateBG(BG& bg)
{
	// 背景縦スクロール.
	bg.obj.pos.y += BG_SCROLL_SPEED;

	// 背景のy座標が背景一つ分余計に下になったら上に移動.
	if (bg.obj.pos.y > bg.obj.h + bg.obj.h * 0.5f)
	{
		bg.obj.pos.y -= bg.obj.h * 2;
	}
}

// 描画.
void DrawBG(BG& bg)
{
	DrawGameObject(bg.obj, bg.obj.graph);
}