#pragma once

#include <vector>
#include <string>
#include "Graphics.h"
#include <memory>

class DataPoint
{
public:
	DataPoint() = delete;
	DataPoint( const Vei2& pos,char type )
		:
		pos( pos ),
		type( type )
	{}
public:
	Vei2 pos;
	char type;
};

class TileMap
{
public:
	enum class TileType
	{
		Empty = int( '0' ),
		Wall = int( '1' )
	};
public:
	TileMap() = default;

	void Draw( Graphics& gfx ) const;
	void DrawTileAt( int x,int y,Color c,Graphics& gfx ) const;

	void SetTile( int x,int y,TileType type );
	void LoadFile( const std::string& fileName );
	std::vector<Vec2> FindAllInstances( char searchTerm,
		const std::string& fileName );

	TileType GetTile( int x,int y ) const;
	Vei2 GetTilePos( const Vec2& screenPos ) const;
	TileType GetTileAt( const Vec2& screenPos ) const;
	std::vector<DataPoint> FindSpecialTerms( const std::string& fileName ) const;
private:
	int width; // Number of tiles left-right.
	int height; // Number of tiles up-down.
	Vei2 tileDim; // Size of each tile on screen.
	std::vector<TileType> tiles;
	const Surface wallSpr = Surface{ { "Images/Wall.bmp" },Vei2{ 4,4 } };
	const Surface wallTopSpr = Surface{ { "Images/WallTop.bmp" },Vei2{ 4,4 } };
};