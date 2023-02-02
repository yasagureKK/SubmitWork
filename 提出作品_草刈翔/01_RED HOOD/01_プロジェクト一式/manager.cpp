//=============================================================================
// マネージャー [manager.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "title.h"
#include "game.h"
#include "keyboard.h"
#include "gamepad.h"
#include "mouse.h"
#include "texture.h"
#include "model_load.h"
#include "camera.h"
#include "light.h"
#include "fade.h"
#include "sound.h"
#include "data_attack.h"
#include "data_clear.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CManager::MODE CManager::m_mode = MODE_TITLE;
CTitle *CManager::m_pTitle = NULL;
CGame *CManager::m_pGame = NULL;
CRenderer *CManager::m_pRenderer = NULL;
CKeyboard *CManager::m_pKeyboard = NULL;
CGamePad *CManager::m_pGamePad = NULL;
CMouse *CManager::m_pMouse = NULL;
CTexture *CManager::m_pTexture = NULL;
CModelLoad *CManager::m_pModelLoad = NULL;
CCamera *CManager::m_pCamera = NULL;
CFade *CManager::m_pFade = NULL;
CSound *CManager::m_pSound = NULL;
CDataAttack *CManager::m_pDataAtk = NULL;
CDataClear *CManager::m_pDataClear = NULL;
bool CManager::m_bPause = false;

//=============================================================================
// コンストラクタ
//=============================================================================
CManager::CManager()
{
	// 変数のクリア
	m_pCamera = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CManager::~CManager()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	// レンダラーの生成
	if (m_pRenderer == NULL)
	{
		m_pRenderer = new CRenderer;
		if (m_pRenderer != NULL)
		{
			m_pRenderer->Init(hInstance, hWnd, bWindow);
		}
	}

	// キーボードの生成
	if (m_pKeyboard == NULL)
	{
		m_pKeyboard = new CKeyboard;
		if (m_pKeyboard != NULL)
		{
			m_pKeyboard->Init(hInstance, hWnd);
		}
	}

	// ゲームパッドの生成
	if (m_pGamePad == NULL)
	{
		m_pGamePad = new CGamePad;
		if (m_pGamePad != NULL)
		{
			m_pGamePad->Init(hInstance, hWnd);
		}
	}

	// マウスの生成
	if (m_pMouse == NULL)
	{
		m_pMouse = new CMouse;
		if (m_pMouse != NULL)
		{
			m_pMouse->Init(hInstance, hWnd);
		}
	}

	// サウンドの生成
	if (m_pSound == NULL)
	{
		m_pSound = new CSound;
		if (m_pSound != NULL)
		{
			m_pSound->Init(hWnd);
		}
	}

	// テクスチャの生成
	if (m_pTexture == NULL)
	{
		m_pTexture = new CTexture;
		if (m_pTexture != NULL)
		{
			m_pTexture->Init();
		}
	}

	// モデルロードの生成
	if (m_pModelLoad == NULL)
	{
		m_pModelLoad = new CModelLoad;
		if (m_pModelLoad != NULL)
		{
			m_pModelLoad->Init();
		}
	}

	// 攻撃情報を生成
	if (m_pDataAtk == NULL)
	{
		m_pDataAtk = new CDataAttack;
		if (m_pDataAtk != NULL)
		{
			m_pDataAtk->Init();
		}
	}

	// クリア情報を生成
	if (m_pDataClear == NULL)
	{
		m_pDataClear = new CDataClear;
		if (m_pDataClear != NULL)
		{
			m_pDataClear->Init();
		}
	}
	
	// フェードの生成
	if (m_pFade == NULL)
	{
		m_pFade = new CFade;
		if (m_pFade != NULL)
		{
			m_pFade->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
	}

	// モードの設定
	SetMode(m_mode);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CManager::Uninit(void)
{
	// 全てのオブジェクトの破棄
	CScene::ReleaseAll();

	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	if (m_pCamera != NULL)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}

	if (m_pKeyboard != NULL)
	{
		m_pKeyboard->Uninit();
		delete m_pKeyboard;
		m_pKeyboard = NULL;
	}

	if (m_pGamePad != NULL)
	{
		m_pGamePad->Uninit();
		delete m_pGamePad;
		m_pGamePad = NULL;
	}

	if (m_pMouse != NULL)
	{
		m_pMouse->Uninit();
		delete m_pMouse;
		m_pMouse = NULL;
	}

	if (m_pSound != NULL)
	{
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = NULL;
	}

	if (m_pTexture != NULL)
	{
		m_pTexture->Uninit();
		delete m_pTexture;
		m_pTexture = NULL;
	}
	
	if (m_pModelLoad != NULL)
	{
		m_pModelLoad->Uninit();
		delete m_pModelLoad;
		m_pModelLoad = NULL;
	}

	if (m_pDataAtk != NULL)
	{
		m_pDataAtk->Uninit();
		delete m_pDataAtk;
		m_pDataAtk = NULL;
	}

	if (m_pDataClear != NULL)
	{
		m_pDataClear->Uninit();
		delete m_pDataClear;
		m_pDataClear = NULL;
	}
	
	if (m_pFade != NULL)
	{
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CManager::Update(void)
{
	// キーボードの更新
	if (m_pKeyboard != NULL)
	{
		m_pKeyboard->Update();
	}

	// ゲームパッドの更新
	if (m_pGamePad != NULL)
	{
		m_pGamePad->Update();
	}

	// マウスの更新
	if (m_pMouse != NULL)
	{
		m_pMouse->Update();
	}

	// カメラの更新
	if (m_pCamera != NULL)
	{
		m_pCamera->Update();
	}

	// レンダリングの更新
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Update();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CManager::Draw(void)
{
	// 全オブジェクトの描画
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Draw();
	}
}

//=============================================================================
// モードの設定処理
//=============================================================================
void CManager::SetMode(MODE mode)
{
	// モードの破棄
	switch (m_mode)
	{
	case MODE_TITLE:
		if (m_pTitle != NULL)
		{
			m_pTitle->Uninit();
			m_pTitle = NULL;
		}
		break;

	case MODE_GAME:
		if (m_pGame != NULL)
		{
			m_pGame->Uninit();
			m_pGame = NULL;
		}
		break;

	default:
		break;
	}

	// オブジェクトの開放
	CScene::ReleaseAll();

	//現在のモードを更新
	m_mode = mode;

	// モードの生成
	switch (mode)
	{
	case MODE_TITLE:
		m_pTitle = CTitle::Create();
		break;
		// ゲームの生成
	case MODE_GAME:
		// カメラ・ライトの破棄～生成
		GameSetUp();

		m_pGame = CGame::Create();
		break;

	default:
		break;
	}
}

//=============================================================================
// ゲーム画面のセットアップ処理
//=============================================================================
void CManager::GameSetUp(void)
{
	// カメラの破棄
	if (m_pCamera != NULL)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}

	// カメラの生成
	m_pCamera = CCamera::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
		0.0f, 0.0f,								// 描画する画面の左上X座標、左上Y座標
		(float)SCREEN_WIDTH, SCREEN_HEIGHT);	// 描画する画面の幅、高さ
}