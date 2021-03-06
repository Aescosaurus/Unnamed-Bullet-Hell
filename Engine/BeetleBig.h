#pragma once

#include "EnemyBase.h"
#include "Bullet.h"
#include "Timer.h"
#include "Beetle.h"

class BeetleBig
	:
	public EnemyBase
{
private:
	enum class State
	{
		Moving,
		WindingUp,
		WindingDown,
		Exploding
	};
public:
	BeetleBig( const Vec2& pos,const TileMap& map,
		std::vector<std::unique_ptr<Bullet>>& bulletVec,
		std::vector<std::unique_ptr<EnemyBase>>& enemies );

	void Update( const EnemyUpdateInfo& info,float dt ) override;
	void Draw( Graphics& gfx ) const override;

	void Attack( int damage,const Vec2& loc ) override;
private:
	void Retarget( const Vec2& theTarget );
	void SpawnBulletCircle();

	Vec2 GetCenter() const;
private:
	static constexpr Vei2 size = { 64,64 };
	static constexpr int myHP = 16;
	std::vector<std::unique_ptr<Bullet>>* pBulletVec;
	std::vector<std::unique_ptr<EnemyBase>>* pEnemyVec;
	CSurfPtr sprSheet = SurfCodex::Fetch( "Images/BeetleBigAnim.bmp" );
	Anim walking;
	Anim jumping;
	Anim landing;
	Anim explode;
	State curState = State::Moving;
	Vec2 vel = { 0.0f,0.0f };
	Timer moveReset = { 3.54f,true };
	Timer retarget = { 0.7f,true };
	static constexpr float speed = 79.24f;
	static constexpr float bullSpeed = 186.2f;
	bool deadDead = false;
	static constexpr int nMinis = 6;
};