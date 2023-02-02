//=============================================================================
// ゲームパッド入力処理 [gamepad.cpp]
// Author : Kusakari Kakeru
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "gamepad.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CGamePad::CGamePad()
{
	m_pDevice = NULL;

	for (int nCntPad = 0; nCntPad < XUSER_MAX_COUNT; nCntPad++)
	{
		// 変数のクリア
		ZeroMemory(&m_aState[nCntPad], sizeof(XINPUT_STATE));			// 入力情報(プレス)
		ZeroMemory(&m_aStateTrigger, sizeof(XINPUT_STATE));				// 入力情報(トリガー)
		ZeroMemory(&m_aStateRelease, sizeof(XINPUT_STATE));				// 入力情報(リリース)
		m_bConnect[nCntPad] = false;
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CGamePad::~CGamePad()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CGamePad::Init(HINSTANCE hInstance, HWND hWnd)
{
	// 基本クラスの初期化処理
	CInput::Init(hInstance, hWnd);

	// 変数の初期化
	for (int nCntPad = 0; nCntPad < XUSER_MAX_COUNT; nCntPad++)
	{
		m_bConnect[nCntPad] = false;
	}

	XINPUT_STATE state[XUSER_MAX_COUNT];
	for (int nCntPad = 0; nCntPad < XUSER_MAX_COUNT; nCntPad++)
	{
		ZeroMemory(&state[nCntPad], sizeof(XINPUT_STATE));
		DWORD dwResult = XInputGetState(nCntPad, &state[nCntPad]);

		// コントローラーが接続されているか確認する
		if (dwResult == ERROR_SUCCESS)
		{
			m_bConnect[nCntPad] = true;
		}
		else
		{
			m_bConnect[nCntPad] = false;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGamePad::Uninit(void)
{
	// 入力情報をニュートラルにする
	XInputEnable(false);

	// 基本クラスの終了処理
	CInput::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CGamePad::Update(void)
{
	XINPUT_STATE state[XUSER_MAX_COUNT];
	for (int nCntPad = 0; nCntPad < XUSER_MAX_COUNT; nCntPad++)
	{
		ZeroMemory(&state[nCntPad], sizeof(XINPUT_STATE));
		DWORD dwResult = XInputGetState(nCntPad, &state[nCntPad]);

		// コントローラーが接続されているか確認する
		if (dwResult == ERROR_SUCCESS)
		{
			m_bConnect[nCntPad] = true;
		}
		else
		{
			m_bConnect[nCntPad] = false;
		}
	}

	for (int nCntPad = 0; nCntPad < XUSER_MAX_COUNT; nCntPad++)
	{
		if (XInputGetState(nCntPad, &state[nCntPad]) == ERROR_SUCCESS)
		{
			// 左スティックのデッドゾーンの設定
			if ((m_aState[nCntPad].Gamepad.sThumbLX <= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
				m_aState[nCntPad].Gamepad.sThumbLX >= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
				(m_aState[nCntPad].Gamepad.sThumbLY <= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
					m_aState[nCntPad].Gamepad.sThumbLY >= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
			{
				m_aState[nCntPad].Gamepad.sThumbLX = 0;
				m_aState[nCntPad].Gamepad.sThumbLY = 0;
			}

			// 右スティックのデッドゾーンの設定
			if ((m_aState[nCntPad].Gamepad.sThumbRX <= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
				m_aState[nCntPad].Gamepad.sThumbRX >= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
				(m_aState[nCntPad].Gamepad.sThumbRY <= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
					m_aState[nCntPad].Gamepad.sThumbRY >= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
			{
				m_aState[nCntPad].Gamepad.sThumbRX = 0;
				m_aState[nCntPad].Gamepad.sThumbRY = 0;
			}

			m_aStateTrigger[nCntPad] = m_aState[nCntPad];
			m_aStateRelease[nCntPad] = m_aState[nCntPad];
			m_aState[nCntPad] = state[nCntPad];
		}
	}
}

//=============================================================================
// 入力状態の取得処理(プレス)
//=============================================================================
bool CGamePad::GetPress(int nInput, int nNum)
{
	// 接続していない場合はfalseを返して処理を終了させる
	if (GetConnect(nNum) == false)
	{
		return false;
	}

	return ReturnDenialButton(m_aState, nNum, nInput);
}

//=============================================================================
// 入力状態の取得処理(トリガー)
//=============================================================================
bool CGamePad::GetTrigger(int nInput, int nNum)
{
	if (GetConnect(nNum) == false)
	{
		return false;
	}

	return  ReturnDenialButton(m_aStateTrigger, nNum, nInput) == false && ReturnDenialButton(m_aState, nNum, nInput) ? true : false;
}

//=============================================================================
// 入力状態の取得処理(リリース)
//=============================================================================
bool CGamePad::GetRelease(int nInput, int nNum)
{
	if (GetConnect(nNum) == false)
	{
		return false;
	}

	return  ReturnDenialButton(m_aStateRelease, nNum, nInput) == true && ReturnDenialButton(m_aState, nNum, nInput) == false ? true : false;
}

//=============================================================================
// ボタン入力状態の取得処理(プレス)
//=============================================================================
bool CGamePad::GetButtonPress(int nButton, int nNum)
{
	if (GetConnect(nNum) == false)
	{
		return false;
	}

	return m_aState[nNum].Gamepad.wButtons & nButton ? true : false;
}

//=============================================================================
// ボタン入力状態の取得処理(トリガー)
//=============================================================================
bool CGamePad::GetButtonTrigger(int nButton, int nNum)
{
	if (GetConnect(nNum) == false)
	{
		return false;
	}

	return (((m_aStateTrigger[nNum].Gamepad.wButtons & nButton) == false) && (m_aState[nNum].Gamepad.wButtons & nButton)) ? true : false;
}

//=============================================================================
// ボタン入力状態の取得処理(リリース)
//=============================================================================
bool CGamePad::GetButtonRelease(int nButton, int nNum)
{
	if (GetConnect(nNum) == false)
	{
		return false;
	}

	return (((m_aStateRelease[nNum].Gamepad.wButtons & nButton)) && (m_aState[nNum].Gamepad.wButtons & nButton) == false) ? true : false;
}

//=============================================================================
// 全ての入力状態の取得処理(トリガー)
//=============================================================================
bool CGamePad::GetAnyButton(int nNum)
{
	if (GetConnect(nNum) == false)
	{
		return false;
	}

	// ボタンの数だけ回す
	for (int nCntButton = 0; nCntButton < 14; nCntButton++)
	{
		int nButton = ReturnDPAD(nCntButton);
		
		if ((((m_aStateTrigger[nNum].Gamepad.wButtons & nButton) == false) && (m_aState[nNum].Gamepad.wButtons & nButton)) == true)
		{
			return true;
		}
	}

	// ボタン以外の入力方法の数だけ回す
	for (int nCntInput = 0; nCntInput < PAD_INPUT_MAX; nCntInput++)
	{
		if (ReturnDenialButton(m_aStateTrigger, nNum, nCntInput) == false && ReturnDenialButton(m_aState, nNum, nCntInput) == true)
		{
			return true;
		}
	}

	return false;
}

//=============================================================================
// ボタンの値を返す処理
//=============================================================================
int CGamePad::ReturnDPAD(int nButton)
{
	if (nButton == 0)
	{
		return XINPUT_GAMEPAD_DPAD_UP;
	}
	else if (nButton == 1)
	{
		return XINPUT_GAMEPAD_DPAD_DOWN;
	}
	else if (nButton == 2)
	{
		return XINPUT_GAMEPAD_DPAD_LEFT;
	}
	else if (nButton == 3)
	{
		return XINPUT_GAMEPAD_DPAD_RIGHT;
	}
	else if (nButton == 4)
	{
		return XINPUT_GAMEPAD_START;
	}
	else if (nButton == 5)
	{
		return XINPUT_GAMEPAD_BACK;
	}
	else if (nButton == 6)
	{
		return XINPUT_GAMEPAD_LEFT_THUMB;
	}
	else if (nButton == 7)
	{
		return XINPUT_GAMEPAD_RIGHT_THUMB;
	}
	else if (nButton == 8)
	{
		return XINPUT_GAMEPAD_LEFT_SHOULDER;
	}
	else if (nButton == 9)
	{
		return XINPUT_GAMEPAD_RIGHT_SHOULDER;
	}
	else if (nButton == 10)
	{
		return XINPUT_GAMEPAD_A;
	}
	else if (nButton == 11)
	{
		return XINPUT_GAMEPAD_B;
	}
	else if (nButton == 12)
	{
		return XINPUT_GAMEPAD_X;
	}
	else if (nButton == 13)
	{
		return XINPUT_GAMEPAD_Y;
	}

	return 0;
}

//=============================================================================
// ボタン以外の入力情報を返す処理
//=============================================================================
bool CGamePad::ReturnDenialButton(XINPUT_STATE aState[], int nNum, int nInput)
{
	switch (nInput)
	{
	case PAD_INPUT_LSTICK_UP:	// 左スティック：上
		return aState[nNum].Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ? true : false;
		break;
	case PAD_INPUT_LSTICK_DOWN:	// 左スティック：下
		return aState[nNum].Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ? true : false;
		break;
	case PAD_INPUT_LSTICK_LEFT:	// 左スティック：左
		return aState[nNum].Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ? true : false;
		break;
	case PAD_INPUT_LSTICK_RIGHT:// 左スティック：右
		return aState[nNum].Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ? true : false;
		break;
	case PAD_INPUT_RSTICK_UP:	// 右スティック：上
		return aState[nNum].Gamepad.sThumbRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ? true : false;
		break;
	case PAD_INPUT_RSTICK_DOWN:	// 右スティック：下
		return aState[nNum].Gamepad.sThumbRY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ? true : false;
		break;
	case PAD_INPUT_RSTICK_LEFT:	// 右スティック：左
		return aState[nNum].Gamepad.sThumbRX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ? true : false;
		break;
	case PAD_INPUT_RSTICK_RIGHT:// 右スティック：右
		return aState[nNum].Gamepad.sThumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ? true : false;
		break;
	case PAD_INPUT_LEFT_TRIGGER:	// トリガー：左
		return aState[nNum].Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD ? true : false;
		break;
	case PAD_INPUT_RIGHT_TRIGGER:	// トリガー：右
		return aState[nNum].Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD ? true : false;
		break;
	default:
		return false;
		break;
	}
}