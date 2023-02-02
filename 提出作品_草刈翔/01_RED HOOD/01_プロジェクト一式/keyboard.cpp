//=============================================================================
// キーボード入力処理 [keyboard.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "keyboard.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CKeyboard::CKeyboard()
{
	// キー情報のクリア
	for (int nCntKey = 0; nCntKey < 256; nCntKey++)
	{
		m_aKeyState[nCntKey] = NULL;
		m_aKeyStateTrigger[nCntKey] = NULL;
		m_aKeyStateRelease[nCntKey] = NULL;
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CKeyboard::~CKeyboard()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	// 継承元クラスの初期化処理
	CInput::Init(hInstance, hWnd);

	// 入力デバイス(キーボード)の生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	m_pDevice->Acquire();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CKeyboard::Uninit(void)
{
	// 継承元クラスの終了処理
	CInput::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CKeyboard::Update(void)
{
	BYTE aState[NUM_KEY_MAX];
	int nCntKey;

	//入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aState), &aState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aState[nCntKey]) &aState[nCntKey];
			m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aState[nCntKey]) &m_aKeyState[nCntKey];
			m_aKeyState[nCntKey] = aState[nCntKey];
		}
	}
	else
	{
		m_pDevice->Acquire();
	}
}

//=============================================================================
// 入力状態の取得処理(プレス)
//=============================================================================
bool CKeyboard::GetPress(int nKey)
{
	return(m_aKeyState[nKey] & 0x80) ? true : false;
}

//=============================================================================
// 入力状態の取得処理(トリガー)
//=============================================================================
bool CKeyboard::GetTrigger(int nKey)
{
	return(m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//=============================================================================
// 入力状態の取得処理(リリース)
//=============================================================================
bool CKeyboard::GetRelease(int nKey)
{
	return(m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//=============================================================================
// 全てのキーの入力状態の取得処理(トリガー)
//=============================================================================
bool CKeyboard::GetAnyKey(void)
{
	for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		if ((m_aKeyStateTrigger[nCntKey] & 0x80) != 0)
		{
			return true;
		}
	}

	return false;
}