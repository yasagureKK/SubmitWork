//=============================================================================
// �e�N�X�`���Ǘ� [texture.cpp]
// Author : ���� ��
//=============================================================================
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_TEXTURE_2D	(64)	// �e�N�X�`���̍ő吔
#define MAX_TEXTURE_3D	(32)	// �e�N�X�`���̍ő吔

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CTexture
{
public:
	CTexture();											// �R���X�g���N�^
	~CTexture();										// �f�X�g���N�^
	void Init(void);									// ����������
	void Uninit(void);									// �I������
	LPDIRECT3DTEXTURE9 GetTexture2D(int nTexture);		// �e�N�X�`���擾����
	LPDIRECT3DTEXTURE9 GetTexture3D(int nTexture);		// �e�N�X�`���擾����

private:
	int LoadTexture(LPDIRECT3DTEXTURE9 tex[]);			// �e�N�X�`�����[�h����
	LPDIRECT3DTEXTURE9 m_apTexture2D[MAX_TEXTURE_2D];	// �e�N�X�`���̃|�C���^
	LPDIRECT3DTEXTURE9 m_apTexture3D[MAX_TEXTURE_3D];	// �e�N�X�`���̃|�C���^
	int m_nMaxTex2D;									// �e�N�X�`���̍ő吔
	int m_nMaxTex3D;									// �e�N�X�`���̍ő吔
};

#endif