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
class CParticle;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PLAYER_JUMP							(8.0f)	// �W�����v��
#define PLAYER_DOUBLE_JUMP					(10.0f)	// 2�i�W�����v�ł̃W�����v��
#define PLAYER_JUMP_MAGNIFICATION			(14.0f)	// �W�����v�ʂ̔{���̑傫��
#define PLAYER_GRAVITY						(0.4f)	// �d�͂̑傫��
#define PLAYER_MAX_GRAVITY					(10.0f)	// �d�͂̍ő�l
#define PLAYER_MAX_DOUBLE_JUMP_GRAVITY		(7.0f)	// 2�i�W�����v���̏d�͂̍ő�l
#define PLAYER_MOVE_SPEED					(0.5f)	// �ړ��ʂ̊�l
#define PLAYER_ADD_SPEED					(0.05f)	// �ړ��ʂɉ�����l
#define PLAYER_LAND_INTERIA_SUBTRACTION		(0.86f)	// �n��ł̊����̌��Z�l
#define PLAYER_AIR_INTERIA_SUBTRACTION		(0.92f)	// �󒆂ł̊����̌��Z�l
#define PLAYER_MOVE_STOP_COUNT				(0.02f)	// �v���C���[�̈ړ��ʂ�0�ɂ��鎞�̈ړ��ʂ̒l

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CControlPlayer : public CControl
{
	// ���ɓ��͂��鑀��̎��
	typedef enum
	{
		PLAYER_NEXT_INPUT_NONE = 0,	// �Ȃ�
		PLAYER_NEXT_INPUT_LEFT,		// ��
		PLAYER_NEXT_INPUT_RIGHT,	// �E
		PLAYER_NEXT_MAX
	} PLAYER_NEXT_INPUT;

public:
	CControlPlayer();						// �R���X�g���N�^
	~CControlPlayer();						// �f�X�g���N�^
	HRESULT Init(void);						// ����������
	void Uninit(void);						// �I������
	void Update(CScene *pScene);			// �X�V����
	static CControlPlayer *Create(void);	// ��������

private:
	void Move(CPlayer *pPlayer);			// �ړ�����
	void Jump(CPlayer *pPlayer);			// �W�����v����
	void MoveInteria(CPlayer *pPlayer);		// �ړ��̊����ɂ��Ă̏���
	void Rotate(CPlayer *pPlayer);			// ��]����
	void CommandMove(CPlayer *pPlayer);		// �s�����w�肳�ꂽ���̈ړ�����

	D3DXVECTOR3 m_rot;						// ����
	D3DXVECTOR3 m_moveOld;					// 1�t���[���O�̈ړ���
	float m_fObjectiveRot;					// �ړI�̌���
	float m_fNumRot;						// ������ς����
	float m_fSpeed;							// �ړ��ʂɉ����鑬��
	float m_fFloat;							// ����
	bool m_bJump;							// �W�����v���Ă邩�ǂ���
	bool m_bDoubleJump;						// 2�i�W�����v���Ă邩�ǂ���
	bool m_bRotate;							// ��]���Ă��邩�ǂ���
	int m_nSpinCount;						// SE�Đ��̃J�E���g
	int m_nMoveCount;						// �ړ����̃J�E���g
	PLAYER_NEXT_INPUT m_nextInput;			// ���ɓ��͂��鑀��
	CParticle *m_pParticle;					// �p�[�e�B�N���̃|�C���^

#ifdef _DEBUG
	void DebugMove(CPlayer *pPlayer);		// �f�o�b�O�p�̈ړ�����
#endif
};

#endif	//_CONTROL_PLAYER_H_