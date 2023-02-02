//=============================================================================
// �r���{�[�h�̃G�t�F�N�g���� [billboard_effect.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "billboard_effect.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBillboardEffect::CBillboardEffect()
{
	// �ϐ��̃N���A
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_type = EFFECT_TYPE_NORMAL;
	m_nLife = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBillboardEffect::~CBillboardEffect()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CBillboardEffect::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col)
{
	// �p�����N���X�̏���������
	CBillboard::Init(pos, size);

	// �ϐ��̏�����
	m_pos = pos;
	m_size = size / 2;
	m_col = col;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CBillboardEffect::Uninit(void)
{
	// �p�����N���X�̏I������
	CBillboard::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CBillboardEffect::Update(void)
{
	// �p�����N���X�̍X�V����
	CBillboard::Update();

	// ���X�ɏ�����^�C�v�Ȃ�
	if (m_type == EFFECT_TYPE_VANISH)
	{
		m_col.a -= 0.05f;

		if (m_col.a <= 0)
		{
			m_col.a = 0;
		}
	}
	
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
void CBillboardEffect::Draw(void)
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
	CBillboard::Draw();

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
CBillboardEffect *CBillboardEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR3 move,
	int nLife, EFFECT_TYPE type)
{
	//�C���X�^���X�̐���
	CBillboardEffect *pBillboardEffect = NULL;

	// �k���`�F�b�N
	if (pBillboardEffect == NULL)
	{
		// �N���X�̐���
		pBillboardEffect = new CBillboardEffect;
		if (pBillboardEffect != NULL)
		{
			// �e�N�X�`���̊���
			pBillboardEffect->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_EFFECT);
			pBillboardEffect->BindTexture(pBillboardEffect->m_pTexture);

			// �ϐ��̐ݒ�
			pBillboardEffect->m_move = move;
			pBillboardEffect->m_nLife = nLife;
			pBillboardEffect->m_type = type;

			// ����������
			pBillboardEffect->Init(pos, size, col);
		}
	}
	return pBillboardEffect;
}