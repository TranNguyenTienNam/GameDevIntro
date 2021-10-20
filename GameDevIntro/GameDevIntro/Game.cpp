#include "Game.h"
#include "Animations.h"
#include "Utils.h"
#include "InputHandler.h"
#include "Scenes.h"

CGame* CGame::instance = NULL;
DWORD CGame::deltaTime = 0;

CGame* CGame::GetInstance()
{
	if (instance == NULL) instance = new CGame();
	return instance;
}

void CGame::InitDirectX(HWND hWnd)
{
	LPDIRECT3D9 d3d = Direct3DCreate9(D3D_SDK_VERSION);

	this->hWnd = hWnd;

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = NULL;

	// retrieve window width & height so that we can create backbuffer height & width accordingly 
	RECT r;
	GetClientRect(hWnd, &r);

	screen_width = r.right + 1;
	screen_height = r.bottom + 1;

	d3dpp.BackBufferHeight = screen_height;
	d3dpp.BackBufferWidth = screen_width;

	d3d->CreateDevice(
		D3DADAPTER_DEFAULT,			// use default video card in the system, some systems have more than one video cards
		D3DDEVTYPE_HAL,				// HAL = Hardware Abstraction Layer - a "thin" software layer to allow application to directly interact with video card hardware
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddv);

	if (d3ddv == NULL)
	{
		DebugOut(L"[ERROR] CreateDevice failed\n");
		return;
	}

	d3ddv->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);

	// Initialize sprite handler from Direct3DX helper library
	D3DXCreateSprite(d3ddv, &spriteHandler);
	
	if (spriteHandler == NULL)
	{
		DebugOut(L"[ERROR] Create spriteHandler failed\n");
		return;
	}

	DebugOut(L"[INFO] InitDirectX OK\n");
}

void CGame::Draw(Vector2 position, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha)
{
	Vector3 p(floor(position.x - cam_pos.x), floor(position.y - cam_pos.y), 0);
	RECT r;
	r.left = left;
	r.top = top;
	r.right = right;
	r.bottom = bottom;
	spriteHandler->Draw(texture, &r, NULL, &p, D3DCOLOR_ARGB(alpha, 255, 255, 255));
}

LPDIRECT3DTEXTURE9 CGame::LoadTexture(LPCWSTR texturePath, D3DCOLOR transparentColor)
{
	LPDIRECT3DTEXTURE9 texture;

	LPDIRECT3DDEVICE9 d3ddv = CGame::GetInstance()->GetDirect3DDevice();

	HRESULT result = D3DXCreateTextureFromFileEx(
		d3ddv,								// Pointer to Direct3D device object
		texturePath,						// Path to the image to load
		D3DX_DEFAULT_NONPOW2, 				// Auto texture width (get from file)
		D3DX_DEFAULT_NONPOW2, 				// Auto texture height (get from file)
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		transparentColor,		// Transparent color
		NULL,
		NULL,
		&texture);							// Created texture pointer

	if (result != D3D_OK)
	{
		DebugOut(L"[ERROR] CreateTextureFromFileEx %s failed\n", texturePath);
		return NULL;
	}

	DebugOut(L"[INFO] Texture loaded Ok: %s \n", texturePath);
	return texture;
}

void CGame::Update(DWORD dt)
{
	CGame::GetInstance()->GetService<CScenes>()->GetCurrentScene()->Update(dt);
}

void CGame::Render()
{
	if (d3ddv->BeginScene() == D3D_OK)
	{
		// Clear the whole window with a color
		d3ddv->ColorFill(GetBackBuffer(), NULL, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		CGame::GetInstance()->GetService<CScenes>()->GetCurrentScene()->Render();

		spriteHandler->End();
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

void CGame::GameInit(HWND hWnd)
{
	InitDirectX(hWnd);

	CGame* game = CGame::GetInstance();

	game->AddService(new CTextures);
	game->AddService(new CSprites);
	game->AddService(new CAnimations);
	game->AddService(new CInputHandler);
	game->GetService<CInputHandler>()->SetHandleWindow(hWnd);
	game->GetService<CInputHandler>()->Initialize();
	game->AddService(new CScenes);
	game->GetService<CScenes>()->Load(L"database\\mario-sample.txt");
}

void CGame::GameRun()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		deltaTime = now - frameStart;

		if (deltaTime >= tickPerFrame)
		{
			frameStart = now;

			CGame::GetInstance()->GetService<CInputHandler>()->ProcessKeyboard();

			Update(deltaTime);
			Render();
		}
		else
			Sleep(tickPerFrame - deltaTime);
	}
}

void CGame::GameEnd()
{
	if (spriteHandler != NULL) spriteHandler->Release();
	if (backBuffer != NULL)backBuffer->Release();
	if (d3ddv != NULL)d3ddv->Release();
	if (d3d != NULL) d3d->Release();
	DebugOut(L"[INFO] Cleanup Ok\n");
}