//=============================================================================
// ���b�V��(�t�B�[���h)���� [mesh_field.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "mesh_field.h"
#include "manager.h"
#include "renderer.h"
#include "scene3D.h"
#include "player.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CMeshField::CMeshField(int nPriority) :CScene(nPriority)
{
	// �ϐ��̃N���A
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pIdxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nRow = 0;
	m_nLine = 0;
	m_nNum = 0;
	m_type = MESHFIELD_TYPE_NONE;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMeshField::~CMeshField()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CMeshField::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// �I�u�W�F�N�g�̎�ނ̐ݒ�
	SetObjType(CScene::OBJECT_TYPE_MODEL);

	// �ϐ��̏�����
	m_pos = pos;
	m_size = size;

	// �ʒu�E�T�C�Y�ݒ菈��
	CScene::SetPos(m_pos);
	CScene::SetSize(m_size);

	// ���_�o�b�t�@�̐���
	//��(�c�̕������{�P)�~(���̕������{�P)�̒l�̒��_�𐶐�����
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (m_nRow + 1) * (m_nLine + 1),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_�o�b�t�@�ւ̃|�C���^
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nCntVtx = 0;

	// ���_���W�̐ݒ�
	for (int nCntLine = 0; nCntLine < m_nLine + 1; nCntLine++)
	{
		for (int nCntRow = 0; nCntRow < m_nRow + 1; nCntRow++, nCntVtx++)
		{
			pVtx[nCntVtx].pos = D3DXVECTOR3((-m_size.x / 2.0f) + (nCntRow * (m_size.x / m_nRow)),
				m_pos.y,
				(m_size.z / 2.0f) - (nCntLine * (m_size.z / m_nLine)));

			// �@��
			pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

			// �J���[�̐ݒ�
			pVtx[nCntVtx].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			// �e�N�X�`��
			pVtx[nCntVtx].tex = D3DXVECTOR2(nCntRow * 1.0f, nCntLine * 1.0f);
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	// �C���f�b�N�X�o�b�t�@�̐���
	//��(���_�̍��v)+(�d�����Ēu���ꂽ���_)�̒l�𐶐�
	pDevice->CreateIndexBuffer(sizeof(WORD) * (((m_nRow + 1) * (m_nLine + 1)) + ((m_nRow + 3) * (m_nLine - 1))),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// �C���f�b�N�X���ւ̃|�C���^
	WORD *pIdx;

	// �C���f�b�N�X�o�b�t�@�����b�N���A�ԍ��f�[�^�ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void **)&pIdx, 0);

	// �ԍ��f�[�^�̐ݒ�
	for (int nCntLine = 0; nCntLine < m_nLine; nCntLine++)
	{
		for (int nCntRow = 0; nCntRow < m_nRow + 1; nCntRow++)
		{
			// ���̗��2���ݒ肷��
			// pIdx[(��̈ʒu)�~(���݂̉��̕�����)�{(���炷��)] = (���炷��)�{(��̈ʒu)�{(���̕�����+1�~���݂̉��̕�����)
			pIdx[(m_nRow * 2 + 4) * nCntLine + 0 + (nCntRow * 2)] = nCntRow + (m_nRow + 1) + (m_nRow + 1) * nCntLine;
			pIdx[(m_nRow * 2 + 4) * nCntLine + 1 + (nCntRow * 2)] = nCntRow + 0			   + (m_nRow + 1) * nCntLine;
		}
	}
	// �|���S����`�悳���Ȃ������̔ԍ��f�[�^�̐ݒ�
	for (int nCntLine = 0; nCntLine < m_nLine - 1; nCntLine++)
	{
		// pIdx[(��̈ʒu)�{(���炷��)] = (���̕�����)/(���̕�����+2�{���炷��)�{(���̕�����+1�~���݂̉��̕�����)
		pIdx[(m_nRow * 2 + 2) + 0 + nCntLine * (m_nRow * 2 + 4)] = m_nRow			 + (m_nRow + 1) * nCntLine;
		pIdx[(m_nRow * 2 + 2) + 1 + nCntLine * (m_nRow * 2 + 4)] = (m_nRow * 2 + 2)  + (m_nRow + 1) * nCntLine;
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N����
	m_pIdxBuff->Unlock();

	return S_OK;
}

//================================================
// �I������
//================================================
void CMeshField::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// �C���f�b�N�X�o�b�t�@�̔j��
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CMeshField::Update(void)
{
	if (m_type == MESHFIELD_TYPE_RIGHT_LOOP)
	{
		// �v���C���[��ǔ�
		TrackingPlayer();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CMeshField::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

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

	// �J�����O���s��
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
		0,
		0,
		((m_nRow + 1) * (m_nLine + 1)),					// ���_�̐�
		0,												// �J�n���钸�_�̃C���f�b�N�X
		(m_nRow * m_nLine * 2) + (m_nLine * 4) - 4);	// �`�悷��v���~�e�B�u��
}

//=============================================================================
// ��������
//=============================================================================
CMeshField* CMeshField::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, int nRow, int nLine, MESHFIELD_TYPE type, CTexture::TEX_TYPE tex)
{
	// �C���X�^���X�̐���
	CMeshField *pMeshField = NULL;

	// �k���`�F�b�N
	if (pMeshField == NULL)
	{
		// �N���X�̐���
		pMeshField = new CMeshField;
		if (pMeshField != NULL)
		{
			// �ϐ��̏�����
			pMeshField->m_rot = rot;
			pMeshField->m_nRow = nRow;
			pMeshField->m_nLine = nLine;
			pMeshField->m_type = type;
			pMeshField->m_pTexture = CManager::GetTexture()->GetTexture(tex);

			// ����������
			pMeshField->Init(pos, size);
		}
	}

	return pMeshField;
}

//=============================================================================
// �e�N�X�`����������
//=============================================================================
void CMeshField::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
// �ԍ��ݒ菈��
//=============================================================================
void CMeshField::SetNum(int nNum)
{
	m_nNum = nNum;
}

//=============================================================================
// �v���C���[�ǔ�����
//=============================================================================
void CMeshField::TrackingPlayer(void)
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

			// �ԍ����v���C���[�̔ԍ��ƈ�v���Ă�����
			if (m_nNum == pPlayer->GetNum())
			{
				// �v���C���[�̈ʒu���擾
				D3DXVECTOR3 pos = pPlayer->GetPos();

				if (pos.x > m_pos.x + m_size.x)
				{
 					m_pos.x += m_size.x * 2;
				}

				SetPos(m_pos);
			}
		}
		// ���̃I�u�W�F�N�g���擾
		pObject = pSaveObject->GetObjNext(pSaveObject);
	}
}

//================================================
//�����蔻�菈��
//================================================
bool CMeshField::Collision(CScene *pScene)
{
	//���n���Ă��邩�ǂ���
	bool bLand = false;

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
			//���f���V���O���ɃL���X�g
			CMeshField *pMeshField = NULL;
			pMeshField = (CMeshField*)pObject;

			D3DXVECTOR3 posField = pMeshField->GetPos();		//�n�ʂ̈ʒu
			D3DXVECTOR3 sizeField = pMeshField->GetSize();		//�n�ʂ̃T�C�Y
			D3DXVECTOR3 pos = pScene->GetPos();					//�Ώۂ̈ʒu
			D3DXVECTOR3 size = pScene->GetSize();				//�Ώۂ̃T�C�Y
			D3DXVECTOR3 posOld = pScene->GetPosOld();			//�Ώۂ�1�t���[���O�̈ʒu

			if (pos.x + size.x / 2.0f > posField.x - sizeField.x / 2.0f &&
				pos.x - size.x / 2.0f < posField.x + sizeField.x / 2.0f &&
				pos.z + size.x / 2.0f > posField.z - sizeField.z / 2.0f &&
				pos.z - size.x / 2.0f < posField.z + sizeField.z / 2.0f &&
				pos.y <= posField.y &&
				posOld.y >= posField.y)
			{
				pos.y = posField.y;
				//�ʒu��ݒ�
				pScene->SetPos(pos);

				bLand = true;
			}
		}
		pObject = pSaveObject->GetObjNext(pSaveObject);
	}
	return bLand;
}