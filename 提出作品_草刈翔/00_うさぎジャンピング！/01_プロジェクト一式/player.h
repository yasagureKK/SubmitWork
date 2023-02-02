//=============================================================================
// �v���C���[���� [player.h]
// Authore : ���� ��
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CModel;
class CMotionPlayer;
class CControl;
class CShadow;
class CParticle;

//*****************************************************************************
//�}�N����`
//*****************************************************************************
#define MAX_PLAYER_MODEL	(12)	// ���f���̐�

//*****************************************************************************
//�N���X�̒�`
//*****************************************************************************
class CPlayer : public CScene
{
public:
	// �v���C���[�̑���̎��
	typedef enum
	{
		PLAYER_COMMAND_NONE = 0,	// �Ȃ�
		PLAYER_COMMAND_DASH,		// �_�b�V��
		PLAYER_COMMAND_JUMP,		// �W�����v
		PLAYER_COMMAND_DOUBLE_JUMP,	// 2�i�W�����v
		PLAYER_COMMAND_HOVERING,	// �z�o�����O
		PLAYER_COMMAND_MAX
	} PLAYER_COMMAND;

	CPlayer(int nPriority = PRIORITY_MODEL);					// �R���X�g���N�^
	~CPlayer();													// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);			// ����������
	void Uninit(void);											// �I������
	void Update(void);											// �X�V����
	void Draw(void);											// �`�揈��
	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3	rot,
		int nNumPlayer, bool bCpu);								// ��������
	D3DXVECTOR3 GetPosOld(void);								// 1�t���[���O�̈ʒu�擾����
	D3DXVECTOR3 GetRot(void);									// �����擾����
	void SetRot(D3DXVECTOR3 rot);								// �����ݒ菈��
	void SetMove(D3DXVECTOR3 move);								// �ړ��ʐݒ菈��
	D3DXVECTOR3 GetMove(void);									// �ړ��ʎ擾����
	void SetJump(bool bJump);									// �W�����v�ݒ菈��
	bool GetJump(void);											// �W�����v�擾����
	void SetLand(bool bLand);									// ���n�ݒ菈��
	bool GetLand(void);											// ���n�擾����
	void SetLandFinish(bool bLandFinish);						// ���n�Ńt�B�j�b�V���������ǂ����ݒ菈��
	bool GetLandFinish(void);									// ���n�Ńt�B�j�b�V���������ǂ����擾����
	void SetStop(bool bStop);									// �~�܂��Ă��邩�ǂ����ݒ菈��
	bool GetStop(void);											// �~�܂��Ă��邩�ǂ����擾����
	void SetModelPos(int nCntModel, D3DXVECTOR3 pos);			// ���f�����̈ʒu�ݒ菈��
	D3DXVECTOR3 GetModelPos(int nCntModel);						// ���f�����̈ʒu�擾����
	void SetModelRot(int nCntModel, D3DXVECTOR3 rot);			// ���f�����̌����ݒ菈��
	D3DXVECTOR3 GetModelRot(int nCntModel);						// ���f�����̌����擾����
	CMotionPlayer *GetMotionPlayer(void);						// �v���C���[�̃��[�V�����擾����
	void Collision(CScene *pSCene);								// �I�u�W�F�N�g�Ƃ̓����蔻��
	void ShadowCollision(void);									// �e�̕\���̂��߂̃I�u�W�F�N�g�Ƃ̓����蔻��
	void SetNum(int nNum);										// �v���C���[�̔ԍ��ݒ菈��
	int GetNum(void);											// �v���C���[�̔ԍ��擾����
	void SetCommand(PLAYER_COMMAND command);					// �v���C���[�̑���ݒ菈��
	PLAYER_COMMAND GetCommand(void);							// �v���C���[�̑���擾����
	bool GetCpu(void);											// CPU���ǂ����擾����

private:
	void ModelCreate(int nNum);									// ���f����������
	void Move(void);											// �ړ�����
	bool Collision(void);										// �����蔻��
	void CpuMove();												// CPU���쏈��

	D3DXVECTOR3 m_pos;											// �ʒu
	D3DXVECTOR3 m_posOld;										// 1�t���[���O�̈ʒu
	D3DXVECTOR3 m_move;											// �|���S���̈ړ���
	D3DXVECTOR3	m_size;											// �T�C�Y
	D3DXVECTOR3 m_rot;											// ����
	CModel *m_apModel[MAX_PLAYER_MODEL];						// ���f���̃|�C���^
	D3DXMATRIX m_mtxWorld;										// ���[���h�}�g���b�N�X
	CModel *m_pParent;											// �e���f���ւ̃|�C���^
	CControl *m_pControl;										// �R���g���[���̃|�C���^
	CShadow *m_pShadow;											// �V���h�E�̃|�C���^
	CMotionPlayer *m_pMotionPlayer;								// �v���C���[���[�V�����̃|�C���^
	CParticle *m_pParticle;										// �p�[�e�B�N���̃|�C���^
	bool m_bLand;												// ���n���Ă��邩�ǂ���
	bool m_bJump;												// �W�����v���Ă��邩�ǂ���
	bool m_bJumpOld;											// �O�t���[���W�����v���Ă��邩�ǂ���
	bool m_bLandFinish;											// ���n�Ńt�B�j�b�V���������ǂ���
	bool m_bStop;												// �~�܂��Ă��邩�ǂ���
	bool m_bCpu;												// CPU���ǂ���
	int m_nNum;													// �v���C���[�̔ԍ�
	int m_nCpuCount;											// CPU����̃J�E���g
	PLAYER_COMMAND m_command;									// ����̎��
};

#endif // _PLAYER_H_