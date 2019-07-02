#pragma once

// プロトタイプ宣言
struct GameObject;

#define BG_SCROLL_SPEED		2		// 背景の縦スクロールスピード.

// 背景.
struct BG
{
	GameObject	obj;
};

// 初期化.
void InitBG(BG& bg, int graph, int index);

// アップデート.
void UpdateBG(BG& bg);

// 描画.
void DrawBG(BG& bg);