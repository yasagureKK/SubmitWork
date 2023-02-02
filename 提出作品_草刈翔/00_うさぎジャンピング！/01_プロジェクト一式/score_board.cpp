//=============================================================================
// �Ŕɕ\��������X�R�A���� [score_board.cpp]
// Authore : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "score_board.h"
#include "number3D.h"
#include "scene.h"
#include "player.h"
#include "ui.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CScoreBoard::CScoreBoard(int nPriority) : CScene3D(nPriority)
{
	memset(&m_apNumber3D, NULL, sizeof(m_apNumber3D));
	m_nScore = 0;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScoreBoard::~CScoreBoard()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CScoreBoard::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DCOLOR col)
{
	// �ϐ��̏�����
	m_pos = pos;
	m_size = size;

	for (int nCntScore = 0; nCntScore < MAX_SCORE_BOARD_DIGIT; nCntScore++)
	{
		// �i���o�[�̐���
		m_apNumber3D[nCntScore] = CNumber3D::Create(D3DXVECTOR3(m_pos.x + (nCntScore * (m_size.x - 10.0f)) - (1.5f * (m_size.x - 10.0f)), m_pos.y, m_pos.z), m_size, D3DXVECTOR3(0.0f, 0.0f, 0.0f), col);
	}

	m_nScore = (int)m_pos.x / 20;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CScoreBoard::Uninit(void)
{
	//�i���o�[�̔j��
	//for (int nCntScore = 0; nCntScore < MAX_SCORE_BOARD_DIGIT; nCntScore++)
	//{
	//	// �k���`�F�b�N
	//	if (m_apNumber3D[nCntScore] != NULL)
	//	{
	//		m_apNumber3D[nCntScore]->Uninit();
	//		delete m_apNumber3D[nCntScore];
	//		m_apNumber3D[nCntScore] = NULL;
	//	}
	//}

	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CScoreBoard::Update(void)
{
	for (int nCntNumber = 0; nCntNumber < MAX_SCORE_BOARD_DIGIT; nCntNumber++)
	{
		if (nCntNumber == MAX_SCORE_BOARD_DIGIT - 1)
		{
			// �Ō��10��n�悪�g���Ȃ��Ȃ邽�߁A���ߑł��ŏ���
			m_apNumber3D[nCntNumber]->SetNumber(m_nScore % 10 / 1);
		}
		else
		{
			// ���X�R�A % 10�~n / 10�~(n-1)�� �ŏo���l�����ꂼ��̃i���o�[�ɐݒ�
			//��powf(float x, float y) �c x��y��̒l
			m_apNumber3D[nCntNumber]->SetNumber(m_nScore % (int)powf(10, MAX_SCORE_BOARD_DIGIT - (float)nCntNumber) / (int)powf(10, MAX_SCORE_BOARD_DIGIT - ((float)nCntNumber + 1)));
		}

		// �ʒu�̐ݒ�E�X�R�A�̍X�V
		m_apNumber3D[nCntNumber]->SetPos(D3DXVECTOR3(m_pos.x + (nCntNumber * (m_size.x - 10.0f)) - (1.5f * (m_size.x - 10.0f)), m_pos.y, m_pos.z));
		m_nScore = (int)m_pos.x / 20;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CScoreBoard::Draw(void)
{
	for (int nCntScore = 0; nCntScore < MAX_SCORE_BOARD_DIGIT; nCntScore++)
	{
		// �k���`�F�b�N
		if (m_apNumber3D[nCntScore] != NULL)
		{
			m_apNumber3D[nCntScore]->Draw();
		}
	}
}

//=============================================================================
// ��������
//=============================================================================
CScoreBoard *CScoreBoard::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DCOLOR col)
{
	// �C���X�^���X�̐���
	CScoreBoard *pScoreBoard = NULL;

	// �k���`�F�b�N
	if (pScoreBoard == NULL)
	{
		pScoreBoard = new CScoreBoard;
		if (pScoreBoard != NULL)
		{
			// ����������
			pScoreBoard->Init(pos, size, col);
		}
	}
	return pScoreBoard;
}

//=============================================================================
// �ʒu�ݒ菈��
//=============================================================================
void CScoreBoard::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}