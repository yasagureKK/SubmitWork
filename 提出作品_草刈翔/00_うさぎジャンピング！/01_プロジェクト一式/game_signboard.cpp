//=============================================================================
// �Q�[�����̊ŔI�u�W�F�N�g�������� [game_signbpard.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "game_signboard.h"
#include "manager.h"
#include "texture.h"
#include "model_single.h"
#include "player.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CGameSignboard::CGameSignboard()
{
	// �ϐ��̃N���A
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	memset(&m_apModelSingle, NULL, sizeof(m_apModelSingle));
	m_nNumPlayer = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGameSignboard::~CGameSignboard()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CGameSignboard::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	m_pos = pos;
	m_size = size;

	if (m_apModelSingle == NULL)
	{
		m_apModelSingle = CModelSingle::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), CModelLoad::MODEL_TYPE_SIGNBOARD, CModelSingle::MODEL_SINGLE_TYPE_SIGNBOARD);
	}
	
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CGameSignboard::Uninit(void)
{
	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CGameSignboard::Update(void)
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
			if (m_nNumPlayer == pPlayer->GetNum())
			{
				// �v���C���[�̈ʒu���擾
				D3DXVECTOR3 pos = pPlayer->GetPos();

				// �ʒu�{�T�C�Y�̈ʒu�Ƀv���C���[�����������
				if (pos.x > m_pos.x + m_size.x)
				{
					// �ʒu�����炷
					m_pos.x = m_pos.x + m_size.x * 2;

					// ���f���̈ʒu��ݒ�
					m_apModelSingle->SetPos(m_pos);
				}
			}
		}
		// ���̃I�u�W�F�N�g���擾
		pObject = pSaveObject->GetObjNext(pSaveObject);
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void CGameSignboard::Draw(void)
{

}

//=============================================================================
// ��������
//=============================================================================
CGameSignboard *CGameSignboard::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nNumPlayer)
{
	// �C���X�^���X�̐���
	CGameSignboard *pGameSignboard = NULL;

	// �k���`�F�b�N
	if (pGameSignboard == NULL)
	{
		// �N���X�̐���
		pGameSignboard = new CGameSignboard;
		if (pGameSignboard != NULL)
		{
			// �ϐ��̏�����
			pGameSignboard->m_nNumPlayer = nNumPlayer;

			// ����������
			pGameSignboard->Init(pos, size);
		}
	}

	return pGameSignboard;
}