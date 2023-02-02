//=============================================================================
// 3D�I�u�W�F�N�g�ł̃i���o�[�Ǘ����� [number3D.h]
// Author : ���� ��
//=============================================================================
#ifndef _NUMBER3D_H_
#define _NUMBER3D_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CNumber3D
{
public:
	CNumber3D();								// �R���X�g���N�^
	~CNumber3D();								// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos,
		D3DXVECTOR3 size, D3DXCOLOR col);		// ����������
	void Uninit(void);							// �I������
	void Update(void);							// �X�V����
	void Draw(void);							// �`�揈��
	static CNumber3D *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 size, D3DXVECTOR3 rot,
		D3DXCOLOR col);							// ��������
	void SetNumber(int nNumber);				// �i���o�[�ݒ菈��
	void SetPos(D3DXVECTOR3 pos);				// �ʒu�ݒ菈��
	void SetColor(D3DXCOLOR col);				// �J���[�ݒ菈��

private:
	D3DXVECTOR3 m_pos;										// �ʒu
	D3DXVECTOR3	m_size;										// �T�C�Y
	D3DXVECTOR3 m_rot;										// ����
	D3DXCOLOR m_color;										// �J���[
	static LPDIRECT3DTEXTURE9 m_pTexture;					// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;						// ���_�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX m_mtxWorld;									// ���[���h�}�g���b�N�X
};

#endif	//_NUMBER3D_H_