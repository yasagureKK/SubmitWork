//=============================================================================
// ���x���A�b�v���� [level_up.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "level_up.h"
#include "manager.h"
#include "texture.h"
#include "ui.h"
#include "fade.h"
#include "keyboard.h"
#include "gamepad.h"
#include "mouse.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define LEVEL_UP_TOP_POS_Y			(120.0f)	// �g�b�v������Y���W�ʒu
#define LEVEL_UP_TOP_SIZE_X			(650.0f)	// �g�b�v�����̃T�C�YX
#define LEVEL_UP_TOP_SIZE_Y			(170.0f)	// �g�b�v�����̃T�C�YY
#define LEVEL_UP_SELECT_POS_X		(320.0f)	// �I������X���W�ʒu
#define LEVEL_UP_SELECT_POS_Y		(360.0f)	// �I������Y���W�ʒu
#define LEVEL_UP_SELECT_SIZE_X		(360.0f)	// �I�����̃T�C�YX
#define LEVEL_UP_SELECT_SIZE_Y		(250.0f)	// �I�����̃T�C�YY
#define LEVEL_UP_SENTENCE_POS_Y		(630.0f)	// ���ڐ������̈ʒuY
#define LEVEL_UP_SENTENCE_SIZE_X	(700.0f)	// ���ڐ������̃T�C�YX
#define LEVEL_UP_SENTENCE_SIZE_Y	(100.0f)	// ���ڐ������̃T�C�YY

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
int CLevelUp::m_nCorrectionDef = 0;
int CLevelUp::m_nCorrectionAtk = 0;
int CLevelUp::m_nCorrectionSpd = 0;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLevelUp::CLevelUp(int nPriority) :CScene(nPriority)
{
	// �ϐ��̃N���A
	memset(&m_apUI, NULL, sizeof(m_apUI));
	memset(&m_apSelectUI, NULL, sizeof(m_apSelectUI));
	m_nSelect = 0;
	m_nFlash = 0;
	m_Phase = LEVELUP_PHASE_BG;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLevelUp::~CLevelUp()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CLevelUp::Init(D3DXVECTOR3 pos)
{
	// �ϐ��̏�����
	m_nSelect = 0;
	m_nFlash = 0;
	m_Phase = LEVELUP_PHASE_BG;

	// �|�[�Y��Ԃɂ���
	CManager::SetPause(true);

	// �w�i�E�g�b�v����
	m_apUI[0] = CUI::Create(D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), 16, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	m_apUI[1] = CUI::Create(D3DXVECTOR2(SCREEN_WIDTH / 2, LEVEL_UP_TOP_POS_Y), D3DXVECTOR2(LEVEL_UP_TOP_SIZE_X, LEVEL_UP_TOP_SIZE_Y), 34, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	// ������
	m_apUI[2] = CUI::Create(D3DXVECTOR2(SCREEN_WIDTH / 2, LEVEL_UP_SENTENCE_POS_Y), D3DXVECTOR2(LEVEL_UP_SENTENCE_SIZE_X, LEVEL_UP_SENTENCE_SIZE_Y), 36, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	m_apUI[2]->SetTexture(0, 0, 1, 3);

	// �I����
	for (int nCntIcon = 0; nCntIcon < LEVELUP_MAX_SELECT; nCntIcon++)
	{
		float fTexType = (float)nCntIcon;
		m_apSelectUI[nCntIcon] = CUI::Create(D3DXVECTOR2(LEVEL_UP_SELECT_POS_X + (nCntIcon * LEVEL_UP_SELECT_POS_X), LEVEL_UP_SELECT_POS_Y), D3DXVECTOR2(LEVEL_UP_SELECT_SIZE_X, LEVEL_UP_SELECT_SIZE_Y), 35, D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.0f));
		m_apSelectUI[nCntIcon]->SetTexture(fTexType, 0, 3, 1);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CLevelUp::Uninit(void)
{
	// �|�[�Y��Ԃ�����
	CManager::SetPause(false);

	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CLevelUp::Update(void)
{
	// �L�[�{�[�h�擾����
	CKeyboard *pKeyboard = CManager::GetKeyboard();
	// �Q�[���p�b�h�擾����
	CGamePad *pGamePad = CManager::GetGamePad();

	//�T�E���h�擾����
	CSound *pSound = CManager::GetSound();

	switch (m_Phase)
	{
	case LEVELUP_PHASE_BG:
		// �A���t�@�x���グ��
		m_nAlpha += 8;

		if (m_nAlpha >= 255)
		{
			m_nAlpha = 255;
		}

		// �摜�̃A���t�@�x��ύX���A�������\��������
		for (int nCntClear = 0; nCntClear < LEVELUP_MAX_UI; nCntClear++)
		{
			if (m_apUI[nCntClear] != NULL)
			{
				m_apUI[nCntClear]->SetColor(D3DCOLOR_RGBA(255, 255, 255, m_nAlpha));
			}
		}

		for (int nCntSelect = 0; nCntSelect < LEVELUP_MAX_SELECT; nCntSelect++)
		{
			if (m_apSelectUI[nCntSelect] != NULL)
			{
				m_apSelectUI[nCntSelect]->SetColor(D3DCOLOR_RGBA(127, 127, 127, m_nAlpha));
			}
		}

		// ���S�ɕs�����ɂȂ�����
		if (m_nAlpha == 255)
		{
			m_Phase = LEVELUP_PHASE_SELECT;
		}
		break;

	case LEVELUP_PHASE_SELECT:
		//***********************************************************************
		// �I�� (�`�L �[/ �c�L�[ �܂��� �p�b�h���X�e�B�b�N)
		//***********************************************************************
		//��
		if (pKeyboard->GetTrigger(DIK_A) == true ||
			pGamePad->GetTrigger(CGamePad::PAD_INPUT_LSTICK_LEFT, 0) == true)
		{
			// �I��SE�̍Đ�
			pSound->Play(27);
			pSound->ControllVoice(27, 0.4f);

			m_nSelect--;
		}
		//�E
		else if (pKeyboard->GetTrigger(DIK_D) == true ||
			pGamePad->GetTrigger(CGamePad::PAD_INPUT_LSTICK_RIGHT, 0) == true)
		{
			// �I��SE�̍Đ�
			pSound->Play(27);
			pSound->ControllVoice(27, 0.4f);

			m_nSelect++;
		}

		// �I�����̐���
		if (m_nSelect < 0)
		{
			m_nSelect = LEVELUP_MAX_SELECT - 1;
		}
		else if (m_nSelect >= LEVELUP_MAX_SELECT)
		{
			m_nSelect = 0;
		}

		for (int nCntUi = 0; nCntUi < LEVELUP_MAX_SELECT; nCntUi++)
		{
			if (m_apSelectUI[nCntUi] != NULL)
			{
				// �I������Ă���UI��
				if (nCntUi == m_nSelect)
				{
					// �F�𖾂邭����
					m_apSelectUI[nCntUi]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
				else
				{
					// �F���Â�����
					m_apSelectUI[nCntUi]->SetColor(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
				}
			}
		}

		if (m_apUI[2] != NULL)
		{
			// �������̃e�N�X�`�����X�V
			m_apUI[2]->SetTexture(0, (float)m_nSelect, 1, 3);
		}

		//***********************************************************************
		// �I�� (Space�L�[ �܂��� �p�b�hA�{�^��)
		//***********************************************************************
		if (pKeyboard->GetTrigger(DIK_SPACE) == true ||
			pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_A, 0) == true)
		{
			// �I������Ă������ڂŏグ��X�e�[�^�X��ς���
			if (m_nSelect == 0)
			{
				// �U���␳�l�̏㏸
				m_nCorrectionAtk++;
			}
			else if (m_nSelect == 1)
			{
				// �h��␳�l�̏㏸
				m_nCorrectionDef++;
			}
			else if (m_nSelect == 2)
			{
				// ���x�␳�l�̏㏸
				m_nCorrectionSpd++;
			}

			// ���x���A�b�v���̌���SE�̍Đ�
			pSound->Play(14);
			pSound->ControllVoice(14, 0.4f);

			m_Phase = LEVELUP_PHASE_FINISH;
		}
		break;

	case LEVELUP_PHASE_FINISH:
		// �_�ŗp�̃J�E���g�𑝂₷
		m_nFlash++;

		// �_�ŗp�̃J�E���g����萔�o������
		if (m_nFlash >= 30)
		{
			// �A���t�@�x�����炷
			m_nAlpha -= 12;
		}

		if (m_nAlpha <= 0)
		{
			m_nAlpha = 0;
		}

		// �摜�̃A���t�@�x��ύX���A�����Ă���
		for (int nCntClear = 0; nCntClear < LEVELUP_MAX_UI; nCntClear++)
		{
			if (m_apUI[nCntClear] != NULL)
			{
				m_apUI[nCntClear]->SetColor(D3DCOLOR_RGBA(255, 255, 255, m_nAlpha));
			}
		}

		for (int nCntSelect = 0; nCntSelect < LEVELUP_MAX_SELECT; nCntSelect++)
		{
			if (m_apSelectUI[nCntSelect] != NULL)
			{
				// �I������Ă���UI��
				if (nCntSelect == m_nSelect)
				{
					if (m_nFlash % 6 == 0 && m_nFlash != 0)
					{
						// �_��
						m_apSelectUI[m_nSelect]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 0));
					}
					else
					{
						// �_��
						m_apSelectUI[m_nSelect]->SetColor(D3DCOLOR_RGBA(255, 255, 255, m_nAlpha));
					}
				}
				else
				{
					m_apSelectUI[nCntSelect]->SetColor(D3DCOLOR_RGBA(127, 127, 127, m_nAlpha));
				}
			}
		}

		// ���S�ɓ����ɂȂ�����
		if (m_nAlpha == 0)
		{
			// UI�̔j��
			for (int nCntUi = 0; nCntUi < LEVELUP_MAX_UI; nCntUi++)
			{
				if (m_apUI[nCntUi] != NULL)
				{
					m_apUI[nCntUi]->Uninit();
				}
			}

			for (int nCntUi = 0; nCntUi < LEVELUP_MAX_SELECT; nCntUi++)
			{
				if (m_apSelectUI[nCntUi] != NULL)
				{
					m_apSelectUI[nCntUi]->Uninit();
				}
			}

			// �I������
			Uninit();
		}
		break;

	default:
		break;
	}

	if (m_nCorrectionAtk > 9)
	{
		m_nCorrectionAtk = 9;
	}
	if (m_nCorrectionDef > 9)
	{
		m_nCorrectionDef = 9;
	}
	if (m_nCorrectionSpd > 9)
	{
		m_nCorrectionSpd = 9;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CLevelUp::Draw(void)
{

}

//=============================================================================
// ��������
//=============================================================================
CLevelUp *CLevelUp::Create(D3DXVECTOR3 pos)
{
	// �N���X�̐���
	CLevelUp *pPause = NULL;
	pPause = new CLevelUp;

	if (pPause != NULL)
	{
		// ����������
		pPause->Init(pos);
	}
	return pPause;
}