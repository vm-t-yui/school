//----------------------------//
// ユーティリティ（便利）関数群
//----------------------------//
#include "Utility.h"
#include "DxLib.h"

// 指定された全文字列の幅を取得.
int GetDrawStringWidthFull(char* str)
{
	return GetDrawStringWidth(str, (int)strlen(str));
}

// ディグリーをラジアンに変換
float GetRadian(float degree)
{
	return (PI / 180.0f) * degree;
}