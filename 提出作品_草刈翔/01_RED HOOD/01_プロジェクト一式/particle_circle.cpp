//=============================================================================
// �p�[�e�B�N��(�~�`)���� [particle_circle.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "particle_circle.h"
#include "manager.h"
#include "renderer.h"
#include "effect_billboard.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CParticleCircle::CParticleCircle()
{
	// �ϐ��̃N���A
	memset(&pEffectB, NULL, sizeof(pEffectB));
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DCOLOR_RGBA(255, 255, 255, 255);
	m_nLife = 0;
	m_nParticleNum = 0;
	m_fSpeed = 0.0f;
	m_fAngle = 0.0f;
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
	m_fAngle = 0;

	// �p�x�֘A�̕ϐ��̏�����
	m_fAngle = 0;
	for (int nCntParticle = 0; nCntParticle < m_nParticleNum; nCntParticle++)
	{
		// ���S�ʒu����~�`�ɍL����悤�Ɉړ��ʂ�ݒ�
		pEffectB[nCntParticle] = CEffectB::Create(m_pos, m_size, m_col,
			D3DXVECTOR3(cosf(m_fAngle) * m_fSpeed, 0.0f, sinf(m_fAngle) * m_fSpeed),
			m_nLife, CEffectB::EFFECT_TYPE_VANISH, NULL);
		// �p�x��ύX
		m_fAngle += (D3DX_PI * 2.0f / m_nParticleNum);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CParticleCircle::Uninit(void)
{
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CParticleCircle::Update(void)
{
	for (int nCntParticle = 0; nCntParticle < m_nParticleNum; nCntParticle++)
	{
		if (pEffectB[nCntParticle] != NULL)
		{
			// �p�[�e�B�N���̎������擾
			int nLife = pEffectB[nCntParticle]->GetLife();

			// �p�[�e�B�N���̎������s������
			if (nLife <= 0)
			{
				pEffectB[nCntParticle] = NULL;
			}
		}

		// �Ō�̃p�[�e�B�N����NULL�ɂȂ�����
		if (pEffectB[m_nParticleNum - 1] == NULL)
		{
			// �I������
			Uninit();
		}
	}
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
CParticleCircle *CParticleCircle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, int nLife, float fSpeed, int nNum)
{
	// �N���X�̐���
	CParticleCircle *pParticleCircle = NULL;
	pParticleCircle = new CParticleCircle;

	if (pParticleCircle != NULL)
	{
		// �ϐ��̏�����
		pParticleCircle->m_col = col;
		pParticleCircle->m_nLife = nLife;
		pParticleCircle->m_fSpeed = fSpeed;
		pParticleCircle->m_nParticleNum = nNum;

		// ����������
		pParticleCircle->Init(pos, size);
	}
	return pParticleCircle;
}