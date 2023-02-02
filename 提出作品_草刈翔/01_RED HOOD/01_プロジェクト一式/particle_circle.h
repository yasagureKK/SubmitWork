//=============================================================================
// �p�[�e�B�N��(�~�`)���� [particle_circle.h]
// Author : ���� ��
//=============================================================================
#ifndef _PARTICLE_CIRCLE_H_
#define _PARTICLE_CIRCLE_H_

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
class CParticleCircle : public CParticle
{
public:
	CParticleCircle();										// �R���X�g���N�^
	~CParticleCircle();										// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);		// ����������
	void Uninit(void);										// �I������
	void Update(void);										// �X�V����
	void Draw(void);										// �`�揈��
	static CParticleCircle *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 size,  D3DXCOLOR col, int nLife,
		float fSpeed, int nNum);							// ��������

private:
	CEffectB *pEffectB[MAX_PARTICLE];	// �G�t�F�N�g�̃|�C���^
	D3DXVECTOR3 m_pos;					// �ʒu
	D3DXVECTOR3	m_size;					// �T�C�Y
	D3DXCOLOR m_col;					// �J���[
	int m_nLife;						// ����
	int m_nParticleNum;					// 1�x�ɏo���p�[�e�B�N���̐�
	float m_fSpeed;						// ����
	float m_fAngle;						// �p�x
};

#endif	//_PARTICLE_CIRCLE_H_