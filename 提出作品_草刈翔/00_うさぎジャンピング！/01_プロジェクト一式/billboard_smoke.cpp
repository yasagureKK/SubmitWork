//=============================================================================
// �r���{�[�h(�X���[�N)���� [billboard_smoke.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "billboard_smoke.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBillboardSmoke::CBillboardSmoke()
{
	// �ϐ��̃N���A
	m_nAlpha = 0;
	m_nLife = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBillboardSmoke::~CBillboardSmoke()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CBillboardSmoke::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �p�����N���X�̏���������
	CBillboard::Init(pos, size);

	// �ϐ��̏�����
	m_pos = pos;
	m_size = size;
	m_nAlpha = 0;

	// �㏸�ʂ�1.0�`2.5�̒l���烉���_���ɐݒ�
	m_move.y = float(rand() % 15 / 10) + 1.0f;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CBillboardSmoke::Uninit(void)
{
	// �p�����N���X�̏I������
	CBillboard::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CBillboardSmoke::Update(void)
{
	// �p�����N���X�̍X�V����
	CBillboard::Update();

	// �����x���グ�Ă���
	m_nAlpha+= 7;
	if (m_nAlpha >= 255)
	{
		m_nAlpha = 255;
	}
	SetColor(D3DCOLOR_RGBA(255, 255, 255, 255 - m_nAlpha));

	// �������s��������ł�����
	m_nLife--;
	if (m_nLife <= 0)
	{
		Uninit();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CBillboardSmoke::Draw(void)
{
	// �p�����N���X�̕`�揈��
	CBillboard::Draw();
}

//=============================================================================
// ��������
//=============================================================================
CBillboardSmoke* CBillboardSmoke::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nLife)
{
	// �C���X�^���X�̐���
	CBillboardSmoke *pBillboardSmoke = NULL;

	// �k���`�F�b�N
	if (pBillboardSmoke == NULL)
	{
		// �D�揇�ʂ��������ăN���X�𐶐�
		pBillboardSmoke = new CBillboardSmoke;
		if (pBillboardSmoke != NULL)
		{
			// �e�N�X�`���̊���
			pBillboardSmoke->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_SMOKE);
			pBillboardSmoke->BindTexture(pBillboardSmoke->m_pTexture);

			// �ϐ��̏�����
			pBillboardSmoke->m_nLife = nLife;

			// ����������
			pBillboardSmoke->Init(pos, size);
		}
	}

	return pBillboardSmoke;
}