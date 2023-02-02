//=============================================================================
// �Q�[����� [game.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "game.h"
#include "manager.h"
#include "light.h"
#include "player.h"
#include "data_stage.h"
#include "enemy.h"
#include "enemy_box.h"
#include "pause.h"
#include "sound.h"
#include "fog.h"
#include "score.h"
#include "time.h"
#include "clear.h"
#include "start.h"
#include "keyboard.h"
#include "collision_sphere.h"
#include "data_clear.h"
#include "camera.h"
#include "particle_hit.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
bool CGame::m_bDefeatBoss = false;
bool CGame::m_bBossBattle = false;
bool CGame::m_bClear = false;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CGame::CGame()
{
	// �ϐ��̃N���A
	m_pPlayer = NULL;
	m_pDataStage = NULL;
	memset(&m_pEnemyBox, NULL, sizeof(m_pEnemyBox));
	m_pPause = NULL;
	m_pFog = NULL;
	m_pScore = NULL;
	m_pTime = NULL;
	memset(&m_apLight, NULL, sizeof(m_apLight));
	m_pStart = NULL;
	m_bBossBattle = false;
	m_bDefeatBoss = false;
	m_bClear = false;
	nCntGame = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CGame::Init(D3DXVECTOR3 pos)
{
	// �ϐ��̏�����
	m_bBossBattle = false;
	m_bDefeatBoss = false;
	m_bClear = false;
	nCntGame = 0;

	//�T�E���h�擾����
	CSound *pSound = CManager::GetSound();

	// �Q�[��BGM�̍Đ�
	pSound->Play(1);
	pSound->ControllVoice(1, 0.3f);

	// �J�����𑀍�s�\�ɂ���
	CCamera::SetControl(false);

	// �X�R�A�̏�����
	CManager::GetDataClear()->SetScore(0);

	// ���C�g�̐���
	m_apLight[0] = CLight::Create(D3DXVECTOR3(0.3f, -0.8f, -0.5f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_apLight[1] = CLight::Create(D3DXVECTOR3(-0.7f, -0.8f, -0.2f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_apLight[2] = CLight::Create(D3DXVECTOR3(0.1f, -0.8f, 0.5f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_apLight[3] = CLight::Create(D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// �t�H�O�̐���
	m_pFog = CFog::Create(D3DCOLOR_RGBA(0, 0, 0, 255), 0.5f, 3700.0f);

	// �X�e�[�W�f�[�^�̐���
	m_pDataStage = CDataStage::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), CDataStage::STAGE_TYPE_GAME);

	// �v���C���[�̐���
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));

	// �G�l�~�[�{�b�N�X�̐���
	m_pEnemyBox[0] = CEnemyBox::Create(D3DXVECTOR3(-3000.0f, 0.0f, 3100.0f), D3DXVECTOR3(800.0f, 550.0f, 800.0f), 0);
	m_pEnemyBox[1] = CEnemyBox::Create(D3DXVECTOR3(5100.0f, 0.0f, 4700.0f), D3DXVECTOR3(700.0f, 500.0f, 700.0f), 1);
	m_pEnemyBox[2] = CEnemyBox::Create(D3DXVECTOR3(0.0f, 0.0f, 6300.0f), D3DXVECTOR3(800.0f, 500.0f, 800.0f), 2);
	m_pEnemyBox[3] = CEnemyBox::Create(D3DXVECTOR3(0.0f, 0.0f, 10000.0f), D3DXVECTOR3(1500.0f, 500.0f, 1500.0f), 3);
	
	// �|�[�Y�̐���
	m_pPause = CPause::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// �|�[�Y��s�\�ɂ���
	CPause::SetPossiblePause(false);

	// �X�R�A�̐���
	m_pScore = CScore::Create(D3DXVECTOR2(70.0f, 145.0f), D3DXVECTOR2(130.0f, 35.0f));

	// �^�C���̐���
	m_pTime = CTime::Create(D3DXVECTOR2(1100.0f, 40.0f), D3DXVECTOR2(120.0f, 55.0f));
	m_pTime->SetTimeStop(true);

	// �X�^�[�gUI�̐���
	m_pStart = CUIStart::Create(D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), D3DXVECTOR2(800.0f, 200.0f));
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CGame::Uninit(void)
{
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

	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CGame::Update(void)
{
	if (m_pStart != NULL &&  m_pStart->GetStartMoment() == true)
	{
		//�T�E���h�擾����
		CSound *pSound = CManager::GetSound();

		// �X�^�[�gSE�̍Đ�
		pSound->Play(30);
		pSound->ControllVoice(30, 0.6f);

		// �|�[�Y���\�ɂ���
		CPause::SetPossiblePause(true);

		// �v���C���[�𑀍�\�ɂ���
		m_pPlayer->SetCanControl(true);

		// �J�����𑀍�\�ɂ���
		CCamera::SetControl(true);

		if (m_pTime != NULL)
		{
			// �^�C����i�܂���
			m_pTime->SetTimeStop(false);
		}
	}

	// �v���C���[�̃��C�t��0�ɂȂ�����
	if (m_pPlayer->GetDeath() == true)
	{
		if (m_pTime != NULL)
		{
			// �^�C�����~�߂�
			m_pTime->SetTimeStop(true);
		}
	}

	// �{�X�Ƃ̐킢���n�܂�����
	if (m_bBossBattle == true)
	{
		// �J�����̓������{�X��̓����ɐݒ�
		CManager::GetCamera()->SetCameraMove(CCamera::CAMERA_MOVE_TYPE_BOSS);
	}
	
	// �{�X��|������
	if (m_bDefeatBoss == true)
	{
		if (nCntGame == 0)
		{
			// �J�����̓������v���C���[�Ǐ]�ɖ߂�
			CManager::GetCamera()->SetCameraMove(CCamera::CAMERA_MOVE_TYPE_PLAYER);

			if (m_pTime != NULL)
			{
				// �^�C�����~�߂�
				m_pTime->SetTimeStop(true);
			}
		}

		// �J�E���g��i�߂�
		nCntGame++;

		// 2�b�o�߂�����
		if (nCntGame == 120)
		{
			if (m_pTime != NULL)
			{
				// �^�C���ƃv���C���[�̃��x�����擾
				int nTime = m_pTime->GetTime();
				int nLevel = m_pPlayer->GetLevel();

				// �N���A���̃f�[�^����X�R�A��ݒ�
				CManager::GetDataClear()->SetNewScore(nTime, nLevel);
			}

			// �N���A�̏�Ԃɂ���
			m_bClear = true;

			// �J�����̓������N���A���̓����ɂ���
			CManager::GetCamera()->SetCameraMove(CCamera::CAMERA_MOVE_TYPE_CLEAR);

			// �|�[�Y��s�\�ɂ���
			CPause::SetPossiblePause(false);

			// �X�R�A��UI������
			if (m_pScore != NULL)
			{
				m_pScore->Uninit();
				delete m_pScore;
				m_pScore = NULL;
			}
			
			// �^�C����UI������
			if (m_pTime != NULL)
			{
				m_pTime->Uninit();
				delete m_pTime;
				m_pTime = NULL;
			}

			// �N���A��ʂ̐���
			CClear::Create();
		}
	}

#ifdef _DEBUG
	//�L�[�{�[�h�擾����
	CKeyboard *pKeyboard = CManager::GetKeyboard();

	if (pKeyboard->GetTrigger(DIK_F2) == true)
	{
		CCollisionSphere::SetVisual();
	}
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void CGame::Draw(void)
{

}

//=============================================================================
// ��������
//=============================================================================
CGame *CGame::Create(void)
{
	// �N���X�̐���
	CGame *pGame = NULL;
	pGame = new CGame;

	if (pGame != NULL)
	{
		// ����������
		pGame->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
	return pGame;
}

//=============================================================================
// �v���C���[�擾����
//=============================================================================
CPlayer *CGame::GetPlayer(void)
{
	return m_pPlayer;
}

//=============================================================================
// �{�X�킩�ǂ����ݒ菈��
//=============================================================================
void CGame::SetBossBattle(bool bDefeat)
{
	m_bBossBattle = bDefeat;

	// �{�X��t���O��true�ɂȂ�����
	if (m_bBossBattle == true)
	{
		if (m_pFog != NULL)
		{
			// ���͈̔͂𒲐�
			m_pFog->SetFogLine(1500.0f, 5000.0f);
		}

		//�T�E���h�擾����
		CSound *pSound = CManager::GetSound();

		// �Q�[��BGM�̒�~
		pSound->Stop(1);

		// �{�XBGM�̍Đ�
		pSound->Play(2);
		pSound->ControllVoice(2, 0.3f);
	}
}