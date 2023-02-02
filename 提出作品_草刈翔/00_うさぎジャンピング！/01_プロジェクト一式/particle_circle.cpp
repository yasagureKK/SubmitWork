//=============================================================================
// �p�[�e�B�N��(�T�[�N��)���� [particle_circle.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "particle_circle.h"
#include "manager.h"
#include "renderer.h"
#include "billboard_effect.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CParticleCircle::CParticleCircle()
{
	// �ϐ��̃N���A
	memset(&pBillboardEffect, NULL, sizeof(pBillboardEffect));
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CParticleCircle::~CParticleCircle()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CParticleCircle::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// �ϐ��̏�����
	m_pos = pos;
	m_size = size;

	// �p�x�֘A�̕ϐ��̏�����
	m_fAngle = atan2f(100.0f, 100.0f);
	m_fAddAngle = 0;

	for (int nCntParticle = 0; nCntParticle < 90; nCntParticle++)
	{
		// ���S�ʒu����~�`�ɍL����悤�Ɉړ��ʂ�ݒ�
		pBillboardEffect[nCntParticle] = CBillboardEffect::Create(m_pos, m_size, m_col,
			D3DXVECTOR3(cosf(m_fAngle - m_fAddAngle) * m_fSpeed, 0.0f, sinf(m_fAngle - m_fAddAngle) * m_fSpeed),
			m_nLife, CBillboardEffect::EFFECT_TYPE_NORMAL);

		// �p�x��ύX
		m_fAddAngle += (D3DX_PI / (90 / 2.0f));

		// ���₵���p�x���P���������ɂȂ�����I��
		if (m_fAddAngle >= D3DX_PI * 2)
		{
			break;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CParticleCircle::Uninit(void)
{
	
}

//=============================================================================
// �X�V����
//=============================================================================
void CParticleCircle::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CParticleCircle::Draw(void)
{

}

//=============================================================================
// ��������
//=============================================================================
CParticleCircle* CParticleCircle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, D3DXCOLOR col,
	int nLife, float fSpeed)
{
	// �C���X�^���X�̐���
	CParticleCircle *pParticleCircle = NULL;

	// �k���`�F�b�N
	if (pParticleCircle == NULL)
	{
		// �D�揇�ʂ��������ăN���X�𐶐�
		pParticleCircle = new CParticleCircle;
		if (pParticleCircle != NULL)
		{
			// �ϐ��̏�����
			pParticleCircle->m_rot = rot;
			pParticleCircle->m_col = col;
			pParticleCircle->m_nLife = nLife;
			pParticleCircle->m_fSpeed = fSpeed;

			// ����������
			pParticleCircle->Init(pos, size);
		}
	}

	return pParticleCircle;
}