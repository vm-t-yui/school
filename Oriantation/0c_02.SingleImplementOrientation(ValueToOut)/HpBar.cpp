#include "HpBar.h"
#include "DxLib.h"

void HpBar::Init()
{
	graph = LoadGraph("data/texture/hp.png");
}

void HpBar::Update()
{
	// 処理なし.
}

void HpBar::Draw(int hp, int ScreenW, int HpMax)
{
	// 画面サイズに合わせて、HPゲージを表示する
	printfDx("hp:%d\n", hp);
	if (hp > 0)
	{
		DrawExtendGraph(
			HpBarSideSpace,
			HpBarStartY,
			HpBarSideSpace + (int)((ScreenW - (HpBarSideSpace * 2)) * ((float)drawHp / HpMax)),
			HpBarStartY + HpBarHight,
			graph,
			TRUE);
	}
}
