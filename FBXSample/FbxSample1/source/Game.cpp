//-----------------------------------------------------------------------------
// @brief  ゲームのメインクラス.
// Copyright (c) 2017 VikingMaxx Takeru Yui. All rights reserved.
//-----------------------------------------------------------------------------
#include "vmlib.h"
#include "Game.h"
#include "Camera.h"
#include "VMMesh.h"

//-----------------------------------------------------------------------------
// @brief  コンストラクタ.
//-----------------------------------------------------------------------------
Game::Game()
	:  mesh(NULL)
	, camera(NULL)
{
	// 処理なし.
}

//-----------------------------------------------------------------------------
// @brief  デストラクタ.
//-----------------------------------------------------------------------------
Game::~Game()
{
	Finalize();
}

//-----------------------------------------------------------------------------
// @brief  初期化.
//-----------------------------------------------------------------------------
void Game::Initialize()
{
	mesh = new VMMesh();
    //mesh->Initialize("data/test/", "xi_sq");
    mesh->Initialize("data/test/", "xi_tri");
    //mesh->Initialize("data/test/", "box_sq");
    //mesh->Initialize("data/test/", "box_tri");
    //mesh->Initialize("data/test/", "cone");
    //mesh->Initialize("data/unitychan/", "SD_unitychan_generic");
    //mesh->Initialize("data/miku/", "miku");

	camera = new Camera();
	camera->Initialize();
}

//-----------------------------------------------------------------------------
// @brief  終了処理.
//-----------------------------------------------------------------------------
void Game::Finalize()
{
	if (camera != NULL)
	{
		camera->Finalize();
		delete camera;
		camera = NULL;
	}
	if (mesh != NULL)
	{
		mesh->Finalize();
		delete mesh;
		mesh = NULL;
	}
}

//-----------------------------------------------------------------------------
// @brief  アップデート.
//-----------------------------------------------------------------------------
void Game::Update()
{
}

//-----------------------------------------------------------------------------
// @brief  描画.
//-----------------------------------------------------------------------------
void Game::Render()
{
	// バックバッファを青色でクリアする.
    GetDxDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	// シーン描画を開始する.
	if (SUCCEEDED(GetDxDevice()->BeginScene()))
	{
		// カメラ設定.
		camera->SetCameraMatrices();

		// マトリクス設定.
		mesh->SetWorldMatrices();

		// レンダリング.
		mesh->Render();

		// シーン描画を終了する.
		GetDxDevice()->EndScene();
	}

	// 実際の描画を確定する.
	GetDxDevice()->Present(NULL, NULL, NULL, NULL);
}
