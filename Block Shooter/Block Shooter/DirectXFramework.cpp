#include "DirectXFramework.h"

using namespace std;

DirectXFramework::DirectXFramework()
{

}
DirectXFramework::~DirectXFramework()
{
	Shutdown();
}
void DirectXFramework::Init(HWND& hWnd, HINSTANCE& hInst, bool bWindowed)
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

	//////////////////////////////////////////////////
	//	Direct Show COM Objects Initialized
	/////////////////////////////////////////////////
	CoInitialize(NULL);//Initialize the objects to NULL
	//Create the instance for our Filter Graph Manager
	CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**) &m_pGraphBuilder);
	//IMediaControl object created to control our streaming
	m_pGraphBuilder->QueryInterface(IID_IMediaControl,
                                (void**)&m_pMediaControl);
	//IMediaEvent object created to receive events from our Filter Graph Manager
	m_pGraphBuilder->QueryInterface(IID_IMediaEvent,
                                (void**)&m_pMediaEvent);
	//initiate and build a filter graph for intro.wmv
	m_pGraphBuilder->RenderFile(L"Content\\Movie\\Intro.wmv", NULL);
	// Find the width and height of window using hWnd and GetWindowRect()
    // Video Window
    IVideoWindow* m_pVideoWindow;
    m_pMediaControl->QueryInterface(IID_IVideoWindow,
                                    (void**)&m_pVideoWindow);
    // Setup the window
    m_pVideoWindow->put_Owner((OAHWND)m_hWnd);
    // Set the style
    m_pVideoWindow->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE);
    // Obtain the size of the window
    RECT WinRect;
    GetClientRect(m_hWnd, &WinRect);
    // Set the video size to the size of the window
    m_pVideoWindow->SetWindowPosition(WinRect.left, WinRect.top,
                                        WinRect.right, WinRect.bottom);


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
	//*************************************************************************
	//////////////////////////////////////////////////////////////////////////
	// D3DXVECTOR3: eyePos, lookAt, upVec
	//////////////////////////////////////////////////////////////////////////
	//Declare values for your vectors
	eyePos = D3DXVECTOR3(0.0f, 0.0f, -10.0f);
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
	// Create a Font Object
	//////////////////////////////////////////////////////////////////////////
	
	// Load the D3DX Font

	D3DXCreateFont(m_pD3DDevice, 30, 0, FW_BOLD, 0, false,
                      DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
                      DEFAULT_PITCH | FF_DONTCARE, TEXT("Times New Roman"),
                      &m_pD3DFont);

	//////////////////////
	// RENDER STATES	//
	//////////////////////
	//set up the depth and z buffers
	//set render state
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(60, 60, 60));

	//////////////////////////////////////////////////////////////////////////
	// Create Sprite Object and Textures
	//////////////////////////////////////////////////////////////////////////
	/**********************CREATE SPRITE OBJECT*********************************/
	D3DXCreateSprite(m_pD3DDevice, &m_pD3DSprite);

	//////////////////////
	// LIGHT PROPERTIES //
	//////////////////////

	// ZERO MEMORY FOR LIGHT INSTANCE //
	ZeroMemory(&m_D3DLight, sizeof(m_D3DLight));
	/*************************************************************************************************
	**************************************************************************************************/
	//Set up properties of the light source: Type
	m_D3DLight.Type = D3DLIGHT_POINT;
	//Set up the properties of the light source: Ambient
	m_D3DLight.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//Set up the properties of the light source: Diffuse
	m_D3DLight.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//Set up the properties of the light source: Specular
	m_D3DLight.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//Set up properties of the light source: Position
	m_D3DLight.Position = D3DXVECTOR3(30.0f, 10.0f, -10.0f);
	//Set up properties of the light source: Range
	m_D3DLight.Range = 100.0f;
	//Set up properties of the light source: Attenuation
	m_D3DLight.Attenuation0 = 0.0f;  //Constant
	m_D3DLight.Attenuation1 = 0.05f; //Linear
	m_D3DLight.Attenuation2 = 0.0f;  //Quadratic
	//Set the light of the device
	m_pD3DDevice->SetLight(0, &m_D3DLight);
	//Enable the light
	m_pD3DDevice->LightEnable(0, true);
	/*************************************************************************************************/
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

	//////////////////////////////////////////////////////////////////////////
	// EFFECT FILE
	//////////////////////////////////////////////////////////////////////////
	// Load effect from file (Lab5.fx)	
	#ifdef _DEBUG	// If Debug flag is set in compiler, use shader debug
	
		m_pEffectError = 0;
		D3DXCreateEffectFromFile(m_pD3DDevice, 
			L"Content\\Effects\\EffectShaders.fx", 
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
	m_pEffect->SetFloatArray("lightAttenuation", (float*)&D3DXVECTOR3(m_D3DLight.Attenuation0, m_D3DLight.Attenuation1, m_D3DLight.Attenuation2), 3);

	/****************
	*	Play movie	*
	*****************/
	if(SysObj.GetGameState() == 0)
	{
		hr = m_pMediaControl->Run();//Play the movie after all objects are initialized
		if(SUCCEEDED(hr))
		{
			//still need to add movie
		}
	}
}
void DirectXFramework::Update(float dt)
{//********************************************************************
	HRESULT hr;
	////////////////////
	//	Calculate FPS //
	////////////////////
	//Make static variables to have persistant values across function calls
	static float numFrames = 0.0f;
	static float timeElapsed = 0.0f;

	//increment the frame count
	numFrames += 1.0f;

	//Accumulate how much time has passed 
	timeElapsed += dt;

	//Check if 1 second has passed, and then compute the framerate
	if(timeElapsed >= 1.0f)
	{
		//Frames per Second = numFrames / timeElapsed
		mFPS = numFrames;

		//avg frams in milliseconds
		mMilliSecPerFrame = 1000.0f / mFPS;

		//Reset counter and frame count to prepare 
		//for computing the average stats over the next second
		timeElapsed = 0.0f;
		numFrames = 0.0f;
	}
	
	//Movie Check
	long evCode;//event code
	LONG_PTR eventParam1, eventParam2;
	hr = m_pMediaEvent->GetEvent(&evCode, &eventParam1, &eventParam2, 0);
	if(SUCCEEDED(hr))
	{
		if(evCode == EC_COMPLETE)
		{
			m_bVideoOver = true;
			m_pMediaControl->Stop();
			//m_pVideoWindow->put_Visible(OAFALSE);
			//m_pVideoWindow->put_Owner((OAHWND)m_hWnd);
			//SysObj.SetGameState(1);
		}
	}
	
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
		ZeroMemory(&m_pMouseState, sizeof(m_pMouseState));
		hr = m_pDIMouse->Acquire();
		//hr = m_pDIMouse->GetDeviceState(sizeof(&m_pMouseState),(void**) &m_pMouseState);
	}
	if(m_pKeyboardState[DIK_RETURN] & 0x80)
	{
		m_pMediaControl->Stop();
		m_bVideoOver = true;
		//SysObj.SetGameState(1);
	}
	

}
void DirectXFramework::Shutdown()
{

}
void DirectXFramework::Render()
{

}

bool DirectXFramework::GetMovieState()
{
	return m_bVideoOver;
}

/*IDirect3DDevice9* DirectXFramework::GetDevice()
{
	return m_pD3DDevice;
}*/
