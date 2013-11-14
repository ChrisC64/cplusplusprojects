#include "Menu.h"
using namespace std;

Menu::Menu()
{
	m_iMenuSelect		= 0;
	MIN_SELECT		= 0;
	MAX_SELECT		= 3;
}
Menu::~Menu()
{
	
}
//Use operation to set State of Game Applicaation based upon user input
void Menu::SetState(int choice)
{
	//load the game
	m_iMenuSelect = choice;
}
//return the state back to system class
int Menu::GetState()
{
	return m_iMenuSelect;
}
//update function, used to run our menu when it is called
void Menu::Update()
{
	HRESULT hr;
	//////////////////////////
	//	Update Device State //
	//////////////////////////
	hr = m_pDIKeyboard->GetDeviceState(sizeof(m_pKeyboardState),(void**)&m_pKeyboardState);
	if(FAILED(hr))
	{
		hr = m_pDIKeyboard->GetDeviceState(sizeof(m_pKeyboardState),(void**)&m_pKeyboardState);
	}
	hr = m_pDIMouse->GetDeviceState(sizeof(DIMOUSESTATE2), (void**) &m_pMouseState);
	if(FAILED(hr))
	{
		//ZeroMemory(&m_pMouseState, sizeof(m_pMouseState));
		hr = m_pDIMouse->Acquire();
	}
	if(m_iMenuSelect == 1)
	{
		if(m_pKeyboardState[DIK_P] & 0x80)
		{
			m_iMenuSelect = 3;
		}
		if(m_pKeyboardState[DIK_C] & 0x80)
		{
			m_iMenuSelect = 4;
		}
		if(m_pKeyboardState[DIK_Q] & 0x80)
		{
			m_iMenuSelect = 5;
		}
		if(m_pKeyboardState[DIK_O] & 0x80)
		{
			m_iMenuSelect = 2;
		}
	}
	if(m_iMenuSelect == 2 || m_iMenuSelect == 4)
	{
		if(m_pKeyboardState[DIK_M] & 0x80)
		{
			m_iMenuSelect = 1;
		}
	}
	if(m_iMenuSelect == 2)
	{
		if(m_pKeyboardState[DIK_F] & 0x80)
		{
			//set fullscreen mode
		}
	}

}
//initialize objects of this class; sprites, text, and any other variables
void Menu::Init(HWND& hWnd, HINSTANCE& hInst, bool bWindowed)
{
	m_hWnd = hWnd; //copy the handler to the window in m_hWnd
	HRESULT hr;
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

	///////////////////
	// Create Device //
	///////////////////
	
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
	//*************************************************************************
	//////////////////////////////////////////////////////////////////////////
	// D3DXVECTOR3: eyePos, lookAt, upVec
	//////////////////////////////////////////////////////////////////////////
	//Declare values for your vectors
	eyePos = D3DXVECTOR3(0.0f, 5.0f, -10.0f);
	lookAt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	upVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	// define and init the vectors
	D3DXMatrixLookAtLH(
		&viewMat, 
		&eyePos, 
		&lookAt, 
		&upVec);
	m_pD3DDevice->SetTransform(D3DTS_VIEW, &viewMat);
	//Se up the Perspective FOV
	D3DXMatrixPerspectiveFovLH(
		&projMat, 
		D3DXToRadian(65.0f), 
		(float)width / (float)height, 
		1.0f, 
		2000.0f);
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &projMat);

	//*************************************************************************
	//////////////////////////////////////////////////////////////////////////
	// Create Sprite Object and Textures
	//////////////////////////////////////////////////////////////////////////
	/**********************CREATE SPRITE OBJECT*********************************/
	D3DXCreateSprite(m_pD3DDevice, &m_pD3DSprite);
	//Texture for Title Screen
	hr = D3DXCreateTextureFromFileEx(m_pD3DDevice, 
		L"Content\\Texture\\Menu_Title.png", 
		0, 0, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_XRGB(255, 255, 255), &m_imageInfo, 0, &mm_pTitleTexture);
	hr = D3DXCreateTextureFromFileEx(m_pD3DDevice, 
		L"Content\\Texture\\Menu_Game.png", 
		0, 0, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_XRGB(255, 255, 255), &m_imageInfo, 0, &mm_pPlayTexture);

	/////////////////
	//Create Box
	////////////////
	//D3DXCreateBox(m_pD3DDevice, 1.0f, 1.0f, 1.0f, &m_pBoxMesh, &m_pAdjBuffer);
	//Fone
	D3DXCreateFont(m_pD3DDevice, 30, 0, FW_BOLD, 0, false,
                      DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
                      DEFAULT_PITCH | FF_DONTCARE, TEXT("Times New Roman"),
                      &mm_MenuFont);

}
//draw the menu to the screen
void Menu::Draw()
{
		
		// If the device was not created successfully, return
		//if(!mm_pD3DDevice)
		//{return;}
	
	//Get Window's size and state
	RECT rect; 
	GetWindowRect(m_hWnd, &rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	// Clear the back buffer, call BeginScene()
	m_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 0, 0, 0), 1.0f, 0 );
	m_pD3DDevice->BeginScene();
	//////////////////////////////////////////////////////////////////////////
	// Draw 3D Objects (for future labs - not used in Week #1)
	//////////////////////////////////////////////////////////////////////////
	if(m_iMenuSelect == 1)
	{
		//////////////////////////////////////////////////////////////////////////
		// Draw 2D sprites
		//////////////////////////////////////////////////////////////////////////
		// Call Sprite's Begin to start rendering 2D sprite objects
		m_pD3DSprite->Begin(D3DXSPRITE_ALPHABLEND);
		//////////////////////////////////////////////////////////////////////////
		// Matrix Transformations to control sprite position, scale, and rotate
		// Set these matrices for each object you want to render to the screen
		//////////////////////////////////////////////////////////////////////////
		D3DXMATRIX transMat, rotMat, scaleMat, worldMat, mSCMat;
		//setup the Matrix Identity (initiate the arrays)
		D3DXMatrixIdentity(&rotMat);
		D3DXMatrixIdentity(&transMat);
		D3DXMatrixIdentity(&scaleMat);
		D3DXMatrixIdentity(&worldMat);
		D3DXMatrixIdentity(&mSCMat);
		// Scale the sprite
		D3DXMatrixScaling(&scaleMat, 1.0f, 1.0f, 0.0f);
		// Rotation on Z axis, value in radians, converting from degrees from rot angle
		D3DXMatrixRotationZ(&rotMat, D3DXToRadian(0.0f));
		//D3DXMatrixIdentity(&rotMat);
		// Translation
		D3DXMatrixTranslation(&transMat, width * 0.25, height * 0.025, 0.0f);
		// Multiply scale and rotation, store in scale
		D3DXMatrixMultiply(&mSCMat, &rotMat, &scaleMat);
		// Multiply scale and translation, store in world
		D3DXMatrixMultiply(&worldMat, &mSCMat, &transMat);
	
	
		// Set Transform
		m_pD3DSprite->SetTransform(&worldMat);

		// Draw the texture with the sprite object:: Draw(pSrcTexture, pSrcRect, pCenter, pTranslation, Color)
		m_pD3DSprite->Draw(mm_pTitleTexture, 0, &D3DXVECTOR3(m_imageInfo.Width * 0.5, m_imageInfo.Height * 0.5, 0.0f), 
			&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
										D3DCOLOR_ARGB(255, 255, 255, 255));

	
		// End drawing 2D sprite
		m_pD3DSprite->End();
	}
	///////////////////////////////////////////////
	//		DISPLAY GRAPHIC STAT TEXT			//
	//////////////////////////////////////////////
	//************************************************************************
	//Make static memory so we do not allocate the memory every time the function is called
	static char buffer[256];
	rect.top = height * 0.5;
	rect.left = width * 0.05;
	if(m_iMenuSelect == 1)
	{
		//set sprintf to store the buffer size, format, and output)
		sprintf(buffer, "Menu: \nP: Play Game\nO: Options\nC: Credits\nQ: Quit Game");
		mm_MenuFont->DrawTextA(0, buffer, -1, &rect, DT_NOCLIP, 
						  D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	

	if(m_iMenuSelect == 2)
	{
		//options
		sprintf(buffer, "Options: \nControls: \nLeft Arrow Key: Move Left\nRight Arrow Key: Move Right\nSpaceBar: Fire Laser \n\nPress (M) to return to Menu");
		//display controls
		mm_MenuFont->DrawTextA(0, buffer, -1, &rect, DT_NOCLIP, 
						  D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if(m_iMenuSelect == 4)
	{
		//credits
		sprintf(buffer, "Block Shooter by Christopher Carlos\nAll Sound Files were from the original \nSpace Invaders Game by Taito. \nSound Files borrowed \nfrom: www.classicgaming.cc/classics/\nspaceinvaders/sounds.php\n\nPress (M) to return to Menu \n\nBackground Music: Ghost Trick: Phantom Detective OST- \nTrack 19 Disc 1 (no name given) \n(c) Capcom 2011");
		mm_MenuFont->DrawTextA(0, buffer, -1, &rect, DT_NOCLIP, 
						  D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	//EndScene
	m_pD3DDevice->EndScene();
	//Present Scene
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);

}