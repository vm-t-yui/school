// ---------------------------------------------------------------------------- -
// @brief  共通の宣言やインクルードを置いておく場所.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------

#pragma once

#include "DxLib.h"

// 画面サイズ.
const int SCREEN_WIDTH = 1024;
const float SCREEN_WIDTH_F = (float)SCREEN_WIDTH;
const int SCREEN_HEIGHT = 720;
const float SCREEN_HEIGHT_F = (float)SCREEN_HEIGHT;

// 画面サイズからそれっぽく計算させるためのポジションスケール.
const float POS_SCALE_FACTOR_2POINT5D = 0.115f;

// ビルボードのサイズを1としてそれっぽく表示するための画像スケール係数.
const float GRAPHIC_SCALE_FACTOR_2POINT5D = 7.0f;