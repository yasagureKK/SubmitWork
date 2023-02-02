//=============================================================================
// �}�l�[�W���[ [manager.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
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
// �ÓI�����o�ϐ��錾
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
// �R���X�g���N�^
//=============================================================================
CManager::CManager()
{
	// �ϐ��̃N���A
	m_pCamera = NULL;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CManager::~CManager()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	// �����_���[�̐���
	if (m_pRenderer == NULL)
	{
		m_pRenderer = new CRenderer;
		if (m_pRenderer != NULL)
		{
			m_pRenderer->Init(hInstance, hWnd, bWindow);
		}
	}

	// �L�[�{�[�h�̐���
	if (m_pKeyboard == NULL)
	{
		m_pKeyboard = new CKeyboard;
		if (m_pKeyboard != NULL)
		{
			m_pKeyboard->Init(hInstance, hWnd);
		}
	}

	// �Q�[���p�b�h�̐���
	if (m_pGamePad == NULL)
	{
		m_pGamePad = new CGamePad;
		if (m_pGamePad != NULL)
		{
			m_pGamePad->Init(hInstance, hWnd);
		}
	}

	// �}�E�X�̐���
	if (m_pMouse == NULL)
	{
		m_pMouse = new CMouse;
		if (m_pMouse != NULL)
		{
			m_pMouse->Init(hInstance, hWnd);
		}
	}

	// �T�E���h�̐���
	if (m_pSound == NULL)
	{
		m_pSound = new CSound;
		if (m_pSound != NULL)
		{
			m_pSound->Init(hWnd);
		}
	}

	// �e�N�X�`���̐���
	if (m_pTexture == NULL)
	{
		m_pTexture = new CTexture;
		if (m_pTexture != NULL)
		{
			m_pTexture->Init();
		}
	}

	// ���f�����[�h�̐���
	if (m_pModelLoad == NULL)
	{
		m_pModelLoad = new CModelLoad;
		if (m_pModelLoad != NULL)
		{
			m_pModelLoad->Init();
		}
	}

	// �U�����𐶐�
	if (m_pDataAtk == NULL)
	{
		m_pDataAtk = new CDataAttack;
		if (m_pDataAtk != NULL)
		{
			m_pDataAtk->Init();
		}
	}

	// �N���A���𐶐�
	if (m_pDataClear == NULL)
	{
		m_pDataClear = new CDataClear;
		if (m_pDataClear != NULL)
		{
			m_pDataClear->Init();
		}
	}
	
	// �t�F�[�h�̐���
	if (m_pFade == NULL)
	{
		m_pFade = new CFade;
		if (m_pFade != NULL)
		{
			m_pFade->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
	}

	// ���[�h�̐ݒ�
	SetMode(m_mode);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CManager::Uninit(void)
{
	// �S�ẴI�u�W�F�N�g�̔j��
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
// �X�V����
//=============================================================================
void CManager::Update(void)
{
	// �L�[�{�[�h�̍X�V
	if (m_pKeyboard != NULL)
	{
		m_pKeyboard->Update();
	}

	// �Q�[���p�b�h�̍X�V
	if (m_pGamePad != NULL)
	{
		m_pGamePad->Update();
	}

	// �}�E�X�̍X�V
	if (m_pMouse != NULL)
	{
		m_pMouse->Update();
	}

	// �J�����̍X�V
	if (m_pCamera != NULL)
	{
		m_pCamera->Update();
	}

	// �����_�����O�̍X�V
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Update();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CManager::Draw(void)
{
	// �S�I�u�W�F�N�g�̕`��
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Draw();
	}
}

//=============================================================================
// ���[�h�̐ݒ菈��
//=============================================================================
void CManager::SetMode(MODE mode)
{
	// ���[�h�̔j��
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

	// �I�u�W�F�N�g�̊J��
	CScene::ReleaseAll();

	//���݂̃��[�h���X�V
	m_mode = mode;

	// ���[�h�̐���
	switch (mode)
	{
	case MODE_TITLE:
		m_pTitle = CTitle::Create();
		break;
		// �Q�[���̐���
	case MODE_GAME:
		// �J�����E���C�g�̔j���`����
		GameSetUp();

		m_pGame = CGame::Create();
		break;

	default:
		break;
	}
}

//=============================================================================
// �Q�[����ʂ̃Z�b�g�A�b�v����
//=============================================================================
void CManager::GameSetUp(void)
{
	// �J�����̔j��
	if (m_pCamera != NULL)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}

	// �J�����̐���
	m_pCamera = CCamera::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
		0.0f, 0.0f,								// �`�悷���ʂ̍���X���W�A����Y���W
		(float)SCREEN_WIDTH, SCREEN_HEIGHT);	// �`�悷���ʂ̕��A����
}