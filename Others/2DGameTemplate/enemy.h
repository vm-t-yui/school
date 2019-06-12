//-----------------------------------------------------------------------------
// @brief  .
// 2016 VikingMaxx Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------

#pragma once

// エネミー.
class Enemy
{
public:
    // エネミー関数群.
    void Init();        // 初期化.
    void Update();      // アップデート.
    void Draw();        // 描画.

    int     X;
    int     Y;
    int     Graph;
    bool    DamageFlag;
    int     DamageCounter;
    int     DamageGraph;
    int     W;
    int     H;
    int     Life;
    bool    RightMove;
};
