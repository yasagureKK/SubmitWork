//=============================================================================
// 2D�̃G�t�F�N�g���� [effect_2D.h]
// Author : ���� ��
//=============================================================================
#ifndef _EFFECT_2D_H_
#define _EFFECT_2D_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene2D.h"

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CEffect2D : public CScene2D
{
public:
	CEffect2D(int nPriority = PRIORITY_UI);				// �R���X�g���N�^
	~CEffect2D();										// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size,
		D3DXCOLOR col);									// ����������
	void Uninit(void);									// �I������
	void Update(void);									// �X�V����
	void Draw(void);									// �`�揈��
	static CEffect2D *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 size, D3DXCOLOR col,
		D3DXVECTOR3 move, int nLife);					// ��������

private:
	D3DXVECTOR3 m_pos;									// �ʒu
	D3DXVECTOR3 m_size;									// �T�C�Y
	D3DXVECTOR3 m_move;									// �ړ���
	int m_nLife;										// ����
	D3DXCOLOR m_col;									// �J���[
};

#endif // _EFFECT_2D_H_