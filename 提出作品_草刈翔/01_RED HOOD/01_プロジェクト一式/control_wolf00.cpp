//=============================================================================
// �G�l�~�[(�T00)���쏈�� [control_wolf00.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "control_wolf00.h"
#include "manager.h"
#include "enemy.h"
#include "motion.h"
#include "sound.h"
#include "game.h"
#include "player.h"
#include "collision_sphere.h"
#include "model.h"
#include "data_attack.h"
#include "effect_billboard.h"

//*****************************************************************************
//�}�N����`
//*****************************************************************************
#define WOLF00_GRAVITY				(0.6f)	// �d�͂̑傫��
#define WOLF00_MAX_GRAVITY			(15.0f)	// �d�͂̍ő�l
#define WOLF00_MOVE_SPEED			(0.5f)	// �ړ��ʂ̊�l
#define WOLF00_INTERIA_SUBTRACTION	(0.86f)	// �n��ł̊����̌��Z�l
#define WOLF00_MOVE_STOP_COUNT		(0.02f)	// �v���C���[�̈ړ��ʂ�0�ɂ��鎞�̈ړ��ʂ̒l
#define MAX_WOLF00_ATTACK_INTERVAL	(75)	// ��{�̍U���Ԋu
#define MAX_WOLF00_MOVE_RANGE		(2000)	// �v���C���[��ǔ�����͈�

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CControlWolf00::CControlWolf00()
{
	// �ϐ��̃N���A
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_moveOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fObjectiveRot = 0.0f;
	m_fNumRot = 0.0f;
	m_fSpeed = 0.0f;
	m_bMove = false;
	m_bRotate = false;
	m_bAttack = false;
	m_nMoveCount = 0;
	m_nAtkInterval = 0;
	m_nAtkCount = 0;
	m_nAttackWaitTime = 0;
	m_fAtkBeginEffect = 0;
	m_fAtkEffectCount = 0;
	m_pCollisionS = NULL;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CControlWolf00::~CControlWolf00()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CControlWolf00::Init(void)
{
	// �ϐ��̏�����
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_moveOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fObjectiveRot = 0.0f;
	m_fNumRot = 0.0f;
	m_nMoveCount = 0;
	m_fAtkEffectCount = 0;
	m_nAtkInterval = MAX_WOLF00_ATTACK_INTERVAL;
	m_fSpeed = WOLF00_MOVE_SPEED;
	m_bMove = false;
	m_bRotate = false;
	m_bAttack = false;
	m_fAtkBeginEffect = 0;
	m_nAttackWaitTime = 0;
	m_pCollisionS = NULL;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CControlWolf00::Uninit(void)
{
	if (m_pCollisionS != NULL)
	{
		m_pCollisionS = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CControlWolf00::Update(CScene *pScene)
{
	// �����̃|�C���^���v���C���[�N���X�̃|�C���^�ɃL���X�g
	CEnemy *pEnemy = NULL;
	pEnemy = (CEnemy*)pScene;

	// ���[�V�����擾����
	CMotion *pMotion = pEnemy->GetMotion();

	// �ړ��ʂ�ݒ�
	m_move = pEnemy->GetMove();
	m_moveOld = m_move;

	// �d��
	m_move.y -= WOLF00_GRAVITY;

	// �d�͂������Ȃ肷������d�͂̍ő�l�ŌŒ肵�Ă���
	if (m_move.y <= -WOLF00_MAX_GRAVITY)
	{
		m_move.y = -WOLF00_MAX_GRAVITY;
	}

	// �ړ�����
	Move(pEnemy);

	// �U������
	Attack(pEnemy);

	// �ړ��̊���
	MoveInteria(pEnemy);
	
	// �U�����[�V��������Ȃ����A���[�V�������Ȃ��Ă��Ȃ��Ȃ�
	if (pEnemy->GetMotion()->GetMotion() != 2 && pEnemy->GetMotion()->GetConnect() == false)
	{
		m_bAttack = false;
	}

	//---------------------------------------------------
	// ���[�V�����J��
	//---------------------------------------------------
	if (pMotion != NULL)
	{
		// �U�����Ă��Ȃ����A��_���[�W���[�V��������Ȃ��Ȃ�
		if (m_bAttack == false && pMotion->GetMotion() != 3)
		{
			// ���ݎ~�܂��Ă��邩�A�����ړ����[�V�������A���[�V�������Ȃ��Ă��Ȃ�������
			if (m_move.x == 0.0f && m_move.z == 0.0f && pMotion->GetMotion() == 1 && pMotion->GetConnect() == false)
			{
				// �j���[�g�������[�V�����ɂ���
				pMotion->SetMotion(0);
			}

			// ���ݓ����Ă��邩�A���[�V�������Ȃ��Ă��Ȃ����A�����ړ����[�V��������Ȃ��Ȃ�
			if (m_bMove == true && pMotion->GetConnect() == false && pMotion->GetMotion() != 1)
			{
				// �ړ����[�V�����ɂ���
				pMotion->SetMotion(1);
			}
		}
	}

	// �U�����̏���
	DoingAttack(pEnemy);

	// ��]�̊���(�ڂ��������͊֐��̒�)
	Rotate(pEnemy);

	//---------------------------------------------------
	// ����
	//---------------------------------------------------
	// ���ݓ����Ă���Ȃ�
	if (m_bMove == true)
	{
		// �T�E���h�擾����
		CSound *pSound = CManager::GetSound();

		if (m_nMoveCount % 32 == 0)
		{
			// �G�̑���(1)SE�̍Đ�
			pSound->Play(23);
			pSound->ControllVoice(23, 0.2f);
		}

		m_nMoveCount++;
	}
	else
	{
		m_nMoveCount = 0;
	}
}

//=============================================================================
// ��������
//=============================================================================
CControlWolf00 *CControlWolf00::Create(void)
{
	// �N���X�̐���
	CControlWolf00 *pControlEnemy = NULL;
	pControlEnemy = new CControlWolf00;

	if (pControlEnemy != NULL)
	{
		// ����������
		pControlEnemy->Init();
	}
	return pControlEnemy;
}

//=============================================================================
// �ړ�����
//=============================================================================
void CControlWolf00::Move(CEnemy *pEnemy)
{
	m_bMove = false;

	// ���[�V�����擾����
	CMotion *pMotion = pEnemy->GetMotion();

	// �����U�����[�V��������Ȃ����A��_���[�W���[�V��������Ȃ��ꍇ
	if (pMotion->GetMotion() != 2 && pMotion->GetMotion() != 3)
	{
		// �I�u�W�F�N�g��������|�C���^�E���̏���ۑ�����|�C���^
		CScene *pObject = NULL;
		CScene *pSaveObject = NULL;

		// �擪�̃|�C���^����
		pObject = pObject->GetTopObj(CScene::PRIORITY_MODEL);

		while (pObject != NULL)
		{
			// ���݂̃I�u�W�F�N�g�̃|�C���^��ۑ�
			pSaveObject = pObject;
			if (pObject->GetObjType() == CScene::OBJECT_TYPE_PLAYER)
			{
				// �v���C���[�N���X�ɃL���X�g
				CPlayer *pPlayer = NULL;
				pPlayer = (CPlayer*)pObject;

				// �v���C���[�����S���Ă��Ȃ����
				if (pPlayer->GetDeath() == false)
				{
					D3DXVECTOR3 plyPos = pPlayer->GetPos();			// �v���C���[�̈ʒu
					D3DXVECTOR3 plyPosOld = pPlayer->GetPosOld();	// �v���C���[��1�t���[���O�̈ʒu
					D3DXVECTOR3 plySize = pPlayer->GetSize();		// �v���C���[�̃T�C�Y
					D3DXVECTOR3 enePos = pEnemy->GetPos();			// �G�̈ʒu
					D3DXVECTOR3 enePosOld = pEnemy->GetPosOld();	// �G��1�t���[���O�̈ʒu

					float fRadius = sqrtf((plyPos.x - enePos.x) * (plyPos.x - enePos.x) + (plyPos.z - enePos.z) * (plyPos.z - enePos.z));
					float fRot = (float)atan2((plyPos.x - enePos.x), (plyPos.z - enePos.z)) + D3DX_PI / 2.0f;
					float fStopRadius = plySize.x * 3; // �v���C���[�̑O�Œ�~���鋗��

					// �v���C���[��ǔ�����͈͓����A��~���鋗�����ɂ��Ȃ��Ȃ�
					if (fRadius <= MAX_WOLF00_MOVE_RANGE && fRadius >= fStopRadius)
					{
						// �ړ����Ă����Ԃɂ���
						m_bMove = true;

						// �ړ��ʉ��Z
						m_move.x += -cosf(fRot) * m_fSpeed;
						m_move.z += +sinf(fRot) * m_fSpeed;

						// �ړI�̌�����ݒ�
						m_fObjectiveRot = fRot + D3DX_PI / 2.0f;
						// ��]�̊������I���ɂ���
						m_bRotate = true;
					}

					// �v���C���[��ǔ�����͈͊O�Ȃ�
					if (fRadius > MAX_WOLF00_MOVE_RANGE)
					{
						// �ړI�̌�����ݒ�
						m_fObjectiveRot = fRot + D3DX_PI / 2.0f;
						// ��]�̊������I���ɂ���
						m_bRotate = true;
					}

					// �v���C���[�̑O�Œ�~���鋗���Ȃ�
					if (fRadius <= fStopRadius)
					{
						// �U��������Ȃ�������
						if (m_bAttack == false)
						{
							//�ړI�̌�����ݒ�
							m_fObjectiveRot = fRot + D3DX_PI / 2.0f;

							//��]�̊������I���ɂ���
							m_bRotate = true;
						}
					}

					// �v���C���[����~���鋗�����ɂ���Ȃ�
					if (fRadius <= fStopRadius)
					{
						m_nAttackWaitTime++;
					}
				}
			}

			// ���̃I�u�W�F�N�g���擾
			pObject = pSaveObject->GetObjNext(pSaveObject);
		}
	}
}

//=============================================================================
// �U������
//=============================================================================
void CControlWolf00::Attack(CEnemy *pEnemy)
{
	// ���[�V�����擾����
	CMotion *pMotion = pEnemy->GetMotion();

	// �܂��U�����Ă��Ȃ����A���[�V�������Ȃ��Ă��Ȃ����A��_���[�W���[�V��������Ȃ��Ȃ�
	if (m_bAttack == false && pMotion->GetConnect() == false && pMotion->GetMotion() != 3)
	{
		if (m_nAttackWaitTime >= m_nAtkInterval)
		{
			// �U�����Ă����Ԃ�
			m_bAttack = true;

			// �U�����[�V�����ɂ���
			pMotion->SetMotion(2);

			// �J�E���^�[�̃��Z�b�g
			m_nAttackWaitTime = 0;
			m_nAtkCount = 0;

			// ���̍U������A��{�̍U���Ԋu����ɂ��ĊԊu�������_���ɂ���
			m_nAtkInterval = (rand() % 40 - 10) + MAX_WOLF00_ATTACK_INTERVAL;
		}
	}

	// �U�����[�V�������Ȃ�
	if (pMotion->GetMotion() == 2)
	{
		if (pMotion->GetNextMotion() == 3 || pMotion->GetNextMotion() == 4)
		{
			m_nAtkCount = 0;
		}

		if (pMotion->GetNextMotion() == 2)
		{
			if (m_nAtkCount >= 20 && m_nAtkCount <= 24)
			{
				// �G�l�~�[�̌������擾���A���i������
				D3DXVECTOR3 rot = pEnemy->GetRot();
				//�ړ��ʉ��Z
				m_move.x += -sinf(rot.y) * 1.5f;
				m_move.z += -cosf(rot.y) * 1.5f;
			}
		}
	}
}

//=============================================================================
// �U�����̏���
//=============================================================================
void CControlWolf00::DoingAttack(CEnemy *pEnemy)
{
	// ���[�V�����擾����
	CMotion *pMotion = pEnemy->GetMotion();

	// �U�����擾����
	CDataAttack *pDataAtk = CManager::GetDataAtk();

	// �U�����[�V�����̎�
	if (pMotion->GetMotion() == 2)
	{
		// �G�t�F�N�g�̊J�n�t���[����ݒ�
		m_fAtkBeginEffect = pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_WOLF00, CDataAttack::ATTACK_DATA_TYPE_BEGIN_EFFECT);

		// �G�t�F�N�g�̎������Ԃ�ݒ�
		m_fAtkEffectCount = pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_WOLF00, CDataAttack::ATTACK_DATA_TYPE_TIME_EFFECT);

		// ����̃t���[���ɂȂ�����R���W����������
		if (m_nAtkCount == pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_WOLF00, CDataAttack::ATTACK_DATA_TYPE_BEGIN_COLLISION))
		{
			// �؂��(1)SE�̍Đ�
			CSound *pSound = CManager::GetSound();
			pSound->Play(17);
			pSound->ControllVoice(17, 0.6f);

			// �G�l�~�[�̉E��ɂ����郂�f���̈ʒu�E�����擾
			D3DXVECTOR3 modelPos = pEnemy->GetModelPos(4);
			CModel *pModel = pEnemy->GetModel(4);

			// �E��ɃR���W����(�G�U������)�����A�e�q�֌W����������
			m_pCollisionS = CCollisionSphere::Create(D3DXVECTOR3(modelPos.x, modelPos.y, modelPos.z), 100.0f, CCollisionSphere::COLLISION_S_TYPE::COLLISION_S_TYPE_ENEMY_ATTACK, pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_WOLF00, CDataAttack::ATTACK_DATA_TYPE_TIME_COLLISION));
			m_pCollisionS->SetParent(pModel);

			// �R���W�����������̈ʒu���A���g�̈ʒu�Ƃ��Đݒ�
			D3DXVECTOR3 enemyPos = pEnemy->GetPos();
			m_pCollisionS->SetPosSource(enemyPos);

			// �{�������̂܂܍U���͂Ƃ��Đݒ�
			m_pCollisionS->SetDamageValue(pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_WOLF00, CDataAttack::ATTACK_DATA_TYPE_DAMAGE));
		}
	}

	// �U�������A���̃��[�V���������݂̃��[�V�����Ɠ����Ȃ�
	if (m_bAttack == true && pMotion->GetNextMotion() == pMotion->GetMotion())
	{
		// ����̃t���[���ɂȂ������A�������Ԓ��Ȃ�
		if (m_nAtkCount >= m_fAtkBeginEffect && m_nAtkCount <= m_fAtkBeginEffect + m_fAtkEffectCount)
		{
			D3DXVECTOR3 modelPos = pEnemy->GetModelPos(4);
			CModel *pModel = pEnemy->GetModel(4);

			// �O�ՂɃG�t�F�N�g������
			CEffectB::Create(D3DXVECTOR3(modelPos.x - 25.0f, modelPos.y, modelPos.z - 10.0f), D3DXVECTOR3(30.0f, 30.0f, 0.0f),
				D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 35, CEffectB::EFFECT_TYPE_SMALL, pModel);
			CEffectB::Create(D3DXVECTOR3(modelPos.x - 25.0f, modelPos.y, modelPos.z + 5.0f), D3DXVECTOR3(30.0f, 30.0f, 0.0f),
				D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 35, CEffectB::EFFECT_TYPE_SMALL, pModel);
			CEffectB::Create(D3DXVECTOR3(modelPos.x - 5.0f, modelPos.y, modelPos.z + 25.0f), D3DXVECTOR3(30.0f, 30.0f, 0.0f),
				D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 35, CEffectB::EFFECT_TYPE_SMALL, pModel);
		}
		// �J�E���g�𑝂₷
		m_nAtkCount++;
	}

	// ��_���[�W���[�V�����A�܂��̓_�E�����[�V�����ɂȂ�����
	if (pMotion->GetMotion() == 3 || pMotion->GetMotion() == 4)
	{
		if (m_pCollisionS != NULL)
		{
			// �U���̓����蔻�������
			m_pCollisionS->SetTime(0);
			m_nAttackWaitTime = 0;
		}
	}
}

//=============================================================================
// �ړ��̊����ɂ��Ă̏���
//=============================================================================
void CControlWolf00::MoveInteria(CEnemy *pEnemy)
{
	// �����̌��Z
	m_move.z *= WOLF00_INTERIA_SUBTRACTION;
	m_move.x *= WOLF00_INTERIA_SUBTRACTION;

	// �ړ��ʂ�����̒l�ɂȂ�����0�ɂ���
	if (m_move.x <= WOLF00_MOVE_STOP_COUNT && m_move.x >= -WOLF00_MOVE_STOP_COUNT)
	{
		m_move.x = 0.0f;
	}
	if (m_move.z <= WOLF00_MOVE_STOP_COUNT && m_move.z >= -WOLF00_MOVE_STOP_COUNT)
	{
		m_move.z = 0.0f;
	}
}

//=============================================================================
// ��]����
//=============================================================================
void CControlWolf00::Rotate(CEnemy *pEnemy)
{
	// �v���C���[�̌������擾
	D3DXVECTOR3 rotPlayer = pEnemy->GetRot();

	// ��]�������ԂȂ�
	if (m_bRotate == true)
	{
		// �ړI�̌������v�Z
		if (m_fObjectiveRot > D3DX_PI)
		{
			m_fObjectiveRot = -D3DX_PI - (D3DX_PI - m_fObjectiveRot);
		}
		else if (m_fObjectiveRot < -D3DX_PI)
		{
			m_fObjectiveRot = D3DX_PI - (-D3DX_PI - m_fObjectiveRot);
		}

		// �v���C���[�̌��݂̌������Ƃɂ��ꂼ�������ς���ʂ��v�Z
		if (rotPlayer.y < 0.0f && -rotPlayer.y + m_fObjectiveRot > D3DX_PI)
		{
			m_fNumRot = (-D3DX_PI - rotPlayer.y) + -(D3DX_PI - m_fObjectiveRot);
		}
		else if (rotPlayer.y >= D3DX_PI / 2.0f && rotPlayer.y - m_fObjectiveRot > D3DX_PI)
		{
			m_fNumRot = (D3DX_PI - rotPlayer.y) - (-D3DX_PI - m_fObjectiveRot);
		}
		else
		{
			m_fNumRot = (m_fObjectiveRot - rotPlayer.y);
		}

		// �v���C���[�̌����ɉ��Z
		rotPlayer.y += m_fNumRot * 0.2f;

		// ����̒l�ɒB�������]����߂�
		if (rotPlayer.y - m_fObjectiveRot < 0.001 && rotPlayer.y - m_fObjectiveRot > -0.001)
		{
			m_bRotate = false;
		}
	}

	// ������3.14����-3.14�̒l�͈͓̔��Ɏ��߂�
	if (rotPlayer.y > D3DX_PI)
	{
		rotPlayer.y -= D3DX_PI * 2.0f;
	}
	else if (rotPlayer.y < -D3DX_PI)
	{
		rotPlayer.y += D3DX_PI * 2.0f;
	}

	// �����𔽉f
	pEnemy->SetRot(rotPlayer);
}