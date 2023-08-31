//-----------------------------------------------------------------------------
// @brief  アクションサンプルプログラム.
// 2019 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
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
#define PLAYER_SIZE      (30)                           // プレイヤーのサイズ

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

class Player
{
public:
    Player() {}
    ~Player(){}

    float x, y;             // プレイヤーの座標(中心座標)
    float fallSpeed;        // プレイヤーの落下速度
    bool isJump;            // プレイヤーがジャンプ中か、のフラグ
};

Player player;

int prevInput;              // 前フレームの入力状態
int nowInput;               // 今の入力状態

// 矩形でどこの辺に当たったか
enum HitPosition
{
    NONE,
    TOP,
    BOTTOM,
    LEFT,
    RIGHT
};


// プロトタイプ宣言
// 指定の座標に該当するマップのチップ番号を返す関数
int GetMapChipNoWithoutSlope(float playerX, float playerY);

// 坂道ブロックを考慮したマップのチップ番号を返す関数
int GetMapChipNoWithSlope(float playerX, float playerY);

// プレイヤーをマップとの当たり判定を考慮しながら移動する関数
int MovePlayer(float* playerX, float* playerY, float* fallSpeed,
    float MoveX, float MoveY, float playerSize, bool* JumpFlag);

// マップとの当たり判定(（プレイヤーがマップチップのどの辺に当たったか：通常ブロックとの判定のみ )
// 注意：MoveX と MoveY 、どっちか片方が０じゃないとまともに動作しません
HitPosition CheckHitMapWithoutSlope(float playerX, float playerY, float* MoveX, float* MoveY);

// マップとの当たり判定( 坂道ブロック、上下判定ブロックとの判定もあり )
// ( 戻り値：どの辺にあたったか)
HitPosition CheckHitMapWithSlope(float playerX, float playerY, float* MoveX, float* MoveY);


// WinMain関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    ChangeWindowMode(TRUE);

    // ＤＸライブラリの初期化
    if (DxLib_Init() == -1) return -1;

    // 描画先を裏画面にセット
    SetDrawScreen(DX_SCREEN_BACK);

    // 垂直同期信号を待たない
    SetWaitVSyncFlag(FALSE);

    // プレイヤーの座標を初期化
    player.x = 320.0F;        player.y = 240.0F;

    // プレイヤーの落下速度を初期化
    player.fallSpeed = 0.0F;

    // ジャンプ中フラグを倒す
    player.isJump = FALSE;

    // 入力状態の初期化
    prevInput = 0;
    nowInput = 0;

    // ６０ＦＰＳ固定用、フレームの開始時の時間
    int startFrameTime = GetNowCount();

    // メインループ開始、ＥＳＣキーで外に出る
    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        // 画面のクリア
        ClsDrawScreen();

        // １/６０秒立つまで待つ
        while (GetNowCount() - startFrameTime < 1000 / 60) { Sleep(1); }

        // そのフレームの開始時のタイムカウントを保存
        startFrameTime = GetNowCount();

        // 入力状態を更新
        {
            int i;

            // パッド１とキーボードから入力を得る
            i = GetJoypadInputState(DX_INPUT_KEY_PAD1);

            // エッジを取った入力をセット
            nowInput = i & ~prevInput;

            // 入力状態の保存
            prevInput = i;
        }

        // プレイヤーの移動処理
        {
            float MoveX, MoveY;

            // 移動量の初期化
            MoveX = 0.0F;
            MoveY = 0.0F;

            // 左右の移動を見る
            if ((prevInput & PAD_INPUT_LEFT) != 0) MoveX -= SPEED;
            if ((prevInput & PAD_INPUT_RIGHT) != 0) MoveX += SPEED;

            // 地に足が着いている場合のみジャンプボタン(ボタン１ or Ｚキー)を見る
            if (player.isJump == FALSE && (nowInput & PAD_INPUT_A) != 0)
            {
                player.fallSpeed = -JUMP_POWER;
                player.isJump = TRUE;
            }

            // 落下処理
            player.fallSpeed += G;

            // 落下速度を移動量に加える
            MoveY = player.fallSpeed;

            // 移動量に基づいてプレイヤーの座標を移動
            MovePlayer(&player.x, &player.y, &player.fallSpeed, MoveX, MoveY, PLAYER_SIZE, &player.isJump);
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

        // プレイヤーの描画
        DrawBox((int)(player.x - PLAYER_SIZE * 0.5F), (int)(player.y - PLAYER_SIZE * 0.5F),
            (int)(player.x + PLAYER_SIZE * 0.5F) + 1, (int)(player.y + PLAYER_SIZE * 0.5F) + 1,
            GetColor(255, 0, 0), TRUE);

        // 画面の更新
        ScreenFlip();
    }

    // ＤＸライブラリの後始末
    DxLib_End();

    // 終了
    return 0;
}


// プレイヤーをマップとの当たり判定を考慮しながら移動する
int MovePlayer(float* playerX, float* playerY, float* fallSpeed,
    float MoveX, float MoveY, float playerSize, bool* JumpFlag)
{
    float Dummy = 0.0F;         // ?
    float MoveXBackup = MoveX;
    float MoveYBackup = MoveY;

    // プレイヤーの左上、右上、左下、右下部分が当たり判定のある
    // マップに衝突しているか調べ、衝突していたら補正する

    // プレイヤー画像の半分のサイズを算出
    float playerSizeHalf = playerSize * 0.5F;

    // まず上下移動成分だけでチェックして、落下速度をコントロール
    {
        // プレイヤー矩形の左下座標がマップチップと当たっているかチェック、もしマップチップの上辺に当たっていたら落下を止める
        if (CheckHitMapWithoutSlope(*playerX - playerSizeHalf, *playerY + playerSizeHalf, &Dummy, &MoveY) == HitPosition::TOP) *fallSpeed = 0.0F;

        // プレイヤー矩形の右下座標がマップチップと当たっているかチェック、もしマップチップの上辺に当たっていたら落下を止める
        if (CheckHitMapWithoutSlope(*playerX + playerSizeHalf, *playerY + playerSizeHalf, &Dummy, &MoveY) == HitPosition::TOP) *fallSpeed = 0.0F;

        // 上辺に当たる場合、ジャンプ中のときはジャンプ速度をそのまま反転する
        // プレイヤー矩形の左上座標がマップチップと当たっているかチェック、もしマップチップの下辺に当たっていたら落下を止める
        if (CheckHitMapWithoutSlope(*playerX - playerSizeHalf, *playerY - playerSizeHalf, &Dummy, &MoveY) == HitPosition::BOTTOM && *fallSpeed < 0.0f) *fallSpeed *= -1.0F;

        // プレイヤー矩形の右上座標がマップチップと当たっているかチェック、もしマップチップの下辺に当たっていたら落下を止める
        if (CheckHitMapWithoutSlope(*playerX + playerSizeHalf, *playerY - playerSizeHalf, &Dummy, &MoveY) == HitPosition::BOTTOM && *fallSpeed < 0.0f) *fallSpeed *= -1.0F;
    }

    // 次に左右移動成分だけでチェック
    {
        // 左下のチェック
        // プレイヤー矩形の左下座標がマップチップと当たっているかチェック
        CheckHitMapWithoutSlope(*playerX - playerSizeHalf, *playerY + playerSizeHalf, &MoveX, &Dummy);

        // プレイヤー矩形の右下座標がマップチップと当たっているかチェック
        CheckHitMapWithoutSlope(*playerX + playerSizeHalf, *playerY + playerSizeHalf, &MoveX, &Dummy);

        // プレイヤー矩形の左上座標がマップチップと当たっているかチェック
        CheckHitMapWithoutSlope(*playerX - playerSizeHalf, *playerY - playerSizeHalf, &MoveX, &Dummy);

        // プレイヤー矩形の右上座標がマップチップと当たっているかチェック
        CheckHitMapWithoutSlope(*playerX + playerSizeHalf, *playerY - playerSizeHalf, &MoveX, &Dummy);

        // 左右移動成分を加算
        *playerX += MoveX;
        MoveX = 0.0f;
    }

    // 最後に坂道ブロック、上下判定ブロックを含めてチェック
    {
        // 上辺のチェック、もしブロックの上辺に着いていたら落下を止める
        if (CheckHitMapWithSlope(*playerX, *playerY + playerSizeHalf, &MoveX, &MoveY) == HitPosition::TOP) *fallSpeed = 0.0F;

        // 下辺のチェック、もしブロックの下辺に着いていたら落下させる
        if (CheckHitMapWithSlope(*playerX, *playerY - playerSizeHalf, &MoveX, &MoveY) == HitPosition::BOTTOM && *fallSpeed < 0.0f) *fallSpeed *= -1.0f;

        // 上下移動成分を加算
        *playerY += MoveY;
        MoveY = 0.0f;
    }

    // 接地状態保存
    {
        int ChipL1, ChipC1, ChipR1;
        int ChipL2, ChipC2, ChipR2;

        // 足元にブロックがあるか判定
        ChipL1 = GetMapChipNoWithSlope(*playerX - playerSize * 0.5F, *playerY + playerSize * 0.5F + 2.0F);
        ChipC1 = GetMapChipNoWithSlope(*playerX, *playerY + playerSize * 0.5F + 2.0F);
        ChipR1 = GetMapChipNoWithSlope(*playerX + playerSize * 0.5F, *playerY + playerSize * 0.5F + 2.0F);
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
                ChipL1 = GetMapChipNoWithoutSlope(*playerX - playerSize * 0.5f, *playerY + playerSize * 0.5F);
                ChipC1 = GetMapChipNoWithoutSlope(*playerX, *playerY + playerSize * 0.5F);
                ChipR1 = GetMapChipNoWithoutSlope(*playerX + playerSize * 0.5f, *playerY + playerSize * 0.5F);
                ChipL2 = GetMapChipNoWithoutSlope(*playerX - playerSize * 0.5f, *playerY + playerSize * 0.5F + CHIP_SIZE / 2.0f);
                ChipC2 = GetMapChipNoWithoutSlope(*playerX, *playerY + playerSize * 0.5F + CHIP_SIZE / 2.0f);
                ChipR2 = GetMapChipNoWithoutSlope(*playerX + playerSize * 0.5f, *playerY + playerSize * 0.5F + CHIP_SIZE / 2.0f);
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
                    MoveY = (((int)(*playerY + playerSizeHalf) / CHIP_SIZE + 1 + addcy) * CHIP_SIZE - 1.0f) - (*playerY + playerSizeHalf);
                    CheckHitMapWithSlope(*playerX, *playerY + playerSizeHalf, &MoveX, &MoveY);
                    *playerY += MoveY;
                    MoveY = 0.0f;

                    // 落下速度0で接地中にする
                    *fallSpeed = 0.0F;
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
// 注意：MoveX と MoveY 、どっちか片方が０じゃないとまともに動作しません
HitPosition CheckHitMapWithoutSlope(float playerX, float playerY, float* MoveX, float* MoveY)
{
    // 移動後のプレイヤーの座標
    float afterPlayerX, afterPlayerY; 

    // 移動量を足す
    afterPlayerX = playerX + *MoveX;
    afterPlayerY = playerY + *MoveY;

    // 当たり判定のあるブロックに、移動後に当たるかチェック
    if (GetMapChipNoWithoutSlope(afterPlayerX, afterPlayerY) == 1)
    {
        float blx, bty, brx, bby;

        // 当たっていたら壁から離す処理を行う

        // ブロックの上下左右の座標を算出
        blx = (float)((int)afterPlayerX / CHIP_SIZE) * CHIP_SIZE;        // 左辺のx座標
        brx = (float)((int)afterPlayerX / CHIP_SIZE + 1) * CHIP_SIZE;    // 右辺のx座標

        bty = (float)((int)afterPlayerY / CHIP_SIZE) * CHIP_SIZE;        // 上辺のy座標
        bby = (float)((int)afterPlayerY / CHIP_SIZE + 1) * CHIP_SIZE;    // 下辺のy座標

        // 上辺に当たっていた場合
        if (*MoveY > 0.0F)
        {
            // 移動量を補正する
            *MoveY = bty - playerY - 1.0F;

            // 上辺に当たったと返す
            return HitPosition::TOP;
        }

        // 下辺に当たっていた場合
        if (*MoveY < 0.0F)
        {
            // 移動量を補正する
            *MoveY = bby - playerY + 1.0F;

            // 下辺に当たったと返す
            return HitPosition::BOTTOM;
        }

        // 左辺に当たっていた場合
        if (*MoveX > 0.0F)
        {
            // 移動量を補正する
            *MoveX = blx - playerX - 1.0F;

            // 左辺に当たったと返す
            return HitPosition::LEFT;
        }

        // 右辺に当たっていた場合
        if (*MoveX < 0.0F)
        {
            // 移動量を補正する
            *MoveX = brx - playerX + 1.0F;

            // 右辺に当たったと返す
            return HitPosition::RIGHT;
        }

        // ここに来たら下辺に当たったと見なす
        return HitPosition::BOTTOM;
    }

    // どこにも当たらなかったと返す
    return HitPosition::NONE;
}

// マップとの当たり判定( 坂道ブロック、上下判定ブロックとの判定もあり )
// ( 戻り値 0:当たらなかった  1:上辺に当たった  2:下辺に当たった )
HitPosition CheckHitMapWithSlope(float playerX, float playerY, float* MoveX, float* MoveY)
{
    float afX, afY;
    int Chip;

    // 移動量を足す
    afX = playerX + *MoveX;
    afY = playerY + *MoveY;

    // ブロックに当たっているかチェック
    Chip = GetMapChipNoWithoutSlope(afX, afY);
    if (Chip != 0)
    {
        float blx, bty, brx, bby;
        float blh;
        float plx, ply;

        // ブロックの上下左右の座標を算出
        blx = (float)((int)afX / CHIP_SIZE) * CHIP_SIZE;        // 左辺の playerX 座標
        brx = (float)((int)afX / CHIP_SIZE + 1) * CHIP_SIZE;    // 右辺の playerX 座標

        bty = (float)((int)afY / CHIP_SIZE) * CHIP_SIZE;        // 上辺の playerY 座標
        bby = (float)((int)afY / CHIP_SIZE + 1) * CHIP_SIZE;    // 下辺の playerY 座標

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
            Chip = GetMapChipNoWithoutSlope(afX, afY + addy);

            // ブロックの上下左右の座標を再算出
            bty = (float)((int)(afY + addy) / CHIP_SIZE) * CHIP_SIZE;        // 上辺の playerY 座標
            bby = (float)((int)(afY + addy) / CHIP_SIZE + 1) * CHIP_SIZE;    // 下辺の playerY 座標

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
                *MoveY = blh - playerY - 1.0f;

                return HitPosition::BOTTOM;
            }
            break;

        case 3:    // 右方向下りブロック
            // プレイヤーの位置でのブロックの高さを算出
            blh = bty + plx * CHIP_SIZE;

            // ブロックにあたっていた場合
            if (afY > blh)
            {
                // 移動量を補正する
                *MoveY = blh - playerY - 1.0f;

                return HitPosition::BOTTOM;
            }
            break;

        case 4:    // 右方向下り天井ブロック
            // プレイヤーの位置でのブロックの高さを算出
            blh = bty + plx * CHIP_SIZE;

            // ブロックにあたっていた場合
            if (afY < blh)
            {
                // 移動量を補正する
                *MoveY = blh - playerY + 1.0f;

                // 上辺に当たったと返す
                return HitPosition::TOP;
            }
            break;

        case 5:    // 右方向登り天井ブロック
            // プレイヤーの位置でのブロックの高さを算出
            blh = bby - plx * CHIP_SIZE;

            // ブロックにあたっていた場合
            if (afY < blh)
            {
                // 移動量を補正する
                *MoveY = blh - playerY + 1.0f;

                // 上辺に当たったと返す
                return HitPosition::TOP;
            }
            break;

        case 1:    // 通常ブロック
        case 6:    // 上下判定のみブロック
            // 上辺に当たっていた場合
            if (*MoveY > 0.0F)
            {
                // 移動量を補正する
                *MoveY = bty - playerY - 1.0F;

                // 下辺に当たったと返す
                return HitPosition::BOTTOM;
            }

            // 下辺に当たっていた場合
            if (*MoveY < 0.0F)
            {
                // 移動量を補正する
                *MoveY = bby - playerY + 1.0F;

                // 上辺に当たったと返す
                return HitPosition::TOP;
            }

            // ここに来たら上辺に当たったとみなす
            return HitPosition::TOP;
        }
    }

    // どこにも当たらなかったと返す
    return HitPosition::NONE;
}

// 指定の座標に該当するマップのチップ番号を返す関数
int GetMapChipNoWithoutSlope(float X, float Y)
{
    int mapDataIndexX, mapDataIndexY;

    // 指定座標をマップチップのサイズで割ることで、マップチップの二次元配列のどの添え字に該当するかを割り出す
    mapDataIndexX = (int)X / CHIP_SIZE;
    mapDataIndexY = (int)Y / CHIP_SIZE;

    // マップチップの二次元配列からはみ出ていたら 0 を返す
    if (mapDataIndexX >= MAP_WIDTH || mapDataIndexY >= MAP_HEIGHT || mapDataIndexX < 0 || mapDataIndexY < 0) return 0;

    return MapData[mapDataIndexY][mapDataIndexX];
}

// 坂道ブロックを考慮したマップチップの値を取得する関数
int GetMapChipNoWithSlope(float playerX, float playerY)
{
    int Chip;

    Chip = GetMapChipNoWithoutSlope(playerX, playerY);

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
        blx = (float)((int)playerX / CHIP_SIZE) * CHIP_SIZE;        // 左辺の playerX 座標
        brx = (float)((int)playerX / CHIP_SIZE + 1) * CHIP_SIZE;    // 右辺の playerX 座標

        bty = (float)((int)playerY / CHIP_SIZE) * CHIP_SIZE;        // 上辺の playerY 座標
        bby = (float)((int)playerY / CHIP_SIZE + 1) * CHIP_SIZE;    // 下辺の playerY 座標

        // プレイヤーの位置がブロックのどの位置にあるか算出
        xr = (playerX - blx) / CHIP_SIZE;
        yr = (playerY - bty) / CHIP_SIZE;

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
            Chip = GetMapChipNoWithoutSlope(playerX, playerY + addy);

            // ブロックの上下左右の座標を再算出
            bty = (float)((int)(playerY + addy) / CHIP_SIZE) * CHIP_SIZE;        // 上辺の playerY 座標
            bby = (float)((int)(playerY + addy) / CHIP_SIZE + 1) * CHIP_SIZE;    // 下辺の playerY 座標

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
            if (playerY > blh)
            {
                return Chip;
            }
            break;

        case 3:    // 右方向下りブロック
            // プレイヤーの位置でのブロックの高さを算出
            blh = bty + xr * CHIP_SIZE;

            // ブロックに当たっているか判定
            if (playerY > blh)
            {
                return Chip;
            }
            break;

        case 4:    // 右方向下り天井ブロック
            // プレイヤーの位置でのブロックの高さを算出
            blh = bty + xr * CHIP_SIZE;

            // ブロックに当たっているか判定
            if (playerY < blh)
            {
                return Chip;
            }
            break;

        case 5:    // 右方向登り天井ブロック
            // プレイヤーの位置でのブロックの高さを算出
            blh = bby - xr * CHIP_SIZE;

            // ブロックに当たっているか判定
            if (playerY < blh)
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
