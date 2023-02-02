//=============================================================================
// �r���{�[�h�̃��C�t���� [life_billboard.cpp]
// Author : ���� ��
//=============================================================================
#include "life_billboard.h"
#include "ui.h"
#include "manager.h"
#include "billboard.h"
#include "texture.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLifeBillboard::CLifeBillboard()
{
	// �ϐ��̃N���A
	m_fLife = 0;
	m_fMaxLife = 0;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_emptySize = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	memset(&m_apBillboard, NULL, sizeof(m_apBillboard));
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLifeBillboard::~CLifeBillboard()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CLifeBillboard::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	m_pos = D3DXVECTOR3(pos.x, pos.y + size.y * 3.0f, pos.z);
	m_size = size;
	m_fObjectiveSize = 0.0f;
	m_fMaxSize = m_size.x;
	m_emptySize = D3DXVECTOR3(0.0f, m_size.y, 0.0f);

	SetPos(m_pos);
	SetSize(m_size);

	// ���C�t(�g)
	m_apBillboard[0] = CBillboard::Create(m_pos, D3DXVECTOR3(size.x + 15.0f, size.y + 5.0f, size.z), 7, false);

	// ���C�t(�Q�[�W)
	m_apBillboard[1] = CBillboard::Create(m_pos, m_size, 8, false);

	//  ���C�t(�󕔕�)
	m_apBillboard[2] = CBillboard::Create(m_pos, m_size, 9, false);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CLifeBillboard::Uninit(void)
{
	for (int nCntLife = 0; nCntLife < MAX_LIFE_BILLBOARD; nCntLife++)
	{
		if (m_apBillboard[nCntLife] != NULL)
		{
			// �r���{�[�h�̏I������
			m_apBillboard[nCntLife]->Uninit();
			m_apBillboard[nCntLife] = NULL;
		}
	}

	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CLifeBillboard::Update(void)
{
	for (int nCntLife = 0; nCntLife < MAX_LIFE_BILLBOARD; nCntLife++)
	{
		if (m_apBillboard[nCntLife] != NULL)
		{
			// �ʒu�̐ݒ�
			D3DXVECTOR3 pos = m_apBillboard[nCntLife]->GetPos();
			D3DXVECTOR3 newPos = D3DXVECTOR3(m_pos.x, m_pos.y + pos.y, m_pos.z);
			m_apBillboard[nCntLife]->SetPos(newPos);
		}
	}

	// ���C�t�̋󕔕���NULL����Ȃ�������
	if (m_apBillboard[2] != NULL)
	{
		// ���C�t�o�[�̐ݒ菈��
		SetLifeBar();
	}

	// ���C�t��0�����������
	if (m_fLife <= 0)
	{
		m_fLife = 0;

		// ����
		Uninit();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CLifeBillboard::Draw(void)
{
	for (int nCntLife = 0; nCntLife < MAX_LIFE_BILLBOARD; nCntLife++)
	{
		if (m_apBillboard[nCntLife] != NULL)
		{
			m_apBillboard[nCntLife]->Draw();
		}
	}
}

//=============================================================================
// ��������
//=============================================================================
CLifeBillboard *CLifeBillboard::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// �N���X�̐���
	CLifeBillboard *pLifeBill = NULL;
	pLifeBill = new CLifeBillboard;

	if (pLifeBill != NULL)
	{
		// ����������
		pLifeBill->Init(pos, D3DXVECTOR3(fSizeX, fSizeY, 0.0f));
	}
	return pLifeBill;
}

//=============================================================================
// ���C�t�o�[�̐ݒ菈��
//=============================================================================
void CLifeBillboard::SetLifeBar(void)
{
	if (m_apBillboard[2] != NULL)
	{
		D3DXVECTOR3 size = m_apBillboard[2]->GetSize();

		// �摜�̃T�C�Y�ƃ��C�t�̍ő�l���犄�����o��
		float fRatio = m_fMaxSize / m_fMaxLife;

		// <�����~���������C�t>�ŖړI�̃T�C�Y���o��
		m_fObjectiveSize = fRatio * (m_fMaxLife - m_fLife);

		// �ړI�̃T�C�Y�ƌ��݂̃T�C�Y���Ⴄ�Ȃ�
		if (m_fObjectiveSize != m_emptySize.x)
		{
			// �ړI�̃T�C�Y�����݂̃T�C�Y���傫���ꍇ
			if (m_fObjectiveSize > m_emptySize.x)
			{
				// �󕔕��̃T�C�Y�𑝂₷
				m_emptySize.x += 3.0f;

				if (m_emptySize.x > m_fObjectiveSize)
				{
					m_emptySize.x = m_fObjectiveSize;
				}
			}
		}

		// �󕔕��̒��_���W��ύX
		m_apBillboard[2]->SetVertex(D3DXVECTOR3(+size.x / 2.0f - m_emptySize.x, size.y / 2, 0.0f),
			D3DXVECTOR3(+size.x / 2.0f, size.y / 2, 0.0f),
			D3DXVECTOR3(+size.x / 2.0f - m_emptySize.x, -size.y / 2, 0.0f),
			D3DXVECTOR3(+size.x / 2.0f, -size.y / 2, 0.0f));
	}
}