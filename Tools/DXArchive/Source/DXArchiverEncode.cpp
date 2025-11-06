// ============================================================================
//
//			ＤＸＡファイル作成ソフト
//
//	Creator			: 山田　巧
//	Creation Date	: 2003/09/13
//	Version			: 1.08
//
// ============================================================================

// include ----------------------------
#include "DXArchive.h"
#include "FileLib.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <direct.h>

// define -----------------------------
#define VERSION			"1.09"				// バージョン
#define FILETYPE		"dxa"				// 拡張子

// code -------------------------------

// メイン関数
int main( int argc, char *argv[] )
{
	char SrcPath[256], DestPath[256], *KeyString, *SrcDirectoryPath, *DestFilePath ;
	bool Press ;
	bool MaxPress ;
	bool NoOutput ;
	bool NoKey ;
	bool AlwaysHuffman ;
	int HuffmanSize ;
	int i ;

	printf( "ＤＸＡファイル簡易作成ソフト  Ver%s\n", VERSION ) ;

	// 引数が一個だったらヘルプを出す
	if( argc == 1 ) goto ERR ;

	Press = true ;
	MaxPress = false ;
	NoOutput = false ;
	NoKey = false ;
	HuffmanSize = 10 ;
	AlwaysHuffman = false ;
	SrcDirectoryPath = NULL ;
	DestFilePath = NULL ;
	KeyString = NULL ;
	for( i = 1 ; i < argc ; i ++ )
	{
		if( argv[i][0] == '-' && argv[i][1] == 'K' && argv[i][2] == ':' )
		{
			KeyString = argv[i] + 3 ;
		}
		else
		if( argv[i][0] == '-' && argv[i][1] == 'N' && argv[i][2] == 'P' && argv[i][3] == '\0' )
		{
			Press = false ;
		}
		else
		if( argv[i][0] == '-' && argv[i][1] == 'N' && argv[i][2] == 'S' && argv[i][3] == '\0' )
		{
			NoOutput = true ;
		}
		else
		if( argv[i][0] == '-' && argv[i][1] == 'N' && argv[i][2] == 'K' && argv[i][3] == '\0' )
		{
			NoKey = true ;
		}
		else
		if( argv[i][0] == '-' && argv[i][1] == 'H' && argv[i][2] == ':' )
		{
			HuffmanSize = atoi( argv[i] + 3 ) ;
			if( HuffmanSize < 0 )
			{
				HuffmanSize = 0 ;
			}
			else
			if( HuffmanSize > 255 )
			{
				HuffmanSize = 255 ;
			}
		}
		else
		if( argv[i][0] == '-' && argv[i][1] == 'A' && argv[i][2] == 'H' && argv[i][3] == '\0' )
		{
			AlwaysHuffman = true ;
		}
		else
		if( argv[i][0] == '-' && argv[i][1] == 'M' && argv[i][2] == 'P' && argv[i][3] == '\0' )
		{
			MaxPress = true ;
		}
		else
		if( SrcDirectoryPath == NULL )
		{
			SrcDirectoryPath = argv[i] ;
		}
		else
		{
			DestFilePath = argv[i] ;
		}
	}

	if( SrcDirectoryPath == NULL )
		goto ERR ;

	// ソースパスのフルパスを作成する
	ConvertFullPath__( SrcDirectoryPath, SrcPath ) ;

	// 出力ファイルパスの作成
	{
		if( DestFilePath != NULL )
		{
			char temp[256] ;

			strcpy( temp, DestFilePath ) ;
			ConvertFullPath__( temp, DestPath ) ;
		}
		else
		{
			// ソースパスから出力ファイル名を作成する
			sprintf( DestPath, "%s.%s", SrcPath, FILETYPE ) ;
		}
	}

	// アーカイブファイルを作成する
	DXArchive::EncodeArchiveOneDirectory( DestPath, SrcPath, Press, AlwaysHuffman, ( u8 )HuffmanSize, KeyString, NoKey, NoOutput ? false : true, MaxPress ) ;

	// 終了
	return 0 ;

ERR:

	printf( "DxaEncode.exe [Option] SrcDirectoryPath  [DestFilePath]\n\n" ) ;
	printf( "・ＤＸＡファイルにしたいフォルダをドラッグ＆ドロップするだけでも作成可能です\n" ) ; 
	printf( "・-K  オプションで鍵を設定出来ます(最大半角63文字) 　例  -K:KeyCode \n" ) ; 
	printf( "・-NP アーカイブ時に圧縮を行わない\n" ) ;
	printf( "・-NS 進行状況を表示しない\n" ) ;
	printf( "・-NK 暗号化処理を施さない\n" ) ;
	printf( "・-H  ファイルの先端と末端をハフマン圧縮するサイズ(単位:キロバイト)( 0 ～ 255 )\n" ) ;
	printf( "      デフォルトは10　　例  -H:20  ( 尚、-H:255 でファイル全体をハフマン圧縮 )\n" ) ;
	printf( "・-AH 全てのファイルフォーマットをハフマン圧縮の対象とする\n" ) ;
	printf( "      ( デフォルトでは png や wav などメジャーなファイルフォーマットのみ対象 )\n" ) ;
	printf( "・-MP 独自圧縮の圧縮率を最大にする\n" ) ;
	return -1; 
}


