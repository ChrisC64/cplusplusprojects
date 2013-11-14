#ifndef PLAYINGFIELD_H
#define PLAYINGFIELD_H
#include <iostream>
#include <math.h>
#include "GameFramework.h"
#include "FModSound.h"
using namespace std;
//////////////////////////////////////
//Class: PlayinField
//Info: This class is our Playing Field
//Object that the player will play in.
//This object has a width and height, 
//and is only in 2D. The user cannot
//leave the space of this Playing Field.
//The user can only move left and right
//of the playing field, which is populated with 
//targets the user must shoot. An alien space ship
//may also visit from time to time, to try and shoot the
//user down from time to time. 
////////////////////////////////////////
class PlayingField : public GameFramework
{
private:
	float mpf_fWidth, mpf_fHeight;
	float mpf_fCenterX, mpf_fCenterY;
	ID3DXSprite*	mpf_pBackgroundSprite;
	IDirect3DTexture9* mpf_pBackgroundTexture;
	float MAX_X, MAX_Y;
	float MIN_X, MIN_Y;

	//////////////////
	// SHIP STRUCT //
	/////////////////
	struct PlayerShip
	{
		//vectors used for center of ship and direction
		D3DXVECTOR3 Center;
		D3DXVECTOR3 Position;
		D3DXVECTOR3 Direction;
		//floats
		float CenterX, CenterY;
		float PosX, PosY, PosZ;
		float FireDelay;
		float Width;
		float Height;
		float Depth;
		float AccelX;
		float FireInterval;
		//int
		int Health;
		//bools
		bool bIsAlive;
		bool bIsAbleToFire;
		
	};

	///////////////////
	// ALIEN STRUCT //
	//////////////////
	struct AlienShip
	{
		D3DXVECTOR3 Position;
		D3DXVECTOR3 Center;
		D3DXVECTOR3 Direction;
		float PosX, PosY, PosZ;
		float Radius;
		float AccelX;
		float FireInterval;

		bool bIsAbleToFire;
		bool bIsAlive;

		//DWORD m_AlienMatrls;
	};

	////////////////////////
	// BLOCK ENEMY STRUCT //
	////////////////////////
	struct Block
	{
		D3DXVECTOR3 Position;
		D3DXVECTOR3 Center;
		D3DXVECTOR3 Direction;
		float PosX, PosY, PosZ;
		float Width, Height, Depth;
		float AccelX, AccelY;
		int Health;
		bool bIsAlive;
	};

	//////////////////
	// LASER STRUCT //
	//////////////////
	struct Laser
	{
		D3DXVECTOR3 Position;
		D3DXVECTOR3 Center;
		D3DXVECTOR3 Direction;
		float PosX, PosY, PosZ;
		float Width, Height, Depth;
		float AccelY;

		bool bIsAlive;

	};
	//////////////////////
	// Bounding Objects //
	//////////////////////
	struct BoundingSphere
	{
		D3DXVECTOR3 pos;
		float radius;
	};

	struct BoundingBox
	{
		D3DXVECTOR3 MinPoint;
		D3DXVECTOR3 MaxPoint;
		D3DXVECTOR3 Center;
	};

	/////////////////////
	/// VERTEXT STRUCT //
	/////////////////////
	struct Vertex 
	{
		D3DXVECTOR3 position, normal;
		D3DXVECTOR2 uv;
	};
	/////////////////////////////
	// Windows Objects
	////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// Application Variables
	//////////////////////////////////////////////////////////////////////////
	HWND				m_hWnd;			// Handle to the window
	bool				m_bVsync;		// Boolean for vertical syncing
	////////////////////////////////
	// Direct 3D Objects
	////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// Direct3D Variables
	//////////////////////////////////////////////////////////////////////////
	IDirect3D9*			mm_pD3DObject;	// Direct3D 9 Object
	IDirect3DDevice9*	mm_pD3DDevice;	// Direct3D 9 Device
	D3DCAPS9			mm_D3DCaps;		// Device Capabilities
	ID3DXSprite*		mm_pD3DSprite;	// Sprite Object

	//////////////////
	//Camera info
	//////////////////
	//Eye, Look, and Up Vectors
	D3DXVECTOR3 eyePos;
	D3DXVECTOR3 lookAt;
	D3DXVECTOR3 upVec;
	//View and Projection matrix
	D3DXMATRIX  viewMat;
	D3DXMATRIX	projMat;

	////////////////////////////////////////
	// 3D Mesh Objects
	////////////////////////////////////////

	//Game Objects
	ID3DXMesh*			m_pShipStaticMesh;
	ID3DXMesh*			ms_pAnimatedMesh;
	ID3DXMesh*			m_pBlockMesh;
	ID3DXMesh*			m_pPlayingFieldMesh;
	ID3DXMesh*			m_pLaserMesh;

	//HitBox Meshes (For Testing)
	ID3DXMesh*			m_pShipBox;
	ID3DXMesh*			m_pAlienSphere;
	ID3DXMesh*			m_pBlockBox;
	ID3DXMesh*			m_pLaserBox;

	//Adjacency Buffer
	ID3DXBuffer*		ms_pAdjBuffer;

	//Texture and Material information
	D3DMATERIAL9		m_pPlayerMaterial;
	IDirect3DTexture9*	m_pPlayerTexture[1];
	IDirect3DTexture9*	m_pAlienTexture[1];
	IDirect3DTexture9*	m_pBlockTexture[1];
	IDirect3DTexture9*	m_pPFTexture[1];
	IDirect3DTexture9*	m_pLaserTexture[1];

	//Matrix for Materials and Textures
	D3DXMATERIAL*		d3dxMaterials;
	D3DMATERIAL9*		meshMaterials;
	LPDIRECT3DTEXTURE9* meshTextures;

	
	//////////////////////////
	// CLASS GAMEFRAMEWORK //
	/////////////////////////
	GameFramework GFObj;
	Sound GameSound;
	Menu MenuObj;
	//////////////////
	// Struct Names //
	//////////////////
	PlayerShip  PS;
	AlienShip AS;
	Block Bk;
	Laser Lsr;
	Laser BlockLsr;
	//Collision Struct declarations
	BoundingSphere BoundSphere;
	BoundingBox BoundBox;
	BoundingBox BlockBoundBox;
	BoundingBox LaserBoundBox;
	//Vertex buffers for verts of objects
	Vertex* sphereVerts;
	Vertex* boxVerts;
	Vertex* blockVerts;
	Vertex* laserVerts;

	//Matrix offsets for bounding boxes
	D3DXMATRIX	m_pPSBoundBoxOffset;
	D3DXMATRIX	m_pLaserBoundBoxOffset;
	D3DXMATRIX	m_pEnemyBoundBoxOffset;

	D3DXVECTOR3 center;

	///////////////////
	// D3DX MATRICES //
	///////////////////
	// Set object specific parameters
	D3DXMATRIX WIT;
	//Create Matrices
	D3DXMATRIX transMat, rotMat, scaleMat, worldMat, multRSMat;

	/////////////////////////
	// Collision Variables //
	/////////////////////////
	float term1, term2, term3;
	float distance;
	bool bIsCollided;

	//Random Number
	unsigned int RanNumGen;
	bool bRanUsed;

	//boolean for gameplay
	bool m_bIsGameOver;

public:
	///////////////////////////
	//Constructor/Destructors//
	///////////////////////////
	PlayingField();
	~PlayingField();
	/**********************************
	/Name: Init
	/Return: Void
	/Param: Void
	/Info: This is our initializer function
	/***********************************/
	void Init(HWND& hWnd, HINSTANCE& hInst, bool bWindowed);

	/**********************************
	/Name: Update
	/Return: Void
	/Param: Float
	/Info: This is our update function called every frame
	/***********************************/
	void Update(float);

	/**********************************
	/Name: Draw
	/Return: Void
	/Param: Void
	/Info: This is our Draw function called every frame
	/***********************************/
	void Draw();

	/**********************************
	/Name: SetWidth
	/Return: Void
	/Param: float
	/Info: This is our setup function for width variable
	/***********************************/
	void SetWidth(float);

	/**********************************
	/Name: SetHeight
	/Return: Void
	/Param: float
	/Info: This is our setup function for heightvariable
	/***********************************/
	void SetHeight(float);

	/**********************************
	/Name: SetCenter
	/Return: Void
	/Param: float
	/Info: This is our setup function for centerx/centery variable
	/***********************************/
	void SetCenter(float, float);

	/**********************************
	/Name: SetMaxWidth
	/Return: Void
	/Param: float
	/Info: This is our setup function for heightvariable
	/***********************************/
	void SetMaxWidth(float);

	/**********************************
	/Name: SetMaxHeight
	/Return: Void
	/Param: float
	/Info: This is our setup function for heightvariable
	/***********************************/
	void SetMaxHeight(float);

	/**********************************
	/Name: SetMinHeight
	/Return: Void
	/Param: float
	/Info: This is our setup function for heightvariable
	/***********************************/
	void SetMinHeight(float);

	/**********************************
	/Name: SetMinWidth
	/Return: Void
	/Param: float
	/Info: This is our setup function for heightvariable
	/***********************************/
	void SetMinWidth(float);

	/**********************************
	/Name: GetWidth
	/Return: float
	/Param: void
	/Info: This is our access function for Wdith
	/***********************************/
	float GetWidth();

	/**********************************
	/Name: GetHeight
	/Return: float
	/Param: void
	/Info: This is our access function for Height
	/***********************************/
	float GetHeight();

	/**********************************
	/Name: GetCenterX
	/Return: float
	/Param: void
	/Info: This is our access function for centerX
	/***********************************/
	float GetCenterX();

	/**********************************
	/Name: GetCenterY
	/Return: float
	/Param: void
	/Info: This is our access function for CenterY
	/***********************************/
	float GetCenterY();

	/**********************************
	/Name: GetMaxWidth
	/Return: float
	/Param: void
	/Info: This is our access function for CenterY
	/***********************************/
	float GetMaxWidth();

	/**********************************
	/Name: GetMinWidth
	/Return: float
	/Param: void
	/Info: This is our access function for CenterY
	/***********************************/
	float GetMinWidth();
	
	/**********************************
	/Name: GetMaxHeight
	/Return: float
	/Param: void
	/Info: This is our access function for CenterY
	/***********************************/
	float GetMaxHeight();

	/**********************************
	/Name: GetMinHeight
	/Return: float
	/Param: void
	/Info: This is our access function for CenterY
	/***********************************/
	float GetMinHeight();
	
	void ShootLaser(float, float, float);
	void EShootLaser(float, float, float);

	float calcDistance(float, float, float, float, float, float);

	float calcBoxCollision(float, float, float, float, float, float);

	float calcY(float, float);
	float calcX(float, float);
	float calcZ(float, float);

	bool ReturnToMenu;
	bool GetGameOver();
	bool GetMenuBool();
};
#endif PLAYINGFIELD_H