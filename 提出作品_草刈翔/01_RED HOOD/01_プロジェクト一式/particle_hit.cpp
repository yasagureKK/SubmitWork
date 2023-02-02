//=============================================================================
// �p�[�e�B�N��(�q�b�g)���� [particle_hit.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "particle_hit.h"
#include "manager.h"
#include "renderer.h"
#include "effect_billboard.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CParticleHit::CParticleHit()
{
	// �ϐ��̃N���A
	memset(&pEffectB, NULL, sizeof(pEffectB));
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DCOLOR_RGBA(255, 255, 255, 255);
	m_nLife = 0;
	m_fSpeed = 0.0f;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CParticleHit::~CParticleHit()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CParticleHit::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// �ϐ��̏�����
	m_pos = pos;
	m_size = size;

	for (int nCntParticle = 0; nCntParticle < m_nParticleNum; nCntParticle++)
	{
		// -3.14�`3.14�̒l�������_���ŏo��
		float fAngleX = (-314 + rand() % 618) / 100.0f;
		float fAngleZ = (-314 + rand() % 618) / 100.0f;

		// 
		pEffectB[nCntParticle] = CEffectB::Create(m_pos,
			m_size, m_col, D3DXVECTOR3((float)(sinf(fAngleX) * -cos(fAngleZ) * m_fSpeed),
												cosf(fAngleX) * m_fSpeed,
										(float)(sinf(fAngleX) * -cos(fAngleZ + D3DX_PI / 2.0f) * m_fSpeed)),
			m_nLife, CEffectB::EFFECT_TYPE_SMALL, NULL);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CParticleHit::Uninit(void)
{
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CParticleHit::Update(void)
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
void CParticleHit::Draw(void)
{

}

//=============================================================================
// ��������
//=============================================================================
CParticleHit *CParticleHit::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, D3DXCOLOR col,
	int nLife, float fSpeed, int nNum)
{
	// �N���X�̐���
	CParticleHit *pParticleHit = NULL;
	pParticleHit = new CParticleHit;

	if (pParticleHit != NULL)
	{
		// �ϐ��̏�����
		pParticleHit->m_rot = rot;
		pParticleHit->m_col = col;
		pParticleHit->m_nLife = nLife;
		pParticleHit->m_fSpeed = fSpeed;
		pParticleHit->m_nParticleNum = nNum;

		// ����������
		pParticleHit->Init(pos, size);
	}
	return pParticleHit;
}