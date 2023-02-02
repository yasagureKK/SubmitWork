//=============================================================================
// �}�l�[�W���[���� [manager.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "title.h"
#include "menu.h"
#include "tutorial.h"
#include "solo_menu.h"
#include "game.h"
#include "pvp.h"
#include "result.h"
#include "ranking.h"
#include "soundtest.h"
#include "input_keyboard.h"
#include "input_pad.h"
#include "texture.h"
#include "sound.h"
#include "fade.h"
#include "model_load.h"
#include "pause.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
CManager::MODE CManager::m_mode = MODE_TITLE;
CManager::MODE CManager::m_BeforeMode = MODE_TITLE;
CTitle *CManager::m_pTitle = NULL;
CMenu *CManager::m_pMenu = NULL;
CTutorial *CManager::m_pTutorial = NULL;
CSoloMenu *CManager::m_pSoloMenu = NULL;
CGame *CManager::m_pGame = NULL;
CPvP *CManager::m_pPvP = NULL;
CResult *CManager::m_pResult = NULL;
CRanking *CManager::m_pRanking = NULL;
CSoundTest *CManager::m_pSoundTest = NULL;
CRenderer *CManager::m_pRenderer = NULL;
CInputKeyboard *CManager::m_pInputKeyboard = NULL;
CInputPad *CManager::m_pInputPad = NULL;
CTexture *CManager::m_pTexture = NULL;
CCamera *CManager::m_apCamera[MAX_CAMERA] = {};
CLight *CManager::m_apLight[MAX_LIGHT] = {};
CSound *CManager::m_pSound = NULL;
CFade *CManager::m_pFade = NULL;
CModelLoad *CManager::m_pModelLoad = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CManager::CManager()
{
	// �J�����ƃ��C�g�͔z��̂��߁Amemset�ň�x�ɃN���A
	memset(&m_apCamera, NULL, sizeof(m_apCamera));
	memset(&m_apLight, NULL, sizeof(m_apLight));
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
	// ���Ԃŗ�����������
	srand((unsigned int)time(NULL));

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
	if (m_pInputKeyboard == NULL)
	{
		m_pInputKeyboard = new CInputKeyboard;
		if (m_pInputKeyboard != NULL)
		{
			m_pInputKeyboard->Init(hInstance, hWnd);
		}
	}

	// �Q�[���p�b�h�̐���
	if (m_pInputPad == NULL)
	{
		m_pInputPad = new CInputPad;
		if (m_pInputPad != NULL)
		{
			m_pInputPad->Init(hInstance, hWnd);
		}
	}

	// �T�E���h�̐���
	if (m_pSound == NULL)
	{
		m_pSound = new CSound;
		if (m_pInputKeyboard != NULL)
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

	// ���f�����[�h�𐶐�
	if (m_pModelLoad == NULL)
	{
		m_pModelLoad = new CModelLoad;
		if (m_pModelLoad != NULL)
		{
			m_pModelLoad->Init();
		}
	}

	// �J�����̐���(�t�F�[�h�������̃G���[����̂���)
	for (int nCntCamera = 0; nCntCamera < MAX_CAMERA; nCntCamera++)
	{
		m_apCamera[nCntCamera] = CCamera::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			0.0f, 0.0f,						// �`�悷���ʂ̍���X���W�A����Y���W
			SCREEN_WIDTH, SCREEN_HEIGHT);	// �`�悷���ʂ̕��A����
		m_apCamera[nCntCamera]->SetNum(nCntCamera);
	}

	// �t�F�[�h�̐���
	if (m_pFade == NULL)
	{
		m_pFade = new CFade;
		if (m_pFade != NULL)
		{
			m_pFade->Init(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f),
				D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));
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

	// �����_���[�̔j��
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	// �L�[�{�[�h�̔j��
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	// �Q�[���p�b�h�̔j��
	if (m_pInputPad != NULL)
	{
		m_pInputPad->Uninit();
		delete m_pInputPad;
		m_pInputPad = NULL;
	}

	// �T�E���h�̔j��
	if (m_pSound != NULL)
	{
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = NULL;
	}

	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Uninit();
		delete m_pTexture;
		m_pTexture = NULL;
	}

	// ���f�����[�h�̔j��
	if (m_pModelLoad != NULL)
	{
		m_pModelLoad->Uninit();
		delete m_pModelLoad;
		m_pModelLoad = NULL;
	}

	// �J�����̔j��
	for (int nCntCamera = 0; nCntCamera < MAX_CAMERA; nCntCamera++)
	{
		if (m_apCamera[nCntCamera] != NULL)
		{
			m_apCamera[nCntCamera]->Uninit();
			delete m_apCamera[nCntCamera];
			m_apCamera[nCntCamera] = NULL;
		}
	}

	// ���C�g�̔j��
	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		if (m_apLight[nCntLight] != NULL)
		{
			m_apLight[nCntLight]->Uninit();
			delete m_apLight[nCntLight];
			m_apLight[nCntLight] = NULL;
		}
	}

	// �I�u�W�F�N�g�̊J��
	CScene::ReleaseAll();
}

//=============================================================================
// �X�V����
//=============================================================================
void CManager::Update(void)
{
	// �|�[�Y������Ȃ��Ȃ�
	if (CPause::GetPause() == false)
	{
		// �J�����̍X�V
		for (int nCntCamera = 0; nCntCamera < MAX_CAMERA; nCntCamera++)
		{
			if (m_apCamera[nCntCamera] != NULL)
			{
				m_apCamera[nCntCamera]->Update();
			}
		}
	}

	// �L�[�{�[�h�̍X�V
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Update();
	}

	// �Q�[���p�b�h�̍X�V
	if (m_pInputPad != NULL)
	{
		m_pInputPad->Update();
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
		// �^�C�g���̔j��
	case MODE_TITLE:
		if (m_pTitle != NULL)
		{
			m_pTitle->Uninit();
			m_pTitle = NULL;
		}
		break;
		// ���j���[�̔j��
	case MODE_MENU:
		if (m_pMenu != NULL)
		{
			m_pMenu->Uninit();
			m_pMenu = NULL;
		}
		break;
		// �`���[�g���A���̔j��
	case MODE_TUTORIAL:
		if (m_pTutorial != NULL)
		{
			m_pTutorial->Uninit();
			m_pTutorial = NULL;
		}
		break;
		// 1�l�p�Q�[�����j���[�̔j��
	case MODE_SOLO_MENU:
		if (m_pSoloMenu != NULL)
		{
			m_pSoloMenu->Uninit();
			m_pSoloMenu = NULL;
		}
		break;
		// CPU�ΐ�Q�[���̔j��
	case MODE_SOLO_CPU:
		if (m_pPvP != NULL)
		{
			m_pPvP->Uninit();
			m_pPvP = NULL;
		}
		break;
		// �Q�[���̔j��
	case MODE_GAME:
		if (m_pGame != NULL)
		{
			m_pGame->Uninit();
			m_pGame = NULL;
		}
		break;
		// 2�l�p�Q�[���̔j��
	case MODE_PVP:
		if (m_pPvP != NULL)
		{
			m_pPvP->Uninit();
			m_pPvP = NULL;
		}
		break;
		// ���U���g�̔j��
	case MODE_RESULT:
		if (m_pResult != NULL)
		{
			m_pResult->Uninit();
			m_pResult = NULL;
		}
		break;
		// �����L���O�̔j��
	case MODE_RANKING:
		if (m_pRanking != NULL)
		{
			m_pRanking->Uninit();
			m_pRanking = NULL;
		}
		break;
		// �T�E���h�e�X�g�̔j��
	case MODE_SOUNDTEST:
		if (m_pSoundTest != NULL)
		{
			m_pSoundTest->Uninit();
			m_pSoundTest = NULL;
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
		// �^�C�g���̐���
	case MODE_TITLE:
		// �k���`�F�b�N
		if (m_pTitle == NULL)
		{
			// �N���X�̐���
			m_pTitle = new CTitle;

			if (m_pTitle != NULL)
			{
				// ���O�̃��[�h��ݒ�
				SetBeforeMode(mode);

				// ����������
				m_pTitle->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}
		}
		break;
		// ���j���[�̐���
	case MODE_MENU:
		// �k���`�F�b�N
		if (m_pMenu == NULL)
		{
			// �N���X�̐���
			m_pMenu = new CMenu;

			if (m_pMenu != NULL)
			{
				// ����������
				m_pMenu->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}
		}
		break;
		// �`���[�g���A���̐���
	case MODE_TUTORIAL:
		// �k���`�F�b�N
		if (m_pTutorial == NULL)
		{
			// �N���X�̐���
			m_pTutorial = new CTutorial;

			if (m_pTutorial != NULL)
			{
				// �J�����E���C�g�̔j���`����
				GameSetUp(1);

				// ����������
				m_pTutorial->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}
		}
		break;
		// 1�l�p�Q�[�����j���[�̐���
	case MODE_SOLO_MENU:
		// �k���`�F�b�N
		if (m_pSoloMenu == NULL)
		{
			// �N���X�̐���
			m_pSoloMenu = new CSoloMenu;

			if (m_pSoloMenu != NULL)
			{
				// ����������
				m_pSoloMenu->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}
		}
		break;
		// CPU�ΐ�Q�[���̐���
	case MODE_SOLO_CPU:
		// �k���`�F�b�N
		if (m_pPvP == NULL)
		{
			// �N���X�̐���
			m_pPvP = new CPvP;

			if (m_pPvP != NULL)
			{
				// ���O�̃��[�h��ݒ�
				SetBeforeMode(mode);

				// �J�����E���C�g�̔j���`����
				GameSetUp(2);

				// ����������
				m_pPvP->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}
		}
		break;
		// �Q�[���̐���
	case MODE_GAME:
		// �k���`�F�b�N
		if (m_pGame == NULL)
		{
			// �N���X�̐���
			m_pGame = new CGame;

			if (m_pGame != NULL)
			{
				// ���O�̃��[�h��ݒ�
				SetBeforeMode(mode);

				// �J�����E���C�g�̔j���`����
				GameSetUp(1);

				// ����������
				m_pGame->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}
		}
		break;
		// 2�l�p�Q�[���̐���
	case MODE_PVP:
		// �k���`�F�b�N
		if (m_pPvP == NULL)
		{
			// �N���X�̐���
			m_pPvP = new CPvP;

			if (m_pPvP != NULL)
			{
				// ���O�̃��[�h��ݒ�
				SetBeforeMode(mode);

				// �J�����E���C�g�̔j���`����
				GameSetUp(2);

				// ����������
				m_pPvP->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}
		}
		break;
		// ���U���g�̐���
	case MODE_RESULT:
		// �k���`�F�b�N
		if (m_pResult == NULL)
		{
			// �N���X�̐���
			m_pResult = new CResult;

			if (m_pResult != NULL)
			{
				// �J�����E���C�g�̔j���`����
				GameSetUp(1);

				// ����������
				m_pResult->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}
		}
		break;
		// �����L���O�̐���
	case MODE_RANKING:
		// �k���`�F�b�N
		if (m_pRanking == NULL)
		{
			// �N���X�̐���
			m_pRanking = new CRanking;

			if (m_pRanking != NULL)
			{
				// ����������
				m_pRanking->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}
		}
		break;
		// �T�E���h�e�X�g�̐���
	case MODE_SOUNDTEST:
		// �k���`�F�b�N
		if (m_pSoundTest == NULL)
		{
			// �N���X�̐���
			m_pSoundTest = new CSoundTest;

			if (m_pSoundTest != NULL)
			{
				// ����������
				m_pSoundTest->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}
		}
		break;

	default:
		break;
	}
}

//=============================================================================
// ���[�h�擾����
//=============================================================================
CManager::MODE CManager::GetMode(void)
{
	return m_mode;
}

//=============================================================================
// ���O�̃��[�h�̐ݒ菈��
//=============================================================================
void CManager::SetBeforeMode(MODE mode)
{
	m_BeforeMode = mode;
}

//=============================================================================
// ���O�̃��[�h�擾����
//=============================================================================
CManager::MODE CManager::GetBeforeMode(void)
{
	return m_BeforeMode;
}

//=============================================================================
// �Q�[���擾����
//=============================================================================
CGame* CManager::GetGame(void)
{
	return m_pGame;
}

//=============================================================================
// �f�o�C�X�擾����
//=============================================================================
CRenderer *CManager::GetRenderer(void)
{
	return m_pRenderer;
}

//=============================================================================
// �L�[�{�[�h�擾����
//=============================================================================
CInputKeyboard *CManager::GetInputKeyboard(void)
{
	return m_pInputKeyboard;
}

//=============================================================================
// �Q�[���p�b�h�擾����
//=============================================================================
CInputPad *CManager::GetInputPad(void)
{
	return m_pInputPad;
}

//=============================================================================
// �e�N�X�`���擾����
//=============================================================================
CTexture *CManager::GetTexture(void)
{
	return m_pTexture;
}

//=============================================================================
// �J�����擾����
//=============================================================================
CCamera **CManager::GetCamera(void)
{
	return &m_apCamera[0];
}

//=============================================================================
// ���C�g�擾����
//=============================================================================
CLight *CManager::GetLight(void)
{
	return m_apLight[0];
}

//=============================================================================
// �T�E���h�擾����
//=============================================================================
CSound *CManager::GetSound(void)
{
	return m_pSound;
}

//=============================================================================
// �t�F�[�h�擾����
//=============================================================================
CFade *CManager::GetFade(void)
{
	return m_pFade;
}

//=============================================================================
// ���f�����[�h�擾����
//=============================================================================
CModelLoad *CManager::GetModelLoad(void)
{
	return m_pModelLoad;
}

//=============================================================================
// �Q�[����ʂ̃Z�b�g�A�b�v����
//=============================================================================
void CManager::GameSetUp(int nCamera)
{
	// �J�����̔j��
	for (int nCntCamera = 0; nCntCamera < MAX_CAMERA; nCntCamera++)
	{
		if (m_apCamera[nCntCamera] != NULL)
		{
			m_apCamera[nCntCamera]->Uninit();
			delete m_apCamera[nCntCamera];
			m_apCamera[nCntCamera] = NULL;
		}
	}
	// ���C�g�̔j��
	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		if (m_apLight[nCntLight] != NULL)
		{
			m_apLight[nCntLight]->Uninit();
			delete m_apLight[nCntLight];
			m_apLight[nCntLight] = NULL;
		}
	}

	// ���C�g�̐���
	m_apLight[0] = CLight::Create(D3DXVECTOR3(-0.8f, -0.8f, 0.4f), D3DXVECTOR3(400.0f, 800.0f, -400.0f));
	m_apLight[1] = CLight::Create(D3DXVECTOR3(0.4f, 1.8f, -0.4f), D3DXVECTOR3(-100.0f, 0.0f, 100.0f));
	m_apLight[2] = CLight::Create(D3DXVECTOR3(0.0f, -0.4f, 0.4f), D3DXVECTOR3(100.0f, 0.0f, -200.0f));

	// �J�����̐���
	for (int nCntCamera = 0; nCntCamera < nCamera; nCntCamera++)
	{
		float fHeight;

		if (nCamera == 1)
		{
			fHeight = SCREEN_HEIGHT;
		}
		else if(nCamera == 2)
		{
			fHeight = SCREEN_HEIGHT / 2;
		}

		m_apCamera[nCntCamera] = CCamera::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			0.0f, (float)(SCREEN_HEIGHT / 2 * nCntCamera),		// �`�悷���ʂ̍���X���W�A����Y���W
			(float)SCREEN_WIDTH, fHeight);						// �`�悷���ʂ̕��A����

		// �J�����̔ԍ���ݒ�
		m_apCamera[nCntCamera]->SetNum(nCntCamera);
	}
}