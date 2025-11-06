#include "CreateDXFontData.h"
#include "FileLib.h"
#include "CharCode.h"
#include <stdio.h>
#include <malloc.h>
#include <stdarg.h>
#include <locale.h>

int wmain( int argc, wchar_t *argv[] )
{
	int i ;
	wchar_t *FontName = NULL ;
	int FontSize      = -1 ;
	int FontThickness = -1 ;
	int FontBitDepth  = DX_FONTIMAGE_BIT_8 ;
	int FontItalic    = FALSE ;
	int FontCharSet   = -1 ;
	int FontCharCodeFormat = -1 ;
	wchar_t *FontSaveCharaListFilePath = NULL ;
	wchar_t *SaveFilePath = NULL ;
	wchar_t *SaveCharaList = NULL ;

	setlocale( LC_ALL, "Japanese" ) ;

	wprintf( L"ＤＸフォントデータファイル作成ソフト  Ver0.5   Yamada Takumi\n" ) ;

	for( i = 1; i < argc; i ++ )
	{
		if( wcsncmp( argv[ i ], L"-?", 2 ) == 0 ||
			wcsncmp( argv[ i ], L"/?", 2 ) == 0 )
		{
			wprintf( L"CreateDXFontData [/?][/Fフォント名][/Sサイズ][/T太さ][/Bビット数][/I][/C文字セット][/Aテキストファイルパス][/Oフォントデータファイルパス]\n" ) ;
			wprintf( L"  /? ヘルプ\n" ) ;
			wprintf( L"  /F フォント名\n" ) ;
			wprintf( L"  /S サイズ\n" ) ;
			wprintf( L"  /T 太さ\n" ) ;
			wprintf( L"  /B １ピクセルのビット数( 1 or 4 or 8 )\n" ) ;
			wprintf( L"  /I イタリックにします\n" ) ;
			wprintf( L"  /C 文字セット\n" ) ;
			wprintf( L"       DEFAULT  デフォルト文字\n" ) ;
			wprintf( L"       SHIFTJIS シフトJIS文字\n" ) ;
			wprintf( L"       HANGEUL  ハングル文字\n" ) ;
			wprintf( L"       BIG5     繁体文字\n" ) ;
			wprintf( L"       GB2312   簡体文字\n" ) ;
			wprintf( L"       UTF8     UTF-8\n" ) ;
			wprintf( L"  /A ＤＸフォントデータファイルに含める文字の一覧が載ったテキストファイルのパスの指定、対応形式は「シフトJIS(メモ帳では「ANSI」)」「UTF-16LE(メモ帳では「Unicode」)」の２種類です\n" ) ;
			wprintf( L"  /O ＤＸフォントデータファイルの保存ファイルパス\n" ) ;
			return 0 ;
		}
		else
		if( wcsncmp( argv[ i ], L"-F", 2 ) == 0 ||
			wcsncmp( argv[ i ], L"/F", 2 ) == 0 )
		{
			FontName = argv[ i ] + 2 ;
		}
		else
		if( wcsncmp( argv[ i ], L"-S", 2 ) == 0 ||
			wcsncmp( argv[ i ], L"/S", 2 ) == 0 )
		{
			FontSize = CL_atoi( CHARCODEFORMAT_UTF16LE, ( char * )argv[ i ] + 2 ) ;
		}
		else
		if( wcsncmp( argv[ i ], L"-T", 2 ) == 0 ||
			wcsncmp( argv[ i ], L"/T", 2 ) == 0 )
		{
			FontThickness = CL_atoi( CHARCODEFORMAT_UTF16LE, ( char * )argv[ i ] + 2 ) ;
		}
		else
		if( wcsncmp( argv[ i ], L"-B", 2 ) == 0 ||
			wcsncmp( argv[ i ], L"/B", 2 ) == 0 )
		{
			switch( CL_atoi( CHARCODEFORMAT_UTF16LE, ( char * )argv[ i ] + 2 ) )
			{
			default :
			case 1 :
				FontBitDepth = DX_FONTIMAGE_BIT_1 ;
				break ;

			case 4 :
				FontBitDepth = DX_FONTIMAGE_BIT_4 ;
				break ;

			case 8 :
				FontBitDepth = DX_FONTIMAGE_BIT_8 ;
				break ;
			}
		}
		else
		if( wcsncmp( argv[ i ], L"-I", 2 ) == 0 ||
			wcsncmp( argv[ i ], L"/I", 2 ) == 0 )
		{
			FontItalic = TRUE;
		}
		else
		if( wcsncmp( argv[ i ], L"-C", 2 ) == 0 ||
			wcsncmp( argv[ i ], L"/C", 2 ) == 0 )
		{
			if( wcscmp( argv[ i ] + 2, L"DEFAULT" ) == 0 )
			{
				FontCharSet = DX_CHARSET_DEFAULT ;
			}
			else
			if( wcscmp( argv[ i ] + 2, L"SHIFTJIS" ) == 0 )
			{
				FontCharSet = DX_CHARSET_SHFTJIS ;
				FontCharCodeFormat = CHARCODEFORMAT_SHIFTJIS ;
			}
			else
			if( wcscmp( argv[ i ] + 2, L"HANGEUL" ) == 0 )
			{
				FontCharSet = DX_CHARSET_HANGEUL ;
				FontCharCodeFormat = CHARCODEFORMAT_UHC ;
			}
			else
			if( wcscmp( argv[ i ] + 2, L"BIG5" ) == 0 )
			{
				FontCharSet = DX_CHARSET_BIG5 ;
				FontCharCodeFormat = CHARCODEFORMAT_BIG5 ;
			}
			else
			if( wcscmp( argv[ i ] + 2, L"GB2312" ) == 0 )
			{
				FontCharSet = DX_CHARSET_GB2312 ;
				FontCharCodeFormat = CHARCODEFORMAT_GB2312 ;
			}
			else
			if( wcscmp( argv[ i ] + 2, L"WINDOWS_1252" ) == 0 )
			{
				FontCharSet = DX_CHARSET_WINDOWS_1252 ;
				FontCharCodeFormat = CHARCODEFORMAT_WINDOWS_1252 ;
			}
			else
			if( wcscmp( argv[ i ] + 2, L"ISO_IEC_8859_15" ) == 0 )
			{
				FontCharSet = DX_CHARSET_ISO_IEC_8859_15 ;
				FontCharCodeFormat = CHARCODEFORMAT_ISO_IEC_8859_15 ;
			}
			else
			if( wcscmp( argv[ i ] + 2, L"UTF8" ) == 0 )
			{
				FontCharSet = DX_CHARSET_UTF8 ;
				FontCharCodeFormat = CHARCODEFORMAT_UTF8 ;
			}
		}
		else
		if( wcsncmp( argv[ i ], L"-A", 2 ) == 0 ||
			wcsncmp( argv[ i ], L"/A", 2 ) == 0 )
		{
			FontSaveCharaListFilePath = argv[ i ] + 2 ;
		}
		else
		if( wcsncmp( argv[ i ], L"-O", 2 ) == 0 ||
			wcsncmp( argv[ i ], L"/O", 2 ) == 0 )
		{
			SaveFilePath = argv[ i ] + 2 ;
		}
	}

	if( SaveFilePath == NULL )
	{
		SaveFilePath = L"FontData.dft" ;
	}

	if( FontSaveCharaListFilePath != NULL )
	{
		FILE *fp ;
		long FileSize ;
		BYTE *FileBuffer = NULL ;

		fp = _wfopen( FontSaveCharaListFilePath, L"rb" ) ;
		if( fp != NULL )
		{
			fseek( fp, 0, SEEK_END ) ;
			FileSize = ftell( fp ) ;
			fseek( fp, 0, SEEK_SET ) ;
			FileBuffer = ( BYTE * )malloc( FileSize + 16 ) ;
			memset( FileBuffer, 0, FileSize + 16 ) ;
			if( FileBuffer != NULL )
			{
				fread( FileBuffer, FileSize, 1, fp ) ;

				if( FileBuffer[ 0 ] == 0xff && FileBuffer[ 1 ] == 0xfe )
				{
					SaveCharaList = ( wchar_t * )malloc( sizeof( wchar_t ) * ( FileSize / 2 + 16 ) ) ;
					memset( SaveCharaList, 0, sizeof( wchar_t ) * ( FileSize / 2 + 16 ) ) ;
					memcpy( SaveCharaList, FileBuffer + 2, FileSize - 2 ) ;
				}
				else
				if( FileBuffer[ 0 ] == 0xfe && FileBuffer[ 1 ] == 0xff )
				{
					SaveCharaList = ( wchar_t * )malloc( sizeof( wchar_t ) * ( FileSize / 2 + 16 ) ) ;
					memset( SaveCharaList, 0, sizeof( wchar_t ) * ( FileSize / 2 + 16 ) ) ;
					ConvString( ( const char * )FileBuffer + 2, CHARCODEFORMAT_UTF16BE, ( char * )SaveCharaList, CHARCODEFORMAT_UTF16LE ) ;
				}
				else
				if( FileBuffer[ 0 ] == 0xef && FileBuffer[ 1 ] == 0xbb && FileBuffer[ 2 ] == 0xbf )
				{
					SaveCharaList = ( wchar_t * )malloc( sizeof( wchar_t ) * ( FileSize + 16 ) ) ;
					memset( SaveCharaList, 0, sizeof( wchar_t ) * ( FileSize + 16 ) ) ;
					ConvString( ( const char * )FileBuffer + 3, CHARCODEFORMAT_UTF8, ( char * )SaveCharaList, CHARCODEFORMAT_UTF16LE ) ;
				}
				else
				{
					SaveCharaList = ( wchar_t * )malloc( sizeof( wchar_t ) * ( FileSize + 1 ) ) ;
					memset( SaveCharaList, 0, sizeof( wchar_t ) * ( FileSize + 1 ) ) ;
					ConvString( ( const char * )FileBuffer, FontCharCodeFormat == -1 ? CHARCODEFORMAT_SHIFTJIS : FontCharCodeFormat, ( char * )SaveCharaList, CHARCODEFORMAT_UTF16LE ) ;
				}

				free( FileBuffer ) ;
			}
			fclose( fp ) ;
		}
	}

	InitializeFont() ;

	CreateFontDataFile( SaveFilePath, FontName, FontSize, FontBitDepth, FontThickness, FontItalic, FontCharSet, FontCharCodeFormat, SaveCharaList ) ;

	if( SaveCharaList != NULL )
	{
		free( SaveCharaList ) ;
		SaveCharaList = NULL ;
	}

	return 0;
}
