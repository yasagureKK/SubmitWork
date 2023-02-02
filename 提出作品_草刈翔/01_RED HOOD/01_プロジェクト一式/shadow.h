//=============================================================================
// �V���h�E���� [shadow.h]
// Author : ���� ��
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene3D.h"

//*****************************************************************************
//�N���X�̒�`
//*****************************************************************************
class CShadow : public CScene3D
{
public:
	CShadow(int nPriority = PRIORITY_MODEL);					// �R���X�g���N�^
	~CShadow();													// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);			// ����������
	void Uninit(void);											// �I������
	void Update(void);											// �X�V����
	void Draw(void);											// �`�揈��
	static CShadow *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// ��������
	void SetShadow(D3DXVECTOR3 pos, float fRange);				// �e�̐ݒ菈��
	void SetUseShadow(bool bUse);								// �g�p��Ԃ̐ݒ菈��

private:
	LPDIRECT3DTEXTURE9 m_pTexture;								// �e�N�X�`���̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;							// ���_�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX m_mtxWorld;										// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;											// �ʒu
	D3DXVECTOR3 m_size;											// �T�C�Y
	D3DXVECTOR3 m_rot;											// ����
	D3DXVECTOR3 m_Defaultsize;									// �f�t�H���g�̉e�̃T�C�Y
	bool m_bUse;												// �g�p���Ă��邩�ǂ���
};
#endif // _SHADOW_H_