//=============================================================================
// �p�[�e�B�N��(�_�b�V��)���� [particle_dash.h]
// Author : ���� ��
//=============================================================================
#ifndef _PARTICLE_DASH_H_
#define _PARTICLE_DASH_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "particle.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CEffectB;

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CParticleDash : public CParticle
{
public:
	CParticleDash();									// �R���X�g���N�^
	~CParticleDash();									// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// ����������
	void Uninit(void);									// �I������
	void Update(void);									// �X�V����
	void Draw(void);									// �`�揈��
	static CParticleDash *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 size, D3DXVECTOR3 rot,
		D3DXCOLOR col, int nLife, float fSpeed,
		float fRotOff, float fCicrcleOff, int nNum);	// ��������

private:
	CEffectB *pEffectB_Circle[MAX_PARTICLE];			// �G�t�F�N�g�̃|�C���^
	CEffectB *pEffectB_Wind[MAX_PARTICLE];				// �G�t�F�N�g�̃|�C���^
	D3DXVECTOR3 m_pos;									// �ʒu
	D3DXVECTOR3	m_size;									// �T�C�Y
	D3DXVECTOR3	m_rot;									// ����
	D3DXCOLOR m_col;									// �J���[
	int m_nLife;										// ����
	int m_nParticleNum;									// 1�x�ɏo���p�[�e�B�N���̐�
	float m_fSpeed;										// ����
	float m_fAngle;										// �p�x
	float m_fRotOffset;									// ���ʂ̌�������̃I�t�Z�b�g
	float m_fCircleOffset;								// �p�[�e�B�N�������ʒu����̃I�t�Z�b�g
};

#endif	//_PARTICLE_DASH_H_