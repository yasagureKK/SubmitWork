//=============================================================================
// �r���{�[�h���� [billboard.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "billboard.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "shadow.h"
#include "model_single.h"
#include "mesh_field.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBillboard::CBillboard(int nPriority) :CScene3D(nPriority)
{
	// �ϐ��̃N���A
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pShadow = NULL;
	m_bShadow = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBillboard::~CBillboard()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CBillboard::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	
	// �ϐ��̏�����
	m_pos = pos;
	m_size = size;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	VERTEX_3D *pVtx;
	//���_�o�b�t�@�����b�N���A���_�f�[�^�̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W
	pVtx[0].pos = D3DXVECTOR3(-m_size.x / 2.0f, +m_size.y / 2.0f, +m_size.z / 2.0f);
	pVtx[1].pos = D3DXVECTOR3(+m_size.x / 2.0f, +m_size.y / 2.0f, +m_size.z / 2.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x / 2.0f, -m_size.y / 2.0f, -m_size.z / 2.0f);
	pVtx[3].pos = D3DXVECTOR3(+m_size.x / 2.0f, -m_size.y / 2.0f, -m_size.z / 2.0f);

	// �@��
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// �J���[
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`��
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CBillboard::Uninit(void)
{
	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CBillboard::Update(void)
{
	// �ʒu�擾
	D3DXVECTOR3 pos = GetPos();
	m_pos = pos;

	// �ړ��ʔ��f
	m_pos += m_move;

	// �e������Ȃ�
	if (m_bShadow == true)
	{
		// �e�̒Ǐ]����
		TrackingShadow();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CBillboard::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �A���t�@�e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView, mtxRot, mtxTrans, mtxScale;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �r���[�}�g���b�N�X���擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// �|���S�����J�����ɑ΂��Đ��ʂɌ�����
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// �A���t�@�e�X�g�𖳌���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// ��������
//=============================================================================
CBillboard* CBillboard::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nTexType, bool bShadow)
{
	// �N���X�̐���
	CBillboard *pBillboard = NULL;
	pBillboard = new CBillboard();

	if (pBillboard != NULL)
	{
		// �ϐ��̏�����
		pBillboard->m_pTexture = CManager::GetTexture()->GetTexture3D(nTexType);
		pBillboard->m_bShadow = bShadow;

		// ����������
		pBillboard->Init(pos, size);
	}
	return pBillboard;
}

//=============================================================================
// �ʒu�ݒ菈��
//=============================================================================
void CBillboard::SetPos(D3DXVECTOR3 pos)
{
	// �ʒu���X�V
	m_pos = pos;

	// ���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	// ���_���W���X�V
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	pVtx[0].pos = D3DXVECTOR3(-m_size.x / 2.0f, +m_size.y / 2.0f, +m_size.z / 2.0f);
	pVtx[1].pos = D3DXVECTOR3(+m_size.x / 2.0f, +m_size.y / 2.0f, +m_size.z / 2.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x / 2.0f, -m_size.y / 2.0f, -m_size.z / 2.0f);
	pVtx[3].pos = D3DXVECTOR3(+m_size.x / 2.0f, -m_size.y / 2.0f, -m_size.z / 2.0f);
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �T�C�Y�ݒ菈��
//=============================================================================
void CBillboard::SetSize(D3DXVECTOR3 size)
{
	// �T�C�Y���X�V
	m_size = size;

	// ���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	// ���_���W���X�V
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	pVtx[0].pos = D3DXVECTOR3(-m_size.x / 2.0f, +m_size.y / 2.0f, +m_size.z / 2.0f);
	pVtx[1].pos = D3DXVECTOR3(+m_size.x / 2.0f, +m_size.y / 2.0f, +m_size.z / 2.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x / 2.0f, -m_size.y / 2.0f, -m_size.z / 2.0f);
	pVtx[3].pos = D3DXVECTOR3(+m_size.x / 2.0f, -m_size.y / 2.0f, -m_size.z / 2.0f);
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �e�N�X�`���ݒ菈��
//=============================================================================
void CBillboard::SetTexture(float fU, float fV, int nCutU, int nCutV)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

	//�e�N�X�`���ݒ�(U���W�̈ʒu�AV���W�̈ʒu�A���̕������A�c�̕�����������)
	pVtx[0].tex = D3DXVECTOR2(fU * (1.0f / nCutU), fV * (1.0f / nCutV));
	pVtx[1].tex = D3DXVECTOR2(fU * (1.0f / nCutU) + (1.0f / nCutU), fV * (1.0f / nCutV));
	pVtx[2].tex = D3DXVECTOR2(fU * (1.0f / nCutU), fV * (1.0f / nCutV) + (1.0f / nCutV));
	pVtx[3].tex = D3DXVECTOR2(fU * (1.0f / nCutU) + (1.0f / nCutU), fV * (1.0f / nCutV) + (1.0f / nCutV));

	m_pVtxBuff->Unlock();
}

//=============================================================================
// �J���[�ݒ菈��
//=============================================================================
void CBillboard::SetColor(D3DXCOLOR col)
{
	// ���_���̃|�C���^
	VERTEX_3D *pVtx;

	// �F���X�V
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �e�̒Ǐ]����
//=============================================================================
void CBillboard::TrackingShadow(void)
{
	// �e���Ȃ�������
	if (m_pShadow == NULL)
	{
		// �e�̐������e���g�p���Ă����Ԃɐݒ�
		m_pShadow = CShadow::Create(D3DXVECTOR3(m_pos.x, m_pos.y - m_size.y, m_pos.z), D3DXVECTOR3(m_size.x, 0.0f, m_size.x));
		m_pShadow->SetUseShadow(true);
	}

	// �e������Ȃ�
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
					posObj.y + sizeObj.y >= m_pos.y + (m_size.y / 2) - 500.0f &&
					posObj.y + sizeObj.y <= m_pos.y + (m_size.y / 2))
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
// �e�̏�������
//=============================================================================
void CBillboard::DeleteShadow(void)
{
	// �e���g�p���Ă��������
	if (m_pShadow != NULL)
	{
		m_pShadow->Uninit();
		delete m_pShadow;
		m_pShadow = NULL;
	}
}

//=============================================================================
// ���_���W�ʒu�ݒ菈��
//=============================================================================
void CBillboard::SetVertex(D3DXVECTOR3 Vertex0, D3DXVECTOR3 Vertex1, D3DXVECTOR3 Vertex2, D3DXVECTOR3 Vertex3)
{
	// ���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	// �ʒu���X�V
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);
	pVtx[0].pos = D3DXVECTOR3(Vertex0.x, Vertex0.y, +m_size.z / 2.0f);
	pVtx[1].pos = D3DXVECTOR3(Vertex1.x, Vertex1.y, +m_size.z / 2.0f);
	pVtx[2].pos = D3DXVECTOR3(Vertex2.x, Vertex2.y, -m_size.z / 2.0f);
	pVtx[3].pos = D3DXVECTOR3(Vertex3.x, Vertex3.y, -m_size.z / 2.0f);
	m_pVtxBuff->Unlock();
}