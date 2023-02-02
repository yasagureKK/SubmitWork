//=============================================================================
// �V���h�E���� [shadow.h]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "shadow.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CShadow::CShadow(int nPriority) :CScene3D(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Defaultsize = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bUse = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CShadow::~CShadow()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CShadow::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	//�ϐ��̏�����
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = pos;
	m_size = size;
	m_Defaultsize = size;
	m_bUse = false;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-m_size.x / 2.0f, 0.0f, +m_size.z / 2.0f);
	pVtx[1].pos = D3DXVECTOR3(+m_size.x / 2.0f, 0.0f, +m_size.z / 2.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x / 2.0f, 0.0f, -m_size.z / 2.0f);
	pVtx[3].pos = D3DXVECTOR3(+m_size.x / 2.0f, 0.0f, -m_size.z / 2.0f);

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
void CShadow::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)  //�k���`�F�b�N
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// �p�����N���X�̏I������
	CScene3D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CShadow::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CShadow::Draw(void)
{
	//�g�p���Ă��鎞�̂ݕ`�悳����
	if (m_bUse == true)
	{
		//�f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		//���C�e�B���O�𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		//���Z�����̐ݒ�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		//�A���t�@�e�X�g
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

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

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, m_pTexture);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		//�A���t�@�e�X�g
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);

		//���Z��������ʏ�̍����ɖ߂�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		//���C�e�B���O��L���ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
}

//=============================================================================
// ��������
//=============================================================================
CShadow* CShadow::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �N���X�̐���
	CShadow *pShadow = NULL;
	pShadow = new CShadow;

	if (pShadow != NULL)
	{
		// �G�t�F�N�g�̃e�N�X�`�����擾
		pShadow->m_pTexture = CManager::GetTexture()->GetTexture3D(0);
		pShadow->Init(pos, size);
	}
	return pShadow;
}

//=============================================================================
// �ʒu�̐ݒ菈��
//=============================================================================
void CShadow::SetShadow(D3DXVECTOR3 pos, float fRange)
{
	// �ʒu���X�V
	m_pos = pos;

	//�v���C���[�Ɖe�̈ʒu�̋����ɂ���ĉe��傫������
	m_size.x = m_Defaultsize.x + (fRange / 4.0f);
	m_size.z = m_Defaultsize.z + (fRange / 4.0f);

	int nAlpha = (int)(fRange / 3.0f);

	if (nAlpha >= 190)
	{
		nAlpha = 190;
	}

	D3DXCOLOR col = D3DCOLOR_RGBA(255, 255, 255, 205 - nAlpha);

	//�v���C���[�Ɖe�̈ʒu�̋����ɂ���ĉe�𔖂�����
	VERTEX_3D *pVtx;
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	pVtx[0].pos = D3DXVECTOR3(-m_size.x / 2.0f, 0.0f, +m_size.z / 2.0f);
	pVtx[1].pos = D3DXVECTOR3(+m_size.x / 2.0f, 0.0f, +m_size.z / 2.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x / 2.0f, 0.0f, -m_size.z / 2.0f);
	pVtx[3].pos = D3DXVECTOR3(+m_size.x / 2.0f, 0.0f, -m_size.z / 2.0f);
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �g�p��Ԃ̐ݒ菈��
//=============================================================================
void CShadow::SetUseShadow(bool bUse)
{
	m_bUse = bUse;
}