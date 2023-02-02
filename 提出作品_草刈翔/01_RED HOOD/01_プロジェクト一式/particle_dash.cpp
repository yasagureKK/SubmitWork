//=============================================================================
// �p�[�e�B�N��(�_�b�V��)���� [particle_dash.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "particle_dash.h"
#include "manager.h"
#include "renderer.h"
#include "effect_billboard.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CParticleDash::CParticleDash()
{
	// �ϐ��̃N���A
	memset(&pEffectB_Circle, NULL, sizeof(pEffectB_Circle));
	memset(&pEffectB_Wind, NULL, sizeof(pEffectB_Wind));
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DCOLOR_RGBA(255, 255, 255, 255);
	m_nLife = 0;
	m_fSpeed = 0.0f;
	m_fAngle = 0.0f;
	m_fRotOffset = 0.0f;
	m_fCircleOffset = 0.0f;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CParticleDash::~CParticleDash()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CParticleDash::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// �ϐ��̏�����
	m_pos = pos;
	m_size = size;
	m_fAngle = 0;

	for (int nCntParticle = 0; nCntParticle < m_nParticleNum; nCntParticle++)
	{
		D3DXVECTOR3 pos = m_pos;
		pos.x += -sinf(m_rot.y) * m_fRotOffset;
		pos.z += -cosf(m_rot.y) * m_fRotOffset;

		// -3.14�`3.14�̒l�������_���ŏo��
		float fAngle = (-314 + rand() % 618) / 100.0f;

		// �v���C���[�̌����ɍ��킹�ăG�t�F�N�g�𐶐�
		pEffectB_Wind[nCntParticle] = CEffectB::Create(D3DXVECTOR3(pos.x + cosf(fAngle) * -cosf(m_rot.y) * m_fCircleOffset,
																   pos.y + sinf(fAngle) * m_fCircleOffset,
																   pos.z + cosf(fAngle) * sinf(m_rot.y) * m_fCircleOffset),
			m_size, m_col, D3DXVECTOR3((sinf(m_rot.y) * 7.0f) + (-cosf(fAngle) * cosf(m_rot.y) * m_fSpeed),
																  sinf(fAngle) * m_fSpeed,
										(cosf(m_rot.y) * 7.0f) + (cosf(fAngle) * -cosf(m_rot.y + D3DX_PI / 2) * m_fSpeed)),
			m_nLife, CEffectB::EFFECT_TYPE_VANISH, NULL);

		// �p�x��ύX
		m_fAngle += (D3DX_PI * 2.0f / 15);

		// ���₵���p�x���P���������ɂȂ�����I��
		if (m_fAngle >= D3DX_PI * 2)
		{
			break;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CParticleDash::Uninit(void)
{
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CParticleDash::Update(void)
{
	for (int nCntParticle = 0; nCntParticle < m_nParticleNum; nCntParticle++)
	{
		if (pEffectB_Wind[nCntParticle] != NULL)
		{
			// �p�[�e�B�N���̎����ƈʒu���擾
			int nLife = pEffectB_Wind[nCntParticle]->GetLife();
			D3DXVECTOR3 pos = pEffectB_Wind[nCntParticle]->GetPos();

			// �������Ԓ��ɃG�t�F�N�g���o��
			CEffectB::Create(pos, m_size, m_col, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 10, CEffectB::EFFECT_TYPE_SMALL, NULL);

			// �p�[�e�B�N���̎������s������
			if (nLife <= 0)
			{
				pEffectB_Wind[nCntParticle] = NULL;
			}
		}

		// �Ō�̃p�[�e�B�N����NULL�ɂȂ�����
		if (pEffectB_Wind[m_nParticleNum - 1] == NULL)
		{
			// �I������
			Uninit();
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CParticleDash::Draw(void)
{

}

//=============================================================================
// ��������
//=============================================================================
CParticleDash *CParticleDash::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, D3DXCOLOR col,
	int nLife, float fSpeed, float fRotOff, float fCicrcleOff, int nNum)
{
	// �N���X�̐���
	CParticleDash *pParticleDash = NULL;
	pParticleDash = new CParticleDash;

	if (pParticleDash != NULL)
	{
		// �ϐ��̏�����
		pParticleDash->m_rot = rot;
		pParticleDash->m_col = col;
		pParticleDash->m_nLife = nLife;
		pParticleDash->m_fSpeed = fSpeed;
		pParticleDash->m_fRotOffset = fRotOff;
		pParticleDash->m_fCircleOffset = fCicrcleOff;
		pParticleDash->m_nParticleNum = nNum;

		// ����������
		pParticleDash->Init(pos, size);
	}
	return pParticleDash;
}