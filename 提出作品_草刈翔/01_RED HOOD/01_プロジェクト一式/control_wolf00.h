//=============================================================================
// �T00���쏈�� [control_wolf00.h]
// Author : ���� ��
//=============================================================================
#ifndef _CONTROL_WOLF00_H_
#define _CONTROL_WOLF00_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "control.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScene;
class CEnemy;
class CCollisionSphere;

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CControlWolf00 : public CControl
{
public:
	CControlWolf00();						// �R���X�g���N�^
	~CControlWolf00();						// �f�X�g���N�^
	HRESULT Init(void);						// ����������
	void Uninit(void);						// �I������
	void Update(CScene *pScene);			// �X�V����
	static CControlWolf00 *Create(void);	// ��������

private:
	void Move(CEnemy *pEnemy);				// ��{�̈ړ�����
	void Attack(CEnemy *pEnemy);			// �U������
	void DoingAttack(CEnemy *pEnemy);		// �U�����̏���
	void MoveInteria(CEnemy *pEnemy);		// �ړ��̊����ɂ��Ă̏���
	void Rotate(CEnemy *pEnemy);			// ��]����

	D3DXVECTOR3 m_rot;						// ����
	D3DXVECTOR3 m_moveOld;					// 1�t���[���O�̈ړ���
	float m_fObjectiveRot;					// �ړI�̌���
	float m_fNumRot;						// ������ς����
	float m_fSpeed;							// �ړ��ʂɉ����鑬��
	bool m_bMove;							// �ړ����Ă��邩�ǂ���
	bool m_bRotate;							// ��]���Ă��邩�ǂ���
	bool m_bAttack;							// �U�����Ă��邩�ǂ���
	int m_nMoveCount;						// �ړ����̃J�E���g
	int m_nAtkInterval;						// �U���̊Ԋu
	int m_nAtkCount;						// �U�����̃J�E���g
	int m_nAttackWaitTime;					// �U���܂ł̑ҋ@����
	float m_fAtkBeginEffect;				// �U�����A�G�t�F�N�g���o���J�E���g
	float m_fAtkEffectCount;				// �U�����A�G�t�F�N�g�̎�������
	CCollisionSphere *m_pCollisionS;		// ���̃R���W�����̃|�C���^
};
#endif	//_CONTROL_WOLF00_H_