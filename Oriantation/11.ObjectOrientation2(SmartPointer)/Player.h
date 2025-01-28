//-----------------------------------------------------------------------------
// @brief  プレイヤー処理.
//-----------------------------------------------------------------------------
#pragma once

// プレイヤー.
class Player
{
public:
	Player();
	virtual ~Player();
	void Init();				// 初期化.
	void Update(ShotContainer& shotArray);	// アップデート.
	void Draw();				// 描画.

private:
	int Graph;
	int X;
	int Y;
	int W;
	int H;

	// ショットのインターバルを設定するカウンター
	int shotIntervalCount;
};