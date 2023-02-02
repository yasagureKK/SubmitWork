//=============================================================================
// �G�l�~�[���� [enemy.h]
// Author : ���� ��
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene3D.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CModel;
class CControl;
class CShadow;
class CCollisionSphere;
class CMotion;
class CLifeBillboard;
class CLife;

//*****************************************************************************
//�}�N����`
//*****************************************************************************
#define MAX_ENEMY_MODEL_WOLF00		(12)	// �T00�̃��f���̐�
#define MAX_ENEMY_MODEL_WOLF01		(16)	// �T01�̃��f���̐�
#define MAX_ENEMY_MODEL_BOSS		(15)	// �{�X�̃��f���̐�
#define MAX_ENEMY_MODEL				(16)	// ���f���̍ő吔
#define BEGIN_BOSS_LIFE				(80)	// �{�X�̏������C�t
#define MAX_ENEMY_COLLSION			(4)		// �R���W�����̐�

//*****************************************************************************
//�N���X�̒�`
//*****************************************************************************
class CEnemy : public CScene3D
{
public:
	typedef enum
	{
		ENEMY_TYPE_WOLF_0 = 0,
		ENEMY_TYPE_WOLF_1,
		ENEMY_TYPE_BOSS,
		ENEMY_TYPE_MAX
	} ENEMY_TYPE;

	CEnemy(int nPriority = PRIORITY_MODEL);							// �R���X�g���N�^
	~CEnemy();														// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, float fLife);	// ����������
	void Uninit(void);												// �I������
	void Update(void);												// �X�V����
	void Draw(void);												// �`�揈��
	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3	rot,
		ENEMY_TYPE type, float fLife);								// ��������
	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_ENEMY; }		// �I�u�W�F�N�g�̎�ނ̎擾����

	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }				// 1�t���[���O�̈ʒu�擾����
	void SetSize(D3DXVECTOR3 size) { m_size = size; }				// �T�C�Y�ݒ菈��
	D3DXVECTOR3 GetSize(void) { return m_size; }					// �T�C�Y�擾����
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }					// �����ݒ菈��
	D3DXVECTOR3 GetRot(void) { return m_rot; }						// �����擾����
	void SetMove(D3DXVECTOR3 move) { m_move = move; }				// �ړ��ʐݒ菈��
	D3DXVECTOR3 GetMove(void) { return m_move; }					// �ړ��ʎ擾����
	void SetLand(bool bLand) { m_bLand = bLand; }					// ���n�ݒ菈��
	bool GetLand(void) { return m_bLand; }							// ���n�擾����
	void SetModelPos(int nCntModel, D3DXVECTOR3 pos);				// ���f�����̈ʒu�ݒ菈��
	D3DXVECTOR3 GetModelPos(int nCntModel);							// ���f�����̈ʒu�擾����
	void SetModelRot(int nCntModel, D3DXVECTOR3 rot);				// ���f�����̌����ݒ菈��
	D3DXVECTOR3 GetModelRot(int nCntModel);							// ���f�����̌����擾����
	CMotion *GetMotion(void){ return m_pMotion; }					// ���[�V�����擾����
	float GetLife(void) { return m_fLife; }							// ���C�t�擾����
	void SetEnemyType(ENEMY_TYPE type) { m_type = type; }			// �G�̐ݒ�擾����
	ENEMY_TYPE GetEnemyType(void) { return m_type; }				// �G�̎�ގ擾����
	CModel *GetModel(int nCntModel);								// �G�l�~�[�̃��f���擾����
	void SetNewPos(D3DXVECTOR3 pos) { m_pos = pos; }				// �V�����ʒu�ݒ菈��

private:
	void Move(void);						// �ړ�����
	void TrackingShadow(void);				// �e�̒Ǐ]����
	void TouchCollision(void);				// ���ꂼ��̃R���W�����̐ڐG����
	void CollisionEnemy(CEnemy *pEnemy);	// �G���m��(�����o���̂��߂�)�Փˏ���
	void CreateModel(ENEMY_TYPE type);		// ���f���������̏���
	void SendExp(void);						// �o���l�󂯓n������
	void KnockDown(void);					// �|���ꂽ���̏���
	void CanControl(CEnemy *pEnemy);		// ���삳����ǂ����̐ݒ菈��

	D3DXVECTOR3 m_pos;										// �ʒu
	D3DXVECTOR3 m_posOld;									// 1�t���[���O�̈ʒu
	D3DXVECTOR3 m_move;										// �|���S���̈ړ���
	D3DXVECTOR3	m_size;										// �T�C�Y
	D3DXVECTOR3 m_rot;										// ����
	D3DXMATRIX m_mtxWorld;									// ���[���h�}�g���b�N�X
	CModel *m_apModel[MAX_ENEMY_MODEL];						// ���f���̃|�C���^
	CModel *m_pParent;										// �e���f���ւ̃|�C���^
	CMotion *m_pMotion;										// ���[�V�����̃|�C���^
	CControl *m_pControl;									// �R���g���[���̃|�C���^
	CShadow *m_pShadow;										// �V���h�E�̃|�C���^
	CLifeBillboard *m_pLifeBill;							// ���C�t(�r���{�[�h)�̃|�C���^
	CLife *m_pLife;											// ���C�t�̃|�C���^
	CCollisionSphere *m_pCollisionS[MAX_ENEMY_COLLSION];	// ���̃R���W�����̃|�C���^
	int m_nModelNum;										// ���f��(�p�[�c)�̐�
	int m_nDeathCount;										// ���ł܂ł̃J�E���g
	int m_nScore;											// �|�������̃X�R�A
	float m_fLife;											// ���C�t
	bool m_bLand;											// ���n���Ă��邩�ǂ���
	bool m_bKnockDown;										// �|�ꂽ���ǂ���
	bool m_bTouchWall;										// �ǂɐG��Ă��邩�ǂ���
	bool m_bCanControl;										// ���삳���邩�ǂ���
	ENEMY_TYPE m_type;										// �G�̎��
};
#endif // _PLAYER_H_