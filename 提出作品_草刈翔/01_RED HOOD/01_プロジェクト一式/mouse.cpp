//=============================================================================
// マウス入力処理 [mouse.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "mouse.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CMouse::CMouse()
{
	m_pDevice = NULL;

	// 変数のクリア
	ZeroMemory(&m_aMouseState, sizeof(DIMOUSESTATE));			// 入力情報(プレス)
	ZeroMemory(&m_aMouseStateTrigger, sizeof(DIMOUSESTATE));	// 入力情報(トリガー)
	m_bConnect = false;
	m_hWnd = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CMouse::~CMouse()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	// 継承元クラスの初期化処理
	CInput::Init(hInstance, hWnd);

	// 変数の初期化
	ZeroMemory(&m_aMouseState, sizeof(DIMOUSESTATE));			// 入力情報(プレス)
	ZeroMemory(&m_aMouseStateTrigger, sizeof(DIMOUSESTATE));	// 入力情報(トリガー)
	m_bConnect = false;
	m_hWnd = hWnd;

	// 入力デバイス(マウス)の生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// マウスへのアクセス権を獲得(入力制御開始)
	m_pDevice->Acquire();
	m_bConnect = true;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CMouse::Uninit(void)
{
	// 継承元クラスの終了処理
	CInput::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CMouse::Update(void)
{
	// マウスが接続されていないかつ、デバイスがまだ生成されていなかったら
	if (m_bConnect == false && m_pDevice == NULL)
	{
		// 入力デバイス(マウス)の生成
		if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
		{
			return;
		}

		// データフォーマットを設定
		if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse)))
		{
			return;
		}

		// 協調モードを設定
		if (FAILED(m_pDevice->SetCooperativeLevel(m_hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
		{
			return;
		}

		m_bConnect = true;
	}

	// マウスが接続されている場合のみ
	if (m_bConnect == true)
	{
		DIMOUSESTATE aMouseState;

		//入力デバイスからデータを取得
		if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE), &aMouseState)))
		{
			for (int nCntMouse = 0; nCntMouse < MOUSE_INPUT_MAX; nCntMouse++)
			{
				m_aMouseStateTrigger.rgbButtons[nCntMouse] = (m_aMouseState.rgbButtons[nCntMouse] ^ aMouseState.rgbButtons[nCntMouse]) &aMouseState.rgbButtons[nCntMouse];
				m_aMouseState.rgbButtons[nCntMouse] = aMouseState.rgbButtons[nCntMouse];
			}
		}
		else
		{
			m_pDevice->Acquire();
		}
	}
}

//=============================================================================
// 入力状態の取得処理(プレス)
//=============================================================================
bool CMouse::GetPress(MOUSE_INPUT nType)
{
	return(m_aMouseState.rgbButtons[nType] & 0x80) ? true : false;
}

//=============================================================================
// 入力状態の取得処理(トリガー)
//=============================================================================
bool CMouse::GetTrigger(MOUSE_INPUT nType)
{
	return(m_aMouseStateTrigger.rgbButtons[nType] & 0x80) ? true : false;
}