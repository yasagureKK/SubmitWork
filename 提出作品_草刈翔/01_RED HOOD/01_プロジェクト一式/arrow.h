//=============================================================================
// �A���[���� [arrow.h]
// Author : ���� ��
//=============================================================================
#ifndef _ARROW_H_
#define _ARROW_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene3D.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CModelSingle;
class CModel;
class CCollisionSphere;

//*****************************************************************************
//�}�N����`
//*****************************************************************************
#define SUSTAIN_TIME		(120)	// ��������

//*****************************************************************************
//�N���X�̒�`
//*****************************************************************************
class CArrow : public CScene3D
{
public:
	CArrow(int nPriority = PRIORITY_MODEL);						// �R���X�g���N�^
	~CArrow();													// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);			// ����������
	void Uninit(void);											// �I������
	void Update(void);											// �X�V����
	void Draw(void);											// �`�揈��
	static CArrow *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
		float fAttackValue);									// ��������
	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_NONE; }	// �I�u�W�F�N�g�̎�ނ̎擾����
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }			// 1�t���[���O�̈ʒu�擾����

	void SetModelRot(D3DXVECTOR3 rot) { m_rot = rot; }			// �����ݒ菈��
	D3DXVECTOR3 GetRot(void) { return m_rot; }					// �����擾����

private:
	void TouchCollision(void);

	D3DXVECTOR3 m_pos;											// �ʒu
	D3DXVECTOR3 m_posOld;										// 1�t���[���O�̈ʒu
	D3DXVECTOR3	m_size;											// �T�C�Y
	D3DXVECTOR3 m_rot;											// ����
	D3DXMATRIX m_mtxWorld;										// ���[���h�}�g���b�N�X
	CModel *m_pModel;											// ���f���ւ̃|�C���^
	CCollisionSphere *m_pCollisionS;							// ���̃R���W�����̃|�C���^
	int m_nLife;												// ����
	float m_fAttackValue;										// �U����
};
#endif // _ARROW_H_