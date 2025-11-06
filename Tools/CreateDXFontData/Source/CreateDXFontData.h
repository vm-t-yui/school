#ifndef CREATEFONTDATA_H
#define CREATEFONTDATA_H

#include <windows.h>

/* バージョン情報

0      初期バージョン
1      カーニング情報追加

*/

/* データマップ

FONTDATAFILEHEADER

圧縮されて格納
	FONTDATAFILEPRESSHEADER
	FONTDATAFILECHARADATA × 文字の数
	FONTDATAFILEKERNINGPAIRDATA × カーニングペアの数	

フォントイメージデータ

*/

// バージョン
#define DX_FONTDATAFILE_VERSION						(1)

// 文字セット
#define DX_CHARSET_DEFAULT							(0)					// デフォルト文字セット
#define DX_CHARSET_SHFTJIS							(1)					// 日本語文字セット
#define DX_CHARSET_HANGEUL							(2)					// 韓国語文字セット
#define DX_CHARSET_BIG5								(3)					// 繁体文字セット
#define DX_CHARSET_GB2312							(4)					// 簡体文字セット
#define DX_CHARSET_WINDOWS_1252						(5)					// 欧文(ラテン文字の文字コード)
#define DX_CHARSET_ISO_IEC_8859_15					(6)					// 欧文(ラテン文字の文字コード)
#define DX_CHARSET_UTF8								(7)					// UTF8

// フォントの元イメージのデータタイプ
#define DX_FONT_SRCIMAGETYPE_1BIT					(0)					// 1ピクセル1ビット
#define DX_FONT_SRCIMAGETYPE_1BIT_SCALE4			(1)					// 1ピクセル1ビット、画像解像度4倍
#define DX_FONT_SRCIMAGETYPE_1BIT_SCALE8			(2)					// 1ピクセル1ビット、画像解像度8倍
#define DX_FONT_SRCIMAGETYPE_1BIT_SCALE16			(3)					// 1ピクセル1ビット、画像解像度16倍
#define DX_FONT_SRCIMAGETYPE_4BIT_MAX15				(4)					// 1ピクセル4ビット、値の範囲は0～15
#define DX_FONT_SRCIMAGETYPE_8BIT_ON_OFF			(5)					// 1ピクセル8ビット、値の範囲は0又は0以外
#define DX_FONT_SRCIMAGETYPE_8BIT_MAX16				(6)					// 1ピクセル8ビット、値の範囲は0～16
#define DX_FONT_SRCIMAGETYPE_8BIT_MAX64				(7)					// 1ピクセル8ビット、値の範囲は0～64
#define DX_FONT_SRCIMAGETYPE_8BIT_MAX255			(8)					// 1ピクセル8ビット、値の範囲は0～255

// フォント画像の階調ビット数
#define DX_FONTIMAGE_BIT_1							(0)
#define DX_FONTIMAGE_BIT_4							(1)
#define DX_FONTIMAGE_BIT_8							(2)

#define DEFAULT_FONT_SIZE							(16)				// フォントのデフォルトのサイズ
#define DEFAULT_FONT_THICKNESS						(6)					// フォントのデフォルトの太さ

// フォント列挙時用データ構造体
struct ENUMFONTDATA
{
	wchar_t *				FontBuffer ;
	int						FontNum ;
	int						BufferNum ;
	const wchar_t *			EnumFontName ;
} ;

// フォントの基本情報
struct FONTBASEINFO
{
	WORD					FontThickness ;						// フォントの太さ
	WORD					FontSize ;							// 描画するフォントのサイズ
	WORD					FontHeight ;						// 描画するフォントの最大縦幅
	WORD					FontAddHeight ;						// サイズ調整の為に足した高さ
	BYTE					Italic ;							// イタリックフォントかどうか(TRUE:イタリック FALSE:非イタリック)
	BYTE					Padding ;
	WORD					MaxWidth ;							// フォントの最大幅
	WORD					CharSet ;							// キャラクタセット
	WORD					CharCodeFormat ;					// コードページ
	WORD					Ascent ;							// 文字のベースラインから一番上までの高さ
	WORD					Padding2 ;
	DWORD					KerningPairNum ;					// カーニングペアの数
	DWORD					Padding3[ 10 ] ;
} ;

// フォントファイルのヘッダ情報の圧縮する部分
struct FONTDATAFILEPRESSHEADER
{
	WORD					FontName[ 128 ] ;	// フォントの名前( UTF-16LE )
	FONTBASEINFO			BaseInfo ;			// 基本情報
	BYTE					ImageBitDepth ;		// 画像のビット深度( DX_FONTIMAGE_BIT_1 等 )
	BYTE					Padding[ 3 ] ;
	DWORD					Padding2[ 16 ] ;
} ;

// フォントファイルのヘッダ情報
struct FONTDATAFILEHEADER
{
	BYTE					Magic[ 4 ] ;		// FNTF
	WORD					Version ;			// バージョン
	WORD					Padding ;
	DWORD					ImageAddress ;		// 画像データが保存されている先頭アドレス
	DWORD					MaxImageBytes ;		// 画像データの最大サイズ
	DWORD					CharaNum ;			// 文字情報の数
	DWORD					CharaExNum ;		// 文字情報の内、文字コードが 0x10000 以上の文字の数
	FONTDATAFILEPRESSHEADER	Press ;				// ヘッダファイル内の圧縮対象の部分
} ;

// カーニングペア情報
struct FONTDATAFILEKERNINGPAIRDATA
{
	DWORD					First ;				// ペアの１文字目
	DWORD					Second ;			// ペアの２文字目
	int						KernAmount ;		// ずらすドット数
	DWORD					Padding ;
} ;

// ファイル保存用のフォント１文字の情報
struct FONTDATAFILECHARADATA
{
	DWORD					CodeUnicode ;		// Unicode の文字コード
	BYTE					Press ;				// 圧縮しているかどうか( 1:圧縮している  0:圧縮していない )
	BYTE					Padding ;
	short					DrawX ;				// 文字画像を描画すべきＸ座標
	short					DrawY ;				// 文字画像を描画すべきＹ座標
	short					AddX ;				// 次の文字を描画すべき相対Ｘ座標
	WORD					SizeX ;				// 文字のＸサイズ
	WORD					SizeY ;				// 文字のＹサイズ
	DWORD					ImageAddress ;		// 画像データが保存されているアドレス( FONTDATAFILEHEADER.ImageAddress の示すアドレスを 0 とする )
	DWORD					ImagePitch ;		// 画像データの１ラインあたりのバイト数
	DWORD					Padding2[ 2 ] ;
} ;

// フォントキャッシュの管理データ
struct FONTMANAGE
{
	wchar_t					FontName[ 128 ] ;					// フォント名
	FONTBASEINFO			BaseInfo ;							// 基本情報
	int						ImageBitDepth ;						// フォント画像のビット深度( DX_FONTIMAGE_BIT_1 など )

	int						UseTextOut ;						// TextOut を使用するタイプのフォントかどうか( TRUE:TextOutを使用する  FALSE:GetGlyphOutline を使用する )
	HFONT					FontObj ;							// フォントのオブジェクトデータ
	HBITMAP					CacheBitmap ;						// テキストキャッシュ用ビットマップ
	unsigned char			*CacheBitmapMem ;					// テキストキャッシュ用ビットマップのメモリ
	int						CacheBitmapMemPitch ;				// テキストキャッシュ用ビットマップのピッチ

	FONTDATAFILECHARADATA	CharaData ;							// １文字分のデータ

	FONTDATAFILEKERNINGPAIRDATA * KerningPairData ;				// カーニングペア情報

	void *					GetGlyphOutlineBuffer ;				// GetGlyphOutline のデータ取得用に使用するバッファ
	DWORD					GetGlyphOutlineBufferSize ;			// GetGlyphOutline のデータ取得用に使用するバッファのサイズ

	unsigned char			*CacheMem ;							// テキストキャッシュメモリ
	int						CachePitch ;						// テキストキャッシュメモリのピッチ	
	int						CacheDataBitNum ;					// テキストキャッシュ上の１ピクセルのビット数
} ;

// Windows用 フォントシステム用構造体
struct FONTSYSTEM_WIN
{
	HDC						Devicecontext ;						// FontCacheCharAddToHandle の処理で使用するデバイスコンテキスト
	HFONT					OldFont ;							// FontCacheCharAddToHandle の処理で使用するフォント
	HBITMAP					OldBitmap ;							// FontCacheCharAddToHandle の処理で使用するビットマップ
	TEXTMETRIC				TextMetric ;						// FontCacheCharAddToHandle の処理で使用するフォントの情報
} ;

// フォントシステム用構造体
struct FONTSYSTEM
{
	BYTE					BitCountTable[ 256 ] ;				// ビットカウントテーブル
} ;

// フォント処理の初期化
extern int InitializeFont( void ) ;

// フォントデータの作成
extern FONTMANAGE *CreateFontManageData(
	const wchar_t *	FontName,
	int				Size,
	int				ImageBitDepth /* DX_FONTIMAGE_BIT_1等 */ , 
	int				Thick,
	int				Italic = FALSE ,
	int				CharSet = -1,
	int				CharCodeFormat = -1
) ;

// フォントデータの後始末
extern int DeleteFontManageData( FONTMANAGE *ManageData ) ;

// 文字キャッシュの文字を変更する
extern int FontCacheCharChange( FONTMANAGE *ManageData, const wchar_t *CharCode ) ;

// フォントデータファイルの作成
extern int CreateFontDataFile(
	const wchar_t *	SaveFilePath,
	const wchar_t *	FontName,
	int				Size,
	int				ImageBitDepth /* DX_FONTIMAGE_BIT_1等 */ , 
	int				Thick,
	int				Italic = FALSE ,
	int				CharSet = -1,
	int				CharCodeFormat = -1,
	const wchar_t *	SaveCharaList = NULL
) ;

#endif // CREATEFONTDATA_H

