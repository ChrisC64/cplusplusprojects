//////////////////////////////////////////////////////////////////////////
// Name:	WinMain.h
// Date:	April 2nd, 2010
// Author:	Kyle Lauing [klauing@devry.edu] or [kylelauing@gmail.com]
// Purpose: This file is used to create a simple window application for
//			use with the DirectXFramework developed for the GSP 381 course
//			for DeVry University.
// Disclaimer:	
//			Copyright � 2010 by DeVry Educational Development Corporation.
//			All rights reserved.  No part of this work may be reproduced 
//			or used in any form or by any means � graphic, electronic, or 
//			mechanical, including photocopying, recording, Web distribution 
//			or information storage and retrieval systems � without the 
//			prior consent of DeVry Educational Development Corporation.
//////////////////////////////////////////////////////////////////////////
#include <iostream>
using namespace std;
#include <windows.h>
#define VC_EXTRALEAN

#include "DirectXFramework.h"
#include "System.h"
#include "Menu.h"
#include "GameFramework.h"
#include "FModSound.h"
#include "PlayingField.h"
#include "PlayerShip.h"

//////////////////////////////////////////////////////////////////////////
// Global Variables
//////////////////////////////////////////////////////////////////////////
#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 800
#define WINDOW_TITLE L"Block Shooter - Christopher Carlos"

HWND				g_hWnd;			// Handle to the window
HINSTANCE			g_hInstance;	// Handle to the application instance
bool				g_bWindowed;	// Boolean for windowed or full-screen
bool				g_bIsEnabled = false;	// Boolean for game init()
//*************************************************************************

System			 SysClass;
DirectXFramework DXObj;
Menu			 MenuObj;
GameFramework	 GFObj;
Sound			 SoundClass;
PlayingField	 PFObj;
PlayerShip		 PSObj;
//*************************************************************************

// Entry point for the game or application.
int WINAPI wWinMain(HINSTANCE hInstance,	// Handle to the application
				   HINSTANCE hPrevInstance,	// Handle to the previous app
				   LPTSTR lpCmdLine,		// Command line string
				   int lpCmdShow);			// Show window value

LRESULT CALLBACK WndProc(HWND hWnd,			// Handle to the window
						 UINT message,		// Incoming Message
						 WPARAM wparam,		// Message Info
						 LPARAM lparam);	// Message Info

void InitWindow(void)
{
	WNDCLASSEX wndClass;  
	ZeroMemory(&wndClass, sizeof(wndClass));

	// set up the window
	wndClass.cbSize			= sizeof(WNDCLASSEX);			// size of window structure
	wndClass.lpfnWndProc	= (WNDPROC)WndProc;				// message callback
	wndClass.lpszClassName	= WINDOW_TITLE;					// class name
	wndClass.hInstance		= g_hInstance;					// handle to the application
	wndClass.hCursor		= LoadCursor(NULL, IDC_ARROW);	// default cursor
	wndClass.hbrBackground	= (HBRUSH)(COLOR_WINDOWFRAME);	// background brush

	// register a new type of window
	RegisterClassEx(&wndClass);

	g_hWnd = CreateWindow(
		WINDOW_TITLE, WINDOW_TITLE, 							// window class name and title
		g_bWindowed ? WS_OVERLAPPEDWINDOW | WS_VISIBLE:(WS_POPUP | WS_VISIBLE),// window style
		CW_USEDEFAULT, CW_USEDEFAULT,							// x and y coordinates
		SCREEN_WIDTH, SCREEN_HEIGHT,							// width and height of window
		NULL, NULL,												// parent window and menu
		g_hInstance,											// handle to application
		NULL);

	// Display the window
	ShowWindow(g_hWnd, SW_SHOW);
	UpdateWindow(g_hWnd);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
	g_hInstance = hInstance;	// Store application handle
	g_bWindowed = true;			// Windowed mode or full-screen

	// Init the window
	InitWindow();

	// Use this msg structure to catch window messages
	MSG msg; 
	ZeroMemory(&msg, sizeof(msg));
	//perform the count for frames per second
	_int64 cntsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);//perform function QueryPerformanceFrequency: returns a BOOL value; true if timer exist
	float secsPerCnt = 1.0f / (float)cntsPerSec; //typecast the long int into a float
	//create an int for prev time
	_int64 prevTimeStamp = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&prevTimeStamp);

	//*************************************************************************
	// Initialize DirectX/Game here (call the Init method of your framwork)
	DXObj.Init(g_hWnd, g_hInstance, g_bWindowed);
	MenuObj.Init(g_hWnd, g_hInstance, g_bWindowed);
	//*************************************************************************

	// Main Windows/Game Loop
	while(msg.message != WM_QUIT)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//begin current time stamp counter
		_int64 currTimeStamp = 0;
		//receive counter performance
		QueryPerformanceCounter((LARGE_INTEGER*)&currTimeStamp);
		float dt = (currTimeStamp - prevTimeStamp)*secsPerCnt;
		if(MenuObj.GetState() == 0) // Intro movie
		{
				DXObj.Update(dt);
				if(DXObj.GetMovieState() == true)
				{
					MenuObj.SetState(1);
				}
		}
		if(MenuObj.GetState() == 1)
		{
				//menu
				MenuObj.Update();
				MenuObj.Draw();
		}
		if(MenuObj.GetState() == 2)
		{
				//options
			MenuObj.Update();
			MenuObj.Draw();
		}
		if(MenuObj.GetState() == 3)
		{
				//game
			if(!g_bIsEnabled)
			{
				PFObj.Init(g_hWnd, g_hInstance, g_bWindowed);
				g_bIsEnabled = true;
			}
				PFObj.Update(dt);
				PFObj.Draw();
				if(PFObj.GetMenuBool())
				{
					MenuObj.SetState(1);
				}
		}
		if(MenuObj.GetState() == 4)
		{
				//credits
			MenuObj.Update();
			MenuObj.Draw();
		}
		if(MenuObj.GetState() == 5)
		{
				//quit game
				DXObj.Shutdown();
				PostQuitMessage(0);
		}
		//*************************************************************************
		// This is where you call your DirectXFramework/Game render/update calls
		
		//MenuObj.Draw();
		//PSObj.Draw();
		//*************************************************************************
		//Prepare for the next iteration: The current time
		//stamp becomes the previous time stamp for the next iteration
		
		prevTimeStamp = currTimeStamp; 

	}

	//*************************************************************************
	//Shutdown DirectXFramework/Game here
	DXObj.Shutdown();

	//*************************************************************************

	// Unregister window
	UnregisterClass(WINDOW_TITLE, g_hInstance);

	// Return successful
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	// attempt to handle your messages
	switch(message)
	{
		case (WM_PAINT):
		{
			InvalidateRect(hWnd,NULL,TRUE);
			break;
		}		
		case(WM_DESTROY):
		{
			PostQuitMessage(0); 
			break;
		}
		case(WM_KEYDOWN):
		{
			switch(wparam)
			{
				// Escape Key will close the application
				// Remove/Modify this call when creating a real game with menus, etc.
				case VK_ESCAPE:
				{
					
				}
			}
		}
	}

	// pass to default handler
	return DefWindowProc(hWnd, message, wparam, lparam);
}
