//=============================================================================
// �P�̂̃��f������ [model_single.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "model_single.h"
#include "manager.h"
#include "model_load.h"
#include "renderer.h"
#include "model.h"
#include "mesh_field.h"
#include "player.h"
#include "shadow.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CModelSingle::CModelSingle(int nPriority) :CScene3D(nPriority)
{
	// �ϐ��̃N���A
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pModel = NULL;
	//m_pShadow = NULL;
}

//================================================
// �f�X�g���N�^
//================================================
CModelSingle::~CModelSingle()
{

}

//================================================
// ����������
//================================================
HRESULT CModelSingle::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nModelType)
{
	// �ϐ��̏�����
	m_pos = pos;
	m_posOld = m_pos;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ���f���̐���
	m_pModel = CModel::Create(m_pos, m_rot, CManager::GetModelLoad()->GetModelName(nModelType));

	// �e���f���̐ݒ�(�P�̂̂���NULL������)
	m_pModel->SetParent(NULL);

	// �T�C�Y���擾
	m_size = m_pModel->GetSize();

	SetPos(m_pos);

	return S_OK;
}

//================================================
// �I������
//================================================
void CModelSingle::Uninit(void)
{
	// �J������
	Release();
}

//================================================
// �X�V����
//================================================
void CModelSingle::Update(void)
{
	// �ʒu�擾
	D3DXVECTOR3 pos = GetPos();

	// �ʒu�̔��f
	m_pos = pos;
	m_posOld = pos;

	// �ړ�
	m_pos += m_move;

	// �ʒu��ݒ�
	SetPos(m_pos);
}

//================================================
// �`�揈��
//================================================
void CModelSingle::Draw(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	//���f���̃��[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//���f���̌�����ݒ�
	m_pModel->SetRot(m_rot);

	//���f���̈ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���f���̕`��
	m_pModel->Draw();
}

//================================================
// ��������
//================================================
CModelSingle *CModelSingle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nModelType)
{
	// �N���X�̐���
	CModelSingle *pModelSingle = NULL;
	pModelSingle = new CModelSingle;
	
	if (pModelSingle != NULL)
	{
		// ����������
		pModelSingle->Init(pos, rot, nModelType);
	}
	return pModelSingle;
}

//================================================
// �����蔻�菈��
//================================================
bool CModelSingle::Collision(CScene *pScene)
{
	//���n���Ă��邩�ǂ���
	bool bLand = false;

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
			//���f���V���O���ɃL���X�g
			CModelSingle *pModelSingle = NULL;
			pModelSingle = (CModelSingle*)pObject;

			D3DXVECTOR3 posModel = pModelSingle->GetPos();		//���f���̈ʒu
			D3DXVECTOR3 sizeModel = pModelSingle->GetSize();	//���f���̃T�C�Y
			D3DXVECTOR3 pos = pScene->GetPos();					//�Ώۂ̈ʒu
			D3DXVECTOR3 size = pScene->GetSize();				//�Ώۂ̃T�C�Y
			D3DXVECTOR3 posOld = pScene->GetPosOld();			//�Ώۂ�1�t���[���O�̈ʒu

			if (posOld.x + size.x / 2.0f > posModel.x - sizeModel.x / 2.0f &&
				posOld.x - size.x / 2.0f < posModel.x + sizeModel.x / 2.0f &&
				posOld.z + size.x / 2.0f > posModel.z - sizeModel.z / 2.0f &&
				posOld.z - size.x / 2.0f < posModel.z + sizeModel.z / 2.0f &&
				pos.y <= posModel.y + sizeModel.y &&
				posOld.y >= posModel.y + sizeModel.y)
			{
				//�ʒu��ݒ�
				pos.y = posModel.y + sizeModel.y;
				pScene->SetPos(pos);

				bLand = true;
			}
			else if (posOld.x + size.x / 2.0f > posModel.x - sizeModel.x / 2.0f &&
				posOld.x - size.x / 2.0f < posModel.x + sizeModel.x / 2.0f &&
				posOld.z + size.x / 2.0f > posModel.z - sizeModel.z / 2.0f &&
				posOld.z - size.x / 2.0f < posModel.z + sizeModel.z / 2.0f &&
				pos.y + size.y >= posModel.y &&
				posOld.y + size.y <= posModel.y)
			{
				//�ʒu��ݒ�
				pos.y = posModel.y - size.y;
				pScene->SetPos(pos);
			}
			if (pos.y + size.y > posModel.y &&
				pos.y < posModel.y + sizeModel.y &&
				posOld.z + size.z / 2.0f > posModel.z - sizeModel.z / 2.0f &&
				posOld.z - size.z / 2.0f < posModel.z + sizeModel.z / 2.0f &&
				pos.x + size.x / 2.0f >= posModel.x - sizeModel.x / 2.0f &&
				posOld.x + size.x / 2.0f <= posModel.x - sizeModel.x / 2.0f)
			{
				//�ʒu��ݒ�
				pos.x = posModel.x - sizeModel.x / 2.0f - size.x / 2.0f;
				pScene->SetPos(pos);
			}
			else if (pos.y + size.y > posModel.y &&
				pos.y < posModel.y + sizeModel.y &&
				posOld.z + size.z / 2.0f > posModel.z - sizeModel.z / 2.0f &&
				posOld.z - size.z / 2.0f < posModel.z + sizeModel.z / 2.0f &&
				pos.x - size.x / 2.0f <= posModel.x + sizeModel.x / 2.0f &&
				posOld.x - size.x / 2.0f >= posModel.x + sizeModel.x / 2.0f)
			{
				//�ʒu��ݒ�
				pos.x = posModel.x + sizeModel.x / 2.0f + size.x / 2.0f;
				pScene->SetPos(pos);
			}
			if (pos.y + size.y > posModel.y &&
				pos.y < posModel.y + sizeModel.y &&
				posOld.x + size.x / 2.0f > posModel.x - sizeModel.x / 2.0f &&
				posOld.x - size.x / 2.0f < posModel.x + sizeModel.x / 2.0f &&
				pos.z + size.z / 2.0f >= posModel.z - sizeModel.z / 2.0f &&
				posOld.z + size.z / 2.0f <= posModel.z - sizeModel.z / 2.0f)
			{
				//�ʒu��ݒ�
				pos.z = posModel.z - sizeModel.z / 2.0f - size.z / 1.95f;
				pScene->SetPos(pos);
			}
			else if (pos.y + size.y > posModel.y &&
				pos.y < posModel.y + sizeModel.y &&
				posOld.x + size.x / 2.0f > posModel.x - sizeModel.x / 2.0f &&
				posOld.x - size.x / 2.0f < posModel.x + sizeModel.x / 2.0f &&
				pos.z - size.z / 2.0f <= posModel.z + sizeModel.z / 2.0f &&
				posOld.z - size.z / 2.0f >= posModel.z + sizeModel.z / 2.0f)
			{
				//�ʒu��ݒ�
				pos.z = posModel.z + sizeModel.z / 2.0f + size.z / 1.95f;
				pScene->SetPos(pos);
			}
		}
		pObject = pSaveObject->GetObjNext(pSaveObject);
	}
	return bLand;
}

//=============================================================================
// �J���[�ݒ菈��
//=============================================================================
void CModelSingle::SetColor(D3DXCOLOR col)
{
	m_pModel->SetColor(col);
}