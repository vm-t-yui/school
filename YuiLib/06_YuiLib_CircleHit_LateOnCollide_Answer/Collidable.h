// 2024 Takeru Yui All Rights Reserved.
#pragma once

namespace YuiLib {

class Rigidbody;
class Physics;

/// <summary>
/// 衝突できるもの。物理・衝突判定をする場合はこれを継承させる
/// </summary>
class Collidable abstract
{
public:
	// 当たったものの判別を行うためのタグ
	// FIXME: ゲームによって変わる定数群をライブラリ側に持たせるのは良くない。string等にしてゲーム側で定数を。
	enum class Tag
	{
		Player,		// プレイヤー
		Enemy		// 敵
	};

	Collidable(Tag tag);				// コンストラクタ
	virtual void OnCollide(const Collidable& colider) abstract;	// 衝突したとき

	Tag GetTag() const { return tag; }	// タグ情報

protected:
	Rigidbody	rigidbody;

	// FIXME: これは円の当たり判定限定
	float		radius;

private:
	Tag			tag;

	// PhysicsがCollidableを自由に管理するためにフレンド化
	friend Physics;

	// 以下、フレンドであるPhisicsのみが扱う型や変数
private:
	// FIXME: これはPhysicsが持つ方が望ましい
	VECTOR		nextPos;
};

}

