//=============================================================================
// �v���C���[���쏈�� [control_player.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "control_player.h"
#include "manager.h"
#include "game.h"
#include "keyboard.h"
#include "gamepad.h"
#include "mouse.h"
#include "player.h"
#include "motion.h"
#include "level_up.h"
#include "camera.h"
#include "sound.h"
#include "collision_sphere.h"
#include "model.h"
#include "arrow.h"
#include "effect_billboard.h"
#include "data_attack.h"
#include "particle_dash.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PLAYER_JUMP					(15.5f)	// �W�����v��
#define PLAYER_GRAVITY				(0.6f)	// �d�͂̑傫��
#define PLAYER_MAX_GRAVITY			(15.0f)	// �d�͂̍ő�l
#define PLAYER_MOVE_SPEED			(1.1f)	// �ړ��ʂ̊�l
#define PLAYER_INTERIA_SUBTRACTION	(0.86f)	// �n��ł̊����̌��Z�l
#define PLAYER_MOVE_STOP_COUNT		(0.02f)	// �v���C���[�̈ړ��ʂ�0�ɂ��鎞�̈ړ��ʂ̒l
#define PLAYER_DASH					(4.0f)	// �_�b�V���̊�l
#define PLAYER_DASH_TIME			(15)	// �_�b�V������
#define PLAYER_DASH_COOLTIME		(12)	// �_�b�V����̃N�[���^�C��

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CControlPlayer::CControlPlayer()
{
	// �ϐ��̃N���A
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fObjectiveRot = 0.0f;
	m_fNumRot = 0.0f;
	m_fSpeed = 0.0f;
	m_fJump = 0.0f;
	m_fAttackValue = 0.0f;
	m_bRotate = false;
	m_bJump = false;
	m_bDoubleJump = false;
	m_bDash = false;
	m_bAttack = false;
	m_bInputAttack = false;
	m_bDamage = false;
	m_nMoveCount = 0;
	m_nDashCount = 0;
	m_nDashCoolTime = 0;
	m_nAtkCount = 0;
	m_fAtkBeginEffect = 0;
	m_fAtkEffectCount = 0;
	m_pCollisionS = NULL;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CControlPlayer::~CControlPlayer()
{
	if (m_pCollisionS != NULL)
	{
		m_pCollisionS = NULL;
	}
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CControlPlayer::Init(void)
{
	// �ϐ��̏�����
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fObjectiveRot = D3DX_PI;
	m_fNumRot = 0.0f;
	m_fSpeed = PLAYER_MOVE_SPEED;
	m_fBasicSpeed = m_fSpeed;
	m_fJump = PLAYER_JUMP;
	m_bRotate = false;
	m_bJump = false;
	m_bDoubleJump = false;
	m_bDash = false;
	m_bAttack = false;
	m_bInputAttack = false;
	m_bDamage = false;
	m_nMoveCount = 0;
	m_nDashCount = 0;
	m_nDashCoolTime = PLAYER_DASH_COOLTIME;
	m_nAtkCount = 0;
	m_fAtkBeginEffect = 0;
	m_fAtkEffectCount = 0;

#ifdef _DEBUG
	// �e�L�X�g�t�@�C���̓ǂݍ���
	char cString[256];
	FILE *pFile = fopen("data/TEXT/motion_player.txt", "r");

	if (pFile != NULL)
	{
		while (1)
		{
			fscanf(pFile, "%s", &cString[0]);

			if (strcmp(&cString[0], "CHARACTERSET") == 0)	//CHARACTERSET�̕�����
			{
				while (1)
				{
					fscanf(pFile, "%s", &cString[0]);

					if (strcmp(&cString[0], "MOVE") == 0) //�ړ���
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%f", &m_fSpeed);
						m_fBasicSpeed = m_fSpeed;
					}
					if (strcmp(&cString[0], "JUMP") == 0) //�W�����v
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%f", &m_fJump);
					}
					if (strcmp(&cString[0], "END_CHARACTERSET") == 0)
					{
						break;
					}
				}
			}

			if (strcmp(&cString[0], "END_SCRIPT") == 0) // END_SCRIPT�̕�����
			{
				break;
			}
		}
		// �t�@�C�������
		fclose(pFile);
	}
#endif

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CControlPlayer::Uninit(void)
{
	if (m_pCollisionS != NULL)
	{
		m_pCollisionS = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CControlPlayer::Update(CScene *pScene)
{
	// �����̃|�C���^���v���C���[�N���X�̃|�C���^�ɃL���X�g
	CPlayer *pPlayer = NULL;
	pPlayer = (CPlayer*)pScene;

	// �L�[�{�[�h�擾����
	CKeyboard *pKeyboard = CManager::GetKeyboard();
	// �Q�[���p�b�h�擾����
	CGamePad *pGamePad = CManager::GetGamePad();

	// ���[�V�����擾����
	CMotion *pMotion = pPlayer->GetMotion();

	// �T�E���h�擾����
	CSound *pSound = CManager::GetSound();

	if (CManager::GetGame()->GetClear() == true)
	{
		// �N���A���̏���
		Clear(pPlayer);
	}

	// �ړ��ʂ�ݒ�
	m_move = pPlayer->GetMove();

	//---------------------------------------------------
	// �d��
	//---------------------------------------------------
	m_move.y -= PLAYER_GRAVITY;

	// �d�͂������Ȃ肷������d�͂̍ő�l�ŌŒ肵�Ă���
	if (m_move.y <= -PLAYER_MAX_GRAVITY)
	{
		m_move.y = -PLAYER_MAX_GRAVITY;
	}

	// ��_���[�W����
	TakeDamage(pPlayer);

	// ���S���Ă��Ȃ��Ȃ�
	if (pPlayer->GetDeath() == false)
	{
		//---------------------------------------------------
		// ��{�A�N�V����
		//---------------------------------------------------
		if (m_bDamage == false)
		{
			// �_�b�V�����Ă��Ȃ�������
			if (m_bDash == false)
			{
				// �ړ�����(�ڂ��������͊֐��̒�)
				Move(pPlayer);

				// �A�^�b�N����
				Attack(pPlayer);
			}

			// �U�����Ă��Ȃ�������
			if (m_bAttack == false)
			{
				// �_�b�V������
				Dash(pPlayer);

#ifdef _DEBUG
				// �W�����v����
				//Jump(pPlayer);
#endif
			}

			// ����؂�ւ�����
			WeaponChange(pPlayer);
		}

		// �U�����[�V��������Ȃ��ꍇ�A�U�����Ă��Ȃ���Ԃɐݒ�
		if (pMotion->GetMotion() != 5 &&
			pMotion->GetMotion() != 6 &&
			pMotion->GetMotion() != 7 &&
			pMotion->GetMotion() != 8 &&
			pMotion->GetConnect() == false)
		{
			m_bAttack = false;
		}

		//---------------------------------------------------
		// ���[�V�����J��
		//---------------------------------------------------
		// ���ݓ����Ă��炸�A���[�V�������Ȃ��Ă��Ȃ����A�����ړ����[�V�������A�_�b�V�����[�V��������Ȃ��Ȃ�
		if (m_move.x == 0.0f && m_move.z == 0.0f && pMotion->GetConnect() == false &&
			pMotion->GetMotion() == 1)
		{
			// �j���[�g�������[�V�����ɂ���
			pMotion->SetMotion(0);
		}

		// ���ݓ����Ă��邩�A���[�V�������Ȃ��Ă��Ȃ����A�j���[�g�������[�V������������
		if ((m_move.x != 0.0f || m_move.z != 0.0f) && pMotion->GetConnect() == false &&
			pMotion->GetMotion() == 0)
		{
			// �����̈ړ��ʂňړ����[�V�����ɑJ�ڂ��邽�߁A�ړ��̓��͂������ɉ�����
			if ((pKeyboard->GetPress(DIK_W) == true || pGamePad->GetPress(CGamePad::PAD_INPUT_LSTICK_UP, 0) == true) ||
				(pKeyboard->GetPress(DIK_A) == true || pGamePad->GetPress(CGamePad::PAD_INPUT_LSTICK_LEFT, 0) == true) ||
				(pKeyboard->GetPress(DIK_S) == true || pGamePad->GetPress(CGamePad::PAD_INPUT_LSTICK_DOWN, 0) == true) ||
				(pKeyboard->GetPress(DIK_D) == true || pGamePad->GetPress(CGamePad::PAD_INPUT_LSTICK_RIGHT, 0) == true))
			{
				// �ړ����[�V�����ɂ���
				pMotion->SetMotion(1);
			}
		}

		// �_�b�V�����[�V�������A���[�V�������Ȃ��Ă��Ȃ����A�_�b�V�����Ă��Ȃ��Ȃ�
		if (pMotion->GetMotion() == 4 && pMotion->GetConnect() == false && m_bDash == false)
		{
			if (pPlayer->GetLand() == false)
			{
				// �W�����v���[�V�����ɂ���
				pMotion->SetMotion(2);
			}
			// �ړ��̓��͒��Ȃ�
			else if ((pKeyboard->GetPress(DIK_W) == true || pGamePad->GetPress(CGamePad::PAD_INPUT_LSTICK_UP, 0) == true) ||
				(pKeyboard->GetPress(DIK_A) == true || pGamePad->GetPress(CGamePad::PAD_INPUT_LSTICK_LEFT, 0) == true) ||
				(pKeyboard->GetPress(DIK_S) == true || pGamePad->GetPress(CGamePad::PAD_INPUT_LSTICK_DOWN, 0) == true) ||
				(pKeyboard->GetPress(DIK_D) == true || pGamePad->GetPress(CGamePad::PAD_INPUT_LSTICK_RIGHT, 0) == true))
			{
				// �ړ����[�V�����ɂ���
				pMotion->SetMotion(1);
			}
			else
			{
				// �j���[�g�������[�V�����ɂ���
				pMotion->SetMotion(0);
			}
		}
	}

	//---------------------------------------------------
	// ����
	//---------------------------------------------------
	// �ړ��̊���(�ڂ��������͊֐��̒�)
	MoveInteria(pPlayer);

	// ��]�̊���(�ڂ��������͊֐��̒�)
	Rotate(pPlayer);

	// �U�����̃R���W����
	DoingAttack(pPlayer);

	//---------------------------------------------------
	// ����
	//---------------------------------------------------
	// �ړ����[�V�������A���n���Ă���Ȃ�
	if (pMotion->GetMotion() == 1 && pPlayer->GetLand() == true)
	{
		if (m_nMoveCount % 40 == 0)
		{
			// ����(2)SE�̍Đ�
			pSound->Play(5);
			pSound->ControllVoice(5, 0.3f);
		}
		else
		{
			if (m_nMoveCount % 20 == 0)
			{
				// ����(1)SE�̍Đ�
				pSound->Play(6);
				pSound->ControllVoice(6, 0.3f);
			}
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
CControlPlayer *CControlPlayer::Create(void)
{
	// �N���X�̐���
	CControlPlayer *pControlPlayer = NULL;
	pControlPlayer = new CControlPlayer;

	// �k���`�F�b�N
	if (pControlPlayer != NULL)
	{
		// ����������
		pControlPlayer->Init();
	}
	return pControlPlayer;
}

//=============================================================================
// �ړ�����
//=============================================================================
void CControlPlayer::Move(CPlayer *pPlayer)
{
	// �L�[�{�[�h�擾����
	CKeyboard *pKeyboard = CManager::GetKeyboard();
	// �Q�[���p�b�h�擾����
	CGamePad *pGamePad = CManager::GetGamePad();

	// ���[�V�����擾����
	CMotion *pMotion = pPlayer->GetMotion();

	// �J�����̌����擾
	CCamera *pCamera = CManager::GetCamera();
	D3DXVECTOR3 rotCamera = pCamera->GetRotV();

	//---------------------------------------------------
	// �ړ����x�̐ݒ�
	//---------------------------------------------------
	// �U�����Ă��Ȃ�������
	if (m_bAttack == false)
	{
		// �ړ��͊�{�̑��x��
		m_fSpeed = m_fBasicSpeed;
	}
	// �{�E�K���ȊO�̍U�����Ȃ�
	else if (m_bAttack == true && pMotion->GetMotion() != 8)
	{
		// �ړ��X�s�[�h��x������
		m_fSpeed = m_fBasicSpeed / 3.0f;
	}
	// �{�E�K���ōU�����Ȃ�
	else if (m_bAttack == true && pMotion->GetMotion() == 8)
	{
		// �ړ��ł��Ȃ�����
		m_fSpeed = 0.0f;
	}

	// ���x�␳�l�ɂ���đ��x��ς���
	float fSpeed = m_fSpeed * (1.0f + CLevelUp::GetCorrectionSpd() * 0.3f);

	// ����\�Ȃ�
	if (pPlayer->GetCanControl() == true)
	{
		//***********************************************************************
		// �ړ� (�L�[�{�[�h�v/�`/�r/�c �܂��� �p�b�h���X�e�B�b�N)
		//***********************************************************************
		//���ړ�
		if (pKeyboard->GetPress(DIK_A) == true ||
			pGamePad->GetPress(CGamePad::PAD_INPUT_LSTICK_LEFT, 0) == true)
		{
			//�����ړ�
			if (pKeyboard->GetPress(DIK_W) == true ||
				pGamePad->GetPress(CGamePad::PAD_INPUT_LSTICK_UP, 0) == true)
			{
				//�ړ��ʉ��Z
				m_move.x += +cosf(rotCamera.y + D3DX_PI / 4.0f) * fSpeed;
				m_move.z += -sinf(rotCamera.y + D3DX_PI / 4.0f) * fSpeed;
				//�ړI�̌�����ݒ�
				m_fObjectiveRot = rotCamera.y - D3DX_PI / 4.0f;
				//��]�̊������I���ɂ���
				m_bRotate = true;
			}
			//����O�ړ�
			else if (pKeyboard->GetPress(DIK_S) == true ||
				pGamePad->GetPress(CGamePad::PAD_INPUT_LSTICK_DOWN, 0) == true)
			{
				//�ړ��ʉ��Z
				m_move.x += +cosf(rotCamera.y - D3DX_PI / 4.0f) * fSpeed;
				m_move.z += -sinf(rotCamera.y - D3DX_PI / 4.0f) * fSpeed;
				//�ړI�̌�����ݒ�
				m_fObjectiveRot = rotCamera.y - (D3DX_PI / 4.0f) * 3.0f;
				//��]�̊������I���ɂ���
				m_bRotate = true;
			}
			else
			{
				//�ړ��ʉ��Z
				m_move.x += +cosf(rotCamera.y) * fSpeed;
				m_move.z += -sinf(rotCamera.y) * fSpeed;
				//�ړI�̌�����ݒ�
				m_fObjectiveRot = rotCamera.y - D3DX_PI / 2.0f;
				//��]�̊������I���ɂ���
				m_bRotate = true;
			}
		}
		//�E�ړ�
		else if (pKeyboard->GetPress(DIK_D) == true ||
			pGamePad->GetPress(CGamePad::PAD_INPUT_LSTICK_RIGHT, 0) == true)
		{
			//�E���ړ�
			if (pKeyboard->GetPress(DIK_W) == true ||
				pGamePad->GetPress(CGamePad::PAD_INPUT_LSTICK_UP, 0) == true)
			{
				//�ړ��ʉ��Z
				m_move.x += -cosf(rotCamera.y - D3DX_PI / 4.0f) * fSpeed;
				m_move.z += +sinf(rotCamera.y - D3DX_PI / 4.0f) * fSpeed;
				//�ړI�̌�����ݒ�
				m_fObjectiveRot = rotCamera.y + D3DX_PI / 4.0f;
				//��]�̊������I���ɂ���
				m_bRotate = true;
			}
			//�E��O�ړ�
			else if (pKeyboard->GetPress(DIK_S) == true ||
				pGamePad->GetTrigger(CGamePad::PAD_INPUT_LSTICK_DOWN, 0) == true)
			{
				//�ړ��ʉ��Z
				m_move.x += -cosf(rotCamera.y + D3DX_PI / 4.0f) * fSpeed;
				m_move.z += +sinf(rotCamera.y + D3DX_PI / 4.0f) * fSpeed;
				//�ړI�̌�����ݒ�
				m_fObjectiveRot = rotCamera.y + D3DX_PI / 1.5f;
				//��]�̊������I���ɂ���
				m_bRotate = true;
			}
			else
			{
				//�ړ��ʉ��Z
				m_move.x += -cosf(rotCamera.y) * fSpeed;
				m_move.z += +sinf(rotCamera.y) * fSpeed;
				//�ړI�̌�����ݒ�
				m_fObjectiveRot = rotCamera.y + D3DX_PI / 2.0f;
				//��]�̊������I���ɂ���
				m_bRotate = true;
			}
		}
		//���ړ�
		else if (pKeyboard->GetPress(DIK_W) == true ||
			pGamePad->GetPress(CGamePad::PAD_INPUT_LSTICK_UP, 0) == true)
		{
			//�ړ��ʉ��Z
			m_move.x += -sinf(rotCamera.y) * fSpeed;
			m_move.z += -cosf(rotCamera.y) * fSpeed;
			//�ړI�̌�����ݒ�
			m_fObjectiveRot = rotCamera.y;
			//��]�̊������I���ɂ���
			m_bRotate = true;
		}
		//��O�ړ�
		else if (pKeyboard->GetPress(DIK_S) == true ||
			pGamePad->GetPress(CGamePad::PAD_INPUT_LSTICK_DOWN, 0) == true)
		{
			//�ړ��ʉ��Z
			m_move.x += +sinf(rotCamera.y) * fSpeed;
			m_move.z += +cosf(rotCamera.y) * fSpeed;
			//�ړI�̌�����ݒ�
			m_fObjectiveRot = rotCamera.y + D3DX_PI;
			//��]�̊������I���ɂ���
			m_bRotate = true;
		}
	}
}

//=============================================================================
// �_�b�V������
//=============================================================================
void CControlPlayer::Dash(CPlayer *pPlayer)
{
	// �L�[�{�[�h�擾����
	CKeyboard *pKeyboard = CManager::GetKeyboard();
	// �Q�[���p�b�h�擾����
	CGamePad *pGamePad = CManager::GetGamePad();

	// ���[�V�����擾����
	CMotion *pMotion = pPlayer->GetMotion();

	// �R���W�����擾����
	CCollisionSphere *pSphere = pPlayer->GetCollision();

	// �T�E���h�擾����
	CSound *pSound = CManager::GetSound();

	// �_�b�V��������Ȃ��Ȃ�
	if (m_bDash == false)
	{
		// �N�[���^�C���̃J�E���g�𑝂₷
		m_nDashCoolTime++;

		// �N�[���^�C�����߂��Ă���Ȃ�
		if (m_nDashCoolTime >= PLAYER_DASH_COOLTIME)
		{
			// ����\�Ȃ�
			if (pPlayer->GetCanControl() == true)
			{
				//***********************************************************************
				// �_�b�V�� (�L�[�{�[�hShift �܂��� �p�b�hA�{�^��)
				//***********************************************************************
				if (pKeyboard->GetTrigger(DIK_LSHIFT) == true ||
					pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_A, 0) == true)
				{
					// �_�b�V�����Ă����Ԃɐݒ�
					m_bDash = true;

					// �v���C���[�̌�����ړI�̌����ɐݒ�
					pPlayer->SetRot(D3DXVECTOR3(0.0f, m_fObjectiveRot, 0.0f));

					// �_�b�V��SE�̍Đ�
					pSound->Play(4);
					pSound->ControllVoice(4, 0.5f);

					// �_�b�V�����[�V�����ɂ���
					pMotion->SetMotion(4);

					// �_�b�V���̃p�[�e�B�N���𔭐�������
					D3DXVECTOR3 pos = pPlayer->GetPos();
					D3DXVECTOR3 size = pPlayer->GetSize();
					D3DXVECTOR3 rot = pPlayer->GetRot();
					pos.y += size.y / 2.0f;
					CParticleDash::Create(pos, D3DXVECTOR3(50.0f, 50.0f, 0.0f), rot, D3DCOLOR_RGBA(150, 220, 255, 153),
						5, 10.0f, 70.0f, 15.0f, 15);
				}
			}
		}
	}

	// �_�b�V����
	if (m_bDash == true)
	{
		// �N�[���^�C����i�܂��Ȃ��悤�ɂ���
		m_nDashCoolTime = 0;

		// �R���W�����̔��������
		pSphere->SetCollisionType(CCollisionSphere::COLLISION_S_TYPE_NONE);

		// �J�E���g�𑝂₷
		m_nDashCount++;

		if (m_nDashCount <= PLAYER_DASH_TIME)
		{
			// �v���C���[�̌������擾���A���i������
			D3DXVECTOR3 rot = pPlayer->GetRot();
			m_move.x += -sinf(rot.y) * PLAYER_DASH;
			m_move.z += -cosf(rot.y) * PLAYER_DASH;
		}
		else if (m_nDashCount > PLAYER_DASH_TIME)
		{
			pSphere->SetCollisionType(CCollisionSphere::COLLISION_S_TYPE_PLAYER);
			m_bDash = false;
			m_nDashCount = 0;
		}
	}
}

//=============================================================================
// �U������
//=============================================================================
void CControlPlayer::Attack(CPlayer *pPlayer)
{
	// �L�[�{�[�h�擾����
	CKeyboard *pKeyboard = CManager::GetKeyboard();
	// �Q�[���p�b�h�擾����
	CGamePad *pGamePad = CManager::GetGamePad();
	// �}�E�X�擾����
	CMouse *pMouse = CManager::GetMouse();

	// ���[�V�����擾����
	CMotion *pMotion = pPlayer->GetMotion();

	// �T�E���h�擾����
	CSound *pSound = CManager::GetSound();

	// �U�����擾����
	CDataAttack *pDataAtk = CManager::GetDataAtk();

	// ����\�Ȃ�
	if (pPlayer->GetCanControl() == true)
	{
		//---------------------------------------------------
		// ���𕐊�ɂ��Ă��鎞
		//---------------------------------------------------
		if (pPlayer->GetWeapon() == CPlayer::PLAYER_WEAPON_AXE)
		{
			//***********************************************************************
			// �U�� (�}�E�X���N���b�N �܂��� �p�b�hX�{�^��)
			//***********************************************************************
			if (pMouse->GetTrigger(CMouse::MOUSE_INPUT_LEFT) == true ||
				pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_X, 0) == true)
			{
				//***********************************************************************
				// �U��3�i��
				//***********************************************************************
				// �U�������A���U��(2�i��)�̃��[�V�������Ȃ�
				if (m_bAttack == true && pMotion->GetMotion() == 6)
				{
					// �U���̓��͂��Ă����Ԃɂ���
					m_bInputAttack = true;
				}

				//***********************************************************************
				// �U��2�i��
				//***********************************************************************
				// �U�������A���U��(1�i��)�̃��[�V�������Ȃ�
				if (m_bAttack == true && pMotion->GetMotion() == 5)
				{
					// �U���̓��͂��Ă����Ԃɂ���
					m_bInputAttack = true;
				}

				//***********************************************************************
				// �U��1�i��
				//***********************************************************************
				// �܂��U�����Ă��Ȃ����A���[�V�������Ȃ��Ă��Ȃ�������
				if (m_bAttack == false && pMotion->GetConnect() == false)
				{
					m_bAttack = true;

					// ���U��(1�i��)�̃��[�V�����ɂ���
					pMotion->SetMotion(5);

					// ���̃X�C���OSE�̍Đ�
					pSound->Play(7);
					pSound->ControllVoice(7, 0.4f);

					// �J�E���^�[�̃��Z�b�g
					m_nAtkCount = 0;
				}
			}
			
			// �U�������A���U��(1�i��)�̃��[�V�������Ȃ�
			if (m_bAttack == true && pMotion->GetMotion() == 5)
			{
				if (m_bInputAttack == true && m_nAtkCount >= pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_AXE_2, CDataAttack::ATTACK_DATA_TYPE_WAIT))
				{
					// ���U��(2�i��)�̃��[�V�����ɂ���
					pMotion->SetMotion(6);

					// ���̃X�C���OSE�̍Đ�
					pSound->Play(7);
					pSound->ControllVoice(7, 0.4f);

					// �J�E���^�[�̃��Z�b�g
					m_nAtkCount = 0;

					// �U���̓��͂̏�Ԃ�߂�
					m_bInputAttack = false;
				}
			}
			
			// �U�������A���U��(2�i��)�̃��[�V�������Ȃ�
			if (m_bAttack == true && pMotion->GetMotion() == 6)
			{
				if (m_bInputAttack == true && m_nAtkCount >= pDataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_AXE_3, CDataAttack::ATTACK_DATA_TYPE_WAIT))
				{
					// ���U��(3�i��)�̃��[�V�����ɂ���
					pMotion->SetMotion(7);

					// ���̃X�C���OSE�̍Đ�
					pSound->Play(7);
					pSound->ControllVoice(7, 0.4f);

					// �J�E���^�[�̃��Z�b�g
					m_nAtkCount = 0;

					// �U���̓��͂̏�Ԃ�߂�
					m_bInputAttack = false;
				}
			}
		}
		//---------------------------------------------------
		// ������{�E�K���ɂ��Ă��鎞
		//---------------------------------------------------
		else if (pPlayer->GetWeapon() == CPlayer::PLAYER_WEAPON_BOWGUN)
		{
			//***********************************************************************
			// �U�� (�}�E�X���N���b�N �܂��� �p�b�hX�{�^��)
			//***********************************************************************
			if (pMouse->GetTrigger(CMouse::MOUSE_INPUT_LEFT) == true ||
				pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_X, 0) == true)
			{
				// �܂��U�����Ă��Ȃ����A���[�V�������Ȃ��Ă��Ȃ�������
				if (m_bAttack == false && pMotion->GetConnect() == false)
				{
					m_bAttack = true;

					// �{�E�K���U���̃��[�V�����ɂ���
					pMotion->SetMotion(8);

					// �J�E���^�[�̃��Z�b�g
					m_nAtkCount = 0;
				}
			}
		}
	}
}

//=============================================================================
// �U�����̏���
//=============================================================================
void CControlPlayer::DoingAttack(CPlayer *pPlayer)
{
	// ���[�V�����擾����
	CMotion *pMotion = pPlayer->GetMotion();

	// �U�����擾����
	CDataAttack *pAtkData = CManager::GetDataAtk();
	CDataAttack::ATTACK_TYPE atkType = CDataAttack::ATTACK_TYPE_AXE;
	CCollisionSphere::COLLISION_S_TYPE collisionType = CCollisionSphere::COLLISION_S_TYPE_WEAPON;

	// �U���␳�l�ɂ���Ċ�{�̍U���͂�ς���
	m_fAttackValue = (1.0f + CLevelUp::GetCorrectionAtk() * 0.5f);

	// ���U��(1�i��)�̎�
	if (pMotion->GetMotion() == 5)
	{
		atkType = CDataAttack::ATTACK_TYPE_AXE;
	}
	// ���U��(2�i��)�̎�
	else if (pMotion->GetMotion() == 6)
	{
		atkType = CDataAttack::ATTACK_TYPE_AXE_2;
	}
	// ���U��(3�i��)�̎�
	else if (pMotion->GetMotion() == 7)
	{
		atkType = CDataAttack::ATTACK_TYPE_AXE_3;
		collisionType = CCollisionSphere::COLLISION_S_TYPE_WEAPON_P;
	}

	// ���炩�̕��U��
	if ((pMotion->GetMotion() == 5 && pMotion->GetNextMotion() == 5) ||
		(pMotion->GetMotion() == 6 && pMotion->GetNextMotion() == 6) ||
		pMotion->GetMotion() == 7 && pMotion->GetNextMotion() == 7)
	{
		// �G�t�F�N�g�̊J�n�t���[����ݒ�
		m_fAtkBeginEffect = pAtkData->GetAttackData(atkType, CDataAttack::ATTACK_DATA_TYPE_BEGIN_EFFECT);

		// �G�t�F�N�g�̎������Ԃ�ݒ�
		m_fAtkEffectCount = pAtkData->GetAttackData(atkType, CDataAttack::ATTACK_DATA_TYPE_TIME_EFFECT);

		// ����̃t���[���ɂȂ�����R���W����������
		if (m_nAtkCount == pAtkData->GetAttackData(atkType, CDataAttack::ATTACK_DATA_TYPE_BEGIN_COLLISION))
		{
			// �v���C���[�̕���ɂ����郂�f���̈ʒu�E�����擾
			D3DXVECTOR3 modelPos = pPlayer->GetModelPos(15);
			CModel *pModel = pPlayer->GetModel(15);

			// �E��̕���ɃR���W����(���픻��)�����A�e�q�֌W����������
			m_pCollisionS = CCollisionSphere::Create(D3DXVECTOR3(modelPos.x, modelPos.y, modelPos.z - 50.0f), 70.0f, collisionType, pAtkData->GetAttackData(atkType, CDataAttack::ATTACK_DATA_TYPE_TIME_COLLISION));
			m_pCollisionS->SetParent(pModel);

			// �_���[�W�ʁ~�{���ōU���͂�ݒ�
			m_pCollisionS->SetDamageValue(m_fAttackValue * pAtkData->GetAttackData(atkType, CDataAttack::ATTACK_DATA_TYPE_DAMAGE));
		}
	}

	// �{�E�K���U���̎�
	if (pMotion->GetMotion() == 8)
	{
		// ����̃t���[���ɂȂ�����R���W����������
		if (m_nAtkCount == pAtkData->GetAttackData(CDataAttack::ATTACK_TYPE_BOWGUN, CDataAttack::ATTACK_DATA_TYPE_BEGIN_COLLISION))
		{
			// �v���C���[�̕���ɂ����郂�f���̈ʒu�A�v���C���[�̌������擾
			CModel *pModel = pPlayer->GetModel(15);
			D3DXVECTOR3 pos = pModel->GetMtxPos();
			D3DXVECTOR3 rot = pPlayer->GetRot();

			// ��𐶐�
			CArrow::Create(pos, rot, m_fAttackValue);
		}
	}

	// �U�������A���̃��[�V���������݂̃��[�V�����Ɠ����Ȃ�
	if (m_bAttack == true && pMotion->GetNextMotion() == pMotion->GetMotion())
	{
		// �{�E�K���̃��[�V����������Ȃ�������
		if (pMotion->GetMotion() != 8)
		{
			// ����̃t���[���ɂȂ������A�������Ԓ��Ȃ�
			if (m_nAtkCount >= m_fAtkBeginEffect && m_nAtkCount <= m_fAtkBeginEffect + m_fAtkEffectCount)
			{
				D3DXVECTOR3 modelPos = pPlayer->GetModelPos(15);
				CModel *pModel = pPlayer->GetModel(15);

				// �O�ՂɃG�t�F�N�g������
				CEffectB::Create(D3DXVECTOR3(modelPos.x, modelPos.y, modelPos.z - 40.0f), D3DXVECTOR3(30.0f, 30.0f, 0.0f),
					D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 35, CEffectB::EFFECT_TYPE_SMALL, pModel);
			}
		}

		// �J�E���g�𑝂₷
		m_nAtkCount++;
	}
}

//=============================================================================
// ����؂�ւ�����
//=============================================================================
void CControlPlayer::WeaponChange(CPlayer *pPlayer)
{
	// �Q�[���p�b�h�擾����
	CGamePad *pGamePad = CManager::GetGamePad();
	// �}�E�X�擾����
	CMouse *pMouse = CManager::GetMouse();

	// ���[�V�����擾����
	CMotion *pMotion = pPlayer->GetMotion();

	// �T�E���h�擾����
	CSound *pSound = CManager::GetSound();

	// ����\�Ȃ�
	if (pPlayer->GetCanControl() == true)
	{
		//***********************************************************************
		// ����`�F���W (�}�E�X�E�N���b�N �܂��� �p�b�hR�g���K�[)
		//***********************************************************************
		if (pMouse->GetTrigger(CMouse::MOUSE_INPUT_RIGHT) == true ||
			pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_RIGHT_SHOULDER, 0) == true)
		{
			// �U��������Ȃ����A���[�V�������Ȃ��Ă��Ȃ�������
			if (m_bAttack == false && pMotion->GetConnect() == false)
			{
				// �؂�ւ����̏�Ԃ�
				pPlayer->SetWeaponChange(true);

				// ����؂�ւ�SE�̍Đ�
				pSound->Play(11);
				pSound->ControllVoice(11, 0.8f);

				if (pPlayer->GetWeapon() == CPlayer::PLAYER_WEAPON_AXE)
				{
					pPlayer->SetWeapon(CPlayer::PLAYER_WEAPON_BOWGUN);
				}
				else if (pPlayer->GetWeapon() == CPlayer::PLAYER_WEAPON_BOWGUN)
				{
					pPlayer->SetWeapon(CPlayer::PLAYER_WEAPON_AXE);
				}
			}
		}
	}
}

//=============================================================================
// ��_���[�W����
//=============================================================================
void CControlPlayer::TakeDamage(CPlayer *pPlayer)
{
	// ���[�V�����擾����
	CMotion *pMotion = pPlayer->GetMotion();

	// �v���C���[�̏�Ԃ�<�������>�ɂȂ������A�_���[�W���󂯂Ă��Ȃ�������
	if (pPlayer->GetState() == CPlayer::PLAYER_STATE_BLOWAWAY && m_bDamage == false)
	{
		// Y�����ւ̈ړ��ʂ����Z�b�g���A�W�����v������
		m_move.y = 0.0f;
		m_move.y += 6.0f;

		// �_�b�V���ƍU����Ԃ̉���
		m_bDash = false;
		m_nDashCount = 0;
		m_bAttack = false;
		m_nAtkCount = 0;
		m_bInputAttack = false;

		// �U�����ɔ������Ă����R���W�����͏���
		if (m_pCollisionS != NULL)
		{
			// �U�����̔��������
			m_pCollisionS = NULL;
		}

		// �_���[�W���󂯂���Ԃɂ��A���n���Ă��Ȃ���Ԃɂ���
		m_bDamage = true;
		pPlayer->SetLand(false);

		// �ړI�̌�����ݒ�
		D3DXVECTOR3 rot = pPlayer->GetRot();
		m_fObjectiveRot = rot.y;

		// ��_���[�W���[�V�����ɂ���
		pMotion->SetMotion(9);
	}

	// �_���[�W��
	if (m_bDamage == true)
	{
		// ���n���Ă��Ȃ��Ȃ�
		if (pPlayer->GetLand() == false)
		{
			// �v���C���[�̐��ʂ���t�����֌�ނ�����
			D3DXVECTOR3 rot = pPlayer->GetRot();
			m_move.x = sinf(rot.y) * 10.0f;
			m_move.z = cosf(rot.y) * 10.0f;
		}
		// ���n���Ă���Ȃ�
		else if (pPlayer->GetLand() == true)
		{
			// ���[�V�������Ȃ��Ă��Ȃ����A���C�t��0�ɂȂ�����
			if (pMotion->GetMotion() != 10 && pMotion->GetConnect() == false && pPlayer->GetLife() <= 0)
			{
				// �_�E�����[�V�����ɂ���
				pMotion->SetMotion(10);
			}
			// ���C�t���܂��c���Ă���Ȃ�
			else if(pPlayer->GetLife() > 0)
			{
				// �j���[�g�������[�V�����ɂ���
				pMotion->SetMotion(0);

				// �_���[�W��Ԃ�߂�
				m_bDamage = false;

				// �v���C���[�̏�Ԃ�<�ʏ�>�ɖ߂�
				pPlayer->SetState(CPlayer::PLAYER_STATE_NORMAL);
			}
		}
	}
}

//=============================================================================
// �ړ��̊����ɂ��Ă̏���
//=============================================================================
void CControlPlayer::MoveInteria(CPlayer *pPlayer)
{
	// �����̌��Z
	m_move.z *= PLAYER_INTERIA_SUBTRACTION;
	m_move.x *= PLAYER_INTERIA_SUBTRACTION;

	// �ړ��ʂ�����̒l�ɂȂ�����0�ɂ���
	if (m_move.x <= PLAYER_MOVE_STOP_COUNT && m_move.x >= -PLAYER_MOVE_STOP_COUNT)
	{
		m_move.x = 0.0f;
	}
	if (m_move.z <= PLAYER_MOVE_STOP_COUNT && m_move.z >= -PLAYER_MOVE_STOP_COUNT)
	{
		m_move.z = 0.0f;
	}
}

//=============================================================================
// ��]����
//=============================================================================
void CControlPlayer::Rotate(CPlayer *pPlayer)
{
	// ���[�V�����擾����
	CMotion *pMotion = pPlayer->GetMotion();

	// �v���C���[�̌������擾
	D3DXVECTOR3 rotPlayer = pPlayer->GetRot();

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

		// �U�����Ă��Ȃ��ꍇ���A�{�E�K���ōU�����Ă���ꍇ
		if (m_bAttack == false ||
			m_bAttack == true && pMotion->GetMotion() == 8)
		{
			// �v���C���[�̌����ɉ��Z
			rotPlayer.y += m_fNumRot * 0.2f;
		}
		// ���Ŕh���U�����̏ꍇ
		else if (pMotion->GetMotion() == 6 || pMotion->GetMotion() == 7)
		{
			// �v���C���[�̌����ɉ��Z���A��]���~�߂�
			rotPlayer.y += m_fNumRot * 0.025f;
			m_bRotate = false;
		}

		// ����̒l�ɒB�������]����߂�
		if (rotPlayer.y - m_fObjectiveRot < 0.001f && rotPlayer.y - m_fObjectiveRot > -0.001f)
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
	pPlayer->SetRot(rotPlayer);
}

//=============================================================================
// �N���A���̏���
//=============================================================================
void CControlPlayer::Clear(CPlayer *pPlayer)
{
	// ���[�V�����擾����
	CMotion *pMotion = pPlayer->GetMotion();

	if (pPlayer->GetLand() == true)
	{
		// ���[�V�������Ȃ��Ă��Ȃ����A�N���A�֘A�̃��[�V��������Ȃ�������
		if (pMotion->GetConnect() == false && (pMotion->GetMotion() != 11 && pMotion->GetMotion() != 12))
		{
			// �N���A���[�V�����ɂ���
			pMotion->SetMotion(11);

			// �J�E���^�[���Ԃ̃��Z�b�g
			m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_nMoveCount = 0;
			m_nDashCount = 0;
			m_nAtkCount = 0;
			m_bDash = false;
			m_bAttack = false;
			m_bDamage = false;
		}

		// �J�����̎擾
		CCamera *pCamera = CManager::GetCamera();
		D3DXVECTOR3 rotCamera = pCamera->GetRotV();

		//�ړI�̌�����ݒ�
		rotCamera.y += D3DX_PI;
		m_fObjectiveRot = rotCamera.y;
		pPlayer->SetRot(rotCamera);
	}
}

#ifdef _DEBUG
//=============================================================================
// �W�����v���� (�f�o�b�O�p�Ƃ��Ďg�p)
//=============================================================================
void CControlPlayer::Jump(CPlayer *pPlayer)
{
	// �L�[�{�[�h�擾����
	CKeyboard *pKeyboard = CManager::GetKeyboard();
	// �Q�[���p�b�h�擾����
	CGamePad *pGamePad = CManager::GetGamePad();

	// ���[�V�����擾����
	CMotion *pMotion = pPlayer->GetMotion();

	// �T�E���h�擾����
	CSound *pSound = CManager::GetSound();

	// ����\�Ȃ�
	if (pPlayer->GetCanControl() == true)
	{
		// �v���C���[�����n���Ă��Ȃ����A2�i�W�����v���܂����Ă��Ȃ��Ȃ�
		if (pPlayer->GetLand() == false && m_bDoubleJump == false)
		{
			//***********************************************************************
			// 2�i�W�����v
			//***********************************************************************
			if (pKeyboard->GetTrigger(DIK_SPACE) == true ||
				pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_A, 0) == true)
			{
				// �W�����vSE�̍Đ�
				pSound->Play(3);
				pSound->ControllVoice(3, 0.8f);

				// �ʒu���擾
				D3DXVECTOR3 pos = pPlayer->GetPos();

				// Y�����̈ړ��ʂ�0��
				m_move.y = 0.0f;

				// �W�����v���[�V�����ɂ���
				pMotion->SetMotion(2);

				// �ړ��ʂ��W�����v�͂Ԃ���Z
				m_move.y += m_fJump;

				m_bDoubleJump = true;
			}
		}

		// �v���C���[�����n���Ă���Ȃ�
		if (pPlayer->GetLand() == true)
		{
			//***********************************************************************
			// �W�����v
			//***********************************************************************
			if (pKeyboard->GetTrigger(DIK_SPACE) == true ||
				pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_A, 0) == true)
			{
				// �W�����vSE�̍Đ�
				pSound->Play(3);
				pSound->ControllVoice(3, 0.8f);

				// �W�����v���[�V�����ɂ���
				pMotion->SetMotion(2);

				// �ړ��ʂ��W�����v�͂Ԃ���Z
				m_move.y += m_fJump;

				// �v���C���[�̔�����W�����v�����A���n���Ă��Ȃ��ɐݒ�
				pPlayer->SetJump(true);
				pPlayer->SetLand(false);

				m_bJump = true;
			}
			else
			{
				// ���n������܂��W�����v���\�ɂ���
				m_bJump = false;
				m_bDoubleJump = false;
			}
		}
	}
}
#endif