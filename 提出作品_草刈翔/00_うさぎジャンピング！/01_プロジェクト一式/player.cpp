//=============================================================================
// �v���C���[���� [player.cpp]
// Authore : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "input_keyboard.h"
#include "model.h"
#include "model_load.h"
#include "model_single.h"
#include "mesh_field.h"
#include "camera.h"
#include "motion_player.h"
#include "control_player.h"
#include "game.h"
#include "tutorial.h"
#include "solo_menu.h"
#include "shadow.h"
#include "particle_circle.h"
#include "sound.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPlayer::CPlayer(int nPriority) :CScene(nPriority)
{
	// �ϐ��̃N���A
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	for (int nCntModel = 0; nCntModel < MAX_PLAYER_MODEL; nCntModel++)
	{
		m_apModel[nCntModel] = NULL;
	}
	m_pParent = NULL;
	m_pControl = NULL;
	m_pShadow = NULL;
	m_pMotionPlayer = NULL;
	m_pParticle = NULL;
	m_bLand = false;
	m_bJumpOld = false;
	m_bLandFinish = false;
	m_bStop = true;
	m_bCpu = false;
	m_nNum = 0;
	m_nCpuCount = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �ϐ��̏�����
	m_pos = pos;
	m_posOld = pos;
	m_move = m_pControl->GetMove();
	m_bLand = false;
	m_bJump = true;
	m_bJumpOld = true;
	m_bLandFinish = false;
	m_bStop = true;
	m_nCpuCount = 0;

	// ���f���̐���
	ModelCreate(m_nNum);

	// �ʒu�̐ݒ�
	SetPos(m_pos);
	SetPosOld(m_posOld);

	// �I�u�W�F�N�g�̎�ނ̐ݒ�
	SetObjType(CScene::OBJECT_TYPE_PLAYER);

	// �T�C�Y���擾
	m_size = m_apModel[0]->GetSize();

	// �T�C�Y�̐ݒ�
	SetSize(m_size);

	// ���[�V�����̐���
	m_pMotionPlayer = CMotionPlayer::Create(this);

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
			if (m_nNum == pCamera->GetNum())
			{
				if (CManager::GetMode() == CManager::MODE_RESULT)
				{
					// �J�����̌������v���C���[�̌����Ɠ������Ȃ�悤�ݒ�
					pCamera->SetRotV(m_rot);
					// �J�����̒����_���v���C���[�̈ʒu�ɂȂ�悤�ݒ�
					pCamera->SetPosR(m_pos);
					// �J�����̎��_���v���C���[�̌��ɂȂ�悤�ݒ�
					pCamera->SetPosV(D3DXVECTOR3(m_pos.x + sinf(m_rot.y) * CAMERA_DISTANCE,
						CAMERA_HEIGHT,
						m_pos.z + cosf(m_rot.y) * CAMERA_DISTANCE));
				}
				else
				{
					// �J�����̌������v���C���[�̌����Ɠ������Ȃ�悤�ݒ�
					pCamera->SetRotV(m_rot);
					// �J�����̒����_���v���C���[�̈ʒu�ɂȂ�悤�ݒ�
					pCamera->SetPosR(m_pos);
					// �J�����̎��_���v���C���[�̌��ɂȂ�悤�ݒ�
					pCamera->SetPosV(D3DXVECTOR3(m_pos.x + sinf(m_rot.y) * CAMERA_DISTANCE,
						CAMERA_HEIGHT,
						m_pos.z + cosf(m_rot.y) * CAMERA_DISTANCE));
				}
			}
		}
	}

	// �`���[�g���A����ʂŐ���������ꍇ
	if (CManager::GetMode() == CManager::MODE_TUTORIAL && CTutorial::GetExplanation() == true)
	{
		// �v���C���[�̕�����������
		m_rot.y = 0.0f;
	}
	// ���U���g��ʂȂ�
	else if (CManager::GetMode() == CManager::MODE_RESULT)
	{
		// �v���C���[�̕�����������
		m_rot.y = 0.0f;
	}
	else
	{
		// �v���C���[���猩�ĉE�Ɍ�������
		m_rot.y = -D3DX_PI / 2;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPlayer::Uninit(void)
{
	//�e������
	if (m_pShadow != NULL)
	{
		m_pShadow->Uninit();
	}

	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CPlayer::Update(void)
{
	if (this != NULL)
	{
		// �T�E���h�擾����
		CSound *pSound;
		pSound = CManager::GetSound();

		// �ʒu�̎擾
		D3DXVECTOR3 pos = GetPos();
		m_pos = pos;
		m_posOld = pos;

		// 1�t���[���O�̈ʒu�ݒ�
		SetPosOld(m_posOld);

		// �O�̃t���[���ɃW�����v���Ă��邩�ǂ����̏�����
		m_bJumpOld = m_bJump;

		// �ړ�����
		Move();

		// �ړ��ʔ��f
		m_pos += m_move;

		// �ʒu���f
		SetPos(m_pos);

		// ���U���g��ʂ̎�
		if (CManager::GetMode() == CManager::MODE_RESULT == true)
		{
			// ���ꂪ�����������邽�߁A����0�ŌŒ�
			if (m_pos.y <= 0)
			{
				m_pos.y = 0.0f;
				SetPos(m_pos);

				m_bLand = true;
			}
		}
		else
		{
			// ���f���Ƃ̓����蔻��
			m_bLand = CModelSingle::Collision(this);

			if (m_bLand == false)
			{
				// �n�ʂƂ̓����蔻��
				m_bLand = CMeshField::Collision(this);
			}
		}
			
		if (m_bLand == true)
		{
			if (m_bJumpOld == true)
			{
				// �W�����v�֘A���[�V�����̎��Ȃ�
				if (m_pMotionPlayer->GetMotion() == CMotionPlayer::MOTION_PLAYER_TYPE_JUMP ||
					m_pMotionPlayer->GetMotion() == CMotionPlayer::MOTION_PLAYER_TYPE_JUMP_FALL ||
					m_pMotionPlayer->GetMotion() == CMotionPlayer::MOTION_PLAYER_TYPE_DOUBLE_JUMP)
				{
					//���n���[�V�����ɂ���
					m_pMotionPlayer->SetMotion(CMotionPlayer::MOTION_PLAYER_TYPE_LANDING);

					// �ʒu�����ɖ��܂����ہA�p�[�e�B�N���������Ȃ��Ȃ邱�Ƃ�h��
					D3DXVECTOR3 pos = m_pos;
					if (pos.y <= 0.0f)
					{
						pos.y = 0.0f;
					}

					// �p�[�e�B�N���̐���
					m_pParticle = CParticleCircle::Create(pos, D3DXVECTOR3(15.0f, 5.0f, 0.0f),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 100), 15, 3.0f);

					m_pParticle = CParticleCircle::Create(pos, D3DXVECTOR3(15.0f, 5.0f, 0.0f),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 100), 15, 2.0f);

					// SE�̍Đ�
					pSound->Play(CSound::SOUND_LABEL_LANDING);
					pSound->ControllVoice(CSound::SOUND_LABEL_LANDING, 0.4f);

					m_bJump = false;
					m_move.y = 0.0f;
				}
			}

			// �L�^�n�_(x = 0�ȍ~)�ɒ��n������
			if (m_pos.x > 0)
			{
				// �v���I���ɂ���(�Q�[����ʂŌ��ʂ��擾���čX�ɏ���������)
				m_bLandFinish = true;
			}
		}

		// ���[�V����
		m_pMotionPlayer->Update(this);

		// �e�̒Ǐ]
		ShadowCollision();

		// �ʒu�擾
		m_pos = GetPos();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CPlayer::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	// �v���C���[�̃��[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �v���C���[�̌����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �v���C���[�̈ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���f���̕`��
	for (int nCntModel = 0; nCntModel < MAX_PLAYER_MODEL; nCntModel++)
	{
		m_apModel[nCntModel]->Draw();
	}

}

//=============================================================================
// ��������
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nNumPlayer, bool bCpu)
{
	// �C���X�^���X�̐���
	CPlayer *pPlayer = NULL;

	// �k���`�F�b�N
	if (pPlayer == NULL)
	{
		// �N���X�̐���
		pPlayer = new CPlayer;
		if (pPlayer != NULL)
		{
			// �e�̐���
			pPlayer->m_pShadow = CShadow::Create(D3DXVECTOR3(pos.x, 0.0f, pos.z), D3DXVECTOR3(40.0f, 0.0f, 40.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			
			// �v���C���[����̃N���X�𐶐�
			pPlayer->m_pControl = CControlPlayer::Create();

			// �ϐ��̏�����
			pPlayer->m_nNum = nNumPlayer;
			pPlayer->m_rot = rot;
			pPlayer->m_bCpu = bCpu;

			// ����������
			pPlayer->Init(pos, pPlayer->m_size);
		}
	}

	return pPlayer;
}


//=============================================================================
// ���f����������
//=============================================================================
void CPlayer::ModelCreate(int nNum)
{
	// �e�L�X�g�ۑ��p�̕ϐ�
	char cString[MAX_STRING];

	// �e�L�X�g�t�@�C���̓ǂݍ���
	FILE *pFile = fopen("data/TEXT/motion_player.txt", "r");

	if (pFile != NULL)
	{
		// ���f���̎��
		int nModelType = 0;

		if (nNum == 0)
		{
			nModelType = (int)CModelLoad::MODEL_TYPE_RABBIT_BODY;
		}
		else if (nNum == 1)
		{
			nModelType = (int)CModelLoad::MODEL_TYPE_RABBIT_2_BODY;
		}

		// �z�u���̓ǂݍ���
		for (int nCntMotion = 0; nCntMotion < MAX_PLAYER_MODEL; nCntMotion++, nModelType++)
		{
			// �e�L�X�g�t�@�C���̍Ō�(NULL)�܂œǂݍ���
			while (fgets(cString, MAX_STRING, pFile) != NULL)
			{
				// �������ۑ�
				fscanf(pFile, "%s", cString);

				// ������̒���"PARTSSET"����������
				if (strncmp("PARTSSET", cString, 9) == 0)
				{
					break;
				}
			}

			while (fgets(cString, MAX_STRING, pFile) != NULL)
			{
				// �������ۑ�
				fscanf(pFile, "%s", cString);

				// ������̒���"INDEX"����������
				if (strncmp("INDEX", cString, 6) == 0)
				{
					int nIdx, nParents;
					D3DXVECTOR3 pos, rot;

					//�C���f�b�N�X�ԍ��Ɛe�q�t���̔ԍ����擾
					fscanf(pFile, "%s%d%*s%*s", cString, &nIdx);
					fscanf(pFile, "%s%*s%d%*s%*s%*s", cString, &nParents);

					//�ʒu�E��]�����擾
					fscanf(pFile, "%s%f%f%f", cString, &pos.x, &pos.y, &pos.z);
					fscanf(pFile, "%s%*s%f%f%f", cString, &rot.x, &rot.y, &rot.z);

					// ���f���̐���
					m_apModel[nCntMotion] = CModel::Create(pos, rot, (CModelLoad::MODEL_TYPE)(nModelType));

					// �e�̐ݒ�(-1���w�肳��Ă����ꍇ�A�e�q�t���͖����Ƃ���NULL�Őݒ肵�Ă���)
					if (nParents == -1)
					{
						m_apModel[nCntMotion]->SetParent(NULL);
					}
					else
					{
						m_apModel[nCntMotion]->SetParent(m_apModel[nParents]);
					}

					break;
				}
			}

			while (fgets(cString, MAX_STRING, pFile) != NULL)
			{
				// �������ۑ�
				fscanf(pFile, "%s", cString);

				// ������̒���"END_PARTSSET"���������炻���ŏI��
				if (strncmp("END_PARTSSET", cString, 13) == 0)
				{
					break;
				}
			}
		}
		// �t�@�C�������
		fclose(pFile);
	}
}

//=============================================================================
// �ړ�����
//=============================================================================
void CPlayer::Move(void)
{
	// �k���`�F�b�N
	if (m_pControl != NULL)
	{
		// �v���C���[����̃N���X�Ƀv���C���[�̃|�C���^�����A�ړ��ʂ��擾
		m_pControl->Update(this);

		if (m_bCpu == true)
		{
			CpuMove();
		}

		m_move = m_pControl->GetMove();
	}
}

//=============================================================================
// �����蔻�菈��
//=============================================================================
bool CPlayer::Collision(void)
{
	return false;
}

//=============================================================================
// 1�t���[���O�̈ʒu�擾����
//=============================================================================
D3DXVECTOR3 CPlayer::GetPosOld(void)
{
	return m_posOld;
}

//=============================================================================
// �����ݒ菈��
//=============================================================================
void CPlayer::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//=============================================================================
// �����擾����
//=============================================================================
D3DXVECTOR3 CPlayer::GetRot(void)
{
	return m_rot;
}

//=============================================================================
// �ړ��ʐݒ菈��
//=============================================================================
void CPlayer::SetMove(D3DXVECTOR3 move)
{
	m_move = move;
}

//=============================================================================
// �ړ��ʎ擾����
//=============================================================================
D3DXVECTOR3 CPlayer::GetMove(void)
{
	return m_move;
}

//=============================================================================
// �W�����v�ݒ菈��
//=============================================================================
void CPlayer::SetJump(bool bJump)
{
	m_bJump = bJump;
}

//=============================================================================
// �W�����v�擾����
//=============================================================================
bool CPlayer::GetJump(void)
{
	return m_bJump;
}

//=============================================================================
// ���n�ݒ菈��
//=============================================================================
void CPlayer::SetLand(bool bLand)
{
	m_bLand = bLand;
}

//=============================================================================
// ���n�擾����
//=============================================================================
bool CPlayer::GetLand(void)
{
	return m_bLand;
}

//=============================================================================
// ���n�Ńt�B�j�b�V���������ǂ����ݒ菈��
//=============================================================================
void CPlayer::SetLandFinish(bool bLandFinish)
{
	m_bLandFinish = bLandFinish;
}

//=============================================================================
// ���n�Ńt�B�j�b�V���������ǂ����擾����
//=============================================================================
bool CPlayer::GetLandFinish(void)
{
	return m_bLandFinish;
}

//=============================================================================
// �~�܂��Ă��邩�ǂ����ݒ菈��
//=============================================================================
void CPlayer::SetStop(bool bStop)
{
	m_bStop = bStop;
}

//=============================================================================
// �~�܂��Ă��邩�ǂ����ݒ菈��
//=============================================================================
bool CPlayer::GetStop(void)
{
	return m_bStop;
}

//=============================================================================
// ���f�����̈ʒu�ݒ菈��
//=============================================================================
void CPlayer::SetModelPos(int nCntModel, D3DXVECTOR3 pos)
{
	m_apModel[nCntModel]->SetPos(pos);
}

//=============================================================================
// ���f�����̈ʒu�擾����
//=============================================================================
D3DXVECTOR3 CPlayer::GetModelPos(int nCntModel)
{
	return m_apModel[nCntModel]->GetPos();
}

//=============================================================================
// ���f�����̌����ݒ菈��
//=============================================================================
void CPlayer::SetModelRot(int nCntModel, D3DXVECTOR3 rot)
{
	m_apModel[nCntModel]->SetRot(rot);
}

//=============================================================================
// ���f�����̌����擾����
//=============================================================================
D3DXVECTOR3 CPlayer::GetModelRot(int nCntModel)
{
	return m_apModel[nCntModel]->GetRot();
}

//=============================================================================
// ���[�V�����v���C���[�擾����
//=============================================================================
CMotionPlayer *CPlayer::GetMotionPlayer(void)
{
	return m_pMotionPlayer;
}

//=============================================================================
// �I�u�W�F�N�g�Ƃ̓����蔻��
//=============================================================================
void CPlayer::Collision(CScene *pScene)
{
	//�I�u�W�F�N�g�̈ʒu���擾
	D3DXVECTOR3 posObj = pScene->GetPos();
	//�I�u�W�F�N�g��1�t���[���O�̈ʒu���擾
	D3DXVECTOR3 posObjOld = pScene->GetPosOld();
	//�I�u�W�F�N�g�̃T�C�Y���擾
	D3DXVECTOR3 sizeObj = pScene->GetSize();

	if (posObj.x + sizeObj.x / 2.0f > m_pos.x - m_size.x / 2.0f &&
		posObj.x - sizeObj.x / 2.0f < m_pos.x + m_size.x / 2.0f &&
		posObj.z + sizeObj.z / 2.0f > m_pos.z - m_size.x / 2.0f &&
		posObj.z - sizeObj.z / 2.0f < m_pos.z + m_size.x / 2.0f &&
		posObj.y + sizeObj.y >= m_pos.y&&
		posObjOld.y + sizeObj.y <= m_pos.y)
	{
		m_pos.y = posObj.y + sizeObj.y;
		//�ʒu��ݒ�
		SetPos(m_pos);
		//1�t���[���O�̈ʒu��ݒ�
		SetPosOld(m_pos);
	}
	else if (posObj.x + sizeObj.x / 2.0f > m_pos.x - m_size.x / 2.0f &&
		posObj.x - sizeObj.x / 2.0f < m_pos.x + m_size.x / 2.0f &&
		posObj.z + sizeObj.z / 2.0f > m_pos.z - m_size.x / 2.0f &&
		posObj.z - sizeObj.z / 2.0f < m_pos.z + m_size.x / 2.0f &&
		posObj.y <= m_pos.y + m_size.y &&
		posObjOld.y >= m_pos.y + m_size.y)
	{
		m_pos.y = posObj.y - m_size.y;
		//�ʒu��ݒ�
		SetPos(m_pos);
		//1�t���[���O�̈ʒu��ݒ�
		SetPosOld(m_pos);
	}
	if (posObj.y + sizeObj.y < m_pos.y &&
		posObj.y > m_pos.y + m_size.y &&
		posObj.z + sizeObj.z / 2.0f > m_pos.z - m_size.x / 2.0f &&
		posObj.z - sizeObj.z / 2.0f < m_pos.z + m_size.x / 2.0f &&
		posObj.x + sizeObj.x / 2.0f <= m_pos.x - m_size.x / 2.0f &&
		posObjOld.x + sizeObj.x / 2.0f >= m_pos.x - m_size.x / 2.0f)
	{
		m_pos.x = posObj.x + sizeObj.x / 2.0f - m_size.x / 2.0f;
		//�ʒu��ݒ�
		SetPos(m_pos);
		//1�t���[���O�̈ʒu��ݒ�
		SetPosOld(m_pos);
	}
	else if (posObj.y + sizeObj.y < m_pos.y &&
		posObj.y > m_pos.y + m_size.y &&
		posObj.z + sizeObj.z / 2.0f > m_pos.z - m_size.x / 2.0f &&
		posObj.z - sizeObj.z / 2.0f < m_pos.z + m_size.x / 2.0f &&
		posObj.x - sizeObj.x / 2.0f >= m_pos.x + m_size.x / 2.0f &&
		posObjOld.x - sizeObj.x / 2.0f <= m_pos.x + m_size.x / 2.0f)
	{
		m_pos.x = posObj.x - sizeObj.x / 2.0f + m_size.x / 2.0f;
		//�ʒu��ݒ�
		SetPos(m_pos);
		//1�t���[���O�̈ʒu��ݒ�
		SetPosOld(m_pos);
	}
	if (posObj.y + sizeObj.y < m_pos.y &&
		posObj.y > m_pos.y + m_size.y &&
		posObj.x + sizeObj.x / 2.0f > m_pos.x - m_size.x / 2.0f &&
		posObj.x - sizeObj.x / 2.0f < m_pos.x + m_size.x / 2.0f &&
		posObj.z + sizeObj.z / 2.0f <= m_pos.z - m_size.x / 2.0f &&
		posObjOld.z + sizeObj.z / 2.0f >= m_pos.z - m_size.x / 2.0f)
	{
		m_pos.z = posObj.z - sizeObj.z / 2.0f + m_size.x / 2.0f;
		//�ʒu��ݒ�
		SetPos(m_pos);
		//1�t���[���O�̈ʒu��ݒ�
		SetPosOld(m_pos);
	}
	else if (posObj.y + sizeObj.y < m_pos.y &&
		posObj.y > m_pos.y + m_size.y &&
		posObj.x + sizeObj.x / 2.0f > m_pos.x - m_size.x / 2.0f &&
		posObj.x - sizeObj.x / 2.0f < m_pos.x + m_size.x / 2.0f &&
		posObj.z - sizeObj.z / 2.0f >= m_pos.z + m_size.x / 2.0f &&
		posObjOld.z - sizeObj.z / 2.0f <= m_pos.z + m_size.x / 2.0f)
	{
		m_pos.z = posObj.z + sizeObj.z / 2.0f - m_size.x / 2.0f;
		//�ʒu��ݒ�
		SetPos(m_pos);
		//1�t���[���O�̈ʒu��ݒ�
		SetPosOld(m_pos);
	}
}

//=============================================================================
// �e�̕\���̂��߂̃I�u�W�F�N�g�Ƃ̓����蔻��
//=============================================================================
void CPlayer::ShadowCollision(void)
{
	//�I�u�W�F�N�g��������|�C���^
	CScene *pObject = NULL;
	//�I�u�W�F�N�g����ۑ�����|�C���^�ϐ�
	CScene *pSaveObject = NULL;

	//�擪�̃|�C���^����
	pObject = pObject->GetTopObj(CScene::PRIORITY_MODEL);

	while (pObject != NULL)
	{
		//���݂̃I�u�W�F�N�g�̃|�C���^��ۑ�
		pSaveObject = pObject;

		if (pObject->GetObjType() == CScene::OBJECT_TYPE_MODEL)
		{
			// ���f���V���O���N���X�ɃL���X�g
			CModelSingle *pModelSingle = NULL;
			pModelSingle = (CModelSingle*)pObject;

			D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ۑ��p�̈ʒu
			D3DXVECTOR3 posObj = pModelSingle->GetPos();		// �I�u�W�F�N�g�̈ʒu���擾
			D3DXVECTOR3 posObjOld = pModelSingle->GetPosOld();	// �I�u�W�F�N�g��1�t���[���O�̈ʒu���擾
			D3DXVECTOR3 sizeObj = pModelSingle->GetSize();		// �I�u�W�F�N�g�̃T�C�Y���擾

			if (posObj.x + sizeObj.x / 2.0f > m_pos.x - m_size.x / 2.0f &&
				posObj.x - sizeObj.x / 2.0f < m_pos.x + m_size.x / 2.0f &&
				posObj.z + sizeObj.z / 2.0f > m_pos.z - m_size.x / 2.0f &&
				posObj.z - sizeObj.z / 2.0f < m_pos.z + m_size.x / 2.0f &&
				posObj.y + sizeObj.y >= m_pos.y - 50.0f &&
				posObj.y + sizeObj.y <= m_pos.y)
			{
				//�ʒu�����f���̏��
				pos.y = posObj.y + sizeObj.y;

				//�e�̒Ǐ]
				m_pShadow->SetShadow(D3DXVECTOR3(m_pos.x, pos.y, m_pos.z), (m_pos.y - pos.y));

				//�e���g���Ă����Ԃɂ���
				m_pShadow->SetUseShadow(true);

				return;
			}
			else
			{
				//�e���g���Ă��Ȃ���Ԃɂ���
				m_pShadow->SetUseShadow(false);
			}
		}

		pObject = pSaveObject->GetObjNext(pSaveObject);
	}

	//�擪�̃|�C���^����
	pObject = pObject->GetTopObj(CScene::PRIORITY_NONE);

	while (pObject != NULL)
	{
		//���݂̃I�u�W�F�N�g�̃|�C���^��ۑ�
		pSaveObject = pObject;

		if (pObject->GetObjType() == CScene::OBJECT_TYPE_MODEL)
		{
			// ���b�V���t�B�[���h�N���X�ɃL���X�g
			CMeshField *pMeshField = NULL;
			pMeshField = (CMeshField*)pObject;

			D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ۑ��p�̈ʒu
			D3DXVECTOR3 posObj = pMeshField->GetPos();			// �I�u�W�F�N�g�̈ʒu���擾
			D3DXVECTOR3 posObjOld = pMeshField->GetPosOld();	// �I�u�W�F�N�g��1�t���[���O�̈ʒu���擾
			D3DXVECTOR3 sizeObj = pMeshField->GetSize();		// �I�u�W�F�N�g�̃T�C�Y���擾

			if (posObj.x + sizeObj.x / 2.0f > m_pos.x - m_size.x / 2.0f &&
				posObj.x - sizeObj.x / 2.0f < m_pos.x + m_size.x / 2.0f &&
				posObj.z + sizeObj.z / 2.0f > m_pos.z - m_size.x / 2.0f &&
				posObj.z - sizeObj.z / 2.0f < m_pos.z + m_size.x / 2.0f &&
				posObj.y + sizeObj.y >= m_pos.y - 500.0f &&
				posObj.y + sizeObj.y <= m_pos.y + 10.0f)
			{
				//�ʒu���t�B�[���h�̏��
				pos.y = posObj.y * 2;

				//�e�̒Ǐ]
				m_pShadow->SetShadow(D3DXVECTOR3(m_pos.x, pos.y, m_pos.z), (m_pos.y - pos.y));

				//�e���g���Ă����Ԃɂ���
				m_pShadow->SetUseShadow(true);

				return;
			}
			else
			{
				//�e���g���Ă��Ȃ���Ԃɂ���
				m_pShadow->SetUseShadow(false);
			}
		}

		pObject = pSaveObject->GetObjNext(pSaveObject);
	}
}

//=============================================================================
// �v���C���[�̔ԍ��ݒ菈��
//=============================================================================
void CPlayer::SetNum(int nNum)
{
	m_nNum = nNum;
}

//=============================================================================
// �v���C���[�̔ԍ��擾����
//=============================================================================
int CPlayer::GetNum(void)
{
	return m_nNum;
}

//=============================================================================
// �v���C���[�̑���ݒ菈��
//=============================================================================
void CPlayer::SetCommand(PLAYER_COMMAND command)
{
	m_command = command;
}

//=============================================================================
// �v���C���[�̑���擾����
//=============================================================================
CPlayer::PLAYER_COMMAND CPlayer::GetCommand(void)
{
	return m_command;
}

//=============================================================================
// CPU���ǂ����擾����
//=============================================================================
bool CPlayer::GetCpu(void)
{
	return m_bCpu;
}

//=============================================================================
// CPU���쏈��
//=============================================================================
void CPlayer::CpuMove(void)
{
	// ��Փx(�l)���擾
	int nDifficulty = CSoloMenu::GetDifficulty();
	int nRandom = 0;

	// �X�^�[�g����`���ؒ��O�܂�
	if (m_pos.x > GAME_PLAYER_START &&  m_pos.x < -50)
	{
		// ��킢
		if (nDifficulty == 0)
		{
			// ������15�`50�̒l���烉���_���ɐݒ�
			nRandom = rand() % 36 + 15;
		}
		// �ӂ�
		else if (nDifficulty == 1)
		{
			// ������5�`30�̒l���烉���_���ɐݒ�
			nRandom = rand() % 26 + 5;
		}
		// �悢
		else if (nDifficulty == 2)
		{
			// ������1�`20�̒l���烉���_���ɐݒ�
			nRandom = rand() % 20 + 1;
		}
		// ������
		else if (nDifficulty == 3)
		{
			// ������1�`15�̒l���烉���_���ɐݒ�
			nRandom = rand() % 15 + 1;
		}

		m_nCpuCount++;

		// �����_���Ō��߂��������J�E���g����������
		if (m_nCpuCount > nRandom)
		{
			// �_�b�V��������
			SetCommand(CPlayer::PLAYER_COMMAND_DASH);

			m_nCpuCount = 0;
		}
	}
	// ���ؒ��O
	if (m_pos.x > -50.0f && m_pos.x < 0.0f)
	{
		// �W�����v������
		SetCommand(CPlayer::PLAYER_COMMAND_JUMP);

		m_nCpuCount = 0;
	}

	// �W�����v��
	if (m_pos.x > 0)
	{
		m_nCpuCount++;

		// ��킢
		if (nDifficulty == 0)
		{
			// ������15�`18�̒l���烉���_���ɐݒ�
			nRandom = rand() % 4 + 15;
		}
		// �ӂ�
		else if (nDifficulty == 1)
		{
			// ������10�`12�̒l���烉���_���ɐݒ�
			nRandom = rand() % 3 + 10;
		}
		// �悢�E������
		else if (nDifficulty == 2 || nDifficulty == 3)
		{
			// ������8�`9�̒l���烉���_���ɐݒ�
			nRandom = rand() % 2 + 8;
		}

		// �ړ��ʂ�0������������A�܂��W�����v���[�V����(2�i�W�����v�����Ă��Ȃ�)��������
		if (m_move.y < -1.0f && GetMotionPlayer()->GetMotion() == CMotionPlayer::MOTION_PLAYER_TYPE_JUMP)
		{
			// 2�i�W�����v������
			SetCommand(CPlayer::PLAYER_COMMAND_DOUBLE_JUMP);
		}

		if (m_nCpuCount > nRandom && GetMotionPlayer()->GetMotion() == CMotionPlayer::MOTION_PLAYER_TYPE_DOUBLE_JUMP)
		{
			// �z�o�����O������
			SetCommand(CPlayer::PLAYER_COMMAND_HOVERING);

			m_nCpuCount = 0;
		}
	}
}