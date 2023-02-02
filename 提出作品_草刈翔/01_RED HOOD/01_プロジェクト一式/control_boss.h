//=============================================================================
// �{�X���쏈�� [control_boss.h]
// Author : ���� ��
//=============================================================================
#ifndef _CONTROL_BOSS_H_
#define _CONTROL_BOSS_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "control.h"

//*****************************************************************************
//�}�N����`
//*****************************************************************************
#define MAX_BOSS_COLLSION	(4)		// �R���W�����̐�

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScene;
class CEnemy;
class CCollisionSphere;

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CControlBoss : public CControl
{
public:
	typedef enum
	{
		BOSS_THINK_STOP = 0,
		BOSS_THINK_MOVE,
		BOSS_THINK_CLAW,
		BOSS_THINK_JUMP_NORMAL,
		BOSS_THINK_JUMP_BIG,
		BOSS_THINK_CHARGE,
		BOSS_THINK_CHARGE_BEGIN,
		BOSS_THINK_DASH,
		BOSS_THINK_DASH_TURN,
		BOSS_THINK_DASH_END,
	} BOSS_THINK;

	CControlBoss();							// �R���X�g���N�^
	~CControlBoss();						// �f�X�g���N�^
	HRESULT Init(void);						// ����������
	void Uninit(void);						// �I������
	void Update(CScene *pScene);			// �X�V����
	static CControlBoss *Create(void);		// ��������

private:
	void Move(CEnemy *pEnemy);							// ��{�̈ړ�����
	void ClawAttack(CEnemy *pEnemy);					// �U��(��)����
	void DashAttack(CEnemy *pEnemy);					// �U��(�_�b�V��)����
	void DoingAttack(CEnemy *pEnemy);					// �U�����̏���
	void Jump(CEnemy *pEnemy);							// ��{�̈ړ�����
	void MoveInteria(CEnemy *pEnemy);					// �ړ��̊����ɂ��Ă̏���
	void Rotate(CEnemy *pEnemy);						// ��]����

	D3DXVECTOR3 m_rot;									// ����
	D3DXVECTOR3 m_moveOld;								// 1�t���[���O�̈ړ���
	D3DXVECTOR3 m_posDash;								// �_�b�V���O�̒��n�ʒu
	D3DXVECTOR3 m_beginPos;								// �����ʒu
	float m_fObjectiveRot;								// �ړI�̌���
	float m_fNumRot;									// ������ς����
	float m_fSpeed;										// �ړ��ʂɉ����鑬��
	bool m_bMove;										// �ړ����Ă��邩�ǂ���
	bool m_bRotate;										// ��]���Ă��邩�ǂ���
	bool m_bClaw;										// �܂̍U�������Ă��邩�ǂ���
	int m_nAtkInterval;									// �U���̊Ԋu
	int m_nAtkCount;									// �U�����̃J�E���g
	int m_nAttackWaitTime;								// �U���܂ł̑ҋ@����
	int m_nClawNum;										// �܍U���̐�
	int m_nJumpInterval;								// �W�����v�̊Ԋu
	int m_nJump;										// �W�����v����܂ł̃J�E���g
	int m_nJumpNum;										// �W�����v�̉�
	int m_nMoveCount;									// �ړ����̃J�E���g
	int m_nMoveJump;									// �ړ����A�W�����v����܂ł̃J�E���g
	int m_nMoveStop;									// �������~�߂�J�E���g
	int m_nDashCount;									// �_�b�V�����̃J�E���g
	int m_nDashNum;										// �_�b�V���U���̐�
	float m_fAtkBeginEffect;							// �U�����A�G�t�F�N�g���o���J�E���g
	float m_fAtkEffectCount;							// �U�����A�G�t�F�N�g�̎�������
	CCollisionSphere *m_pCollisionS[MAX_BOSS_COLLSION];	// ���̃R���W�����̃|�C���^
	BOSS_THINK m_think;									// �v�l
};
#endif	// _CONTROL_BOSS_H_