// ３Ｄアクション基本
// 
//＜注意＞
//このサンプルプログラムで行っているステージモデルとの当たり判定処理はステージモデルが以下の項目を考慮して
//作成されていることを前提としています。
//この項目を考慮していないステージモデルでは壁や床のすり抜けや嵌まり現象が発生しますので、注意してください。
//・壁としたい面は床に対して完全に垂直にしてください(少しでも垂直ではないと「坂」として処理されます)
//・壁ポリゴンが水平方向の差があまりない状態で重なっているとめり込むことがあります
//・床ポリゴンがＹ軸方向の差があまりない状態で重なっているとめり込むことがあります
//・９０度以下の鋭角なカドをがあると、外側から当たっても内側から当たっても、すり抜けや嵌りが発生することがあります

#include "DxLib.h"
#include <math.h>

// 固定値定義 -------------------------------------------------------------------------------------

// プレイヤー関係の定義
#define PLAYER_PLAY_ANIM_SPEED			250.0f		// アニメーション速度
#define PLAYER_MOVE_SPEED				30.0f		// 移動速度
#define PLAYER_ANIM_BLEND_SPEED			0.1f		// アニメーションのブレンド率変化速度
#define PLAYER_ANGLE_SPEED				0.2f		// 角度変化速度
#define PLAYER_JUMP_POWER				100.0f		// ジャンプ力

#define PLAYER_FALL_UP_POWER			20.0f		// 足を踏み外した時のジャンプ力

#define PLAYER_GRAVITY					3.0f		// 重力
#define PLAYER_MAX_HITCOLL				2048		// 処理するコリジョンポリゴンの最大数
#define PLAYER_ENUM_DEFAULT_SIZE		800.0f		// 周囲のポリゴン検出に使用する球の初期サイズ
#define PLAYER_HIT_WIDTH				200.0f		// 当たり判定カプセルの半径
#define PLAYER_HIT_HEIGHT				700.0f		// 当たり判定カプセルの高さ
#define PLAYER_HIT_TRYNUM				16			// 壁押し出し処理の最大試行回数
#define PLAYER_HIT_SLIDE_LENGTH			5.0f		// 一度の壁押し出し処理でスライドさせる距離
#define PLAYER_SHADOW_SIZE				200.0f		// 影の大きさ
#define PLAYER_SHADOW_HEIGHT			700.0f		// 影が落ちる高さ

// カメラ関係の定義
#define CAMERA_ANGLE_SPEED				0.05f		// 旋回速度
#define CAMERA_PLAYER_TARGET_HEIGHT		400.0f		// プレイヤー座標からどれだけ高い位置を注視点とするか
#define CAMERA_PLAYER_LENGTH			1600.0f		// プレイヤーとの距離
#define CAMERA_COLLISION_SIZE			50.0f		// カメラの当たり判定サイズ



// 構造体定義 -------------------------------------------------------------------------------------

// 入力情報構造体
struct PADINPUT
{
	int		NowFrameInput;				// 現在のフレームで押されているボタン
	int		NowFrameNewInput;			// 現在のフレームで新たに押されたボタン
};

// プレイヤーの状態
enum class PlayerState : int
{
	STAND	= 0,		// 立ち止まり
	RUN		= 1,		// 走り
	JUMP	= 2,		// ジャンプ
};

// プレイヤーのアニメーション種別
enum class PlayerAnimKind : int
{
	NONE	= -1,		// なし
	UNKNOWN	= 0,		// 不明
	RUN		= 1,		// 走り
	JUMP	= 2,		// ジャンプ
	FALL	= 3,		// 落下中
	STOP	= 4,		// 立ち止まり
};

// プレイヤー情報構造体
struct PLAYER
{
	VECTOR		Position;				// 座標
	VECTOR		TargetMoveDirection;	// モデルが向くべき方向のベクトル
	float		Angle;					// モデルが向いている方向の角度
	float		JumpPower;				// Ｙ軸方向の速度
	int			ModelHandle;			// モデルハンドル
	int			ShadowHandle;			// 影画像ハンドル
	PlayerState	State;					// 状態

	int			CurrentPlayAnim;		// 再生しているアニメーション１のアタッチ番号( -1:何もアニメーションがアタッチされていない )
	float		CurrentAnimCount;		// 再生しているアニメーション１の再生時間
	int			PrevPlayAnim;			// 再生しているアニメーション２のアタッチ番号( -1:何もアニメーションがアタッチされていない )
	float		PrevAnimCount;			// 再生しているアニメーション２の再生時間
	float		AnimBlendRate;			// 再生しているアニメーション１と２のブレンド率
};

// ステージ情報構造体
struct STAGE
{
	int		ModelHandle;				// モデルハンドル
};

// カメラ情報構造体
struct CAMERA
{
	float		AngleH;					// 水平角度
	float		AngleV;					// 垂直角度
	VECTOR		Eye;					// カメラ座標
	VECTOR		Target;					// 注視点座標
};


// 実体宣言 ---------------------------------------------------------------------------------------
PADINPUT	input;			// 入力情報の実体宣言
PLAYER		player;			// プレイヤー情報の実体宣言
STAGE		stage;			// ステージ情報の実体宣言
CAMERA		camera;			// カメラ情報の実体宣言

// HACK: ProcessはUpdateのような毎フレーム呼ばれる処理だが、Drawも呼ばれているので固有名称になっている
//
// 関数プロトタイプ宣言 ---------------------------------------------------------------------------
void Input_Process();							// 入力処理
void Player_Initialize();						// プレイヤーの初期化
void Player_Terminate();						// プレイヤーの後始末
void Player_Process();							// プレイヤーの処理
void Player_Move(VECTOR MoveVector);			// プレイヤーの移動処理
void Player_UpdateAngle();						// プレイヤーの回転制御
void Player_PlayAnim(PlayerAnimKind PlayAnim);	// プレイヤーに新たなアニメーションを再生する
void Player_AnimProcess();						// プレイヤーのアニメーション処理
void Player_ShadowRender();						// プレイヤーの影を描画
void Stage_Initialize();						// ステージの初期化処理
void Stage_Terminate();							// ステージの後始末処理
void Camera_Initialize();						// カメラの初期化処理
void Camera_Process();							// カメラの処理
void Render_Process();							// 描画処理


// 関数実体 ---------------------------------------------------------------------------------------

// 入力処理
void Input_Process()
{
	int Old;

	// ひとつ前のフレームの入力を変数にとっておく
	Old = input.NowFrameInput;

	// 現在の入力状態を取得
	input.NowFrameInput = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	// 今のフレームで新たに押されたボタンのビットだけ立っている値を NowFrameNewInput に代入する
	input.NowFrameNewInput = input.NowFrameInput & ~Old;
}

// プレイヤーの初期化
void Player_Initialize()
{
	// 初期座標は原点
	player.Position = VGet(0.0f, 0.0f, 0.0f);

	// 回転値は０
	player.Angle = 0.0f;

	// ジャンプ力は初期状態では０
	player.JumpPower = 0.0f;

	// モデルの読み込み
	player.ModelHandle = MV1LoadModel("DxChara.x");

	// 影描画用の画像の読み込み
	player.ShadowHandle = LoadGraph("Shadow.tga");

	// 初期状態では「立ち止り」状態
	player.State = PlayerState::STAND;

	// 初期状態でプレイヤーが向くべき方向はＸ軸方向
	player.TargetMoveDirection = VGet(1.0f, 0.0f, 0.0f);

	// アニメーションのブレンド率を初期化
	player.AnimBlendRate = 1.0f;

	// 初期状態ではアニメーションはアタッチされていないにする
	player.CurrentPlayAnim = -1;
	player.PrevPlayAnim = -1;

	// ただ立っているアニメーションを再生
	// TODO: マジックナンバーの4が何なのか解析して定数化
	Player_PlayAnim(PlayerAnimKind::STOP);			// HACK: 中ではアタッチとカウンタの初期化をしているだけ
}

// プレイヤーの後始末
void Player_Terminate()
{
	// モデルの削除
	MV1DeleteModel(player.ModelHandle);

	// 影用画像の削除
	DeleteGraph(player.ShadowHandle);
}

// プレイヤーの処理
void Player_Process()
{
	VECTOR	UpMoveVec;		// 方向ボタン「↑」を入力をしたときのプレイヤーの移動方向ベクトル
	VECTOR	LeftMoveVec;	// 方向ボタン「←」を入力をしたときのプレイヤーの移動方向ベクトル
	VECTOR	MoveVec;		// このフレームの移動ベクトル
	bool	IsMove;			// 移動したかどうかのフラグ( TRUE:移動した  FALSE:移動していない )

	// HACK:
	// ・DXライブラリのモデルファイル内には、複数のメッシュ（ポリゴンの集合）やカメラ、ライトを入れることができる
	// ・置いた複数のファイルは、親子関係をつけたり、Unityのヒエラルキーみたいに、階層構造が作れる
	// ・この階層それぞれには名前が付けられる Root-Meshes-Model1
	//											         |-Model2
	// ・この名前の付いた階層のことをDXライブラリではフレームという
	// ・一番親の階層を「ルートフレーム」と呼ぶ。ルートフレームは一つ
	// 
	// HACK: 何のために？モデルの一番親フレーム（親階層）のZ軸方向の移動パラメータをゼロにしている

	// ルートフレームのＺ軸方向の移動パラメータを無効にする
	{
		MATRIX LocalMatrix;

		// ユーザー行列を解除する
		MV1ResetFrameUserLocalMatrix(player.ModelHandle, 2);

		// 現在のルートフレームの行列を取得する
		LocalMatrix = MV1GetFrameLocalMatrix(player.ModelHandle, 2);

		// Ｚ軸方向の平行移動成分を無効にする
		LocalMatrix.m[3][2] = 0.0f;

		// ユーザー行列として平行移動成分を無効にした行列をルートフレームにセットする
		MV1SetFrameUserLocalMatrix(player.ModelHandle, 2, LocalMatrix);
	}

	// プレイヤーの移動方向のベクトルを算出
	{
		// 方向ボタン「↑」を押したときのプレイヤーの移動ベクトルはカメラの視線方向からＹ成分を抜いたもの
		UpMoveVec = VSub(camera.Target, camera.Eye);
		UpMoveVec.y = 0.0f;

		// 方向ボタン「←」を押したときのプレイヤーの移動ベクトルは上を押したときの方向ベクトルとＹ軸のプラス方向のベクトルに垂直な方向
		LeftMoveVec = VCross(UpMoveVec, VGet(0.0f, 1.0f, 0.0f));

		// 二つのベクトルを正規化( ベクトルの長さを１．０にすること )
		UpMoveVec = VNorm(UpMoveVec);
		LeftMoveVec = VNorm(LeftMoveVec);
	}

	// このフレームでの移動ベクトルを初期化
	MoveVec = VGet(0.0f, 0.0f, 0.0f);

	// 移動したかどうかのフラグを初期状態では「移動していない」を表すFALSEにする
	IsMove = false;

	// パッドの３ボタンと左シフトがどちらも押されていなかったらプレイヤーの移動処理
	if (CheckHitKey(KEY_INPUT_LSHIFT) == 0 && (input.NowFrameInput & PAD_INPUT_C) == 0)
	{
		// 方向ボタン「←」が入力されたらカメラの見ている方向から見て左方向に移動する
		if (input.NowFrameInput & PAD_INPUT_LEFT)
		{
			// 移動ベクトルに「←」が入力された時の移動ベクトルを加算する
			MoveVec = VAdd(MoveVec, LeftMoveVec);

			// 移動したかどうかのフラグを「移動した」にする
			IsMove = true;
		}
		else
			// 方向ボタン「→」が入力されたらカメラの見ている方向から見て右方向に移動する
			if (input.NowFrameInput & PAD_INPUT_RIGHT)
			{
				// 移動ベクトルに「←」が入力された時の移動ベクトルを反転したものを加算する
				MoveVec = VAdd(MoveVec, VScale(LeftMoveVec, -1.0f));

				// 移動したかどうかのフラグを「移動した」にする
				IsMove = true;
			}

		// 方向ボタン「↑」が入力されたらカメラの見ている方向に移動する
		if (input.NowFrameInput & PAD_INPUT_UP)
		{
			// 移動ベクトルに「↑」が入力された時の移動ベクトルを加算する
			MoveVec = VAdd(MoveVec, UpMoveVec);

			// 移動したかどうかのフラグを「移動した」にする
			IsMove = true;
		}
		else
			// 方向ボタン「↓」が入力されたらカメラの方向に移動する
			if (input.NowFrameInput & PAD_INPUT_DOWN)
			{
				// 移動ベクトルに「↑」が入力された時の移動ベクトルを反転したものを加算する
				MoveVec = VAdd(MoveVec, VScale(UpMoveVec, -1.0f));

				// 移動したかどうかのフラグを「移動した」にする
				IsMove = true;
			}

		// プレイヤーの状態が「ジャンプ」ではなく、且つボタン１が押されていたらジャンプする
		if (player.State != PlayerState::JUMP && (input.NowFrameNewInput & PAD_INPUT_A))
		{
			// 状態を「ジャンプ」にする
			player.State = PlayerState::JUMP;

			// Ｙ軸方向の速度をセット
			player.JumpPower = PLAYER_JUMP_POWER;

			// ジャンプアニメーションの再生
			Player_PlayAnim(PlayerAnimKind::JUMP);
		}
	}

	// 移動ボタンが押されたかどうかで処理を分岐
	if (IsMove)
	{
		// 移動ベクトルを正規化したものをプレイヤーが向くべき方向として保存
		player.TargetMoveDirection = VNorm(MoveVec);

		// プレイヤーが向くべき方向ベクトルをプレイヤーのスピード倍したものを移動ベクトルとする
		MoveVec = VScale(player.TargetMoveDirection, PLAYER_MOVE_SPEED);

		// もし今まで「立ち止まり」状態だったら
		if (player.State == PlayerState::STAND)
		{
			// 走りアニメーションを再生する
			Player_PlayAnim(PlayerAnimKind::RUN);

			// 状態を「走り」にする
			player.State = PlayerState::RUN;
		}
	}
	else
	{
		// このフレームで移動していなくて、且つ状態が「走り」だったら
		if (player.State == PlayerState::RUN)
		{
			// 立ち止りアニメーションを再生する
			Player_PlayAnim(PlayerAnimKind::STOP);

			// 状態を「立ち止り」にする
			player.State = PlayerState::STAND;
		}
	}

	// 状態が「ジャンプ」の場合は
	if (player.State == PlayerState::JUMP)
	{
		// Ｙ軸方向の速度を重力分減算する
		player.JumpPower -= PLAYER_GRAVITY;

		// もし落下していて且つ再生されているアニメーションが上昇中用のものだった場合は
		if (player.JumpPower < 0.0f && MV1GetAttachAnim(player.ModelHandle, player.CurrentPlayAnim) == 2)
		{
			// 落下中ようのアニメーションを再生する
			Player_PlayAnim(PlayerAnimKind::JUMP);
		}

		// 移動ベクトルのＹ成分をＹ軸方向の速度にする
		MoveVec.y = player.JumpPower;
	}

	// プレイヤーの移動方向にモデルの方向を近づける
	Player_UpdateAngle();

	// 移動ベクトルを元にコリジョンを考慮しつつプレイヤーを移動
	Player_Move(MoveVec);

	// アニメーション処理
	Player_AnimProcess();
}

// プレイヤーの移動処理
void Player_Move(VECTOR MoveVector)
{
	int		i, j, k;						// 汎用カウンタ変数
	int		IsMove;							// 水平方向に移動したかどうかのフラグ( TRUE:移動していない  FALSE:移動した )
	bool	DoFixPos;						// 壁から押し出し処理(ポジションの補正)を行うかどうかのフラグ
	MV1_COLL_RESULT_POLY_DIM HitDim;		// プレイヤーの周囲にあるポリゴンを検出した結果が代入される当たり判定結果構造体
	int KabeNum;							// 壁ポリゴンと判断されたポリゴンの数
	int YukaNum;							// 床ポリゴンと判断されたポリゴンの数
	MV1_COLL_RESULT_POLY* Kabe[PLAYER_MAX_HITCOLL];	// 壁ポリゴンと判断されたポリゴンの構造体のアドレスを保存しておくためのポインタ配列
	MV1_COLL_RESULT_POLY* Yuka[PLAYER_MAX_HITCOLL];	// 床ポリゴンと判断されたポリゴンの構造体のアドレスを保存しておくためのポインタ配列
	MV1_COLL_RESULT_POLY* Poly;				// ポリゴンの構造体にアクセスするために使用するポインタ( 使わなくても済ませられますがプログラムが長くなるので・・・ )
	HITRESULT_LINE LineRes;					// 線分とポリゴンとの当たり判定の結果を代入する構造体
	VECTOR OldPos;							// 移動前の座標	
	VECTOR NowPos;							// 移動後の座標

	// 移動前の座標を保存
	OldPos = player.Position;

	// 移動後の座標を算出
	NowPos = VAdd(player.Position, MoveVector);

	// プレイヤーの周囲にあるステージポリゴンを取得する
	// ( 検出する範囲は移動距離も考慮する )
	HitDim = MV1CollCheck_Sphere(stage.ModelHandle, -1, player.Position, PLAYER_ENUM_DEFAULT_SIZE + VSize(MoveVector));

	// HACK: 移動距離が0.01未満で微妙に移動していた場合はじんわり移動してバグる
	// x軸かy軸方向に 0.01f 以上移動した場合は「移動した」フラグを１にする
	if (fabs(MoveVector.x) > 0.01f || fabs(MoveVector.z) > 0.01f)
	{
		IsMove = true;
	}
	else
	{
		IsMove = false;
	}

	// HACK: 壁はXZ平面に垂直である前提で成り立っている。それ以外を置くとバグる
	// 検出されたポリゴンが壁ポリゴン( ＸＺ平面に垂直なポリゴン )か床ポリゴン( ＸＺ平面に垂直ではないポリゴン )かを判断する
	{
		// 壁ポリゴンと床ポリゴンの数を初期化する
		KabeNum = 0;
		YukaNum = 0;

		// 検出されたポリゴンの数だけ繰り返し
		for (i = 0; i < HitDim.HitNum; i++)
		{
			// ＸＺ平面に垂直かどうかはポリゴンの法線のＹ成分が０に限りなく近いかどうかで判断する
			if (HitDim.Dim[i].Normal.y < 0.000001f && HitDim.Dim[i].Normal.y > -0.000001f)
			{
				// 壁ポリゴンと判断された場合でも、プレイヤーのＹ座標＋１．０ｆより高いポリゴンのみ当たり判定を行う
				if (HitDim.Dim[i].Position[0].y > player.Position.y + 1.0f ||
					HitDim.Dim[i].Position[1].y > player.Position.y + 1.0f ||
					HitDim.Dim[i].Position[2].y > player.Position.y + 1.0f)
				{
					// ポリゴンの数が列挙できる限界数に達していなかったらポリゴンを配列に追加
					if (KabeNum < PLAYER_MAX_HITCOLL)
					{
						// ポリゴンの構造体のアドレスを壁ポリゴンポインタ配列に保存する
						Kabe[KabeNum] = &HitDim.Dim[i];

						// 壁ポリゴンの数を加算する
						KabeNum++;
					}
				}
			}
			else
			{
				// ポリゴンの数が列挙できる限界数に達していなかったらポリゴンを配列に追加
				if (YukaNum < PLAYER_MAX_HITCOLL)
				{
					// ポリゴンの構造体のアドレスを床ポリゴンポインタ配列に保存する
					Yuka[YukaNum] = &HitDim.Dim[i];

					// 床ポリゴンの数を加算する
					YukaNum++;
				}
			}
		}
	}

	// ポジションの補正はしない状態にしておく
	DoFixPos = false;

	// 壁ポリゴンとの当たり判定処理
	if (KabeNum != 0)
	{
		// 移動したかどうかで処理を分岐
		if (IsMove)
		{
			// 壁ポリゴンの数だけ繰り返し
			for (i = 0; i < KabeNum; i++)
			{
				// i番目の壁ポリゴンのアドレスを壁ポリゴンポインタ配列から取得
				Poly = Kabe[i];

				// ポリゴンとプレイヤーが当たっていなかったら次のカウントへ
				if (HitCheck_Capsule_Triangle(NowPos, VAdd(NowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), PLAYER_HIT_WIDTH, Poly->Position[0], Poly->Position[1], Poly->Position[2]) == FALSE)
				{
					continue;
				}

				// ここにきたらポリゴンとプレイヤーが当たっているということなので、いったんポジション補正を行う状態にする
				DoFixPos = true;

				// 壁に当たったら壁に遮られない移動成分だけ移動する
				{
					VECTOR MoveNormalCross;	// 進行方向ベクトルと壁ポリゴンの法線ベクトルに垂直なベクトル
					VECTOR SlideVec;		// プレイヤーをスライドさせるベクトル

					// 進行方向ベクトルと壁ポリゴンの法線ベクトルに垂直なベクトルを算出
					MoveNormalCross = VCross(MoveVector, Poly->Normal);

					// 算出したベクトルと壁ポリゴンの法線ベクトルに垂直なベクトルを算出、これが
					// 元の移動成分から壁方向の移動成分を抜いたベクトル
					// このベクトルを使ってスライドすることで、壁にそって移動した場合の位置に近い場所が出る
					// かつ壁からははみ出ない
					SlideVec = VCross(Poly->Normal, MoveNormalCross);

					// NowPosは移動後の座標
					// 元々は移動ベクトルを現在の座標に足したもの
					// それを移動量と関係なく完全に上書きしている
					
					// それを移動前の座標に足したものを新たな座標とする
					NowPos = VAdd(OldPos, SlideVec);
				}

				// 新たな移動座標で壁ポリゴンと当たっていないかどうかを判定する
				for (j = 0; j < KabeNum; j++)
				{
					// j番目の壁ポリゴンのアドレスを壁ポリゴンポインタ配列から取得
					Poly = Kabe[j];

					// 当たっていたらループから抜ける
					if (HitCheck_Capsule_Triangle(NowPos, VAdd(NowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), PLAYER_HIT_WIDTH, Poly->Position[0], Poly->Position[1], Poly->Position[2]) == TRUE)
					{
						// NowPosを更新したうえで壁に当たったことにするので、のちの押し出し処理を行う
						break;
					}
				}

				// j が KabeNum だった場合はどのポリゴンとも当たらなかったということなので
				// 壁に当たったフラグを倒した上でループから抜ける
				// NowPosを更新したうえで壁に当たらなかったことにするので、のちの押し出し処理を行わない
				if (j == KabeNum)
				{
					DoFixPos = false;
					break;
				}
			}
		}
		else
		{
			// 移動していない場合の処理

			// 壁ポリゴンの数だけ繰り返し
			for (i = 0; i < KabeNum; i++)
			{
				// i番目の壁ポリゴンのアドレスを壁ポリゴンポインタ配列から取得
				Poly = Kabe[i];

				// ポリゴンに当たっていたらポジション補正を行う
				if (HitCheck_Capsule_Triangle(NowPos, VAdd(NowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), PLAYER_HIT_WIDTH, Poly->Position[0], Poly->Position[1], Poly->Position[2]) == TRUE)
				{
					DoFixPos = true;
					break;
				}
			}
		}

		// 壁に当たるなどして、ポジションの補正が必要な場合、補正を行う
		if (DoFixPos)
		{
			// 壁からの押し出し処理を試みる最大数だけ繰り返し
			for (k = 0; k < PLAYER_HIT_TRYNUM; k++)
			{
				// 壁ポリゴンの数だけ繰り返し
				for (i = 0; i < KabeNum; i++)
				{
					// i番目の壁ポリゴンのアドレスを壁ポリゴンポインタ配列から取得
					Poly = Kabe[i];

					// プレイヤーと当たっているかを判定
					if (HitCheck_Capsule_Triangle(NowPos, VAdd(NowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), PLAYER_HIT_WIDTH, Poly->Position[0], Poly->Position[1], Poly->Position[2]) == FALSE) continue;

					// 当たっていたら規定距離分プレイヤーを壁の法線方向に移動させる
					NowPos = VAdd(NowPos, VScale(Poly->Normal, PLAYER_HIT_SLIDE_LENGTH));

					// 移動した上で壁ポリゴンと接触しているかどうかを判定
					for (j = 0; j < KabeNum; j++)
					{
						// 当たっていたらループを抜ける
						Poly = Kabe[j];
						if (HitCheck_Capsule_Triangle(NowPos, VAdd(NowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), PLAYER_HIT_WIDTH, Poly->Position[0], Poly->Position[1], Poly->Position[2]) == TRUE) break;
					}

					// 全てのポリゴンと当たっていなかったらここでループ終了
					if (j == KabeNum) break;
				}

				// i が KabeNum ではない場合は全部のポリゴンで押し出しを試みる前に全ての壁ポリゴンと接触しなくなったということなのでループから抜ける
				if (i != KabeNum) break;
			}
		}
	}

	// 床ポリゴンとの当たり判定
	if (YukaNum != 0)
	{
		// 当たったかどうかのフラグを初期化
		bool IsHitYuka = false;

		// ジャンプ中且つ上昇中の場合は処理を分岐
		if (player.State == PlayerState::JUMP && player.JumpPower > 0.0f)
		{
			float MinY;

			// 天井に頭をぶつける処理を行う

			// 一番低い天井にぶつける為の判定用変数を初期化
			MinY = 0.0f;

			// 床ポリゴンの数だけ繰り返し
			for (i = 0; i < YukaNum; i++)
			{
				// i番目の床ポリゴンのアドレスを床ポリゴンポインタ配列から取得
				Poly = Yuka[i];

				// 足先から頭の高さまでの間でポリゴンと接触しているかどうかを判定
				LineRes = HitCheck_Line_Triangle(NowPos, VAdd(NowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), Poly->Position[0], Poly->Position[1], Poly->Position[2]);

				// 接触していなかったら何もしない
				if (LineRes.HitFlag == false) continue;

				// 既にポリゴンに当たっていて、且つ今まで検出した天井ポリゴンより高い場合は何もしない
				if (IsHitYuka == true && MinY < LineRes.Position.y) continue;

				// ポリゴンに当たったフラグを立てる
				IsHitYuka = true;

				// 接触したＹ座標を保存する
				MinY = LineRes.Position.y;
			}

			// 接触したポリゴンがあったかどうかで処理を分岐
			if (IsHitYuka == true)
			{
				// 接触した場合はプレイヤーのＹ座標を接触座標を元に更新
				NowPos.y = MinY - PLAYER_HIT_HEIGHT;

				// Ｙ軸方向の速度は反転
				player.JumpPower = -player.JumpPower;
			}
		}
		else
		{
			float MaxY;

			// 下降中かジャンプ中ではない場合の処理
			// 一番高い床ポリゴンにぶつける為の判定用変数を初期化
			MaxY = 0.0f;

			// 床ポリゴンの数だけ繰り返し
			for (i = 0; i < YukaNum; i++)
			{
				// i番目の床ポリゴンのアドレスを床ポリゴンポインタ配列から取得
				Poly = Yuka[i];

				// ジャンプ中かどうかで処理を分岐
				if (player.State == PlayerState::JUMP)
				{
					// ジャンプ中の場合は頭の先から足先より少し低い位置の間で当たっているかを判定
					LineRes = HitCheck_Line_Triangle(VAdd(NowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), VAdd(NowPos, VGet(0.0f, -1.0f, 0.0f)), Poly->Position[0], Poly->Position[1], Poly->Position[2]);
				}
				else
				{
					// 走っている場合は頭の先からそこそこ低い位置の間で当たっているかを判定( 傾斜で落下状態に移行してしまわない為 )
					LineRes = HitCheck_Line_Triangle(VAdd(NowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), VAdd(NowPos, VGet(0.0f, -40.0f, 0.0f)), Poly->Position[0], Poly->Position[1], Poly->Position[2]);
				}

				// 当たっていなかったら何もしない
				if (LineRes.HitFlag == FALSE) continue;

				// 既に当たったポリゴンがあり、且つ今まで検出した床ポリゴンより低い場合は何もしない
				if (IsHitYuka == true && MaxY > LineRes.Position.y) continue;

				// ポリゴンに当たったフラグを立てる
				IsHitYuka = true;

				// 接触したＹ座標を保存する
				MaxY = LineRes.Position.y;
			}

			// 床ポリゴンに当たったかどうかで処理を分岐
			if (IsHitYuka == true)
			{
				// 当たった場合

				// 接触したポリゴンで一番高いＹ座標をプレイヤーのＹ座標にする
				NowPos.y = MaxY;

				// Ｙ軸方向の移動速度は０に
				player.JumpPower = 0.0f;

				// もしジャンプ中だった場合は着地状態にする
				if (player.State == PlayerState::JUMP)
				{
					// 移動していたかどうかで着地後の状態と再生するアニメーションを分岐する
					if (IsMove)
					{
						// 移動している場合は走り状態に
						Player_PlayAnim(PlayerAnimKind::RUN);
						player.State = PlayerState::RUN;
					}
					else
					{
						// 移動していない場合は立ち止り状態に
						Player_PlayAnim(PlayerAnimKind::STOP);
						player.State = PlayerState::STAND;
					}

					// 着地時はアニメーションのブレンドは行わない
					player.AnimBlendRate = 1.0f;
				}
			}
			else
			{
				// 床コリジョンに当たっていなくて且つジャンプ状態ではなかった場合は
				if (player.State != PlayerState::JUMP)
				{
					// ジャンプ中にする
					player.State = PlayerState::JUMP;

					// ちょっとだけジャンプする
					player.JumpPower = PLAYER_FALL_UP_POWER;

					// アニメーションは落下中のものにする
					Player_PlayAnim(PlayerAnimKind::JUMP);
				}
			}
		}
	}

	// 新しい座標を保存する
	player.Position = NowPos;

	// プレイヤーのモデルの座標を更新する
	MV1SetPosition(player.ModelHandle, player.Position);

	// 検出したプレイヤーの周囲のポリゴン情報を開放する
	MV1CollResultPolyDimTerminate(HitDim);
}

// プレイヤーの回転制御
void Player_UpdateAngle()
{
	// プレイヤーの移動方向にモデルの方向を近づける
	float TargetAngle;			// 目標角度
	float difference;			// 目標角度と現在の角度との差

	// 目標の方向ベクトルから角度値を算出する
	TargetAngle = static_cast<float>(atan2(player.TargetMoveDirection.x, player.TargetMoveDirection.z));

	// 目標の角度と現在の角度との差を割り出す
	{
		// 最初は単純に引き算
		difference = TargetAngle - player.Angle;

		// ある方向からある方向の差が１８０度以上になることは無いので
		// 差の値が１８０度以上になっていたら修正する
		if (difference < -DX_PI_F)
		{
			difference += DX_TWO_PI_F;
		}
		else
			if (difference > DX_PI_F)
			{
				difference -= DX_TWO_PI_F;
			}
	}

	// 角度の差が０に近づける
	if (difference > 0.0f)
	{
		// 差がプラスの場合は引く
		difference -= PLAYER_ANGLE_SPEED;
		if (difference < 0.0f)
		{
			difference = 0.0f;
		}
	}
	else
	{
		// 差がマイナスの場合は足す
		difference += PLAYER_ANGLE_SPEED;
		if (difference > 0.0f)
		{
			difference = 0.0f;
		}
	}

	// モデルの角度を更新
	player.Angle = TargetAngle - difference;
	MV1SetRotationXYZ(player.ModelHandle, VGet(0.0f, player.Angle + DX_PI_F, 0.0f));
}

// プレイヤーのアニメーションを再生する
void Player_PlayAnim(PlayerAnimKind PlayAnim)
{
	// HACK: 指定した番号のアニメーションをアタッチし、直前に再生していたアニメーションの情報をprevに移行している
	// 入れ替えを行うので、１つ前のモーションがが有効だったらデタッチする
	if (player.PrevPlayAnim != -1)
	{
		MV1DetachAnim(player.ModelHandle, player.PrevPlayAnim);
		player.PrevPlayAnim = -1;
	}

	// 今まで再生中のモーションだったものの情報をPrevに移動する
	player.PrevPlayAnim = player.CurrentPlayAnim;
	player.PrevAnimCount = player.CurrentAnimCount;

	// 新たに指定のモーションをモデルにアタッチして、アタッチ番号を保存する
	player.CurrentPlayAnim = MV1AttachAnim(player.ModelHandle, static_cast<int>(PlayAnim));
	player.CurrentAnimCount = 0.0f;

	// ブレンド率はPrevが有効ではない場合は１．０ｆ( 現在モーションが１００％の状態 )にする
	player.AnimBlendRate = player.PrevPlayAnim == -1 ? 1.0f : 0.0f;
}

// プレイヤーのアニメーション処理
void Player_AnimProcess()
{
	float AnimTotalTime;		// 再生しているアニメーションの総時間

	// ブレンド率が１以下の場合は１に近づける
	if (player.AnimBlendRate < 1.0f)
	{
		player.AnimBlendRate += PLAYER_ANIM_BLEND_SPEED;
		if (player.AnimBlendRate > 1.0f)
		{
			player.AnimBlendRate = 1.0f;
		}
	}

	// 再生しているアニメーション１の処理
	if (player.CurrentPlayAnim != -1)
	{
		// アニメーションの総時間を取得
		AnimTotalTime = MV1GetAttachAnimTotalTime(player.ModelHandle, player.CurrentPlayAnim);

		// 再生時間を進める
		player.CurrentAnimCount += PLAYER_PLAY_ANIM_SPEED;

		// 再生時間が総時間に到達していたら再生時間をループさせる
		if (player.CurrentAnimCount >= AnimTotalTime)
		{
			player.CurrentAnimCount = static_cast<float>(fmod(player.CurrentAnimCount, AnimTotalTime));
		}

		// 変更した再生時間をモデルに反映させる
		MV1SetAttachAnimTime(player.ModelHandle, player.CurrentPlayAnim, player.CurrentAnimCount);

		// アニメーション１のモデルに対する反映率をセット
		MV1SetAttachAnimBlendRate(player.ModelHandle, player.CurrentPlayAnim, player.AnimBlendRate);
	}

	// 再生しているアニメーション２の処理
	if (player.PrevPlayAnim != -1)
	{
		// アニメーションの総時間を取得
		AnimTotalTime = MV1GetAttachAnimTotalTime(player.ModelHandle, player.PrevPlayAnim);

		// 再生時間を進める
		player.PrevAnimCount += PLAYER_PLAY_ANIM_SPEED;

		// 再生時間が総時間に到達していたら再生時間をループさせる
		if (player.PrevAnimCount > AnimTotalTime)
		{
			player.PrevAnimCount = static_cast<float>(fmod(player.PrevAnimCount, AnimTotalTime));
		}

		// 変更した再生時間をモデルに反映させる
		MV1SetAttachAnimTime(player.ModelHandle, player.PrevPlayAnim, player.PrevAnimCount);

		// アニメーション２のモデルに対する反映率をセット
		MV1SetAttachAnimBlendRate(player.ModelHandle, player.PrevPlayAnim, 1.0f - player.AnimBlendRate);
	}
}

// プレイヤーの影を描画
void Player_ShadowRender()
{
	MV1_COLL_RESULT_POLY_DIM HitResultDim;
	MV1_COLL_RESULT_POLY* HitResult;
	VERTEX3D Vertex[3];
	VECTOR SlideVec;

	// ライティングを無効にする
	SetUseLighting(FALSE);

	// Ｚバッファを有効にする
	SetUseZBuffer3D(TRUE);

	// テクスチャアドレスモードを CLAMP にする( テクスチャの端より先は端のドットが延々続く )
	SetTextureAddressMode(DX_TEXADDRESS_CLAMP);

	// プレイヤーの直下に存在する地面のポリゴンを取得
	HitResultDim = MV1CollCheck_Capsule(stage.ModelHandle, -1, player.Position, VAdd(player.Position, VGet(0.0f, -PLAYER_SHADOW_HEIGHT, 0.0f)), PLAYER_SHADOW_SIZE);

	// 頂点データで変化が無い部分をセット
	Vertex[0].dif = GetColorU8(255, 255, 255, 255);
	Vertex[0].spc = GetColorU8(0, 0, 0, 0);
	Vertex[0].su = 0.0f;
	Vertex[0].sv = 0.0f;
	Vertex[1] = Vertex[0];
	Vertex[2] = Vertex[0];

	// 球の直下に存在するポリゴンの数だけ繰り返し
	HitResult = HitResultDim.Dim;
	for (int i = 0; i < HitResultDim.HitNum; i++, HitResult++)
	{
		// ポリゴンの座標は地面ポリゴンの座標
		Vertex[0].pos = HitResult->Position[0];
		Vertex[1].pos = HitResult->Position[1];
		Vertex[2].pos = HitResult->Position[2];

		// ちょっと持ち上げて重ならないようにする
		SlideVec = VScale(HitResult->Normal, 0.5f);
		Vertex[0].pos = VAdd(Vertex[0].pos, SlideVec);
		Vertex[1].pos = VAdd(Vertex[1].pos, SlideVec);
		Vertex[2].pos = VAdd(Vertex[2].pos, SlideVec);

		// ポリゴンの不透明度を設定する
		Vertex[0].dif.a = 0;
		Vertex[1].dif.a = 0;
		Vertex[2].dif.a = 0;
		if (HitResult->Position[0].y > player.Position.y - PLAYER_SHADOW_HEIGHT)
			Vertex[0].dif.a = static_cast<BYTE>(128 * (1.0f - static_cast<float>(fabs(HitResult->Position[0].y - player.Position.y) / PLAYER_SHADOW_HEIGHT)));

		if (HitResult->Position[1].y > player.Position.y - PLAYER_SHADOW_HEIGHT)
			Vertex[1].dif.a = static_cast<BYTE>(128 * (1.0f - static_cast<float>(fabs(HitResult->Position[1].y - player.Position.y) / PLAYER_SHADOW_HEIGHT)));

		if (HitResult->Position[2].y > player.Position.y - PLAYER_SHADOW_HEIGHT)
			Vertex[2].dif.a = static_cast<BYTE>(128 * (1.0f - static_cast<float>(fabs(HitResult->Position[2].y - player.Position.y) / PLAYER_SHADOW_HEIGHT)));

		// ＵＶ値は地面ポリゴンとプレイヤーの相対座標から割り出す
		Vertex[0].u = (HitResult->Position[0].x - player.Position.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
		Vertex[0].v = (HitResult->Position[0].z - player.Position.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
		Vertex[1].u = (HitResult->Position[1].x - player.Position.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
		Vertex[1].v = (HitResult->Position[1].z - player.Position.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
		Vertex[2].u = (HitResult->Position[2].x - player.Position.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
		Vertex[2].v = (HitResult->Position[2].z - player.Position.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;

		// 影ポリゴンを描画
		DrawPolygon3D(Vertex, 1, player.ShadowHandle, TRUE);
	}

	// 検出した地面ポリゴン情報の後始末
	MV1CollResultPolyDimTerminate(HitResultDim);

	// ライティングを有効にする
	SetUseLighting(TRUE);

	// Ｚバッファを無効にする
	SetUseZBuffer3D(FALSE);
}

// ステージの初期化処理
void Stage_Initialize()
{
	// ステージモデルの読み込み
	stage.ModelHandle = MV1LoadModel("ColTestStage.mqo");

	// モデル全体のコリジョン情報のセットアップ
	MV1SetupCollInfo(stage.ModelHandle, -1);
}

// ステージの後始末処理
void Stage_Terminate()
{
	// ステージモデルの後始末
	MV1DeleteModel(stage.ModelHandle);
}

// カメラの初期化処理
void Camera_Initialize()
{
	// カメラの初期水平角度は１８０度
	camera.AngleH = DX_PI_F;

	// 垂直角度は０度
	camera.AngleV = 0.0f;
}

// カメラの処理
void Camera_Process()
{
	// パッドの３ボタンか、シフトキーが押されている場合のみ角度変更操作を行う
	if (CheckHitKey(KEY_INPUT_LSHIFT) || (input.NowFrameInput & PAD_INPUT_C))
	{
		// 「←」ボタンが押されていたら水平角度をマイナスする
		if (input.NowFrameInput & PAD_INPUT_LEFT)
		{
			camera.AngleH -= CAMERA_ANGLE_SPEED;

			// －１８０度以下になったら角度値が大きくなりすぎないように３６０度を足す
			if (camera.AngleH < -DX_PI_F)
			{
				camera.AngleH += DX_TWO_PI_F;
			}
		}

		// 「→」ボタンが押されていたら水平角度をプラスする
		if (input.NowFrameInput & PAD_INPUT_RIGHT)
		{
			camera.AngleH += CAMERA_ANGLE_SPEED;

			// １８０度以上になったら角度値が大きくなりすぎないように３６０度を引く
			if (camera.AngleH > DX_PI_F)
			{
				camera.AngleH -= DX_TWO_PI_F;
			}
		}

		// 「↑」ボタンが押されていたら垂直角度をマイナスする
		if (input.NowFrameInput & PAD_INPUT_UP)
		{
			camera.AngleV -= CAMERA_ANGLE_SPEED;

			// ある一定角度以下にはならないようにする
			if (camera.AngleV < -DX_PI_F / 2.0f + 0.6f)
			{
				camera.AngleV = -DX_PI_F / 2.0f + 0.6f;
			}
		}

		// 「↓」ボタンが押されていたら垂直角度をプラスする
		if (input.NowFrameInput & PAD_INPUT_DOWN)
		{
			camera.AngleV += CAMERA_ANGLE_SPEED;

			// ある一定角度以上にはならないようにする
			if (camera.AngleV > DX_PI_F / 2.0f - 0.6f)
			{
				camera.AngleV = DX_PI_F / 2.0f - 0.6f;
			}
		}
	}

	// カメラの注視点はプレイヤー座標から規定値分高い座標
	camera.Target = VAdd(player.Position, VGet(0.0f, CAMERA_PLAYER_TARGET_HEIGHT, 0.0f));

	// カメラの座標を決定する
	{
		MATRIX RotZ, RotY;
		float CameraPlayerLength;
		MV1_COLL_RESULT_POLY_DIM HitResult;
		int HitNum;

		// 水平方向の回転はＹ軸回転
		RotY = MGetRotY(camera.AngleH);

		// 垂直方向の回転はＺ軸回転 )
		RotZ = MGetRotZ(camera.AngleV);

		// カメラからプレイヤーまでの初期距離をセット
		CameraPlayerLength = CAMERA_PLAYER_LENGTH;

		// カメラの座標を算出
		// Ｘ軸にカメラとプレイヤーとの距離分だけ伸びたベクトルを
		// 垂直方向回転( Ｚ軸回転 )させたあと水平方向回転( Ｙ軸回転 )して更に
		// 注視点の座標を足したものがカメラの座標
		camera.Eye = VAdd(VTransform(VTransform(VGet(-CameraPlayerLength, 0.0f, 0.0f), RotZ), RotY), camera.Target);

		// 注視点からカメラの座標までの間にステージのポリゴンがあるか調べる
		HitResult = MV1CollCheck_Capsule(stage.ModelHandle, -1, camera.Target, camera.Eye, CAMERA_COLLISION_SIZE);
		HitNum = HitResult.HitNum;
		MV1CollResultPolyDimTerminate(HitResult);
		if (HitNum != 0)
		{
			float NotHitLength;
			float HitLength;
			float TestLength;
			VECTOR TestPosition;

			// あったら無い位置までプレイヤーに近づく

			// ポリゴンに当たらない距離をセット
			NotHitLength = 0.0f;

			// ポリゴンに当たる距離をセット
			HitLength = CameraPlayerLength;
			do
			{
				// 当たるかどうかテストする距離をセット( 当たらない距離と当たる距離の中間 )
				TestLength = NotHitLength + (HitLength - NotHitLength) / 2.0f;

				// テスト用のカメラ座標を算出
				TestPosition = VAdd(VTransform(VTransform(VGet(-TestLength, 0.0f, 0.0f), RotZ), RotY), camera.Target);

				// 新しい座標で壁に当たるかテスト
				HitResult = MV1CollCheck_Capsule(stage.ModelHandle, -1, camera.Target, TestPosition, CAMERA_COLLISION_SIZE);
				HitNum = HitResult.HitNum;
				MV1CollResultPolyDimTerminate(HitResult);
				if (HitNum != 0)
				{
					// 当たったら当たる距離を TestLength に変更する
					HitLength = TestLength;
				}
				else
				{
					// 当たらなかったら当たらない距離を TestLength に変更する
					NotHitLength = TestLength;
				}

				// HitLength と NoHitLength が十分に近づいていなかったらループ
			} while (HitLength - NotHitLength > 0.1f);

			// カメラの座標をセット
			camera.Eye = TestPosition;
		}
	}

	// カメラの情報をライブラリのカメラに反映させる
	SetCameraPositionAndTarget_UpVecY(camera.Eye, camera.Target);
}

// 描画処理
void Render_Process()
{
	// ステージモデルの描画
	MV1DrawModel(stage.ModelHandle);

	// プレイヤーモデルの描画
	MV1DrawModel(player.ModelHandle);

	// プレイヤーの影の描画
	Player_ShadowRender();
}

// WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// ウインドウモードで起動
	ChangeWindowMode(TRUE);

	// ライブラリの初期化
	if (DxLib_Init() < 0)
		return -1;

	// プレイヤーの初期化
	Player_Initialize();

	// ステージの初期化
	Stage_Initialize();

	// カメラの初期化
	Camera_Initialize();

	// 描画先を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);

	// ＥＳＣキーが押されるか、ウインドウが閉じられるまでループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// 画面をクリア
		ClearDrawScreen();

		// 入力処理
		Input_Process();

		// プレイヤーの処理
		Player_Process();

		// カメラの処理
		Camera_Process();

		// 描画処理
		Render_Process();

		// 裏画面の内容を表画面に反映
		ScreenFlip();
	}

	// プレイヤーの後始末
	Player_Terminate();

	// ステージの後始末
	Stage_Terminate();

	// ライブラリの後始末
	DxLib_End();

	// ソフト終了
	return 0;
}
