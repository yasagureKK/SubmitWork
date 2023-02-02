//=============================================================================
// �v���C���[���� [player.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "model.h"
#include "model_single.h"
#include "mesh_field.h"
#include "mesh_wall.h"
#include "camera.h"
#include "motion.h"
#include "control_player.h"
#include "shadow.h"
#include "sound.h"
#include "collision_sphere.h"
#include "life.h"
#include "level.h"
#include "level_up.h"
#include "enemy.h"
#include "flash.h"
#include "death.h"
#include "pause.h"
#include "weapon.h"

//*****************************************************************************
//�}�N����`
//*****************************************************************************
#define PLAYER_BEGIN_LIFE	(200.0f)	// �������C�t
#define INVINCIBLE_TIME		(120)		// ���G����
#define LIFE_SIZE_X			(250.0f)	// ���C�t�̃T�C�YX
#define LIFE_SIZE_Y			(20.0f)		// ���C�t�̃T�C�YY
#define LEVEL_SIZE_Y		(10.0f)		// ���x���̃T�C�YY

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPlayer::CPlayer(int nPriority) :CScene3D(nPriority)
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
	m_pMotion = NULL;
	m_pControl = NULL;
	m_pShadow = NULL;
	m_pCollisionS = NULL;
	m_pLife = NULL;
	m_pLevel = NULL;
	m_fLife = 0.0f;
	m_fMaxLife = 0.0f;
	m_nExp = 0;
	m_nCntInvincible = 0;
	m_bLand = false;
	m_bLandOld = false;
	m_bJump = false;
	m_bWeaponChange = false;
	m_bInvincible = false;
	m_bCanControl = false;
	m_bDeath = false;
	m_state = PLAYER_STATE_NORMAL;
	m_weapon = PLAYER_WEAPON_AXE;
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
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// �ϐ��̏�����
	m_rot = rot;
	m_pos = pos;
	m_posOld = pos;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLife = PLAYER_BEGIN_LIFE;
	m_fMaxLife = PLAYER_BEGIN_LIFE;
	m_nExp = 0;
	m_nCntInvincible = 0;
	m_bLand = false;
	m_bLandOld = false;
	m_bJump = false;
	m_bWeaponChange = false;
	m_bInvincible = false;
	m_bCanControl = false;
	m_bDeath = false;
	m_state = PLAYER_STATE_NORMAL;
	m_weapon = PLAYER_WEAPON_AXE;

	// ���f����������
	ModelCreate();

	// �ʒu�̐ݒ�
	SetPos(m_pos);

	// ���̃p�[�c�̃T�C�Y���擾
	m_size = m_apModel[2]->GetSize();

	if (m_size.x > m_size.z)
	{
		m_size.z = m_size.x;
	}
	else if (m_size.z > m_size.x)
	{
		m_size.x = m_size.z;
	}

	D3DXVECTOR3 modelPos = m_apModel[0]->GetPos();
	m_size.y = modelPos.y + m_size.y * 2;

	// ���[�V�����̐���
	//��1�t���[�������s���|�[�Y�ɂȂ邽�߁A�X�V����������
	m_pMotion = CMotion::Create(this, CMotion::MOTION_TYPE_PLAYER);
	m_pMotion->Update(this);

	// �̂ɃR���W����(�v���C���[����)������
	m_pCollisionS = CCollisionSphere::Create(m_pos, m_size.x * 1.5f, CCollisionSphere::COLLISION_S_TYPE::COLLISION_S_TYPE_PLAYER, -1.0f);

	// ���C�t�𐶐����A���݂̃��C�t�ƍő僉�C�t��ݒ�
	m_pLife = CLife::Create(D3DXVECTOR2(150.0f, 30.0f), D3DXVECTOR2(LIFE_SIZE_X, LIFE_SIZE_Y), CLife::LIFE_TYPE_PLAYER);
	m_pLife->SetLife(m_fLife);
	m_pLife->SetMaxLife(m_fMaxLife);

	// ���x���𐶐�
	m_pLevel = CLevel::Create(D3DXVECTOR2(150.0f, 60.0f), D3DXVECTOR2(LIFE_SIZE_X, LEVEL_SIZE_Y));

	// ����UI�𐶐�
	m_pWepaon = CWeapon::Create(D3DXVECTOR2(0.0f, SCREEN_HEIGHT));

	// �e�𐶐����A�e���g�p���Ă����Ԃɐݒ�
	m_pShadow = CShadow::Create(D3DXVECTOR3(m_pos.x, m_pos.y - m_size.y, m_pos.z), D3DXVECTOR3(m_size.x * 2, 0.0f, m_size.x * 2));
	m_pShadow->SetUseShadow(true);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPlayer::Uninit(void)
{
	// �e�̏I������
	if (m_pShadow != NULL)
	{
		m_pShadow->Uninit();
	}

	// �R���g���[���̏I������
	if (m_pControl != NULL)
	{
		m_pControl->Uninit();
		m_pControl = NULL;
	}

	// ���[�V�����̔j��
	if (m_pMotion != NULL)
	{
		m_pMotion->Uninit();
	}

	// ���f���̏I������
	for (int nCntModel = 0; nCntModel < MAX_PLAYER_MODEL; nCntModel++)
	{
		if (m_apModel[nCntModel] != NULL)
		{
			m_apModel[nCntModel]->Uninit();
		}
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
		CSound *pSound = CManager::GetSound();

		// �e�̒Ǐ]
		TrackingShadow();

		// �ʒu�̎擾
		D3DXVECTOR3 pos = GetPos();
		m_pos = pos;
		m_posOld = m_pos;

		// 1�t���[���O�̈ʒu�ݒ�
		SetPosOld(m_posOld);

		// �O�̃t���[���ɒ��n���Ă��邩�ǂ����̏�����
		m_bLandOld = m_bLand;

		// �ړ�����
		Move();

		// �ړ��ʔ��f
		m_pos += m_move;

		// �ʒu���f
		SetPos(m_pos);

		// �G�Ƃ̉����o������
		CollisionEnemy();

		// ���f���Ƃ̓����蔻��
		m_bLand = CModelSingle::Collision(this);

		// �ǂƂ̓����蔻��
		CMeshWall::Collision(this);

		if (m_bLand == false)
		{
			// �n�ʂƂ̓����蔻��
			m_bLand = CMeshField::Collision(this);
		}

		// ���n��
		if (m_bLand == true)
		{
			// ���n������Y�����̈ړ��ʂ�0��
			m_move.y = 0.0f;

			if (m_bDeath == false)
			{
				if (m_bLandOld == true)
				{
					// �W�����v���[�V�������A���[�V�������Ȃ��Ă��Ȃ�������
					if (m_pMotion->GetMotion() == 2 && GetMotion()->GetConnect() == false)
					{
						// ����(2)SE�̍Đ�
						pSound->Play(6);
						pSound->ControllVoice(6, 0.3f);

						// ���n���[�V�����ɂ���
						m_pMotion->SetMotion(3);
					}

					m_bJump = false;
				}
			}
		}

		if (m_bDeath == false)
		{
			// ���n���Ă��Ȃ����A���[�V�������Ȃ��Ă��Ȃ�������
			if (GetLand() == false && GetMotion()->GetConnect() == false)
			{
				if (GetMotion()->GetMotion() != 2 &&
					GetMotion()->GetMotion() != 4 &&
					GetMotion()->GetMotion() != 5 &&
					GetMotion()->GetMotion() != 6 &&
					GetMotion()->GetMotion() != 7 &&
					GetMotion()->GetMotion() != 8 &&
					GetMotion()->GetMotion() != 9)
				{
					// �W�����v���[�V�����ɂ���(�����̕\��)
					GetMotion()->SetMotion(2);
				}
			}
		}

		// ���[�V����
		m_pMotion->Update(this);

		// �ʒu�擾
		m_pos = GetPos();

		if (m_pCollisionS != NULL)
		{
			// ���̃R���W������Ǐ]������
			D3DXVECTOR3 modelPos = m_apModel[0]->GetMtxPos();
			m_pCollisionS->SetPosCollision(modelPos);
		}

		// ���̃R���W�����ƐڐG�������̏���
		TouchCollision();

		if (m_pLife != NULL)
		{
			// ���x�����オ�����u��
			if (m_pLevel != NULL && m_pLevel->GetMoment() == true)
			{
				m_fLife += m_fMaxLife / 5;

				if (m_fLife > m_fMaxLife)
				{
					m_fLife = m_fMaxLife;
				}
			}

			// UI�Ɍ��݂̃��C�t�E�ő僉�C�t��ݒ�
			m_pLife->SetLife(m_fLife);
			m_pLife->SetMaxLife(m_fMaxLife);
		}

		if (m_pLevel != NULL)
		{
			// UI�Ɍ��݂̌o���l��ݒ�
			m_pLevel->SetExp(m_nExp);		
		}

		// ����ύX���̏���
		ChangeWeapon();

		// ���C�t���Ȃ��Ȃ�����
		if (m_fLife <= 0.0f)
		{
			m_fLife = 0.0f;

			// �R���W�����̔��������
			m_pCollisionS->SetCollisionType(CCollisionSphere::COLLISION_S_TYPE_NONE);

			if (m_bDeath == false)
			{
				// ���S�����true�ɂ���
				m_bDeath = true;

				// �J�����𑀍�s�\�ɂ���
				CCamera::SetControl(false);

				// �|�[�Y��s�\�ɂ���
				CPause::SetPossiblePause(false);

				// �Q�[���I�[�o�[��ʂ𐶐�
				CDeath::Create();
			}
		}

		if (m_pCollisionS != NULL)
		{
			// ���G���ȂǁA�v���C���[�̏�ԏ���
			State();
		}

		// �N���A�̏�ԂɂȂ�����
		if (CGame::GetClear() == true)
		{
			// �����s�\�ɂ���
			m_bCanControl = false;

			// �|�[�Y��s�\�ɂ���
			CPause::SetPossiblePause(false);

			// ���C�t��UI������
			if (m_pLife != NULL)
			{
				m_pLife->Uninit();
				delete m_pLife;
				m_pLife = NULL;
			}

			// ���x����UI������
			if (m_pLevel != NULL)
			{
				m_pLevel->Uninit();
				delete m_pLevel;
				m_pLevel = NULL;
			}

			// �����UI������
			if (m_pWepaon != NULL)
			{
				m_pWepaon->Uninit();
				delete m_pWepaon;
				m_pWepaon = NULL;
			}
		}
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

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
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
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// �N���X�̐���
	CPlayer *pPlayer = NULL;
	pPlayer = new CPlayer;

	if (pPlayer != NULL)
	{
		// �v���C���[����̃N���X�𐶐�
		pPlayer->m_pControl = CControlPlayer::Create();

		// ����������
		pPlayer->Init(pos, rot);
	}
	return pPlayer;
}

//=============================================================================
// ���f����������
//=============================================================================
void CPlayer::ModelCreate(void)
{
	// �e�L�X�g�t�@�C���̓ǂݍ���
	char cString[256];
	FILE *pFile = fopen("data/TEXT/motion_player.txt", "r");

	if (pFile != NULL)
	{
		int nCntModel = 0;
		char cFileName[MAX_PLAYER_MODEL][128];

		while (1)
		{
			fscanf(pFile, "%s", &cString[0]);

			// ���f���̃p�X���擾
			if (strcmp(&cString[0], "MODEL_FILENAME") == 0)
			{
				fscanf(pFile, "%s", &cString[0]);
				fscanf(pFile, "%s", &cFileName[nCntModel][0]);
				nCntModel++;
			}

			// ���f�����ő吔�ɂȂ�����p�X�̓ǂݍ��݂��I��
			if (nCntModel >= MAX_PLAYER_MODEL)
			{
				nCntModel = 0;
				break;
			}
		}

		int nIdx = 0, nParents = 0;
		D3DXVECTOR3 pos, rot;

		while (1)
		{
			fscanf(pFile, "%s", &cString[0]);

			if (strcmp(&cString[0], "PARTSSET") == 0)	//PARTSSET�̕�����
			{
				// ���f���̃p�[�c��������
				while (1)
				{
					fscanf(pFile, "%s", &cString[0]);

					if (strcmp(&cString[0], "INDEX") == 0) //�C���f�b�N�X�ԍ�
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%d", &nIdx);
					}
					if (strcmp(&cString[0], "PARENT") == 0) //�e�̃��f��
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%d", &nParents);
					}
					if (strcmp(&cString[0], "POS") == 0) //�ʒu
					{
						// �ʒu���擾����
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%f%f%f", &pos.x, &pos.y, &pos.z);
					}
					if (strcmp(&cString[0], "ROT") == 0) //����
					{
						// �������擾����
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%f%f%f", &rot.x, &rot.y, &rot.z);
					}
					if (strcmp(&cString[0], "END_PARTSSET") == 0)
					{
						break;
					}
				}

				// ���f���𐶐����A�����ƈʒu��ݒ�
				m_apModel[nCntModel] = CModel::Create(pos, rot, &cFileName[nCntModel][0]);

				// �e���f����ݒ�
				if (nParents == -1)
				{
					// -1 �Ȃ�e���f������
					m_apModel[nCntModel]->SetParent(NULL);
				}
				else
				{
					// -1 �ȊO�Ȃ�e�q�t��
					m_apModel[nCntModel]->SetParent(m_apModel[nParents]);
				}

				nCntModel++;
			}

			// ���f�����ő吔�ɂȂ�����z�u���I��
			if (nCntModel >= MAX_PLAYER_MODEL)
			{
				break;
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
		m_move = m_pControl->GetMove();
	}
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
// ���f�����̃��[���h�}�g���b�N�X���W�擾����
//=============================================================================
D3DXVECTOR3 CPlayer::GetModelMtxPos(int nCntModel)
{
	return m_apModel[nCntModel]->GetMtxPos();
}

//=============================================================================
// ���x���擾����(level.cpp���o�R)
//=============================================================================
int CPlayer::GetLevel(void)
{
	if (m_pLevel != NULL)
	{
		// ���݂̃��x�����擾���A�Ԃ�
		int nLevel = m_pLevel->GetLevel();
		return nLevel;
	}
	else
	{
		return false;
	}
}

//=============================================================================
// �e�̒Ǐ]����
//=============================================================================
void CPlayer::TrackingShadow(void)
{
	// �e����������
	if (m_pShadow != NULL)
	{
		//�I�u�W�F�N�g��������|�C���^
		CScene *pObject = NULL;
		//�I�u�W�F�N�g����ۑ�����|�C���^�ϐ�
		CScene *pSaveObject = NULL;

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
					posObj.y + sizeObj.y <= m_pos.y)
				{
					//�ʒu���t�B�[���h�̏��
					pos.y = posObj.y;

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
}

//=============================================================================
// ���ꂼ��̃R���W�����̐ڐG����
//=============================================================================
void CPlayer::TouchCollision(void)
{
	if (m_pCollisionS != NULL)
	{
		if (m_bInvincible == false)
		{
			// �̂̃R���W�������G�̍U������̃R���W�����ƐG�ꂽ�u�ԂȂ�
			if (m_pCollisionS->OnCollisionEnter(CCollisionSphere::COLLISION_S_TYPE_ENEMY_ATTACK) == true)
			{
				// �Ԃ��t���b�V��������
				CFlash::Create(D3DCOLOR_RGBA(204, 0, 0, 200), 0.1f, 0.03f);

				// �h��␳�l�ɂ���Ď󂯂�_���[�W�ʂ�ς���
				float fDamage = m_pCollisionS->GetDamageValue() * (1.0f - CLevelUp::GetCorrectionDef() * 0.1f);

				if (fDamage <= 0)
				{
					fDamage = 0;
				}

				// ���C�t�����炷
				m_fLife -= fDamage;

				// �_���[�WSE�̍Đ�
				CSound *pSound = CManager::GetSound();
				pSound->Play(12);
				pSound->ControllVoice(12, 0.4f);

				// �Ώۂ̃R���W�����̕�������������
				m_rot.y = m_pCollisionS->GetObjectiveRot();

				// ������я�Ԃɂ���
				m_state = PLAYER_STATE_BLOWAWAY;

				// ���G��Ԃɂ���
				m_bInvincible = true;
			}
		}
	}
}

//=============================================================================
// ��ԏ���
//=============================================================================
void CPlayer::State(void)
{
	if (m_bInvincible == true && m_bDeath == false)
	{
		// �̂̃R���W�������Ȃ��ɐݒ�
		m_pCollisionS->SetCollisionType(CCollisionSphere::COLLISION_S_TYPE_NONE);

		// �J�E���g��i�߂�
		m_nCntInvincible++;
		int nAlpha = 255;

		if (m_nCntInvincible % 8 == 0 || m_nCntInvincible % 8 == 1)
		{
			nAlpha = 128;
		}

		// ���f���̐F�𔖂�����
		for (int nCntModel = 0; nCntModel < MAX_PLAYER_MODEL; nCntModel++)
		{
			m_apModel[nCntModel]->SetColor(D3DCOLOR_RGBA(0, 0, 0, nAlpha));
		}

		// ���G���Ԃ��I�������
		if (m_nCntInvincible > INVINCIBLE_TIME)
		{
			// �J���[�����ɖ߂�
			for (int nCntModel = 0; nCntModel < MAX_PLAYER_MODEL; nCntModel++)
			{
				m_apModel[nCntModel]->SetColor(D3DCOLOR_RGBA(0, 0, 0, 255));
			}

			// �J�E���g�����Z�b�g
			m_nCntInvincible = 0;

			// ���G��Ԃ���߂�
			m_bInvincible = false;

			// �̂̃R���W�������v���C���[�ɖ߂�
			m_pCollisionS->SetCollisionType(CCollisionSphere::COLLISION_S_TYPE_PLAYER);
		}
	}
	else if (m_state == PLAYER_STATE_DEBUG)
	{
		// �̂̃R���W�������Ȃ��ɐݒ�
		m_pCollisionS->SetCollisionType(CCollisionSphere::COLLISION_S_TYPE_NONE);
	}
}

//=============================================================================
// �G�Ƃ�(�����o���̂��߂�)�Փˏ���
//=============================================================================
void CPlayer::CollisionEnemy(void)
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

		if (pObject->GetObjType() == CScene::OBJECT_TYPE_ENEMY)
		{
			//�v���C���[�ɃL���X�g
			CEnemy *pEnemy = NULL;
			pEnemy = (CEnemy*)pObject;

			D3DXVECTOR3 posEne = pEnemy->GetPos();		// �G�̈ʒu���擾
			float fEneLife = pEnemy->GetLife();			// �G�̃��C�t

			float fDistance = sqrtf((m_pos.x - posEne.x) * (m_pos.x - posEne.x) + (m_pos.y - posEne.y) * (m_pos.y - posEne.y) + (m_pos.z - posEne.z) * (m_pos.z - posEne.z));
			float fRot = (float)atan2((posEne.x - m_pos.x), (posEne.z - m_pos.z)) - D3DX_PI;

			// �������v���C���[�̃T�C�Y��菬�������A�G�̃��C�t���c���Ă�����
			if (fDistance <= m_size.x && fEneLife > 0.0f)
			{
				m_pos.x = posEne.x + (sinf(fRot) * m_size.x);
				m_pos.z = posEne.z + (cosf(fRot) * m_size.z);

				// �ʒu��ݒ�
				SetPos(m_pos);
			}
		}
		pObject = pSaveObject->GetObjNext(pSaveObject);
	}
}

//=============================================================================
// ����ύX���̏���
//=============================================================================
void CPlayer::ChangeWeapon(void)
{
	// ���킪�ς�����Ȃ�
	if (m_bWeaponChange == true)
	{
		if (m_pWepaon != NULL)
		{
			// ����A�C�R����UI��؂�ւ���
			m_pWepaon->SetUIChange((int)m_weapon);
		}

		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		switch (m_weapon)
		{
		case PLAYER_WEAPON_AXE:
			if (m_apModel != NULL)
			{
				m_apModel[15]->Uninit();
				m_apModel[15] = NULL;
			}

			if (m_apModel[15] == NULL)
			{
				// �E��̃p�[�c�̈ʒu���擾
				pos = m_apModel[5]->GetPos();

				// �{�E�K���𐶐����A�E��̃p�[�c�ɐe�q�֌W����������
				m_apModel[15] = CModel::Create(D3DXVECTOR3(pos.x - 5.0f, pos.y + 0.0f, pos.z + 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data/MODEL/Player/12_axe.x");
				m_apModel[15]->SetParent(m_apModel[5]);
			}
			break;

		case PLAYER_WEAPON_BOWGUN:
			if (m_apModel != NULL)
			{
				m_apModel[15]->Uninit();
				m_apModel[15] = NULL;
			}

			if (m_apModel[15] == NULL)
			{
				// �E��̃p�[�c�̈ʒu���擾
				pos = m_apModel[5]->GetPos();

				// ���𐶐����A�E��̃p�[�c�ɐe�q�֌W����������
				m_apModel[15] = CModel::Create(D3DXVECTOR3(pos.x - 5.0f, pos.y + 0.0f, pos.z + 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data/MODEL/Player/12_bowgun.x");
				m_apModel[15]->SetParent(m_apModel[5]);
			}
			break;

		default:
			break;
		}

		// ����̐؂�ւ����I��
		m_bWeaponChange = false;
	}
}