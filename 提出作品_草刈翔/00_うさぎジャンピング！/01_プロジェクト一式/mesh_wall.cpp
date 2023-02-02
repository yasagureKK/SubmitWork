//=============================================================================
// ���b�V��(�E�H�[���\��)���� [mesh_wall.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "mesh_wall.h"
#include "manager.h"
#include "renderer.h"
#include "scene3D.h"
#include "player.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CMeshWall::CMeshWall(int nPriority) :CScene(nPriority)
{
	// �ϐ��̃N���A
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pIdxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nVertical = 0;
	m_nRow = 0;
	m_nNum = 0;
	m_type = MESHWALL_TYPE_NONE;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMeshWall::~CMeshWall()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CMeshWall::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// �ϐ��̏�����
	m_pos = pos;
	m_size = size;

	// �ʒu�E�T�C�Y�ݒ菈��
	CScene::SetPos(m_pos);
	CScene::SetSize(m_size);

	// ���_�o�b�t�@�̐���
	//��(�c�̕������{�P)�~(���̕������{�P)�̒l�̒��_�𐶐�����
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (m_nVertical + 1) * (m_nRow + 1),
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
	for (int nCntVertical = 0; nCntVertical < m_nVertical + 1; nCntVertical++)
	{
		for (int nCntRow = 0; nCntRow < m_nRow + 1; nCntRow++, nCntVtx++)
		{
			pVtx[nCntVtx].pos = D3DXVECTOR3((-m_size.x / 2.0f) + (nCntRow * (m_size.x / m_nRow)),
				(m_size.y / 2.0f) + (m_size.y / 2.0f) - (nCntVertical * (m_size.y / m_nVertical)),
				m_pos.z);

			// �@��
			pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

			// �J���[�̐ݒ�
			pVtx[nCntVtx].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			// �e�N�X�`��
			pVtx[nCntVtx].tex = D3DXVECTOR2(0.0f + (nCntRow * 1.0f), 0.0f + (nCntVertical * 1.0f));
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	// �C���f�b�N�X�o�b�t�@�̐���
	//��(���_�̍��v)+(�d�����Ēu���ꂽ���_)�̒l�𐶐�
	pDevice->CreateIndexBuffer(sizeof(WORD) * (((m_nRow + 1) * (m_nVertical + 1)) + ((m_nRow + 3) * (m_nVertical - 1))),
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
	for (int nCntVertical = 0; nCntVertical < m_nVertical; nCntVertical++)
	{
		for (int nCntRow = 0; nCntRow < m_nRow + 1; nCntRow++)
		{
			// ���̗��2���ݒ肷��
			// pIdx[(��̈ʒu)�~(���݂̉��̕�����)�{(���炷��)] = (���炷��)�{(��̈ʒu)�{(���̕�����+1�~���݂̏c�̕�����)
			pIdx[(m_nRow * 2 + 4) * nCntVertical + 0 + (nCntRow * 2)] = nCntRow + (m_nRow + 1) + (m_nRow + 1) * nCntVertical;
			pIdx[(m_nRow * 2 + 4) * nCntVertical + 1 + (nCntRow * 2)] = nCntRow + 0 + (m_nRow + 1) * nCntVertical;
		}
	}
	// �|���S����`�悳���Ȃ������̔ԍ��f�[�^�̐ݒ�
	for (int nCntVertical = 0; nCntVertical < m_nVertical - 1; nCntVertical++)
	{
		// pIdx[(��̈ʒu)�{(���炷��)] = (���̕�����)/(���̕�����+2�{���炷��)�{(���̕�����+1�~���݂̏c�̕�����)
		pIdx[(m_nRow * 2 + 2) + 0 + nCntVertical * (m_nRow * 2 + 4)] = m_nRow + (m_nRow + 1) * nCntVertical;
		pIdx[(m_nRow * 2 + 2) + 1 + nCntVertical * (m_nRow * 2 + 4)] = (m_nRow * 2 + 2) + (m_nRow + 1) * nCntVertical;
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N����
	m_pIdxBuff->Unlock();

	return S_OK;
}

//================================================
// �I������
//================================================
void CMeshWall::Uninit(void)
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
void CMeshWall::Update(void)
{
	if (m_type == MESHWALL_TYPE_RIGHT_LOOP)
	{
		// �v���C���[��ǔ�
		TrackingPlayer();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CMeshWall::Draw(void)
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
		((m_nRow + 1) * (m_nVertical + 1)),						// ���_�̐�
		0,														// �J�n���钸�_�̃C���f�b�N�X
		(m_nRow * m_nVertical * 2) + (m_nVertical * 4) - 4);	// �`�悷��v���~�e�B�u��
}

//=============================================================================
// ��������
//=============================================================================
CMeshWall* CMeshWall::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, int nVertical, int nRow, MESHWALL_TYPE type, CTexture::TEX_TYPE tex)
{
	// �C���X�^���X�̐���
	CMeshWall *pMeshWall = NULL;

	// �k���`�F�b�N
	if (pMeshWall == NULL)
	{
		// �N���X�̐���
		pMeshWall = new CMeshWall;
		if (pMeshWall != NULL)
		{
			// �ϐ��̏�����
			pMeshWall->m_rot = rot;
			pMeshWall->m_nVertical = nVertical;
			pMeshWall->m_nRow = nRow;
			pMeshWall->m_type = type;
			pMeshWall->m_pTexture = CManager::GetTexture()->GetTexture(tex);

			// ����������
			pMeshWall->Init(pos, size);
		}
	}

	return pMeshWall;
}

//=============================================================================
// �e�N�X�`����������
//=============================================================================
void CMeshWall::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
// �ԍ��ݒ菈��
//=============================================================================
void CMeshWall::SetNum(int nNum)
{
	m_nNum = nNum;
}

//=============================================================================
// �v���C���[�ǔ�����
//=============================================================================
void CMeshWall::TrackingPlayer(void)
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