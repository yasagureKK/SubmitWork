//=============================================================================
// �G�l�~�[�{�b�N�X���� [enemy_box.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "enemy_box.h"
#include "manager.h"
#include "renderer.h"
#include "scene3D.h"
#include "player.h"
#include "enemy.h"
#include "mesh_wall.h"
#include "flash.h"
#include "sound.h"
#include "game.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CEnemyBox::CEnemyBox(int nPriority) :CScene3D(nPriority)
{
	// �ϐ��̃N���A
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_type = ENEMY_BOX_TYPE_0;
	m_nEnemyNum = 0;
	m_bInside = false;
	m_bDefeat = false;
	memset(&m_pEnemy, NULL, sizeof(m_pEnemy));
	memset(&m_pMeshWall, NULL, sizeof(m_pMeshWall));
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEnemyBox::~CEnemyBox()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CEnemyBox::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �ϐ��̏�����
	m_pos = pos;
	m_size = size;
	m_bInside = false;
	m_bDefeat = false;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEnemyBox::Uninit(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEBOX_ENEMY; nCntEnemy++)
	{
		if (m_pEnemy[nCntEnemy] != NULL)
		{
			m_pEnemy[nCntEnemy] = NULL;
		}
	}

	for (int nCntWall = 0; nCntWall < MAX_ENEBOX_WALL; nCntWall++)
	{
		if (m_pMeshWall[nCntWall] != NULL)
		{
			m_pMeshWall[nCntWall]->Uninit();
		}
	}

	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CEnemyBox::Update(void)
{
	InsidePlayer();

	// �v���C���[�����ɂ����Ԃ�
	if (m_bInside == true)
	{
		for (int nCntEnemy = 0; nCntEnemy < m_nEnemyNum; nCntEnemy++)
		{
			// 1�̂ł����C�t���c���Ă���ΏI������
			if (m_pEnemy[nCntEnemy] != NULL && m_pEnemy[nCntEnemy]->GetLife() > 0)
			{
				break;
			}

			// �Ō�̓G�܂�for�������[�v�ł�����
			if (nCntEnemy == m_nEnemyNum - 1)
			{
				// �S�Ă̓G��NULL(�|���ꂽ)�ꍇ�A�S�ł�����Ԃɂ���
				m_bDefeat = true;
			}
		}
	}

	// �G���S�ł�����
	if (m_bDefeat == true)
	{
		// �G�l�~�[�{�b�N�X�̃N���ASE�̍Đ�
		CSound *pSound = CManager::GetSound();
		pSound->Play(26);
		pSound->ControllVoice(26, 0.8f);

		// �����t���b�V��������
		CFlash::Create(D3DCOLOR_RGBA(255, 255, 255, 200), 0.5f, 0.01f);

		// �ԍ���2(�{�X�𐶐�����{�b�N�X)��������
		if (m_type == ENEMY_BOX_TYPE_BOSS)
		{
			// �{�X��t���O��false��
			CManager::GetGame()->SetBossBattle(false);

			// �Q�[���̃{�X���j�t���O��true��
			CManager::GetGame()->SetDefeatBoss(true);
		}

		Uninit();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CEnemyBox::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;

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

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);
}

//=============================================================================
// ��������
//=============================================================================
CEnemyBox* CEnemyBox::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nType)
{
	// �N���X�̐���
	CEnemyBox *pEnemyBox = NULL;
	pEnemyBox = new CEnemyBox;

	if (pEnemyBox != NULL)
	{
		// �ϐ��̏�����
		pEnemyBox->m_type = (CEnemyBox::ENEMY_BOX_TYPE)nType;

		// ����������
		pEnemyBox->Init(pos, size);
	}
	return pEnemyBox;
}

//=============================================================================
// �v���C���[�����ɓ��������𔻕ʂ��鏈��
//=============================================================================
void CEnemyBox::InsidePlayer(void)
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

			D3DXVECTOR3 wallSize = m_size * 2 * 0.98f / 2;
			D3DXVECTOR3 posPlayer = pPlayer->GetPos();		// �v���C���[�̈ʒu
			D3DXVECTOR3 posPlyOld = pPlayer->GetPosOld();	// �v���C���[��1�t���[���O�̈ʒu
			D3DXVECTOR3 sizePlayer = pPlayer->GetSize();	// �v���C���[�̃T�C�Y

			if (m_pos.y + m_size.y > posPlayer.y &&
				m_pos.y < posPlayer.y + sizePlayer.y &&
				m_pos.z + wallSize.z >= posPlayer.z + sizePlayer.z / 2.0f &&
				m_pos.z - wallSize.z <= posPlayer.z - sizePlayer.z / 2.0f &&
				m_pos.x + wallSize.x >= posPlayer.x + sizePlayer.x / 2.0f &&
				m_pos.x - wallSize.x <= posPlayer.x - sizePlayer.x / 2.0f)
			{
				// �G�E�ǂ�z�u
				if (m_bInside == false)
				{
					// �G�l�~�[�{�b�N�X�̃C��SE�̍Đ�
					CSound *pSound = CManager::GetSound();
					pSound->Play(25);
					pSound->ControllVoice(25, 0.8f);

					// �{�X��̃G���A�ɓ�������
					if (m_type == ENEMY_BOX_TYPE_BOSS)
					{
						// �v���C���[�����̑���Ɉړ�������
						pPlayer->SetPos(D3DXVECTOR3(posPlayer.x, posPlayer.y + 5000.0f, posPlayer.z));
					}

					// �Ԃ��t���b�V��������
					CFlash::Create(D3DCOLOR_RGBA(178, 0, 0, 200), 0.075f, 0.02f);

					// �I�u�W�F�N�g�̔z�u
					SetObject();
				}
				// �v���C���[�����ɓ��������
				m_bInside = true;
			}
		}

		// ���̃I�u�W�F�N�g���擾
		pObject = pSaveObject->GetObjNext(pSaveObject);
	}
}

//=============================================================================
// �G�E�ǂ̔z�u����
//=============================================================================
void CEnemyBox::SetObject(void)
{
	if (m_type != ENEMY_BOX_TYPE_BOSS)
	{
		// �ǂ̔z�u
		if (m_pMeshWall[0] == NULL)
		{
			// �e�N�X�`�����d�Ȃ��Ă��܂����߁A�t�B�[���h�̕ǂ��班�������Ĕz�u
			m_pMeshWall[0] = CMeshWall::Create(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z + m_size.z * 0.98f), D3DXVECTOR3(m_size.x * 2 * 0.98f, m_size.y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, 1, 3);
			m_pMeshWall[1] = CMeshWall::Create(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z - m_size.z * 0.98f), D3DXVECTOR3(m_size.x * 2 * 0.98f, m_size.y, 0.0f), D3DXVECTOR3(0.0f, 3.14f, 0.0f), 1, 1, 3);
			m_pMeshWall[2] = CMeshWall::Create(D3DXVECTOR3(m_pos.x + m_size.x * 0.98f, m_pos.y, m_pos.z), D3DXVECTOR3(m_size.x * 2 * 0.98f, m_size.y, 0.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 1, 1, 3);
			m_pMeshWall[3] = CMeshWall::Create(D3DXVECTOR3(m_pos.x - m_size.x * 0.98f, m_pos.y, m_pos.z), D3DXVECTOR3(m_size.x * 2 * 0.98f, m_size.y, 0.0f), D3DXVECTOR3(0.0f, -1.57f, 0.0f), 1, 1, 3);
			m_pMeshWall[4] = CMeshWall::Create(D3DXVECTOR3(m_pos.x, m_pos.y - 10.0f, m_pos.z + m_size.z * 0.98f), D3DXVECTOR3(m_size.x * 2 * 0.98f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, 1, 4);
			m_pMeshWall[5] = CMeshWall::Create(D3DXVECTOR3(m_pos.x, m_pos.y - 10.0f, m_pos.z - m_size.z * 0.98f), D3DXVECTOR3(m_size.x * 2 * 0.98f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 3.14f, 0.0f), 1, 1, 4);
			m_pMeshWall[6] = CMeshWall::Create(D3DXVECTOR3(m_pos.x + m_size.x * 0.98f, m_pos.y - 10.0f, m_pos.z), D3DXVECTOR3(m_size.x * 2 * 0.98f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 1, 1, 4);
			m_pMeshWall[7] = CMeshWall::Create(D3DXVECTOR3(m_pos.x - m_size.x * 0.98f, m_pos.y - 10.0f, m_pos.z), D3DXVECTOR3(m_size.x * 2 * 0.98f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, -1.57f, 0.0f), 1, 1, 4);
		}
	}

	// �G�̔z�u
	if (m_pEnemy[0] == NULL)
	{
		switch (m_type)
		{
		case ENEMY_BOX_TYPE_0:
			m_nEnemyNum = 4;
			m_pEnemy[0] = CEnemy::Create(D3DXVECTOR3(m_pos.x + 200.0f, m_pos.y, m_pos.z + 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEnemy::ENEMY_TYPE_WOLF_0, 8.0f);
			m_pEnemy[1] = CEnemy::Create(D3DXVECTOR3(m_pos.x - 200.0f, m_pos.y, m_pos.z + 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEnemy::ENEMY_TYPE_WOLF_0, 8.0f);
			m_pEnemy[2] = CEnemy::Create(D3DXVECTOR3(m_pos.x - 250.0f, m_pos.y, m_pos.z - 400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEnemy::ENEMY_TYPE_WOLF_0, 8.0f);
			m_pEnemy[2] = CEnemy::Create(D3DXVECTOR3(m_pos.x - 350.0f, m_pos.y, m_pos.z - 50.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEnemy::ENEMY_TYPE_WOLF_0, 8.0f);
			break;

		case ENEMY_BOX_TYPE_1:
			m_nEnemyNum = 6;
			m_pEnemy[0] = CEnemy::Create(D3DXVECTOR3(m_pos.x + 200.0f, m_pos.y, m_pos.z - 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEnemy::ENEMY_TYPE_WOLF_0, 15.0f);
			m_pEnemy[1] = CEnemy::Create(D3DXVECTOR3(m_pos.x - 200.0f, m_pos.y, m_pos.z - 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEnemy::ENEMY_TYPE_WOLF_0, 15.0f);
			m_pEnemy[2] = CEnemy::Create(D3DXVECTOR3(m_pos.x + 500.0f, m_pos.y, m_pos.z),			D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEnemy::ENEMY_TYPE_WOLF_1, 15.0f);
			m_pEnemy[3] = CEnemy::Create(D3DXVECTOR3(m_pos.x - 500.0f, m_pos.y, m_pos.z),			D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEnemy::ENEMY_TYPE_WOLF_1, 15.0f);
			m_pEnemy[4] = CEnemy::Create(D3DXVECTOR3(m_pos.x + 350.0f, m_pos.y, m_pos.z + 350.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEnemy::ENEMY_TYPE_WOLF_1, 17.0f);
			m_pEnemy[5] = CEnemy::Create(D3DXVECTOR3(m_pos.x - 350.0f, m_pos.y, m_pos.z + 350.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEnemy::ENEMY_TYPE_WOLF_0, 18.0f);
			break;

		case ENEMY_BOX_TYPE_2:
			m_nEnemyNum = 4;
			m_pEnemy[0] = CEnemy::Create(D3DXVECTOR3(m_pos.x + 200.0f, m_pos.y, m_pos.z + 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEnemy::ENEMY_TYPE_WOLF_1, 10.0f);
			m_pEnemy[1] = CEnemy::Create(D3DXVECTOR3(m_pos.x - 200.0f, m_pos.y, m_pos.z + 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEnemy::ENEMY_TYPE_WOLF_1, 10.0f);
			m_pEnemy[2] = CEnemy::Create(D3DXVECTOR3(m_pos.x + 200.0f, m_pos.y, m_pos.z - 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEnemy::ENEMY_TYPE_WOLF_1, 10.0f);
			m_pEnemy[3] = CEnemy::Create(D3DXVECTOR3(m_pos.x - 200.0f, m_pos.y, m_pos.z - 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEnemy::ENEMY_TYPE_WOLF_1, 10.0f);
			break;

		case ENEMY_BOX_TYPE_BOSS:
			// �{�X��t���O��true��
			CManager::GetGame()->SetBossBattle(true);
			m_nEnemyNum = 1;

			// ���̑���ɔz�u
			m_pEnemy[0] = CEnemy::Create(D3DXVECTOR3(m_pos.x, m_pos.y + 5000.0f, m_pos.z + 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEnemy::ENEMY_TYPE_BOSS, 0.0f);
			break;

		default:
			break;
		}
	}
}