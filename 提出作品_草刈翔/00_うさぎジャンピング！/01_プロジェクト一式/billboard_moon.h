//=============================================================================
// �r���{�[�h(���[��)���� [billboard_moon.h]
// Author : ���� ��
//=============================================================================
#ifndef _BILLBOARD_MOON_H_
#define _BILLBOARD_MOON_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "billboard.h"

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CBillboardMoon : public CBillboard
{
public:
	CBillboardMoon();									// �R���X�g���N�^
	~CBillboardMoon();									// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size,
		int nNum);										// ����������
	void Uninit(void);									// �I������
	void Update(void);									// �X�V����
	void Draw(void);									// �`�揈��
	static CBillboardMoon *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 size, int nNum);					// ��������

private:
	D3DXVECTOR3 m_pos;									// �ʒu
	D3DXVECTOR3	m_size;									// �T�C�Y
	int m_nNum;											// �ԍ�
};

#endif // _BILLBOARD_MOON_H_