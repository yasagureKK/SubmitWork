//=============================================================================
// メイン処理 [main.cpp]
// Author : 草刈 翔
//=============================================================================
#include "main.h"
#include "manager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME		"AppClass"				// ウインドウのクラス名
#define WINDOW_NAME		"うさぎジャンピング！"	// ウインドウのキャプション名(ゲーム名)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//*****************************************************************************
// グローバル変数:
//*****************************************************************************
#ifdef _DEBUG
int	g_nCountFPS;	// FPSカウンタ
#endif

//=============================================================================
// メイン関数
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// 構造体の設定
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};

	// ウィンドウのサイズはここで変更
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	HWND hWnd;				//ウィンドウハンドル(識別子)
	MSG msg;				//メッセージを格納する変数
	DWORD dwCurrentTime;	//現在時刻
	DWORD dwFrameCount;		//フレームカウント
	DWORD dwExecLastTime;	//処理実行最終時刻
	DWORD dwFPSLastTime;	//FPS計画最終時刻

							// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// 指定したクライアント領域を確保するために必要なウィンドウ座標を計算
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウの作成
	hWnd = CreateWindow(CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(rect.right - rect.left),
		(rect.bottom - rect.top),
		NULL,
		NULL,
		hInstance,
		NULL);

	// マネージャークラスの宣言
	CManager *pManager = NULL;

	// ヌルチェック
	if (pManager == NULL)
	{
		// マネージャークラスの生成
		pManager = new CManager();
	}

	// マネージャークラスの初期化(true…通常ウィンドウ  false…全画面表示)
	if (FAILED(pManager->Init(hInstance, hWnd, true)))
	{
		return -1;
	}

	// 分解能を設定
	timeBeginPeriod(1);

	// フレームカウント初期化
	dwCurrentTime = dwFrameCount = 0;
	dwExecLastTime = dwFPSLastTime = timeGetTime();

	// ウインドウの表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			if (msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();	// 現在の時間を取得
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{// 0.5秒ごとに実行
#ifdef _DEBUG
			 // FPSを算出
				g_nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
#endif
				// 現在の時間を保存
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}

			// 1/60秒経過
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				// 現在の時間を保存
				dwExecLastTime = dwCurrentTime;

				// マネージャークラスの更新
				pManager->Update();

				// マネージャークラスの描画
				pManager->Draw();

				dwFrameCount++;
			}
		}
	}

	// マネージャークラスの破棄
	if (pManager != NULL)
	{
		// 終了処理
		pManager->Uninit();

		// メモリの開放
		delete pManager;
		pManager = NULL;
	}

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 分解能を戻す
	timeEndPeriod(1);

	return (int)msg.wParam;
}

//=============================================================================
// ウインドウプロシージャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		break;

		// ウィンドウ破壊メッセージ
	case WM_DESTROY:
		//WM_QUITメッセージを返す
		PostQuitMessage(0);
		break;

		// キーを押したときに閉じる
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:				// [ESC]キーが押された
			DestroyWindow(hWnd);	// ウィンドウを破棄するよう指示する
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=============================================================================
// FPSの取得処理
//=============================================================================
#ifdef _DEBUG
int GetCntFPS(void)
{
	return g_nCountFPS;
}
#endif