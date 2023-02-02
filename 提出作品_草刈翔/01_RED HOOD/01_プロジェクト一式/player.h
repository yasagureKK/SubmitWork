//=============================================================================
// �v���C���[���� [player.h]
// Author : ���� ��
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene3D.h"

//*****************************************************************************
//�}�N����`
//*****************************************************************************
#define MAX_PLAYER_MODEL	(16)		// ���f���̍ő吔

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CModel;
class CMotion;
class CControl;
class CShadow;
class CParticle;
class CCollisionSphere;
class CLife;
class CLevel;
class CWeapon;

//*****************************************************************************
//�N���X�̒�`
//*****************************************************************************
class CPlayer : public CScene3D
{
public:
	// �v���C���[�̏��
	typedef enum
	{
		PLAYER_STATE_NORMAL = 0,	// �ʏ�
		PLAYER_STATE_BLOWAWAY,		// �������
		PLAYER_STATE_DEBUG,			// ���G(�f�o�b�O�p)
		PLAYER_STATE_MAX
	} PLAYER_STATE;

	// ����̎��
	typedef enum
	{
		PLAYER_WEAPON_AXE = 0,	// ��
		PLAYER_WEAPON_BOWGUN,	// �{�E�K��
		PLAYER_WEAPON_MAX
	} PLAYER_WEAPON;

	CPlayer(int nPriority = PRIORITY_MODEL);							// �R���X�g���N�^
	~CPlayer();															// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);						// ����������
	void Uninit(void);													// �I������
	void Update(void);													// �X�V����
	void Draw(void);													// �`�揈��
	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3	rot);			// ��������
	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_PLAYER; }			// �I�u�W�F�N�g�̎�ނ̎擾����

	void SetPosOld(D3DXVECTOR3 pos) { m_posOld = pos; }					// 1�t���[���O�̈ʒu�ݒ菈��
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }					// 1�t���[���O�̈ʒu�擾����
	
	void SetSize(D3DXVECTOR3 size) { m_size = size; }					// �T�C�Y�ݒ菈��
	D3DXVECTOR3 GetSize(void) { return m_size; }						// �T�C�Y�擾����
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }						// �����ݒ菈��
	D3DXVECTOR3 GetRot(void) { return m_rot; }							// �����擾����
	void SetMove(D3DXVECTOR3 move) { m_move = move; }					// �ړ��ʐݒ菈��
	D3DXVECTOR3 GetMove(void) { return m_move; }						// �ړ��ʎ擾����
	void SetJump(bool bJump) { m_bJump = bJump; }						// �W�����v�ݒ菈��
	bool GetJump(void) { return m_bJump; }								// �W�����v�擾����
	void SetLand(bool bLand) { m_bLand = bLand; }						// ���n�ݒ菈��
	bool GetLand(void) { return m_bLand; }								// ���n�擾����
	void SetModelPos(int nCntModel, D3DXVECTOR3 pos);					// ���f�����̈ʒu�ݒ菈��
	D3DXVECTOR3 GetModelPos(int nCntModel);								// ���f�����̈ʒu�擾����
	void SetModelRot(int nCntModel, D3DXVECTOR3 rot);					// ���f�����̌����ݒ菈��
	D3DXVECTOR3 GetModelRot(int nCntModel);								// ���f�����̌����擾����
	D3DXVECTOR3 GetModelMtxPos(int nCntModel);							// ���f�����̃��[���h�}�g���b�N�X���W�擾����
	CMotion *GetMotion(void) { return m_pMotion; }						// ���[�V�����擾����
	CModel *GetModel(int nCntModel) { return m_apModel[nCntModel]; }	// ���f���擾����
	CCollisionSphere *GetCollision(void) { return m_pCollisionS; }		// �R���W�����擾����
	void SetState(PLAYER_STATE state) { m_state = state; }				// ��Ԑݒ菈��
	PLAYER_STATE GetState(void) { return m_state; }						// ��Ԏ擾����
	void SetWeapon(PLAYER_WEAPON weapon) { m_weapon = weapon; }			// ����ݒ菈��
	PLAYER_WEAPON GetWeapon(void) { return m_weapon; }					// ����ݒ菈��
	void AddExp(int nValue) { m_nExp += nValue; }						// �o���l��������
	int GetLevel(void);													// ���x���擾����
	float GetLife(void) { return m_fLife; }								// ���C�t�擾����
	void SetWeaponChange(bool bChange) { m_bWeaponChange = bChange; }	// ����ύX�擾����
	void SetCanControl(bool bControl) { m_bCanControl = bControl; }		// ����ł��邩�擾����
	bool GetCanControl(void) { return m_bCanControl; }					// ����ł��邩�擾����
	bool GetDeath(void) { return m_bDeath; }							// ���S����擾����

private:
	void ModelCreate(void);						// ���f����������
	void Move(void);							// �ړ�����
	void TrackingShadow(void);					// �e�̒Ǐ]����
	void TouchCollision(void);					// ���ꂼ��̃R���W�����̐ڐG����
	void State(void);							// ��ԏ���
	void CollisionEnemy(void);					// �G�Ƃ�(�����o���̂��߂�)�Փˏ���
	void ChangeWeapon(void);					// ����ύX���̏���

	D3DXVECTOR3 m_pos;							// �ʒu
	D3DXVECTOR3 m_posOld;						// 1�t���[���O�̈ʒu
	D3DXVECTOR3 m_move;							// �|���S���̈ړ���
	D3DXVECTOR3	m_size;							// �T�C�Y
	D3DXVECTOR3 m_rot;							// ����
	CModel *m_apModel[MAX_PLAYER_MODEL];		// ���f���̃|�C���^
	D3DXMATRIX m_mtxWorld;						// ���[���h�}�g���b�N�X
	CModel *m_pParent;							// �e���f���ւ̃|�C���^
	CMotion *m_pMotion;							// ���[�V�����̃|�C���^
	CControl *m_pControl;						// �R���g���[���̃|�C���^
	CShadow *m_pShadow;							// �V���h�E�̃|�C���^
	CCollisionSphere *m_pCollisionS;			// ���̃R���W�����̃|�C���^
	CLife *m_pLife;								// ���C�t�̃|�C���^
	CLevel *m_pLevel;							// ���x���̃|�C���^
	CWeapon *m_pWepaon;							// ����UI�̃|�C���^
	float m_fLife;								// ���C�t
	float m_fMaxLife;							// ���C�t�̍ő�l
	int m_nExp;									// �o���l
	int m_nCntInvincible;						// ���G���Ԃ̃J�E���g
	bool m_bLand;								// ���n���Ă��邩�ǂ���
	bool m_bLandOld;							// �O�t���[���ɒ��n���Ă��邩�ǂ���
	bool m_bJump;								// �W�����v���Ă��邩�ǂ���
	bool m_bWeaponChange;						// �����ύX���Ă邩�ǂ���
	bool m_bInvincible;							// ���G�������ǂ���
	bool m_bCanControl;							// ����ł��邩�ǂ���
	bool m_bDeath;								// ���S�������ǂ���
	PLAYER_STATE m_state;						// �v���C���[�̏��
	PLAYER_WEAPON m_weapon;						// ����
};
#endif // _PLAYER_H_