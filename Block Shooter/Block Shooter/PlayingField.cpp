#include "PlayingField.h"
#include <iostream>
using namespace std;

PlayingField::PlayingField()
{
	mpf_fCenterX		= 0.0f;
	mpf_fCenterY		= 0.0f;
	mpf_fWidth			= 600.0;
	mpf_fHeight			= 800.0;
	mpf_pBackgroundSprite = 0;
	D3DXMatrixIdentity(&rotMat);
	D3DXMatrixIdentity(&transMat);
	D3DXMatrixIdentity(&scaleMat);
	D3DXMatrixIdentity(&worldMat);
	D3DXMatrixIdentity(&multRSMat);
	//get random count
	srand(time(NULL));
	bRanUsed = false;
	m_bIsGameOver = false;
	ReturnToMenu = false;
}
PlayingField::~PlayingField()
{

}

void PlayingField::Init(HWND& hWnd, HINSTANCE& hInst, bool bWindowed)
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
	//*************************************************************************
	//////////////////////////////////////////////////////////////////////////
	// D3DXVECTOR3: eyePos, lookAt, upVec
	//////////////////////////////////////////////////////////////////////////
	//Declare values for your vectors
	eyePos = D3DXVECTOR3(0.0f, 10.0f, -10.0f);
	lookAt = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
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

	/************************************************************************/
	
	//////////////////////
	// RENDER STATES	//
	//////////////////////
	//set up the depth and z buffers
	//set render state
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(60, 60, 60));

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
	m_D3DLight.Ambient = D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f);
	//Set up the properties of the light source: Diffuse
	m_D3DLight.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//Set up the properties of the light source: Specular
	m_D3DLight.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//Set up properties of the light source: Position
	m_D3DLight.Position = D3DXVECTOR3(0.0f, 10.0, 0.0f);
	//Set up properties of the light source: Range
	m_D3DLight.Range = 100.0f;
	//Set up properties of the light source: Attenuation
	m_D3DLight.Attenuation0 = 0.0f;  //Constant
	m_D3DLight.Attenuation1 = 0.10f; //Linear
	m_D3DLight.Attenuation2 = 0.0f;  //Quadratic
	//m_D3DLight.Phi = D3DXToRadian(90);
	//m_D3DLight.Theta = D3DXToRadian(100);
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

	//////////////////////
	// CREATE TEXTURE	//
	//////////////////////
	//create texture for background
	D3DXCreateTextureFromFileEx(m_pD3DDevice, 
		L"Content\\Texture\\PlayingField_Background.png", 
		0, 0, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255), &m_imageInfo, 0, &mpf_pBackgroundTexture);


	/*******************************************************************************************************
	*										SHIP INITIALIZATION											   *
	********************************************************************************************************/
	/////////////////
	//Load .X File//
	////////////////
	D3DXLoadMeshFromX(L"Content\\Models\\PlayerShip.x",
		D3DXMESH_MANAGED,
		m_pD3DDevice,
		&m_pAdjBuffer,
		&m_pXMaterials,
		&m_pXEffect,
		&m_numMaterials,
		&m_pShipStaticMesh);
	/////////////////////////
	// MATERIAL PROPERTIES //
	/////////////////////////
	//Zero the memory of each material object
	ZeroMemory(&m_pPlayerMaterial, sizeof(m_pPlayerMaterial));
	//set up the parameters
	m_pMaterial.Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);		// Ambient color reflected
	m_pMaterial.Diffuse = D3DXCOLOR(1.0f, 0.3f, 0.3f, 1.0f);		// Diffuse color reflected
	m_pMaterial.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);		// Emissive color reflected
	m_pMaterial.Specular = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);		// Specular
	m_pMaterial.Power = 30.0f;	

	//---------------------//
	// BOUNDING BOX COLLIS//
	//--------------------//
	//lock vertex of mesh
	m_pShipStaticMesh->LockVertexBuffer(0, (void**)&boxVerts);

	//Compute the bounding box
	hr = D3DXComputeBoundingBox(&boxVerts[0].position,
		m_pShipStaticMesh->GetNumVertices(), 
		sizeof(Vertex),
		&BoundBox.MinPoint, 
		&BoundBox.MaxPoint);
	if(FAILED(hr))
	{
		printf("Ship's bounding box failed", hr);
	}
	//unlock the vertices
	m_pShipStaticMesh->UnlockVertexBuffer();
	//create the width, height, and depth of our bounding box to draw
	PS.Width = BoundBox.MaxPoint.x - BoundBox.MinPoint.x;
	PS.Height = BoundBox.MaxPoint.y - BoundBox.MinPoint.y;
	PS.Depth = BoundBox.MaxPoint.z - BoundBox.MinPoint.z;
	//Create the bounding box mesh
	D3DXCreateBox(m_pD3DDevice, PS.Width, PS.Height, PS.Depth, &m_pShipBox, 0);

	PS.Center.x = ((PS.Width * 0.5) + PS.PosX) * 0.1;
	PS.Center.y = ((PS.Height * 0.5) + PS.PosY) * 0.1;
	PS.Center.z = ((PS.Depth * 0.5) + PS.PosZ) * 0.1;
	/*************************************************************************************************/
	//Set Texture
	D3DXCreateTextureFromFile
		(m_pD3DDevice, 
		L"ShipTex.png", 
		&m_pPlayerTexture[0]); 
	//________________________________________________________________________________________________//
	//Ship Declaration
	
	//Init variables of ship
	PS.Position = D3DXVECTOR3(0, 0, 5);
	//PS.CenterX = PS.Center.x;
	//PS.CenterY = PS.Center.y;
	PS.PosX = PS.Position.x;
	PS.PosY = PS.Position.y;
	PS.PosZ = PS.Position.z;
	////////////////
	// SET HEALTH //
	////////////////
	PS.Health = 3;
	//set center
	BoundBox.Center = 0.5 * (BoundBox.MinPoint + BoundBox.MaxPoint);
	//identity
	D3DXMatrixIdentity(&m_pPSBoundBoxOffset);

	D3DXMatrixTranslation(&m_pPSBoundBoxOffset, BoundBox.Center.x, BoundBox.Center.y, BoundBox.Center.z);
	
	//_________________________________________________________________________________________________//


	/***************************************************************************************************/

	/***************************************************************************************************
	/									ALIEN INITIALIZATION											/
	*****************************************************************************************************/

	//load .X file
	D3DXLoadMeshFromX(L"Content\\Models\\AlienShip.x",
		D3DXMESH_MANAGED,
		m_pD3DDevice,
		&m_pAdjBuffer,
		&m_pXMaterials,
		&m_pXEffect,
		&m_numMaterials,
		&ms_pAnimatedMesh);

	//set texture
	//Set Texture
	D3DXCreateTextureFromFile
		(m_pD3DDevice, 
		L"Content\\Texture\\Alien_Top.png", 
		&m_pAlienTexture[0]); 

	//Declare Struct properties
	
	AS.Position = D3DXVECTOR3(2, 5, 5);
	AS.PosX = AS.Position.x;
	AS.PosY = AS.Position.y;
	AS.PosZ = AS.Position.z;

	//---------------------//
	// BOUNDING SPHERE COLL//
	//--------------------//

	//lock vertex of mesh
	ms_pAnimatedMesh->LockVertexBuffer(0, (void**)&sphereVerts);

	//Compute the bounding box
	hr = D3DXComputeBoundingSphere(&sphereVerts[0].position,
		ms_pAnimatedMesh->GetNumVertices(), 
		sizeof(Vertex),
		&BoundSphere.pos, 
		&BoundSphere.radius);
	if(FAILED(hr))
	{
		printf("Block's bounding box failed", hr);
	}
	//unlock the vertices
	ms_pAnimatedMesh->UnlockVertexBuffer();
	
	//Create the bounding box mesh
	D3DXCreateSphere(m_pD3DDevice, BoundSphere.radius, 25, 25, &m_pAlienSphere, 0);

	/*****************************************************************************************************/

	//--------------------------------------------------------------------------------------------------//
	//										BLOCK INITIALIZATION										//
	//-------------------------------------------------------------------------------------------------//

	//load .X file
	D3DXLoadMeshFromX(L"Content\\Models\\Block.x",
		D3DXMESH_MANAGED,
		m_pD3DDevice,
		&m_pAdjBuffer,
		&m_pXMaterials,
		&m_pXEffect,
		&m_numMaterials,
		&m_pBlockMesh);

	//set texture
	//Set Texture
	D3DXCreateTextureFromFile
		(m_pD3DDevice, 
		L"Content\\Texture\\block.png", 
		&m_pBlockTexture[0]); 

	//Declare Struct properties
	Bk.Position = D3DXVECTOR3(0, 15, 5);
	Bk.PosX = Bk.Position.x;
	Bk.PosY = Bk.Position.y;
	Bk.PosZ = Bk.Position.z;

	////////////////
	// SET HEALTH //
	///////////////
	Bk.Health = 3;
	Bk.bIsAlive = true;

	//---------------------//
	// BOUNDING BOX COLLIS//
	//--------------------//
	//lock vertex of mesh
	m_pBlockMesh->LockVertexBuffer(0, (void**)&boxVerts);

	//Compute the bounding box
	hr = D3DXComputeBoundingBox(&boxVerts[0].position,
		m_pBlockMesh->GetNumVertices(), 
		sizeof(Vertex),
		&BlockBoundBox.MinPoint, 
		&BlockBoundBox.MaxPoint);
	if(FAILED(hr))
	{
		printf("Block's bounding box failed", hr);
	}
	//unlock the vertices
	m_pBlockMesh->UnlockVertexBuffer();
	//create the width, height, and depth of our bounding box to draw
	Bk.Width = BlockBoundBox.MaxPoint.x - BlockBoundBox.MinPoint.x;
	Bk.Height = BlockBoundBox.MaxPoint.y - BlockBoundBox.MinPoint.y;
	Bk.Depth = BlockBoundBox.MaxPoint.z - BlockBoundBox.MinPoint.z;
	//Create the bounding box mesh
	D3DXCreateBox(m_pD3DDevice, Bk.Width, Bk.Height, Bk.Depth, &m_pBlockBox, 0);
	
	//------------------------------------------------------------------------------------------------//
	
	//--------------------------------------------------------------------------------------------------//
	//									Playing FieldINITIALIZATION										//
	//-------------------------------------------------------------------------------------------------//

	//load .X file
	D3DXLoadMeshFromX(L"Content\\Models\\PlayingField.x",
		D3DXMESH_MANAGED,
		m_pD3DDevice,
		&m_pAdjBuffer,
		&m_pXMaterials,
		&m_pXEffect,
		&m_numMaterials,
		&m_pPlayingFieldMesh);

	//Set Texture
	D3DXCreateTextureFromFile
		(m_pD3DDevice, 
		L"Content\\Texture\\PlayingField_Background.png", 
		&m_pPFTexture[0]); 


	//------------------------------------------------------------------------------------------------//
	
	//===============================================================================================//
	//										LASER INTIALIZATION										//
	//==============================================================================================//

	//load .X file
	D3DXLoadMeshFromX(L"Content\\Models\\Laser.x",
		D3DXMESH_MANAGED,
		m_pD3DDevice,
		&m_pAdjBuffer,
		&m_pXMaterials,
		&m_pXEffect,
		&m_numMaterials,
		&m_pLaserMesh);

	//Set Texture
	D3DXCreateTextureFromFile
		(m_pD3DDevice, 
		L"Content\\Texture\\Laser.png", 
		&m_pLaserTexture[0]); 
	
	//Set Struct Properties
	Lsr.Center = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Lsr.PosX = Lsr.Center.x;
	Lsr.PosY = Lsr.Center.y;
	Lsr.PosZ = Lsr.Center.z;

	//---------------------//
	// BOUNDING BOX COLLIS//
	//--------------------//
	//lock vertex of mesh
	m_pLaserMesh->LockVertexBuffer(0, (void**)&laserVerts);

	//Compute the bounding box
	hr = D3DXComputeBoundingBox(&laserVerts[0].position,
		m_pLaserMesh->GetNumVertices(), 
		sizeof(Vertex),
		&LaserBoundBox.MinPoint, 
		&LaserBoundBox.MaxPoint);
	if(FAILED(hr))
	{
		printf("Block's bounding box failed", hr);
	}
	//unlock the vertices
	m_pLaserMesh->UnlockVertexBuffer();
	//create the width, height, and depth of our bounding box to draw
	Lsr.Width = LaserBoundBox.MaxPoint.x - LaserBoundBox.MinPoint.x;
	Lsr.Height = LaserBoundBox.MaxPoint.y - LaserBoundBox.MinPoint.y;
	Lsr.Depth = LaserBoundBox.MaxPoint.z - LaserBoundBox.MinPoint.z;
	//Create the bounding box mesh
	D3DXCreateBox(m_pD3DDevice, Lsr.Width, Lsr.Height, Lsr.Depth, &m_pLaserBox, 0);

	//------------------------------------------------------------------------------------------------//
	//===============================================================================================//
	//								ENEMY LASER INTIALIZATION										//
	//==============================================================================================//

	//load .X file
	D3DXLoadMeshFromX(L"Content\\Models\\Laser.x",
		D3DXMESH_MANAGED,
		m_pD3DDevice,
		&m_pAdjBuffer,
		&m_pXMaterials,
		&m_pXEffect,
		&m_numMaterials,
		&m_pLaserMesh);

	//Set Texture
	D3DXCreateTextureFromFile
		(m_pD3DDevice, 
		L"Content\\Texture\\Laser.png", 
		&m_pLaserTexture[0]); 
	
	//Set Struct Properties
	BlockLsr.Center = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	BlockLsr.PosX = Lsr.Center.x;
	BlockLsr.PosY = Lsr.Center.y;
	BlockLsr.PosZ = Lsr.Center.z;

	//---------------------//
	// BOUNDING BOX COLLIS//
	//--------------------//
	//lock vertex of mesh
	m_pLaserMesh->LockVertexBuffer(0, (void**)&laserVerts);

	//Compute the bounding box
	hr = D3DXComputeBoundingBox(&laserVerts[0].position,
		m_pLaserMesh->GetNumVertices(), 
		sizeof(Vertex),
		&LaserBoundBox.MinPoint, 
		&LaserBoundBox.MaxPoint);
	if(FAILED(hr))
	{
		printf("Block's bounding box failed", hr);
	}
	//unlock the vertices
	m_pLaserMesh->UnlockVertexBuffer();
	//create the width, height, and depth of our bounding box to draw
	BlockLsr.Width = LaserBoundBox.MaxPoint.x - LaserBoundBox.MinPoint.x;
	BlockLsr.Height = LaserBoundBox.MaxPoint.y - LaserBoundBox.MinPoint.y;
	BlockLsr.Depth = LaserBoundBox.MaxPoint.z - LaserBoundBox.MinPoint.z;
	//Create the bounding box mesh
	D3DXCreateBox(m_pD3DDevice, Lsr.Width, Lsr.Height, Lsr.Depth, &m_pLaserBox, 0);

	//------------------------------------------------------------------------------------------------//
	////////////////////////////
	// CREATE EFFECTS FOR FILE//
	////////////////////////////
	meshMaterials = new D3DMATERIAL9[m_numMaterials];
	meshTextures = new LPDIRECT3DTEXTURE9[m_numMaterials];
	d3dxMaterials = (D3DXMATERIAL*)m_pXMaterials->GetBufferPointer();
	for(DWORD i = 0; i < m_numMaterials; i++)
	{
		//Copy the data of D3DXMaterials into meshMaterials (set defaults)
		meshMaterials[i] = d3dxMaterials[i].MatD3D;
		//Set Ambient color for material
		meshMaterials[i].Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		meshMaterials[i].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		meshMaterials[i].Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		meshMaterials[i].Power = 100; 
		meshMaterials[i].Specular = D3DXCOLOR(1.0, 1.0, 1.0, 1.0f);
		//Create our textures
		meshTextures[i] = 0;
		if(d3dxMaterials[i].pTextureFilename != 0)
		{
			int len = 0; //local variable for length of string
			//store the length of the texture's filename + 1
			len = (int)strlen(d3dxMaterials[i].pTextureFilename + 1);
			wchar_t *ucString = new wchar_t[len];//create a wide char pointer
			mbstowcs(ucString, d3dxMaterials[i].pTextureFilename, len);
			LPCWSTR filename = (LPCWSTR)ucString;
			// Load the texture from the file name
			D3DXCreateTextureFromFile(m_pD3DDevice, filename, &meshTextures[i]);
		}
		
	}
	
	FMOD_RESULT result;
	/////////////////////////////////////
	// FMOD SOUND OBJECTS CREATED HERE //
	/////////////////////////////////////
	result = FMOD::System_Create(&m_pFSystem);
	if(result != FMOD_OK)
	{
		result = FMOD::System_Create(&m_pFSystem);
		//should have error message
	}
	result = m_pFSystem->init(100, FMOD_INIT_NORMAL, 0);
	if(result != FMOD_OK)
	{
		result = m_pFSystem->init(100, FMOD_INIT_NORMAL, 0);
		//should have error message
	}
	/********************************************************************************************************/
	//CREATE SOUNDS
	//SOUND 1 
	result = m_pFSystem->createSound("Content\\Sound\\shoot.wav",
		FMOD_DEFAULT, 0, &m_pLaserSound);
	if(result != FMOD_OK)
	{
		//should have error message
	}
	//SOUND 2 
	result = m_pFSystem->createSound("Content\\Sound\\invaderkilled.wav",
		FMOD_DEFAULT, 0, &m_pHitSound);
	if(result != FMOD_OK)
	{
		//should have error message
	}
	//SOUND 2 
	result = m_pFSystem->createSound("Content\\Sound\\explosion.wav",
		FMOD_DEFAULT, 0, &m_pExplosionSound);
	if(result != FMOD_OK)
	{
		//should have error message
	}
	//SOUND LOOP 
	result = m_pFSystem->createStream("Content\\Sound\\GT19.mp3",
		FMOD_DEFAULT, 0, &m_pFBGMStream);
	if(result != FMOD_OK)
	{
		//should have error message
	}
	m_pFSystem->playSound(FMOD_CHANNEL_FREE, m_pFBGMStream, false, 0);
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


}

void PlayingField::Update(float dt)
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
		ZeroMemory(&m_pMouseState, sizeof(m_pMouseState));
		hr = m_pDIMouse->Acquire();
		//hr = m_pDIMouse->GetDeviceState(sizeof(&m_pMouseState),(void**) &m_pMouseState);
	}
	if(!m_bIsGameOver)
	{
		////////////////////
		//	Calculate FPS //
		////////////////////
		//Make static variables to have persistant values across function calls
		static float numFrames = 0.0f;
		static float timeElapsed = 0.0f;
		static float timer;
		static float laserTimer;
		//increment the frame count
		numFrames += 1.0f;

		//Accumulate how much time has passed 
		timeElapsed += dt;
		timer += dt;
		laserTimer += dt;
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
		/////////////////////////
		// Laser Shooting      //
		/////////////////////////
		if(Lsr.bIsAlive)
		{
			Lsr.AccelY = 0.1;
			Lsr.PosY += Lsr.AccelY;
			float distY;
			distY = calcY(Lsr.PosY, Bk.PosY);
			if(distY <= 1)
			{
				printf("collision at Y Position");
				float distX = calcX(Lsr.PosX, Bk.PosX);
				if(distX <= 1)
				{
					printf("collision at X position");
					m_pFSystem->playSound(FMOD_CHANNEL_FREE, m_pHitSound, false, 0);
					Bk.Health -= 1;
					Lsr.PosX = 0.0f;
					Lsr.PosY = 0.0f;
					Lsr.AccelY = 0.0f;
					Lsr.bIsAlive = false;
					if(Bk.Health == 0)
					{
						printf("Enemy down! Good Job!");
						m_pFSystem->playSound(FMOD_CHANNEL_FREE, m_pExplosionSound, false, 0);
						m_bIsGameOver = true;
					}
				}
			}
		}
		if(Lsr.PosY >= 21.0f)
		{
			Lsr.bIsAlive = false;
			Lsr.AccelY = 0.0f;
		}

		//Block Enemy Laser
		if(BlockLsr.bIsAlive)
		{
			BlockLsr.AccelY = -0.1;
			BlockLsr.PosY += BlockLsr.AccelY;
			float enDistY;
			enDistY = calcY(BlockLsr.PosY, PS.PosY);
			if(enDistY <= 1)
			{
				printf("collision at Y Position");
				float enDistX = calcX(BlockLsr.PosX, PS.PosX);
				if(enDistX <= 1)
				{
					printf("collision at X position");
					m_pFSystem->playSound(FMOD_CHANNEL_FREE, m_pHitSound, false, 0);
					PS.Health -= 1;
					BlockLsr.PosX = 0.0f;
					BlockLsr.PosY = 0.0f;
					BlockLsr.AccelY = 0.0f;
					BlockLsr.bIsAlive = false;
					if(PS.Health == 0)
					{
						printf("Enemy down! Good Job!");
						m_pFSystem->playSound(FMOD_CHANNEL_FREE, m_pExplosionSound, false, 0);
						m_bIsGameOver = true;
					}
				}
			}
		}
		if(BlockLsr.PosY < 0.0)
		{
			BlockLsr.bIsAlive = false;
			BlockLsr.AccelY = 0.0f;
		}
		//////////////////////
		// Enemy Movement	//
		//////////////////////
		if(Bk.bIsAlive)
		{
			// Use a random number to determine if we move opposite direction or stay current direction
			if(timer >= 1.00)
			{
				RanNumGen = rand()%(4 - 1) + 1;
			}
			// Block's movement speed declared here. 
			if(RanNumGen >= 3)
			{
				Bk.AccelX = 0.025;
			}
			else if(RanNumGen <=2)
			{
				Bk.AccelX = -0.025;
			}
			// When we go off the screen, wrap around to the other side
			if(Bk.PosX >= 7.0f)
			{
				Bk.PosX = Bk.PosX * -1.0;
			}
			else if(Bk.PosX <= -7.0f)
			{
				Bk.PosX = Bk.PosX * -1.0;
			}
			// After 1 second, enemy shoots laser
			if(timer > 1.0 && !BlockLsr.bIsAlive)
			{
				EShootLaser(Bk.PosX, Bk.PosY, Bk.PosZ);
				m_pFSystem->playSound(FMOD_CHANNEL_FREE, m_pLaserSound, false, 0);
				BlockLsr.bIsAlive = true;
				timer = 0.0f;
			}
			Bk.PosX += Bk.AccelX;
		}
	
		///////////////////
		// GAME CONTROLS //
		///////////////////
		if(m_pKeyboardState[DIK_LEFT] & 0x80)
		{
			if(PS.PosX > -7.00)
			{
				PS.PosX -= 0.05;
			}
		}
		if(m_pKeyboardState[DIK_RIGHT] & 0x81)
		{
			if(PS.PosX <= 7.00)
			{
				PS.PosX += 0.05;
			}
		}
		if(m_pKeyboardState[DIK_SPACE] & 0x82)
		{
			Lsr.bIsAlive = true;
			if(laserTimer > 1.0)
			{
				ShootLaser(PS.PosX, PS.PosY, PS.PosZ);
				m_pFSystem->playSound(FMOD_CHANNEL_FREE, m_pLaserSound, false, 0);
				laserTimer = 0.0f;
			}
		}

		/*if(m_pKeyboardState[DIK_UP] & 0x80)
		{
			PS.PosY += 0.1f;
		}
		if(m_pKeyboardState[DIK_DOWN] & 0x80)
		{
			PS.PosY -= 0.1f;
		}*/
	}
	if(m_bIsGameOver)
	{
		Bk.Position = D3DXVECTOR3(0, 15, 5);
		PS.Position = D3DXVECTOR3(0, 0, 5);
		BlockLsr.bIsAlive = false;
		Lsr.bIsAlive = false;
		if(m_pKeyboardState[DIK_M] & 0x80)
		{
			MenuObj.SetGameState(1);
			ReturnToMenu = true;
		}
	}
}

void PlayingField::Draw()
{
	
	// Clear the back buffer, call BeginScene()
	m_pD3DDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DXCOLOR(0.0f, 0.35f, 0.8f, 1.0f), 1.0f, 0);
	m_pD3DDevice->BeginScene();

	//Obtain Rect of Windo
	RECT rect; 
	GetWindowRect(m_hWnd, &rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	// Begin Pass for Effects
	// Set the technique to use
	m_pEffect->SetTechnique(m_hTech);
	// Begin Effect
	UINT numPasses = 0;
	m_pEffect->Begin(&numPasses, 0);

	for(UINT i = 0; i < numPasses; ++i)
	{
		m_pEffect->BeginPass(i);
		// Set object specific parameters
		//D3DXMATRIX WIT;
		//Create Matrices
		//D3DXMATRIX transMat, rotMat, scaleMat, worldMat, multRSMat;
		//setup the Matrix Identity (initiate the arrays)
		/*D3DXMatrixIdentity(&rotMat);
		D3DXMatrixIdentity(&transMat);
		D3DXMatrixIdentity(&scaleMat);
		D3DXMatrixIdentity(&worldMat);
		D3DXMatrixIdentity(&multRSMat);*/
		
		////////////////////////////
		// DRAW Player Ship Model //
		////////////////////////////
		// Scale the object
		D3DXMatrixScaling(&scaleMat, 0.1, 0.1, 0.1);
		//Rotate on Yaw
		D3DXMatrixRotationYawPitchRoll(&rotMat, 0.0f, 0.0f, 0.0f);

		//Translate
		D3DXMatrixTranslation(&transMat, PS.PosX, PS.PosY, PS.PosZ);

		//Multiply scale and rot
		D3DXMatrixMultiply(&multRSMat, &scaleMat, &rotMat);
		//Multiply for world
		D3DXMatrixMultiply(&worldMat, &multRSMat, &transMat);
		// Set object specific parameters
		D3DXMatrixInverse(&WIT, 0, &worldMat);
		D3DXMatrixTranspose(&WIT, &WIT);
		for(int i = 0; i < m_numMaterials; ++i) 
		{
			m_pEffect->SetMatrix("worldViewProjMat", &(worldMat * viewMat * projMat));
			m_pEffect->SetMatrix("worldInverseTransposeMat", &WIT);
			m_pEffect->SetMatrix("worldMat", &worldMat);
			m_pEffect->SetValue("ambientMaterial", &meshMaterials[i].Ambient, sizeof(D3DXCOLOR));
			m_pEffect->SetValue("diffuseMaterial", &meshMaterials[i].Diffuse, sizeof(D3DXCOLOR));
			m_pEffect->SetValue("specularMaterial",	&meshMaterials[i].Specular, sizeof(D3DXCOLOR));
			m_pEffect->SetFloat("specularPower", meshMaterials[i].Power);
			m_pEffect->SetTexture("tex", m_pPlayerTexture[0]);
			m_pEffect->SetBool("isTex", true);
			// After setting object specific parameters, commit changes
			m_pEffect->CommitChanges();
			// Draw the mesh subset
			m_pShipStaticMesh->DrawSubset(i);
		}

		////////////////////////////
		// DRAW Alien Ship Model  //
		////////////////////////////
		// Scale the object
		/*D3DXMatrixScaling(&scaleMat, 0.1, 0.1, 0.1);
		//Rotate on Yaw
		D3DXMatrixRotationYawPitchRoll(&rotMat, 0.0f, 0.0f, 0.0f);

		//Translate
		D3DXMatrixTranslation(&transMat, AS.PosX, AS.PosY, AS.PosZ);

		//Multiply scale and rot
		D3DXMatrixMultiply(&multRSMat, &scaleMat, &rotMat);
		//Multiply for world
		D3DXMatrixMultiply(&worldMat, &multRSMat, &transMat);
		// Set object specific parameters
		D3DXMatrixInverse(&WIT, 0, &worldMat);
		D3DXMatrixTranspose(&WIT, &WIT);
		for(int i = 0; i < m_numMaterials; ++i) 
		{
			m_pEffect->SetMatrix("worldViewProjMat", &(worldMat * viewMat * projMat));
			m_pEffect->SetMatrix("worldInverseTransposeMat", &WIT);
			m_pEffect->SetMatrix("worldMat", &worldMat);
			m_pEffect->SetValue("ambientMaterial",
			&meshMaterials[i].Ambient, sizeof(D3DXCOLOR));
			m_pEffect->SetValue("diffuseMaterial", &meshMaterials[i].Diffuse, sizeof(D3DXCOLOR));
			m_pEffect->SetValue("specularMaterial",	&meshMaterials[i].Specular, sizeof(D3DXCOLOR));
			m_pEffect->SetFloat("specularPower", meshMaterials[i].Power);
			m_pEffect->SetTexture("tex", m_pAlienTexture[0]);
			m_pEffect->SetBool("isTex", true);
			// After setting object specific parameters, commit changes
			m_pEffect->CommitChanges();
			// Draw the mesh subset
			ms_pAnimatedMesh->DrawSubset(i);
		}*/
		

		//////////////////////////////
		// DRAW Block Enemy Model  //
		////////////////////////////
		for(unsigned int i = 0; i < GFObj.GetBlocksLeft(); i++ )
		{
			// Scale the object
			D3DXMatrixScaling(&scaleMat, 0.1, 0.1, 0.1);
			//Rotate on Yaw
			D3DXMatrixRotationYawPitchRoll(&rotMat, 0.0f, 0.0f, 0.0f);
			
			//Translate
			D3DXMatrixTranslation(&transMat, Bk.PosX, Bk.PosY, Bk.PosZ);

			//Multiply scale and rot
			D3DXMatrixMultiply(&multRSMat, &scaleMat, &rotMat);
			//Multiply for world
			D3DXMatrixMultiply(&worldMat, &multRSMat, &transMat);
			// Set object specific parameters
			D3DXMatrixInverse(&WIT, 0, &worldMat);
			D3DXMatrixTranspose(&WIT, &WIT);
			for(int i = 0; i < m_numMaterials; ++i) 
			{
				m_pEffect->SetMatrix("worldViewProjMat", &(worldMat * viewMat * projMat));
				m_pEffect->SetMatrix("worldInverseTransposeMat", &WIT);
				m_pEffect->SetMatrix("worldMat", &worldMat);
				m_pEffect->SetValue("ambientMaterial",
				&meshMaterials[i].Ambient, sizeof(D3DXCOLOR));
				m_pEffect->SetValue("diffuseMaterial", &meshMaterials[i].Diffuse, sizeof(D3DXCOLOR));
				m_pEffect->SetValue("specularMaterial",	&meshMaterials[i].Specular, sizeof(D3DXCOLOR));
				m_pEffect->SetFloat("specularPower", meshMaterials[i].Power);
				m_pEffect->SetTexture("tex", m_pBlockTexture[0]);
				m_pEffect->SetBool("isTex", true);
				// After setting object specific parameters, commit changes
				m_pEffect->CommitChanges();
				// Draw the mesh subset
				m_pBlockMesh->DrawSubset(i);
			}
		}

		//==========================//
		// DRAW BLOCK COLLISION		//
		//==========================//

		
		// Scale the object
		/*D3DXMatrixScaling(&scaleMat, 0.1, 0.1, 0.1);

		//Rotate on Yaw
		D3DXMatrixRotationYawPitchRoll(&rotMat, 0.0f, 0.0f, 0.0f);

		//Translate
		D3DXMatrixTranslation(&transMat, Bk.PosX, Bk.PosY, Bk.PosZ);

		//Multiply scale and rot
		D3DXMatrixMultiply(&multRSMat, &scaleMat, &rotMat);

		//Multiply for world
		D3DXMatrixMultiply(&worldMat, &multRSMat, &transMat);

		// Set object specific parameters
		D3DXMatrixInverse(&WIT, 0, &worldMat);
		D3DXMatrixTranspose(&WIT, &WIT);

		for(int i = 0; i < m_numMaterials; ++i) 
		{
			m_pEffect->SetMatrix("worldViewProjMat", &(worldMat * viewMat * projMat));
			m_pEffect->SetMatrix("worldInverseTransposeMat", &WIT);
			m_pEffect->SetMatrix("worldMat", &worldMat);
			m_pEffect->SetValue("ambientMaterial", &meshMaterials[i].Ambient, sizeof(D3DXCOLOR));
			m_pEffect->SetValue("diffuseMaterial", &meshMaterials[i].Diffuse, sizeof(D3DXCOLOR));
			m_pEffect->SetValue("specularMaterial",	&meshMaterials[i].Specular, sizeof(D3DXCOLOR));
			m_pEffect->SetFloat("specularPower", meshMaterials[i].Power);
			//m_pEffect->SetTexture("tex", m_pPlayerTexture[0]);
			//m_pEffect->SetBool("isTex", true);
			// After setting object specific parameters, commit changes
			m_pEffect->CommitChanges();
			// Draw the mesh subset
			m_pBlockBox->DrawSubset(i);
		}*/

		//////////////////////////////
		// DRAW Playing Field Model //
		/////////////////////////////
		// Scale the object
		D3DXMatrixScaling(&scaleMat, 1.0f, 1.0f, 1.0f);
		//Rotate on Yaw
		D3DXMatrixRotationYawPitchRoll(&rotMat, 0.0f, 0.0f, 0.0f);

		//Translate
		D3DXMatrixTranslation(&transMat, 0, 0, 7);

		//Multiply scale and rot
		D3DXMatrixMultiply(&multRSMat, &scaleMat, &rotMat);
		//Multiply for world
		D3DXMatrixMultiply(&worldMat, &multRSMat, &transMat);
		// Set object specific parameters
		D3DXMatrixInverse(&WIT, 0, &worldMat);
		D3DXMatrixTranspose(&WIT, &WIT);
		for(int i = 0; i < m_numMaterials; ++i) 
		{
			m_pEffect->SetMatrix("worldViewProjMat", &(worldMat * viewMat * projMat));
			m_pEffect->SetMatrix("worldInverseTransposeMat", &WIT);
			m_pEffect->SetMatrix("worldMat", &worldMat);
			m_pEffect->SetValue("ambientMaterial",
			&meshMaterials[i].Ambient, sizeof(D3DXCOLOR));
			m_pEffect->SetValue("diffuseMaterial", &meshMaterials[i].Diffuse, sizeof(D3DXCOLOR));
			m_pEffect->SetValue("specularMaterial",	&meshMaterials[i].Specular, sizeof(D3DXCOLOR));
			m_pEffect->SetFloat("specularPower", meshMaterials[i].Power);
			m_pEffect->SetTexture("tex", m_pPFTexture[0]);
			m_pEffect->SetBool("isTex", true);
			// After setting object specific parameters, commit changes
			m_pEffect->CommitChanges();
			// Draw the mesh subset
			m_pPlayingFieldMesh->DrawSubset(i);
		}

		//////////////////
		// DRAW LASER	//
		//////////////////
		if(Lsr.bIsAlive == true)
		{
			// Scale the object
			D3DXMatrixScaling(&scaleMat, 0.25f, 0.25f, 0.25f);
			//Rotate on Yaw
			D3DXMatrixRotationYawPitchRoll(&rotMat, 0.0f, 0.0f, 0.0f);

			//Translate
			D3DXMatrixTranslation(&transMat, Lsr.PosX, Lsr.PosY, Lsr.PosZ);
			//Multiply scale and rot
			D3DXMatrixMultiply(&multRSMat, &scaleMat, &rotMat);
			//Multiply for world
			D3DXMatrixMultiply(&worldMat, &multRSMat, &transMat);
			// Set object specific parameters
			D3DXMatrixInverse(&WIT, 0, &worldMat);
			D3DXMatrixTranspose(&WIT, &WIT);
			for(int i = 0; i < m_numMaterials; ++i) 
			{
				m_pEffect->SetMatrix("worldViewProjMat", &(worldMat * viewMat * projMat));
				m_pEffect->SetMatrix("worldInverseTransposeMat", &WIT);
				m_pEffect->SetMatrix("worldMat", &worldMat);
				m_pEffect->SetValue("ambientMaterial",
				&meshMaterials[i].Ambient, sizeof(D3DXCOLOR));
				m_pEffect->SetValue("diffuseMaterial", &meshMaterials[i].Diffuse, sizeof(D3DXCOLOR));
				m_pEffect->SetValue("specularMaterial",	&meshMaterials[i].Specular, sizeof(D3DXCOLOR));
				m_pEffect->SetFloat("specularPower", meshMaterials[i].Power);
				m_pEffect->SetTexture("tex", m_pLaserTexture[0]);
				m_pEffect->SetBool("isTex", true);
				// After setting object specific parameters, commit changes
				m_pEffect->CommitChanges();
				// Draw the mesh subset
				m_pLaserMesh->DrawSubset(i);
			}
		}
		//////////////////////////
		// DRAW  ENEMY LASER	//
		//////////////////////////
		if(BlockLsr.bIsAlive == true)
		{
			// Scale the object
			D3DXMatrixScaling(&scaleMat, 0.25f, 0.25f, 0.25f);
			//Rotate on Yaw
			D3DXMatrixRotationYawPitchRoll(&rotMat, 0.0f, 0.0f, 0.0f);

			//Translate
			D3DXMatrixTranslation(&transMat, BlockLsr.PosX, BlockLsr.PosY, BlockLsr.PosZ);
			//Multiply scale and rot
			D3DXMatrixMultiply(&multRSMat, &scaleMat, &rotMat);
			//Multiply for world
			D3DXMatrixMultiply(&worldMat, &multRSMat, &transMat);
			// Set object specific parameters
			D3DXMatrixInverse(&WIT, 0, &worldMat);
			D3DXMatrixTranspose(&WIT, &WIT);
			for(int i = 0; i < m_numMaterials; ++i) 
			{
				m_pEffect->SetMatrix("worldViewProjMat", &(worldMat * viewMat * projMat));
				m_pEffect->SetMatrix("worldInverseTransposeMat", &WIT);
				m_pEffect->SetMatrix("worldMat", &worldMat);
				m_pEffect->SetValue("ambientMaterial",
				&meshMaterials[i].Ambient, sizeof(D3DXCOLOR));
				m_pEffect->SetValue("diffuseMaterial", &meshMaterials[i].Diffuse, sizeof(D3DXCOLOR));
				m_pEffect->SetValue("specularMaterial",	&meshMaterials[i].Specular, sizeof(D3DXCOLOR));
				m_pEffect->SetFloat("specularPower", meshMaterials[i].Power);
				m_pEffect->SetTexture("tex", m_pLaserTexture[0]);
				m_pEffect->SetBool("isTex", true);
				// After setting object specific parameters, commit changes
				m_pEffect->CommitChanges();
				// Draw the mesh subset
				m_pLaserMesh->DrawSubset(i);
			}
		}
		//End Pass
		m_pEffect->EndPass();
	}

	///////////////////////////////////////////////
	//		DISPLAY GRAPHIC STAT TEXT			//
	//////////////////////////////////////////////
	//************************************************************************
	static char buffer[256];
	if(!m_bIsGameOver)
	{
	//Make static memory so we do not allocate the memory every time the function is called
	rect.top = height * 0.05;
	rect.left = width * 0.10;
	//set sprintf to store the buffer size, format, and output)
	sprintf(buffer, "\nEnemy HP = %d \tShip HP = %d", 
		 Bk.Health, PS.Health);
	m_pD3DFont->DrawTextA(0, buffer, -1, &rect, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 0, 0));
	}
	if(m_bIsGameOver)
	{
		rect.top = height * 0.5;
		rect.left = width * 0.5;
		if(PS.Health != 0)
		{
			sprintf(buffer, "Game over! You win! \nPress M to \nreturn to Menu");
			m_pD3DFont->DrawTextA(0, buffer, -1, &rect, DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 255, 0));
		}
		if(PS.Health == 0)
		{
			sprintf(buffer, "Game Over! You lose! \nPress M to \nreturn to Menu");
			m_pD3DFont->DrawTextA(0, buffer, -1, &rect, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 0, 0));
		}
		
	}
	///////////////////////
	//Set Text For Display 
	//////////////////////
	//Draw display text here
	
	m_pEffect->End();
	m_pD3DDevice->EndScene();
	m_pD3DDevice->Present(0, 0, 0, 0);
}

void PlayingField::SetWidth(float width)
{
	mpf_fWidth = width;
}

void PlayingField::SetHeight(float height)
{
	mpf_fWidth = height;
}

void PlayingField::SetCenter(float x, float y)
{
	mpf_fCenterX = x;
	mpf_fCenterY = y;
}

void PlayingField::SetMaxHeight(float maxHeight)
{
	MAX_Y = maxHeight;
}

void PlayingField::SetMinHeight(float minHeight)
{
	MIN_Y = minHeight;
}

void PlayingField::SetMaxWidth(float maxWidth)
{
	MAX_X = maxWidth;
}

void PlayingField::SetMinWidth(float minWidth)
{
	MIN_X = minWidth;
}

float PlayingField::GetWidth()
{
	return mpf_fWidth;
}

float PlayingField::GetHeight()
{
	return mpf_fHeight;
}

float PlayingField::GetCenterX()
{
	return mpf_fCenterX;
}

float PlayingField::GetCenterY()
{
	return mpf_fCenterY;
}

float PlayingField::GetMaxHeight()
{
	return MAX_Y;
}

float PlayingField::GetMinHeight()
{
	return MIN_Y;
}

float PlayingField::GetMaxWidth()
{
	return MAX_X;
}

float PlayingField::GetMinWidth()
{
	return MIN_X;
}

void PlayingField::ShootLaser(float x, float y, float z)
{
	Lsr.PosX = x;
	Lsr.PosY = y;
	Lsr.PosZ = z;
}

void PlayingField::EShootLaser(float x, float y, float z)
{
	BlockLsr.PosX = x;
	BlockLsr.PosY = y;
	BlockLsr.PosZ = z;
}
float PlayingField::calcDistance(float x1, float y1, float z1, float x2, float y2, float z2)
{
	term1 = pow((x2 - x1), 2);
	term2 = pow((y2 - y1), 2);
	term3 = pow((z2 - z1), 2);

	distance = sqrt(term1 + term2 + term3);
	return distance;
}

float PlayingField::calcBoxCollision(float x1, float y1, float z1, float x2, float y2, float z2)
{
	float width = abs((x2 - x1) * 0.5);
	float height = abs((y2 - y1) * 0.5);
	float depth = abs((z2 - z1) * 0.5);

	float distance = (width + height + depth);

	return distance;
}

float PlayingField::calcX(float x1, float x2)
{
	float distance = abs(x2 - x1);
	return distance;
}

float PlayingField::calcY(float y1, float y2)
{
	float distance = abs(y2 - y1);
	return distance;
}

float PlayingField::calcZ(float z1, float z2)
{
	float distance = abs(z2 - z1);
	return distance;
}

bool PlayingField::GetGameOver()
{
	return m_bIsGameOver;
}

bool PlayingField::GetMenuBool()
{
	return ReturnToMenu;
}
