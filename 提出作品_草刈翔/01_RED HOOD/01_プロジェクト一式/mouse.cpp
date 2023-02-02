//=============================================================================
// �}�E�X���͏��� [mouse.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "mouse.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CMouse::CMouse()
{
	m_pDevice = NULL;

	// �ϐ��̃N���A
	ZeroMemory(&m_aMouseState, sizeof(DIMOUSESTATE));			// ���͏��(�v���X)
	ZeroMemory(&m_aMouseStateTrigger, sizeof(DIMOUSESTATE));	// ���͏��(�g���K�[)
	m_bConnect = false;
	m_hWnd = NULL;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMouse::~CMouse()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	// �p�����N���X�̏���������
	CInput::Init(hInstance, hWnd);

	// �ϐ��̏�����
	ZeroMemory(&m_aMouseState, sizeof(DIMOUSESTATE));			// ���͏��(�v���X)
	ZeroMemory(&m_aMouseStateTrigger, sizeof(DIMOUSESTATE));	// ���͏��(�g���K�[)
	m_bConnect = false;
	m_hWnd = hWnd;

	// ���̓f�o�C�X(�}�E�X)�̐���
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �}�E�X�ւ̃A�N�Z�X�����l��(���͐���J�n)
	m_pDevice->Acquire();
	m_bConnect = true;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CMouse::Uninit(void)
{
	// �p�����N���X�̏I������
	CInput::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CMouse::Update(void)
{
	// �}�E�X���ڑ�����Ă��Ȃ����A�f�o�C�X���܂���������Ă��Ȃ�������
	if (m_bConnect == false && m_pDevice == NULL)
	{
		// ���̓f�o�C�X(�}�E�X)�̐���
		if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
		{
			return;
		}

		// �f�[�^�t�H�[�}�b�g��ݒ�
		if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse)))
		{
			return;
		}

		// �������[�h��ݒ�
		if (FAILED(m_pDevice->SetCooperativeLevel(m_hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
		{
			return;
		}

		m_bConnect = true;
	}

	// �}�E�X���ڑ�����Ă���ꍇ�̂�
	if (m_bConnect == true)
	{
		DIMOUSESTATE aMouseState;

		//���̓f�o�C�X����f�[�^���擾
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
// ���͏�Ԃ̎擾����(�v���X)
//=============================================================================
bool CMouse::GetPress(MOUSE_INPUT nType)
{
	return(m_aMouseState.rgbButtons[nType] & 0x80) ? true : false;
}

//=============================================================================
// ���͏�Ԃ̎擾����(�g���K�[)
//=============================================================================
bool CMouse::GetTrigger(MOUSE_INPUT nType)
{
	return(m_aMouseStateTrigger.rgbButtons[nType] & 0x80) ? true : false;
}