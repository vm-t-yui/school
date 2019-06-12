//-----------------------------------------------------------------------------
// @brief  ゲームのメインクラス.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#ifndef _GAME_H_
#define _GAME_H_

class Game final
{
public:
	Game();				        // コンストラクタ.
	~Game();				        // デストラクタ.

	void Initialize();				// 初期化.
	void Finalize();                // 終了処理.
private:
};

#endif // _GAME_H_