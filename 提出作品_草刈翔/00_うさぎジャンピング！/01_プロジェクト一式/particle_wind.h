//=============================================================================
// �p�[�e�B�N��(�E�B���h)���� [particle_wind.h]
// Author : ���� ��
//=============================================================================
#ifndef _PARTICLE_WIND_H_
#define _PARTICLE_WIND_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "particle.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CEffect2D;

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CParticleWind : public CParticle
{
public:
	CParticleWind();									// �R���X�g���N�^
	~CParticleWind();									// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// ����������
	void Uninit(void);									// �I������
	void Update(void);									// �X�V����
	void Draw(void);									// �`�揈��
	static CParticleWind *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 size, D3DXCOLOR col,
		D3DXVECTOR3 move, int nNum);					// ��������

private:
	CEffect2D *pEffect2D;								// 2D�G�t�F�N�g�̃|�C���^
	D3DXVECTOR3 m_pos;									// �ʒu
	D3DXVECTOR3	m_size;									// �T�C�Y
	D3DXVECTOR3	m_move;									// �ړ���
	D3DXCOLOR m_col;									// �J���[
	int m_nNum;											// �ǂ̃v���C���[�̃p�[�e�B�N���Ȃ̂�
};

#endif	//_PARTICLE_WIND_H_