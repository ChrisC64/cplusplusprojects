#ifndef MENU_H
#define MENU_H
#include <iostream>
#include "DirectXFramework.h"
#include "FModSound.h"
using namespace std;
class Menu : public DirectXFramework, public Sound
{
private:
	//Font object
	ID3DXFont*		mm_MenuFont;
	//Sprite Object
	ID3DXSprite*	mm_MenuSprite;
	//Texture object
	IDirect3DTexture9*	mm_pTitleTexture;		// Texture Object for player 1 sprite
	IDirect3DTexture9*	mm_pPlayTexture;
	//Used in navigating menu selection
	int				m_iMenuSelect;
	int				MIN_SELECT;
	int				MAX_SELECT;
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
	/////////////
	//Mesh
	////////////
	ID3DXMesh*			m_pBoxMesh;
	
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

public:
	///////////////////////////
	//Constructor/Destructors//
	///////////////////////////
	Menu();
	~Menu();
	////////////////////////////
	//Name: SetState
	//Return: void
	//Param: Void
	//Info: This will set the current state of our Menu system,
	//which will load the operation we want in our game. 
	////////////////////////////
	void SetState(int);

	////////////////////////////
	//Name: GetState
	//Return: int
	//Param: Void
	//Info: This will retrieve the
	//current state of the system class object;
	/////////////////////////////
	int GetState();
	
	///////////////////////////
	//Name: Draw
	//Return: void
	//Param: void
	//Info: This will be our Draw Method
	//for our menu system.
	///////////////////////////
	void Draw();

	///////////////////////////
	//Name: Update
	//Return: void
	//Param: void
	//Info: This will be our update for user controls
	////////////////////////////
	void Update();

	///////////////////////////
	//Name: Init
	//Return: void
	//Param: void
	//Info: This will be called first, it initializes and sets up our menu
	//before we draw it.
	///////////////////////////
	void Init(HWND& hWnd, HINSTANCE& hInst, bool bWindowed);
	///////////////////////////
	///////////////////////////
	//Object of System Class//
	//////////////////////////
	System  SysObj;
	DirectXFramework DirectX;
};
#endif MENU_H