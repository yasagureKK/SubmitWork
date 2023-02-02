//=============================================================================
// 2D�I�u�W�F�N�g���� [scene2D.h]
// Author : ���� ��
//=============================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)	// ���_�t�H�[�}�b�g

//*****************************************************************************
// �\���̂̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;  // ���_���W
	float rhw;        // 1.0f�ŌŒ�
	D3DCOLOR col;     // ���_�J���[
	D3DXVECTOR2 tex;  // �e�N�X�`���摜
} VERTEX_2D;

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CScene2D : public CScene
{
public:
	CScene2D(int nPriority = PRIORITY_NONE);									// �R���X�g���N�^
	~CScene2D();																// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos);												// ����������
	void Uninit(void);															// �I������
	void Update(void);															// �X�V����
	void Draw(void);															// �`�揈��
	static CScene2D *Create(D3DXVECTOR3 pos,
		D3DXVECTOR2 size, int nPriority);										// ��������
	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_NONE; }					// �I�u�W�F�N�g�̎�ނ̎擾����
	void SetPos(D3DXVECTOR3 pos);												// �ʒu�ݒ菈��
	D3DXVECTOR3 GetPos(void) { return m_pos; }									// �ʒu�擾����
	void SetSize(D3DXVECTOR2 size);												// �T�C�Y�ݒ菈��
	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(m_size.x, m_size.y, 0.0f); }	// �T�C�Y�擾����
	D3DXVECTOR3 GetPosOld(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// 1�t���[���O�̈ʒu�擾����
	D3DXVECTOR3 GetRot(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }			// �����擾����

	void SetTexture(float fU, float fV, int nCutU, int nCutV);					// �e�N�X�`�����W�ݒ菈��
	void BindTexture(int nTexType);												// �e�N�X�`����������
	void SetColor(D3DXCOLOR col);												// �J���[�ݒ菈��
	void SetBesideBarScale(float fScale, bool bReverse);						// ���C�t�Q�[�W(������)�̃X�P�[���ݒ菈��
	void SetTilt(float fAngle);													// �X���ݒ菈��

private:
	LPDIRECT3DTEXTURE9 m_pTexture;												// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;											// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 m_pos;															// �ʒu
	D3DXVECTOR2 m_size;															// �T�C�Y
	D3DXCOLOR m_col;															// �J���[
	float m_fRadius;															// ���_�܂ł̔��a
	float m_fTilt;																// �X��
};
#endif // _SCENE2D_H_