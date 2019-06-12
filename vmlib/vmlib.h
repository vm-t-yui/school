//-----------------------------------------------------------------------------
// @brief  Viking Maxxライブラリ。このライブラリはVikingMaxxが学校教育に使用するために制作したライブラリです.
// Copyright (c) 2017 VikingMaxx Takeru Yui. All rights reserved.
//-----------------------------------------------------------------------------

#pragma once
#include <Windows.h>
#include <mmsystem.h>
#include <d3dx9.h>
#include <string>
#include <vector>
#include <list>
#include <stdio.h>
#include <wchar.h>
#include <tchar.h>
#include <locale.h>  
#include "assert.h"
#include "fbxsdk.h"
#include "Device.h"
#include "VMFbxManager.h"

extern  LPDIRECT3DDEVICE9   GetDxDevice();                                          // DirectXデバイスを取得する.
extern  VMFbxManager*       GetFbxManager();                                        // FBXマネージャを取得する.

// 初期化関数.
extern	int			VMLib_Init();													// ライブラリ初期化を行う
extern	int			VMLib_End();													// ライブラリ使用の後始末を行う

// メッセージ処理関数
extern	int			ProcessMessage();												// ウインドウズのメッセージループに代わる処理を行う

static TCHAR debugStrBuf[2048];
static TCHAR warnStrBuf[2048];
static WCHAR debugStrBufW[2048];

// デバッグログ関数.
#ifdef _DEBUG
#   define Log( str, ... ) \
      { \
        sprintf( debugStrBuf, str, __VA_ARGS__ ); \
        OutputDebugString( debugStrBuf ); \
      }
#else
#    define Log( str, ... ) // 空実装
#endif
#ifdef _DEBUG
#   define LogWarn( str, ... ) \
      { \
        sprintf( debugStrBuf, str, __VA_ARGS__ ); \
        sprintf( warnStrBuf, "【WARN】:%s", debugStrBuf); \
        OutputDebugString( warnStrBuf ); \
      }
#else
#    define Log( str, ... ) // 空実装
#endif

// デバッグassert関数.
#ifdef _DEBUG
#   define LogAssert( str, ... ) \
      { \
        sprintf( debugStrBuf, str, __VA_ARGS__ ); \
        size_t wLen = 0; \
        setlocale(LC_ALL, "japanese"); \
        mbstowcs_s(&wLen, debugStrBufW, 2048, debugStrBuf, _TRUNCATE); \
        _ASSERT_EXPR(0, debugStrBufW); \
      }
#else
#    define Log( str, ... ) // 空実装
#endif
