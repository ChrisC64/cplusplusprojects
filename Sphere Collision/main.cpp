/*
main.cpp

OpenGL conservation of energy demo
*/

#include <windows.h>        // must include this for windows programs
#include <stdio.h>
#include "resource.h"

#include <gl/gl.h>
#include <gl/glu.h>

#include "CGfxOpenGL.h"
#include "gmath.h"

// macro to allow for asynchronous input detection
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

// Global variables to store dialog and picture box window handles
HWND HOutput = NULL;  // handle to the output control
HWND HSample = NULL;
HWND HDialog = NULL;  // handle to the main dialog window
bool Exiting = false;
long WindowWidth;
long WindowHeight;
long WindowBits = 32;
bool Fullscreen = false;
CGfxOpenGL *g_glRender = NULL;
long DT = 20;               // 20 millisecond timer

bool Wireframe = true;

// current camera settings
int PosX = 0;
int PosY = 30;                       //<--------modofy this to be 30
int PosZ = 50;
int LookX = 0;
int LookY = 0;
int LookZ = 0;
int TransOffset = 50;
int Roll = 0;
int FOV = 45;  // range 20..100??

float BallRadius = 1.0f;

int UserBallIndex = 0;
const int NumBalls = 4;
Vector3 BArrayPos[NumBalls];
Vector3 BArrayVel[NumBalls];
float BArrayColors[NumBalls][3] = 
{
	{ 1.0f, 1.0f, 1.0f },
	{ 1.0f, 0.0f, 0.0f },
	{ 1.0f, 0.0f, 1.0f },
	{ 1.0f, 1.0f, 0.0f }
};


void SetupPixelFormat(HDC hDC)
{
    int pixelFormat;

    PIXELFORMATDESCRIPTOR pfd =
    {   
        sizeof(PIXELFORMATDESCRIPTOR),  // size
            1,                          // version
            PFD_SUPPORT_OPENGL |        // OpenGL window
            PFD_DRAW_TO_WINDOW |        // render to window
            PFD_DOUBLEBUFFER,           // support double-buffering
            PFD_TYPE_RGBA,              // color type
            32,                         // prefered color depth
            0, 0, 0, 0, 0, 0,           // color bits (ignored)
            0,                          // no alpha buffer
            0,                          // alpha bits (ignored)
            0,                          // no accumulation buffer
            0, 0, 0, 0,                 // accum bits (ignored)
            16,                         // depth buffer
            0,                          // no stencil buffer
            0,                          // no auxiliary buffers
            PFD_MAIN_PLANE,             // main layer
            0,                          // reserved
            0, 0, 0,                    // no layer, visible, damage masks
    };

    pixelFormat = ChoosePixelFormat(hDC, &pfd);
    SetPixelFormat(hDC, pixelFormat, &pfd);
}

// given 2 sphere centers and radii
// if a collision has occured
//    will return true
//    and update the velocities of each spere using conservation of momentum
//    and coefficient of elasticity E
// false otherwise
bool SphereCollision(Vector3 p1, float r1, Vector3* v1, float m1,
					 Vector3 p2, float r2, Vector3* v2, float m2,
					 float E) {
	Vector3 temp;

	temp = Subtract(p2, p1); //Here, p1 and p2 are the center of each sphere

//Add the proper sphere-sphere collision detection here in this one line of the if statement:
	if (temp.length() <= r1 + r2 ) 
	{

		// collision has occured so update v1, v2
		float v1i, v2i, v1f, v2f;

		v1i = v1->length();
		v2i = v2->length();

//modify these two lines of code so that the correct final speed formulas are used:
//hint: look at the last set of formulas from the Week 6 lecture.
		v2f = (m1*v1i + m2*v2i + m1*E*v1i - m1*E*v2i) / (m1 + m2);
		v1f = v2f + E * (v1i - v2i); 

		// set the direction of the velocities away from the collision
		// temp is already in p1 -> p2 direction from above
		// NOTE: this does not correctly handle off-center collisions
		// to do this we need to break down velocities into their components
		// along the axis of collision (p1 - p2) and perpendicular to it
		// see the section on Two-dimensional collisions at 
		// http://www.answers.com/topic/elastic-collision
		temp.normalize();
		v2->set(temp);
		v2->scale(v2f);

		temp = Subtract(p1, p2);
		temp.normalize();
		v1->set(temp);
		v1->scale(v1f);

		return true;
	}
	else 
	{
		return false;
	}
}

/*
DialogProc
this is the window event handler for the main dialog
*/
BOOL CALLBACK DialogProc (HWND hwnd, 
                          UINT message, 
                          WPARAM wParam, 
                          LPARAM lParam)
{
    static HDC hDC;
    static HGLRC hRC;
    HWND hCtrl;
    int ctrlID;
    int pos, min, max;
    RECT rect;
    HBRUSH brush;
    int x, y;

    // switch based on message type
    switch(message)
    {

    case WM_LBUTTONDOWN:
            // get the mouse position
            x = LOWORD(lParam);
            y = HIWORD(lParam);


        break;

    case WM_INITDIALOG:
        // dialog is initializing – 
        // store the picture box handle in a global variable for later 
        HOutput = GetDlgItem(hwnd, IDC_PICTURE_OUTPUT);        
		hDC = GetDC(HOutput);

        // initialize the rgb and hsv scroll bars
        hCtrl = GetDlgItem(hwnd, IDC_SCROLLBAR_POSX);
        SetScrollRange(hCtrl, SB_CTL, 0, 100, true);
        SetScrollPos(hCtrl, SB_CTL, PosX + TransOffset, true);
		SetDlgItemInt(hwnd, IDC_STATIC_POSX, PosX, true);

        hCtrl = GetDlgItem(hwnd, IDC_SCROLLBAR_POSY);
        SetScrollRange(hCtrl, SB_CTL, 0, 100, true);
        SetScrollPos(hCtrl, SB_CTL, PosY + TransOffset, true);
		SetDlgItemInt(hwnd, IDC_STATIC_POSY, PosY, true);

        hCtrl = GetDlgItem(hwnd, IDC_SCROLLBAR_POSZ);
        SetScrollRange(hCtrl, SB_CTL, 1, 100, true);
        SetScrollPos(hCtrl, SB_CTL, PosZ, true);
		SetDlgItemInt(hwnd, IDC_STATIC_POSZ, PosZ, true);

        hCtrl = GetDlgItem(hwnd, IDC_SCROLLBAR_LOOKX);
        SetScrollRange(hCtrl, SB_CTL, 0, 100, true);
        SetScrollPos(hCtrl, SB_CTL, LookX + TransOffset, true);
		SetDlgItemInt(hwnd, IDC_STATIC_LOOKX, LookX, true);

        hCtrl = GetDlgItem(hwnd, IDC_SCROLLBAR_LOOKY);
        SetScrollRange(hCtrl, SB_CTL, 0, 100, true);
        SetScrollPos(hCtrl, SB_CTL, LookY + TransOffset, true);
		SetDlgItemInt(hwnd, IDC_STATIC_LOOKY, LookY, true);

        hCtrl = GetDlgItem(hwnd, IDC_SCROLLBAR_LOOKZ);
        SetScrollRange(hCtrl, SB_CTL, 0, 100, true);
        SetScrollPos(hCtrl, SB_CTL, LookZ + TransOffset, true);
		SetDlgItemInt(hwnd, IDC_STATIC_LOOKZ, LookZ, true);

		hCtrl = GetDlgItem(hwnd, IDC_SCROLLBAR_ROLL);
        SetScrollRange(hCtrl, SB_CTL, 0, 360, true);
        SetScrollPos(hCtrl, SB_CTL, Roll, true);
		SetDlgItemInt(hwnd, IDC_STATIC_ROLL, Roll, true);

		hCtrl = GetDlgItem(hwnd, IDC_SCROLLBAR_FOV);
        SetScrollRange(hCtrl, SB_CTL, 10, 120, true);
        SetScrollPos(hCtrl, SB_CTL, FOV, true);
		SetDlgItemInt(hwnd, IDC_STATIC_FOV, FOV, true);

        // set up the opengl render window
        SetupPixelFormat(hDC);
        hRC = wglCreateContext(hDC);
        wglMakeCurrent(hDC, hRC);

        break;

    case WM_TIMER:
		{
			Vector3 ballAccel;
			Vector3 groundPt;
			Vector3 temp;
			Vector3 normal;					// surface normal
			int i, j;
			char str[64];

		
			for (i=0; i < NumBalls-1; i++)
			{
				for (j=i+1; j < NumBalls; j++) 
				{
					if (SphereCollision(BArrayPos[i], BallRadius, &BArrayVel[i], 1,
						                BArrayPos[j], BallRadius, &BArrayVel[j], 1,
										1) ) 
					{
						// SphereCollision updates velocities if they collided
					    // maybe add a sound effect later
					}
				}
			}

			for (i=0; i < NumBalls; i++) 
			{

				// get the position of the ground below the ball
				g_glRender->pTerrain->GetTerrainPoint(&groundPt, BArrayPos[i].x, BArrayPos[i].z);





				// This determines if ball has collided with the terrain boundary:
				if (g_glRender->pTerrain->CheckBoundaryCollision(&normal, BArrayPos[i]) ) 
				{

					BArrayVel[i].makeReflectionVector(BArrayVel[i], normal);


// Treat the ball's velocity,BArrayVel[i], as the incident vector
// and reflect this about the boundary normal, called normal.
//Use the makeReflectionVector method
//See the Week 6 iLab for the single line of code that you will need to put here:




					//This makes the ball lose some energy due to the collision, multiplies velocity by .8
					BArrayVel[i].scale(0.8f);




				}


				// slow the ball due to rolling friction
				ballAccel.set(BArrayVel[i]);
				ballAccel.scale(-.05f);

				// update ball velocity
				temp.set(ballAccel);
				temp.scale(DT / 1000.0);
				BArrayVel[i] = Add(BArrayVel[i], temp);

				// update ball position
				temp.set(BArrayVel[i]);
				temp.scale(DT / 1000.0);
				BArrayPos[i] = Add(BArrayPos[i], temp);

				// get the position of the ground below the ball
				g_glRender->pTerrain->GetTerrainPoint(&groundPt, BArrayPos[i].x, BArrayPos[i].z);

				// if the new ball pos is out of bounds or below the terrain
				// reposition it
				// make the test position in vertical y direction same as lower/contact point
				temp.set(BArrayPos[i]);
				temp.y -= BallRadius;
		
                BArrayPos[i] = Add(BArrayPos[i], g_glRender->pTerrain->GetCorrection(temp)); 
			}

   		    sprintf(str, "%s pos=<%.2f %.2f %.2f> vel=<%.2f %.2f %.2f>",
			    "Chris Carlos",
				BArrayPos[UserBallIndex].x, BArrayPos[UserBallIndex].y, BArrayPos[UserBallIndex].z,
				BArrayVel[UserBallIndex].x, BArrayVel[UserBallIndex].y, BArrayVel[UserBallIndex].z);
		    SetWindowText(HDialog, str);


		}
        break;

    case WM_DESTROY:            // window destroy
    case WM_QUIT:
    case WM_CLOSE:                  // windows is closing

        // deselect rendering context and delete it
        wglMakeCurrent(hDC, NULL);
        wglDeleteContext(hRC);

        // send WM_QUIT to message queue
        PostQuitMessage(0);
        break;


    case WM_HSCROLL:
        // one of the scroll bars has moved

        // update the scroll bars
        hCtrl = (HWND)lParam;
        ctrlID = GetWindowLong(hCtrl, GWL_ID);
        pos = GetScrollPos(hCtrl, SB_CTL);
        GetScrollRange(hCtrl, SB_CTL, &min, &max);
        switch(LOWORD(wParam)) {
            case SB_LINEUP:
            case SB_PAGEUP:
                pos -= 1;
                pos = MAX(pos, min);
            break;
            case SB_LINEDOWN:
            case SB_PAGEDOWN:
                pos += 1;
                pos = MIN(pos, max);
            break;
            case SB_THUMBPOSITION:
            case SB_THUMBTRACK:
                pos = HIWORD(wParam);
            break;
        }

        SetScrollPos(hCtrl, SB_CTL, pos, true);
        switch(ctrlID) {
            case IDC_SCROLLBAR_DX:
			    PosX = GetScrollPos(hCtrl, SB_CTL);
				PosX -= TransOffset;
				SetDlgItemInt(hwnd, IDC_STATIC_POSX, PosX, true);
                break;
            case IDC_SCROLLBAR_DY:
			    PosY = GetScrollPos(hCtrl, SB_CTL);
				PosY -= TransOffset;
				SetDlgItemInt(hwnd, IDC_STATIC_POSY, PosY, true);
                break;
            case IDC_SCROLLBAR_DZ:
			    PosZ = GetScrollPos(hCtrl, SB_CTL);
				SetDlgItemInt(hwnd, IDC_STATIC_POSZ, PosZ, true);
                break;
            case IDC_SCROLLBAR_LOOKX:
			    LookX = GetScrollPos(hCtrl, SB_CTL);
				LookX -= TransOffset;
				SetDlgItemInt(hwnd, IDC_STATIC_LOOKX, LookX, true);
                break;
            case IDC_SCROLLBAR_LOOKY:
			    LookY = GetScrollPos(hCtrl, SB_CTL);
				LookY -= TransOffset;
				SetDlgItemInt(hwnd, IDC_STATIC_LOOKY, LookY, true);
                break;
            case IDC_SCROLLBAR_LOOKZ:
			    LookZ = GetScrollPos(hCtrl, SB_CTL);
				LookZ -= TransOffset;
				SetDlgItemInt(hwnd, IDC_STATIC_LOOKZ, LookZ, true);
                break;
            case IDC_SCROLLBAR_ROLL:
			    Roll = GetScrollPos(hCtrl, SB_CTL);
				SetDlgItemInt(hwnd, IDC_STATIC_ROLL, Roll, true);
                break;
            case IDC_SCROLLBAR_FOV:
			    FOV = GetScrollPos(hCtrl, SB_CTL);
				SetDlgItemInt(hwnd, IDC_STATIC_SCALE, FOV, true);
                break;
        }

        // render opengl stuff
		g_glRender->Prepare(0.0f);
        g_glRender->Render();
        SwapBuffers(hDC);
        break;

    }
    return FALSE;
}

// this is the main function that starts the application
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, char * cmdParam, int cmdShow)
{
	HDC hDC;
	RECT rect;
    Vector3 cannonDirection;
    char str[32];

    g_glRender = new CGfxOpenGL;

    // create the main window
    // store its handle in a global if needed
    HDialog = CreateDialog (GetModuleHandle(NULL), 
        MAKEINTRESOURCE(IDD_DIALOG1), 
        0, 
        DialogProc);

    // make the dialog visible
    ShowWindow(HDialog, SW_SHOW);

    // set up a timer
    SetTimer(HDialog, 1, DT, NULL);

    // remove keyboard focus so arrow keys won't move sliders
    SetFocus(NULL);

    hDC = GetDC(HOutput);

    g_glRender->Init();

	// init the terrain
	Vector3 v0, v1, v2, v3;
	v0.set(-20, 10,  20);
	v1.set( 20, 10,  20);
	v2.set( 20, 10, -20);
	v3.set(-20, 10, -20);
	g_glRender->pTerrain = new Terrain(20, 20, &v0, &v1, &v2, &v3);


	// init ball array positions & velocities
	BArrayPos[0].set(10, 0, 0);
	BArrayVel[0].set(0, 0, 0);
	BArrayPos[0] = Add(BArrayPos[0], g_glRender->pTerrain->GetCorrection(BArrayPos[0]));
	BArrayPos[0].y += BallRadius;

	BArrayPos[1].set(10, 0, 10);
	BArrayVel[1].set(0, 0, 0);
	BArrayPos[1] = Add(BArrayPos[1], g_glRender->pTerrain->GetCorrection(BArrayPos[1]));
	BArrayPos[1].y += BallRadius;

	BArrayPos[2].set(15, 0, -10);
	BArrayVel[2].set(0, 0, 0);
	BArrayPos[2] = Add(BArrayPos[2], g_glRender->pTerrain->GetCorrection(BArrayPos[2]));
	BArrayPos[2].y += BallRadius;

	BArrayPos[3].set(5, 10, -5);
	BArrayVel[3].set(0, 0, 0);
	BArrayPos[3] = Add(BArrayPos[3], g_glRender->pTerrain->GetCorrection(BArrayPos[3]));
	BArrayPos[3].y += BallRadius;

	GetClientRect(HOutput, &rect);
    WindowWidth = rect.right - rect.left;
    WindowHeight = rect.bottom - rect.top;

    g_glRender->SetupProjection(WindowWidth, WindowHeight);


    // standard windows message loop
    MSG  msg;
    while (!Exiting)
    {
        // render opengl stuff
		g_glRender->Prepare(0.0f);
        g_glRender->Render();
        SwapBuffers(hDC);

        // check for input
        if (KEY_DOWN(VK_RIGHT)) {
			BArrayVel[UserBallIndex].x += 0.05f;
        }
        if (KEY_DOWN(VK_LEFT)) {
			BArrayVel[UserBallIndex].x += -0.05f;
        }
		
        if (KEY_DOWN(VK_UP)) {
			BArrayVel[UserBallIndex].z += -0.05f;

        }

        if (KEY_DOWN(VK_DOWN)) {
			BArrayVel[UserBallIndex].z += 0.05f;
        }


        if (KEY_DOWN(VK_SPACE)) {
        }

        // handle window message stuff
        if (PeekMessage (&msg, NULL, 0, 0, PM_NOREMOVE))
        {
            if (!GetMessage (&msg, NULL, 0, 0))
            {
                Exiting = true;
                break;
            }

			// avoid processing messages for the dialog
			if (!IsDialogMessage (HDialog, & msg))
			{
				TranslateMessage ( & msg );
				DispatchMessage ( & msg );
			}
        }
    }

    return (int)(msg.wParam);
}