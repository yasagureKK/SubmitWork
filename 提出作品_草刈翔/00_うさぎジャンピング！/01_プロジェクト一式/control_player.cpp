//=============================================================================
// �v���C���[���쏈�� [control_player.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "control_player.h"
#include "manager.h"
#include "input_keyboard.h"
#include "input_pad.h"
#include "player.h"
#include "motion_player.h"
#include "camera.h"
#include "tutorial.h"
#include "sound.h"
#include "particle_circle.h"
#include "particle_wind.h"
#include "billboard_smoke.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CControlPlayer::CControlPlayer()
{
	// �ϐ��̃N���A
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_moveOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fObjectiveRot = 0.0f;
	m_fNumRot = 0.0f;
	m_fSpeed = 0.0f;
	m_fFloat = 0.0f;
	m_bRotate = false;
	m_bJump = false;
	m_bDoubleJump = false;
	m_nSpinCount = 0;
	m_nMoveCount = 0;
	m_nextInput = PLAYER_NEXT_INPUT_NONE;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CControlPlayer::~CControlPlayer()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CControlPlayer::Init(void)
{
	// �ϐ��̏�����
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_moveOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fObjectiveRot = 0.0f;
	m_fNumRot = 0.0f;
	m_fSpeed = PLAYER_MOVE_SPEED;
	m_fFloat = 0.0f;
	m_bRotate = false;
	m_bJump = false;
	m_bDoubleJump = false;
	m_nSpinCount = 0;
	m_nMoveCount = 0;
	m_nextInput = PLAYER_NEXT_INPUT_NONE;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CControlPlayer::Uninit(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void CControlPlayer::Update(CScene *pScene)
{
	// �����̃|�C���^���v���C���[�N���X�̃|�C���^�ɃL���X�g
	CPlayer *pPlayer = NULL;
	pPlayer = (CPlayer*)pScene;

	// ���[�V�����擾����
	CMotionPlayer *pMotionPlayer = NULL;
	pMotionPlayer = pPlayer->GetMotionPlayer();

	// �T�E���h�擾����
	CSound *pSound;
	pSound = CManager::GetSound();

	D3DXVECTOR3 rotCamera = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �J�����̃|�C���^�̃A�h���X�擾
	CCamera **pCameraAddress = CManager::GetCamera();

	// �J�����̍ő吔�Ԃ�A�|�C���^�z���i�߂�
	for (int nCntCamera = 0; nCntCamera < MAX_CAMERA; nCntCamera++, pCameraAddress++)
	{
		// �J�����̎擾
		CCamera *pCamera = &**pCameraAddress;

		if (pCamera != NULL)
		{
			// �v���C���[�̔ԍ��ƃJ�����̔ԍ���������������
			if (pPlayer->GetNum() == pCamera->GetNum())
			{
				// �J�����̌����擾
				rotCamera = pCamera->GetRotV();
			}
		}
	}

	// �ړ��ʂ�ݒ�
	m_move = pPlayer->GetMove();
	m_moveOld = m_move;

	// �d��
	m_move.y -= (PLAYER_GRAVITY - m_fFloat);

	// 2�i�W�����v���Ȃ�
	if (pPlayer->GetMotionPlayer()->GetMotion() == CMotionPlayer::MOTION_PLAYER_TYPE_DOUBLE_JUMP)
	{
		// �d�͂������Ȃ肷������d�͂̍ő�l�ŌŒ肵�Ă���
		if (m_move.y <= -PLAYER_MAX_DOUBLE_JUMP_GRAVITY)
		{
			m_move.y = -PLAYER_MAX_DOUBLE_JUMP_GRAVITY;
		}
	}
	else
	{
		if (m_move.y <= -PLAYER_MAX_GRAVITY)
		{
			m_move.y = -PLAYER_MAX_GRAVITY;
		}
	}

	// �~�߂��Ă��Ȃ����A�v�����I����Ă��Ȃ��Ȃ�
	if (pPlayer->GetStop() == false && pPlayer->GetLandFinish() == false)
	{
		// �ړ�����(�ڂ��������͊֐��̒�)
		Move(pPlayer);

#ifdef _DEBUG
		// �f�o�b�O�p�̈ړ�
		//DebugMove(pPlayer);
#endif
	}

	// �ړ��̊���(�ڂ��������͊֐��̒�)
	MoveInteria(pPlayer);

	// ���ݎ~�܂��Ă��邩�A�����ړ����[�V�������A���[�V�������Ȃ��Ă��Ȃ�������
	if (m_move.x == 0.0f && m_move.z == 0.0f &&
		pPlayer->GetMotionPlayer()->GetMotion() == CMotionPlayer::MOTION_PLAYER_TYPE_MOVE &&
		pPlayer->GetMotionPlayer()->GetConnect() == false)
	{
		// �j���[�g�������[�V�����ɂ���
		pPlayer->GetMotionPlayer()->SetMotion(CMotionPlayer::MOTION_PLAYER_TYPE_NEUTRAL);
	}
	// �O�̃t���[���Ɏ~�܂��Ă��Č��ݓ����Ă��邩�A�W�����v�֘A���[�V�����ł͂Ȃ��Ƃ�
	else if (((m_moveOld.x == 0.0f && m_move.x != 0.0f) || (m_moveOld.z == 0.0f && m_move.z != 0.0f)) &&
		(pPlayer->GetMotionPlayer()->GetMotion() != CMotionPlayer::MOTION_PLAYER_TYPE_JUMP &&
		pPlayer->GetMotionPlayer()->GetMotion() != CMotionPlayer::MOTION_PLAYER_TYPE_JUMP_FALL &&
		pPlayer->GetMotionPlayer()->GetMotion() != CMotionPlayer::MOTION_PLAYER_TYPE_DOUBLE_JUMP) &&
		pPlayer->GetLandFinish() == false)
	{
		// �ړ����[�V�����ɂ���
		pPlayer->GetMotionPlayer()->SetMotion(CMotionPlayer::MOTION_PLAYER_TYPE_MOVE);
	}
	// �ړ��ʂ��������Ă��鎞���A�W�����v���Ă��邩�A���n���Ă��Ȃ������A2�i�W�����v���Ă��Ȃ������A���[�V�������Ȃ��Ă��Ȃ�������
	else if (m_move.y < -PLAYER_GRAVITY && pPlayer->GetJump() == true && pPlayer->GetLand() == false &&
		pPlayer->GetMotionPlayer()->GetMotion() != CMotionPlayer::MOTION_PLAYER_TYPE_DOUBLE_JUMP &&
		pPlayer->GetMotionPlayer()->GetConnect() == false)
	{
		// �������[�V�����ɂ���
		pPlayer->GetMotionPlayer()->SetMotion(CMotionPlayer::MOTION_PLAYER_TYPE_JUMP_FALL);
	}

	// �n��ɒ��n���Ă��鎞
	if (pPlayer->GetMotionPlayer()->GetMotion() != CMotionPlayer::MOTION_PLAYER_TYPE_JUMP_FALL &&
		pPlayer->GetMotionPlayer()->GetConnect() == true)
	{
		m_fSpeed = PLAYER_MOVE_SPEED;
	}

	// �ړ����Ȃ�
	if (pPlayer->GetMotionPlayer()->GetMotion() == CMotionPlayer::MOTION_PLAYER_TYPE_MOVE &&
		(m_move.x >= 1.0f || m_move.x <= -1.0f || m_move.z >= 1.0f || m_move.z <= -1.0f))
	{
		m_nMoveCount++;

		if (m_nMoveCount % 8 == 0)
		{
			// ���̃p�[�e�B�N���̐���
			m_pParticle = CParticleWind::Create(D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f), D3DXVECTOR3(850.0f, 10.0f, 0.0f),
				D3DCOLOR_RGBA(255, 255, 255, 105), D3DXVECTOR3(-40.0f - m_move.x, 0.0f, 0.0f), pPlayer->GetNum());
		}

		if (m_nMoveCount % 10 == 0)
		{
			// �ʒu���擾
			D3DXVECTOR3 pos = pPlayer->GetPos();

			// �y���̐���
			CBillboardSmoke::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR3(35.0f, 35.0f, 0.0f), 20);
		}
	}
	else
	{
		m_nMoveCount = 0;
	}

	// 2�i�W�����v���Ȃ�
	if (pPlayer->GetMotionPlayer()->GetMotion() == CMotionPlayer::MOTION_PLAYER_TYPE_DOUBLE_JUMP)
	{
		// ���x��������
		m_fSpeed = PLAYER_MOVE_SPEED * 0.5f;

		m_nSpinCount++;

		if (m_nSpinCount >= 8)
		{
			// SE�̍Đ�
			pSound->Play(CSound::SOUND_LABEL_SPIN);
			pSound->ControllVoice(CSound::SOUND_LABEL_SPIN, 0.2f);
			m_nSpinCount = 0;
		}
	}
	else
	{
		m_nSpinCount = 0;
	}
	
	// ��]�̊���(�ڂ��������͊֐��̒�)
	Rotate(pPlayer);

	// �v���C���[�����n���Ă���Ȃ�
	if (pPlayer->GetLand() == true)
	{
		// Y�����̈ړ��ʂ�0��
		m_move.y = 0.0f;
		m_fFloat = 0.0f;
	}

	// �`���[�g���A���Ő���������Ȃ����ACPU����Ȃ��ꍇ�̂�
	if (CTutorial::GetExplanation() == false && pPlayer->GetCpu() == false)
	{
		// �v�����I����Ă��Ȃ��Ȃ�
		if (pPlayer->GetLandFinish() == false)
		{
			// �W�����v����(�ڂ��������͊֐��̒�)
			Jump(pPlayer);
		}
	}

	// �w�肳�ꂽ���̈ړ�����
	CommandMove(pPlayer); 
}

//=============================================================================
// ��������
//=============================================================================
CControlPlayer *CControlPlayer::Create(void)
{
	// �C���X�^���X�̐���
	CControlPlayer *pControlPlayer = NULL;

	// �k���`�F�b�N
	if (pControlPlayer == NULL)
	{
		// �N���X�̐���
		pControlPlayer = new CControlPlayer;
		if (pControlPlayer != NULL)
		{
			// ����������
			pControlPlayer->Init();
		}
	}

	return pControlPlayer;
}

//=============================================================================
// �ړ�����
//=============================================================================
void CControlPlayer::Move(CPlayer *pPlayer)
{
	// �L�[�{�[�h�擾����
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	// �Q�[���p�b�h�擾����
	CInputPad *pInputPad;
	pInputPad = CManager::GetInputPad();

	//�T�E���h�擾����
	CSound *pSound;
	pSound = CManager::GetSound();

	D3DXVECTOR3 rotCamera = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �J�����̃|�C���^�z��1�Ԗڂ̃A�h���X�擾
	CCamera **pCameraAddress = CManager::GetCamera();

	// �J�����̍ő吔�Ԃ�A�|�C���^�z���i�߂�
	for (int nCntCamera = 0; nCntCamera < MAX_CAMERA; nCntCamera++, pCameraAddress++)
	{
		// �J�����̎擾
		CCamera *pCamera = &**pCameraAddress;
		if (pCamera != NULL)
		{
			// �v���C���[�̔ԍ��ƃJ�����̔ԍ���������������
			if (pPlayer->GetNum() == pCamera->GetNum())
			{
				// �J�����̌����擾
				rotCamera = pCamera->GetRotV();
			}
		}
	}

	// �v���C���[�̔ԍ����擾
	int nNumPlayer = pPlayer->GetNum();

	// ���ꂼ��̃v���C���[�̓��̓L�[��ݒ�
	int nLeft, nRight;
	if (nNumPlayer == 0)
	{
		nLeft = DIK_A;
		nRight = DIK_D;
	}
	else if (nNumPlayer == 1)
	{
		nLeft = DIK_LEFT;
		nRight = DIK_RIGHT;
	}

	// �n��ɂ��鎞
	if (pPlayer->GetLand() == true)
	{
		// �`���[�g���A���Ő���������Ȃ����ACPU����Ȃ��ꍇ�̂�
		if (CTutorial::GetExplanation() == false && pPlayer->GetCpu() == false)
		{
			//***********************************************************************
				// ���E�����݂ɘA�łŃ_�b�V�� (1P:A/D�A2P:��/�� �܂��� �p�b�h���X�e�B�b�N)
				//***********************************************************************
			if (pInputKeyboard->GetTrigger(nLeft) == true ||
				pInputPad->GetTrigger(CInputPad::PAD_INPUTTYPE_LSTICK_LEFT, nNumPlayer) == true ||
				pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_DPAD_LEFT, nNumPlayer) == true)
			{
				if (m_nextInput == PLAYER_NEXT_INPUT_NONE || m_nextInput == PLAYER_NEXT_INPUT_LEFT)
				{
					// SE�̍Đ�
					pSound->Play(CSound::SOUND_LABEL_ACCELERATION);
					pSound->ControllVoice(CSound::SOUND_LABEL_ACCELERATION, 0.4f);

					// �����x�ɉ��Z���A���X�ɉ���������
					m_fSpeed += PLAYER_ADD_SPEED;

					// ���ɓ��͂��鑀���ݒ�
					m_nextInput = PLAYER_NEXT_INPUT_RIGHT;
				}
			}
			else if (pInputKeyboard->GetTrigger(nRight) == true ||
				pInputPad->GetTrigger(CInputPad::PAD_INPUTTYPE_LSTICK_RIGHT, nNumPlayer) == true ||
				pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_DPAD_RIGHT, nNumPlayer) == true)
			{
				if (m_nextInput == PLAYER_NEXT_INPUT_NONE || m_nextInput == PLAYER_NEXT_INPUT_RIGHT)
				{
					// SE�̍Đ�
					pSound->Play(CSound::SOUND_LABEL_ACCELERATION);
					pSound->ControllVoice(CSound::SOUND_LABEL_ACCELERATION, 0.4f);

					// �����x�ɉ��Z���A���X�ɉ���������
					m_fSpeed += PLAYER_ADD_SPEED;

					// ���ɓ��͂��鑀���ݒ�
					m_nextInput = PLAYER_NEXT_INPUT_LEFT;
				}
			}
		}

		//�ړI�̌�����ݒ�
		m_fObjectiveRot = rotCamera.y + D3DX_PI / 2.0f;
		//��]�̊������I���ɂ���
		m_bRotate = true;
	}

	//�ړ��ʉ��Z
	m_move.x += -cosf(rotCamera.y) * m_fSpeed;
	m_move.z += +sinf(rotCamera.y) * m_fSpeed;
}

//=============================================================================
// �W�����v����
//=============================================================================
void CControlPlayer::Jump(CPlayer *pPlayer)
{
	// �L�[�{�[�h�擾����
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	// �Q�[���p�b�h�擾����
	CInputPad *pInputPad;
	pInputPad = CManager::GetInputPad();

	// ���[�V�����擾����
	CMotionPlayer *pMotionPlayer = NULL;
	pMotionPlayer = pPlayer->GetMotionPlayer();

	// �T�E���h�擾����
	CSound *pSound;
	pSound = CManager::GetSound();

	// �v���C���[�̔ԍ����擾
	int nNumPlayer = pPlayer->GetNum();

	// ���ꂼ��̃v���C���[�̓��̓L�[��ݒ�
	int nJump;
	if (nNumPlayer == 0)
	{
		nJump = DIK_SPACE;
	}
	else if (nNumPlayer == 1)
	{
		nJump = DIK_RETURN;
	}

	// 2�i�W�����v�����A���[�V������2�i�W�����v�Ȃ�
	if (m_bDoubleJump == true && pPlayer->GetMotionPlayer()->GetMotion() == CMotionPlayer::MOTION_PLAYER_TYPE_DOUBLE_JUMP)
	{
		// �v���C���[���~�߂��Ă��Ȃ����AY�����̈ړ��ʂ��}�C�i�X��������
		if (pPlayer->GetStop() == false && m_move.y < 0.0f)
		{
			// �L�[�{�[�h�ł�2�l�̓��͏���ʁX�ŕۑ�
			for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_NUM; nCntPlayer++)
			{
				//***********************************************************************
				// �z�o�����O (1P:SPACE, 2P:O �܂��� �p�b�hA�{�^��)
				//***********************************************************************
				if (pInputKeyboard->GetTrigger(nJump) == true ||
					pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_A, nNumPlayer) == true)
				{
					// Y�����ւ̈ړ��ʂ���������0��
					m_move.y = 0.0f;

					// �����΂���
					m_fFloat += 0.1f;

					// ���͂��d�͂̒l�𒴂���Ə㏸����̂�h��
					if (m_fFloat >= PLAYER_GRAVITY - 0.1f)
					{
						m_fFloat = PLAYER_GRAVITY - 0.1f;
					}
				}
				else
				{
					// ���͂����Z�b�g
					m_fFloat = 0.0f;
				}
			}
		}
	}

	// �v���C���[���W�����v�������A2�i�W�����v���܂����Ă��Ȃ��Ȃ�
	if (pPlayer->GetJump() == true && m_bJump == true &&
		m_bDoubleJump == false)
	{
		// �~�߂��Ă��Ȃ��ꍇ
		if (pPlayer->GetStop() == false)
		{
			// �L�[�{�[�h�ł�2�l�̓��͏���ʁX�ŕۑ�
			for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_NUM; nCntPlayer++)
			{
				// �v���C���[���ɓ��͏��𕪂���
				if (nNumPlayer == nCntPlayer)
				{
					//***********************************************************************
					// 2�i�W�����v (1P:SPACE, 2P:O �܂��� �p�b�hA�{�^��)
					//***********************************************************************
					if (pInputKeyboard->GetTrigger(nJump) == true ||
						pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_A, nNumPlayer) == true)
					{
						// �ʒu���擾
						D3DXVECTOR3 pos = pPlayer->GetPos();
						// �p�[�e�B�N���ŉ~�`�̃G�t�F�N�g���o��
						m_pParticle = CParticleCircle::Create(pos, D3DXVECTOR3(10.0f, 3.5f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							D3DCOLOR_RGBA(255, 255, 255, 55), 10, 2.0f);
						m_pParticle = CParticleCircle::Create(pos, D3DXVECTOR3(10.0f, 3.5f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							D3DCOLOR_RGBA(255, 255, 255, 55), 10, 3.0f);

						// Y�����̈ړ��ʂ�0��
						m_move.y = 0.0f;

						// �ړ����x�̒l���}�C�i�X�ɂȂ�Ȃ��悤�ɂ���
						if (m_fSpeed <= 0.1f)
						{
							m_fSpeed = 0.1f;
						}

						// �W�����v���[�V�����ɂ���
						pMotionPlayer->SetMotion(CMotionPlayer::MOTION_PLAYER_TYPE_DOUBLE_JUMP);

						// �ړ��ʂ��W�����v�͂Ԃ���Z
						m_move.y = PLAYER_DOUBLE_JUMP;

						m_bDoubleJump = true;
					}
				}
			}
		}
	}

	// �v���C���[�����n���Ă���Ȃ�
	if (pPlayer->GetLand() == true)
	{
		// �W�����v�͂̐ݒ�
		float fJump = 0.0f;
		fJump = m_fSpeed * PLAYER_JUMP_MAGNIFICATION;

		// �~�߂��Ă��Ȃ��ꍇ
		if (pPlayer->GetStop() == false)
		{
			// �L�[�{�[�h�ł�2�l�̓��͏���ʁX�ŕۑ�
			for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_NUM; nCntPlayer++)
			{
				// �v���C���[���ɓ��͏��𕪂���
				if (nNumPlayer == nCntPlayer)
				{
					//***********************************************************************
					// �W�����v (1P:SPACE, 2P:O �܂��� �p�b�hA�{�^��)
					//***********************************************************************
					if (pInputKeyboard->GetTrigger(nJump) == true ||
						pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_A, nNumPlayer) == true)
					{
						// SE�̍Đ�
						pSound->Play(CSound::SOUND_LABEL_JUMP);
						pSound->ControllVoice(CSound::SOUND_LABEL_JUMP, 0.4f);

						// �W�����v���[�V�����ɂ���
						pMotionPlayer->SetMotion(CMotionPlayer::MOTION_PLAYER_TYPE_JUMP);

						// �W�����v���ɒl���Ⴗ�����ꍇ�͍Œ���̃W�����v�͂���������
						if (fJump <= 8.0f)
						{
							fJump = 8.0f;
						}

						// �ړ��ʂ��W�����v�͂Ԃ���Z
						m_move.y = fJump;

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
	}
}

//=============================================================================
// �ړ��̊����ɂ��Ă̏���
//=============================================================================
void CControlPlayer::MoveInteria(CPlayer *pPlayer)
{
	// �n��ɂ����銵���̌��Z
	if (pPlayer->GetLand() == true)
	{
		m_move.z *= PLAYER_LAND_INTERIA_SUBTRACTION;
		m_move.x *= PLAYER_LAND_INTERIA_SUBTRACTION;
	}
	// �󒆂ɂ����銵���̌��Z
	else
	{
		m_move.z *= PLAYER_AIR_INTERIA_SUBTRACTION;
		m_move.x *= PLAYER_AIR_INTERIA_SUBTRACTION;
	}

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
	pPlayer->SetRot(rotPlayer);
}

//=============================================================================
// �s�����w�肳�ꂽ���̈ړ�����
//=============================================================================
void CControlPlayer::CommandMove(CPlayer *pPlayer)
{
	// �T�E���h�擾����
	CSound *pSound;
	pSound = CManager::GetSound();

	// ���[�V�����擾����
	CMotionPlayer *pMotionPlayer = NULL;
	pMotionPlayer = pPlayer->GetMotionPlayer();

	// �R�}���h�ɂ�鐧��
	switch (pPlayer->GetCommand())
	{
		// �_�b�V��
	case CPlayer::PLAYER_COMMAND_DASH:
		// SE�̍Đ�
		pSound->Play(CSound::SOUND_LABEL_ACCELERATION);
		pSound->ControllVoice(CSound::SOUND_LABEL_ACCELERATION, 0.3f);

		// �����x�ɉ��Z���A���X�ɉ���������
		m_fSpeed += PLAYER_ADD_SPEED;
		break;

	case CPlayer::PLAYER_COMMAND_JUMP:
		// ���n���Ă���Ȃ�
		if (pPlayer->GetLand() == true)
		{
			// SE�̍Đ�
			pSound->Play(CSound::SOUND_LABEL_JUMP);
			pSound->ControllVoice(CSound::SOUND_LABEL_JUMP, 0.4f);

			// �W�����v�͂̐ݒ�
			float fJump = 0.0f;
			fJump = m_fSpeed * PLAYER_JUMP_MAGNIFICATION;

			// �W�����v���[�V�����ɂ���
			pMotionPlayer->SetMotion(CMotionPlayer::MOTION_PLAYER_TYPE_JUMP);

			// �W�����v���ɒl���Ⴗ�����ꍇ�͍Œ���̃W�����v�͂���������
			if (fJump <= 8.0f)
			{
				fJump = 8.0f;
			}

			// �ړ��ʂ��W�����v�͂Ԃ���Z
			m_move.y = fJump;

			// �v���C���[�̔�����W�����v�����A���n���Ă��Ȃ��ɐݒ�
			pPlayer->SetJump(true);
			pPlayer->SetLand(false);
			m_bJump = true;
		}
		break;

	case CPlayer::PLAYER_COMMAND_DOUBLE_JUMP:
		// �v���C���[���W�����v�������A2�i�W�����v���܂����Ă��Ȃ��Ȃ�
		if (pPlayer->GetJump() == true && m_bJump == true &&
			m_bDoubleJump == false)
		{
			// �ʒu���擾
			D3DXVECTOR3 pos = pPlayer->GetPos();
			// �p�[�e�B�N���ŉ~�`�̃G�t�F�N�g���o��
			m_pParticle = CParticleCircle::Create(pos, D3DXVECTOR3(10.0f, 3.5f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 55), 10, 2.0f);
			m_pParticle = CParticleCircle::Create(pos, D3DXVECTOR3(10.0f, 3.5f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 55), 10, 3.0f);

			// Y�����̈ړ��ʂ�0��
			m_move.y = 0.0f;

			// �W�����v���[�V�����ɂ���
			pMotionPlayer->SetMotion(CMotionPlayer::MOTION_PLAYER_TYPE_DOUBLE_JUMP);

			// �ړ��ʂ��W�����v�͂Ԃ���Z
			m_move.y = PLAYER_DOUBLE_JUMP;

			m_bDoubleJump = true;
		}
		break;

	case CPlayer::PLAYER_COMMAND_HOVERING:
		// 2�i�W�����v�����A���[�V������2�i�W�����v�Ȃ�
		if (m_bDoubleJump == true && pPlayer->GetMotionPlayer()->GetMotion() == CMotionPlayer::MOTION_PLAYER_TYPE_DOUBLE_JUMP)
		{
			if (m_move.y < 0.0f)
			{
				// ���͂𑝂₷
				m_fFloat += 0.1f;

				m_move.y = 0.0f;

				// ���͂��d�͂̒l�𒴂���Ə㏸����̂�h��
				if (m_fFloat >= PLAYER_GRAVITY - 0.1f)
				{
					m_fFloat = PLAYER_GRAVITY - 0.1f;
				}

				// ���͂����X�ɉ�����
				m_fFloat = 0.0f;
			}
		}
		break;

	default:
		break;
	}

	// �R�}���h��߂�
	pPlayer->SetCommand(CPlayer::PLAYER_COMMAND_NONE);
}

#ifdef _DEBUG
//=============================================================================
// �f�o�b�O�p�̈ړ�����
//=============================================================================
void CControlPlayer::DebugMove(CPlayer *pPlayer)
{
	// �L�[�{�[�h�擾����
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	// �Q�[���p�b�h�擾����
	CInputPad *pInputPad;
	pInputPad = CManager::GetInputPad();

	D3DXVECTOR3 rotCamera = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �J�����̃|�C���^�z��1�Ԗڂ̃A�h���X�擾
	CCamera **pCameraAddress = CManager::GetCamera();

	// �J�����̍ő吔�Ԃ�A�|�C���^�z���i�߂�
	for (int nCntCamera = 0; nCntCamera < MAX_CAMERA; nCntCamera++, pCameraAddress++)
	{
		// �J�����̎擾
		CCamera *pCamera = &**pCameraAddress;
		if (pCamera != NULL)
		{
			// �v���C���[�̔ԍ��ƃJ�����̔ԍ���������������
			if (pPlayer->GetNum() == pCamera->GetNum())
			{
				// �J�����̌����擾
				rotCamera = pCamera->GetRotV();
			}
		}
	}

	// �v���C���[�̔ԍ����擾
	int nNumPlayer = pPlayer->GetNum();

	// �L�[�{�[�h�ł�2�l�̓��͏���ʁX�ŕۑ�
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_NUM; nCntPlayer++)
	{
		int nLeft, nRight, nUp, nDown;

		// ���ꂼ��̃v���C���[�̓��̓L�[��ݒ�
		if (nNumPlayer == 0)
		{
			nLeft = DIK_A;
			nRight = DIK_D;
			nUp = DIK_W;
			nDown = DIK_S;
		}
		else if (nNumPlayer == 1)
		{
			nLeft = DIK_LEFT;
			nRight = DIK_RIGHT;
			nUp = DIK_UP;
			nDown = DIK_DOWN;
		}

		//�ړ��̉����x
		m_fSpeed = PLAYER_MOVE_SPEED;

		//***********************************************************************
		// �ړ� (1P:W/A/S/D, 2P:��/��/��/�� �܂��� �p�b�h���X�e�B�b�N)
		//***********************************************************************
		if (pInputKeyboard->GetPress(nLeft) == true ||
			pInputPad->GetPress(CInputPad::PAD_INPUTTYPE_LSTICK_LEFT, nNumPlayer) == true)
		{
			//�����ړ�
			if (pInputKeyboard->GetPress(nUp) == true ||
				pInputPad->GetPress(CInputPad::PAD_INPUTTYPE_LSTICK_UP, nNumPlayer) == true)
			{
				//�ړ��ʉ��Z
				m_move.x += +cosf(rotCamera.y + D3DX_PI / 4.0f) * m_fSpeed;
				m_move.z += -sinf(rotCamera.y + D3DX_PI / 4.0f) * m_fSpeed;

				//�ړI�̌�����ݒ�
				m_fObjectiveRot = rotCamera.y - D3DX_PI / 4.0f;
				//��]�̊������I���ɂ���
				m_bRotate = true;
			}
			//����O�ړ�
			else if (pInputKeyboard->GetPress(nDown) == true ||
				pInputPad->GetPress(CInputPad::PAD_INPUTTYPE_LSTICK_DOWN, nNumPlayer) == true)
			{
				//�ړ��ʉ��Z
				m_move.x += +cosf(rotCamera.y - D3DX_PI / 4.0f) * m_fSpeed;
				m_move.z += -sinf(rotCamera.y - D3DX_PI / 4.0f) * m_fSpeed;

				//�ړI�̌�����ݒ�
				m_fObjectiveRot = rotCamera.y - (D3DX_PI / 4.0f) * 3.0f;
				//��]�̊������I���ɂ���
				m_bRotate = true;
			}
			//���ړ�
			else
			{
				//�ړ��ʉ��Z
				m_move.x += +cosf(rotCamera.y) * m_fSpeed;
				m_move.z += -sinf(rotCamera.y) * m_fSpeed;

				//�ړI�̌�����ݒ�
				m_fObjectiveRot = rotCamera.y - D3DX_PI / 2.0f;
				//��]�̊������I���ɂ���
				m_bRotate = true;
			}
		}
		else if (pInputKeyboard->GetPress(nRight) == true ||
			pInputPad->GetPress(CInputPad::PAD_INPUTTYPE_LSTICK_RIGHT, nNumPlayer) == true)
		{
			//�E���ړ�
			if (pInputKeyboard->GetPress(nUp) == true ||
				pInputPad->GetPress(CInputPad::PAD_INPUTTYPE_LSTICK_UP, nNumPlayer) == true)
			{
				//�ړ��ʉ��Z
				m_move.x += -cosf(rotCamera.y - D3DX_PI / 4.0f) * m_fSpeed;
				m_move.z += +sinf(rotCamera.y - D3DX_PI / 4.0f) * m_fSpeed;

				//�ړI�̌�����ݒ�
				m_fObjectiveRot = rotCamera.y + D3DX_PI / 4.0f;
				//��]�̊������I���ɂ���
				m_bRotate = true;
			}
			//�E��O�ړ�
			else if (pInputKeyboard->GetPress(nDown) == true ||
				pInputPad->GetPress(CInputPad::PAD_INPUTTYPE_LSTICK_DOWN, nNumPlayer) == true)
			{
				//�ړ��ʉ��Z
				m_move.x += -cosf(rotCamera.y + D3DX_PI / 4.0f) * m_fSpeed;
				m_move.z += +sinf(rotCamera.y + D3DX_PI / 4.0f) * m_fSpeed;

				//�ړI�̌�����ݒ�
				m_fObjectiveRot = rotCamera.y + D3DX_PI / 1.5f;
				//��]�̊������I���ɂ���
				m_bRotate = true;
			}
			//�E�ړ�
			else
			{
				//�ړ��ʉ��Z
				m_move.x += -cosf(rotCamera.y) * m_fSpeed;
				m_move.z += +sinf(rotCamera.y) * m_fSpeed;

				//�ړI�̌�����ݒ�
				m_fObjectiveRot = rotCamera.y + D3DX_PI / 2.0f;
				//��]�̊������I���ɂ���
				m_bRotate = true;
			}
		}
		//���ړ�
		else if (pInputKeyboard->GetPress(nUp) == true ||
			pInputPad->GetPress(CInputPad::PAD_INPUTTYPE_LSTICK_UP, nNumPlayer) == true)
		{
			//�ړ��ʉ��Z
			m_move.z += -cosf(rotCamera.y) * m_fSpeed;
			m_move.x += -sinf(rotCamera.y) * m_fSpeed;

			//��������������
			//�ړI�̌�����ݒ�
			m_fObjectiveRot = rotCamera.y;
			//��]�̊������I���ɂ���
			m_bRotate = true;
		}
		//��O�ړ�
		else if (pInputKeyboard->GetPress(nDown) == true ||
			pInputPad->GetPress(CInputPad::PAD_INPUTTYPE_LSTICK_DOWN, nNumPlayer) == true)
		{
			//�ړ��ʉ��Z
			m_move.z += +cosf(rotCamera.y) * m_fSpeed;
			m_move.x += +sinf(rotCamera.y) * m_fSpeed;

			//����O����������
			//�ړI�̌�����ݒ�
			m_fObjectiveRot = rotCamera.y + D3DX_PI;
			//��]�̊������I���ɂ���
			m_bRotate = true;
		}
	}
}
#endif