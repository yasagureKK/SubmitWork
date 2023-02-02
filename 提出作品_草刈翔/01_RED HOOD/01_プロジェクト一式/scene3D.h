//=============================================================================
// 3D�I�u�W�F�N�g���� [scene3D.h]
// Author : ���� ��
//=============================================================================
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)		//���_�t�H�[�}�b�g3D

//*****************************************************************************
// �\���̂̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		//���_���W
	D3DXVECTOR3 nor;		//�@���x�N�g��
	D3DCOLOR col;			//���_�J���[
	D3DXVECTOR2 tex;		//�e�N�X�`�����W
} VERTEX_3D;

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CScene3D : public CScene
{
public:
	CScene3D(int nPriority = PRIORITY_MODEL);								// �R���X�g���N�^
	~CScene3D();															// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos);											// ����������
	void Uninit(void);														// �I������
	void Update(void);														// �X�V����
	void Draw(void);														// �`�揈��
	static CScene3D *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 size, D3DXVECTOR3 rot, int nPriolty);					// ��������
	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_NONE; }				// �I�u�W�F�N�g�̎�ނ̎擾����
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }							// �ʒu�ݒ菈��
	D3DXVECTOR3 GetPos(void) { return m_pos; };								// �ʒu�擾����
	D3DXVECTOR3 GetPosOld(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	// 1�t���[���O�̈ʒu�擾����
	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// �T�C�Y�擾����
	D3DXVECTOR3 GetRot(void) { return m_rot; }								// �����擾����

private:
	D3DXVECTOR3 m_pos;														// �ʒu
	D3DXVECTOR3 m_rot;														// ����
	D3DXMATRIX m_mtxWorld;													// ���[���h�}�g���b�N�X
};
#endif // _SCENE3D_H_