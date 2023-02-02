//=============================================================================
// �G�l�~�[���� [enemy.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "enemy.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "model.h"
#include "model_single.h"
#include "mesh_field.h"
#include "mesh_wall.h"
#include "motion.h"
#include "control_wolf00.h"
#include "control_wolf01.h"
#include "control_boss.h"
#include "shadow.h"
#include "life_billboard.h"
#include "life.h"
#include "sound.h"
#include "collision_sphere.h"
#include "player.h"
#include "data_clear.h"
#include "particle_hit.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CEnemy::CEnemy(int nPriority) :CScene3D(nPriority)
{
	// �ϐ��̃N���A
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	for (int nCntModel = 0; nCntModel < MAX_ENEMY_MODEL; nCntModel++)
	{
		m_apModel[nCntModel] = NULL;
	}
	m_pParent = NULL;
	m_pMotion = NULL;
	m_pControl = NULL;
	m_pShadow = NULL;
	m_pLifeBill = NULL;
	m_pLife = NULL;
	memset(&m_pCollisionS, NULL, sizeof(m_pCollisionS));
	m_nModelNum = 0;
	m_nDeathCount = 0;
	m_nScore = 0;
	m_fLife = 0.0f;
	m_bLand = false;
	m_bKnockDown = false;
	m_bTouchWall = false;
	m_bCanControl = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEnemy::~CEnemy()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, float fLife)
{
	// �ϐ��̏�����
	m_pos = pos;
	m_posOld = pos;
	m_fLife = fLife;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nDeathCount = 0;
	m_bLand = false;
	m_bKnockDown = false;
	m_bTouchWall = false;
	m_bCanControl = false;

	// ���f���̐�������
	CreateModel(m_type);

	// �ʒu�̐ݒ�
	SetPos(m_pos);

	// �T�C�Y���擾
	m_size = m_apModel[0]->GetSize();

	if (m_size.x > m_size.z)
	{
		m_size.z = m_size.x;
	}
	else if (m_size.z > m_size.x)
	{
		m_size.x = m_size.z;
	}

	// �T�C�Y�̐ݒ�
	SetSize(m_size);

	CModel *pModel = NULL;
	D3DXVECTOR3 modelPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �R���g���[���A���[�V�����A�X�R�A�A�R���W�����̐���
	switch (m_type)
	{
	case ENEMY_TYPE_WOLF_0:
		m_pControl = CControlWolf00::Create();
		m_pMotion = CMotion::Create(this, CMotion::MOTION_TYPE_WOLF00);
		m_nScore = 200;
		// �̂ɂ����郂�f���̈ʒu������擾���A�e�q�t��
		modelPos = GetModelPos(0);
		pModel = GetModel(0);
		m_pCollisionS[0] = CCollisionSphere::Create(modelPos, m_size.x * 1.2f, CCollisionSphere::COLLISION_S_TYPE::COLLISION_S_TYPE_ENEMY, -1.0f);
		m_pCollisionS[0]->SetParent(pModel);
		break;

	case ENEMY_TYPE_WOLF_1:
		m_pControl = CControlWolf01::Create();
		m_pMotion = CMotion::Create(this, CMotion::MOTION_TYPE_WOLF01);
		m_nScore = 300;
		// �̂ɂ����郂�f���̈ʒu������擾���A�e�q�t��
		modelPos = GetModelPos(0);
		pModel = GetModel(0);
		m_pCollisionS[0] = CCollisionSphere::Create(D3DXVECTOR3(modelPos.x, modelPos.y - m_size.y, modelPos.z), m_size.x * 1.2f, CCollisionSphere::COLLISION_S_TYPE::COLLISION_S_TYPE_ENEMY, -1.0f);
		m_pCollisionS[0]->SetParent(pModel);
		break;

	case ENEMY_TYPE_BOSS:
		m_pControl = CControlBoss::Create();
		m_pMotion = CMotion::Create(this, CMotion::MOTION_TYPE_BOSS);
		m_nScore = 3000;
		m_fLife = BEGIN_BOSS_LIFE;
		
		// �̂ɂ����郂�f���̈ʒu������擾���A�e�q�t��
		modelPos = GetModelPos(1);
		pModel = GetModel(1);
		m_pCollisionS[0] = CCollisionSphere::Create(D3DXVECTOR3(modelPos.x, modelPos.y, modelPos.z), m_size.x, CCollisionSphere::COLLISION_S_TYPE::COLLISION_S_TYPE_ENEMY, -1.0f);
		m_pCollisionS[0]->SetParent(pModel);
		// ���ɂ����郂�f���̏����擾���A�e�q�t��
		modelPos = GetModelPos(2);
		pModel = GetModel(2);
		m_pCollisionS[1] = CCollisionSphere::Create(D3DXVECTOR3(modelPos.x, modelPos.y - 100.0f, modelPos.z + 40.0f), m_size.x * 0.6f, CCollisionSphere::COLLISION_S_TYPE::COLLISION_S_TYPE_ENEMY, -1.0f);
		m_pCollisionS[1]->SetParent(pModel);
		// �E��ɂ����郂�f���̏����擾���A�e�q�t��
		modelPos = GetModelPos(5);
		pModel = GetModel(5);
		m_pCollisionS[2] = CCollisionSphere::Create(D3DXVECTOR3(modelPos.x + 60.0f, modelPos.y, modelPos.z), m_size.x * 0.4f, CCollisionSphere::COLLISION_S_TYPE::COLLISION_S_TYPE_ENEMY, -1.0f);
		m_pCollisionS[2]->SetParent(pModel);
		// ����ɂ����郂�f���̏����擾���A�e�q�t��
		modelPos = GetModelPos(8);
		pModel = GetModel(8);
		m_pCollisionS[3] = CCollisionSphere::Create(D3DXVECTOR3(modelPos.x - 60.0f, modelPos.y, modelPos.z), m_size.x * 0.4f, CCollisionSphere::COLLISION_S_TYPE::COLLISION_S_TYPE_ENEMY, -1.0f);
		m_pCollisionS[3]->SetParent(pModel);
		break;

	default:
		break;
	}
	m_pMotion->Update(this);

	modelPos = m_apModel[0]->GetPos();
	m_size.y = modelPos.y + m_size.y * 2;

	// �e�̐������A�e���g�p���Ă����Ԃɐݒ�
	m_pShadow = CShadow::Create(D3DXVECTOR3(m_pos.x, m_pos.y - m_size.y, m_pos.z), D3DXVECTOR3(m_size.x, 0.0f, m_size.x));
	m_pShadow->SetUseShadow(true);

	if (m_type == ENEMY_TYPE_BOSS)
	{
		// ���C�t�𐶐����A���݂̃��C�t�ƍő僉�C�t��ݒ�
		m_pLife = CLife::Create(D3DXVECTOR2(SCREEN_WIDTH / 2 + 180.0f, SCREEN_HEIGHT - 70.0f), D3DXVECTOR2(SCREEN_WIDTH - 480.0f, 35.0f), CLife::LIFE_TYPE_BOSS);
		m_pLife->SetLife((float)m_fLife);
		m_pLife->SetMaxLife((float)BEGIN_BOSS_LIFE);
	}
	else
	{
		// ���C�t�ƍő僉�C�t��ݒ�
		m_pLifeBill = CLifeBillboard::Create(D3DXVECTOR3(m_pos.x, m_pos.y + m_size.y / 2, m_pos.z), 130.0f, 10.0f);
		m_pLifeBill->SetMaxLife(m_fLife);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEnemy::Uninit(void)
{
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
	for (int nCntPlayer = 0; nCntPlayer < MAX_ENEMY_MODEL; nCntPlayer++)
	{
		if (m_apModel[nCntPlayer] != NULL)
		{
			m_apModel[nCntPlayer]->Uninit();
		}
	}

	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CEnemy::Update(void)
{
	if (this != NULL)
	{
		// ���삳���邩�ǂ���
		CanControl(this);

		if (m_bCanControl == true)
		{
			// �T�E���h�擾����
			CSound *pSound = CManager::GetSound();

			// �e�̒Ǐ]
			TrackingShadow();

			// �ʒu�̎擾
			D3DXVECTOR3 pos = GetPos();
			m_pos = pos;
			m_posOld = pos;

			// �|����Ă��Ȃ��ꍇ
			if (m_bKnockDown == false)
			{
				// �ړ�����
				Move();
			}
			// �|���ꂽ�ꍇ
			else
			{
				m_move.x = 0.0f;
				m_move.z = 0.0f;
			}

			// �ړ��ʔ��f
			m_pos += m_move;

			// �ʒu���f
			SetPos(m_pos);

			// �G���m�̉����o������
			CollisionEnemy(this);

			// ���f���Ƃ̓����蔻��
			m_bLand = CModelSingle::Collision(this);

			// �ǂƂ̓����蔻��
			m_bTouchWall = CMeshWall::Collision(this);

			if (m_bLand == false)
			{
				// �n�ʂƂ̓����蔻��
				m_bLand = CMeshField::Collision(this);
			}

			if (m_bLand == true)
			{
				// ���n������Y�����̈ړ��ʂ�0��
				m_move.y = 0.0f;
			}

			// ���[�V����
			m_pMotion->Update(this);

			// �ʒu�擾
			m_pos = GetPos();

			// ���̃R���W�����ƐڐG�������̏���
			TouchCollision();
		}

		if (m_pLifeBill != NULL)
		{
			// �ʒu���X�V���A���݂̃��C�t��ݒ�
			m_pLifeBill->SetPos(m_pos);
			m_pLifeBill->SetLife(m_fLife);
		}

		if (m_pLife != NULL)
		{
			// ���݂̃��C�t��ݒ�
			m_pLife->SetLife(m_fLife);
		}

		// ���C�t���Ȃ��Ȃ�����
		if (m_fLife <= 0)
		{
			// �|���ꂽ���̏���
			KnockDown();
		}

		// �N���A�̏�ԂɂȂ�����
		if (CGame::GetClear() == true)
		{
			// ���C�t��UI������
			if (m_pLife != NULL)
			{
				m_pLife->Uninit();
				delete m_pLife;
				m_pLife = NULL;
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CEnemy::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

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
	for (int nCntModel = 0; nCntModel < m_nModelNum; nCntModel++)
	{
		m_apModel[nCntModel]->Draw();
	}
}

//=============================================================================
// ��������
//=============================================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, ENEMY_TYPE type, float fLife)
{
	// �N���X�̐���
	CEnemy *pEnemy = NULL;
	pEnemy = new CEnemy;

	if (pEnemy != NULL)
	{
		// �ϐ��̏�����
		pEnemy->m_rot = rot;
		pEnemy->m_type = type;

		switch (pEnemy->m_type)
		{
		case ENEMY_TYPE_WOLF_0:
			pEnemy->m_nModelNum = MAX_ENEMY_MODEL_WOLF00;
			break;
		case ENEMY_TYPE_WOLF_1:
			pEnemy->m_nModelNum = MAX_ENEMY_MODEL_WOLF01;
			break;
		case ENEMY_TYPE_BOSS:
			pEnemy->m_nModelNum = MAX_ENEMY_MODEL_BOSS;
			break;
		default:
			break;
		}

		// ����������
		pEnemy->Init(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), fLife);
	}
	return pEnemy;
}

//=============================================================================
// �ړ�����
//=============================================================================
void CEnemy::Move(void)
{
	// �k���`�F�b�N
	if (m_pControl != NULL)
	{
		// �G�l�~�[����̃N���X�Ƀv���C���[�̃|�C���^�����A�ړ��ʂ��擾
		m_pControl->Update(this);
		m_move = m_pControl->GetMove();
	}
}

//=============================================================================
// �e�̕\���̂��߂̃I�u�W�F�N�g�Ƃ̓����蔻��
//=============================================================================
void CEnemy::TrackingShadow(void)
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
void CEnemy::TouchCollision(void)
{
	for (int nCollision = 0; nCollision < MAX_ENEMY_COLLSION; nCollision++)
	{
		if (m_pCollisionS[nCollision] != NULL)
		{
			// �̂̃R���W���������炩�̕��픻��̃R���W�����ƐG�ꂽ�u�ԂȂ�
			if (m_pCollisionS[nCollision]->OnCollisionEnter(CCollisionSphere::COLLISION_S_TYPE_WEAPON) == true || m_pCollisionS[nCollision]->OnCollisionEnter(CCollisionSphere::COLLISION_S_TYPE_WEAPON_P) == true)
			{
				// ���C�t�����炷
				m_fLife -= m_pCollisionS[nCollision]->GetDamageValue();

				// ���̃q�b�gSE�̍Đ�
				CSound *pSound = CManager::GetSound();
				pSound->Play(8);
				pSound->ControllVoice(8, 0.8f);

				// �p�[�e�B�N���̐���
				D3DXVECTOR3 collpos = m_pCollisionS[nCollision]->GetMtxPos();
				CParticleHit::Create(collpos, D3DXVECTOR3(50.0f, 50.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DCOLOR_RGBA(255, 50, 50, 255), 20, 8.0f, 10);

				// �U�R�G�͔�_���[�W���[�V�����ɂ���
				if (m_type == ENEMY_TYPE_WOLF_0 || m_type == ENEMY_TYPE_WOLF_1)
				{
					// ���C�t���c���Ă�����
					if (m_pCollisionS[nCollision]->OnCollisionEnter(CCollisionSphere::COLLISION_S_TYPE_WEAPON_P) == true && m_fLife > 0)
					{
						// �_���[�W���[�V�����ɂ���
						m_pMotion->SetMotion(3);
					}
				}
			}
		}
	}
}

//=============================================================================
// �G�Ƃ�(�����o���̂��߂�)�Փˏ���
//=============================================================================
void CEnemy::CollisionEnemy(CEnemy *pEnemy)
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
			// �����ȊO�̓G��������
			if (pObject != this)
			{
				//�G�l�~�[�ɃL���X�g
				CEnemy *pObjEnemy = NULL;
				pObjEnemy = (CEnemy*)pObject;

				D3DXVECTOR3 posEne = pObjEnemy->GetPos();		// �Ώۂ̓G�̈ʒu���擾
				D3DXVECTOR3 posEneOld = pObjEnemy->GetPosOld();	// �Ώۂ̓G��1�t���[���O�̈ʒu���擾
				D3DXVECTOR3 sizeEne = pObjEnemy->GetSize();		// �Ώۂ̓G�̃T�C�Y���擾
				float fEneLife = pObjEnemy->GetLife();			// �Ώۂ̓G�̃��C�t

				float fDistance = sqrtf((pEnemy->m_pos.x - posEne.x) * (pEnemy->m_pos.x - posEne.x) + (pEnemy->m_pos.y - posEne.y) * (pEnemy->m_pos.y - posEne.y) + (pEnemy->m_pos.z - posEne.z) * (pEnemy->m_pos.z - posEne.z));
				float fRot = (float)atan2((posEne.x - pEnemy->m_pos.x), (posEne.z - pEnemy->m_pos.z)) - D3DX_PI;

				// �������G�l�~�[�̃T�C�Y��菬�������A�Ώۂ̓G�̃��C�t���c���Ă�����
				if (fDistance <= pEnemy->m_size.x && fEneLife > 0.0f)
				{
					pEnemy->m_pos.x = posEne.x + (sinf(fRot) * m_size.x);
					pEnemy->m_pos.z = posEne.z + (cosf(fRot) * m_size.z);

					// �ʒu��ݒ�
					SetPos(pEnemy->m_pos);
				}
			}
		}
		pObject = pSaveObject->GetObjNext(pSaveObject);
	}
}

//=============================================================================
// ���f���������̏���
//=============================================================================
void CEnemy::CreateModel(ENEMY_TYPE type)
{
	char cString[256];		// �e�L�X�g�ۑ��p�̕ϐ�
	FILE *pFile = NULL;		// �e�L�X�g�t�@�C���̓ǂݍ���

	// �T00
	if (type == ENEMY_TYPE_WOLF_0)
	{
		pFile = fopen("data/TEXT/motion_wolf00.txt", "r");
		//nModelType = 17; // [�T00�F��]�̃��f���ԍ�
	}
	// �T01
	else if (type == ENEMY_TYPE_WOLF_1)
	{
		pFile = fopen("data/TEXT/motion_wolf01.txt", "r");
		//nModelType = 29; // [�T01�F��]�̃��f���ԍ�
	}
	// �{�X
	else if (type == ENEMY_TYPE_BOSS)
	{
		pFile = fopen("data/TEXT/motion_boss.txt", "r");
		//nModelType = 45; // [�{�X�F��]�̃��f���ԍ�
	}

	if (pFile != NULL)
	{
		int nCntModel = 0;
		char cFileName[MAX_ENEMY_MODEL][128];

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
			if (nCntModel >= m_nModelNum)
			{
				nCntModel = 0;
				break;
			}
		}

		int nParents = 0;
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
			if (nCntModel >= m_nModelNum)
			{
				break;
			}
		}
		// �t�@�C�������
		fclose(pFile);
	}
}

//=============================================================================
// �o���l�󂯓n������
//=============================================================================
void CEnemy::SendExp(void)
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

		if (pObject->GetObjType() == CScene::OBJECT_TYPE_PLAYER)
		{
			// �����ȊO�̓G��������
			if (pObject != NULL)
			{
				//�v���C���[�ɃL���X�g
				CPlayer *pPlayer = NULL;
				pPlayer = (CPlayer*)pObject;

				int nExp = 0;
				switch (m_type)
				{
				case ENEMY_TYPE_WOLF_0:
					nExp = 2;
					break;

				case ENEMY_TYPE_WOLF_1:
					nExp = 3;
					break;

				default:
					break;
				}

				// �ʒu��ݒ�
				pPlayer->AddExp(nExp);
			}
		}
		pObject = pSaveObject->GetObjNext(pSaveObject);
	}
}

//=============================================================================
// �|���ꂽ���̏���
//=============================================================================
void CEnemy::KnockDown(void)
{
	if (m_bKnockDown == false)
	{
		// �v���C���[�Ɍo���l��n��
		SendExp();

		// �X�R�A�����Z
		CManager::GetDataClear()->AddScore(m_nScore);

		// ���C�t��0�ɐݒ�
		if (m_pLife != NULL)
		{
			m_fLife = 0;
			m_pLife->SetLife(m_fLife);
		}

		// �R���W����������
		for (int nCollision = 0; nCollision < MAX_ENEMY_COLLSION; nCollision++)
		{
			if (m_pCollisionS[nCollision] != NULL)
			{
				m_pCollisionS[nCollision]->Uninit();
				m_pCollisionS[nCollision] = NULL;
			}
		}

		// �e�̏I������
		if (m_pShadow != NULL)
		{
			m_pShadow->Uninit();
			m_pShadow = NULL;
		}

		m_bKnockDown = true;
	}

	switch (m_type)
	{
		// �{�X
	case ENEMY_TYPE_BOSS:
		if (m_pMotion->GetConnect() == false && m_pMotion->GetMotion() != 3)
		{
			// �_�E�����[�V�����ɂ���
			m_pMotion->SetMotion(3);
		}
		break;
		// �U�R�G
	default:
		if (m_pMotion->GetConnect() == false && m_pMotion->GetMotion() != 4)
		{
			// �_�E�����[�V�����ɂ���
			m_pMotion->SetMotion(4);
		}

		// ���C�t������
		if (m_pLifeBill != NULL)
		{
			m_pLifeBill->Uninit();
			m_pLifeBill = NULL;
		}

		// �J�E���g��i�߂�
		m_nDeathCount++;

		// 2�b�o���������
		if (m_nDeathCount >= 120)
		{
			Uninit();
		}
		break;
	}
}

//=============================================================================
// ���삳����ǂ����̐ݒ菈��
//=============================================================================
void CEnemy::CanControl(CEnemy *pEnemy)
{
	//���G�̐��������ƁA�S�Ă̓G�����삷��ۂɏ����������������邽�߁A�v���C���[�ɋ߂��G�𓮍삳����
	m_bCanControl = false;

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

			D3DXVECTOR3 plyPos = pPlayer->GetPos();		// �v���C���[�̈ʒu
			D3DXVECTOR3 enePos = pEnemy->GetPos();		// �G�̈ʒu

			float fRadius = sqrtf((plyPos.x - enePos.x) * (plyPos.x - enePos.x) + (plyPos.z - enePos.z) * (plyPos.z - enePos.z));

			// �v���C���[�Ƃ̋�����4000.0f�ȏ㗣��Ă�����
			if (fRadius >= 4000.0f)
			{
				// ���삳���Ȃ�
				m_bCanControl = false;
			}
			else
			{
				// ���삳����
				m_bCanControl = true;
			}
		}
		// ���̃I�u�W�F�N�g���擾
		pObject = pSaveObject->GetObjNext(pSaveObject);
	}
}

//=============================================================================
// ���f�����̈ʒu�ݒ菈��
//=============================================================================
void CEnemy::SetModelPos(int nCntModel, D3DXVECTOR3 pos)
{
	if (m_apModel[nCntModel] != NULL)
	{
		m_apModel[nCntModel]->SetPos(pos);
	}
}

//=============================================================================
// ���f�����̈ʒu�擾����
//=============================================================================
D3DXVECTOR3 CEnemy::GetModelPos(int nCntModel)
{
	if (m_apModel[nCntModel] != NULL)
	{
		return m_apModel[nCntModel]->GetPos();
	}
	else
	{
		return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//=============================================================================
// ���f�����̌����ݒ菈��
//=============================================================================
void CEnemy::SetModelRot(int nCntModel, D3DXVECTOR3 rot)
{
	if (m_apModel[nCntModel] != NULL)
	{
		m_apModel[nCntModel]->SetRot(rot);
	}
}

//=============================================================================
// ���f�����̌����擾����
//=============================================================================
D3DXVECTOR3 CEnemy::GetModelRot(int nCntModel)
{
	if (m_apModel[nCntModel] != NULL)
	{
		return m_apModel[nCntModel]->GetRot();
	}
	else
	{
		return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//=============================================================================
// �G�l�~�[�̃��f���擾����
//=============================================================================
CModel *CEnemy::GetModel(int nCntModel)
{
	if (m_apModel[nCntModel] != NULL)
	{
		return m_apModel[nCntModel];
	}
	else
	{
		return false;
	}
}