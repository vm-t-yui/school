// 2024 Takeru Yui All Rights Reserved.
#include "DxLib.h"
#include <string>
#include "Screen.h"
#include "YuiLib.h"
#include "StepGround.h"

/// <summary>
/// コンストラクタ
/// </summary>
StepGround::StepGround(const VECTOR& start, const VECTOR& end)
	: Collidable		(Collidable::Priority::Static, GameObjectTag::StepGround, YuiLib::ColliderData::Kind::OneWayLine2D)
	 ,sourceModelHandle (-1)
{
	auto lineColliderData = dynamic_cast<YuiLib::ColliderDataOneWayLine2D*>(colliderData);

	// 地面の位置を作る
	lineColliderData->startPoint = start;
	lineColliderData->endPoint = end;
	lineColliderData->isStartToEndUp = true;
}

/// <summary>
/// 初期化
/// </summary>
void StepGround::Initialize(YuiLib::Physics* physics)
{
	Collidable::Initialize(physics);

	// モデルロード
	sourceModelHandle = MV1LoadModel("data/model/tileSmall_forest.mv1");

	auto lineColliderData = dynamic_cast<YuiLib::ColliderDataOneWayLine2D*>(colliderData);

	// ブロックの大きさは1.0fで、pivot(中心点)は左右が中央、上下が下
	// 始点→終点の長さ / blockSizeの数モデルを準備する
	float blockSize = 1.0f;
	int modelNum = static_cast<int>(VSize(VSub(lineColliderData->endPoint, lineColliderData->startPoint)) / blockSize);
	for (int i = 0; i < modelNum; i++)
	{
		modelHandles.emplace_back(MV1DuplicateModel(sourceModelHandle));
	}

	// 物理挙動の初期化
	rigidbody.Initialize();
	rigidbody.SetPos(lineColliderData->startPoint);

	// 位置決定
	int indexCount = 0;
	for (const auto& item : modelHandles)
	{
		// ブロックの大きさは1.0fで、pivot(中心点)は左右が中央、上下が下
		// ラインの始点をブロック左上に持ってくるには、(+0.5f、-1.0f)で調整が必要
		// ついでにZ位置も手前にせり出しててイケてないので、-0.4fくらいにして+0.1fブロックが手前にせり出ているようにする）
		const auto& startPos = rigidbody.GetPos();
		auto pos = VGet(startPos.x + (blockSize * 0.5f) + (blockSize * indexCount), startPos.y - blockSize, startPos.z + (blockSize * 0.4f));
		//auto pos = VGet(startPos.x + blockSize * indexCount, startPos.y, startPos.z);
		MV1SetPosition(item, pos);
		++indexCount;
	}
}

/// <summary>
/// 終了
/// </summary>
void StepGround::Finalize(YuiLib::Physics* physics)
{
	// モデルアンロード
	for (const auto& item : modelHandles)
	{
		MV1DeleteModel(item);
	}
	MV1DeleteModel(sourceModelHandle);
	Collidable::Finalize(physics);
}

/// <summary>
/// 更新
/// </summary>
void StepGround::Update()
{
	// どこにも移動しない
	rigidbody.SetVelocity(VGet(0, 0, 0));

	// 動かないので物理的な位置は無視。仮で始点にする
	auto lineColliderData = dynamic_cast<YuiLib::ColliderDataLine2D*>(colliderData);
	rigidbody.SetPos(lineColliderData->startPoint);
}

/// <summary>
/// 更新
/// </summary>
void StepGround::Draw()
{
	for (const auto& item : modelHandles)
	{
		MV1DrawModel(item);
	}
}

/// <summary>
/// 衝突したとき
/// </summary>
void StepGround::OnCollide(const Collidable& colider)
{
	std::string message = "足場が";
	if (colider.GetTag() == GameObjectTag::Player)
	{
		message += "プレイヤー";
	}
	else if (colider.GetTag() == GameObjectTag::Enemy)
	{
		message += "敵";
	}
	else
	{
		// 壁同士は当たらないので無視
	}
	message += "と当たった！\n";
	printfDx(message.c_str());
}

