//=============================================================================
// �r���{�[�h(���[��)���� [billboard_moon.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "billboard_moon.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "player.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBillboardMoon::CBillboardMoon()
{
	// �ϐ��̃N���A
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBillboardMoon::~CBillboardMoon()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CBillboardMoon::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nNum)
{
	// �p�����N���X�̏���������
	CBillboard::Init(pos, size);

	// �ϐ��̏�����
	m_pos = pos;
	m_size = size;
	m_nNum = nNum;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CBillboardMoon::Uninit(void)
{
	// �p�����N���X�̏I������
	CBillboard::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CBillboardMoon::Update(void)
{
	// �p�����N���X�̍X�V����
	CBillboard::Update();

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

			// �v���C���[�̈ʒu���擾
			D3DXVECTOR3 pos = pPlayer->GetPos();

			// �ԍ����v���C���[�̔ԍ��ƈ�v���Ă�����
			if (m_nNum == pPlayer->GetNum())
			{
				m_pos.x = pos.x + 500.0f;

				SetPos(m_pos, m_size);
			}
		}
		// ���̃I�u�W�F�N�g���擾
		pObject = pSaveObject->GetObjNext(pSaveObject);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CBillboardMoon::Draw(void)
{
	// �p�����N���X�̍X�V����
	CBillboard::Draw();
}

//=============================================================================
// ��������
//=============================================================================
CBillboardMoon *CBillboardMoon::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nNum)
{
	//�C���X�^���X�̐���
	CBillboardMoon *pBillboardMoon = NULL;

	// �k���`�F�b�N
	if (pBillboardMoon == NULL)
	{
		// �N���X�̐���
		pBillboardMoon = new CBillboardMoon;
		if (pBillboardMoon != NULL)
		{
			// �e�N�X�`���̊���
			pBillboardMoon->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_MOON);
			pBillboardMoon->BindTexture(pBillboardMoon->m_pTexture);

			// ����������
			pBillboardMoon->Init(pos, size, nNum);
		}
	}
	return pBillboardMoon;
}