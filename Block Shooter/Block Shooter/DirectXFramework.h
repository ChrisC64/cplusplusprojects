#ifndef DIRECTXFRAMEWORK_H
#define DIRECTXFRAMEWORK_H
//#pragma once
//#pragma comment(lib, "winmm.lib")
//#pragma comment(lib, "dinput8.lib")
//#pragma comment(lib, "dxguid.lib")
//#pragma comment(lib, "strmiids.lib")
//////////////////////////////////////////////////////////////////////////
// Direct3D 9 headers and libraries required
//////////////////////////////////////////////////////////////////////////
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include <stdio.h>
#include <Windows.h>
#include <DShow.h>
#include <ctime>
#include "System.h"
//#pragma comment(lib, "d3d9.lib")
//#pragma comment(lib, "d3dx9.lib")

// Macro to release COM objects fast and safely
#define SAFE_RELEASE(x) if(x){x->Release(); x = 0;}

class DirectXFramework : public System
{
protected:
	//////////////////////////////////////////////////////////////////////////
	// Application Variables
	//////////////////////////////////////////////////////////////////////////
	HWND				m_hWnd;			// Handle to the window
	bool				m_bVsync;		// Boolean for vertical syncing
	
	//////////////////////////////////////////////////////////////////////////
	// Direct Input Variables
	/////////////////////////////////////////////////////////////////////////
	IDirectInput8*		m_pDIObject;	// Direct Input Object
	IDirectInputDevice8* m_pDIKeyboard; // Direct Input Keyboard
	IDirectInputDevice8* m_pDIMouse;	// Direct Input Mouse
	DIMOUSESTATE		m_pMouseState;	// Mouse State
	char				m_pKeyboardState[256];//Buffer for keyboard state
	bool				m_bKeyDown[256];// Boolean to check if key is down; true = down, false = up
	//////////////////////////////////////////////////////////////////////////
	// Font Variables
	//////////////////////////////////////////////////////////////////////////
	ID3DXFont*			m_pD3DFont;		// Font Object
	//////////////////////////////////////////////////////////////////////////
	// Sprite Variables
	//////////////////////////////////////////////////////////////////////////
	ID3DXSprite*		m_pD3DSprite;	// Sprite Object

	/*****************************************************************************/
	//Textures for Objects in Game
	//IDirect3DTexture9*	m_pBarP1Texture;		// Texture Object for player 1 sprite

	/****************************************************************************/
	D3DXIMAGE_INFO		m_imageInfo;	// File details of a texture
	float posX, posY; //used for updating the position of our object in x and y coordinates (2D game)


	////////////////////////////////////////////////////////////////////////
	// Graphic stat variables
	////////////////////////////////////////////////////////////////////////
	float mFPS;
	float mMilliSecPerFrame;
	////////////////////////////////////////////////////////////////////////
	//	Direct Show Objects
	////////////////////////////////////////////////////////////////////////
	IGraphBuilder*		m_pGraphBuilder;	// Graph builder COM object
	IMediaControl*		m_pMediaControl;	// Media Control COM Object
	IMediaEvent*		m_pMediaEvent;		// Media Event COM Object
	IVideoWindow*		m_pVideoWindow;		// Video Window COM Object
	bool				m_bVideoOver;		// Stop operations until video has finished playing
	//////////////////////////////////////////////////////////////////////////
	// Direct3D Variables
	//////////////////////////////////////////////////////////////////////////
	IDirect3D9*			m_pD3DObject;	// Direct3D 9 Object
	IDirect3DDevice9*	m_pD3DDevice;	// Direct3D 9 Device
	D3DCAPS9			m_D3DCaps;		// Device Capabilities
	//////////////////////////////////////////////////////////////////////////
	// Effects
	//////////////////////////////////////////////////////////////////////////
	ID3DXEffect*		m_pEffect;
	ID3DXBuffer*		m_pEffectError;
	D3DXHANDLE			m_hTech;
	/////////////////////////////////////////////////////////////////////////
	// 3D Mesh Objects
	/////////////////////////////////////////////////////////////////////////
	ID3DXMesh*			m_pBoxMesh;

	///////////////
	// Sys Obj
	///////////////
	System SysObj;

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

	//////////////////////////////////////////////////////////////////////////
	// Lighting, Material, Textures
	//////////////////////////////////////////////////////////////////////////
	D3DLIGHT9			m_D3DLight;
	D3DMATERIAL9		m_pMaterial;
	IDirect3DTexture9*	m_pTexture;

	///////////////////////
	// .X File Variables //
	///////////////////////
	//Adjacency Buffer
	ID3DXBuffer*				 m_pAdjBuffer;

	//Materials Loader
	ID3DXBuffer*		m_pXMaterials;
	ID3DXBuffer*		m_pXEffect;

	//Double word for our Materials
	DWORD				m_numMaterials;

	//Matrix for Materials and Textures
	D3DXMATERIAL*		d3dxMaterials;
	D3DMATERIAL9*		meshMaterials;
	LPDIRECT3DTEXTURE9* meshTextures;
	
public:
	
	//////////////////
	// Constructors
	/////////////////
	DirectXFramework();
	~DirectXFramework();

	/**********************************************************
	* Function: Init()
	* Param.: HWND hWnd - Handle to the window application
	*		  HINSTANCE hInst - Handle to the application instance
	*		  bool Windowed - boolean for window or fullscreen
	* Return: void
	* Description: This will be our initializer for the Direct X 
	* components. We run this function once to initialize all components.
	***************************************************************/
	void Init(HWND& hWnd, HINSTANCE& hInst, bool bWindowed);

	/****************************************************************
	* Function: Update(float)
	* Param.: Float dt - Delta time float
	* Return: Void
	* Description: This is our update function that we call every frame
	* to enable the update of position of objects on the screen, drawing, and
	* variables, and processing input commands. 
	*******************************************************************/
	void Update(float dt);

	/******************************************************************
	* Function: Render()
	* Param: void
	* Return: void 
	* Description: This runs at every frame to draw our scene. 
	*******************************************************************/
	void Render();
	//void RenderIntro();
	//void RenderGame(); //Render and play game
	//void RenderMenu(); // Render Menu
	//void RenderCredits();// Render Credits
	/******************************************************************
	* Function: Shutdown();
	* Param: Void
	* Return: Void
	* Description: Releases our COM objects and deallocate memory
	* when we quit the game. 
	******************************************************************/
	void Shutdown();

	bool GetMovieState();
};
#endif DIRECTXFRAMEWORK_H