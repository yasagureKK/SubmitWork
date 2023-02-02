//=============================================================================
// �P�̂̃��f������ [model_single.h]
// Author  : ���� ��
//=============================================================================
#ifndef _MODEL_SINGLE_H_
#define _MODEL_SINGLE_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene3D.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CModel;
//class CShadow;

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CModelSingle : public CScene3D
{
public:
	CModelSingle(int nPriority = PRIORITY_MODEL);					// �R���X�g���N�^
	~CModelSingle();												// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nModelType);	// ����������
	void Uninit(void);												// �I������
	void Update(void);												// �X�V����
	void Draw(void);												// �`�揈��
	static CModelSingle *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 rot, int nModelType);							// ��������
	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_MODEL; }		// �I�u�W�F�N�g�̎�ނ̎擾����
	static bool Collision(CScene *pScene);							// �����蔻��
	void SetColor(D3DXCOLOR col);									// �J���[�ݒ菈��

private:
	D3DXVECTOR3 m_pos;			// �ʒu
	D3DXVECTOR3 m_posOld;		// 1�t���[���O�̈ʒu
	D3DXVECTOR3	m_size;			// �T�C�Y
	D3DXVECTOR3 m_rot;			// ����
	D3DXVECTOR3 m_move;			// �ړ���
	CModel *m_pModel;			// ���f���̃|�C���^
	D3DXMATRIX m_mtxWorld;		// ���[���h�}�g���b�N�X
};
#endif // _MODEL_SINGLE_H_