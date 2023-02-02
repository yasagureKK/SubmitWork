//=============================================================================
// ���C�t���� [life.cpp]
// Author : ���� ��
//=============================================================================
#include "life.h"
#include "ui.h"
#include "manager.h"
#include "texture.h"

//*****************************************************************************
//�}�N����`
//*****************************************************************************
#define NUMBER_SIZE_X	(20)	// �i���o�[�̃T�C�YX
#define NUMBER_SIZE_Y	(30)	// �i���o�[�̃T�C�YY

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLife::CLife()
{
	// �ϐ��̃N���A
	m_fLife = 0.0f;
	m_fMaxLife = 0.0f;
	m_fObjectiveSize = 0.0f;
	m_fMaxSize = 0.0f;
	m_pos = D3DXVECTOR2(0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_emptyPos = D3DXVECTOR2(0.0f, 0.0f);
	m_emptySize = D3DXVECTOR2(0.0f, 0.0f);
	memset(&m_apUI, NULL, sizeof(m_apUI));
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLife::~CLife()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CLife::Init(D3DXVECTOR2 pos, D3DXVECTOR2 size, LIFE_TYPE type)
{
	// �ϐ��̏�����
	m_pos = pos;
	m_size = D3DXVECTOR2(size.x, size.y);
	m_fObjectiveSize = 0.0f;
	m_fMaxSize = m_size.x;
	m_emptyPos = D3DXVECTOR2(m_pos.x + (m_size.x / 2), m_pos.y);
	m_emptySize = D3DXVECTOR2(0.0f, m_size.y);

	int nLifeCaseTex = 0, LifeBarTex = 0;
	float fCaseSizeX = 0.0f, fCaseSizeY = 0;
	if (type == LIFE_TYPE_PLAYER)
	{
		nLifeCaseTex = 11;
		LifeBarTex = 9;
		fCaseSizeX = 30.0f;
		fCaseSizeY = 15.0f;
	}
	else if (type == LIFE_TYPE_BOSS)
	{
		nLifeCaseTex = 12;
		LifeBarTex = 10;
		fCaseSizeX = 60.0f;
		fCaseSizeY = 20.0f;
	}

	// ���C�t(�g)
	m_apUI[0] = CUI::Create(m_pos, D3DXVECTOR2(m_size.x + fCaseSizeX, m_size.y + fCaseSizeY), nLifeCaseTex, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// ���C�t(�Q�[�W)
	m_apUI[1] = CUI::Create(m_pos, m_size, LifeBarTex, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// ���C�t(�󕔕�)
	m_apUI[2] = CUI::Create(m_emptyPos, m_emptySize, 13, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CLife::Uninit(void)
{
	// UI�̏I������
	for (int nCntUI = 0; nCntUI < MAX_LIFE_POLYGON; nCntUI++)
	{
		if (m_apUI[nCntUI] != NULL)
		{
			m_apUI[nCntUI]->Uninit();
		}
	}

	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CLife::Update(void)
{
	// ���C�t�o�[�̍X�V
	SetLifeBar();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CLife::Draw(void)
{
	for (int nCntLife = 0; nCntLife < MAX_LIFE_POLYGON; nCntLife++)
	{
		if (m_apUI[nCntLife] != NULL)
		{
			m_apUI[nCntLife]->Draw();
		}
	}
}

//=============================================================================
// ��������
//=============================================================================
CLife *CLife::Create(D3DXVECTOR2 pos, D3DXVECTOR2 size, LIFE_TYPE type)
{
	// �N���X�̐���
	CLife *pLife = NULL;
	pLife = new CLife;

	if (pLife != NULL)
	{
		// ����������
		pLife->Init(pos, size, type);
	}
	return pLife;
}

//=============================================================================
// ���C�t�o�[�̐ݒ菈��
//=============================================================================
void CLife::SetLifeBar(void)
{
	// �摜�̃T�C�Y�ƃ��C�t�̍ő�l���犄���̒l���o��
	float fRatio = m_fMaxSize / m_fMaxLife;

	// <�����~���������C�t>�ŖړI�̃T�C�Y���o��
	m_fObjectiveSize = fRatio * (m_fMaxLife - m_fLife);

	// ���C�t��0�ɂȂ�����
	if (m_fLife <= 0)
	{
		// �T�C�Y�������ɕύX���A�Q�[�W������
		m_emptySize.x = m_fObjectiveSize;
		m_apUI[2]->SetBesideBarScale(m_emptySize.x, true);
	}

	// �ړI�̃T�C�Y�Ƌ󕔕��̃T�C�Y���Ⴄ�Ȃ�
	if (m_fObjectiveSize != m_emptySize.x)
	{
		// �ړI�̃T�C�Y���󕔕��̃T�C�Y���傫���ꍇ
		if (m_fObjectiveSize > m_emptySize.x)
		{
			// �󕔕��̃T�C�Y�𑝂₷
			m_emptySize.x += 5.0f;

			if (m_emptySize.x > m_fObjectiveSize)
			{
				m_emptySize.x = m_fObjectiveSize;
			}
		}

		if (m_fObjectiveSize < m_emptySize.x)
		{
			// �󕔕��̃T�C�Y�����炷
			m_emptySize.x -= 5.0f;

			if (m_emptySize.x < m_fObjectiveSize)
			{
				m_emptySize.x = m_fObjectiveSize;
			}
		}

		// �󕔕��̃T�C�Y��ύX
 		m_apUI[2]->SetBesideBarScale(m_emptySize.x, true);
	}
}