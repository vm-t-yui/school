// アクションサンプルプログラム( 坂道あり )
//
// 　２Ｄアクションの基本、坂道のあるマップとの当たり判定です。
// アクションサンプルプログラムを改造して坂道に対応させたものです。
// 坂道を実現させるために「坂道補助ブロック」や「上下判定のみブロック」が追加されています。
// 　アルゴリズムの関係で坂道を上りきった箇所には必ず「上下判定のみブロック」が必要です。
// また「上下判定のみブロック」は横から当たると正常に動作しないので、坂道を登りきった箇所に
// 崖を作る場合は「坂道ブロック」「上下判定のみブロック」「通常ブロック」という配置をする
// 必要があります。
// 　そのようにしなくても良い方法もありますが、その場合判定の処理や補助の情報が更に増えるので、
// 今回はこのような仕様にしました。
// 　斜度45°以外の坂道の実装も、処理が更に増えることになるので、今回は斜度45°の坂道のみと
// しました。

#include "DxLib.h"

#define SCREEN_WIDTH     (640)                          // 画面の横幅
#define SCREEN_HEIGHT    (480)                          // 画面の縦幅
#define CHIP_SIZE        (32)                           // 一つのチップのサイズ
#define MAP_WIDTH        (SCREEN_WIDTH / CHIP_SIZE)     // マップの横幅
#define MAP_HEIGHT       (SCREEN_HEIGHT / CHIP_SIZE)    // マップの縦幅

#define G                (0.3F)                         // キャラに掛かる重力加速度
#define JUMP_POWER       (9.0F)                         // キャラのジャンプ力
#define SPEED            (5.0F)                         // キャラの移動スピード
#define CHAR_SIZE        (30)                           // プレイヤーのサイズ

// マップデータ
// 1 : 通常ブロック
// 2 : 右方向登り地面ブロック
// 3 : 右方向下り地面ブロック
// 4 : 右方向下り天井ブロック
// 5 : 右方向登り天井ブロック
// 6 : 上下判定のみブロック
// 7 : 坂道補助ブロック
char MapData[MAP_HEIGHT][MAP_WIDTH] =
{
    0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
    1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,2,1,
    1,0,0,1,6, 3,0,0,0,0, 0,0,0,0,0, 0,0,2,7,1,
    1,0,0,4,7, 7,3,0,0,0, 0,0,0,0,0, 0,2,7,7,1,
    1,0,0,0,4, 6,1,0,0,0, 0,0,0,0,0, 1,1,1,1,1,

    1,0,0,0,0, 0,0,0,1,1, 0,0,0,0,0, 0,0,1,0,1,
    1,0,0,0,0, 0,0,0,0,0, 0,0,1,1,0, 0,0,1,0,1,
    1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,1,0,1,
    1,0,1,1,1, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
    1,0,0,0,0, 0,0,1,6,3, 0,0,0,0,2, 6,1,0,0,1,

    1,0,0,0,0, 0,0,4,7,1, 0,0,0,2,7, 7,1,0,0,1,
    1,0,0,0,0, 0,0,0,1,1, 0,0,0,1,1, 1,1,0,0,1,
    1,0,0,2,6, 3,0,0,0,0, 0,0,0,1,1, 1,1,0,0,1,
    1,0,2,7,7, 7,3,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
    1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
};

float PlX, PlY;            // プレイヤーの座標(中心座標)
float PlDownSp;            // プレイヤーの落下速度
char PlJumpFlag;           // プレイヤーがジャンプ中か、のフラグ

int Input, EdgeInput;      // 入力状態
int FrameStartTime;        // ６０ＦＰＳ固定用、時間保存用変数



// マップチップの値を取得する関数
int GetChipParam(float X, float Y);

// 坂道ブロックを考慮したマップチップの値を取得する関数
int GetChipParam2(float X, float Y);

// キャラクタをマップとの当たり判定を考慮しながら移動する関数
int CharMove(float* X, float* Y, float* DownSP,
    float MoveX, float MoveY, float Size, char* JumpFlag);

// アクションサンプルプログラムメイン関数
int ActMain(void);

// マップとの当たり判定( 通常ブロックとの判定のみ )
// ( 戻り値 0:当たらなかった  1:左辺に当たった  2:右辺に当たった
//                            3:上辺に当たった  4:下辺に当たった )
// 注意：MoveX と MoveY 、どっちか片方が０じゃないとまともに動作しません
int MapHitCheck(float X, float Y, float* MoveX, float* MoveY);

// マップとの当たり判定( 坂道ブロック、上下判定ブロックとの判定もあり )
// ( 戻り値 0:当たらなかった  1:上辺に当たった  2:下辺に当たった )
int MapHitCheck2(float X, float Y, float* MoveX, float* MoveY);



// WinMain関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    ChangeWindowMode(TRUE);

    // ＤＸライブラリの初期化
    if (DxLib_Init() == -1) return -1;

    // アクションゲームのメイン関数を呼ぶ
    ActMain();

    // ＤＸライブラリの後始末
    DxLib_End();

    // 終了
    return 0;
}

// アクションサンプルプログラムメイン
int ActMain(void)
{
    // 描画先を裏画面にセット
    SetDrawScreen(DX_SCREEN_BACK);

    // 垂直同期信号を待たない
    SetWaitVSyncFlag(FALSE);

    // プレイヤーの座標を初期化
    PlX = 135.0F;        PlY = 240.0F;

    // プレイヤーの落下速度を初期化
    PlDownSp = 0.0F;

    // ジャンプ中フラグを倒す
    PlJumpFlag = FALSE;

    // 入力状態の初期化
    Input = 0;
    EdgeInput = 0;

    // ６０ＦＰＳ固定用、時間保存用変数を現在のタイムカウントにセット
    FrameStartTime = GetNowCount();

    // メインループ開始、ＥＳＣキーで外に出る
    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        // 画面のクリア
        ClsDrawScreen();

        // １/６０秒立つまで待つ
        while (GetNowCount() - FrameStartTime < 1000 / 60) { Sleep(1); }

        // 現在のタイムカウントを保存
        FrameStartTime = GetNowCount();

        // 入力状態を更新
        {
            int i;

            // パッド１とキーボードから入力を得る
            i = GetJoypadInputState(DX_INPUT_KEY_PAD1);

            // エッジを取った入力をセット
            EdgeInput = i & ~Input;

            // 入力状態の保存
            Input = i;
        }

        // プレイヤーの移動処理
        {
            float MoveX, MoveY;

            // 移動量の初期化
            MoveX = 0.0F;
            MoveY = 0.0F;

            // 左右の移動を見る
            if ((Input & PAD_INPUT_LEFT) != 0) MoveX -= SPEED;
            if ((Input & PAD_INPUT_RIGHT) != 0) MoveX += SPEED;

            // 地に足が着いている場合のみジャンプボタン(ボタン１ or Ｚキー)を見る
            if (PlJumpFlag == FALSE && (EdgeInput & PAD_INPUT_A) != 0)
            {
                PlDownSp = -JUMP_POWER;
                PlJumpFlag = TRUE;
            }

            // 落下処理
            PlDownSp += G;

            // 落下速度を移動量に加える
            MoveY = PlDownSp;

            // 移動量に基づいてキャラクタの座標を移動
            CharMove(&PlX, &PlY, &PlDownSp, MoveX, MoveY, CHAR_SIZE, &PlJumpFlag);
        }

        // マップの描画
        {
            int i, j;

            for (i = 0; i < MAP_HEIGHT; i++)
            {
                for (j = 0; j < MAP_WIDTH; j++)
                {
                    // １～７はブロックチップを表しているので１～７のところだけ描画
                    switch (MapData[i][j])
                    {
                    case 1:
                    case 6:
                    case 7:
                        DrawBox(
                            j * CHIP_SIZE, i * CHIP_SIZE,
                            j * CHIP_SIZE + CHIP_SIZE, i * CHIP_SIZE + CHIP_SIZE,
                            GetColor(255, 255, 255), TRUE);
                        break;

                    case 2:
                        DrawTriangle(
                            j * CHIP_SIZE, i * CHIP_SIZE + CHIP_SIZE,
                            j * CHIP_SIZE + CHIP_SIZE, i * CHIP_SIZE,
                            j * CHIP_SIZE + CHIP_SIZE, i * CHIP_SIZE + CHIP_SIZE,
                            GetColor(255, 255, 255), TRUE);
                        break;

                    case 3:
                        DrawTriangle(
                            j * CHIP_SIZE, i * CHIP_SIZE,
                            j * CHIP_SIZE + CHIP_SIZE, i * CHIP_SIZE + CHIP_SIZE,
                            j * CHIP_SIZE, i * CHIP_SIZE + CHIP_SIZE,
                            GetColor(255, 255, 255), TRUE);
                        break;

                    case 4:
                        DrawTriangle(
                            j * CHIP_SIZE, i * CHIP_SIZE,
                            j * CHIP_SIZE + CHIP_SIZE, i * CHIP_SIZE,
                            j * CHIP_SIZE + CHIP_SIZE, i * CHIP_SIZE + CHIP_SIZE,
                            GetColor(255, 255, 255), TRUE);
                        break;

                    case 5:
                        DrawTriangle(
                            j * CHIP_SIZE, i * CHIP_SIZE + CHIP_SIZE,
                            j * CHIP_SIZE, i * CHIP_SIZE,
                            j * CHIP_SIZE + CHIP_SIZE, i * CHIP_SIZE,
                            GetColor(255, 255, 255), TRUE);
                        break;
                    }
                }
            }
        }

        // キャラクタの描画
        DrawBox((int)(PlX - CHAR_SIZE * 0.5F), (int)(PlY - CHAR_SIZE * 0.5F),
            (int)(PlX + CHAR_SIZE * 0.5F) + 1, (int)(PlY + CHAR_SIZE * 0.5F) + 1,
            GetColor(255, 0, 0), TRUE);

        // 画面の更新
        ScreenFlip();
    }

    // 終了
    return 0;
}

// キャラクタをマップとの当たり判定を考慮しながら移動する
int CharMove(float* X, float* Y, float* DownSP,
    float MoveX, float MoveY, float Size, char* JumpFlag)
{
    float Dummy = 0.0F;
    float hsize;
    float MoveXBackup = MoveX;
    float MoveYBackup = MoveY;

    // キャラクタの左上、右上、左下、右下部分が当たり判定のある
    // マップに衝突しているか調べ、衝突していたら補正する

    // 半分のサイズを算出
    hsize = Size * 0.5F;

    // まず上下移動成分だけでチェック
    {
        // 左下のチェック、もしブロックの上辺に着いていたら落下を止める
        if (MapHitCheck(*X - hsize, *Y + hsize, &Dummy, &MoveY) == 3)
            *DownSP = 0.0F;

        // 右下のチェック、もしブロックの上辺に着いていたら落下を止める
        if (MapHitCheck(*X + hsize, *Y + hsize, &Dummy, &MoveY) == 3)
            *DownSP = 0.0F;

        // 左上のチェック、もしブロックの下辺に当たっていたら落下させる
        if (MapHitCheck(*X - hsize, *Y - hsize, &Dummy, &MoveY) == 4 && *DownSP < 0.0f)
            *DownSP *= -1.0F;

        // 右上のチェック、もしブロックの下辺に当たっていたら落下させる
        if (MapHitCheck(*X + hsize, *Y - hsize, &Dummy, &MoveY) == 4 && *DownSP < 0.0f)
            *DownSP *= -1.0F;
    }

    // 次に左右移動成分だけでチェック
    {
        // 左下のチェック
        MapHitCheck(*X - hsize, *Y + hsize, &MoveX, &Dummy);

        // 右下のチェック
        MapHitCheck(*X + hsize, *Y + hsize, &MoveX, &Dummy);

        // 左上のチェック
        MapHitCheck(*X - hsize, *Y - hsize, &MoveX, &Dummy);

        // 右上のチェック
        MapHitCheck(*X + hsize, *Y - hsize, &MoveX, &Dummy);

        // 左右移動成分を加算
        *X += MoveX;
        MoveX = 0.0f;
    }

    // 最後に坂道ブロック、上下判定ブロックを含めてチェック
    {
        // 上辺のチェック、もしブロックの上辺に着いていたら落下を止める
        if (MapHitCheck2(*X, *Y + hsize, &MoveX, &MoveY) == 1)
            *DownSP = 0.0F;

        // 下辺のチェック、もしブロックの下辺に着いていたら落下させる
        if (MapHitCheck2(*X, *Y - hsize, &MoveX, &MoveY) == 2 && *DownSP < 0.0f)
            *DownSP *= -1.0f;

        // 上下移動成分を加算
        *Y += MoveY;
        MoveY = 0.0f;
    }

    // 接地状態保存
    {
        int ChipL1, ChipC1, ChipR1;
        int ChipL2, ChipC2, ChipR2;

        // 足元にブロックがあるか判定
        ChipL1 = GetChipParam2(*X - Size * 0.5F, *Y + Size * 0.5F + 2.0F);
        ChipC1 = GetChipParam2(*X, *Y + Size * 0.5F + 2.0F);
        ChipR1 = GetChipParam2(*X + Size * 0.5F, *Y + Size * 0.5F + 2.0F);
        if (ChipL1 == 1 || ChipC1 != 0 || ChipR1 == 1)
        {
            // ブロックがあったら接地中にする
            *JumpFlag = FALSE;
        }
        else
        {
            // ブロックが無い場合

            // 前回ジャンプ中であったり、落下方向への速度が無い場合は無条件でジャンプ中にする
            if (*JumpFlag == TRUE || MoveYBackup <= 0.0f)
            {
                // ジャンプ中にする
                *JumpFlag = TRUE;
            }
            else
            {
                // 前回接地していて、今回接地していなかった場合、下り坂に入ったかを判定する

                // 足元周辺にブロックがあるか調べる
                ChipL1 = GetChipParam(*X - Size * 0.5f, *Y + Size * 0.5F);
                ChipC1 = GetChipParam(*X, *Y + Size * 0.5F);
                ChipR1 = GetChipParam(*X + Size * 0.5f, *Y + Size * 0.5F);
                ChipL2 = GetChipParam(*X - Size * 0.5f, *Y + Size * 0.5F + CHIP_SIZE / 2.0f);
                ChipC2 = GetChipParam(*X, *Y + Size * 0.5F + CHIP_SIZE / 2.0f);
                ChipR2 = GetChipParam(*X + Size * 0.5f, *Y + Size * 0.5F + CHIP_SIZE / 2.0f);
                if (ChipL1 == 1 || ChipL1 == 2 || ChipL1 == 3 || ChipL1 == 6 || ChipL1 == 7 ||
                    ChipL2 == 1 || ChipL2 == 2 || ChipL2 == 3 || ChipL2 == 6 || ChipL2 == 7 ||
                    ChipC1 == 1 || ChipC1 == 2 || ChipC1 == 3 || ChipC1 == 6 || ChipC1 == 7 ||
                    ChipC2 == 1 || ChipC2 == 2 || ChipC2 == 3 || ChipC2 == 6 || ChipC2 == 7 ||
                    ChipR1 == 1 || ChipR1 == 2 || ChipR1 == 3 || ChipR1 == 6 || ChipR1 == 7 ||
                    ChipR2 == 1 || ChipR2 == 2 || ChipR2 == 3 || ChipR2 == 6 || ChipR2 == 7)
                {
                    int addcy = 0;

                    // 足元周辺にブロックがあった場合は、足元のブロックに無理やり接地させる
                    if (ChipC1 != 0 || ChipC2 != 0)
                    {
                        if (ChipC1 == 0) addcy = 1;
                    }
                    else
                        if (ChipL1 != 0 || ChipL2 != 0)
                        {
                            if (ChipL1 == 0) addcy = 1;
                        }
                        else
                        {
                            if (ChipR1 == 0) addcy = 1;
                        }

                    // 接地させるために無理やり足元のブロックまで移動させる
                    MoveY = (((int)(*Y + hsize) / CHIP_SIZE + 1 + addcy) * CHIP_SIZE - 1.0f) - (*Y + hsize);
                    MapHitCheck2(*X, *Y + hsize, &MoveX, &MoveY);
                    *Y += MoveY;
                    MoveY = 0.0f;

                    // 落下速度0で接地中にする
                    *DownSP = 0.0F;
                    *JumpFlag = FALSE;
                }
                else
                {
                    // 足場が無かったらジャンプ中にする
                    *JumpFlag = TRUE;
                }
            }
        }
    }

    // 終了
    return 0;
}

// マップとの当たり判定( 通常ブロックとの判定のみ )
// ( 戻り値 0:当たらなかった  1:左辺に当たった  2:右辺に当たった
//                            3:上辺に当たった  4:下辺に当たった )
// 注意：MoveX と MoveY 、どっちか片方が０じゃないとまともに動作しません
int MapHitCheck(float X, float Y,
    float* MoveX, float* MoveY)
{
    float afX, afY;

    // 移動量を足す
    afX = X + *MoveX;
    afY = Y + *MoveY;

    // 当たり判定のあるブロックに当たっているかチェック
    if (GetChipParam(afX, afY) == 1)
    {
        float blx, bty, brx, bby;

        // 当たっていたら壁から離す処理を行う

        // ブロックの上下左右の座標を算出
        blx = (float)((int)afX / CHIP_SIZE) * CHIP_SIZE;        // 左辺の X 座標
        brx = (float)((int)afX / CHIP_SIZE + 1) * CHIP_SIZE;    // 右辺の X 座標

        bty = (float)((int)afY / CHIP_SIZE) * CHIP_SIZE;        // 上辺の Y 座標
        bby = (float)((int)afY / CHIP_SIZE + 1) * CHIP_SIZE;    // 下辺の Y 座標

        // 上辺に当たっていた場合
        if (*MoveY > 0.0F)
        {
            // 移動量を補正する
            *MoveY = bty - Y - 1.0F;

            // 上辺に当たったと返す
            return 3;
        }

        // 下辺に当たっていた場合
        if (*MoveY < 0.0F)
        {
            // 移動量を補正する
            *MoveY = bby - Y + 1.0F;

            // 下辺に当たったと返す
            return 4;
        }

        // 左辺に当たっていた場合
        if (*MoveX > 0.0F)
        {
            // 移動量を補正する
            *MoveX = blx - X - 1.0F;

            // 左辺に当たったと返す
            return 1;
        }

        // 右辺に当たっていた場合
        if (*MoveX < 0.0F)
        {
            // 移動量を補正する
            *MoveX = brx - X + 1.0F;

            // 右辺に当たったと返す
            return 2;
        }

        // ここに来たら適当な値を返す
        return 4;
    }

    // どこにも当たらなかったと返す
    return 0;
}

// マップとの当たり判定( 坂道ブロック、上下判定ブロックとの判定もあり )
// ( 戻り値 0:当たらなかった  1:上辺に当たった  2:下辺に当たった )
int MapHitCheck2(float X, float Y, float* MoveX, float* MoveY)
{
    float afX, afY;
    int Chip;

    // 移動量を足す
    afX = X + *MoveX;
    afY = Y + *MoveY;

    // ブロックに当たっているかチェック
    Chip = GetChipParam(afX, afY);
    if (Chip != 0)
    {
        float blx, bty, brx, bby;
        float blh;
        float plx, ply;

        // ブロックの上下左右の座標を算出
        blx = (float)((int)afX / CHIP_SIZE) * CHIP_SIZE;        // 左辺の X 座標
        brx = (float)((int)afX / CHIP_SIZE + 1) * CHIP_SIZE;    // 右辺の X 座標

        bty = (float)((int)afY / CHIP_SIZE) * CHIP_SIZE;        // 上辺の Y 座標
        bby = (float)((int)afY / CHIP_SIZE + 1) * CHIP_SIZE;    // 下辺の Y 座標

        // プレイヤーの位置がブロックのどの位置にあるか算出
        plx = (afX - blx) / CHIP_SIZE;
        ply = (afY - bty) / CHIP_SIZE;

        // 坂道補助ブロックだった場合は上下の坂道ブロックを判定対象にする
        if (Chip == 7)
        {
            float addy;

            // 上下の近いブロックを対象にする
            if (ply < 0.5f)
            {
                addy = -CHIP_SIZE;
            }
            else
            {
                addy = CHIP_SIZE;
            }

            // ブロック取り直し
            Chip = GetChipParam(afX, afY + addy);

            // ブロックの上下左右の座標を再算出
            bty = (float)((int)(afY + addy) / CHIP_SIZE) * CHIP_SIZE;        // 上辺の Y 座標
            bby = (float)((int)(afY + addy) / CHIP_SIZE + 1) * CHIP_SIZE;    // 下辺の Y 座標

            // プレイヤーの位置がブロックのどの位置にあるか算出
            ply = (afY - bty) / CHIP_SIZE;
        }

        // ブロックのタイプによって処理を分岐
        switch (Chip)
        {
        case 2:    // 右方向登りブロック
            // プレイヤーの位置でのブロックの高さを算出
            blh = bby - plx * CHIP_SIZE;

            // ブロックにあたっていた場合
            if (afY > blh)
            {
                // 移動量を補正する
                *MoveY = blh - Y - 1.0f;

                // 下辺に当たったと返す
                return 1;
            }
            break;

        case 3:    // 右方向下りブロック
            // プレイヤーの位置でのブロックの高さを算出
            blh = bty + plx * CHIP_SIZE;

            // ブロックにあたっていた場合
            if (afY > blh)
            {
                // 移動量を補正する
                *MoveY = blh - Y - 1.0f;

                // 下辺に当たったと返す
                return 1;
            }
            break;

        case 4:    // 右方向下り天井ブロック
            // プレイヤーの位置でのブロックの高さを算出
            blh = bty + plx * CHIP_SIZE;

            // ブロックにあたっていた場合
            if (afY < blh)
            {
                // 移動量を補正する
                *MoveY = blh - Y + 1.0f;

                // 上辺に当たったと返す
                return 2;
            }
            break;

        case 5:    // 右方向登り天井ブロック
            // プレイヤーの位置でのブロックの高さを算出
            blh = bby - plx * CHIP_SIZE;

            // ブロックにあたっていた場合
            if (afY < blh)
            {
                // 移動量を補正する
                *MoveY = blh - Y + 1.0f;

                // 上辺に当たったと返す
                return 2;
            }
            break;

        case 1:    // 通常ブロック
        case 6:    // 上下判定のみブロック
            // 上辺に当たっていた場合
            if (*MoveY > 0.0F)
            {
                // 移動量を補正する
                *MoveY = bty - Y - 1.0F;

                // 下辺に当たったと返す
                return 1;
            }

            // 下辺に当たっていた場合
            if (*MoveY < 0.0F)
            {
                // 移動量を補正する
                *MoveY = bby - Y + 1.0F;

                // 上辺に当たったと返す
                return 2;
            }

            // ここに来たら適当な値を返す
            return 1;
        }
    }

    // どこにも当たらなかったと返す
    return 0;
}

// マップチップの値を取得する関数
int GetChipParam(float X, float Y)
{
    int x, y;

    // 整数値へ変換
    x = (int)X / CHIP_SIZE;
    y = (int)Y / CHIP_SIZE;

    // マップからはみ出ていたら 0 を返す
    if (x >= MAP_WIDTH || y >= MAP_HEIGHT || x < 0 || y < 0) return 0;

    // 指定の座標に該当するマップの情報を返す
    return MapData[y][x];
}

// 坂道ブロックを考慮したマップチップの値を取得する関数
int GetChipParam2(float X, float Y)
{
    int Chip;

    Chip = GetChipParam(X, Y);

    // 坂道ブロックではなかったらそのまま値を返す
    if (Chip < 2 || Chip > 7)
    {
        return Chip;
    }

    // 坂道ブロックの場合は坂道を考慮して当たっているか判定
    {
        float blx, bty, brx, bby;
        float blh;
        float xr, yr;

        // ブロックの上下左右の座標を算出
        blx = (float)((int)X / CHIP_SIZE) * CHIP_SIZE;        // 左辺の X 座標
        brx = (float)((int)X / CHIP_SIZE + 1) * CHIP_SIZE;    // 右辺の X 座標

        bty = (float)((int)Y / CHIP_SIZE) * CHIP_SIZE;        // 上辺の Y 座標
        bby = (float)((int)Y / CHIP_SIZE + 1) * CHIP_SIZE;    // 下辺の Y 座標

        // プレイヤーの位置がブロックのどの位置にあるか算出
        xr = (X - blx) / CHIP_SIZE;
        yr = (Y - bty) / CHIP_SIZE;

        // 坂道補助ブロックだった場合は上下の坂道ブロックを判定対象にする
        if (Chip == 7)
        {
            float addy;

            // 上下の近いブロックを対象にする
            if (yr < 0.5f)
            {
                addy = -CHIP_SIZE;
            }
            else
            {
                addy = CHIP_SIZE;
            }

            // ブロック取り直し
            Chip = GetChipParam(X, Y + addy);

            // ブロックの上下左右の座標を再算出
            bty = (float)((int)(Y + addy) / CHIP_SIZE) * CHIP_SIZE;        // 上辺の Y 座標
            bby = (float)((int)(Y + addy) / CHIP_SIZE + 1) * CHIP_SIZE;    // 下辺の Y 座標

            // プレイヤーの位置がブロックのどの位置にあるか算出
            yr = (yr - bty) / CHIP_SIZE;
        }

        // ブロックのタイプによって処理を分岐
        switch (Chip)
        {
        case 2:    // 右方向登りブロック
            // プレイヤーの位置でのブロックの高さを算出
            blh = bby - xr * CHIP_SIZE;

            // ブロックに当たっているか判定
            if (Y > blh)
            {
                return Chip;
            }
            break;

        case 3:    // 右方向下りブロック
            // プレイヤーの位置でのブロックの高さを算出
            blh = bty + xr * CHIP_SIZE;

            // ブロックに当たっているか判定
            if (Y > blh)
            {
                return Chip;
            }
            break;

        case 4:    // 右方向下り天井ブロック
            // プレイヤーの位置でのブロックの高さを算出
            blh = bty + xr * CHIP_SIZE;

            // ブロックに当たっているか判定
            if (Y < blh)
            {
                return Chip;
            }
            break;

        case 5:    // 右方向登り天井ブロック
            // プレイヤーの位置でのブロックの高さを算出
            blh = bby - xr * CHIP_SIZE;

            // ブロックに当たっているか判定
            if (Y < blh)
            {
                return Chip;
            }
            break;

        case 6:    // 上下判定のみブロック
            return Chip;
        }
    }

    // 当たっていなかったらブロックなしの値を返す
    return 0;
}
