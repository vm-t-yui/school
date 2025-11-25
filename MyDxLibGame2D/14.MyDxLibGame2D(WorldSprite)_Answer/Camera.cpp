// 2016 Takeru Yui All Rights Reserved.
#include "DxLib.h"
#include <vector>
#include "GlobalConstants.h"
#include "Camera.h"

/// <summary>
/// コンストラクタ
/// </summary>
Camera::Camera()
{
	//奥行0.1～1000までをカメラの描画範囲とする
	SetCameraNearFar(CameraNear, CameraFar);

	// 透視投影のカメラをセットアップする（視野角の設定）
	float fovRadian = FovDegree * (DX_PI_F / 180.0f);
	SetupCamera_Perspective(fovRadian);	// ラジアンに変換して使う

	// ポジションとターゲットからカメラの回転とポジションを同時設定する
	// カメラのポジションは３Dスクリーンの中心からZ-方向に少し離れたところ
	// カメラのターゲットは３Dスクリーンの中心
	VECTOR pos		= VGet(Graphics::ScreenW * 0.5f, Graphics::ScreenH * 0.5f, 0);
	VECTOR target	= VGet(Graphics::ScreenW * 0.5f, Graphics::ScreenH * 0.5f, 0);
	pos = Graphics::Get3DPosition(pos);
	pos = VGet(pos.x, pos.y, pos.z + CameraZPos);
	target = Graphics::Get3DPosition(target);
	SetCameraPositionAndTarget_UpVecY(pos, target);
}

/// <summary>
/// デストラクタ
/// </summary>
Camera::~Camera()
{
	// 処理なし
}

#if _DEBUG
/// <summary>
/// デバッグ描画
/// </summary>
void Camera::DrawDebug()
{
	//////////////////////////////////////////////////////////////////
	// 3Dの原点にXYZ座標系ラインを「本来の1のサイズで」表示
	//////////////////////////////////////////////////////////////////
	VECTOR zero = VGet(0, 0, 0);
	DrawLine3D(zero, VGet(1, 0, 0), 0xff0000);	// X
	DrawLine3D(zero, VGet(0, 1, 0), 0x00ff00);	// Y
	DrawLine3D(zero, VGet(0, 0, 1), 0x0000ff);	// Z

	//////////////////////////////////////////////////////////////////
	// 3DのZ=0平面に2Dスクリーンサイズを3Dに変換したラインを表示
	//////////////////////////////////////////////////////////////////
	// スクリーンの四隅座標を準備し、３Dに変換
	VECTOR LeftTop2D = VGet(0, 0, 0);
	VECTOR RightTop2D = VGet(Graphics::ScreenW, 0, 0);
	VECTOR LeftBottom2D = VGet(0, Graphics::ScreenH, 0);
	VECTOR RightBottom2D = VGet(Graphics::ScreenW, Graphics::ScreenH, 0);
	VECTOR LeftTop3D = Graphics::Get3DPosition(LeftTop2D);
	VECTOR RightTop3D = Graphics::Get3DPosition(RightTop2D);
	VECTOR LeftBottom3D = Graphics::Get3DPosition(LeftBottom2D);
	VECTOR RightBottom3D = Graphics::Get3DPosition(RightBottom2D);

	// スクリーンの四隅座標を使い、２Dスクリーンの変換後の座標を画面に表示
	DrawLine3D(LeftTop3D, RightTop3D, Debug::Virtual2DScreenColor);	// 上辺
	DrawLine3D(LeftBottom3D, RightBottom3D, Debug::Virtual2DScreenColor);	// 下辺
	DrawLine3D(RightTop3D, RightBottom3D, Debug::Virtual2DScreenColor);	// 右辺
	DrawLine3D(LeftTop3D, LeftBottom3D, Debug::Virtual2DScreenColor);	// 左辺
}
#endif // _DEBUG
