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
	modelHandles.emplace_back(MV1DuplicateModel(sourceModelHandle));

	// 物理挙動の初期化
	auto lineColliderData = dynamic_cast<YuiLib::ColliderDataOneWayLine2D*>(colliderData);
	rigidbody.Initialize();
	rigidbody.SetPos(lineColliderData->startPoint);

	// 位置決定
	for (const auto& item : modelHandles)
	{
		const auto& startPos = rigidbody.GetPos();
		auto pos = VGet(startPos.x, startPos.y, startPos.z);
		MV1SetPosition(item, pos);
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

