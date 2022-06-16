#pragma once
#include "olcPixelGameEngine.h"
#include <cmath>

class AnimationData
{
public:
	olc::vi2d sourcePos = { 0,0 };
	olc::vi2d sourceSize = { 0,0 };
	olc::vi2d location = { 0,0 };
};

class SpriteAnimation
{
private:
	float mInterval = 0.0f;
	int mTotalWidth = 0;
	int mTotalHeight = 0;
	int mTileCountX = 0;
	int mTileCountY = 0;
	int mTotalTileCount = 0;

	int tileSizeX = 0;
	int tileSizeY = 0;

	olc::vi2d tile_location = {0,0}; 

	float mCurrentTime = 0.0f;
	int mCurrentTile = 0;

public:
	void SetParams(float interval, int totalWith, int totalHeight, int tileCountX, int tileCountY, int totalTiles)
	{
		mInterval = interval;
		mTotalWidth = totalWith;
		mTotalHeight = totalHeight;
		mTileCountX = tileCountX;
		mTileCountY = tileCountY;
		mTotalTileCount = totalTiles;

		tileSizeX = (int)((float)mTotalWidth / (float)mTileCountX);
		tileSizeY = (int)((float)mTotalHeight / (float)mTileCountY);
	}

	AnimationData GetSpritesheetTile(int tileNum)
	{
		AnimationData data;
		data.sourceSize.x = tileSizeX;
		data.sourceSize.y = tileSizeY;
		data.sourcePos.x = (tileNum % mTileCountX) * data.sourceSize.x;
		data.sourcePos.y = floorf((float)tileNum / (float)mTileCountX) * data.sourceSize.y;
		data.location = tile_location;
		return data;
	}

	AnimationData GetInfo(float fElapsedTime)
	{
		AnimationData data;
		mCurrentTime += fElapsedTime;

		if (mCurrentTime >= mInterval)
		{
			mCurrentTime = 0.0f;
			mCurrentTile++;
		}

		if (mCurrentTile >= mTotalTileCount)
		{
			mCurrentTile = 0;
		}


		data.sourceSize.x = tileSizeX;
		data.sourceSize.y = tileSizeY;

		data.sourcePos.x = (mCurrentTile % mTileCountX) * data.sourceSize.x;
		data.sourcePos.y = floorf((float)mCurrentTile / (float)mTileCountX) * data.sourceSize.y;	
		return data;
	}
};