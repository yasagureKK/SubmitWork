//=============================================================================
// �U�����Ǘ� [data_attack.h]
// Aouthor : ���� ��
//=============================================================================
#ifndef _DATA_ATTACK_H_
#define _DATA_ATTACK_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
//�}�N����`
//*****************************************************************************
#define MAX_ATTACK	(8)		// �U���̎�ނ̍ő吔

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CDataAttack
{
public:
	// �U���̎��
	typedef enum
	{
		ATTACK_TYPE_AXE = 0,				// ���F�U��1�i��
		ATTACK_TYPE_AXE_2,					// ���F�U��2�i��
		ATTACK_TYPE_AXE_3,					// ���F�U��3�i��
		ATTACK_TYPE_BOWGUN,					// �{�E�K���F�U��
		ATTACK_TYPE_WOLF00,					// �T00�F�U��
		ATTACK_TYPE_WOLF01,					// �T01�F�U��
		ATTACK_TYPE_BOSS_CLAW,				// �{�X�F�U��(��)
		ATTACK_TYPE_BOSS_DASH,				// �{�X�F�U��(�_�b�V��)
		ATTACK_TYPE_MAX
	} ATTACK_TYPE;

	// �U�����̎��
	typedef enum
	{
		ATTACK_DATA_TYPE_WAIT = 0,			// �U���\�܂ł̃t���[��
		ATTACK_DATA_TYPE_BEGIN_COLLISION,	// �R���W����������t���[��
		ATTACK_DATA_TYPE_BEGIN_EFFECT,		// �G�t�F�N�g������t���[��
		ATTACK_DATA_TYPE_TIME_COLLISION,	// �R���W�����̎�������
		ATTACK_DATA_TYPE_TIME_EFFECT,		// �G�t�F�N�g�̎�������
		ATTACK_DATA_TYPE_DAMAGE,			// �_���[�W�{��
		ATTACK_DATA_TYPE_MAX
	} ATTACK_DATA_TYPE;

	// �U�����̍\����
	typedef struct
	{
		int nWait;							// �U���\�܂ł̃t���[��
		int nBeginCollision;				// �R���W����������t���[��
		int nBeginEffect;					// �G�t�F�N�g������t���[��
		float fTimeCollision;				// �R���W�����̎�������
		float fTimeEffect;					// �G�t�F�N�g�̎�������
		float fDamage;						// �_���[�W�{��
	} ATTACK_DATA;

	CDataAttack();							// �R���X�g���N�^
	~CDataAttack();							// �f�X�g���N�^
	HRESULT Init(void);						// ����������
	void Uninit(void);						// �I������
	static CDataAttack *Create();			// ��������
	float GetAttackData(ATTACK_TYPE atkType,
		ATTACK_DATA_TYPE dataType);			// ���擾����

private:
	ATTACK_DATA m_Data[MAX_ATTACK];			// ���
};
#endif // _DATA_ATTACK_H_