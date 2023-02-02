//=============================================================================
// �K�C�h���� [jump_guide.cpp]
// Authore : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "jump_guide.h"
#include "number.h"
#include "manager.h"
#include "scene.h"
#include "player.h"
#include "ui.h"
#include "texture.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CJumpGuide::CJumpGuide(int nPriority) : CScene(nPriority)
{
	memset(&m_apNumber, NULL, sizeof(m_apNumber));
	memset(&m_apUi, NULL, sizeof(m_apUi));
	m_nPos = 0;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	m_nNum = 0;
	m_bChangeTex = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CJumpGuide::~CJumpGuide()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CJumpGuide::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �ϐ��̏�����
	m_pos = pos;
	m_size = size;
	m_bChangeTex = false;

	// �����̐���
	m_apUi[0] = CUi::Create(D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y - m_size.y / 4 * 3, 0.0f),
		D3DXVECTOR3(200.0f, 60.0f, 0.0f), PRIORITY_UI, CUi::UI_TYPE_GAME_JUMP_GUIDE);

	for (int nCntScore = 0; nCntScore < MAX_SCORE_JUMP_GUIDE_DIGIT; nCntScore++)
	{
		// �i���o�[�̐���
		m_apNumber[nCntScore] = CNumber::Create(D3DXVECTOR3(m_pos.x + (nCntScore * (m_size.x / 5 * 4)), m_pos.y, m_pos.z), m_size, m_col);

		// �Ō�̎��������[�g��������
		if (nCntScore == MAX_SCORE_JUMP_GUIDE_DIGIT - 1)
		{
			// m(���[�g��)�̐���
			m_apUi[1] = CUi::Create(D3DXVECTOR3(m_pos.x + (nCntScore * m_size.x) + (m_size.x / 5 * 2), m_pos.y + (m_size.y / 8), 0.0f),
				D3DXVECTOR3(m_size.x * 0.8f, m_size.x * 0.8f, 0.0f), PRIORITY_UI, CUi::UI_TYPE_METER);
			m_apUi[1]->SetColor(m_col);
		}
	}

	// �ʒu�E�T�C�Y�̐ݒ�
	SetPos(m_pos);
	SetSize(m_size);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CJumpGuide::Uninit(void)
{
	//�i���o�[�̔j��
	for (int nCntScore = 0; nCntScore < MAX_SCORE_JUMP_GUIDE_DIGIT; nCntScore++)
	{
		// �k���`�F�b�N
		if (m_apNumber[nCntScore] != NULL)
		{
			m_apNumber[nCntScore]->Uninit();
			delete m_apNumber[nCntScore];
			m_apNumber[nCntScore] = NULL;
		}
	}

	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CJumpGuide::Update(void)
{
	for (int nCntNumber = 0; nCntNumber < MAX_SCORE_JUMP_GUIDE_DIGIT; nCntNumber++)
	{
		if (nCntNumber == MAX_SCORE_JUMP_GUIDE_DIGIT - 1)
		{
			// �Ō��10��n�悪�g���Ȃ��Ȃ邽�߁A���ߑł��ŏ���
			m_apNumber[nCntNumber]->SetNumber(m_nPos % 10 / 1);
		}
		else
		{
			// ���X�R�A % 10�~n / 10�~(n-1)�� �ŏo���l�����ꂼ��̃i���o�[�ɐݒ�
			//��powf(float x, float y) �c x��y��̒l
			m_apNumber[nCntNumber]->SetNumber(m_nPos % (int)powf(10, MAX_SCORE_JUMP_GUIDE_DIGIT - (float)nCntNumber) / (int)powf(10, MAX_SCORE_JUMP_GUIDE_DIGIT - ((float)nCntNumber + 1)));
		}
	}

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

			// �X�R�A�̔ԍ����v���C���[�̔ԍ��ƈ�v���Ă�����
			if (m_nNum == pPlayer->GetNum() && pPlayer->GetLandFinish() == false)
			{
				// �v���C���[�̈ʒu���擾
				D3DXVECTOR3 pos = pPlayer->GetPos();

				// �v���C���[���W�����v����O
				if (pos.x < 0)
				{
					// -1�������A�v���C���[��X�����̈ʒu��ݒ�
					m_nPos = -(int)pos.x / 20;

					// �ʒu���WX���X�V
					SetPosValue(m_nPos);
				}
				// ���ݐ؂���߂��āA�W�����v���Ă�����
				else if (pos.x >= 0 && pPlayer->GetJump() == true)
				{
					if (m_bChangeTex == false)
					{
						// �摜��؂�ւ�
						m_apUi[0]->BindTexture(CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_GAME_JUMP_GUIDE01));
						m_bChangeTex = true;
					}
					
					// �v���C���[��Y�����̈ʒu��ݒ�
					m_nPos = (int)pos.y / 20;

					// �ʒu���WY���X�V
					SetPosValue(m_nPos);
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
void CJumpGuide::Draw(void)
{
	for (int nCntScore = 0; nCntScore < MAX_SCORE_JUMP_GUIDE_DIGIT; nCntScore++)
	{
		// �k���`�F�b�N
		if (m_apNumber[nCntScore] != NULL)
		{
			m_apNumber[nCntScore]->Draw();
		}
	}
}

//=============================================================================
// ��������
//=============================================================================
CJumpGuide *CJumpGuide::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col)
{
	// �C���X�^���X�̐���
	CJumpGuide *pJumpGuide = NULL;

	// �k���`�F�b�N
	if (pJumpGuide == NULL)
	{
		pJumpGuide = new CJumpGuide;
		if (pJumpGuide != NULL)
		{
			// �ϐ��̐ݒ�
			pJumpGuide->m_col = col;

			// ����������
			pJumpGuide->Init(pos, size);
		}
	}
	return pJumpGuide;
}

//=============================================================================
// �ʒu���W�̒l�ݒ菈��
//=============================================================================
void CJumpGuide::SetPosValue(int nPos)
{
	m_nPos = nPos;
}

//=============================================================================
// �ʒu���W�̒l�擾����
//=============================================================================
int CJumpGuide::GetPosValue(void)
{
	return m_nPos;
}

//=============================================================================
// �X�R�A�ԍ��ݒ菈��
//=============================================================================
void CJumpGuide::SetNum(int nNum)
{
	m_nNum = nNum;
}

//=============================================================================
// �X�R�A�ԍ��擾����
//=============================================================================
int CJumpGuide::GetNum(void)
{
	return m_nNum;
}