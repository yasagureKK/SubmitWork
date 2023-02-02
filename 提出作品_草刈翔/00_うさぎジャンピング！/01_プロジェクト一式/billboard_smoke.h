//=============================================================================
// �r���{�[�h(�X���[�N)���� [billboard_smoke.h]
// Author : ���� ��
//=============================================================================
#ifndef _BILLBOARD_SMOKE_H_
#define _BILLBOARD_SMOKE_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "billboard.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CBillboardEffect;

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CBillboardSmoke : public CBillboard
{
public:
	CBillboardSmoke();									// �R���X�g���N�^
	~CBillboardSmoke();									// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// ����������
	void Uninit(void);									// �I������
	void Update(void);									// �X�V����
	void Draw(void);									// �`�揈��
	static CBillboardSmoke *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 size, int nLife);					// ��������

private:
	D3DXVECTOR3 m_pos;									// �ʒu
	D3DXVECTOR3	m_size;									// �T�C�Y
	int m_nLife;										// ����
	int m_nAlpha;										// �����x
};

#endif	//_PARTICLE_SMOKE_H_