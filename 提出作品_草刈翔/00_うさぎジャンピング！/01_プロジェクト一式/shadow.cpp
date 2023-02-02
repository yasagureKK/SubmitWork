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
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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
	m_pos = pos;
	m_size = size;
	m_Defaultsize = size;
	m_bUse = false;

	// �p�����N���X�̏���������
	CScene3D::Init(pos, size);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CShadow::Uninit(void)
{
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
		LPDIRECT3DDEVICE9 pDevice;
		pDevice = CManager::GetRenderer()->GetDevice();

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

		// �p�����N���X�̕`�揈��
		CScene3D::Draw();

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
CShadow* CShadow::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot)
{
	//�C���X�^���X�̐���
	CShadow *pShadow = NULL;
	if (pShadow == NULL)
	{
		pShadow = new CShadow;
		if (pShadow != NULL)
		{
			pShadow->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_EFFECT);
			pShadow->m_rot = rot;
			pShadow->Init(pos, size);
		}
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

	int nAlpha = (int)(fRange / 2.0f);

	if (nAlpha >= 205)
	{
		nAlpha = 205;
	}

	//�v���C���[�Ɖe�̈ʒu�̋����ɂ���ĉe�𔖂�����
	SetColor(D3DCOLOR_RGBA(255, 255, 255, 205 - nAlpha));

	//�ʒu�E�T�C�Y�ݒ菈��
	CScene3D::SetPos(m_pos, m_size);
}

//=============================================================================
// �g�p��Ԃ̐ݒ菈��
//=============================================================================
void CShadow::SetUseShadow(bool bUse)
{
	m_bUse = bUse;
}