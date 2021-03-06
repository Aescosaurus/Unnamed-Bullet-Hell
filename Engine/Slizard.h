#pragma once

#include "EnemyBase.h"
#include "Bullet.h"
#include "Timer.h"

class Slizard
	:
	public EnemyBase
{
private:
	enum class State
	{
		Wander,
		Charge,
		Phase1Attack,
		Phase2Attack,
		Explode
	};
public:
	Slizard( const Vec2& pos,const TileMap& map,
		std::vector<std::unique_ptr<Bullet>>& bullets );

	void Update( const EnemyUpdateInfo& info,float dt ) override;
	void Draw( Graphics& gfx ) const override;

	void Attack( int damage,const Vec2& loc ) override;
private:
	void ShootBullet( float angle );

	// Returns true if there's a wall between start and end.
	bool CheckLineOfSight( const Vec2& start,const Vec2& end ) const;
	Vec2 GetCenter() const;
private:
	static constexpr Vei2 size = { 64,64 };
	static constexpr int health = 9;
	std::vector<std::unique_ptr<Bullet>>* pBulletVec;
	CSurfPtr surfSheet = SurfCodex::Fetch( "Images/SlizardAnim.bmp" );
	Anim walking;
	Anim charging;
	Anim attacking;
	Anim exploding;
	State action = State::Wander;
	static constexpr float moveTolerance = 225.2f;
	static constexpr float speed = 190.1f;
	Vec2 shotTarget = { 0.0f,0.0f };
	// Be careful changing these cuz these have to
	//  add up to 0.8(attacking anim duration).
	//  ( phase1ShotsPerSide * phase1Refire +
	//    phase2ShotsPerSide * phase2Refire ) = 0.8
	static constexpr int nPhase1Shots = 2;
	Timer phase1Refire = 0.2f;
	int curPhase1Shots = 0;
	static constexpr int nPhase2Shots = 5;
	Timer phase2Refire = 0.08f;
	int curPhase2Shots = 0;
	static constexpr float bulletSpeed = 215.5f;
};