#include "Skorp.h"
#include "Random.h"

Skorp::Skorp( const Vec2& pos,const TileMap& map,
	std::vector<std::unique_ptr<Bullet>>& bullets )
	:
	EnemyBase( pos,size,myHealth,map ),
	bullets( bullets ),
	walking( 0,0,size.x,size.y,4,*sprSheet,0.2f ),
	winding( 0,size.y,size.x,size.y,4,*sprSheet,0.2f ),
	attacking( 0,size.y * 2,size.x,size.y,4,*sprSheet,0.2f ),
	exploding( 0,size.y * 3,size.x,size.y,4,*sprSheet,0.2f )
{
	ResetTargeting( moveTolerance,speed );
}

void Skorp::Update( const EnemyUpdateInfo& info,float dt )
{
	// EnemyBase::Update( info,dt );

	switch( action )
	{
	case State::Walk:
	{
		const auto testMove = vel * dt;
		const auto validMove = coll.GetValidMove( pos,testMove );
		pos += validMove;
		coll.MoveTo( pos );

		walking.Update( dt );
		moveStateChange.Update( dt );

		if( moveStateChange.IsDone() || validMove.z )
		{
			walking.Reset();
			moveStateChange.ResetRng();
			action = State::Wander;
		}
	}
		break;
	case State::Wander:
	{
		EnemyBase::Wander( moveTolerance,speed,dt );

		walking.Update( dt );
		wanderDuration.Update( dt );
		if( wanderDuration.IsDone() )
		{
			walking.Reset();
			wanderDuration.ResetRng();
			action = State::WindUp;
		}
	}
		break;
	case State::WindUp:
		winding.Update( dt );
		if( winding.IsFinished() )
		{
			winding.Reset();
			action = State::Attack;
		}
		break;
	case State::Attack:
		attacking.Update( dt );
		if( attacking.IsFinished() )
		{
			attacking.Reset();
			vel = ( info.playerPos - GetCenter() ).GetNormalized() * speed;

			// Shoot shotgun out.
			const Vec2 initAngle = vel;
			const auto shotPos = GetCenter();

			static constexpr float angleDev = 3.1415f / 12.0f;
			for( int i = -2; i <= 2; ++i )
			{
				bullets.emplace_back( std::make_unique<Bullet>(
					shotPos,shotPos + initAngle
					.GetDeviated( float( i ) * angleDev ),
					*map,Bullet::Team::Skorp,bulletSpeed,
					Bullet::Size::Small ) );
			}

			action = State::Walk;
		}
		break;
	case State::Explode:
		if( !exploding.IsFinished() ) exploding.Update( dt );
		if( exploding.IsFinished() ) exploding.SetFrame( 3 );
		break;
	}
}

void Skorp::Draw( Graphics& gfx ) const
{
	switch( action )
	{
	case State::Walk:
	case State::Wander:
		if( !IsFlashing() ) walking.Draw( Vei2( pos ),gfx,vel.x < 0.0f );
		else walking.Draw( Vei2( pos ),gfx,FlashCol(),vel.x < 0.0f );
		break;
	case State::WindUp:
		if( !IsFlashing() ) winding.Draw( Vei2( pos ),gfx,vel.x < 0.0f );
		else winding.Draw( Vei2( pos ),gfx,FlashCol(),vel.x < 0.0f );
		break;
	case State::Attack:
		if( !IsFlashing() ) attacking.Draw( Vei2( pos ),gfx,vel.x < 0.0f );
		else attacking.Draw( Vei2( pos ),gfx,FlashCol(),vel.x < 0.0f );
		break;
	case State::Explode:
		exploding.Draw( Vei2( pos ),gfx,vel.x > 0.0f );
		break;
	}
}

void Skorp::Attack( int damage,const Vec2& loc )
{
	EnemyBase::Attack( damage,loc );

	if( IsExpl() )
	{
		action = State::Explode;
		coll.MoveTo( Vec2{ -9999.0f,-9999.0f } );
	}
}

Vec2 Skorp::GetCenter() const
{
	return( pos + Vec2( size ) / 2.0f );
}
