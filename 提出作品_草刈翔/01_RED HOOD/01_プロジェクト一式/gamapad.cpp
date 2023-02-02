//=============================================================================
// �Q�[���p�b�h���͏��� [gamepad.cpp]
// Author : Kusakari Kakeru
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "gamepad.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CGamePad::CGamePad()
{
	m_pDevice = NULL;

	for (int nCntPad = 0; nCntPad < XUSER_MAX_COUNT; nCntPad++)
	{
		// �ϐ��̃N���A
		ZeroMemory(&m_aState[nCntPad], sizeof(XINPUT_STATE));			// ���͏��(�v���X)
		ZeroMemory(&m_aStateTrigger, sizeof(XINPUT_STATE));				// ���͏��(�g���K�[)
		ZeroMemory(&m_aStateRelease, sizeof(XINPUT_STATE));				// ���͏��(�����[�X)
		m_bConnect[nCntPad] = false;
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGamePad::~CGamePad()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CGamePad::Init(HINSTANCE hInstance, HWND hWnd)
{
	// ��{�N���X�̏���������
	CInput::Init(hInstance, hWnd);

	// �ϐ��̏�����
	for (int nCntPad = 0; nCntPad < XUSER_MAX_COUNT; nCntPad++)
	{
		m_bConnect[nCntPad] = false;
	}

	XINPUT_STATE state[XUSER_MAX_COUNT];
	for (int nCntPad = 0; nCntPad < XUSER_MAX_COUNT; nCntPad++)
	{
		ZeroMemory(&state[nCntPad], sizeof(XINPUT_STATE));
		DWORD dwResult = XInputGetState(nCntPad, &state[nCntPad]);

		// �R���g���[���[���ڑ�����Ă��邩�m�F����
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
// �I������
//=============================================================================
void CGamePad::Uninit(void)
{
	// ���͏����j���[�g�����ɂ���
	XInputEnable(false);

	// ��{�N���X�̏I������
	CInput::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CGamePad::Update(void)
{
	XINPUT_STATE state[XUSER_MAX_COUNT];
	for (int nCntPad = 0; nCntPad < XUSER_MAX_COUNT; nCntPad++)
	{
		ZeroMemory(&state[nCntPad], sizeof(XINPUT_STATE));
		DWORD dwResult = XInputGetState(nCntPad, &state[nCntPad]);

		// �R���g���[���[���ڑ�����Ă��邩�m�F����
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
			// ���X�e�B�b�N�̃f�b�h�]�[���̐ݒ�
			if ((m_aState[nCntPad].Gamepad.sThumbLX <= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
				m_aState[nCntPad].Gamepad.sThumbLX >= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
				(m_aState[nCntPad].Gamepad.sThumbLY <= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
					m_aState[nCntPad].Gamepad.sThumbLY >= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
			{
				m_aState[nCntPad].Gamepad.sThumbLX = 0;
				m_aState[nCntPad].Gamepad.sThumbLY = 0;
			}

			// �E�X�e�B�b�N�̃f�b�h�]�[���̐ݒ�
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
// ���͏�Ԃ̎擾����(�v���X)
//=============================================================================
bool CGamePad::GetPress(int nInput, int nNum)
{
	// �ڑ����Ă��Ȃ��ꍇ��false��Ԃ��ď������I��������
	if (GetConnect(nNum) == false)
	{
		return false;
	}

	return ReturnDenialButton(m_aState, nNum, nInput);
}

//=============================================================================
// ���͏�Ԃ̎擾����(�g���K�[)
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
// ���͏�Ԃ̎擾����(�����[�X)
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
// �{�^�����͏�Ԃ̎擾����(�v���X)
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
// �{�^�����͏�Ԃ̎擾����(�g���K�[)
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
// �{�^�����͏�Ԃ̎擾����(�����[�X)
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
// �S�Ă̓��͏�Ԃ̎擾����(�g���K�[)
//=============================================================================
bool CGamePad::GetAnyButton(int nNum)
{
	if (GetConnect(nNum) == false)
	{
		return false;
	}

	// �{�^���̐�������
	for (int nCntButton = 0; nCntButton < 14; nCntButton++)
	{
		int nButton = ReturnDPAD(nCntButton);
		
		if ((((m_aStateTrigger[nNum].Gamepad.wButtons & nButton) == false) && (m_aState[nNum].Gamepad.wButtons & nButton)) == true)
		{
			return true;
		}
	}

	// �{�^���ȊO�̓��͕��@�̐�������
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
// �{�^���̒l��Ԃ�����
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
// �{�^���ȊO�̓��͏���Ԃ�����
//=============================================================================
bool CGamePad::ReturnDenialButton(XINPUT_STATE aState[], int nNum, int nInput)
{
	switch (nInput)
	{
	case PAD_INPUT_LSTICK_UP:	// ���X�e�B�b�N�F��
		return aState[nNum].Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ? true : false;
		break;
	case PAD_INPUT_LSTICK_DOWN:	// ���X�e�B�b�N�F��
		return aState[nNum].Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ? true : false;
		break;
	case PAD_INPUT_LSTICK_LEFT:	// ���X�e�B�b�N�F��
		return aState[nNum].Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ? true : false;
		break;
	case PAD_INPUT_LSTICK_RIGHT:// ���X�e�B�b�N�F�E
		return aState[nNum].Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ? true : false;
		break;
	case PAD_INPUT_RSTICK_UP:	// �E�X�e�B�b�N�F��
		return aState[nNum].Gamepad.sThumbRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ? true : false;
		break;
	case PAD_INPUT_RSTICK_DOWN:	// �E�X�e�B�b�N�F��
		return aState[nNum].Gamepad.sThumbRY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ? true : false;
		break;
	case PAD_INPUT_RSTICK_LEFT:	// �E�X�e�B�b�N�F��
		return aState[nNum].Gamepad.sThumbRX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ? true : false;
		break;
	case PAD_INPUT_RSTICK_RIGHT:// �E�X�e�B�b�N�F�E
		return aState[nNum].Gamepad.sThumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ? true : false;
		break;
	case PAD_INPUT_LEFT_TRIGGER:	// �g���K�[�F��
		return aState[nNum].Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD ? true : false;
		break;
	case PAD_INPUT_RIGHT_TRIGGER:	// �g���K�[�F�E
		return aState[nNum].Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD ? true : false;
		break;
	default:
		return false;
		break;
	}
}