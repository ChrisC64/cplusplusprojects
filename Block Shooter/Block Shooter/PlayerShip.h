#ifndef PLAYERSHIP_H
#define PLAYERSHIP_H
#include <iostream>
#include "PlayingField.h"
using namespace std;
class PlayerShip : public PlayingField
{
private:
	//vectors used for center of ship and direction
	D3DXVECTOR3 Center;
	D3DXVECTOR3 Direction;
	//floats
	float ms_fCenterX, ms_fCenterY;
	float ms_fPosX, ms_fPosY;
	float ms_fFireDelay;
	float ms_fWidth;
	float ms_fHeight;
	float ms_fDepth;
	float ms_fAccelX;
	float ms_fFireInterval;
	//bools
	bool ms_bIsAlive;
	bool ms_bIsAbleToFire;

	////////////////////////////////////////
	// 3D Mesh Objects
	////////////////////////////////////////
	ID3DXMesh*			ms_pStaticMesh;
	ID3DXMesh*			ms_pAnimatedMesh;
	ID3DXMesh*			ms_pBoxMesh;
	//Adjacency Buffer
	ID3DXBuffer*				 ms_pAdjBuffer;
	//Texture and Material information
	D3DMATERIAL9		m_pMaterial;
	IDirect3DTexture9*	m_pTexture;
	//////////////////////////////////////////////////////////////////////////
	// Application Variables
	//////////////////////////////////////////////////////////////////////////
	HWND				m_hWnd;			// Handle to the window
	bool				m_bVsync;		// Boolean for vertical syncing
public:
	///////////////////////////
	//Constructor/Destructors//
	///////////////////////////
	PlayerShip();
	~PlayerShip();
	//Set Members
	//x and y positions set
	void SetCenter(float, float);
	void SetPos(float, float);

	void SetFireDelay(float);

	void SetWidth(float);
	void SetHeight(float);
	void SetDepth(float);

	void SetAccelX(float);

	void SetFireInterval(float);

	void SetIsAbleToFire();
	void SetIsAlive();

	//Get members
	float GetCenterX();
	float GetCenterY();
	float GetPosX();
	float GetPosY();
	float GetWidth();
	float GetHeight();
	float GetDepth();
	float GetAccelX();
	float GetFireDelay();
	float GetFireInterval();

	bool GetIsAbleToFire();
	bool GetIsAlive();

	//Draw Function
	void Draw();
	//Update Ship's position, direction, user input, and other variables.
	void Update(float);
	//init the ship
	void Init(HWND& hWnd, HINSTANCE& hInst, bool bWindowed);
	//Respawn the ship
	void Respawn();
	//Fire laser
	void FireLaser(float, bool);


};
#endif PLAYERSHIP_H