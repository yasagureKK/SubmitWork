//=============================================================================
// ���̃R���W�������� [collision_sphere.h]
// Author : ���� ��
//=============================================================================
#ifndef _SPHERE_COLLISION_H_
#define _SPHERE_COLLISION_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene3D.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CModel;

//*****************************************************************************
//�N���X�̒�`
//*****************************************************************************
class CCollisionSphere : public CScene3D
{
public:
	// �ǂ̓����蔻�肩
	typedef enum
	{
		COLLISION_S_TYPE_NONE = 0,		// �Ȃ�
		COLLISION_S_TYPE_PLAYER,		// �v���C���[
		COLLISION_S_TYPE_WEAPON,		// ����
		COLLISION_S_TYPE_WEAPON_P,		// ����[��]
		COLLISION_S_TYPE_ENEMY,			// �G
		COLLISION_S_TYPE_ENEMY_ATTACK,	// �G�̍U��
		COLLISION_S_TYPE_MAX
	} COLLISION_S_TYPE;

	CCollisionSphere(int nPriority = PRIORITY_EFFECT);						// �R���X�g���N�^
	~CCollisionSphere();													// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);						// ����������
	void Uninit(void);														// �I������
	void Update(void);														// �X�V����
	void Draw(void);														// �`�揈��
	static CCollisionSphere *Create(D3DXVECTOR3 pos,
		float fSize, COLLISION_S_TYPE type, float fTime);					// ��������

	D3DXVECTOR3 GetSize(void) { return m_size; }							// �T�C�Y�擾����
	void Collision(CScene *pScene);											// �Փˏ���
	void SetPosCollision(D3DXVECTOR3 pos) { m_pos = pos; }					// �ʒu�ݒ菈��
	void SetCollisionType(COLLISION_S_TYPE type);							// �R���W�����̎�ސݒ菈��
	COLLISION_S_TYPE GetCollisionType(void) { return m_collisionType; }		// �R���W�����̎�ނ̎擾����
	void SetParent(CModel *pModel) { m_pParent = pModel; }					// �e���f���ݒ菈��
	D3DXVECTOR3 GetMtxPos(void);											// ���[���h�}�g���b�N�X���W�擾����
	bool OnCollisionEnter(COLLISION_S_TYPE type);							// �Փ˂����u�Ԃ̏���
	void SetTime(float fTime) { m_fTime = fTime; }							// �������Ԑݒ菈��
	void SetDamageValue(float fValue) { m_fDamage = fValue; }				// �_���[�W�l�ݒ菈��
	float GetDamageValue(void) { return m_fDamage; }						// �_���[�W�l�擾����
	float GetObjectiveRot(void) { return m_fObjectiveRot; }					// �ړI�̌����擾����
	void SetPosSource(D3DXVECTOR3 pos) { m_posSource = pos; }				// �R���W�����������̈ʒu�ݒ菈��
	D3DXVECTOR3 GetPosSource(void) { return m_posSource; }					// �R���W�����������̈ʒu�擾����
#ifdef _DEBUG
	static void SetVisual(void);											// ����̉����ݒ菈��
#endif

private:
#ifdef _DEBUG
	static void VisualChange(void);											// ����̉�������
#endif

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;										// ���_�o�b�t�@�̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;										// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 m_pos;														// �ʒu
	D3DXVECTOR3	m_size;														// �T�C�Y
	D3DXVECTOR3 m_rot;														// ����
	D3DXVECTOR3 m_posSource;												// �R���W�����������̈ʒu
	D3DXMATRIX m_mtxWorld;													// ���[���h�}�g���b�N�X
	CModel *m_pParent;														// �e�̃��f��
	D3DXCOLOR m_col;														// �J���[
	COLLISION_S_TYPE m_collisionType;										// �����蔻��̎��
	COLLISION_S_TYPE m_saveCollisionType;									// �ۑ����邽�߂̓����蔻��̎��
	int m_nVertical;														// �c�̕�����
	int m_nSide;															// ���̕�����
	float m_fTime;															// ��������
	float m_fDamage;														// �_���[�W�̒l
	float m_fObjectiveRot;													// �ړI�̌���
	static bool m_bMngVisual;												// ������������邩�ǂ���
	bool m_bContact;														// ��x�ł������������ǂ���
	bool m_bContactOld;														// 1�t���[���O�ɓ������Ă������ǂ���
};
#endif // _SPHERE_COLLISION_H_