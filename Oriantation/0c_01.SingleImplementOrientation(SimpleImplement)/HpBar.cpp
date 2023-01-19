#include "HpBar.h"
#include "DxLib.h"

void HpBar::Init()
{
	graph = LoadGraph("data/texture/hp.png");
}

void HpBar::Update()
{
	// キー入力が押された瞬間だけとる。HPを減らす
	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		if (prevHitKey == false)
		{
			--hp;
			prevHitKey = true;
		}
	}
	else
	{
		prevHitKey = false;
	}
}

void HpBar::Draw()
{
	// 画面サイズに合わせて、HPゲージを表示する
	printfDx("hp:%d\n", hp);
	DrawExtendGraph(
		HpBarSideSpace,
		HpBarStartY,
		(int)((ScreenW - (HpBarSideSpace * 2)) * ((float)hp / HpMax)),
		HpBarStartY + HpBarHight,
		graph,
		TRUE);
}
