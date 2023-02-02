//=============================================================================
// �A���[���� [arrow.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "arrow.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "model_single.h"
#include "mesh_field.h"
#include "mesh_wall.h"
#include "sound.h"
#include "collision_sphere.h"
#include "effect_billboard.h"
#include "data_attack.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CArrow::CArrow(int nPriority) :CScene3D(nPriority)
{
	// �ϐ��̃N���A
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pModel = NULL;
	m_pCollisionS = NULL;
	m_nLife = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CArrow::~CArrow()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CArrow::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �ϐ��̏�����
	m_pos = pos;
	m_posOld = m_pos;
	m_size = size;
	m_nLife = SUSTAIN_TIME;

	// �ʒu�̐ݒ�
	SetPos(m_pos);

	// ��̃��f���𐶐�
	m_pModel = CModel::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data/MODEL/arrow.x");

	// �T�C�Y���擾
	m_size = m_pModel->GetSize();

	if (m_size.x > m_size.z)
	{
		m_size.z = m_size.x;
	}
	else if (m_size.z > m_size.x)
	{
		m_size.x = m_size.z;
	}

	// �U�����擾����
	CDataAttack *pdataAtk = CManager::GetDataAtk();

	// �R���W����(���픻��)������
	m_pCollisionS = CCollisionSphere::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_size.x * 1.5f, CCollisionSphere::COLLISION_S_TYPE::COLLISION_S_TYPE_WEAPON, pdataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOWGUN, CDataAttack::ATTACK_DATA_TYPE_TIME_COLLISION));

	// �_���[�W�ʁ~�{���ōU���͂�ݒ�
	m_pCollisionS->SetDamageValue(m_fAttackValue * pdataAtk->GetAttackData(CDataAttack::ATTACK_TYPE_BOWGUN, CDataAttack::ATTACK_DATA_TYPE_DAMAGE));

	// �T�E���h�擾����
	CSound *pSound = CManager::GetSound();

	// �{�E�K���̃V���b�gSE�̍Đ�
	pSound->Play(9);
	pSound->ControllVoice(9, 0.8f);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CArrow::Uninit(void)
{
	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CArrow::Update(void)
{
	// ���������炷
	m_nLife--;

	// �T�E���h�擾����
	CSound *pSound = CManager::GetSound();

	// �ǂɓ������Ă��Ȃ��Ȃ�
	if (CMeshWall::Collision(this) == false)
	{
		// �O�ՂɃG�t�F�N�g������
		CEffectB::Create(D3DXVECTOR3(m_pos.x - (sinf(m_rot.y) * 20.0f), m_pos.y, m_pos.z - (cosf(m_rot.y) * 20.0f)), D3DXVECTOR3(30.0f, 30.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			35, CEffectB::EFFECT_TYPE_SMALL, NULL);

		// �ʒu�̎擾
		D3DXVECTOR3 pos = GetPos();
		m_pos = pos;

		// 1�t���[���O�̈ʒu�̍X�V
		m_posOld = m_pos;

		// ���i������
		m_pos.x -= sinf(m_rot.y) * 16.0f;
		m_pos.z -= cosf(m_rot.y) * 16.0f;

		// �ʒu���f
		SetPos(m_pos);

		if (m_pCollisionS != NULL)
		{
			// ���̃R���W������Ǐ]������
			m_pCollisionS->SetPosCollision(m_pos);
		}

	}
	// �ǂɓ���������
	else if (CMeshWall::Collision(this) == true)
	{
		if (m_pCollisionS != NULL)
		{
			// �T�E���h�擾����
			CSound *pSound = CManager::GetSound();

			// �{�E�K���̓˂��h����SE�̍Đ�
			pSound->Play(10);
			pSound->ControllVoice(10, 0.8f);

			// �����蔻�������
			m_pCollisionS->Uninit();
			m_pCollisionS = NULL;
		}
	}

	// ���̃R���W�����ƐڐG�������̏���
	TouchCollision();

	// �������Ȃ��Ȃ�����
	if (m_nLife <= 0)
	{
		// �����蔻�������
		if (m_pCollisionS != NULL)
		{
			m_pCollisionS->Uninit();
			m_pCollisionS = NULL;
		}

		// ����
		Uninit();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CArrow::Draw(void)
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
	m_pModel->Draw();
}

//=============================================================================
// ��������
//=============================================================================
CArrow *CArrow::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fAttackValue)
{
	// �N���X�̐���
	CArrow *pArrow = NULL;
	pArrow = new CArrow;
	
	if (pArrow != NULL)
	{
		// �ϐ��̏�����
		pArrow->m_rot = rot;
		pArrow->m_fAttackValue = fAttackValue;

		// ����������
		pArrow->Init(pos, pArrow->m_size);
	}
	return pArrow;
}

//=============================================================================
// ���ꂼ��̃R���W�����̐ڐG����
//=============================================================================
void CArrow::TouchCollision(void)
{
	if (m_pCollisionS != NULL)
	{
		// �G����̃R���W�����ƐG�ꂽ�u�ԂȂ�
		if (m_pCollisionS->OnCollisionEnter(CCollisionSphere::COLLISION_S_TYPE_ENEMY) == true)
		{
			// �T�E���h�擾����
			CSound *pSound = CManager::GetSound();

			// ���̃q�b�gSE�̍Đ�
			pSound->Play(8);
			pSound->ControllVoice(8, 0.8f);

			m_pCollisionS->Uninit();
			Uninit();
		}
	}
}