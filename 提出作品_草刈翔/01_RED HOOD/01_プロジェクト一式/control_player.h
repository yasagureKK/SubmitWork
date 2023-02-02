//=============================================================================
// �v���C���[���쏈�� [control_player.h]
// Author : ���� ��
//=============================================================================
#ifndef _CONTROL_PLAYER_H_
#define _CONTROL_PLAYER_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "control.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScene;
class CPlayer;
class CCollisionSphere;

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CControlPlayer : public CControl
{
public:
	CControlPlayer();						// �R���X�g���N�^
	~CControlPlayer();						// �f�X�g���N�^
	HRESULT Init(void);						// ����������
	void Uninit(void);						// �I������
	void Update(CScene *pScene);			// �X�V����
	static CControlPlayer *Create(void);	// ��������

private:
	void Move(CPlayer *pPlayer);			// �ړ�����
	void Dash(CPlayer *pPlayer);			// �_�b�V������
	void Attack(CPlayer *pPlayer);			// �U������
	void DoingAttack(CPlayer *pPlayer);		// �U�����̏���
	void WeaponChange(CPlayer *pPlayer);	// ����؂�ւ�����
	void TakeDamage(CPlayer *pPlayer);		// ��_���[�W����
	void Clear(CPlayer *pPlayer);			// �N���A���̏���
	void MoveInteria(CPlayer *pPlayer);		// �ړ��̊����ɂ��Ă̏���
	void Rotate(CPlayer *pPlayer);			// ��]����
#ifdef _DEBUG
	void Jump(CPlayer *pPlayer);			// �W�����v����
#endif

	D3DXVECTOR3 m_rot;						// ����
	float m_fObjectiveRot;					// �ړI�̌���
	float m_fNumRot;						// ������ς����
	float m_fSpeed;							// �ړ��ʂɉ����鑬��
	float m_fBasicSpeed;					// ��{�̑���
	float m_fJump;							// �W�����v��
	float m_fAttackValue;					// �U����
	float m_fAtkBeginEffect;				// �U�����A�G�t�F�N�g���o���J�E���g
	float m_fAtkEffectCount;				// �U�����A�G�t�F�N�g�̎�������
	bool m_bJump;							// �W�����v���Ă��邩�ǂ���
	bool m_bDoubleJump;						// 2�i�W�����v���Ă��邩�ǂ���
	bool m_bDash;							// �_�b�V�����Ă��邩�ǂ���
	bool m_bRotate;							// ��]���Ă��邩�ǂ���
	bool m_bAttack;							// �U�����Ă��邩�ǂ���
	bool m_bInputAttack;					// �U���̓��͂����Ă��邩�ǂ���
	bool m_bDamage;							// �_���[�W���󂯂Ă��邩�ǂ���
	int m_nMoveCount;						// �ړ����̃J�E���g
	int m_nDashCount;						// �_�b�V�����̃J�E���g
	int m_nDashCoolTime;					// �_�b�V����̃N�[���^�C��
	int m_nAtkCount;						// �U�����̃J�E���g
	CCollisionSphere *m_pCollisionS;		// ���̃R���W�����̃|�C���^
};
#endif	//_CONTROL_PLAYER_H_