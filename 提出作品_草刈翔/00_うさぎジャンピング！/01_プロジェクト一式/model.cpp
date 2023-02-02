//=============================================================================
// ���f������ [model.cpp]
// Aouthor : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "model.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CModel::CModel()
{
	// �ϐ��̃N���A
	m_pMesh = NULL;
	m_pBuffMat = NULL;
	m_nNumMat = 0;
	memset(&m_pTexture, NULL, sizeof(m_pTexture));
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pParent = NULL;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CModel::~CModel()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CModel::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelLoad::MODEL_TYPE type)
{
	// ���_�̍ő�l�E�ŏ��l�͌�ōČv�Z���邽�ߋɒ[�Ȑ����ɂ��Ă���
	m_vtxMin = D3DXVECTOR3(100000.0f, 100000.0f, 100000.0f);
	m_vtxMax = D3DXVECTOR3(-100000.0f, -100000.0f, -100000.0f);

	// �ϐ��̏�����
	m_rot = rot;
	m_pos = pos;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// X�t�@�C���̏��ǂݍ���
	m_pBuffMat = CManager::GetModelLoad()->GetBuffMat(type);
	m_nNumMat = CManager::GetModelLoad()->GetNumMat(type);
	m_pMesh = CManager::GetModelLoad()->GetMesh(type);

	// �}�e���A���ւ̃|�C���^
	D3DXMATERIAL *pMat;

	// �}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL *)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			// �e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&m_pTexture[nCntMat]);
		}
	}

	// ���_���̎擾
	int nNumVtx;		// ���_��
	DWORD sizeFVF;		// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^

						// ���_�����擾
	nNumVtx = m_pMesh->GetNumVertices();

	// ���_�t�H�[�}�b�g�̃T�C�Y���擾
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	// ���_�o�b�t�@�����b�N
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		// ���_���W�̑��
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		// �S�Ă̒��_���r���čő�l�ƍŏ��l���o��
		// �ő�l
		if (vtx.x >= m_vtxMax.x)
		{
			m_vtxMax.x = vtx.x;
		}
		if (vtx.y >= m_vtxMax.y)
		{
			m_vtxMax.y = vtx.y;
		}
		if (vtx.z >= m_vtxMax.z)
		{
			m_vtxMax.z = vtx.z;
		}
		// �ŏ��l
		if (vtx.x <= m_vtxMin.x)
		{
			m_vtxMin.x = vtx.x;
		}
		if (vtx.y <= m_vtxMin.y)
		{
			m_vtxMin.y = vtx.y;
		}
		if (vtx.z <= m_vtxMin.z)
		{
			m_vtxMin.z = vtx.z;
		}

		// ���f���̃T�C�Y
		m_size.x = m_vtxMax.x - m_vtxMin.x;
		m_size.y = m_vtxMax.y - m_vtxMin.y;
		m_size.z = m_vtxMax.z - m_vtxMin.z;

		// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		pVtxBuff += sizeFVF;
	}

	// ���_�̍ő�l�ƍŏ��l����8���_�̈ʒu�����߂�
	m_vtxPos[0].x = m_vtxMin.x;
	m_vtxPos[0].z = m_vtxMax.z;
	m_vtxPos[0].y = m_vtxMax.y;
	m_vtxPos[1].x = m_vtxMax.x;
	m_vtxPos[1].z = m_vtxMax.z;
	m_vtxPos[1].y = m_vtxMax.y;
	m_vtxPos[2].x = m_vtxMin.x;
	m_vtxPos[2].z = m_vtxMax.z;
	m_vtxPos[2].y = m_vtxMin.y;
	m_vtxPos[3].x = m_vtxMax.x;
	m_vtxPos[3].z = m_vtxMax.z;
	m_vtxPos[3].y = m_vtxMin.y;
	m_vtxPos[4].x = m_vtxMin.x;
	m_vtxPos[4].z = m_vtxMin.z;
	m_vtxPos[4].y = m_vtxMax.y;
	m_vtxPos[5].x = m_vtxMax.x;
	m_vtxPos[5].z = m_vtxMin.z;
	m_vtxPos[5].y = m_vtxMax.y;
	m_vtxPos[6].x = m_vtxMin.x;
	m_vtxPos[6].z = m_vtxMin.z;
	m_vtxPos[6].y = m_vtxMin.y;
	m_vtxPos[7].x = m_vtxMax.x;
	m_vtxPos[7].z = m_vtxMin.z;
	m_vtxPos[7].y = m_vtxMin.y;

	// ���_�o�b�t�@���A�����b�N
	m_pMesh->UnlockVertexBuffer();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CModel::Uninit(void)
{
	// ���b�V���̔j��
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	// �}�e���A���̔j��
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	// �e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < MAX_MODEL_TEXTURE; nCntTex++)
	{
		if (m_pTexture[nCntTex] != NULL)
		{
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CModel::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CModel::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;							// �}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATRIX mtxParent;						// �e�̃}�g���b�N�X

												// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �e�̃��f���̃}�g���b�N�X�̐ݒ�
	if (m_pParent != NULL)
	{
		mtxParent = m_pParent->GetMtx();
	}
	// �e�̃��f���������ꍇ
	else
	{
		// ���[���h�}�g���b�N�X���擾
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	// �Z�o�����p�[�c�̃��[���h�}�g���b�N�X�Ɛe�̃��[���h�}�g���b�N�X���|�����킹��
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// ���ꂼ��̃p�[�c�Ƀ}�e���A���E�e�N�X�`����ݒ肵�ĕ`��
	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, m_pTexture[nCntMat]);

		//���f��(�p�[�c)�̕`��
		m_pMesh->DrawSubset(nCntMat);
	}

	//�ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// ��������
//=============================================================================
CModel* CModel::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelLoad::MODEL_TYPE type)
{
	// �C���X�^���X�̐���
	CModel *pModel = NULL;

	// �k���`�F�b�N
	if (pModel == NULL)
	{
		// �N���X�̐���
		pModel = new CModel;
		if (pModel != NULL)
		{
			// ����������
			pModel->Init(pos, rot, type);
		}
	}

	return pModel;
}

//=============================================================================
// �e���f���ݒ菈��
//=============================================================================
void CModel::SetParent(CModel *pModel)
{
	m_pParent = pModel;
}

//=============================================================================
// ���[���h�}�g���b�N�X�擾����
//=============================================================================
D3DXMATRIX CModel::GetMtx(void)
{
	return m_mtxWorld;
}

//================================================
// �ʒu�ݒ菈��
//================================================
void CModel::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//================================================
// �ʒu�擾����
//================================================
D3DXVECTOR3 CModel::GetPos(void)
{
	return m_pos;
}

//================================================
// �����ݒ菈��
//================================================
void CModel::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//================================================
// �����擾����
//================================================
D3DXVECTOR3 CModel::GetRot(void)
{
	return m_rot;
}

//=============================================================================
// �T�C�Y�擾����
//=============================================================================
D3DXVECTOR3 CModel::GetSize(void)
{
	return m_size;
}