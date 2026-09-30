// 2026 Takeru Yui All rights reserved.
// -----------------------------------------------------------------------------
// 2.5Dシューティングゲーム テンプレート
//
// ゲームの考え方は2Dのまま、3Dモデルを使って表示するサンプル
// 基本的な改造はこのmain.cppだけで行う
//
// 3Dモデルの詳しい扱いに進みたい場合はSystem以下を直接利用・改造してよい
// -----------------------------------------------------------------------------
#include <cstring>
#include "DxLib.h"
#include "BeginnerSupport/Beginner2D3D.h"

// -----------------------------------------------------------------------------
// 定数
// -----------------------------------------------------------------------------
static const int ScreenWidth  = 1600;
static const int ScreenHeight = 900;

static const float PlayerSpeed             = 3.0f;
static const int   PlayerLife              = 10;
static const float PlayerLogicalWidth      = 50.0f;
static const float PlayerLogicalHeight     = 50.0f;
static const float PlayerModelScale        = 0.005f;
static const float PlayerModelOffsetX      = 0.0f;
static const float PlayerModelOffsetY      = 30.0f;
static const float PlayerStartBottomOffset = 100.0f;

static const int   PlayerShotCount          = 3;
static const int   PlayerShotPower          = 1;
static const float PlayerShotSpeed          = 5.0f;
static const int   PlayerShotIntervalFrames = 10;
static const float PlayerShotLogicalWidth   = 24.0f;
static const float PlayerShotLogicalHeight  = 24.0f;
static const float PlayerShotModelScale     = 0.002f;

static const float EnemySpeed          = 3.0f;
static const int   EnemyLife           = 10;
static const float EnemyLogicalWidth   = 50.0f;
static const float EnemyLogicalHeight  = 50.0f;
static const float EnemyModelScale     = 0.003f;
static const float EnemyModelOffsetX   = 0.0f;
static const float EnemyModelOffsetY   = 22.0f;
static const float EnemyStartTopOffset = 50.0f;

static const int   EnemyShotCount          = 3;
static const int   EnemyShotPower          = 1;
static const float EnemyShotSpeed          = 5.0f;
static const int   EnemyShotIntervalFrames = 10;
static const float EnemyShotLogicalWidth   = 24.0f;
static const float EnemyShotLogicalHeight  = 24.0f;
static const float EnemyShotModelScale     = 0.002f;

static const int   DamageDisplayFrames   = 5;
static const float BackgroundScrollSpeed = 2.0f;
static const int   BackgroundCount       = 3;

static const int TimeLimitSeconds            = 30;
static const int StateChangeWaitMilliseconds = 500;

// UIに使う文字列
static const char TitleText[]       = "シューティング";
static const char GameOverText[]    = "ゲームオーバー";
static const char GameClearText[]   = "ゲームクリア！";
static const char StartButtonText[] = "スペースキーでスタート";
static const char ReturnTitleText[] = "スペースキーでタイトルへ";
static const char TimeText[]        = "残り：";

// -----------------------------------------------------------------------------
// ゲームデータ
// -----------------------------------------------------------------------------
class Player
{
public:
    int   modelHandle         = -1;
    float x                   = 0.0f;
    float y                   = 0.0f;
    float width               = 0.0f;
    float height              = 0.0f;
    int   life                = 0;
    bool  isDamaged           = false;
    int   damageCounter       = 0;
    int   shotIntervalCounter = 0;
};

class Enemy
{
public:
    int   modelHandle         = -1;
    int   damageModelHandle   = -1;
    float x                   = 0.0f;
    float y                   = 0.0f;
    float width               = 0.0f;
    float height              = 0.0f;
    int   life                = 0;
    bool  isDamaged           = false;
    int   damageCounter       = 0;
    int   shotIntervalCounter = 0;
    bool  isMovingRight       = true;
};

class Shot
{
public:
    int   modelHandle = -1;
    float x           = 0.0f;
    float y           = 0.0f;
    float width       = 0.0f;
    float height      = 0.0f;
    bool  isVisible   = false;
};

class Background
{
public:
    int   graphHandle = -1;
    float y           = 0.0f;
    float height      = 0.0f;
};

enum class GameState
{
    Title,
    Game,
    Clear,
    GameOver
};

class Rule
{
public:
    int       gameStartTime      = 0;
    GameState state              = GameState::Title;
    bool      wasSpaceKeyPressed = false;
    bool      isSpaceKeyReleased = false;
};

// -----------------------------------------------------------------------------
// グローバル変数
// -----------------------------------------------------------------------------
Player     player;
Enemy      enemy;
Shot       playerShots[PlayerShotCount];
Shot       enemyShots[EnemyShotCount];
Background backgrounds[BackgroundCount];
Rule       rule;
int        hpBarGraphHandle        = -1;
int        hpBackgroundGraphHandle = -1;

// -----------------------------------------------------------------------------
// Player
// -----------------------------------------------------------------------------
/// <summary>
/// プレイヤーを初期化する
/// </summary>
void InitializePlayer()
{
    // モデルの読み込みと表示設定は最初の1回だけ行う
    if (player.modelHandle < 0)
    {
        player.modelHandle = LoadModel2D("data/model/Player/Player_Model.mv1");

        // 論理サイズは当たり判定など、2Dゲーム上で扱う大きさ
        SetModelSize2D(player.modelHandle, PlayerLogicalWidth, PlayerLogicalHeight);

        // Scaleは3Dモデル自体の表示サイズなので、使用する素材に合わせて調整する
        SetModelScale2D(player.modelHandle, PlayerModelScale);

        // 多くの3Dモデルは足元付近が原点になっているため、
        // 2Dゲーム上の中心と見た目の中心が合うように描画位置を調整する
        // +Yは画面下方向。使用するモデル素材に合わせて値を調整する
        SetModelOffset2D(player.modelHandle, PlayerModelOffsetX, PlayerModelOffsetY);

        // 現在の配布モデルは回転不要なので0度
        // 別のモデルを使う場合は、素材の向きに合わせて調整する
        SetModelRotation2D(player.modelHandle, 0.0f, 0.0f, 0.0f);
    }

    // ゲーム開始時の状態は毎回初期化する
    GetModelSize2D(player.modelHandle, &player.width, &player.height);

    player.x                   = ScreenWidth / 2.0f;
    player.y                   = ScreenHeight - PlayerStartBottomOffset;
    player.life                = PlayerLife;
    player.isDamaged           = false;
    player.damageCounter       = 0;
    player.shotIntervalCounter = 0;
}

/// <summary>
/// プレイヤーの入力、移動、ショット発射、ダメージ状態を更新する
/// </summary>
void UpdatePlayer()
{
    if (CheckHitKey(KEY_INPUT_LEFT) == 1)
    {
        player.x -= PlayerSpeed;
    }

    if (CheckHitKey(KEY_INPUT_RIGHT) == 1)
    {
        player.x += PlayerSpeed;
    }

    if (CheckHitKey(KEY_INPUT_SPACE) == 1 && player.shotIntervalCounter == 0)
    {
        for (int i = 0; i < PlayerShotCount; i++)
        {
            if (!playerShots[i].isVisible)
            {
                // PlayerとShotはどちらも中心座標なので、そのまま発射位置に使える
                playerShots[i].x         = player.x;
                playerShots[i].y         = player.y;
                playerShots[i].isVisible = true;
                break;
            }
        }

        player.shotIntervalCounter = PlayerShotIntervalFrames;
    }

    if (player.shotIntervalCounter > 0)
    {
        --player.shotIntervalCounter;
    }

    // 中心座標から半分のサイズを引いた位置が画面端になるように補正する
    if (player.x < player.width / 2.0f)
    {
        player.x = player.width / 2.0f;
    }
    if (player.x > ScreenWidth - player.width / 2.0f)
    {
        player.x = ScreenWidth - player.width / 2.0f;
    }

    if (player.isDamaged)
    {
        ++player.damageCounter;

        if (player.damageCounter >= DamageDisplayFrames)
        {
            player.isDamaged = false;
        }
    }
}

/// <summary>
/// プレイヤーを描画する
/// </summary>
void DrawPlayer()
{
    // ダメージ中は短時間だけ非表示にして被弾を表現する
    if (!player.isDamaged)
    {
        DrawModel2D(player.x, player.y, player.modelHandle);
    }
}

// -----------------------------------------------------------------------------
// Enemy
// -----------------------------------------------------------------------------
/// <summary>
/// 敵を初期化する
/// </summary>
void InitializeEnemy()
{
    // モデルの読み込みと表示設定は最初の1回だけ行う
    if (enemy.modelHandle < 0)
    {
        enemy.modelHandle = LoadModel2D("data/model/Enemy/Enemy.mv1");
        SetModelSize2D(enemy.modelHandle, EnemyLogicalWidth, EnemyLogicalHeight);
        SetModelScale2D(enemy.modelHandle, EnemyModelScale);

        // 足元付近のモデル原点を、2Dゲーム上の見た目の中心へ合わせる
        // 使用するモデル素材に合わせて値を調整する
        SetModelOffset2D(enemy.modelHandle, EnemyModelOffsetX, EnemyModelOffsetY);

        // 現在の配布モデルは回転不要なので0度
        // 別のモデルを使う場合は、素材の向きに合わせて調整する
        SetModelRotation2D(enemy.modelHandle, 0.0f, 0.0f, 0.0f);
    }

    if (enemy.damageModelHandle < 0)
    {
        // ダメージ時は2D画像を差し替えるのと同じ考え方で別モデルへ切り替える
        // 実際の配布素材名に合わせてファイル名を調整する
        enemy.damageModelHandle = LoadModel2D("data/model/Enemy/Enemy_Damage.mv1");
        SetModelSize2D(enemy.damageModelHandle, EnemyLogicalWidth, EnemyLogicalHeight);
        SetModelScale2D(enemy.damageModelHandle, EnemyModelScale);
        SetModelOffset2D(enemy.damageModelHandle, EnemyModelOffsetX, EnemyModelOffsetY);
        SetModelRotation2D(enemy.damageModelHandle, 0.0f, 0.0f, 0.0f);
    }

    // ゲーム開始時の状態は毎回初期化する
    GetModelSize2D(enemy.modelHandle, &enemy.width, &enemy.height);

    enemy.x                   = enemy.width / 2.0f;
    enemy.y                   = EnemyStartTopOffset + enemy.height / 2.0f;
    enemy.life                = EnemyLife;
    enemy.isDamaged           = false;
    enemy.damageCounter       = 0;
    enemy.shotIntervalCounter = 0;
    enemy.isMovingRight       = true;
}

/// <summary>
/// 敵の移動、ショット発射、ダメージ状態を更新する
/// </summary>
void UpdateEnemy()
{
    if (enemy.isMovingRight)
    {
        enemy.x += EnemySpeed;
    }
    else
    {
        enemy.x -= EnemySpeed;
    }

    if (enemy.x > ScreenWidth - enemy.width / 2.0f)
    {
        enemy.x             = ScreenWidth - enemy.width / 2.0f;
        enemy.isMovingRight = false;
    }
    else if (enemy.x < enemy.width / 2.0f)
    {
        enemy.x             = enemy.width / 2.0f;
        enemy.isMovingRight = true;
    }

    if (enemy.shotIntervalCounter == 0)
    {
        for (int i = 0; i < EnemyShotCount; i++)
        {
            if (!enemyShots[i].isVisible)
            {
                enemyShots[i].x         = enemy.x;
                enemyShots[i].y         = enemy.y;
                enemyShots[i].isVisible = true;
                break;
            }
        }

        enemy.shotIntervalCounter = EnemyShotIntervalFrames;
    }

    if (enemy.shotIntervalCounter > 0)
    {
        --enemy.shotIntervalCounter;
    }

    if (enemy.isDamaged)
    {
        ++enemy.damageCounter;

        if (enemy.damageCounter >= DamageDisplayFrames)
        {
            enemy.isDamaged = false;
        }
    }
}

/// <summary>
/// 敵を描画する
/// </summary>
void DrawEnemy()
{
    if (enemy.life <= 0)
    {
        return;
    }

    if (enemy.isDamaged)
    {
        DrawModel2D(enemy.x, enemy.y, enemy.damageModelHandle);
    }
    else
    {
        DrawModel2D(enemy.x, enemy.y, enemy.modelHandle);
    }
}

// -----------------------------------------------------------------------------
// Shot
// -----------------------------------------------------------------------------
/// <summary>
/// プレイヤーと敵のショットを初期化する
/// </summary>
void InitializeShots()
{
    // モデルの読み込みと表示設定は最初の1回だけ行う
    int playerShotModelHandle = playerShots[0].modelHandle;
    if (playerShotModelHandle < 0)
    {
        playerShotModelHandle = LoadModel2D("data/model/Shot/SpikyBall.mv1");
        SetModelSize2D(playerShotModelHandle, PlayerShotLogicalWidth, PlayerShotLogicalHeight);
        SetModelScale2D(playerShotModelHandle, PlayerShotModelScale);
    }

    int enemyShotModelHandle = enemyShots[0].modelHandle;
    if (enemyShotModelHandle < 0)
    {
        // プレイヤー弾とは色違いの別モデルを使う想定
        // 実際の配布素材名に合わせてファイル名を調整する
        enemyShotModelHandle = LoadModel2D("data/model/Shot/SpikyBall_Enemy.mv1");
        SetModelSize2D(enemyShotModelHandle, EnemyShotLogicalWidth, EnemyShotLogicalHeight);
        SetModelScale2D(enemyShotModelHandle, EnemyShotModelScale);
    }

    float playerShotWidth  = 0.0f;
    float playerShotHeight = 0.0f;
    GetModelSize2D(playerShotModelHandle, &playerShotWidth, &playerShotHeight);

    for (int i = 0; i < PlayerShotCount; i++)
    {
        playerShots[i].modelHandle = playerShotModelHandle;
        playerShots[i].width       = playerShotWidth;
        playerShots[i].height      = playerShotHeight;
        playerShots[i].isVisible   = false;
    }

    float enemyShotWidth  = 0.0f;
    float enemyShotHeight = 0.0f;
    GetModelSize2D(enemyShotModelHandle, &enemyShotWidth, &enemyShotHeight);

    for (int i = 0; i < EnemyShotCount; i++)
    {
        enemyShots[i].modelHandle = enemyShotModelHandle;
        enemyShots[i].width       = enemyShotWidth;
        enemyShots[i].height      = enemyShotHeight;
        enemyShots[i].isVisible   = false;
    }
}

/// <summary>
/// ショットの移動と当たり判定を更新する
/// </summary>
void UpdateShots()
{
    for (int i = 0; i < PlayerShotCount; i++)
    {
        if (playerShots[i].isVisible)
        {
            playerShots[i].y -= PlayerShotSpeed;

            if (playerShots[i].y < -playerShots[i].height / 2.0f)
            {
                playerShots[i].isVisible = false;
            }

            if (playerShots[i].isVisible && enemy.life > 0)
            {
                // 中心座標から幅と高さの半分ずつ広げて当たり判定の端を求める
                const float shotLeft   = playerShots[i].x - playerShots[i].width / 2.0f;
                const float shotRight  = playerShots[i].x + playerShots[i].width / 2.0f;
                const float shotTop    = playerShots[i].y - playerShots[i].height / 2.0f;
                const float shotBottom = playerShots[i].y + playerShots[i].height / 2.0f;

                const float enemyLeft   = enemy.x - enemy.width / 2.0f;
                const float enemyRight  = enemy.x + enemy.width / 2.0f;
                const float enemyTop    = enemy.y - enemy.height / 2.0f;
                const float enemyBottom = enemy.y + enemy.height / 2.0f;

                const bool isHit =
                    shotLeft < enemyRight &&
                    shotRight > enemyLeft &&
                    shotTop < enemyBottom &&
                    shotBottom > enemyTop;

                if (isHit)
                {
                    playerShots[i].isVisible = false;
                    enemy.isDamaged          = true;
                    enemy.damageCounter      = 0;
                    enemy.life -= PlayerShotPower;

                    // 発展:
                    // 配布モデルに含まれるDamageアニメーションを使う場合は
                    // System::Animationをゲーム側に用意して直接利用できる
                }
            }
        }
    }

    for (int i = 0; i < EnemyShotCount; i++)
    {
        if (enemyShots[i].isVisible)
        {
            enemyShots[i].y += EnemyShotSpeed;

            if (enemyShots[i].y > ScreenHeight + enemyShots[i].height / 2.0f)
            {
                enemyShots[i].isVisible = false;
            }

            if (enemyShots[i].isVisible && player.life > 0)
            {
                const float shotLeft   = enemyShots[i].x - enemyShots[i].width / 2.0f;
                const float shotRight  = enemyShots[i].x + enemyShots[i].width / 2.0f;
                const float shotTop    = enemyShots[i].y - enemyShots[i].height / 2.0f;
                const float shotBottom = enemyShots[i].y + enemyShots[i].height / 2.0f;

                const float playerLeft   = player.x - player.width / 2.0f;
                const float playerRight  = player.x + player.width / 2.0f;
                const float playerTop    = player.y - player.height / 2.0f;
                const float playerBottom = player.y + player.height / 2.0f;

                const bool isHit =
                    shotLeft < playerRight &&
                    shotRight > playerLeft &&
                    shotTop < playerBottom &&
                    shotBottom > playerTop;

                if (isHit)
                {
                    enemyShots[i].isVisible = false;
                    player.isDamaged        = true;
                    player.damageCounter    = 0;
                    player.life -= EnemyShotPower;
                }
            }
        }
    }
}

/// <summary>
/// プレイヤーと敵のショットを描画する
/// </summary>
void DrawShots()
{
    for (int i = 0; i < PlayerShotCount; i++)
    {
        if (playerShots[i].isVisible)
        {
            DrawModel2D(playerShots[i].x, playerShots[i].y, playerShots[i].modelHandle);
        }
    }

    for (int i = 0; i < EnemyShotCount; i++)
    {
        if (enemyShots[i].isVisible)
        {
            DrawModel2D(enemyShots[i].x, enemyShots[i].y, enemyShots[i].modelHandle);
        }
    }
}

// -----------------------------------------------------------------------------
// Background
// -----------------------------------------------------------------------------
/// <summary>
/// スクロールする背景を初期化する
/// </summary>
void InitializeBackground()
{
    // 背景画像の読み込みは最初の1回だけ行う
    if (backgrounds[0].graphHandle < 0)
    {
        const int backgroundGraphHandle = LoadGraph("data/texture/FancyBG_back.png");

        int backgroundWidth  = 0;
        int backgroundHeight = 0;
        GetGraphSize(backgroundGraphHandle, &backgroundWidth, &backgroundHeight);

        for (int i = 0; i < BackgroundCount; i++)
        {
            backgrounds[i].graphHandle = backgroundGraphHandle;
            backgrounds[i].height      = static_cast<float>(backgroundHeight);
        }
    }

    // 1枚を画面中央、残りを上下に並べて途切れないスクロールにする
    for (int i = 0; i < BackgroundCount; i++)
    {
        backgrounds[i].y =
            ScreenHeight / 2.0f +
            (i - BackgroundCount / 2) * backgrounds[i].height;
    }
}

/// <summary>
/// 背景の縦スクロールを更新する
/// </summary>
void UpdateBackground()
{
    for (int i = 0; i < BackgroundCount; i++)
    {
        backgrounds[i].y += BackgroundScrollSpeed;

        // 背景全体が画面下へ抜けたら、並びの一番上へ戻す
        if (backgrounds[i].y - backgrounds[i].height / 2.0f > ScreenHeight)
        {
            backgrounds[i].y -= backgrounds[i].height * BackgroundCount;
        }
    }
}

/// <summary>
/// 背景画像を3Dモデルより奥へ描画する
/// </summary>
void DrawBackground()
{
    for (int i = 0; i < BackgroundCount; i++)
    {
        DrawGraph3D(ScreenWidth / 2.0f, backgrounds[i].y, backgrounds[i].graphHandle);
    }
}

// -----------------------------------------------------------------------------
// UI
// -----------------------------------------------------------------------------
/// <summary>
/// 文字列全体の描画幅を取得する
/// </summary>
int GetFullStringWidth(const char* text)
{
    return GetDrawStringWidth(text, static_cast<int>(std::strlen(text)));
}

/// <summary>
/// UIで使用する画像を初期化する
/// </summary>
void InitializeUI()
{
    hpBarGraphHandle        = LoadGraph("data/texture/hp.png");
    hpBackgroundGraphHandle = LoadGraph("data/texture/hpBack.png");
}

/// <summary>
/// 現在のゲーム状態に応じたUIを描画する
/// </summary>
void DrawUI()
{
    const int titleTextOffsetY     = -100;
    const int pressTextOffsetY     = 100;
    const int timeTextOffsetX      = -100;
    const int timeTextPositionY    = 25;
    const int titleFontSize        = 64;
    const int pressTextFontSize    = 24;
    const int timeTextFontSize     = 24;
    const int timeNumberFontSize   = 40;
    const int enemyHpTextFontSize  = 24;
    const int enemyHpTextPositionY = 2;
    const int enemyHpTextPositionX = 10;
    const int enemyHpLeftOffsetX   = 80;
    const int enemyHpRightOffsetX  = 30;
    const int enemyHpHeight        = 15;
    const int playerHpOffsetY      = 5;

    char timeNumberText[256];
    char lifeText[256];

    switch (rule.state)
    {
    case GameState::Title:
        SetFontSize(titleFontSize);
        DrawString(
            ScreenWidth / 2 - GetFullStringWidth(TitleText) / 2,
            ScreenHeight / 2 + titleTextOffsetY,
            TitleText,
            GetColor(255, 255, 255));

        SetFontSize(pressTextFontSize);
        DrawString(
            ScreenWidth / 2 - GetFullStringWidth(StartButtonText) / 2,
            ScreenHeight / 2 + pressTextOffsetY,
            StartButtonText,
            GetColor(255, 255, 255));
        break;

    case GameState::Game:
    {
        SetFontSize(timeTextFontSize);
        DrawString(
            ScreenWidth / 2 + timeTextOffsetX,
            timeTextPositionY,
            TimeText,
            GetColor(255, 0, 0));

        const int remainingSeconds = TimeLimitSeconds - (GetNowCount() - rule.gameStartTime) / 1000;
        sprintf_s(timeNumberText, "%d", remainingSeconds);

        SetFontSize(timeNumberFontSize);
        DrawString(
            ScreenWidth / 2 - GetFullStringWidth(timeNumberText) / 2,
            timeTextPositionY,
            timeNumberText,
            GetColor(255, 0, 0));

        sprintf_s(lifeText, "HP:%d", player.life);
        SetFontSize(timeTextFontSize);
        DrawString(
            static_cast<int>(player.x) - GetFullStringWidth(lifeText) / 2,
            static_cast<int>(player.y + player.height / 2.0f) + playerHpOffsetY,
            lifeText,
            GetColor(255, 0, 0));

        SetFontSize(enemyHpTextFontSize);
        DrawString(
            enemyHpTextPositionX,
            enemyHpTextPositionY,
            "Enemy",
            GetColor(255, 0, 0));

        const int hpGraphStartX = enemyHpTextPositionX + enemyHpLeftOffsetX;

        DrawExtendGraph(
            hpGraphStartX,
            enemyHpTextPositionY,
            ScreenWidth - enemyHpRightOffsetX,
            enemyHpTextPositionY + enemyHpHeight,
            hpBackgroundGraphHandle,
            TRUE);

        DrawExtendGraph(
            hpGraphStartX,
            enemyHpTextPositionY,
            hpGraphStartX + static_cast<int>(
                (ScreenWidth - hpGraphStartX - enemyHpRightOffsetX) *
                (static_cast<float>(enemy.life) / EnemyLife)),
            enemyHpTextPositionY + enemyHpHeight,
            hpBarGraphHandle,
            TRUE);
        break;
    }

    case GameState::Clear:
        SetFontSize(titleFontSize);
        DrawString(
            ScreenWidth / 2 - GetFullStringWidth(GameClearText) / 2,
            ScreenHeight / 2 + titleTextOffsetY,
            GameClearText,
            GetColor(255, 255, 0));

        SetFontSize(pressTextFontSize);
        DrawString(
            ScreenWidth / 2 - GetFullStringWidth(ReturnTitleText) / 2,
            ScreenHeight / 2 + pressTextOffsetY,
            ReturnTitleText,
            GetColor(255, 255, 255));
        break;

    case GameState::GameOver:
        SetFontSize(titleFontSize);
        DrawString(
            ScreenWidth / 2 - GetFullStringWidth(GameOverText) / 2,
            ScreenHeight / 2 + titleTextOffsetY,
            GameOverText,
            GetColor(255, 0, 0));

        SetFontSize(pressTextFontSize);
        DrawString(
            ScreenWidth / 2 - GetFullStringWidth(ReturnTitleText) / 2,
            ScreenHeight / 2 + pressTextOffsetY,
            ReturnTitleText,
            GetColor(255, 255, 255));
        break;
    }
}

// -----------------------------------------------------------------------------
// Rule
// -----------------------------------------------------------------------------
/// <summary>
/// ゲームルールを初期化する
/// </summary>
void InitializeRule()
{
    rule.gameStartTime      = 0;
    rule.state              = GameState::Title;
    rule.wasSpaceKeyPressed = false;
    rule.isSpaceKeyReleased = false;
}

/// <summary>
/// ゲームの状態を切り替える
/// </summary>
void ChangeState(GameState state)
{
    // 画面が一瞬で切り替わる違和感を減らすため、少しだけ待つ
    // 本来はフェードイン・フェードアウトなどの画面遷移演出を実装する方が望ましい
    WaitTimer(StateChangeWaitMilliseconds);

    rule.wasSpaceKeyPressed = false;
    rule.isSpaceKeyReleased = false;
    rule.state              = state;

    switch (rule.state)
    {
    case GameState::Title:
        break;

    case GameState::Game:
        rule.gameStartTime = GetNowCount();
        InitializePlayer();
        InitializeEnemy();
        InitializeShots();
        InitializeBackground();
        break;

    case GameState::Clear:
        break;

    case GameState::GameOver:
        break;
    }
}

/// <summary>
/// キー入力とゲーム状態を更新する
/// </summary>
void UpdateRule()
{
    const bool isSpaceKeyPressed = CheckHitKey(KEY_INPUT_SPACE) == 1;
    rule.isSpaceKeyReleased = rule.wasSpaceKeyPressed && !isSpaceKeyPressed;
    rule.wasSpaceKeyPressed = isSpaceKeyPressed;

    switch (rule.state)
    {
    case GameState::Title:
        if (rule.isSpaceKeyReleased)
        {
            ChangeState(GameState::Game);
        }
        break;

    case GameState::Game:
        if (enemy.life <= 0)
        {
            ChangeState(GameState::Clear);
        }
        else if (player.life <= 0 || GetNowCount() - rule.gameStartTime > TimeLimitSeconds * 1000)
        {
            ChangeState(GameState::GameOver);
        }
        break;

    case GameState::Clear:
    case GameState::GameOver:
        if (rule.isSpaceKeyReleased)
        {
            ChangeState(GameState::Title);
        }
        break;
    }
}

// -----------------------------------------------------------------------------
// Main
// -----------------------------------------------------------------------------
/// <summary>
/// アプリケーションを起動してゲームループを実行する
/// </summary>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    const int screenColorBit = 32;

    SetGraphMode(ScreenWidth, ScreenHeight, screenColorBit);
    ChangeWindowMode(TRUE);

    // このテンプレートではゲームロジックを分かりやすくするため60FPS固定で動かす
    // モニターのリフレッシュレートでゲーム速度が変わらないよう垂直同期は使用しない
    // 実際のゲームでは経過時間や垂直同期を含め、内容に合った時間管理を検討する
    SetWaitVSyncFlag(FALSE);

    if (DxLib_Init() == -1)
    {
        return -1;
    }

    SetDrawScreen(DX_SCREEN_BACK);

    // ここで疑似2D表示に必要な3Dカメラと描画設定を一度だけ初期化する
    // FOVなどを変更したい場合はBeginner2D3Dの前提も確認する
    Initialize2D3D();

    InitializeRule();
    InitializePlayer();
    InitializeEnemy();
    InitializeShots();
    InitializeBackground();
    InitializeUI();

    const long long oneFrameMicroseconds = 16667;
    long long lastScreenFlipTime         = GetNowHiPerformanceCount();

    while (true)
    {
        ClearDrawScreen();

        if (rule.state == GameState::Game)
        {
            UpdatePlayer();
            UpdateEnemy();
            UpdateShots();
            UpdateBackground();
        }
        UpdateRule();

        if (rule.state == GameState::Game)
        {
            DrawBackground();
            DrawShots();
            DrawPlayer();
            DrawEnemy();
        }
        DrawUI();

        // 前回の画面更新から約1/60秒経過するまで待つ
        // ChangeStateなどで長く停止した場合は、遅れを取り戻そうとせず次のフレームから測り直す
        while (GetNowHiPerformanceCount() - lastScreenFlipTime < oneFrameMicroseconds)
        {
            // 処理なし
        }

        ScreenFlip();
        lastScreenFlipTime = GetNowHiPerformanceCount();

        if (ProcessMessage() < 0 || CheckHitKey(KEY_INPUT_ESCAPE) == 1)
        {
            break;
        }
    }

    // 初学者用テンプレートでは個別のリソース解放を扱わずDxLib_Endに任せる
    DxLib_End();
    return 0;
}
