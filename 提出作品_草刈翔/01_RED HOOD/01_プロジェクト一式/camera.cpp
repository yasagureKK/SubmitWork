//=============================================================================
// �J�������� [camera.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "player.h"
#include "enemy.h"
#include "keyboard.h"
#include "gamepad.h"
#include "game.h"
#include "pause.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CAMERA_HEIGHT				(170.0f)	// �J�����̍���
#define CAMERA_MAX_HEIGHT			(190.0f)	// �J�����̍ő卂��
#define CAMERA_MIN_HEIGHT			(150.0f)	// �J�����̍ŏ�����
#define CAMERA_MOVE_SPEED			(5.0f)		// �J�����̈ړ����x
#define CAMERA_INDUCTION_SPEED		(45.0f)		// �J�����̗U�����x
#define CAMERA_MAX_ROTATE_SPEED		(0.03f)		// �J�����̍ő���񑬓x
#define CAMERA_INTERIA_SUBTRACTION	(0.92f)		// ����̊����̌��Z�l
#define CAMERA_ROTATE_STOP_COUNT	(0.002f)	// �v���C���[�̈ړ��ʂ�0�ɂ��鎞�̈ړ��ʂ̒l
#define CAMERA_R_DISTANCE			(650.0f)	// �v���C���[�ƒ����_�̋���
#define CAMERA_V_DISTANCE			(1000.0f)	// ���_�ƒ����_�̋���
#define CAMERA_MAX_DISTANCE			(1200.0f)	// ���_�ƒ����_�̍ő勗��
#define CAMERA_MIN_DISTANCE			(900.0f)	// ���_�ƒ����_�̍ŏ�����
#define CAMERA_MIN_RENDERER			(10.0f)		// �`��͈͂̍ŏ��l
#define CAMERA_MAX_RENDERER			(40000.0f)	// �`��͈͂̍ő�l
#define CAMERA_CLEAR_HEIGHT			(60.0f)		// �N���A���̃J�����̍���
#define CAMERA_CLEAR_R_DISTANCE		(650.0f)	// �N���A���̃v���C���[�ƒ����_�̋���
#define CAMERA_CLEAR_V_DISTANCE		(900.0f)	// �N���A���̎��_�ƒ����_�̋���

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
bool CCamera::m_bControl = false;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCamera::CCamera()
{
	// �ϐ��̃N���A
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotAsk = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fSpeed = 0.0f;
	m_fRotateSpeed = 0.0f;
	m_fHeight = 0.0f;
	m_fSaveHeight = 0.0f;
	m_fDistance_PlayerPosR = 0.0f;
	m_fDistance_posVposR = 0.0f;
	m_fSaveDistance_PlayerPosR = 0;
	m_fSaveDistance_posVposR = 0;
	m_fObjectiveRot = 0.0f;
	m_fObjectiveDistance = 0.0f;
	m_fNumRot = 0.0f;
	m_viewport.X = (DWORD)0.0f;
	m_viewport.Y = (DWORD)0.0f;
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 0.0f;
	m_viewport.Width = (DWORD)0.0f;
	m_viewport.Height = (DWORD)0.0f;
	m_bRotate = false;
	m_bControl = false;
	m_type = CAMERA_MOVE_TYPE_PLAYER;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCamera::~CCamera()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CCamera::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float viewportX, float viewportY, float viewportWidth, float viewporHeight)
{
	// �ϐ��̏�����
	m_rot = rot;
	m_rotAsk = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posV = pos;
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_fSpeed = CAMERA_MOVE_SPEED;
	m_fRotateSpeed = 0.0f;
	m_fHeight = CAMERA_HEIGHT;
	m_fSaveHeight = 0.0f;
	m_fDistance_PlayerPosR = CAMERA_R_DISTANCE;
	m_fDistance_posVposR = CAMERA_V_DISTANCE;
	m_fSaveDistance_PlayerPosR = 0;
	m_fSaveDistance_posVposR = 0;
	m_fObjectiveRot = 0.0f;
	m_fObjectiveDistance = CAMERA_V_DISTANCE;
	m_fNumRot = 0.0f;
	m_viewport.X = (DWORD)viewportX;
	m_viewport.Y = (DWORD)viewportY;
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 1.0f;
	m_viewport.Width = (DWORD)viewportWidth;
	m_viewport.Height = (DWORD)viewporHeight;
	m_bRotate = false;
	m_bControl = false;
	m_type = CAMERA_MOVE_TYPE_PLAYER;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CCamera::Uninit(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void CCamera::Update(void)
{
	// �|�[�Y���Ă��Ȃ�������
	if (CManager::GetPause() == false)
	{
		// �v���C���[�̒ǔ�
		if (m_type == CAMERA_MOVE_TYPE_PLAYER)
		{
			MovePlayer();
		}
		// �{�X��
		else if (m_type == CAMERA_MOVE_TYPE_BOSS)
		{
			MoveBoss();
		}
		// �Q�[���N���A
		else if (m_type == CAMERA_MOVE_TYPE_CLEAR)
		{
			MoveClear();
		}
	}
	
	// �f�o�b�O�p�̈ړ�
#ifdef _DEBUG
	if (m_type == CAMERA_MOVE_TYPE_DEBUG)
	{
		DebugMove();
	}
#endif
}

//=============================================================================
// ��������
//=============================================================================
CCamera *CCamera::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float viewportX, float viewportY,
	float viewportWidth, float viewporHeight)
{
	// �N���X�̐���
	CCamera *pCamera = NULL;
	if (pCamera == NULL)
	{
		pCamera = new CCamera;
		if (pCamera != NULL)
		{
			// ����������
			pCamera->Init(pos, rot, viewportX, viewportY, viewportWidth, viewporHeight);
		}
	}
	return pCamera;
}

//=============================================================================
// �ݒ菈��
//=============================================================================
void CCamera::Set(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(50.0f),									// ��p
		(float)m_viewport.Width / (float)m_viewport.Height,		// �䗦
		CAMERA_MIN_RENDERER,									// Z�����̕`��͈�
		CAMERA_MAX_RENDERER);

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_posR, &m_vecU);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//=============================================================================
// �v���C���[�p�̈ړ�����
//=============================================================================
void CCamera::MovePlayer(void)
{
	// �L�[�{�[�h�E�Q�[���p�b�h�擾����
	CKeyboard *pKeyboard = CManager::GetKeyboard();
	CGamePad *pGamePad = CManager::GetGamePad();

	// ����ł���Ȃ�
	if (m_bControl == true)
	{
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// �J������] (�y�L�[ / �b�L�[ �܂��� �p�b�h�E�X�e�B�b�N)
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (pKeyboard->GetPress(DIK_Z) == true && m_bRotate == false ||
			pGamePad->GetPress(CGamePad::PAD_INPUT_RSTICK_LEFT, 0) == true)
		{
			// �����_���玞�v���ɉ�]
			m_fRotateSpeed += 0.01f;

			if (m_fRotateSpeed >= CAMERA_MAX_ROTATE_SPEED)
			{
				m_fRotateSpeed = CAMERA_MAX_ROTATE_SPEED;
			}
		}
		else if (pKeyboard->GetPress(DIK_C) == true && m_bRotate == false ||
			pGamePad->GetPress(CGamePad::PAD_INPUT_RSTICK_RIGHT, 0) == true)
		{
			// �����_���甽���v���ɉ�]
			m_fRotateSpeed -= 0.01f;

			if (m_fRotateSpeed <= -CAMERA_MAX_ROTATE_SPEED)
			{
				m_fRotateSpeed = -CAMERA_MAX_ROTATE_SPEED;
			}
		}
	}

	// ���񂵂Ă���Ȃ�
	if (m_fRotateSpeed != 0.0f)
	{
		// �����ɐ��񑬓x��ǉ����A���_�̈ʒu���ړ�
		m_rot.y += m_fRotateSpeed;
		m_posV.x = m_posR.x + sinf(m_rot.y) * CAMERA_V_DISTANCE;
		m_posV.z = m_posR.z + cosf(m_rot.y) * CAMERA_V_DISTANCE;
	}

	// �����̌��Z
	m_fRotateSpeed *= CAMERA_INTERIA_SUBTRACTION;
	m_fRotateSpeed *= CAMERA_INTERIA_SUBTRACTION;

	// ���񑬓x������̒l�ɂȂ�����0�ɂ���
	if (m_fRotateSpeed <= CAMERA_ROTATE_STOP_COUNT && m_fRotateSpeed >= -CAMERA_ROTATE_STOP_COUNT)
	{
		m_fRotateSpeed = 0.0f;
	}

	// ����ł���Ȃ�
	if (m_bControl == true)
	{
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// �Y�[���C���E�A�E�g (���L�[ / ���L�[ �܂��� �p�b�h�E�X�e�B�b�N)
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (pKeyboard->GetPress(DIK_UP) == true ||
			pGamePad->GetPress(CGamePad::PAD_INPUT_RSTICK_UP, 0) == true)
		{
			if (m_fDistance_posVposR <= CAMERA_MIN_DISTANCE)
			{
				m_fDistance_posVposR = CAMERA_MIN_DISTANCE;
			}
			else
			{
				m_fDistance_posVposR -= 10.0f;
				m_fHeight -= 3.0f;
			}
		}
		else if (pKeyboard->GetPress(DIK_DOWN) == true ||
			pGamePad->GetPress(CGamePad::PAD_INPUT_RSTICK_DOWN, 0) == true)
		{
			if (m_fDistance_posVposR >= CAMERA_MAX_DISTANCE)
			{
				m_fDistance_posVposR = CAMERA_MAX_DISTANCE;
			}
			else
			{
				m_fDistance_posVposR += 10.0f;
				m_fHeight += 3.0f;
			}
		}
	}

	// �I�u�W�F�N�g��������|�C���^�E���̏���ۑ�����|�C���^
	CScene *pObject = NULL;
	CScene *pSaveObject = NULL;
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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

			// �v���C���[�̈ʒu���擾
			pos = pPlayer->GetPos();

			// ����ł���Ȃ�
			if (m_bControl == true)
			{
				//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// �J�������� (Space�L�[ �܂��� �p�b�hL�g���K�[)
				//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				if (m_bRotate == false && (pKeyboard->GetTrigger(DIK_SPACE) == true ||
					pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_LEFT_SHOULDER, 0) == true))
				{
					// �v���C���[�̌������擾
					D3DXVECTOR3 rot = pPlayer->GetRot();

					// �ړI�̌�����ݒ�
					m_fObjectiveRot = rot.y;

					// ��]���ɂ���
					m_bRotate = true;
				}
			}
		}

		// ���̃I�u�W�F�N�g���擾
		pObject = pSaveObject->GetObjNext(pSaveObject);
	}

	// ��]�������ԂȂ�
	if (m_bRotate == true)
	{
		// �v���C���[�̌��݂̌������Ƃɂ��ꂼ�������ς���ʂ��v�Z
		if (m_rot.y < 0.0f && -m_rot.y + m_fObjectiveRot > D3DX_PI)
		{
			m_fNumRot = (-D3DX_PI - m_rot.y) + -(D3DX_PI - m_fObjectiveRot);
		}
		else if (m_rot.y >= D3DX_PI / 2.0f && m_rot.y - m_fObjectiveRot > D3DX_PI)
		{
			m_fNumRot = (D3DX_PI - m_rot.y) - (-D3DX_PI - m_fObjectiveRot);
		}
		else
		{
			m_fNumRot = (m_fObjectiveRot - m_rot.y);
		}

		// �v���C���[�̌����ɉ��Z
		m_rot.y += m_fNumRot * 0.2f;

		// ����̒l�ɒB�������]����߂�
		if ((m_rot.y - m_fObjectiveRot < 0.001 && m_rot.y - m_fObjectiveRot > -0.001) ||
			(-m_rot.y - m_fObjectiveRot < 0.001 && -m_rot.y - m_fObjectiveRot > -0.001))
		{
			m_bRotate = false;
		}
	}

	//�v���C���[�̈ʒu�����Ƃɒ����_�̈ʒu���ړ�
	m_posR.x = pos.x + sinf(m_rot.y) * -(m_fDistance_PlayerPosR);
	m_posR.z = pos.z + cosf(m_rot.y) * -(m_fDistance_PlayerPosR);
	m_posR.y = pos.y;

	//�����_�̈ʒu�����ƂɎ��_�̈ʒu���ړ�
	m_posV.x = m_posR.x + sinf(m_rot.y) * m_fDistance_posVposR;
	m_posV.z = m_posR.z + cosf(m_rot.y) * m_fDistance_posVposR;
	m_posV.y = m_posR.y + m_fHeight;

	// ������3.14����-3.14�̒l�͈͓̔��Ɏ��߂�
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	else if (m_rot.y <= -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}
}

//=============================================================================
// �{�X��p�̈ړ�����
//=============================================================================
void CCamera::MoveBoss(void)
{
	// �L�[�{�[�h�E�Q�[���p�b�h�擾����
	CKeyboard *pKeyboard = CManager::GetKeyboard();
	CGamePad *pGamePad = CManager::GetGamePad();

	// ����ł���Ȃ�
	if (m_bControl == true)
	{
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// �Y�[���C���E�A�E�g (���L�[ / ���L�[ �܂��� �p�b�h�E�X�e�B�b�N)
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (pKeyboard->GetPress(DIK_UP) == true ||
			pGamePad->GetPress(CGamePad::PAD_INPUT_RSTICK_UP, 0) == true)
		{
			if (m_fDistance_posVposR <= CAMERA_MIN_DISTANCE)
			{
				m_fDistance_posVposR = CAMERA_MIN_DISTANCE;
			}
			else
			{
				m_fDistance_posVposR -= 10.0f;
				m_fHeight -= 3.0f;
			}
		}
		else if (pKeyboard->GetPress(DIK_DOWN) == true ||
			pGamePad->GetPress(CGamePad::PAD_INPUT_RSTICK_DOWN, 0) == true)
		{
			if (m_fDistance_posVposR >= CAMERA_MAX_DISTANCE)
			{
				m_fDistance_posVposR = CAMERA_MAX_DISTANCE;
			}
			else
			{
				m_fDistance_posVposR += 10.0f;
				m_fHeight += 3.0f;
			}
		}
	}

	// �I�u�W�F�N�g��������|�C���^�E���̏���ۑ�����|�C���^
	CScene *pObject = NULL;
	CScene *pSaveObject = NULL;
	D3DXVECTOR3 posEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 posPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 rotPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	bool bBeingBoss = false;

	// �擪�̃|�C���^����
	pObject = pObject->GetTopObj(CScene::PRIORITY_MODEL);

	while (pObject != NULL)
	{
		pSaveObject = pObject;
		if (pObject->GetObjType() == CScene::OBJECT_TYPE_ENEMY)
		{
			CEnemy *pEnemy = NULL;
			pEnemy = (CEnemy*)pObject;

			// �G�l�~�[�̎�ނ��{�X�Ȃ�
			if (pEnemy->GetEnemyType() == CEnemy::ENEMY_TYPE_BOSS)
			{
				// �G�l�~�[�̈ʒu���擾
				posEnemy = pEnemy->GetPos();
				bBeingBoss = true;

				// ��]���ɂ���
				m_bRotate = true;
			}
		}
		pObject = pSaveObject->GetObjNext(pSaveObject);
	}
	
	// �擪�̃|�C���^����
	pObject = pObject->GetTopObj(CScene::PRIORITY_MODEL);

	while (pObject != NULL)
	{
		pSaveObject = pObject;
		if (pObject->GetObjType() == CScene::OBJECT_TYPE_PLAYER)
		{
			CPlayer *pPlayer = NULL;
			pPlayer = (CPlayer*)pObject;

			// �v���C���[�̈ʒu���擾
			posPlayer = pPlayer->GetPos();
		}
		pObject = pSaveObject->GetObjNext(pSaveObject);
	}

	if (bBeingBoss == true)
	{
		// �v���C���[�̈ʒu�ƃG�l�~�[�̈ʒu����������Z�o
		m_fObjectiveRot = (float)atan2((posPlayer.x - posEnemy.x), (posPlayer.z - posEnemy.z));
		
		// ��]�������ԂȂ�
		if (m_bRotate == true)
		{
			if (m_rot.y < 0.0f && -m_rot.y + m_fObjectiveRot > D3DX_PI)
			{
				m_fNumRot = (-D3DX_PI - m_rot.y) + -(D3DX_PI - m_fObjectiveRot);
			}
			else if (m_rot.y >= D3DX_PI / 2.0f && m_rot.y - m_fObjectiveRot > D3DX_PI)
			{
				m_fNumRot = (D3DX_PI - m_rot.y) - (-D3DX_PI - m_fObjectiveRot);
			}
			else
			{
				m_fNumRot = (m_fObjectiveRot - m_rot.y);
			}

			// �J�����̌����ɉ��Z
			m_rot.y += m_fNumRot * 0.08f;

			// ����̒l�ɒB�������]����߂�
			if ((m_rot.y - m_fObjectiveRot < 0.001 && m_rot.y - m_fObjectiveRot > -0.001) ||
				(-m_rot.y - m_fObjectiveRot < 0.001 && -m_rot.y - m_fObjectiveRot > -0.001))
			{
				m_bRotate = false;
			}
		}

		//�G�̈ʒu�����Ƃɒ����_�̈ʒu���ړ�
		m_posR.x = posPlayer.x + sinf(m_rot.y) * -(m_fDistance_PlayerPosR);
		m_posR.z = posPlayer.z + cosf(m_rot.y) * -(m_fDistance_PlayerPosR);
		m_posR.y = posPlayer.y;

		//�����_�̈ʒu�����ƂɎ��_�̈ʒu���ړ�
		m_posV.x = m_posR.x + sinf(m_rot.y) * (m_fDistance_posVposR);
		m_posV.z = m_posR.z + cosf(m_rot.y) * (m_fDistance_posVposR);
		m_posV.y = m_posR.y + m_fHeight;
	}
	else if (bBeingBoss == false)
	{
		m_rot.y -= D3DX_PI;
	}

	// ������3.14����-3.14�̒l�͈͓̔��Ɏ��߂�
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	else if (m_rot.y <= -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}
}

//=============================================================================
// �N���A���̈ړ�����
//=============================================================================
void CCamera::MoveClear(void)
{
	// 1�b�����ăv���C���[�ɃY�[������悤�A�����_�⎋�_�E�������ړ�
	if (m_fDistance_posVposR > CAMERA_CLEAR_V_DISTANCE)
	{
		m_fDistance_posVposR -= m_fSaveDistance_posVposR / 60;
	}
	if (m_fDistance_posVposR <= CAMERA_CLEAR_V_DISTANCE)
	{
		m_fDistance_posVposR = CAMERA_CLEAR_V_DISTANCE;
	}

	if (m_fDistance_PlayerPosR > CAMERA_CLEAR_R_DISTANCE)
	{
		m_fDistance_PlayerPosR -= m_fSaveDistance_PlayerPosR / 60;
	}
	if (m_fDistance_PlayerPosR <= CAMERA_CLEAR_R_DISTANCE)
	{
		m_fDistance_PlayerPosR = CAMERA_CLEAR_R_DISTANCE;
	}

	if(m_fHeight > CAMERA_CLEAR_HEIGHT)
	{
		m_fHeight -= m_fSaveHeight / 60;
	}
	if (m_fHeight <= CAMERA_CLEAR_HEIGHT)
	{
		m_fHeight = CAMERA_CLEAR_HEIGHT;
	}

	// �I�u�W�F�N�g��������|�C���^�E���̏���ۑ�����|�C���^
	CScene *pObject = NULL;
	CScene *pSaveObject = NULL;
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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

			// �v���C���[�̈ʒu���擾
			pos = pPlayer->GetPos();
		}

		// ���̃I�u�W�F�N�g���擾
		pObject = pSaveObject->GetObjNext(pSaveObject);
	}

	//�v���C���[�̈ʒu�����Ƃɒ����_�̈ʒu���ړ�
	m_posR.x = pos.x + sinf(m_rot.y) * -(m_fDistance_PlayerPosR);
	m_posR.z = pos.z + cosf(m_rot.y) * -(m_fDistance_PlayerPosR);
	m_posR.y = pos.y;

	//�����_�̈ʒu�����ƂɎ��_�̈ʒu���ړ�
	m_posV.x = m_posR.x + sinf(m_rot.y) * m_fDistance_posVposR;
	m_posV.z = m_posR.z + cosf(m_rot.y) * m_fDistance_posVposR;
	m_posV.y = m_posR.y + m_fHeight;

	// ������3.14����-3.14�̒l�͈͓̔��Ɏ��߂�
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	else if (m_rot.y <= -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}
}

//=============================================================================
// ���ʂ̌������v�Z���鏈��
//=============================================================================
D3DXVECTOR3 CCamera::MathFront(void)
{
	// ���_�ƒ����_�̈ʒu���琳�ʂ̌������v�Z
	D3DXVECTOR3 posR = GetPosV(), posV = GetPosR();
	float fFrontRot = (float)atan2((posR.x - posV.x), (posR.z - posV.z)) - D3DX_PI;

	return D3DXVECTOR3(0.0f, fFrontRot, 0.0f);
}

//=============================================================================
// �J�����̓����̐ݒ菈��
//=============================================================================
void CCamera::SetCameraMove(CAMERA_MOVE_TYPE type)
{
	// �f�o�b�O�p�̈ړ��ȊO�Ȃ�
	if (m_type != CAMERA_MOVE_TYPE_DEBUG)
	{
		// �J�����̓�����ς���
		m_type = type;
	}

	// �N���A�̓����ɂȂ�����
	if (type == CAMERA_MOVE_TYPE_CLEAR)
	{
		// ���݂̃J�����Ԃ̋����⍂����ۑ�
		m_fSaveDistance_PlayerPosR = m_fDistance_PlayerPosR;
		m_fSaveDistance_posVposR = m_fDistance_posVposR;
		m_fSaveHeight = m_fHeight;

		m_fSaveDistance_PlayerPosR -= CAMERA_CLEAR_R_DISTANCE;
		m_fSaveDistance_posVposR -= CAMERA_CLEAR_V_DISTANCE;
		m_fSaveHeight -= CAMERA_CLEAR_HEIGHT;
	}
}

#ifdef _DEBUG
//=============================================================================
// �f�o�b�O�Ŏg���ړ�����
//=============================================================================
void CCamera::DebugMove(void)
{
	// �L�[�{�[�h�擾����
	CKeyboard *pKeyboard = CManager::GetKeyboard();

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// �J������] (�y�L�[ / �b�L�[ �܂��� �p�b�h�E�X�e�B�b�N)
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (pKeyboard->GetPress(DIK_Z) == true && m_bRotate == false)
	{
		m_rot.y += CAMERA_MAX_ROTATE_SPEED;
		m_posV.x = m_posR.x + sinf(m_rot.y) * CAMERA_V_DISTANCE;
		m_posV.z = m_posR.z + cosf(m_rot.y) * CAMERA_V_DISTANCE;
	}
	else if (pKeyboard->GetPress(DIK_C) == true && m_bRotate == false)
	{
		m_rot.y -= CAMERA_MAX_ROTATE_SPEED;
		m_posV.x = m_posR.x + sinf(m_rot.y) * CAMERA_V_DISTANCE;
		m_posV.z = m_posR.z + cosf(m_rot.y) * CAMERA_V_DISTANCE;
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// �J�����ړ� (���L�[ / ���L�[ / ���L�[ / ���L�[)
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (pKeyboard->GetPress(DIK_LEFT) == true)
	{
		//�����ړ�
		if (pKeyboard->GetPress(DIK_UP) == true)
		{
			m_posR.x += +cosf(m_rot.y + D3DX_PI / 4.0f) * m_fSpeed;
			m_posR.z += -sinf(m_rot.y + D3DX_PI / 4.0f) * m_fSpeed;
		}
		//����O�ړ�
		else if (pKeyboard->GetPress(DIK_DOWN) == true)
		{
			m_posR.x += +cosf(m_rot.y - D3DX_PI / 4.0f) * m_fSpeed;
			m_posR.z += -sinf(m_rot.y - D3DX_PI / 4.0f) * m_fSpeed;
		}
		//���ړ�
		else
		{
			m_posR.x += +cosf(m_rot.y) * m_fSpeed;
			m_posR.z += -sinf(m_rot.y) * m_fSpeed;
		}
	}
	else if (pKeyboard->GetPress(DIK_RIGHT) == true)
	{
		//�E���ړ�
		if (pKeyboard->GetPress(DIK_UP) == true)
		{
			m_posR.x += -cosf(m_rot.y - D3DX_PI / 4.0f) * m_fSpeed;
			m_posR.z += +sinf(m_rot.y - D3DX_PI / 4.0f) * m_fSpeed;
		}
		//�E��O�ړ�
		else if (pKeyboard->GetPress(DIK_DOWN) == true)
		{
			m_posR.x += -cosf(m_rot.y + D3DX_PI / 4.0f) * m_fSpeed;
			m_posR.z += +sinf(m_rot.y + D3DX_PI / 4.0f) * m_fSpeed;
		}
		else	//�E�ړ�
		{
			m_posR.x += -cosf(m_rot.y) * m_fSpeed;
			m_posR.z += +sinf(m_rot.y) * m_fSpeed;
		}
	}
	//���ړ�
	else if (pKeyboard->GetPress(DIK_UP) == true)
	{
		m_posR.x += -sinf(m_rot.y) * m_fSpeed;
		m_posR.z += -cosf(m_rot.y) * m_fSpeed;
	}
	//��O�ړ�
	else if (pKeyboard->GetPress(DIK_DOWN) == true)
	{
		m_posR.x += sinf(m_rot.y) * m_fSpeed;
		m_posR.z += cosf(m_rot.y) * m_fSpeed;
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// �Y�[���C�� / �Y�[���A�E�g (U�L�[ / M�L�[)
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (pKeyboard->GetPress(DIK_U) == true)
	{
		if (m_fDistance_posVposR <= CAMERA_V_DISTANCE / 2)
		{
			m_fDistance_posVposR = CAMERA_V_DISTANCE / 2;
		}
		else
		{
			m_fDistance_posVposR -= 5.0f;

			if (m_fHeight <= CAMERA_HEIGHT - 100)
			{
				m_fHeight = CAMERA_HEIGHT - 100;
			}
			else
			{
				m_fHeight -= m_fSpeed;
			}
		}
	}
	else if (pKeyboard->GetPress(DIK_M) == true)
	{
		if (m_fDistance_posVposR >= CAMERA_V_DISTANCE * 4)
		{
			m_fDistance_posVposR = CAMERA_V_DISTANCE * 4;
		}
		else
		{
			m_fDistance_posVposR += 5.0f;

			if (m_fHeight >= CAMERA_HEIGHT * 11)
			{
				m_fHeight = CAMERA_HEIGHT * 11;
			}
			else
			{
				m_fHeight += m_fSpeed;
			}
		}
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// �㉺�ړ� (Y�L�[ / N�L�[)
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//���ړ�
	if (pKeyboard->GetPress(DIK_Y) == true)
	{
		m_posR.y -= m_fSpeed;
	}
	//��ړ�
	if (pKeyboard->GetPress(DIK_N) == true)
	{
		m_posR.y += m_fSpeed;
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// ���x�ύX (�EShift�L�[)
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (pKeyboard->GetTrigger(DIK_RSHIFT) == true)
	{
		m_fSpeed += 5.0f;

		if (m_fSpeed > 30.0f)
		{
			m_fSpeed = CAMERA_MOVE_SPEED;
		}
	}

	// �I�u�W�F�N�g��������|�C���^�E���̏���ۑ�����|�C���^
	CScene *pObject = NULL;
	CScene *pSaveObject = NULL;
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �擪�̃|�C���^����
	pObject = pObject->GetTopObj(CScene::PRIORITY_MODEL);

	while (pObject != NULL)
	{
		// ���݂̃I�u�W�F�N�g�̃|�C���^��ۑ�
		pSaveObject = pObject;
		if (pObject->GetObjType() == CScene::OBJECT_TYPE_PLAYER)
		{
			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// �v���C���[�̈ʒu�܂ňړ� (Enter�L�[)
			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			if (pKeyboard->GetTrigger(DIK_RETURN) == true)
			{
				// �v���C���[�N���X�ɃL���X�g
				CPlayer *pPlayer = NULL;
				pPlayer = (CPlayer*)pObject;

				// �v���C���[�̈ʒu���擾
				pos = pPlayer->GetPos();

				//�v���C���[�̈ʒu�����Ƃɒ����_�̈ʒu���ړ�
				m_posR.x = pos.x + sinf(m_rot.y) * -(m_fDistance_PlayerPosR);
				m_posR.z = pos.z + cosf(m_rot.y) * -(m_fDistance_PlayerPosR);
				m_posR.y = pos.y;

				//�����_�̈ʒu�����ƂɎ��_�̈ʒu���ړ�
				m_posV.x = m_posR.x + sinf(m_rot.y) * m_fDistance_posVposR;
				m_posV.z = m_posR.z + cosf(m_rot.y) * m_fDistance_posVposR;
				m_posV.y = m_posR.y + m_fHeight;
			}
		}

		// ���̃I�u�W�F�N�g���擾
		pObject = pSaveObject->GetObjNext(pSaveObject);
	}

	m_posV.x = m_posR.x + sinf(m_rot.y) * m_fDistance_posVposR;
	m_posV.z = m_posR.z + cosf(m_rot.y) * m_fDistance_posVposR;
	m_posV.y = m_posR.y + m_fHeight;
}
#endif