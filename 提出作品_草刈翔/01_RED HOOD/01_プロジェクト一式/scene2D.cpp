//=============================================================================
// 2D�I�u�W�F�N�g���� [scene2D.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene2D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CScene2D::CScene2D(int nPriority) : CScene(nPriority)
{
	// �ϐ��̃N���A
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_fRadius = 0.0f;
	m_fTilt = 0.0f;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScene2D::~CScene2D()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CScene2D::Init(D3DXVECTOR3 pos)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �ϐ��̏�����
	m_fRadius = 0.0f;
	m_fTilt = 0.0f;

	// �ʒu�̐ݒ�
	m_pos = pos;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	// ���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - (m_size.x / 2), m_pos.y - (m_size.y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (m_size.x / 2), m_pos.y - (m_size.y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - (m_size.x / 2), m_pos.y + (m_size.y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (m_size.x / 2), m_pos.y + (m_size.y / 2), 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

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
void CScene2D::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)  //�k���`�F�b�N
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
void CScene2D::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CScene2D::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �t�H�O�̉e�����󂯂Ă��܂��̂Ńt�H�O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// ��������
//=============================================================================
CScene2D *CScene2D::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nPriority)
{
	// �N���X�̐���
	CScene2D *pScene2D = NULL;
	pScene2D = new CScene2D(nPriority);

	if (pScene2D != NULL)
	{
		// ����������
		pScene2D->Init(pos);
	}
	return pScene2D;
}

//=============================================================================
// �T�C�Y�ݒ菈��
//=============================================================================
void CScene2D::SetPos(D3DXVECTOR3 pos)
{
	// �ʒu���X�V
	m_pos = pos;

	// ���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	// �ʒu���X�V
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - (m_size.x / 2), m_pos.y - (m_size.y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (m_size.x / 2), m_pos.y - (m_size.y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - (m_size.x / 2), m_pos.y + (m_size.y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (m_size.x / 2), m_pos.y + (m_size.y / 2), 0.0f);
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �T�C�Y�ݒ菈��
//=============================================================================
void CScene2D::SetSize(D3DXVECTOR2 size)
{
	// �T�C�Y���X�V
	m_size = size;

	// ���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	// �ʒu���X�V
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - (m_size.x / 2), m_pos.y - (m_size.y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (m_size.x / 2), m_pos.y - (m_size.y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - (m_size.x / 2), m_pos.y + (m_size.y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (m_size.x / 2), m_pos.y + (m_size.y / 2), 0.0f);
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �e�N�X�`�����W�ݒ菈��
//=============================================================================
void CScene2D::SetTexture(float fU, float fV, int nCutU, int nCutV)
{
	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	// �e�N�X�`�����W���X�V
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);
	pVtx[0].tex = D3DXVECTOR2(fU * (1.0f / nCutU),					fV * (1.0f / nCutV));
	pVtx[1].tex = D3DXVECTOR2(fU * (1.0f / nCutU) + (1.0f / nCutU), fV * (1.0f / nCutV));
	pVtx[2].tex = D3DXVECTOR2(fU * (1.0f / nCutU),					fV * (1.0f / nCutV) + (1.0f / nCutV));
	pVtx[3].tex = D3DXVECTOR2(fU * (1.0f / nCutU) + (1.0f / nCutU), fV * (1.0f / nCutV) + (1.0f / nCutV));
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �e�N�X�`����������
//=============================================================================
void CScene2D::BindTexture(int nTexType)
{
	// �e�N�X�`���̊��蓖��
	m_pTexture = CManager::GetTexture()->GetTexture2D(nTexType);
}

//=============================================================================
// �J���[�ݒ菈��
//=============================================================================
void CScene2D::SetColor(D3DXCOLOR col)
{
	// ���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

	// �J���[���X�V
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// ���_���W�ʒu�ݒ菈��
//=============================================================================
void CScene2D::SetBesideBarScale(float fScale, bool bReverse)
{
	m_size.x = fScale;

	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (bReverse == false)
	{
		//���_�̍��W
		pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y - (m_size.y / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y - (m_size.y / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + (m_size.y / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + (m_size.y / 2), 0.0f);
	}
	else if (bReverse == true)
	{
		//���_�̍��W
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y - (m_size.y / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x, m_pos.y - (m_size.y / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y + (m_size.y / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x, m_pos.y + (m_size.y / 2), 0.0f);
	}

	m_pVtxBuff->Unlock();
}

//=============================================================================
// �X���ݒ菈��
//=============================================================================
void CScene2D::SetTilt(float fAngle)
{
	m_fRadius = sqrtf(m_size.x * m_size.x + m_size.y * m_size.y);
	m_fTilt = atan2f(m_size.x, m_size.y);

	// ���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	// �摜���X����
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fRadius * cosf(m_fTilt - fAngle), m_pos.y - m_fRadius * sinf(m_fTilt - fAngle), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fRadius * cosf(m_fTilt + fAngle), m_pos.y - m_fRadius * sinf(m_fTilt + fAngle), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fRadius * cosf(m_fTilt + fAngle), m_pos.y + m_fRadius * sinf(m_fTilt + fAngle), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fRadius * cosf(m_fTilt - fAngle), m_pos.y + m_fRadius * sinf(m_fTilt - fAngle), 0.0f);
	m_pVtxBuff->Unlock();
}