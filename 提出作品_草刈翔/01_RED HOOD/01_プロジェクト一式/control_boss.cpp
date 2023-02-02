//=============================================================================
// �G�l�~�[(�{�X)���쏈�� [control_enemy_boss.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "control_boss.h"
#include "manager.h"
#include "enemy.h"
#include "motion.h"
#include "sound.h"
#include "player.h"
#include "collision_sphere.h"
#include "data_attack.h"
#include "effect_billboard.h"
#include "particle_dash.h"
#include "particle_circle.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BOSS_GRAVITY				(0.6f)	// �d�͂̑傫��
#define BOSS_MAX_GRAVITY			(15.0f)	// �d�͂̍ő�l
#define BOSS_MOVE_SPEED				(0.5f)	// �ړ��ʂ̊�l
#define BOSS_INTERIA_SUBTRACTION	(0.86f)	// �n��ł̊����̌��Z�l
#define BOSS_MOVE_STOP_COUNT		(0.02f)	// �v���C���[�̈ړ��ʂ�0�ɂ��鎞�̈ړ��ʂ̒l
#define MAX_BOSS_JUMP_INTERVAL		(100)	// �W�����v�̊Ԋu
#define BOSS_JUMP_NORMAL			(23.0f)	// �ʏ�̃W�����v��
#define BOSS_JUMP_BIG				(47.0f)	// ��W�����v�ł̃W�����v��
#define BOSS_JUMP_BIG_GRAVITY		(45.0f)	// ��W�����v���̏d�͂̍ő�l
#define MAX_BOSS_ATTACK_INTERVAL	(80)	// ��{�̍U���Ԋu
#define BOSS_DASH_TIME				(70)	// �_�b�V������
#define BOSS_CLAW_NUM				(3)		// �܍U���̉�
#define BOSS_JUMP_NUM				(3)		// �W�����v��
#define BOSS_AREA	(D3DXVECTOR3(0.0f, 0.0f, 10000.0f))	// �{�X��G���A�̒��S�ʒu

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CControlBoss::CControlBoss()
{
	// �ϐ��̃N���A
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_moveOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posDash = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fObjectiveRot = 0.0f;
	m_fNumRot = 0.0f;
	m_fSpeed = 0.0f;
	m_bRotate = false;
	m_bClaw = false;
	m_nAtkInterval = 0;
	m_nAtkCount = 0;
	m_nClawNum = 0;
	m_nJumpInterval = 0;
	m_nJump = 0;
	m_nJumpNum = 0;
	m_nMoveCount = 0;
	m_nMoveJump = 0;
	m_nMoveStop = 0;
	m_fAtkBeginEffect = 0;
	m_fAtkEffectCount = 0;
	m_nAttackWaitTime = 0;
	m_nDashCount = 0;
	m_nDashNum = 0;
	memset(&m_pCollisionS, NULL, sizeof(m_pCollisionS));
	m_think = BOSS_THINK_STOP;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CControlBoss::~CControlBoss()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CControlBoss::Init(void)
{
	// �ϐ��̏�����
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_moveOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posDash = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fObjectiveRot = 0.0f;
	m_fNumRot = 0.0f;
	m_fSpeed = BOSS_MOVE_SPEED;
	m_bRotate = false;
	m_bClaw = false;
	m_nAtkInterval = MAX_BOSS_ATTACK_INTERVAL;
	m_nAtkCount = 0;
	m_nClawNum = 0;
	m_nJumpInterval = MAX_BOSS_JUMP_INTERVAL;
	m_nJump = rand() % 3 + 1;
	m_nJumpNum = 0;
	m_nMoveCount = 0;
	m_nMoveJump = 0;
	m_nMoveStop = 0;
	m_fAtkBeginEffect = 0;
	m_fAtkEffectCount = 0;
	m_nAttackWaitTime = 0;
	m_nDashCount = 0;
	m_nDashNum = 0;
	for (int nCntColl = 0; nCntColl < MAX_BOSS_COLLSION; nCntColl++)
	{
		m_pCollisionS[nCntColl] = NULL;
	}
	m_think = BOSS_THINK_MOVE;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CControlBoss::Uninit(void)
{
	for (int nCntColl = 0; nCntColl < MAX_BOSS_COLLSION; nCntColl++)
	{
		if (m_pCollisionS[nCntColl]  != NULL)
		{
			m_pCollisionS[nCntColl] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CControlBoss::Update(CScene *pScene)
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
	m_move.y -= BOSS_GRAVITY;


	if (m_think == BOSS_THINK_JUMP_BIG)
	{
		// �d�͂������Ȃ肷������d�͂̍ő�l�ŌŒ肵�Ă���
		if (m_move.y <= -BOSS_JUMP_BIG_GRAVITY)
		{
			m_move.y = -BOSS_JUMP_BIG_GRAVITY;
		}
	}
	else
	{
		// �d�͂������Ȃ肷������d�͂̍ő�l�ŌŒ肵�Ă���
		if (m_move.y <= -BOSS_MAX_GRAVITY)
		{
			m_move.y = -BOSS_MAX_GRAVITY;
		}
	}

	if (m_think == BOSS_THINK_STOP)
	{
		m_nMoveStop--;

		if (m_nMoveStop <= 0)
		{
			pMotion->SetMotion(1);
			m_think = BOSS_THINK_MOVE;
		}
	}

	// �ړ�����
	Move(pEnemy);

	if (m_bClaw == false)
	{
		// �W�����v����
		Jump(pEnemy);
	}

	// �U��(��)����
	ClawAttack(pEnemy);

	// �U��(�_�b�V��)����
	DashAttack(pEnemy);

	// �ړ��̊���
	MoveInteria(pEnemy);

	// �U�����[�V��������Ȃ����A���[�V�������Ȃ��Ă��Ȃ��Ȃ�
	if (pMotion->GetMotion() != 2 && pMotion->GetConnect() == false)
	{
		m_bClaw = false;
	}

	//---------------------------------------------------
	// ���[�V�����J��
	//---------------------------------------------------
	// ���ݎ~�܂��Ă��邩�A�����ړ����[�V�������A���[�V�������Ȃ��Ă��Ȃ�������
	if (m_move.x == 0.0f && m_move.z == 0.0f && pMotion->GetMotion() == 1 && pMotion->GetConnect() == false)
	{
		// �j���[�g�������[�V�����ɂ���
		pMotion->SetMotion(0);
	}

	// ���ݓ����Ă��ă��[�V�������Ȃ��Ă��Ȃ����A�����j���[�g�������[�V�����ňړ����[�V��������Ȃ��Ȃ�
	if ((m_move.x != 0.0f || m_move.z != 0.0f) && pMotion->GetConnect() == false &&
		pMotion->GetMotion() == 0 && pMotion->GetMotion() != 1)
	{
		// �ړ����[�V�����ɂ���
		pMotion->SetMotion(1);
	}

	// �U�����̃R���W����
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

		if (m_nMoveCount % 50 == 0)
		{
			// �G�̒��nSE�̍Đ�
			pSound->Play(20);
			pSound->ControllVoice(20, 0.15f);
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
CControlBoss *CControlBoss::Create(void)
{
	// �N���X�̐���
	CControlBoss *pControlBoss = NULL;
	pControlBoss = new CControlBoss;

	if (pControlBoss != NULL)
	{
		// ����������
		pControlBoss->Init();
	}
	return pControlBoss;
}

//=============================================================================
// �ړ�����
//=============================================================================
void CControlBoss::Move(CEnemy *pEnemy)
{
	// ���[�V�����擾����
	CMotion *pMotion = pEnemy->GetMotion();

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
				float fStopRadius = plySize.x * 6; // �v���C���[�̑O�Œ�~���鋗��

				// �v�l��<�ړ�>�̏ꍇ
				if (m_think == BOSS_THINK_MOVE)
				{
					// ��~���鋗�����ɂ��Ȃ��Ȃ�
					if (fRadius >= fStopRadius)
					{
						// �ړ����Ă����Ԃɂ���
						m_bMove = true;

						// �W�����v�񐔂�3�񖢖���������
						if (m_nJumpNum < BOSS_JUMP_NUM)
						{
							if (m_nMoveJump >= m_nJumpInterval)
							{
								// �v�l��<�ʏ�W�����v>�ɂ���
								m_think = BOSS_THINK_JUMP_NORMAL;
								m_nMoveJump = 0;

								// ���̃W�����v����A��{�̊Ԋu����ɂ��ĊԊu�������_���ɂ���
								m_nJumpInterval = (rand() % 60) + MAX_BOSS_JUMP_INTERVAL;
							}
						}
						else
						{
							if (m_nMoveJump >= 150)
							{
								// �v�l��<��W�����v>�ɂ���
								m_think = BOSS_THINK_JUMP_BIG;
								m_nMoveJump = 0;
							}
						}

						//�ړ��ʉ��Z
						m_move.x += -cosf(fRot) * m_fSpeed;
						m_move.z += +sinf(fRot) * m_fSpeed;

						//�ړI�̌�����ݒ�
						m_fObjectiveRot = fRot + D3DX_PI / 2.0f;

						//��]�̊������I���ɂ���
						m_bRotate = true;

						// �J�E���g�𑝂₷
						m_nMoveJump++;
					}

					// �v���C���[�̑O�Œ�~���鋗���Ȃ�
					if (fRadius <= fStopRadius)
					{
						// �ړ����Ă��Ȃ���Ԃɂ���
						m_bMove = false;

						// �U��������Ȃ�������
						if (m_bClaw == false)
						{
							//�ړI�̌�����ݒ�
							m_fObjectiveRot = fRot + D3DX_PI / 2.0f;

							//��]�̊������I���ɂ���
							m_bRotate = true;
						}
					}

					// �v���C���[�ɋ߂��ꍇ���A�U�����[�V��������Ȃ��Ȃ�
					if (fRadius <= fStopRadius && pMotion->GetMotion() != 2)
					{
						m_nAttackWaitTime++;
					}
				}
				// �v�l��<�ʏ�W�����v>�̏ꍇ
				else if (m_think == BOSS_THINK_JUMP_NORMAL)
				{
					// �W�����v(����)���[�V�����Ȃ�
					if (pMotion->GetMotion() == 4)
					{
						//�ړI�̌�����ݒ�
						m_fObjectiveRot = fRot + D3DX_PI / 2.0f;
						//��]�̊������I���ɂ���
						m_bRotate = true;
					}
					// �W�����v���[�V�����Ȃ�
					else if (pMotion->GetMotion() == 5)
					{
						// �G�l�~�[�̌������擾���A���i������
						D3DXVECTOR3 rot = pEnemy->GetRot();
						m_move.x += -cosf(rot.y - D3DX_PI / 2.0f) * m_fSpeed * 3;
						m_move.z += +sinf(rot.y - D3DX_PI / 2.0f) * m_fSpeed * 3;
					}
				}
				// �v�l��<��W�����v>�̏ꍇ
				else if (m_think == BOSS_THINK_JUMP_NORMAL)
				{
					// �W�����v���[�V�����Ȃ�
					if (pMotion->GetMotion() == 5 && pMotion->GetStop() == true)
					{
						//�ړI�̌�����ݒ�
						m_fObjectiveRot = fRot + D3DX_PI / 2.0f;
						//��]�̊������I���ɂ���
						m_bRotate = true;
					}
				}
				// �v�l��<�`���[�W>�܂���<�_�b�V���^�[��>�̏ꍇ
				else if (m_think == BOSS_THINK_CHARGE || m_think == BOSS_THINK_DASH_TURN)
				{
					//�ړI�̌�����ݒ�
					m_fObjectiveRot = fRot + D3DX_PI / 2.0f;
					//��]�̊������I���ɂ���
					m_bRotate = true;
				}

				if (m_think != BOSS_THINK_MOVE)
				{
					m_bMove = false;
				}
			}
		}

		// ���̃I�u�W�F�N�g���擾
		pObject = pSaveObject->GetObjNext(pSaveObject);
	}
}

//=============================================================================
// �U��(��)����
//=============================================================================
void CControlBoss::ClawAttack(CEnemy *pEnemy)
{
	// ���[�V�����擾����
	CMotion *pMotion = pEnemy->GetMotion();

	// �܍U����3��s������
	if (m_nClawNum >= BOSS_CLAW_NUM)
	{
		m_nClawNum = 0;

		// �W�����v�񐔂�3�񖢖���������
		if (m_nJumpNum < BOSS_JUMP_NUM)
		{
			// �v�l��<�ʏ�W�����v>�ɂ���
			m_think = BOSS_THINK_JUMP_NORMAL;
			m_nMoveJump = 0;

			// ���̃W�����v����A��{�̊Ԋu����ɂ��ĊԊu�������_���ɂ���
			m_nJumpInterval = (rand() % 60) + MAX_BOSS_JUMP_INTERVAL;
		}
		else
		{
			// �v�l��<��W�����v>�ɂ���
			m_think = BOSS_THINK_JUMP_BIG;
			m_nMoveJump = 0;
		}
	}

	// �܂��U�����Ă��Ȃ����A���[�V�������Ȃ��Ă��Ȃ�����
	if (m_bClaw == false && pMotion->GetConnect() == false)
	{
		if (m_nAttackWaitTime >= m_nAtkInterval)
		{
			m_nClawNum++;

			// �U��(��)���[�V�����ɂ���
			pMotion->SetMotion(2);

			m_bClaw = true;

			// �J�E���^�[�̃��Z�b�g
			m_nAttackWaitTime = 0;
			m_nAtkCount = 0;

			// ���̍U������A��{�̍U���Ԋu����ɂ��ĊԊu�������_���ɂ���
			m_nAtkInterval = (rand() % 60 - 30) + MAX_BOSS_ATTACK_INTERVAL;
		}
	}

	// �U�����[�V�������Ȃ�
	if (pMotion->GetMotion() == 2)
	{
		//��]�̊������I�t�ɂ���
		m_bRotate = false;

		if (m_nAtkCount >= 24 && m_nAtkCount <= 28)
		{
			// �G�l�~�[�̌������擾���A���i������
			D3DXVECTOR3 rot = pEnemy->GetRot();
			//�ړ��ʉ��Z
			m_move.x += -sinf(rot.y) * 1.5f;
			m_move.z += -cosf(rot.y) * 1.5f;
		}
	}
}

//=============================================================================
// �U��(�_�b�V��)����
//=============================================================================
void CControlBoss::DashAttack(CEnemy *pEnemy)
{
	// ���[�V�����擾����
	CMotion *pMotion = pEnemy->GetMotion();

	// �T�E���h�擾����
	CSound *pSound = CManager::GetSound();

	// �v�l��<�`���[�W�J�n>�̏ꍇ
	if (m_think == BOSS_THINK_CHARGE_BEGIN)
	{
		// �`���[�W(�{�X)SE�̍Đ�
		pSound->Play(21);
		pSound->ControllVoice(21, 0.3f);

		// ���C�t�̊�������U���񐔂�ݒ�
		float fLifeRatio = pEnemy->GetLife() / BEGIN_BOSS_LIFE;

		// �̗͂�7������Ȃ�
		if (fLifeRatio > 0.7f)
		{
			m_nDashNum = 1;
		}
		// �̗͂�3������A7���ȉ��Ȃ�
		else if (fLifeRatio >= 0.3f && fLifeRatio <= 0.7f)
		{
			m_nDashNum = 2;
		}
		// �̗͂�3���ȉ��Ȃ�
		else if (fLifeRatio <= 0.3f)
		{
			m_nDashNum = 3;
		}

		m_think = BOSS_THINK_CHARGE;
	}
	// �v�l��<�`���[�W>�̏ꍇ
	if (m_think == BOSS_THINK_CHARGE)
	{
		// �J�E���g�����炷
		m_nDashCount--;

		// �J�E���g��0�ɂȂ�����
		if (m_nDashCount == 0)
		{
			// �_�b�V�����[�V�����ɂ���
			pMotion->SetMotion(8);

			// �v�l��<�_�b�V��>�ɂ���
			m_think = BOSS_THINK_DASH;
			m_nDashCount = 0;

			// �J�E���^�[�̃��Z�b�g
			m_nAtkCount = 0;
		}
	}
	// �v�l��<�_�b�V��>�̏ꍇ
	if (m_think == BOSS_THINK_DASH)
	{
		// �`���[�W(�{�X)SE�̒�~
		pSound->Stop(21);

		if (m_nDashCount % 18 == 0)
		{
			// �_�b�V��(�{�X)SE�̍Đ�
			pSound->Play(22);
			pSound->ControllVoice(22, 0.3f);
		}

		// �J�E���g�𑝂₷
		m_nDashCount++;

		// �G�l�~�[�̌������擾���A���i������
		D3DXVECTOR3 rot = pEnemy->GetRot();
		m_move.x += -sinf(rot.y) * 5.0f;
		m_move.z += -cosf(rot.y) * 5.0f;

		// �_�b�V���̃p�[�e�B�N���𔭐�������
		D3DXVECTOR3 pos = pEnemy->GetPos();
		D3DXVECTOR3 size = pEnemy->GetSize();
		pos.y += size.y / 3.0f;
		CParticleDash::Create(pos, D3DXVECTOR3(120.0f, 120.0f, 0.0f), rot, D3DCOLOR_RGBA(255, 255, 128, 153), 6, 20.0f, 350.0f, 85.0f, 3);

		// �_�b�V�����Ԃ��I�������
		if (m_nDashCount >= BOSS_DASH_TIME)
		{
			// �v�l��<�_�b�V���^�[��>�ɂ���
			m_think = BOSS_THINK_DASH_TURN;
			m_nDashCount = 0;
		}
	}
	// �v�l��<�_�b�V���^�[��>�̏ꍇ
	if (m_think == BOSS_THINK_DASH_TURN)
	{
		if (pMotion->GetConnect() == false && pMotion->GetMotion() != 7)
		{
			// �U���񐔂����炷
			m_nDashNum--;

			// �`���[�W���[�V�����ɂ���(����̕\��)
			pMotion->SetMotion(7);
		}

		// �U���񐔂��c���Ă�����
		if (m_nDashNum > 0)
		{
			// �`���[�W(�{�X)SE�̍Đ�
			pSound->Play(21);
			pSound->ControllVoice(21, 0.3f);

			// �v�l��<�`���[�W>�ɂ��A�~�߂鎞�Ԃ�ݒ�
			m_think = BOSS_THINK_CHARGE;
			m_nDashCount = 100;
		}
		// �U���񐔂��c���Ă��Ȃ�������
		else if (m_nDashNum <= 0)
		{
			// �v�l��<�_�b�V���I��>�ɂ���
			m_think = BOSS_THINK_DASH_END;
		}
	}
	// �v�l��<�_�b�V���I��>�̏ꍇ
	if (m_think == BOSS_THINK_DASH_END)
	{
		m_nDashCount++;

		if (m_nDashCount >= BOSS_DASH_TIME)
		{
			// �v�l��<��~>�ɂ��A�~�߂鎞�Ԃ�ݒ�
			m_think = BOSS_THINK_STOP;
			m_nMoveStop = 50;
			m_nDashCount = 0;
		}
	}
}

//=============================================================================
// �U�����Ȃǂ̃R���W������������
//=============================================================================
void CControlBoss::DoingAttack(CEnemy *pEnemy)
{
	// ���[�V�����擾����
	CMotion *pMotion = pEnemy->GetMotion();

	// �U�����擾����
	CDataAttack *pDataAtk = CManager::GetDataAtk();

	// �U��(��)���[�V�����̎�
	if (pMotion->GetMotion() == 2)
	{
		// �G�t�F�N�g�̊J�n�t���[����ݒ�
		m_fAtkBeginEffect = pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOSS_CLAW, CDataAttack::ATTACK_DATA_TYPE_BEGIN_EFFECT);

		// �G�t�F�N�g�̎������Ԃ�ݒ�
		m_fAtkEffectCount = pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOSS_CLAW, CDataAttack::ATTACK_DATA_TYPE_TIME_EFFECT);

		if (m_nAtkCount == 0)
		{
			// �؂��(2)SE�̍Đ�
			CSound *pSound = CManager::GetSound();
			pSound->Play(18);
			pSound->ControllVoice(18, 0.6f);
		}

		// ����̃t���[���ɂȂ�����R���W����������
		if (m_nAtkCount == pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOSS_CLAW, CDataAttack::ATTACK_DATA_TYPE_BEGIN_COLLISION))
		{
			// �G�l�~�[�̉E��ɃR���W����������
			D3DXVECTOR3 modelPos = pEnemy->GetModelPos(5);
			CModel *pModel = pEnemy->GetModel(5);
			m_pCollisionS[0] = CCollisionSphere::Create(D3DXVECTOR3(modelPos.x + 60.0f, modelPos.y, modelPos.z), 140.0f, CCollisionSphere::COLLISION_S_TYPE::COLLISION_S_TYPE_ENEMY_ATTACK, pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOSS_CLAW, CDataAttack::ATTACK_DATA_TYPE_TIME_COLLISION));
			m_pCollisionS[0]->SetParent(pModel);

			// �G�l�~�[�̍���ɃR���W����������
			modelPos = pEnemy->GetModelPos(8);
			pModel = pEnemy->GetModel(8);
			m_pCollisionS[1] = CCollisionSphere::Create(D3DXVECTOR3(modelPos.x - 60.0f, modelPos.y, modelPos.z), 140.0f, CCollisionSphere::COLLISION_S_TYPE::COLLISION_S_TYPE_ENEMY_ATTACK, pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOSS_CLAW, CDataAttack::ATTACK_DATA_TYPE_TIME_COLLISION));
			m_pCollisionS[1]->SetParent(pModel);

			// �G�l�~�[�̉E�r�ɃR���W����������
			modelPos = pEnemy->GetModelPos(4);
			pModel = pEnemy->GetModel(4);
			m_pCollisionS[2] = CCollisionSphere::Create(D3DXVECTOR3(modelPos.x + 60.0f, modelPos.y, modelPos.z), 140.0f, CCollisionSphere::COLLISION_S_TYPE::COLLISION_S_TYPE_ENEMY_ATTACK, pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOSS_CLAW, CDataAttack::ATTACK_DATA_TYPE_TIME_COLLISION));
			m_pCollisionS[2]->SetParent(pModel);

			// �G�l�~�[�̍��r�ɃR���W����������
			modelPos = pEnemy->GetModelPos(7);
			pModel = pEnemy->GetModel(7);
			m_pCollisionS[3] = CCollisionSphere::Create(D3DXVECTOR3(modelPos.x - 60.0f, modelPos.y, modelPos.z), 140.0f, CCollisionSphere::COLLISION_S_TYPE::COLLISION_S_TYPE_ENEMY_ATTACK, pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOSS_CLAW, CDataAttack::ATTACK_DATA_TYPE_TIME_COLLISION));
			m_pCollisionS[3]->SetParent(pModel);

			// �R���W�����������̈ʒu���A���g�̈ʒu�Ƃ��Đݒ�
			D3DXVECTOR3 enemyPos = pEnemy->GetPos();
			m_pCollisionS[0]->SetPosSource(enemyPos);
			m_pCollisionS[1]->SetPosSource(enemyPos);
			m_pCollisionS[2]->SetPosSource(enemyPos);
			m_pCollisionS[3]->SetPosSource(enemyPos);

			// �{�������̂܂܍U���͂Ƃ��Đݒ�
			m_pCollisionS[0]->SetDamageValue(pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOSS_CLAW, CDataAttack::ATTACK_DATA_TYPE_DAMAGE));
			m_pCollisionS[1]->SetDamageValue(pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOSS_CLAW, CDataAttack::ATTACK_DATA_TYPE_DAMAGE));
			m_pCollisionS[2]->SetDamageValue(pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOSS_CLAW, CDataAttack::ATTACK_DATA_TYPE_DAMAGE));
			m_pCollisionS[3]->SetDamageValue(pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOSS_CLAW, CDataAttack::ATTACK_DATA_TYPE_DAMAGE));
		}
	}

	// �U�������A���̃��[�V���������݂̃��[�V�����Ɠ����Ȃ�
	if (m_bClaw == true && pMotion->GetNextMotion() == pMotion->GetMotion())
	{
		// ����̃t���[���ɂȂ������A�������Ԓ��Ȃ�
		if (m_nAtkCount >= m_fAtkBeginEffect && m_nAtkCount <= m_fAtkBeginEffect + m_fAtkEffectCount)
		{
			D3DXVECTOR3 modelPos = pEnemy->GetModelPos(5);
			CModel *pModel = pEnemy->GetModel(5);

			// �܂̋O�ՂɃG�t�F�N�g������
			CEffectB::Create(D3DXVECTOR3(modelPos.x - 25.0f, modelPos.y, modelPos.z - 15.0f), D3DXVECTOR3(45.0f, 45.0f, 0.0f), D3DCOLOR_RGBA(255, 0, 0, 255), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 55, CEffectB::EFFECT_TYPE_SMALL, pModel);
			CEffectB::Create(D3DXVECTOR3(modelPos.x - 25.0f, modelPos.y, modelPos.z + 20.0f), D3DXVECTOR3(45.0f, 45.0f, 0.0f), D3DCOLOR_RGBA(255, 0, 0, 255), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 55, CEffectB::EFFECT_TYPE_SMALL, pModel);
			CEffectB::Create(D3DXVECTOR3(modelPos.x - 5.0f, modelPos.y, modelPos.z + 50.0f), D3DXVECTOR3(45.0f, 45.0f, 0.0f), D3DCOLOR_RGBA(255, 0, 0, 255), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 55, CEffectB::EFFECT_TYPE_SMALL, pModel);

			modelPos = pEnemy->GetModelPos(8);
			pModel = pEnemy->GetModel(8);

			// �܂̋O�ՂɃG�t�F�N�g������
			CEffectB::Create(D3DXVECTOR3(modelPos.x + 25.0f, modelPos.y, modelPos.z - 15.0f), D3DXVECTOR3(45.0f, 45.0f, 0.0f), D3DCOLOR_RGBA(255, 0, 0, 255), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 55, CEffectB::EFFECT_TYPE_SMALL, pModel);
			CEffectB::Create(D3DXVECTOR3(modelPos.x + 25.0f, modelPos.y, modelPos.z + 20.0f), D3DXVECTOR3(45.0f, 45.0f, 0.0f), D3DCOLOR_RGBA(255, 0, 0, 255), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 55, CEffectB::EFFECT_TYPE_SMALL, pModel);
			CEffectB::Create(D3DXVECTOR3(modelPos.x + 5.0f, modelPos.y, modelPos.z + 50.0f), D3DXVECTOR3(45.0f, 45.0f, 0.0f), D3DCOLOR_RGBA(255, 0, 0, 255), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 55, CEffectB::EFFECT_TYPE_SMALL, pModel);
		}
		// �J�E���g�𑝂₷
		m_nAtkCount++;
	}

	// �U��(�_�b�V��)���[�V�����̎�
	if (pMotion->GetMotion() == 8)
	{
		// ����̃t���[���ɂȂ�����R���W����������
		if (m_nAtkCount == pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOSS_DASH, CDataAttack::ATTACK_DATA_TYPE_BEGIN_COLLISION))
		{
			// �G�l�~�[�̑̂ɃR���W����������
			D3DXVECTOR3 modelPos = pEnemy->GetModelPos(1);
			CModel *pModel = pEnemy->GetModel(1);
			m_pCollisionS[0] = CCollisionSphere::Create(D3DXVECTOR3(modelPos.x, modelPos.y + 100.0f, modelPos.z - 50.0f), 300.0f, CCollisionSphere::COLLISION_S_TYPE::COLLISION_S_TYPE_ENEMY_ATTACK, pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOSS_DASH, CDataAttack::ATTACK_DATA_TYPE_TIME_COLLISION));
			m_pCollisionS[0]->SetParent(pModel);

			// �G�l�~�[�̓��ɃR���W����������
			modelPos = pEnemy->GetModelPos(2);
			pModel = pEnemy->GetModel(2);
			m_pCollisionS[1] = CCollisionSphere::Create(D3DXVECTOR3(modelPos.x, modelPos.y - 100.0f, modelPos.z + 40.0f), 200.0f, CCollisionSphere::COLLISION_S_TYPE::COLLISION_S_TYPE_ENEMY_ATTACK, pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOSS_DASH, CDataAttack::ATTACK_DATA_TYPE_TIME_COLLISION));
			m_pCollisionS[1]->SetParent(pModel);

			// �G�l�~�[�̉E��ɃR���W����������
			modelPos = pEnemy->GetModelPos(5);
			pModel = pEnemy->GetModel(5);
			m_pCollisionS[2] = CCollisionSphere::Create(D3DXVECTOR3(modelPos.x + 60.0f, modelPos.y, modelPos.z), 70.0f, CCollisionSphere::COLLISION_S_TYPE::COLLISION_S_TYPE_ENEMY_ATTACK, pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOSS_DASH, CDataAttack::ATTACK_DATA_TYPE_TIME_COLLISION));
			m_pCollisionS[2]->SetParent(pModel);

			// �G�l�~�[�̍���ɃR���W����������
			modelPos = pEnemy->GetModelPos(8);
			pModel = pEnemy->GetModel(8);
			m_pCollisionS[3] = CCollisionSphere::Create(D3DXVECTOR3(modelPos.x - 60.0f, modelPos.y, modelPos.z), 70.0f, CCollisionSphere::COLLISION_S_TYPE::COLLISION_S_TYPE_ENEMY_ATTACK, pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOSS_DASH, CDataAttack::ATTACK_DATA_TYPE_TIME_COLLISION));
			m_pCollisionS[3]->SetParent(pModel);

			// �R���W�����������̈ʒu���A���g�̈ʒu�Ƃ��Đݒ�
			D3DXVECTOR3 enemyPos = pEnemy->GetPos();
			m_pCollisionS[0]->SetPosSource(enemyPos);
			m_pCollisionS[1]->SetPosSource(enemyPos);
			m_pCollisionS[2]->SetPosSource(enemyPos);
			m_pCollisionS[3]->SetPosSource(enemyPos);

			// �{�������̂܂܍U���͂Ƃ��Đݒ�
			m_pCollisionS[0]->SetDamageValue(pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOSS_DASH, CDataAttack::ATTACK_DATA_TYPE_DAMAGE));
			m_pCollisionS[1]->SetDamageValue(pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOSS_DASH, CDataAttack::ATTACK_DATA_TYPE_DAMAGE));
			m_pCollisionS[2]->SetDamageValue(pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOSS_DASH, CDataAttack::ATTACK_DATA_TYPE_DAMAGE));
			m_pCollisionS[3]->SetDamageValue(pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOSS_DASH, CDataAttack::ATTACK_DATA_TYPE_DAMAGE));
		}

		m_nAtkCount++;
	}

	// �U���ȊO�̃��[�V�����ɂȂ�����
	if (pMotion->GetMotion() != 2 && pMotion->GetMotion() != 8)
	{
		for (int nCntColl = 0; nCntColl < MAX_BOSS_COLLSION; nCntColl++)
		{
			if (m_pCollisionS[nCntColl] != NULL)
			{
				// �����蔻�������
				m_pCollisionS[nCntColl] = NULL;
			}
		}
	}
}

//=============================================================================
// �W�����v����
//=============================================================================
void CControlBoss::Jump(CEnemy *pEnemy)
{
	// ���[�V�����擾����
	CMotion *pMotion = pEnemy->GetMotion();

	// �T�E���h�擾����
	CSound *pSound = CManager::GetSound();

	// �G�l�~�[�����n���Ă���Ȃ�
	if (pEnemy->GetLand() == true)
	{
		// �v�l��<�ʏ�W�����v>�̎�
		if (m_think == BOSS_THINK_JUMP_NORMAL)
		{
			// ���[�V�������Ȃ��Ă��Ȃ����A�W�����v(����)���[�V��������Ȃ����A���[�V�������~�܂��Ă��Ȃ��Ȃ�
			if (pMotion->GetConnect() == false && pMotion->GetMotion() != 4 && pMotion->GetStop() == false)
			{
				// �W�����v�񐔂��J�E���g
				m_nJumpNum++;

				// �W�����v(����)���[�V�����ɂ���
				pMotion->SetMotion(4);
			}

			// ���[�V�������Ȃ��Ă��Ȃ����A�W�����v���[�V��������Ȃ����A���[�V�������~�܂��Ă���Ȃ�
			if (pMotion->GetConnect() == false && pMotion->GetMotion() != 5 && pMotion->GetStop() == true)
			{
				// �W�����v���[�V�����ɂ���
				pMotion->SetMotion(5);

				// �ړ��ʂ��W�����v�͂Ԃ���Z
				m_move.y += BOSS_JUMP_NORMAL;

				// �G�l�~�[�̔���𒅒n���Ă��Ȃ��ɐݒ�
				pEnemy->SetLand(false);
			}
		}
		// �v�l��<��W�����v>�̎�
		else if (m_think == BOSS_THINK_JUMP_BIG)
		{
			// ���[�V�������Ȃ��Ă��Ȃ����A�W�����v(����)���[�V��������Ȃ����A���[�V�������~�܂��Ă��Ȃ��Ȃ�
			if (pMotion->GetConnect() == false && pMotion->GetMotion() != 4 && pMotion->GetStop() == false)
			{
				// �W�����v�񐔂��J�E���g
				m_nJumpNum++;

				// �W�����v(����)���[�V�����ɂ���
				pMotion->SetMotion(4);
			}

			// ���[�V�������Ȃ��Ă��Ȃ����A�W�����v���[�V��������Ȃ����A���[�V�������~�܂��Ă���Ȃ�
			if (pMotion->GetConnect() == false && pMotion->GetMotion() != 5 && pMotion->GetStop() == true)
			{
				// �W�����v���[�V�����ɂ���
				pMotion->SetMotion(5);

				// �ړ��ʂ��W�����v�͂Ԃ���Z
				m_move.y += BOSS_JUMP_BIG;

				// �G�l�~�[�̔���𒅒n���Ă��Ȃ��ɐݒ�
				pEnemy->SetLand(false);
			}
		}
		
		// ���n����u��
		if (pMotion->GetConnect() == false && pMotion->GetMotion() == 5)
		{
			// �~�`�̃p�[�e�B�N���̐���
			CParticleCircle::Create(pEnemy->GetPos(), D3DXVECTOR3(100.0f, 170.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 180, 153), 20,
				20.0f, 50);

			// ���n(�{�X)SE�̍Đ�
			pSound->Play(20);
			pSound->ControllVoice(20, 0.3f);

			// �v�l��<�ʏ�W�����v>�̎�
			if (m_think == BOSS_THINK_JUMP_NORMAL)
			{
				// ���n���[�V�����ɂ���
				pMotion->SetMotion(6);

				// �v�l��<��~>�ɂ��A�~�߂鎞�Ԃ�ݒ�
				m_think = BOSS_THINK_STOP;
				m_nMoveStop = 40;

			}
			// �v�l��<��W�����v>�̎�
			else if (m_think == BOSS_THINK_JUMP_BIG)
			{
				m_nJumpNum = 0;

				// �`���[�W���[�V�����ɂ���
				pMotion->SetMotion(7);

				// �v�l��<�`���[�W�J�n>�ɂ��A�~�߂鎞�Ԃ�ݒ�
				m_think = BOSS_THINK_CHARGE_BEGIN;
				m_nDashCount = 100;
			}
		}
	}
	// �G�l�~�[�����n���Ă��Ȃ��Ȃ�
	else if (pEnemy->GetLand() == false)
	{
		// �W�����v���[�V�������A�v�l��<��W�����v>�̎�
		if (pMotion->GetMotion() == 5 && m_think == BOSS_THINK_JUMP_BIG)
		{
			if (m_moveOld.y >= 0 && m_move.y < 0)
			{
				// 5�ӏ��̈ʒu�Ƀ����_���Ɉړ�
				D3DXVECTOR3 pos = pEnemy->GetPos();
				D3DXVECTOR3 posRand[5] = { BOSS_AREA,
					D3DXVECTOR3(BOSS_AREA.x - 1000.0f, 0.0f, BOSS_AREA.z + 1000.0f),
					D3DXVECTOR3(BOSS_AREA.x + 1000.0f, 0.0f, BOSS_AREA.z + 1000.0f),
					D3DXVECTOR3(BOSS_AREA.x - 1000.0f, 0.0f, BOSS_AREA.z - 1000.0f),
					D3DXVECTOR3(BOSS_AREA.x + 1000.0f, 0.0f, BOSS_AREA.z - 1000.0f) };

				int nRandPos = rand() % 5;
				pos = D3DXVECTOR3(posRand[nRandPos].x, pos.y, posRand[nRandPos].z);

				pEnemy->SetNewPos(pos);
			}
		}
	}
}

//=============================================================================
// �ړ��̊����ɂ��Ă̏���
//=============================================================================
void CControlBoss::MoveInteria(CEnemy *pEnemy)
{
	if (m_think == BOSS_THINK_CHARGE || m_think == BOSS_THINK_DASH_END)
	{
		m_move.z *= 0.94f;
		m_move.x *= 0.94f;
	}
	else
	{
		// �����̌��Z
		m_move.z *= BOSS_INTERIA_SUBTRACTION;
		m_move.x *= BOSS_INTERIA_SUBTRACTION;
	}

	// �ړ��ʂ�����̒l�ɂȂ�����0�ɂ���
	if (m_move.x <= BOSS_MOVE_STOP_COUNT && m_move.x >= -BOSS_MOVE_STOP_COUNT)
	{
		m_move.x = 0.0f;
	}
	if (m_move.z <= BOSS_MOVE_STOP_COUNT && m_move.z >= -BOSS_MOVE_STOP_COUNT)
	{
		m_move.z = 0.0f;
	}
}

//=============================================================================
// ��]����
//=============================================================================
void CControlBoss::Rotate(CEnemy *pEnemy)
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