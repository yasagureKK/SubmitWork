//=============================================================================
// �p�[�e�B�N��(�q�b�g)���� [particle_hit.h]
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
class CParticleHit : public CParticle
{
public:
	CParticleHit();										// �R���X�g���N�^
	~CParticleHit();									// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// ����������
	void Uninit(void);									// �I������
	void Update(void);									// �X�V����
	void Draw(void);									// �`�揈��
	static CParticleHit *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 size, D3DXVECTOR3 rot,
		D3DXCOLOR col, int nLife, float fSpeed,
		int nNum);										// ��������

private:
	CEffectB *pEffectB[MAX_PARTICLE];				// �G�t�F�N�g�̃|�C���^
	D3DXVECTOR3 m_pos;								// �ʒu
	D3DXVECTOR3	m_size;								// �T�C�Y
	D3DXVECTOR3	m_rot;								// ����
	D3DXCOLOR m_col;								// �J���[
	int m_nLife;									// ����
	int m_nParticleNum;								// 1�x�ɏo���p�[�e�B�N���̐�
	float m_fSpeed;									// ����
};

#endif	//_PARTICLE_DASH_H_