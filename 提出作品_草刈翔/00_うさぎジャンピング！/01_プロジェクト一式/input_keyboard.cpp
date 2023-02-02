//=============================================================================
// キーボード入力処理 [input_keyboard.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "input_keyboard.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CInputKeyboard::CInputKeyboard()
{
	// キー情報のクリア
	for (int nCntKey = 0; nCntKey < MAX_KEYBOARD; nCntKey++)
	{
		m_aKeyState[nCntKey] = NULL;
		m_aKeyStateTrigger[nCntKey] = NULL;
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CInputKeyboard::~CInputKeyboard()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
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
void CInputKeyboard::Uninit(void)
{
	// 継承元クラスの終了処理
	CInput::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[MAX_KEYBOARD];
	int nCntKey;

	//入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < MAX_KEYBOARD; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) &aKeyState[nCntKey];
			m_aKeyState[nCntKey] = aKeyState[nCntKey];
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
bool CInputKeyboard::GetPress(int nKey)
{
	return(m_aKeyState[nKey] & 0x80) ? true : false;
}

//=============================================================================
// 入力状態の取得処理(トリガー)
//=============================================================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	return(m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}