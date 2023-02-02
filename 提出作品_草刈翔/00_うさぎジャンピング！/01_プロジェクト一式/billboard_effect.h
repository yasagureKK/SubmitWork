//=============================================================================
// �r���{�[�h�̃G�t�F�N�g���� [billboard_effect.h]
// Author : ���� ��
//=============================================================================
#ifndef _BILLBOARD_EFFECT_H_
#define _BILLBOARD_EFFECT_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "billboard.h"

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CBillboardEffect : public CBillboard
{
public:
	// �G�t�F�N�g�̎��
	typedef enum
	{
		EFFECT_TYPE_NORMAL,		// �ʏ�
		EFFECT_TYPE_VANISH,		// ���X�ɏ�����
		EFFECT_TYPE_MAX
	} EFFECT_TYPE;

	CBillboardEffect();									// �R���X�g���N�^
	~CBillboardEffect();								// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size,
		D3DXCOLOR col);									// ����������
	void Uninit(void);									// �I������
	void Update(void);									// �X�V����
	void Draw(void);									// �`�揈��
	static CBillboardEffect *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 size, D3DXCOLOR col,
		D3DXVECTOR3 move, int nLife, EFFECT_TYPE type);	// ��������

private:
	D3DXVECTOR3 m_pos;									// �ʒu
	D3DXVECTOR3	m_size;									// �T�C�Y
	int m_nLife;										// ����
	D3DXCOLOR m_col;									// �J���[
	EFFECT_TYPE m_type;									// �G�t�F�N�g�̎��
};

#endif // _BILLBOARD_EFFECT_H_