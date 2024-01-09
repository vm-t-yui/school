//-----------------------------------------------------------------------------
// @brief  アクションサンプルプログラム.
// 2019 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
// アクションサンプルプログラム
// ２Ｄアクションの基本、マップとの当たり判定です。
// 画面外に出たら見えないところで永遠に落ちてゆきますのでご注意
// 実際はキャラクタが真四角ということはないので、いろいろと改良を加える

// YUI HACK:
// 未来の移動先を指定し、そこに移動した場合どうなるかで処理を変えている
// もし未来の移動先でめり込むようなら、移動先を切り替えて再度処理する
// 「直接移動してから戻す」よりも、「前の場所からの移動距離が撮れる」「処理のやり直しが効く」
// 等のメリットがあるので、よく使われる手法
// 基本的に「指定座標がどのマップチップの中にあるか」で当たり判定を分けている
// 真四角にしか対応していないので、すべてのサイズに対応するには、
// 上下左右のポイントを指定している箇所の修正が必要

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
enum class BlockKind : char
{
    NONE = 0,
    NORMAL = 1,
};
char MapData[MAP_HEIGHT][MAP_WIDTH] =
{
    0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
    1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
    1,0,0,1,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
    1,0,0,1,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,1,0,1,
    1,0,0,1,1, 1,1,0,0,0, 0,0,0,0,0, 0,0,1,0,1,

    1,0,0,0,0, 0,0,0,1,1, 0,0,0,0,0, 0,0,1,0,1,
    1,0,0,0,0, 0,0,0,0,0, 0,0,1,1,0, 0,0,1,0,1,
    1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,1,0,1,
    1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
    1,0,0,0,0, 0,0,1,1,0, 0,0,0,0,0, 1,0,0,0,1,

    1,0,0,0,0, 1,1,1,1,1, 0,0,0,0,1, 1,0,0,0,1,
    1,0,0,0,0, 1,1,1,1,1, 0,0,0,1,1, 1,0,0,0,1,
    1,0,0,0,0, 0,0,0,0,0, 0,0,0,1,1, 1,0,0,0,1,
    1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
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
enum class HitPosition
{
    NONE,
    TOP,
    BOTTOM,
    LEFT,
    RIGHT
};


// プロトタイプ宣言
// 指定の座標に該当するマップのチップ番号を返す関数
BlockKind GetMapChipNo(float playerX, float playerY);

// プレイヤーをマップとの当たり判定を考慮しながら移動する関数
int MovePlayer(float* playerX, float* playerY, float* fallSpeed,
    float MoveX, float MoveY, float playerSize, bool* JumpFlag);

// マップとの当たり判定(（プレイヤーがマップチップのどの辺に当たったか：通常ブロックとの判定のみ )
HitPosition CheckHitMap(float playerX, float playerY, float* MoveX, float* MoveY);

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
    player.x = 135.0F;        player.y = 240.0F;

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

        // TODO: 関数化
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

        // TODO: 関数化
        // プレイヤーの移動処理
        {
            // パッドの入力により、落下スピード（ジャンプの場合は反転。重力に近い）
            float MoveX, MoveY;
            // 上下左右の移動量を決定
            {
                // 移動量の初期化
                MoveX = 0.0F;
                MoveY = 0.0F;

                // 左右の移動を見る
                if ((prevInput & PAD_INPUT_LEFT) != 0) MoveX -= SPEED;
                if ((prevInput & PAD_INPUT_RIGHT) != 0) MoveX += SPEED;

                // 地に足が着いている場合のみジャンプボタン(ボタン１ or Ｚキー)を見る
                if (player.isJump == FALSE && (nowInput & PAD_INPUT_A) != 0)
                {
                    player.fallSpeed = -JUMP_POWER;     // プレイヤーの落下スピードに反発する形でジャンプ力を減算
                    player.isJump = TRUE;
                }

                // ジャンプボタンを押していてもいなくても、重力によってプレイヤーの落下速度は加算される
                player.fallSpeed += G;

                // パッド入力による移動量の決定の後に、現在のプレイヤーの落下速度を移動量に加算
                MoveY = player.fallSpeed;
            }

            // 移動量に基づいてプレイヤーの座標を移動
            MovePlayer(&player.x, &player.y, &player.fallSpeed, MoveX, MoveY, PLAYER_SIZE, &player.isJump);
        }

        // マップの描画
        // TODO: 関数化
        {
            int i, j;

            for (i = 0; i < MAP_HEIGHT; i++)
            {
                for (j = 0; j < MAP_WIDTH; j++)
                {
                    // １はブロックチップを表しているので１のところだけ描画
                    switch (static_cast<BlockKind>(MapData[i][j]))
                    {
                        case BlockKind::NORMAL:
                            DrawBox(
                                j * CHIP_SIZE, i * CHIP_SIZE,
                                j * CHIP_SIZE + CHIP_SIZE, i * CHIP_SIZE + CHIP_SIZE,
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
    float Dummy = 0.0F; // 関数を動作させるため、あえて無効な値を入れていることを分かりやすくる
    float MoveXBackup = MoveX;
    float MoveYBackup = MoveY;

    // プレイヤーの左上、右上、左下、右下部分が当たり判定のある
    // マップに衝突しているか調べ、衝突していたら補正する

    // プレイヤー画像の半分のサイズを算出
    float playerSizeHalf = playerSize * 0.5F;

    // まず上下移動成分だけでチェックして、落下速度をコントロール
    {
        // プレイヤー矩形の左下座標がマップチップと当たっているかチェック
        // もしマップチップの上辺に当たっていたら（プレイヤーが地面についていたら）落下を止める
        if (CheckHitMap(*playerX - playerSizeHalf, *playerY + playerSizeHalf, &Dummy, &MoveY) == HitPosition::TOP)
            *fallSpeed = 0.0F;

        // プレイヤー矩形の右下座標がマップチップと当たっているかチェック、もしマップチップの上辺に当たっていたら落下を止める
        if (CheckHitMap(*playerX + playerSizeHalf, *playerY + playerSizeHalf, &Dummy, &MoveY) == HitPosition::TOP)
            *fallSpeed = 0.0F;

        // 上辺に当たる場合、ジャンプ中のときはジャンプ速度をそのまま反転する
        // 反転だと完全な力で反発して急なので、頭が当たったら止めたい等変更が必要な場合はここをいじる
        // プレイヤー矩形の左上座標がマップチップと当たっているかチェック
        // もしマップチップの下辺に当たっていたら（頭がチップの下にぶつかったら）落下を止める
        if (CheckHitMap(*playerX - playerSizeHalf, *playerY - playerSizeHalf, &Dummy, &MoveY) == HitPosition::BOTTOM && *fallSpeed < 0.0f)
            *fallSpeed *= -1.0F;

        // プレイヤー矩形の右上座標がマップチップと当たっているかチェック、もしマップチップの下辺に当たっていたら落下を止める
        if (CheckHitMap(*playerX + playerSizeHalf, *playerY - playerSizeHalf, &Dummy, &MoveY) == HitPosition::BOTTOM && *fallSpeed < 0.0f)
            *fallSpeed *= -1.0F;
    }

    // 次に左右移動成分だけでチェック
    {
        // 左下のチェック
        // プレイヤー矩形の左下座標がマップチップと当たっているかチェック
        CheckHitMap(*playerX - playerSizeHalf, *playerY + playerSizeHalf, &MoveX, &Dummy);

        // プレイヤー矩形の右下座標がマップチップと当たっているかチェック
        CheckHitMap(*playerX + playerSizeHalf, *playerY + playerSizeHalf, &MoveX, &Dummy);

        // プレイヤー矩形の左上座標がマップチップと当たっているかチェック
        CheckHitMap(*playerX - playerSizeHalf, *playerY - playerSizeHalf, &MoveX, &Dummy);

        // プレイヤー矩形の右上座標がマップチップと当たっているかチェック
        CheckHitMap(*playerX + playerSizeHalf, *playerY - playerSizeHalf, &MoveX, &Dummy);

        // 左右移動成分を加算
        *playerX += MoveX;
        MoveX = 0.0f;
    }

    // 終了
    return 0;
}

// マップとの当たり判定( 通常ブロックとの判定のみ )
// HACK: プレイヤーの幅と高さを考慮せず、中心座標がどこにあるかで当たっているか否かを判定している
// HACK: 移動先の方向でどこの辺に当たったかどうか決めつけているため、指定移動量によっては当たり判定が動かない
// HACK: MoveX と MoveY両方に移動量があった場合、MoveYが優先されて判定される（MoveX、MoveYそれぞれ別に判定する前提）
HitPosition CheckHitMap(float playerX, float playerY, float* MoveX, float* MoveY)
{
    // 移動後のプレイヤーの座標
    float afterPlayerX, afterPlayerY; 

    // 移動量を足す
    afterPlayerX = playerX + *MoveX;
    afterPlayerY = playerY + *MoveY;

    // 当たり判定のあるブロックに、移動後に当たるかチェック
    // 移動先の中心座標に存在するマップチップのマップ番号を取得し、
    // その番号が1（当たるブロック）だったら、あたっているものとする
    if (GetMapChipNo(afterPlayerX, afterPlayerY) == BlockKind::NORMAL)
    {
        // 自分と当たっているブロックの、上下左右辺のx/y座標を算出
        float hitBlockRight;
        float hitBlockLeft;
        float hitBlockTop;
        float hitBlockBottom;

        // ブロックの上下左右の座標を算出
        hitBlockLeft = (float)((int)afterPlayerX / CHIP_SIZE) * CHIP_SIZE;        // 左辺のx座標
        hitBlockRight = (float)((int)afterPlayerX / CHIP_SIZE + 1) * CHIP_SIZE;    // 右辺のx座標

        hitBlockTop = (float)((int)afterPlayerY / CHIP_SIZE) * CHIP_SIZE;        // 上辺のy座標
        hitBlockBottom = (float)((int)afterPlayerY / CHIP_SIZE + 1) * CHIP_SIZE;    // 下辺のy座標

        // 移動量が上方向に向かっていた場合
        if (*MoveY > 0.0F)
        {
            // 移動量を補正する
            *MoveY = hitBlockTop - playerY - 1.0F;

            // 上辺に当たったと返す
            return HitPosition::TOP;
        }

        // 移動量が下方向に向かっていた場合
        if (*MoveY < 0.0F)
        {
            // 移動量を補正する
            *MoveY = hitBlockBottom - playerY + 1.0F;

            // 下辺に当たったと返す
            return HitPosition::BOTTOM;
        }

        // 移動量が右方向に向かっていた場合
        if (*MoveX > 0.0F)
        {
            // 移動量を補正する
            *MoveX = hitBlockLeft - playerX - 1.0F;

            // 左辺に当たったと返す
            return HitPosition::LEFT;
        }

        // 移動量が左方向に向かっていた場合
        if (*MoveX < 0.0F)
        {
            // 移動量を補正する
            *MoveX = hitBlockRight - playerX + 1.0F;

            // 右辺に当たったと返す
            return HitPosition::RIGHT;
        }

        // ここに来たら下辺に当たったと見なす
        return HitPosition::BOTTOM;
    }

    // どこにも当たらなかったと返す
    return HitPosition::NONE;
}

// 指定の座標に該当するマップのチップ番号を返す関数
BlockKind GetMapChipNo(float X, float Y)
{
    int mapDataIndexX, mapDataIndexY;

    // 指定座標をマップチップのサイズで割ることで、マップチップの二次元配列のどの添え字に該当するかを割り出す
    mapDataIndexX = (int)X / CHIP_SIZE;
    mapDataIndexY = (int)Y / CHIP_SIZE;

    // マップチップの二次元配列からはみ出ていたら 0 を返す
    if (mapDataIndexX >= MAP_WIDTH || mapDataIndexY >= MAP_HEIGHT || mapDataIndexX < 0 || mapDataIndexY < 0) return BlockKind::NONE;

    return static_cast<BlockKind>(MapData[mapDataIndexY][mapDataIndexX]);
}
