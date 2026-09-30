// 2026 Takeru Yui All rights reserved.
// -----------------------------------------------------------------------------
// 2.5Dシューティングゲーム 超入門テンプレート
//
// ゲームの考え方は2Dのまま、3Dモデルを使って表示するサンプル
// 自作関数やデータ用classを使わず、処理をWinMainの中だけに書いた入門版
//
// 慣れてきたら通常版main.cppで、関数分割やclassを使った整理方法を確認する
// -----------------------------------------------------------------------------
#include <cstring>
#include "DxLib.h"
#include "BeginnerSupport/Beginner2D3D.h"

// -----------------------------------------------------------------------------
// Main
// -----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // -------------------------------------------------------------------------
    // 定数
    // -------------------------------------------------------------------------
    static const int ScreenWidth  = 1600;
    static const int ScreenHeight = 900;

    // LogicalSizeは2Dゲーム上の大きさ、ModelScaleは3Dモデルの見た目の大きさ
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

    // ダメージ表示時間と背景スクロールはフレーム単位で調整する
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

    // -------------------------------------------------------------------------
    // ゲーム状態
    // -------------------------------------------------------------------------
    enum class GameState
    {
        Title,
        Game,
        Clear,
        GameOver
    };

    // -------------------------------------------------------------------------
    // Playerの変数
    // -------------------------------------------------------------------------
    int   playerModelHandle         = -1;
    float playerX                   = 0.0f;
    float playerY                   = 0.0f;
    float playerWidth               = 0.0f;
    float playerHeight              = 0.0f;
    int   playerLife                = 0;
    bool  playerIsDamaged           = false;
    int   playerDamageCounter       = 0;
    int   playerShotIntervalCounter = 0;

    // -------------------------------------------------------------------------
    // Enemyの変数
    // -------------------------------------------------------------------------
    int   enemyModelHandle         = -1;
    int   enemyDamageModelHandle   = -1;
    float enemyX                   = 0.0f;
    float enemyY                   = 0.0f;
    float enemyWidth               = 0.0f;
    float enemyHeight              = 0.0f;
    int   enemyLife                = 0;
    bool  enemyIsDamaged           = false;
    int   enemyDamageCounter       = 0;
    int   enemyShotIntervalCounter = 0;
    bool  enemyIsMovingRight       = true;

    // -------------------------------------------------------------------------
    // Shotの変数
    // -------------------------------------------------------------------------
    // 同じ番号のX / Y / IsVisibleを組み合わせて、1発分の弾として扱う
    int   playerShotModelHandle                 = -1;
    float playerShotX[PlayerShotCount]          = {};
    float playerShotY[PlayerShotCount]          = {};
    bool  playerShotIsVisible[PlayerShotCount]  = {};
    float playerShotWidth                       = 0.0f;
    float playerShotHeight                      = 0.0f;

    // Enemy ShotもPlayer Shotと同じ考え方で管理する
    int   enemyShotModelHandle                  = -1;
    float enemyShotX[EnemyShotCount]            = {};
    float enemyShotY[EnemyShotCount]            = {};
    bool  enemyShotIsVisible[EnemyShotCount]    = {};
    float enemyShotWidth                        = 0.0f;
    float enemyShotHeight                       = 0.0f;

    // -------------------------------------------------------------------------
    // Backgroundの変数
    // -------------------------------------------------------------------------
    int   backgroundGraphHandle            = -1;
    float backgroundY[BackgroundCount]     = {};
    float backgroundHeight                 = 0.0f;

    // -------------------------------------------------------------------------
    // RuleとUIの変数
    // -------------------------------------------------------------------------
    int       ruleGameStartTime      = 0;
    GameState ruleState              = GameState::Title;
    bool      ruleWasSpaceKeyPressed = false;
    bool      ruleIsSpaceKeyReleased = false;

    int uiHpBarGraphHandle        = -1;
    int uiHpBackgroundGraphHandle = -1;

    // -------------------------------------------------------------------------
    // DxLibの初期化
    // -------------------------------------------------------------------------
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

    // 疑似2D表示に必要な3Dカメラと描画設定を一度だけ初期化する
    Initialize2D3D();

    // -------------------------------------------------------------------------
    // Playerモデルの準備
    // -------------------------------------------------------------------------
    playerModelHandle = LoadModel2D("data/model/Player/Player_Model.mv1");

    // 論理サイズは当たり判定など、2Dゲーム上で扱う大きさ
    SetModelSize2D(playerModelHandle, PlayerLogicalWidth, PlayerLogicalHeight);

    // Scaleは3Dモデル自体の表示サイズなので、使用する素材に合わせて調整する
    SetModelScale2D(playerModelHandle, PlayerModelScale);

    // 多くの3Dモデルは足元付近が原点になっているため、
    // 2Dゲーム上の中心と見た目の中心が合うように描画位置を調整する
    SetModelOffset2D(playerModelHandle, PlayerModelOffsetX, PlayerModelOffsetY);

    // 現在の配布モデルは回転不要なので0度
    // 別のモデルを使う場合は、素材の向きに合わせて調整する
    SetModelRotation2D(playerModelHandle, 0.0f, 0.0f, 0.0f);

    GetModelSize2D(playerModelHandle, &playerWidth, &playerHeight);

    // -------------------------------------------------------------------------
    // Enemyモデルの準備
    // -------------------------------------------------------------------------
    // EnemyもPlayerと同じ考え方で論理サイズと見た目を設定する
    enemyModelHandle = LoadModel2D("data/model/Enemy/Enemy.mv1");
    SetModelSize2D(enemyModelHandle, EnemyLogicalWidth, EnemyLogicalHeight);
    SetModelScale2D(enemyModelHandle, EnemyModelScale);
    SetModelOffset2D(enemyModelHandle, EnemyModelOffsetX, EnemyModelOffsetY);
    SetModelRotation2D(enemyModelHandle, 0.0f, 0.0f, 0.0f);

    // ダメージ時は2D画像を差し替えるのと同じ考え方で別モデルへ切り替える
    enemyDamageModelHandle = LoadModel2D("data/model/Enemy/Enemy_Damage.mv1");
    SetModelSize2D(enemyDamageModelHandle, EnemyLogicalWidth, EnemyLogicalHeight);
    SetModelScale2D(enemyDamageModelHandle, EnemyModelScale);
    SetModelOffset2D(enemyDamageModelHandle, EnemyModelOffsetX, EnemyModelOffsetY);
    SetModelRotation2D(enemyDamageModelHandle, 0.0f, 0.0f, 0.0f);

    GetModelSize2D(enemyModelHandle, &enemyWidth, &enemyHeight);

    // -------------------------------------------------------------------------
    // Shotモデルの準備
    // -------------------------------------------------------------------------
    // 同じモデルを複数の弾で使い回すため、モデル自体は1回だけ読み込む
    playerShotModelHandle = LoadModel2D("data/model/Shot/SpikyBall.mv1");
    SetModelSize2D(playerShotModelHandle, PlayerShotLogicalWidth, PlayerShotLogicalHeight);
    SetModelScale2D(playerShotModelHandle, PlayerShotModelScale);
    GetModelSize2D(playerShotModelHandle, &playerShotWidth, &playerShotHeight);

    // プレイヤー弾とは色違いの別モデルを使う想定
    enemyShotModelHandle = LoadModel2D("data/model/Shot/SpikyBall_Enemy.mv1");
    SetModelSize2D(enemyShotModelHandle, EnemyShotLogicalWidth, EnemyShotLogicalHeight);
    SetModelScale2D(enemyShotModelHandle, EnemyShotModelScale);
    GetModelSize2D(enemyShotModelHandle, &enemyShotWidth, &enemyShotHeight);

    // -------------------------------------------------------------------------
    // Backgroundの準備
    // -------------------------------------------------------------------------
    backgroundGraphHandle = LoadGraph("data/texture/FancyBG_back.png");

    // 幅は使用しないが、画像サイズを取得するために変数を用意する
    int backgroundWidth       = 0;
    int backgroundImageHeight = 0;
    // 背景を途切れなく並べ直すため、画像1枚分の高さを取得する
    GetGraphSize(backgroundGraphHandle, &backgroundWidth, &backgroundImageHeight);
    backgroundHeight = static_cast<float>(backgroundImageHeight);

    // -------------------------------------------------------------------------
    // UIの準備
    // -------------------------------------------------------------------------
    uiHpBarGraphHandle        = LoadGraph("data/texture/hp.png");
    uiHpBackgroundGraphHandle = LoadGraph("data/texture/hpBack.png");

    // -------------------------------------------------------------------------
    // ゲームデータの初期化
    // -------------------------------------------------------------------------
    // Playerは画面中央の下寄りから開始する
    playerX                   = ScreenWidth / 2.0f;
    playerY                   = ScreenHeight - PlayerStartBottomOffset;
    playerLife                = PlayerLife;
    playerIsDamaged           = false;
    playerDamageCounter       = 0;
    playerShotIntervalCounter = 0;

    // 座標は中心基準なので、幅の半分だけ右へ置くとEnemyの左端が画面端に揃う
    enemyX                   = enemyWidth / 2.0f;
    enemyY                   = EnemyStartTopOffset + enemyHeight / 2.0f;
    enemyLife                = EnemyLife;
    enemyIsDamaged           = false;
    enemyDamageCounter       = 0;
    enemyShotIntervalCounter = 0;
    enemyIsMovingRight       = true;

    for (int i = 0; i < PlayerShotCount; i++)
    {
        playerShotX[i]         = 0.0f;
        playerShotY[i]         = 0.0f;
        playerShotIsVisible[i] = false;
    }

    for (int i = 0; i < EnemyShotCount; i++)
    {
        enemyShotX[i]         = 0.0f;
        enemyShotY[i]         = 0.0f;
        enemyShotIsVisible[i] = false;
    }

    // 背景を上下に並べ、スクロールしても隙間が出ないようにする
    for (int i = 0; i < BackgroundCount; i++)
    {
        backgroundY[i] =
            ScreenHeight / 2.0f +
            (i - BackgroundCount / 2) * backgroundHeight;
    }

    ruleGameStartTime      = 0;
    ruleState              = GameState::Title;
    ruleWasSpaceKeyPressed = false;
    ruleIsSpaceKeyReleased = false;

    const long long oneFrameMicroseconds = 16667;
    long long lastScreenFlipTime         = GetNowHiPerformanceCount();

    // -------------------------------------------------------------------------
    // ゲームループ
    // -------------------------------------------------------------------------
    while (true)
    {
        ClearDrawScreen();

        // ---------------------------------------------------------------------
        // Playerの更新
        // ---------------------------------------------------------------------
        // ゲーム中だけPlayer / Enemy / Shot / Backgroundを更新する
        if (ruleState == GameState::Game)
        {
            if (CheckHitKey(KEY_INPUT_LEFT) == 1)
            {
                playerX -= PlayerSpeed;
            }

            if (CheckHitKey(KEY_INPUT_RIGHT) == 1)
            {
                playerX += PlayerSpeed;
            }

            if (CheckHitKey(KEY_INPUT_SPACE) == 1 && playerShotIntervalCounter == 0)
            {
                // 画面に出ていない弾を探し、その弾を再利用する
                for (int i = 0; i < PlayerShotCount; i++)
                {
                    if (!playerShotIsVisible[i])
                    {
                        // PlayerとShotはどちらも中心座標なので、そのまま発射位置に使える
                        playerShotX[i]         = playerX;
                        playerShotY[i]         = playerY;
                        playerShotIsVisible[i] = true;
                        break;
                    }
                }

                // 連続発射にならないよう、次に撃てるまでの待ち時間を入れる
                playerShotIntervalCounter = PlayerShotIntervalFrames;
            }

            if (playerShotIntervalCounter > 0)
            {
                --playerShotIntervalCounter;
            }

            // 中心座標から半分のサイズを引いた位置が画面端になるように補正する
            if (playerX < playerWidth / 2.0f)
            {
                playerX = playerWidth / 2.0f;
            }
            if (playerX > ScreenWidth - playerWidth / 2.0f)
            {
                playerX = ScreenWidth - playerWidth / 2.0f;
            }

            // ダメージ表示は一定フレーム経過したら元に戻す
            if (playerIsDamaged)
            {
                ++playerDamageCounter;

                if (playerDamageCounter >= DamageDisplayFrames)
                {
                    playerIsDamaged = false;
                }
            }

            // -----------------------------------------------------------------
            // Enemyの更新
            // -----------------------------------------------------------------
            if (enemyIsMovingRight)
            {
                enemyX += EnemySpeed;
            }
            else
            {
                enemyX -= EnemySpeed;
            }

            // 画面端に到達したら、はみ出さない位置へ戻して移動方向を反転する
            if (enemyX > ScreenWidth - enemyWidth / 2.0f)
            {
                enemyX             = ScreenWidth - enemyWidth / 2.0f;
                enemyIsMovingRight = false;
            }
            else if (enemyX < enemyWidth / 2.0f)
            {
                enemyX             = enemyWidth / 2.0f;
                enemyIsMovingRight = true;
            }

            if (enemyShotIntervalCounter == 0)
            {
                // Player Shotと同じく、使われていない弾を再利用する
                for (int i = 0; i < EnemyShotCount; i++)
                {
                    if (!enemyShotIsVisible[i])
                    {
                        enemyShotX[i]         = enemyX;
                        enemyShotY[i]         = enemyY;
                        enemyShotIsVisible[i] = true;
                        break;
                    }
                }

                enemyShotIntervalCounter = EnemyShotIntervalFrames;
            }

            if (enemyShotIntervalCounter > 0)
            {
                --enemyShotIntervalCounter;
            }

            // ダメージ表示は一定フレーム経過したら元に戻す
            if (enemyIsDamaged)
            {
                ++enemyDamageCounter;

                if (enemyDamageCounter >= DamageDisplayFrames)
                {
                    enemyIsDamaged = false;
                }
            }

            // -----------------------------------------------------------------
            // Player Shotの更新と当たり判定
            // -----------------------------------------------------------------
            for (int i = 0; i < PlayerShotCount; i++)
            {
                if (playerShotIsVisible[i])
                {
                    playerShotY[i] -= PlayerShotSpeed;

                    // 弾全体が画面外へ出たら、次に再利用できる状態へ戻す
                    if (playerShotY[i] < -playerShotHeight / 2.0f)
                    {
                        playerShotIsVisible[i] = false;
                    }

                    if (playerShotIsVisible[i] && enemyLife > 0)
                    {
                        // 中心座標から幅と高さの半分ずつ広げて当たり判定の端を求める
                        const float shotLeft   = playerShotX[i] - playerShotWidth / 2.0f;
                        const float shotRight  = playerShotX[i] + playerShotWidth / 2.0f;
                        const float shotTop    = playerShotY[i] - playerShotHeight / 2.0f;
                        const float shotBottom = playerShotY[i] + playerShotHeight / 2.0f;

                        const float enemyLeft   = enemyX - enemyWidth / 2.0f;
                        const float enemyRight  = enemyX + enemyWidth / 2.0f;
                        const float enemyTop    = enemyY - enemyHeight / 2.0f;
                        const float enemyBottom = enemyY + enemyHeight / 2.0f;

                        // 横方向と縦方向の両方で範囲が重なっていれば当たっている
                        const bool isHit =
                            shotLeft < enemyRight &&
                            shotRight > enemyLeft &&
                            shotTop < enemyBottom &&
                            shotBottom > enemyTop;

                        if (isHit)
                        {
                            playerShotIsVisible[i] = false;
                            enemyIsDamaged          = true;
                            enemyDamageCounter      = 0;
                            enemyLife -= PlayerShotPower;

                            // 発展:
                            // 配布モデルに含まれるDamageアニメーションを使う場合は
                            // System::Animationをゲーム側に用意して直接利用できる
                        }
                    }
                }
            }

            // -----------------------------------------------------------------
            // Enemy Shotの更新と当たり判定
            // -----------------------------------------------------------------
            for (int i = 0; i < EnemyShotCount; i++)
            {
                if (enemyShotIsVisible[i])
                {
                    enemyShotY[i] += EnemyShotSpeed;

                    // 弾全体が画面外へ出たら、次に再利用できる状態へ戻す
                    if (enemyShotY[i] > ScreenHeight + enemyShotHeight / 2.0f)
                    {
                        enemyShotIsVisible[i] = false;
                    }

                    if (enemyShotIsVisible[i] && playerLife > 0)
                    {
                        const float shotLeft   = enemyShotX[i] - enemyShotWidth / 2.0f;
                        const float shotRight  = enemyShotX[i] + enemyShotWidth / 2.0f;
                        const float shotTop    = enemyShotY[i] - enemyShotHeight / 2.0f;
                        const float shotBottom = enemyShotY[i] + enemyShotHeight / 2.0f;

                        const float playerLeft   = playerX - playerWidth / 2.0f;
                        const float playerRight  = playerX + playerWidth / 2.0f;
                        const float playerTop    = playerY - playerHeight / 2.0f;
                        const float playerBottom = playerY + playerHeight / 2.0f;

                        const bool isHit =
                            shotLeft < playerRight &&
                            shotRight > playerLeft &&
                            shotTop < playerBottom &&
                            shotBottom > playerTop;

                        if (isHit)
                        {
                            enemyShotIsVisible[i] = false;
                            playerIsDamaged        = true;
                            playerDamageCounter    = 0;
                            playerLife -= EnemyShotPower;
                        }
                    }
                }
            }

            // -----------------------------------------------------------------
            // Backgroundの更新
            // -----------------------------------------------------------------
            for (int i = 0; i < BackgroundCount; i++)
            {
                backgroundY[i] += BackgroundScrollSpeed;

                // 背景全体が画面下へ抜けたら、並びの一番上へ戻す
                if (backgroundY[i] - backgroundHeight / 2.0f > ScreenHeight)
                {
                    backgroundY[i] -= backgroundHeight * BackgroundCount;
                }
            }
        }

        // ---------------------------------------------------------------------
        // Ruleの更新
        // ---------------------------------------------------------------------
        const bool isSpaceKeyPressed = CheckHitKey(KEY_INPUT_SPACE) == 1;

        // 前フレームでは押されていて、今は押されていなければ「離した瞬間」
        ruleIsSpaceKeyReleased = ruleWasSpaceKeyPressed && !isSpaceKeyPressed;
        ruleWasSpaceKeyPressed = isSpaceKeyPressed;

        switch (ruleState)
        {
        case GameState::Title:
            if (ruleIsSpaceKeyReleased)
            {
                // 画面が一瞬で切り替わる違和感を減らすため、少しだけ待つ
                // 本来はフェードイン・フェードアウトなどの画面遷移演出を実装する方が望ましい
                WaitTimer(StateChangeWaitMilliseconds);

                ruleWasSpaceKeyPressed = false;
                ruleIsSpaceKeyReleased = false;
                ruleState              = GameState::Game;
                ruleGameStartTime      = GetNowCount();

                // タイトルからゲームを開始するたびに、ゲーム中の状態を最初へ戻す
                playerX                   = ScreenWidth / 2.0f;
                playerY                   = ScreenHeight - PlayerStartBottomOffset;
                playerLife                = PlayerLife;
                playerIsDamaged           = false;
                playerDamageCounter       = 0;
                playerShotIntervalCounter = 0;

                enemyX                   = enemyWidth / 2.0f;
                enemyY                   = EnemyStartTopOffset + enemyHeight / 2.0f;
                enemyLife                = EnemyLife;
                enemyIsDamaged           = false;
                enemyDamageCounter       = 0;
                enemyShotIntervalCounter = 0;
                enemyIsMovingRight       = true;

                for (int i = 0; i < PlayerShotCount; i++)
                {
                    playerShotX[i]         = 0.0f;
                    playerShotY[i]         = 0.0f;
                    playerShotIsVisible[i] = false;
                }

                for (int i = 0; i < EnemyShotCount; i++)
                {
                    enemyShotX[i]         = 0.0f;
                    enemyShotY[i]         = 0.0f;
                    enemyShotIsVisible[i] = false;
                }

                for (int i = 0; i < BackgroundCount; i++)
                {
                    backgroundY[i] =
                        ScreenHeight / 2.0f +
                        (i - BackgroundCount / 2) * backgroundHeight;
                }
            }
            break;

        case GameState::Game:
            if (enemyLife <= 0)
            {
                // 画面が一瞬で切り替わる違和感を減らすため、少しだけ待つ
                // 本来はフェードイン・フェードアウトなどの演出を入れる方が望ましい
                WaitTimer(StateChangeWaitMilliseconds);

                ruleWasSpaceKeyPressed = false;
                ruleIsSpaceKeyReleased = false;
                ruleState              = GameState::Clear;
            }
            else if (playerLife <= 0 || GetNowCount() - ruleGameStartTime > TimeLimitSeconds * 1000)
            {
                WaitTimer(StateChangeWaitMilliseconds);

                ruleWasSpaceKeyPressed = false;
                ruleIsSpaceKeyReleased = false;
                ruleState              = GameState::GameOver;
            }
            break;

        case GameState::Clear:
        case GameState::GameOver:
            if (ruleIsSpaceKeyReleased)
            {
                WaitTimer(StateChangeWaitMilliseconds);

                ruleWasSpaceKeyPressed = false;
                ruleIsSpaceKeyReleased = false;
                ruleState              = GameState::Title;
            }
            break;
        }

        // ---------------------------------------------------------------------
        // ゲーム画面の描画
        // ---------------------------------------------------------------------
        if (ruleState == GameState::Game)
        {
            // 奥にあるものから順に描く
            // Background
            for (int i = 0; i < BackgroundCount; i++)
            {
                DrawGraph3D(ScreenWidth / 2.0f, backgroundY[i], backgroundGraphHandle);
            }

            // Player Shot
            for (int i = 0; i < PlayerShotCount; i++)
            {
                if (playerShotIsVisible[i])
                {
                    DrawModel2D(playerShotX[i], playerShotY[i], playerShotModelHandle);
                }
            }

            // Enemy Shot
            for (int i = 0; i < EnemyShotCount; i++)
            {
                if (enemyShotIsVisible[i])
                {
                    DrawModel2D(enemyShotX[i], enemyShotY[i], enemyShotModelHandle);
                }
            }

            // Player
            if (!playerIsDamaged)
            {
                DrawModel2D(playerX, playerY, playerModelHandle);
            }

            // Enemy
            if (enemyLife > 0)
            {
                // ダメージ中だけ別モデルへ切り替える
                if (enemyIsDamaged)
                {
                    DrawModel2D(enemyX, enemyY, enemyDamageModelHandle);
                }
                else
                {
                    DrawModel2D(enemyX, enemyY, enemyModelHandle);
                }
            }
        }

        // ---------------------------------------------------------------------
        // UIの描画
        // ---------------------------------------------------------------------
        static const int titleTextOffsetY     = -100;
        static const int pressTextOffsetY     = 100;
        static const int timeTextOffsetX      = -100;
        static const int timeTextPositionY    = 25;
        static const int titleFontSize        = 64;
        static const int pressTextFontSize    = 24;
        static const int timeTextFontSize     = 24;
        static const int timeNumberFontSize   = 40;
        static const int enemyHpTextFontSize  = 24;
        static const int enemyHpTextPositionY = 2;
        static const int enemyHpTextPositionX = 10;
        static const int enemyHpLeftOffsetX   = 80;
        static const int enemyHpRightOffsetX  = 30;
        static const int enemyHpHeight        = 15;
        static const int playerHpOffsetY      = 5;

        char timeNumberText[256];
        char lifeText[256];

        // 中央揃えする文字は、文字幅の半分を画面中央から引いて描画位置を決める
        switch (ruleState)
        {
        case GameState::Title:
            SetFontSize(titleFontSize);
            DrawString(
                ScreenWidth / 2 -
                    GetDrawStringWidth(
                        TitleText,
                        static_cast<int>(std::strlen(TitleText))) / 2,
                ScreenHeight / 2 + titleTextOffsetY,
                TitleText,
                GetColor(255, 255, 255));

            SetFontSize(pressTextFontSize);
            DrawString(
                ScreenWidth / 2 -
                    GetDrawStringWidth(
                        StartButtonText,
                        static_cast<int>(std::strlen(StartButtonText))) / 2,
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

            // 開始からの経過時間を制限時間から引いて残り時間を求める
            const int remainingSeconds =
                TimeLimitSeconds - (GetNowCount() - ruleGameStartTime) / 1000;

            sprintf_s(timeNumberText, "%d", remainingSeconds);

            SetFontSize(timeNumberFontSize);
            DrawString(
                ScreenWidth / 2 -
                    GetDrawStringWidth(
                        timeNumberText,
                        static_cast<int>(std::strlen(timeNumberText))) / 2,
                timeTextPositionY,
                timeNumberText,
                GetColor(255, 0, 0));

            sprintf_s(lifeText, "HP:%d", playerLife);

            SetFontSize(timeTextFontSize);
            DrawString(
                static_cast<int>(playerX) -
                    GetDrawStringWidth(
                        lifeText,
                        static_cast<int>(std::strlen(lifeText))) / 2,
                static_cast<int>(playerY + playerHeight / 2.0f) + playerHpOffsetY,
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
                uiHpBackgroundGraphHandle,
                TRUE);

            // 現在HPの割合に合わせて、前景のHPバーだけ横幅を変える
            DrawExtendGraph(
                hpGraphStartX,
                enemyHpTextPositionY,
                hpGraphStartX + static_cast<int>(
                    (ScreenWidth - hpGraphStartX - enemyHpRightOffsetX) *
                    (static_cast<float>(enemyLife) / EnemyLife)),
                enemyHpTextPositionY + enemyHpHeight,
                uiHpBarGraphHandle,
                TRUE);
            break;
        }

        case GameState::Clear:
            SetFontSize(titleFontSize);
            DrawString(
                ScreenWidth / 2 -
                    GetDrawStringWidth(
                        GameClearText,
                        static_cast<int>(std::strlen(GameClearText))) / 2,
                ScreenHeight / 2 + titleTextOffsetY,
                GameClearText,
                GetColor(255, 255, 0));

            SetFontSize(pressTextFontSize);
            DrawString(
                ScreenWidth / 2 -
                    GetDrawStringWidth(
                        ReturnTitleText,
                        static_cast<int>(std::strlen(ReturnTitleText))) / 2,
                ScreenHeight / 2 + pressTextOffsetY,
                ReturnTitleText,
                GetColor(255, 255, 255));
            break;

        case GameState::GameOver:
            SetFontSize(titleFontSize);
            DrawString(
                ScreenWidth / 2 -
                    GetDrawStringWidth(
                        GameOverText,
                        static_cast<int>(std::strlen(GameOverText))) / 2,
                ScreenHeight / 2 + titleTextOffsetY,
                GameOverText,
                GetColor(255, 0, 0));

            SetFontSize(pressTextFontSize);
            DrawString(
                ScreenWidth / 2 -
                    GetDrawStringWidth(
                        ReturnTitleText,
                        static_cast<int>(std::strlen(ReturnTitleText))) / 2,
                ScreenHeight / 2 + pressTextOffsetY,
                ReturnTitleText,
                GetColor(255, 255, 255));
            break;
        }

        // ---------------------------------------------------------------------
        // 60FPSになるまで待って画面を更新する
        // ---------------------------------------------------------------------
        while (GetNowHiPerformanceCount() - lastScreenFlipTime < oneFrameMicroseconds)
        {
            // 処理なし
        }

        ScreenFlip();
        lastScreenFlipTime = GetNowHiPerformanceCount();

        // ---------------------------------------------------------------------
        // 終了判定
        // ---------------------------------------------------------------------
        if (ProcessMessage() < 0 || CheckHitKey(KEY_INPUT_ESCAPE) == 1)
        {
            break;
        }
    }

    DxLib_End();
    return 0;
}
