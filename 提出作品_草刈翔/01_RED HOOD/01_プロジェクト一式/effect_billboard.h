//=============================================================================
// �r���{�[�h�̃G�t�F�N�g���� [effect_billboard.h]
// Author : ���� ��
//=============================================================================
#ifndef _EFFECT_BILLBOARD_H_
#define _EFFECT_BILLBOARD_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "billboard.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CModel;

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CEffectB : public CBillboard
{
public:
	// �G�t�F�N�g�̎��
	typedef enum
	{
		EFFECT_TYPE_NORMAL,		// �ʏ�
		EFFECT_TYPE_VANISH,		// ���X�ɏ�����
		EFFECT_TYPE_SMALL,		// ���X�ɏ������Ȃ�
		EFFECT_TYPE_MAX
	} EFFECT_TYPE;

	CEffectB();											// �R���X�g���N�^
	~CEffectB();										// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size,
		D3DXCOLOR col, D3DXVECTOR3 move, int nLife);	// ����������
	void Uninit(void);									// �I������
	void Update(void);									// �X�V����
	void Draw(void);									// �`�揈��
	static CEffectB *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 size, D3DXCOLOR col,
		D3DXVECTOR3 move, int nLife, EFFECT_TYPE type,
		CModel *pModel);								// ��������
	int GetLife(void) { return m_nLife; }				// �����擾����

private:
	D3DXVECTOR3 m_pos;									// �ʒu
	D3DXVECTOR3	m_size;									// �T�C�Y
	D3DXVECTOR3	m_defSize;								// 1�t���[�����ƂɌ��炷�T�C�Y
	D3DXVECTOR3 m_mtxPos;								// �ۑ��p�̃��[���h�}�g���b�N�X���W
	D3DXCOLOR m_col;									// �J���[
	CModel *m_pParent;									// �e�̃��f��
	CBillboard *m_pBillboard;
	EFFECT_TYPE m_type;									// �G�t�F�N�g�̎��
	int m_nLife;										// ����
	int m_nBeginLife;									// 1�t���[���ڂ̎���
	float m_fDefAlpha;									// 1�t���[�����ƂɌ��炷�A���t�@�l
	bool m_bModel;										// ���f���ɂ��������ǂ���
};
#endif // _EFFECT_BILLBOARD_H_