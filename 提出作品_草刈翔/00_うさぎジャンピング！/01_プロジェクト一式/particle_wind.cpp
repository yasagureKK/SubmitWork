//=============================================================================
// �p�[�e�B�N��(�E�B���h)���� [particle_wind.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "particle_wind.h"
#include "manager.h"
#include "renderer.h"
#include "effect2D.h"
#include "texture.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CParticleWind::CParticleWind()
{
	// �ϐ��̃N���A
	memset(&pEffect2D, NULL, sizeof(pEffect2D));
	m_nNum = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CParticleWind::~CParticleWind()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CParticleWind::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// �ϐ��̏�����
	m_pos = pos;
	m_size = size;

	if (CManager::GetMode() == CManager::MODE_GAME || CManager::GetMode() == CManager::MODE_TUTORIAL)
	{
		// �ʒu��10�`710�̒l���烉���_���ɐݒ�
		m_pos.y = float(rand() % 700) + 10;
	}
	else if (CManager::GetMode() == CManager::MODE_PVP || CManager::GetMode() == CManager::MODE_SOLO_CPU)
	{
		if (m_nNum == 0)
		{
			// �ʒu��10�`350�̒l���烉���_���ɐݒ�
			m_pos.y = float(rand() % 340) + 10;
		}
		else if (m_nNum == 1)
		{
			// �ʒu��370�`710�̒l���烉���_���ɐݒ�
			m_pos.y = float(rand() % 700) + 370;
		}
	}

	// ���S�ʒu����~�`�ɍL����悤�Ɉړ��ʂ�ݒ�
	pEffect2D = CEffect2D::Create(m_pos, m_size, m_col, m_move, 10000);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CParticleWind::Uninit(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void CParticleWind::Update(void)
{
	if (pEffect2D != NULL)
	{
		// �ʒu�E�T�C�Y�擾
		D3DXVECTOR3 pos = pEffect2D->GetPos();

		// ��ʍ��[�ɂ�������
		if (pos.x < 0)
		{
			// ���ł�����
			pEffect2D->Uninit();
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CParticleWind::Draw(void)
{

}

//=============================================================================
// ��������
//=============================================================================
CParticleWind* CParticleWind::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR3 move, int nNum)
{
	// �C���X�^���X�̐���
	CParticleWind *pParticleWind = NULL;

	// �k���`�F�b�N
	if (pParticleWind == NULL)
	{
		// �D�揇�ʂ��������ăN���X�𐶐�
		pParticleWind = new CParticleWind;
		if (pParticleWind != NULL)
		{
			// �ϐ��̏�����
			pParticleWind->m_move = move;
			pParticleWind->m_col = col;
			pParticleWind->m_nNum = nNum;

			// ����������
			pParticleWind->Init(pos, size);
		}
	}

	return pParticleWind;
}