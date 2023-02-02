//=============================================================================
// ���b�V��(��)���� [mesh_sky.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "mesh_sky.h"
#include "manager.h"
#include "renderer.h"
#include "scene3D.h"
#include "texture.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CMeshSky::CMeshSky(int nPriority) :CScene3D(nPriority)
{
	// �ϐ��̃N���A
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pIdxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nVertical = 0;
	m_nSide = 0;
	m_fSpin = 0.0f;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMeshSky::~CMeshSky()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CMeshSky::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �ϐ��̏�����
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = pos;
	m_size = size;
	m_nVertical = 128;
	m_nSide = 32;

	// ���_�o�b�t�@�̐���
	//��(�c�̕������{�P)�~(���̕������{�P)�̒l�̒��_�𐶐�����
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * ((m_nVertical + 1) * (m_nSide + 1)),
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
	for (int nCntSide = 0; nCntSide < m_nSide + 1; nCntSide++)
	{
		for (int nCntVertical = 0; nCntVertical < m_nVertical + 1; nCntVertical++, nCntVtx++)
		{
			pVtx[nCntVtx].pos = D3DXVECTOR3(cosf((D3DX_PI * 2.0f / m_nVertical) * nCntVertical) * (sinf(((D3DX_PI * 2.0f / m_nVertical) * nCntSide) + ((D3DX_PI * 2.0f / m_nVertical) * m_nSide)) * (m_size.x / 2.0f)),
				m_pos.y + sinf((D3DX_PI * 2.0f / m_nVertical) * nCntSide) * (m_size.x / 2.0f),
				sinf((D3DX_PI * 2.0f / m_nVertical) * nCntVertical) * (sinf(((D3DX_PI * 2.0f / m_nVertical) * nCntSide) + ((D3DX_PI * 2.0f / m_nVertical) * m_nSide)) * (m_size.x / 2.0f)));

			// �@��
			pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

			// �J���[
			pVtx[nCntVtx].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			// �e�N�X�`��
			pVtx[nCntVtx].tex = D3DXVECTOR2(0.0f + -(1.0f / m_nVertical * nCntVertical), 0.0f + -(1.0f / m_nSide * nCntSide));
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * ((m_nSide - 1) * (m_nVertical + 3) + (m_nVertical + 1) * (m_nSide + 1)),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// �C���f�b�N�X���ւ̃|�C���^
	WORD *pIdx;

	// �C���f�b�N�X�o�b�t�@�����b�N���A�ԍ��f�[�^�ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// �ԍ��f�[�^�̐ݒ�
	for (int nCntSide = 0; nCntSide < m_nSide; nCntSide++)
	{
		for (int nCntVertical = 0; nCntVertical < m_nVertical + 1; nCntVertical++)
		{
			pIdx[(nCntVertical * 2) + 0 + (m_nVertical + 2) * 2 * nCntSide] = (m_nVertical + 1) + nCntVertical + (m_nVertical + 1) * nCntSide;
			pIdx[(nCntVertical * 2) + 1 + (m_nVertical + 2) * 2 * nCntSide] = 0 + nCntVertical + (m_nVertical + 1) * nCntSide;
		}
	}

	for (int nCntSide = 0; nCntSide < m_nSide - 1; nCntSide++)
	{
		pIdx[((m_nVertical + 1) * 2 + 0) * (nCntSide + 1) + (2 * nCntSide)] = m_nVertical + (m_nVertical + 1) * nCntSide;
		pIdx[((m_nVertical + 1) * 2 + 1) * (nCntSide + 1) + (1 * nCntSide)] = m_nVertical * 2 + 2 + (m_nVertical + 1) * nCntSide;
	}

	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	m_pIdxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CMeshSky::Uninit(void)
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
void CMeshSky::Update(void)
{
	// ��]������
	m_rot.y += m_fSpin;

	// ������3.14���璴���Ȃ��悤�ɂ���
	if (m_rot.y >= D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CMeshSky::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

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

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
		0,
		0,
		((m_nVertical + 1) * (m_nSide + 1)),				//���_�̐�
		0,													//�J�n���钸�_�̃C���f�b�N�X
		(m_nSide * m_nVertical * 2) + (m_nSide * 4) - 4);	//�`�悷��v���~�e�B�u��

	//���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// ��������
//=============================================================================
CMeshSky* CMeshSky::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, float fSpin, int nTexType)
{
	// �N���X�̐���
	CMeshSky *pMeshSphere = NULL;
	pMeshSphere = new CMeshSky;
	
	if (pMeshSphere != NULL)
	{
		// �ϐ��̏�����
		pMeshSphere->m_fSpin = fSpin;
		pMeshSphere->m_pTexture = CManager::GetTexture()->GetTexture3D(nTexType);

		// ����������
		pMeshSphere->Init(pos, size);
	}
	return pMeshSphere;
}

//=============================================================================
// �e�N�X�`�����蓖�ď���
//=============================================================================
void CMeshSky::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}