#include "PlayerShip.h"
#include <iostream>
#include <Windows.h>
#include <ctime>
using namespace std;

PlayerShip::PlayerShip()
{
	ms_fPosX		= 0.0f;
	ms_fPosY		= 0.0f;
}

PlayerShip::~PlayerShip()
{

}

void PlayerShip::Update(float dt)
{
	//update position of ship
	
	//read user input

	//check if able to fire laser

}

void PlayerShip::Draw()
{
	//Draw and render ou ship in this function
	// Clear the back buffer, call BeginScene()
	m_pD3DDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DXCOLOR(0.0f, 0.35f, 0.8f, 1.0f), 1.0f, 0);
	m_pD3DDevice->BeginScene();

	//////////////////////////////////////////////////////////////////////////
	// Matrix Transformations to control sprite position, scale, and rotate
	// Set these matrices for each object you want to render to the screen
	//////////////////////////////////////////////////////////////////////////
	D3DXMATRIX transMat, rotMat, scaleMat, worldMat, multRSMat;
	//setup the Matrix Identity (initiate the arrays)
	D3DXMatrixIdentity(&rotMat);
	D3DXMatrixIdentity(&transMat);
	D3DXMatrixIdentity(&scaleMat);
	D3DXMatrixIdentity(&worldMat);
	D3DXMatrixIdentity(&multRSMat);

	// Scale the sprite
	D3DXMatrixScaling(&scaleMat, 1.0f, 1.0f, 0.0f);
	// Rotation on Z axis, value in radians, converting from degrees from rot angle
	D3DXMatrixRotationZ(&rotMat, D3DXToRadian(0.0f));
	//D3DXMatrixIdentity(&rotMat);
	// Translation
	D3DXMatrixTranslation(&transMat, ms_fPosX, ms_fPosY, 0.0f);
	// Multiply scale and rotation, store in scale
	D3DXMatrixMultiply(&multRSMat, &rotMat, &scaleMat);
	// Multiply scale and translation, store in world
	D3DXMatrixMultiply(&worldMat, &multRSMat, &transMat);

	ms_pBoxMesh->DrawSubset(0);
	//EndScene
	m_pD3DDevice->EndScene();
	//Present Scene
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);

}

void PlayerShip::Init(HWND& hWnd, HINSTANCE& hInst, bool bWindowed)
{
	m_hWnd = hWnd; //copy the handler to the window in m_hWnd
	////////////////////
	//  RECT STRUCT	 //
	///////////////////
	// Find the width and height of window using hWnd and GetWindowRect()
	RECT rect;
	GetWindowRect(hWnd, &rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	//----------------------------------------------//
	//		Initiate the Direct 3D Components		//
	//----------------------------------------------//
	// Create the D3D Object
	m_pD3DObject = Direct3DCreate9(D3D_SDK_VERSION);
	// Create the Direct Input Object
	DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDIObject, NULL);
	
	///////////////////
	// Create Device //
	///////////////////
	HRESULT hr;
	hr = m_pDIObject->CreateDevice(GUID_SysKeyboard, &m_pDIKeyboard, NULL);
	if(FAILED(hr))
	{
		hr = m_pDIObject->CreateDevice(GUID_SysKeyboard, &m_pDIKeyboard, NULL);
	}
	hr = m_pDIObject->CreateDevice(GUID_SysMouse, &m_pDIMouse, NULL);
	if(FAILED(hr))
	{
		hr = m_pDIObject->CreateDevice(GUID_SysMouse, &m_pDIMouse, NULL);
	}
	//Set Data Format
	m_pDIKeyboard->SetDataFormat(&c_dfDIKeyboard);
	m_pDIMouse->SetDataFormat(&c_dfDIMouse2);
	//Set Cooperative Level 
	m_pDIKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	m_pDIMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	//Acquire the device 
	hr = m_pDIKeyboard->Acquire();
	if(FAILED(hr))
	{
		hr = m_pDIKeyboard->Acquire();
	}
	hr = m_pDIMouse->Acquire();
	if(FAILED(hr))
	{
		hr = m_pDIMouse->Acquire();
	}

	//////////////////////
	//   D3D Device PP  //
	//////////////////////
	// Set D3D Device presentation parameters before creating the device
	D3DPRESENT_PARAMETERS D3Dpp;
	ZeroMemory(&D3Dpp, sizeof(D3Dpp));  // NULL the structure's memory

	D3Dpp.hDeviceWindow					= hWnd;										// Handle to the focus window
	D3Dpp.Windowed						= bWindowed;								// Windowed or Full-screen boolean
	D3Dpp.AutoDepthStencilFormat		= D3DFMT_D24S8;								// Format of depth/stencil buffer, 24 bit depth, 8 bit stencil
	D3Dpp.EnableAutoDepthStencil		= TRUE;										// Enables Z-Buffer (Depth Buffer)
	D3Dpp.BackBufferCount				= 1;										// Change if need of > 1 is required at a later date
	D3Dpp.BackBufferFormat				= D3DFMT_X8R8G8B8;							// Back-buffer format, 8 bits for each pixel
	D3Dpp.BackBufferHeight				= height;									// Make sure resolution is supported, use adapter modes
	D3Dpp.BackBufferWidth				= width;									// (Same as above)
	D3Dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;					// Discard back-buffer, must stay discard to support multi-sample
	D3Dpp.PresentationInterval			= m_bVsync ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE; // Present back-buffer immediately, unless V-Sync is on								
	D3Dpp.Flags							= D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;		// This flag should improve performance, if not set to NULL.
	D3Dpp.FullScreen_RefreshRateInHz	= bWindowed ? 0 : D3DPRESENT_RATE_DEFAULT;	// Full-screen refresh rate, use adapter modes or default
	D3Dpp.MultiSampleQuality			= 0;										// MSAA currently off, check documentation for support.
	D3Dpp.MultiSampleType				= D3DMULTISAMPLE_NONE;						// MSAA currently off, check documentation for support.

	// Check device capabilities
	DWORD deviceBehaviorFlags = 0;
	m_pD3DObject->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_D3DCaps);

	// Determine vertex processing mode
	if(m_D3DCaps.DevCaps & D3DCREATE_HARDWARE_VERTEXPROCESSING)
	{
		// Hardware vertex processing supported? (Video Card)
		deviceBehaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;	
	}
	else
	{
		// If not, use software (CPU)
		deviceBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING; 
	}
	
	// If hardware vertex processing is on, check pure device support
	if(m_D3DCaps.DevCaps & D3DDEVCAPS_PUREDEVICE && deviceBehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING)
	{
		deviceBehaviorFlags |= D3DCREATE_PUREDEVICE;	
	}
	
	// Create the D3D Device with the present parameters and device flags above
	m_pD3DObject->CreateDevice(
		D3DADAPTER_DEFAULT,		// which adapter to use, set to primary
		D3DDEVTYPE_HAL,			// device type to use, set to hardware rasterization
		hWnd,					// handle to the focus window
		deviceBehaviorFlags,	// behavior flags
		&D3Dpp,					// presentation parameters
		&m_pD3DDevice);			// returned device pointer
	//set up the mesh here
	D3DXCreateBox(m_pD3DDevice, 2.0f, 1.0f, 1.0f, &ms_pBoxMesh, &ms_pAdjBuffer);
	/**************************************************************************************************/
	/////////////////////////
	// MATERIAL PROPERTIES //
	/////////////////////////
	//Zero the memory of each material object
	ZeroMemory(&m_pMaterial, sizeof(m_pMaterial));
	//set up the parameters
	m_pMaterial.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);		// Ambient color reflected
	m_pMaterial.Diffuse = D3DXCOLOR(1.0f, 0.3f, 0.3f, 1.0f);		// Diffuse color reflected
	m_pMaterial.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);		// Emissive color reflected
	m_pMaterial.Specular = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);		// Specular
	m_pMaterial.Power = 30.0f;	
	/*************************************************************************************************/
	//Set Texture
	D3DXCreateTextureFromFile
		(m_pD3DDevice, 
		L"test.tga", 
		&m_pTexture); 
	/************************************************************************************************/
	//Set Center
	
	//////////////////////////////////////////////////////////////////////////
	// EFFECT FILE
	//////////////////////////////////////////////////////////////////////////
	// Load effect from file (Lab5.fx)	
	/*#ifdef _DEBUG	// If Debug flag is set in compiler, use shader debug
	
		m_pEffectError = 0;
		D3DXCreateEffectFromFile(m_pD3DDevice, 
			L"Lab6.fx", 
			0, 
			0, 
			D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION, 
			0, 
			&m_pEffect, 
			&m_pEffectError);

	#else			// else do not use debug flag

		D3DXCreateEffectFromFile(m_pD3DDevice, L"Lab5.fx", 0, 0, 0, 0, &m_pEffect,
				&m_pEffectError);
	#endif

		// Specify technique by name, alternatively you could find the first valid tech
		m_hTech = m_pEffect->GetTechniqueByName("tech0");
		
	//////////////////////////////////////////////////////////////////////////
	// Set shader parameters that are the same for all objects
	//////////////////////////////////////////////////////////////////////////
	m_pEffect->SetFloatArray("eyePos", (float*)eyePos, 3);
	m_pEffect->SetFloatArray("lightPos", (float*)&m_D3DLight.Position, 3);
	m_pEffect->SetFloatArray("ambientLight", (float*)&m_D3DLight.Ambient, 3);
	m_pEffect->SetFloatArray("specularLight", (float*)&m_D3DLight.Specular, 3);
	m_pEffect->SetFloatArray("diffuseLight", (float*)&m_D3DLight.Diffuse, 3);
	m_pEffect->SetFloatArray("lightAttenuation", (float*)&D3DXVECTOR3(m_D3DLight.Attenuation0, m_D3DLight.Attenuation1, m_D3DLight.Attenuation2), 3);*/

	//set up variables like isAbleToFire to false, and isAlive to true;

}

void PlayerShip::FireLaser(float dt, bool canFire)
{
	//check timer

	//if time is <= FireInterval

	//fire laser

	//else cannot fire
	//bool is false
}

//////////////////////
// SET FUNCTIONS	//
/////////////////////
void PlayerShip::SetCenter(float x, float y)
{
	ms_fCenterX = x;
	ms_fCenterY = y;
}

void PlayerShip::SetPos(float x, float y)
{
	ms_fPosX = x;
	ms_fPosY = y;
}

void PlayerShip::SetFireDelay(float delay)
{
	ms_fFireDelay = delay;
}

void PlayerShip::SetFireInterval(float interv)
{
	ms_fFireInterval = interv;
}

void PlayerShip::SetWidth(float width)
{
	ms_fWidth = width;
}

void PlayerShip::SetHeight(float height)
{
	ms_fHeight = height;
}

void PlayerShip::SetAccelX(float xaccel)
{
	ms_fAccelX = xaccel;
}

void PlayerShip::SetIsAlive()
{
	if(ms_bIsAlive == true)
	{
		ms_bIsAlive = false;
	}
	if(ms_bIsAlive == false)
	{
		ms_bIsAlive = true;
	}
}

void PlayerShip::SetIsAbleToFire()
{
	if(ms_bIsAbleToFire == true)
	{
		ms_bIsAbleToFire = false;
	}
	if(ms_bIsAbleToFire== false)
	{
		ms_bIsAbleToFire= true;
	}
}

//Get members
float PlayerShip::GetCenterX()
{
	return ms_fCenterX;		
};
float PlayerShip::GetCenterY()
{
	return ms_fCenterY;
};
float PlayerShip::GetPosX()
{
	return ms_fPosX;
};
float PlayerShip::GetPosY()
{
		return ms_fPosY;
}
float PlayerShip::GetWidth()
{
	return ms_fWidth;
}

float PlayerShip::GetHeight()
{
	return ms_fHeight;
}

float PlayerShip::GetDepth()
{
	return ms_fDepth;
}
float PlayerShip::GetAccelX()
{
	return ms_fAccelX;
}
float PlayerShip::GetFireDelay()
{
	return ms_fFireDelay;
}
float PlayerShip::GetFireInterval()
{
	return ms_fFireInterval;
}

bool PlayerShip::GetIsAbleToFire()
{
	return ms_bIsAbleToFire;
}
bool PlayerShip::GetIsAlive()
{
	return ms_bIsAlive;
}

