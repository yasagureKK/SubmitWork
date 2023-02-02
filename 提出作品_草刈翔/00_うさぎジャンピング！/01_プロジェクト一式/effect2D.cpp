//=============================================================================
// 2D�̃G�t�F�N�g���� [effect_2D.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "effect2D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CEffect2D::CEffect2D(int nPriority) :CScene2D(nPriority)
{
	// �ϐ��̃N���A
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	m_nLife = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEffect2D::~CEffect2D()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CEffect2D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col)
{
	// �p�����N���X�̏���������
	CScene2D::Init(pos, size);

	// �ϐ��̏�����
	m_pos = pos;
	m_size = size;
	m_col = col;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEffect2D::Uninit(void)
{
	// �p�����N���X�̏I������
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CEffect2D::Update(void)
{
	// �ʒu�E�T�C�Y�擾
	D3DXVECTOR3 pos = GetPos();
	m_pos = pos;
	D3DXVECTOR3 size = GetSize();
	m_size = size;

	// �ړ��ʂ𔽉f
	m_pos += m_move;

	// �ʒu��ݒ�
	CScene2D::SetPos(m_pos, m_size);

	// �J���[�̐ݒ菈��
	SetColor(m_col);

	// ���������炷
	m_nLife--;

	// �������Ȃ��Ȃ�����
	if (m_nLife < 0)
	{
		// ���ł�����
		Uninit();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CEffect2D::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �A���t�@�e�X�g
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// �p�����N���X�̕`�揈��
	CScene2D::Draw();

	// �A���t�@�e�X�g
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);

	// ���u�����f�B���O��ʏ�ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// ��������
//=============================================================================
CEffect2D *CEffect2D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR3 move, int nLife)
{
	//�C���X�^���X�̐���
	CEffect2D *pScene2D = NULL;

	// �k���`�F�b�N
	if (pScene2D == NULL)
	{
		// �N���X�̐���
		pScene2D = new CEffect2D(PRIORITY_EFFECT);
		if (pScene2D != NULL)
		{
			// �e�N�X�`���̊���
			pScene2D->BindTexture(CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_EFFECT));

			// �ϐ��̐ݒ�
			pScene2D->m_move = move;
			pScene2D->m_nLife = nLife;

			// ����������
			pScene2D->Init(pos, size, col);
		}
	}
	return pScene2D;
}