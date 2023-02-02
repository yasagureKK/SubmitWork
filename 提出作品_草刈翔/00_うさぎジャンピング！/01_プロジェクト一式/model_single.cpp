//=============================================================================
// �P�̂̃��f������ [model_single.cpp]
// Authore : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "model_single.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "model_load.h"
#include "score_board.h"
#include "shadow.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CModelSingle::CModelSingle(int nPriority) :CScene(nPriority)
{
	// �ϐ��̃N���A
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pModel = NULL;
	m_modelType = MODEL_SINGLE_TYPE_NONE;
	m_pScoreBoard = NULL;
	m_pShadow = NULL;
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
HRESULT CModelSingle::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �ϐ��̏�����
	m_pos = pos;
	m_posOld = m_pos;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ���f���̐���
	m_pModel = CModel::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_rot, m_type);

	// �e���f���̐ݒ�(�P�̂̂���NULL������)
	m_pModel->SetParent(NULL);

	// �I�u�W�F�N�g�̎�ނ̐ݒ�
	SetObjType(CScene::OBJECT_TYPE_MODEL);

	// �T�C�Y���擾
	m_size = m_pModel->GetSize();
	SetSize(m_size);
	SetPos(m_pos);

	// �ŔȂ�X�R�A�������ɐ���
	if (m_modelType == MODEL_SINGLE_TYPE_SIGNBOARD)
	{
		m_pScoreBoard = CScoreBoard::Create(D3DXVECTOR3(pos.x, pos.y + 85.0f, pos.z - 10.0f),
			D3DXVECTOR3(40.0f, 40.0f, 0.0f), D3DCOLOR_RGBA(255, 200, 0, 255));
	}

	return S_OK;
}

//================================================
// �I������
//================================================
void CModelSingle::Uninit(void)
{
	// �e��������������Ă���
	if (m_pShadow != NULL)
	{
		m_pShadow->Uninit();
	}

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
	SetPosOld(m_posOld);

	// �ŔȂ�X�R�A�������ɐ���
	if (m_modelType == MODEL_SINGLE_TYPE_SIGNBOARD)
	{
		m_pScoreBoard->SetPos(D3DXVECTOR3(pos.x, pos.y + 85.0f, pos.z - 10.0f));
	}
}

//================================================
// �`�揈��
//================================================
void CModelSingle::Draw(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	//���f���̃��[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//���f���̌����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

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
CModelSingle *CModelSingle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelLoad::MODEL_TYPE type, MODEL_SINGLE_TYPE modelType)
{
	// �C���X�^���X�̐���
	CModelSingle *pModelSingle = NULL;

	// �k���`�F�b�N
	if (pModelSingle == NULL)
	{
		// �N���X�̐���
		pModelSingle = new CModelSingle;
		if (pModelSingle != NULL)
		{
			// �������������ꍇ
			switch (modelType)
			{
			case MODEL_SINGLE_TYPE_SIGNBOARD:
				break;

			default:
				break;
			}

			// �ϐ��̏�����
			pModelSingle->m_rot = rot;
			pModelSingle->m_type = type;
			pModelSingle->m_modelType = modelType;

			// ����������
			pModelSingle->Init(pos, pModelSingle->m_size);
		}
	}

	return pModelSingle;
}

//================================================
// �����蔻�菈��
//================================================
bool CModelSingle::Collision(CScene *pScene)
{
	bool bLand = false;	//���n���Ă��邩�ǂ���

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


			D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�Ώۂ̈ʒu
			D3DXVECTOR3 posModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//���f���̈ʒu
			D3DXVECTOR3 size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�Ώۂ̃T�C�Y
			D3DXVECTOR3 sizeModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//���f���̃T�C�Y
			D3DXVECTOR3 posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�Ώۂ�1�t���[���O�̈ʒu

																		//���f���̈ʒu�E�T�C�Y�擾
			posModel = pModelSingle->GetPos();
			sizeModel = pModelSingle->GetSize();

			//�Ώۂ̈ʒu�E�T�C�Y�擾
			pos = pScene->GetPos();
			posOld = pScene->GetPosOld();
			size = pScene->GetSize();

			if (pos.x + size.x / 2.0f > posModel.x - sizeModel.x / 2.0f &&
				pos.x - size.x / 2.0f < posModel.x + sizeModel.x / 2.0f &&
				pos.z + size.x / 2.0f > posModel.z - sizeModel.z / 2.0f &&
				pos.z - size.x / 2.0f < posModel.z + sizeModel.z / 2.0f &&
				pos.y <= posModel.y + sizeModel.y &&
				posOld.y >= posModel.y + sizeModel.y)
			{
				pos.y = posModel.y + sizeModel.y;
				//�ʒu��ݒ�
				pScene->SetPos(pos);

				bLand = true;
			}
			else if (pos.x + size.x / 2.0f > posModel.x - sizeModel.x / 2.0f &&
				pos.x - size.x / 2.0f < posModel.x + sizeModel.x / 2.0f &&
				pos.z + size.x / 2.0f > posModel.z - sizeModel.z / 2.0f &&
				pos.z - size.x / 2.0f < posModel.z + sizeModel.z / 2.0f &&
				pos.y + size.y >= posModel.y &&
				posOld.y + size.y <= posModel.y)
			{
				pos.y = posModel.y - size.y;
				//�ʒu��ݒ�
				pScene->SetPos(pos);
			}
			if (pos.y + size.y > posModel.y &&
				pos.y < posModel.y + sizeModel.y &&
				pos.z + size.x / 2.0f > posModel.z - sizeModel.z / 2.0f &&
				pos.z - size.x / 2.0f < posModel.z + sizeModel.z / 2.0f &&
				pos.x + size.x / 2.0f >= posModel.x - sizeModel.x / 2.0f &&
				posOld.x + size.x / 2.0f <= posModel.x - sizeModel.x / 2.0f)
			{
				pos.x = posModel.x - sizeModel.x / 2.0f - size.x / 2.0f;
				//�ʒu��ݒ�
				pScene->SetPos(pos);
			}
			else if (pos.y + size.y > posModel.y &&
				pos.y < posModel.y + sizeModel.y &&
				pos.z + size.x / 2.0f > posModel.z - sizeModel.z / 2.0f &&
				pos.z - size.x / 2.0f < posModel.z + sizeModel.z / 2.0f &&
				pos.x - size.x / 2.0f <= posModel.x + sizeModel.x / 2.0f &&
				posOld.x - size.x / 2.0f >= posModel.x + sizeModel.x / 2.0f)
			{
				pos.x = posModel.x + sizeModel.x / 2.0f + size.x / 2.0f;
				//�ʒu��ݒ�
				pScene->SetPos(pos);
			}
			if (pos.y + size.y > posModel.y &&
				pos.y < posModel.y + sizeModel.y &&
				pos.x + size.x / 2.0f > posModel.x - sizeModel.x / 2.0f &&
				pos.x - size.x / 2.0f < posModel.x + sizeModel.x / 2.0f &&
				pos.z + size.x / 2.0f >= posModel.z - sizeModel.z / 2.0f &&
				posOld.z + size.x / 2.0f <= posModel.z - sizeModel.z / 2.0f)
			{
				pos.z = posModel.z - sizeModel.z / 2.0f - size.x / 2.0f;
				//�ʒu��ݒ�
				pScene->SetPos(pos);
			}
			else if (pos.y + size.y > posModel.y &&
				pos.y < posModel.y + sizeModel.y &&
				pos.x + size.x / 2.0f > posModel.x - sizeModel.x / 2.0f &&
				pos.x - size.x / 2.0f < posModel.x + sizeModel.x / 2.0f &&
				pos.z - size.x / 2.0f <= posModel.z + sizeModel.z / 2.0f &&
				posOld.z - size.x / 2.0f >= posModel.z + sizeModel.z / 2.0f)
			{
				pos.z = posModel.z + sizeModel.z / 2.0f + size.x / 2.0f;
				//�ʒu��ݒ�
				pScene->SetPos(pos);
			}
		}
		pObject = pSaveObject->GetObjNext(pSaveObject);
	}
	return bLand;
}

//=============================================================================
// �e�̐ݒ菈��
//=============================================================================
void CModelSingle::SetShadow(bool bOnShadow, float fHeight)
{
	if (bOnShadow == true && m_pShadow == NULL)
	{
		// �e�̐������e���g�p���Ă����ԂɁ��e�̒Ǐ](�����ɃI�t�Z�b�g�����Ē����\)
		m_pShadow = CShadow::Create(D3DXVECTOR3(m_pos.x, m_pos.y - m_size.y, m_pos.z), D3DXVECTOR3(m_size.x + m_size.x / 3, 0.0f, m_size.x + m_size.x / 4), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_pShadow->SetUseShadow(true);
		m_pShadow->SetShadow(D3DXVECTOR3(m_pos.x, m_pos.y + fHeight, m_pos.z), 0.0f);
	}
}