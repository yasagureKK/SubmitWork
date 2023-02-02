//=============================================================================
// ���x������ [level.cpp]
// Author : ���� ��
//=============================================================================
#include "level.h"
#include "ui.h"
#include "manager.h"
#include "texture.h"
#include "sound.h"
#include "level_up.h"

//*****************************************************************************
//�}�N����`
//*****************************************************************************
#define MAX_LEVEL_POS_X		(50)		// ���x�������̈ʒu���WX
#define MAX_LEVEL_POS_Y		(100)		// ���x�������̈ʒu���WY
#define MAX_LEVEL_SIZE_X	(60)		// ���x�������̃T�C�YX
#define MAX_LEVEL_SIZE_Y	(30)		// ���x�������̃T�C�YY
#define NUMBER_SIZE_X		(30)		// �i���o�[�̃T�C�YX
#define NUMBER_SIZE_Y		(60)		// �i���o�[�̃T�C�YY
#define ICON_SIZE			(50)		// �A�C�R���̃T�C�Y

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLevel::CLevel()
{
	// �ϐ��̃N���A
	m_pos = D3DXVECTOR2(0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_emptyPos = D3DXVECTOR2(0.0f, 0.0f);
	m_emptySize = D3DXVECTOR2(0.0f, 0.0f);
	m_levelPos = D3DXVECTOR2(0.0f, 0.0f);
	memset(&m_apUI, NULL, sizeof(m_apUI));
	memset(&m_apNumber, NULL, sizeof(m_apNumber));
	memset(&m_apIconUI, NULL, sizeof(m_apIconUI));
	memset(&m_apIconNumber, NULL, sizeof(m_apIconNumber));
	m_nLevel = 0;
	m_nExp = 0;
	m_nNextLevelExp = 0;
	for (int nCntLevel = 0; nCntLevel < MAX_LEVEL; nCntLevel++)
	{
		m_nNeedExp[nCntLevel] = 0;
	}
	m_fObjectiveSize = 0.0f;
	m_fMaxSize = 0.0f;
	m_bMoment = false;
	m_bDrawNumber = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLevel::~CLevel()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CLevel::Init(D3DXVECTOR2 pos, D3DXVECTOR2 size)
{
	// �e�L�X�g�t�@�C���̓ǂݍ���
	char cString[256];
	FILE *pFile = fopen("data/TEXT/exp_line.txt", "r");

	int nCntExp = 0;
	if (pFile != NULL)
	{
		while (1)
		{
			fscanf(pFile, "%s", &cString[0]);

			if (strcmp(&cString[0], "NEED_EXP") == 0)	// NEED_EXP�̕�����
			{
				// �K�v�o���l���擾����
				fscanf(pFile, "%s", &cString[0]);
				fscanf(pFile, "%d", &m_nNeedExp[nCntExp]);
				nCntExp++;
			}

			if (strcmp(&cString[0], "END_SCRIPT") == 0) // END_SCRIPT�̕�����
			{
				break;
			}

			for (int nCntNeedExp = nCntExp; nCntNeedExp < MAX_LEVEL; nCntNeedExp++)
			{
				m_nNeedExp[nCntNeedExp] = m_nNeedExp[nCntNeedExp - 1];
			}
		}
		fclose(pFile);
	}

	// �ϐ��̏�����
	m_pos = pos;
	m_size = size;
	m_nLevel = 1;
	m_nExp = 0;
	m_nNextLevelExp = m_nNeedExp[m_nLevel];
	m_fObjectiveSize = m_size.x;
	m_fMaxSize = m_size.x;
	m_bDrawNumber = false;

	m_emptyPos = D3DXVECTOR2(m_pos.x + (m_size.x / 2), m_pos.y);
	m_emptySize = D3DXVECTOR2(m_size.x, m_size.y);
	m_levelPos = D3DXVECTOR2(MAX_LEVEL_POS_X, MAX_LEVEL_POS_Y);

	// �o���l�o�[(�g)
	m_apUI[0] = CUI::Create(m_pos, D3DXVECTOR2(m_size.x + 30.0f, m_size.y + 15.0f), 11, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// �o���l�o�[
	m_apUI[1] = CUI::Create(m_pos, m_size, 15, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// �o���l�̋󕔕�
	m_apUI[2] = CUI::Create(m_emptyPos, m_emptySize, 13, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_apUI[2]->SetBesideBarScale(m_emptySize.x, true);

	// ���x��(����)
	m_apUI[3] = CUI::Create(m_levelPos, D3DXVECTOR2(MAX_LEVEL_SIZE_X, MAX_LEVEL_SIZE_Y), 14, D3DCOLOR_RGBA(230, 0, 0, 255));

	// �����̐���
	for (int nCntScore = 0; nCntScore < MAX_LEVEL_DIGIT; nCntScore++)
	{
		m_apNumber[nCntScore] = CUI::Create(D3DXVECTOR2(m_levelPos.x + (MAX_LEVEL_SIZE_X * 0.9f) + (nCntScore * NUMBER_SIZE_X), m_levelPos.y),
			D3DXVECTOR2(NUMBER_SIZE_X, NUMBER_SIZE_Y), 8, D3DCOLOR_RGBA(230, 90, 0, 255));
	}

	// �A�C�R���Ɛ����̐���
	for (int nCntIcon = 0; nCntIcon < MAX_LEVEL_ICON_UI; nCntIcon++)
	{
		float fTexType = (float)nCntIcon;

		m_apIconUI[nCntIcon] = CUI::Create(D3DXVECTOR2(m_pos.x + (nCntIcon * ICON_SIZE) + NUMBER_SIZE_X, m_levelPos.y),
			D3DXVECTOR2(ICON_SIZE, ICON_SIZE), 38, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f));
		m_apIconUI[nCntIcon]->SetTexture(fTexType, 0, 3, 1);

		m_apIconNumber[nCntIcon] = CUI::Create(D3DXVECTOR2(m_pos.x + (nCntIcon * ICON_SIZE) + NUMBER_SIZE_X, m_levelPos.y),
			D3DXVECTOR2(NUMBER_SIZE_X * 0.8f, NUMBER_SIZE_Y * 0.8f), 8, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	}

	// ���x���A�b�v���̕␳�����Z�b�g
	CLevelUp::SetCorrectionDef(0);
	CLevelUp::SetCorrectionAtk(0);
	CLevelUp::SetCorrectionSpd(0);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CLevel::Uninit(void)
{
	// UI�̔j��
	for (int nCntUI = 0; nCntUI < MAX_LEVEL_UI; nCntUI++)
	{
		if (m_apUI[nCntUI] != NULL)
		{
			m_apUI[nCntUI]->Uninit();
		}
	}

	// �i���o�[�̔j��
	for (int nCntScore = 0; nCntScore < MAX_LEVEL_DIGIT; nCntScore++)
	{
		if (m_apNumber[nCntScore] != NULL)
		{
			m_apNumber[nCntScore]->Uninit();
		}
	}

	// �A�C�R���֘A��UI�̔j��
	for (int nCntIcon = 0; nCntIcon < MAX_LEVEL_ICON_UI; nCntIcon++)
	{
		if (m_apIconUI[nCntIcon] != NULL)
		{
			m_apIconUI[nCntIcon]->Uninit();
		}
		if (m_apIconNumber[nCntIcon] != NULL)
		{
			m_apIconNumber[nCntIcon]->Uninit();
		}
	}

	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CLevel::Update(void)
{
	m_bMoment = false;
	m_bDrawNumber = false;

	// ���݂̌o���l�������邽�тɋ�̃Q�[�W���E�ɓ������A�o���l�̑�����\������
	SetEmptyBar();

	for (int nCntNumber = 0; nCntNumber < MAX_LEVEL_DIGIT; nCntNumber++)
	{
		// �����x�� % 10�~n / 10�~(n-1)�� �ŏo���l�����ꂼ��̃i���o�[�ɐݒ�
		//��powf(float x, float y) �c x��y��̒l
		m_apNumber[nCntNumber]->SetTexture(float(m_nLevel % (int)powf(10, MAX_LEVEL_DIGIT - (float)nCntNumber) / (int)powf(10, MAX_LEVEL_DIGIT - ((float)nCntNumber + 1))), 1.0f, 10, 1);
	}

	for (int nCntNumber = 0, nCntDraw = 0; nCntNumber < MAX_LEVEL_DIGIT; nCntNumber++)
	{
		// �����̐������擾
		int nNumber = m_nLevel % (int)powf(10, MAX_LEVEL_DIGIT - (float)nCntNumber) / (int)powf(10, MAX_LEVEL_DIGIT - ((float)nCntNumber + 1));

		// ������0���A�����̕`������Ȃ��Ȃ�
		if (nNumber == 0 && m_bDrawNumber == false)
		{
			// �����ɂ��A�����Ȃ��悤�ɂ���
			m_apNumber[nCntNumber]->SetColor(D3DCOLOR_RGBA(255, 100, 0, 0));

			// 0�̐������J�E���g�𑝂₷
			nCntDraw++;
		}
		// ������0�ȊO��������
		else if(nNumber != 0)
		{
			// �����x��߂�
			m_apNumber[nCntNumber]->SetColor(D3DCOLOR_RGBA(233, 20, 0, 255));
			m_bDrawNumber = true;
		}

		// �����̍�����0���������Ԃ񂾂��󔒂��J�����߁A�������Ԃ񂾂����ɂ��炷
		m_apNumber[nCntNumber]->SetPos(D3DXVECTOR3(m_levelPos.x + (MAX_LEVEL_SIZE_X * 0.9f) + (nCntNumber * NUMBER_SIZE_X) - (nCntDraw * NUMBER_SIZE_X), m_levelPos.y, 0.0f));
	}

	if (CLevelUp::GetCorrectionAtk() > 0)
	{
		m_apIconUI[0]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		m_apIconNumber[0]->SetTexture(float(CLevelUp::GetCorrectionAtk() % (int)powf(10, 1) / (int)powf(10, 0)), 1.0f, 10, 1);
		m_apIconNumber[0]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
	if (CLevelUp::GetCorrectionDef() > 0)
	{
		m_apIconUI[1]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		m_apIconNumber[1]->SetTexture(float(CLevelUp::GetCorrectionDef() % (int)powf(10, 1) / (int)powf(10, 0)), 1.0f, 10, 1);
		m_apIconNumber[1]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
	if (CLevelUp::GetCorrectionSpd() > 0)
	{
		m_apIconUI[2]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		m_apIconNumber[2]->SetTexture(float(CLevelUp::GetCorrectionSpd() % (int)powf(10, 1) / (int)powf(10, 0)), 1.0f, 10, 1);
		m_apIconNumber[2]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CLevel::Draw(void)
{
	for (int nCntLevel = 0; nCntLevel < MAX_LEVEL_UI; nCntLevel++)
	{
		if (m_apUI[nCntLevel] != NULL)
		{
			m_apUI[nCntLevel]->Draw();
		}
	}

	for (int nCntScore = 0; nCntScore < MAX_LEVEL_DIGIT; nCntScore++)
	{
		if (m_apNumber[nCntScore] != NULL)
		{
			m_apNumber[nCntScore]->Draw();
		}
	}
}

//=============================================================================
// ��������
//=============================================================================
CLevel *CLevel::Create(D3DXVECTOR2 pos, D3DXVECTOR2 size)
{
	// �N���X�̐���
	CLevel *pLevel = NULL;
	pLevel = new CLevel;

	if (pLevel != NULL)
	{
		// ����������
		pLevel->Init(pos, size);
	}
	return pLevel;
}

//=============================================================================
// �󕔕��̐ݒ菈��
//=============================================================================
void CLevel::SetEmptyBar()
{
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// �摜�̃T�C�Y�ƕK�v�o���l�̒l���犄�����o��
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	float fRatio = m_fMaxSize / m_nNeedExp[m_nLevel];

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// ���݂̃��x���ɂ�����A���݂̌o���l�Ǝ����x���܂ł̍��v�o���l�̍������߂�
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	int nDiffExp = m_nExp;
	if (nDiffExp >= m_nNextLevelExp)
	{
		nDiffExp = m_nNextLevelExp;
	}

	int nDifferent = nDiffExp - m_nNextLevelExp;
	if (nDifferent < 0)
	{
		nDifferent *= -1;
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// �K�v�o���l�Ə�L�̕ϐ��̍������߂�
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	int nExp = m_nNeedExp[m_nLevel] - nDifferent;

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// <�ő�T�C�Y - �����~���݂̌o���l>�ŖړI�̃T�C�Y���o��
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	m_fObjectiveSize = m_fMaxSize - (fRatio * nExp);

	// �ړI�̃T�C�Y�ƌ��݂̃T�C�Y���Ⴄ�Ȃ�
	if (m_fObjectiveSize != m_emptySize.x)
	{
		// �T�C�Y�����������炷
		m_emptySize.x -= 5.0f;

		// �ړI�̃T�C�Y��0�ȉ����A���݂̃T�C�Y��0�ȉ��ɂȂ�����
		if (m_fObjectiveSize <= 0 && m_emptySize.x <= 0)
		{
			m_bMoment = true;

			// �ړI�̃T�C�Y�ƌ��݂̃T�C�Y���ő�T�C�Y�ɐݒ�
			m_fObjectiveSize = m_fMaxSize;
			m_emptySize.x = m_fMaxSize;
			m_apUI[2]->SetBesideBarScale(m_emptySize.x, true);

			// �T�E���h�擾����
			CSound *pSound = CManager::GetSound();

			// ���x���A�b�vSE�̍Đ�
			pSound->Play(13);
			pSound->ControllVoice(13, 0.8f);

			// ���x���A�b�v
			m_nLevel += 1;

			// ���̃��x���܂ł̍��v�o���l�̒l���X�V
			m_nNextLevelExp += m_nNeedExp[m_nLevel];

			CLevelUp::Create(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));
		}
		else
		{
			// ���݂̃T�C�Y���ړI�̃T�C�Y��菬����������
			if (m_emptySize.x < m_fObjectiveSize)
			{
				// ���݂̃T�C�Y��ړI�̃T�C�Y�Ɠ����l�ɐݒ�
				m_emptySize.x = m_fObjectiveSize;
			}
		}
		
		// �ő僌�x���܂œ��B������
		if (m_nLevel >= MAX_LEVEL)
		{
			// �󕔕�������
			m_fObjectiveSize = 0.0f;
			m_emptySize.x = 0.0f;
			m_apUI[2]->SetBesideBarScale(m_emptySize.x, true);
		}
		// ����ȊO�͕��ʂɍX�V
		else
		{
			m_apUI[2]->SetBesideBarScale(m_emptySize.x, true);
		}
	}
}