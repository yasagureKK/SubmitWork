//=============================================================================
// �X�R�A���� [score.cpp]
// Authore : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "score.h"
#include "number.h"
#include "manager.h"
#include "scene.h"
#include "player.h"
#include "ui.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
int CScore::m_aScore[MAX_SCORE_DATA] = {};
int CScore::m_nResultScore[2] = {};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CScore::CScore(int nPriority) : CScene(nPriority)
{
	memset(&m_apNumber, NULL, sizeof(m_apNumber));
	memset(&m_apUi, NULL, sizeof(m_apUi));
	m_nScore = 0;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	m_nNum = 99;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScore::~CScore()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CScore::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �ϐ��̏�����
	m_pos = pos;
	m_size = size;

	for (int nCntScore = 0; nCntScore < MAX_SCORE_DIGIT; nCntScore++)
	{
		// �i���o�[�̐���
		m_apNumber[nCntScore] = CNumber::Create(D3DXVECTOR3(m_pos.x + (nCntScore * (m_size.x / 5 * 4)), m_pos.y, m_pos.z), m_size, m_col);

		// �����_�����邽�߁A�����ʒu��r�����炸�炷
		if (nCntScore >= MAX_SCORE_DIGIT - 1)
		{
			// �����_�̐���
			m_apUi[0] = CUi::Create(D3DXVECTOR3(m_pos.x + (nCntScore * (m_size.x / 5 * 4) - (m_size.x / 4)), m_pos.y + (m_size.y / 8), 0.0f),
				D3DXVECTOR3(m_size.x * 0.8f, m_size.x * 0.8f, 0.0f), PRIORITY_UI, CUi::UI_TYPE_COMMA);
			m_apUi[0]->SetColor(m_col);

			// �i���o�[�̐���
			m_apNumber[nCntScore] = CNumber::Create(D3DXVECTOR3(m_pos.x + (nCntScore * (m_size.x / 5 * 4)) + (m_size.x / 3), m_pos.y, m_pos.z), m_size, m_col);

			// m(���[�g��)�̐���
			m_apUi[1] = CUi::Create(D3DXVECTOR3(m_pos.x + (nCntScore * m_size.x) + (m_size.x / 5 * 1), m_pos.y + (m_size.y / 8), 0.0f),
				D3DXVECTOR3(m_size.x * 0.8f, m_size.x * 0.8f, 0.0f), PRIORITY_UI, CUi::UI_TYPE_METER);
			m_apUi[1]->SetColor(m_col);
		}
	}

	// �ԍ����ݒ肳��Ă��Ȃ��X�R�A(�n�C�X�R�A�̃X�R�A)
	if (m_nNum == 99)
	{
		// �����L���O�̈�Ԗڂ̃X�R�A������
		LoadScore();
		m_nScore = m_aScore[0];
	}
	// �v���C���[���̃X�R�A�̓��Z�b�g
	else
	{
		m_nScore = 0;
	}

	// �ʒu�E�T�C�Y�̐ݒ�
	SetPos(m_pos);
	SetSize(m_size);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CScore::Uninit(void)
{
	// �i���o�[�̔j��
	for (int nCntScore = 0; nCntScore < MAX_SCORE_DIGIT; nCntScore++)
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
void CScore::Update(void)
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

			// �v���C���[�̈ʒu���擾
			D3DXVECTOR3 pos = pPlayer->GetPos();

			// �X�R�A�̔ԍ����v���C���[�̔ԍ��ƈ�v���Ă�����
			if (m_nNum == pPlayer->GetNum() && pPlayer->GetLandFinish() == false)
			{
				// ���U���g��ʂł����f����̂�h��
				if (CManager::GetMode() != CManager::MODE_RESULT)
				{
					// ���̐��ɂȂ�ꍇ
					if (pos.x >= 0)
					{
						// �v���C���[��X�����̈ʒu���X�R�A�ɐݒ�
						//�����̂܂܂̐��l���Ƒ喡���������߁A2����1�ɂ��Ă���
						m_nScore = (int)pos.x / 2;
					}
					else
					{
						m_nScore = 0;
					}
				}
			}
		}
		// ���̃I�u�W�F�N�g���擾
		pObject = pSaveObject->GetObjNext(pSaveObject);
	}

	for (int nCntNumber = 0; nCntNumber < MAX_SCORE_DIGIT; nCntNumber++)
	{
		if (nCntNumber == MAX_SCORE_DIGIT - 1)
		{
			// �Ō��10��n�悪�g���Ȃ��Ȃ邽�߁A���ߑł��ŏ���
			m_apNumber[nCntNumber]->SetNumber(m_nScore % 10/ 1);
		}
		else
		{
			// ���X�R�A % 10�~n / 10�~(n-1)�� �ŏo���l�����ꂼ��̃i���o�[�ɐݒ�
			//��powf(float x, float y) �c x��y��̒l
			m_apNumber[nCntNumber]->SetNumber(m_nScore % (int)powf(10, MAX_SCORE_DIGIT - (float)nCntNumber) / (int)powf(10, MAX_SCORE_DIGIT - ((float)nCntNumber + 1)));
		}
	}

	// �Q�[����ʂȂ�
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		// �X�R�A���X�V
		SetScore(m_nScore);
	}
	// ���U���g��ʂȂ�
	else if (CManager::GetMode() == CManager::MODE_RESULT)
	{
		// �X�R�A���X�V
		SetScore(m_nResultScore[m_nNum]);

		// ���O�̃��[�h��CPU�ΐ�E2�l�p���[�h��������
		if (CManager::GetBeforeMode() == CManager::MODE_PVP || CManager::GetBeforeMode() == CManager::MODE_SOLO_CPU)
		{
			// 1P�̃X�R�A������������(1P�̃X�R�A�ԍ���0)
			if (m_nResultScore[0] > m_nResultScore[1] && m_nNum == 0)
			{
				// �����x�𔖂�����
				m_col.a -= 255;
				if (m_col.a < 0)
				{
					// �����x�����ɖ߂�
					m_col.a = 255;
				}

				m_apUi[0]->SetColor(m_col);
				m_apUi[1]->SetColor(m_col);
				for (int nCntNumber = 0; nCntNumber < MAX_SCORE_DIGIT; nCntNumber++)
				{
					m_apNumber[nCntNumber]->SetColor(m_col);
				}
			}
			// 2P�̃X�R�A������������(2P�̃X�R�A�ԍ���1)
			else if (m_nResultScore[0] < m_nResultScore[1] && m_nNum == 1)
			{
				//�J���[�𔖂�����
				m_col.a -= 255;
				if (m_col.a < 0)
				{
					// �����x�����ɖ߂�
					m_col.a = 255;
				}

				m_apUi[0]->SetColor(m_col);
				m_apUi[1]->SetColor(m_col);
				for (int nCntNumber = 0; nCntNumber < MAX_SCORE_DIGIT; nCntNumber++)
				{
					m_apNumber[nCntNumber]->SetColor(m_col);
				}
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CScore::Draw(void)
{
	for (int nCntScore = 0; nCntScore < MAX_SCORE_DIGIT; nCntScore++)
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
CScore *CScore::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col)
{
	// �C���X�^���X�̐���
	CScore *pScore = NULL;

	// �k���`�F�b�N
	if (pScore == NULL)
	{
		pScore = new CScore;
		if (pScore != NULL)
		{
			// �ϐ��̐ݒ�
			pScore->m_col = col;

			// ����������
			pScore->Init(pos, size);
		}
	}
	return pScore;
}

//=============================================================================
// �X�R�A�ݒ菈��
//=============================================================================
void CScore::SetScore(int nScore)
{
	m_nScore = nScore;
}

//=============================================================================
// �X�R�A�ǉ�����
//=============================================================================
void CScore::AddScore(int nValue)
{
	m_nScore += nValue;
}

//=============================================================================
// �X�R�A�擾����
//=============================================================================
int CScore::GetScore(void)
{
	return m_nScore;
}

//=============================================================================
// �Z�[�u����
//=============================================================================
void CScore::SaveScore(void)
{
	// �t�@�C�����J��
	FILE *pFile;
	pFile = fopen(SAVEFILENAME, "w");

	// �k���`�F�b�N
	if (pFile != NULL)
	{
		for (int nCount = 0; nCount < MAX_SCORE_DATA; nCount++)
		{
			// �����L���O�f�[�^���e�L�X�g�t�@�C���ɃR�s�[
			fprintf(pFile, "%d\n", m_aScore[nCount]);
		}

		// �t�@�C�������
		fclose(pFile);
	}
}

//=============================================================================
// ���[�h����
//=============================================================================
void CScore::LoadScore(void)
{
	FILE *pFile;
	pFile = fopen(SAVEFILENAME, "r");

	// �k���`�F�b�N
	if (pFile != NULL)
	{
		for (int nCount = 0; nCount < MAX_SCORE_DATA; nCount++)
		{
			// �����L���O�f�[�^��ǂݍ���
			fscanf(pFile, "%d", &m_aScore[nCount]);
		}

		// �t�@�C�������
		fclose(pFile);
	}
}

//=============================================================================
// �����L���O�ݒ菈��
//=============================================================================
void CScore::SetRankScore(int nNum)
{
	if (nNum != NULL)
	{
		// �����L���O�f�[�^�̍X�V
		for (int nCnt = 0; nCnt < MAX_SCORE_DATA; nCnt++)
		{
			// �V�����X�R�A�������̃f�[�^��荂��������
			if (nNum > m_aScore[nCnt])
			{
				// �Y�������f�[�^�܂ň�Ԍ�납�烋�[�v
				for (int nCnt2 = MAX_SCORE_DATA - 1; nCnt2 > nCnt; nCnt2--)
				{
					// ���̃f�[�^��1�O�̃f�[�^�ŏ㏑��
					m_aScore[nCnt2] = m_aScore[nCnt2 - 1];
				}

				// �����L���O�f�[�^�ɐV�����X�R�A������
				m_aScore[nCnt] = nNum;

				break;
			}
		}
	}
}

//=============================================================================
// �X�R�A�ʒu�ݒ菈��
//=============================================================================
void CScore::SetScorePos(D3DXVECTOR3 pos)
{
	//�ʒu�ݒ�
	m_pos = pos;
	for (int nCntNumber = 0; nCntNumber < MAX_SCORE_DIGIT; nCntNumber++)
	{
		//�����_�����邽�߁A4�ڂ̐������炸�炷
		if (nCntNumber >= MAX_SCORE_DIGIT / 2)
		{
			m_apNumber[nCntNumber]->SetPos(D3DXVECTOR3(pos.x + 20.0f - (m_size.x * MAX_SCORE_DIGIT / 2) + (m_size.x * nCntNumber) + (m_size.x / 2.0f), pos.y, 0.0f));
		}
		else
		{
			m_apNumber[nCntNumber]->SetPos(D3DXVECTOR3(pos.x - (m_size.x * MAX_SCORE_DIGIT / 2) + (m_size.x * nCntNumber) + (m_size.x / 2.0f), pos.y, 0.0f));
		}
	}

	//�ʒu��ݒ�
	CScene::SetPos(m_pos);
}

//=============================================================================
// �X�R�A�ԍ��ݒ菈��
//=============================================================================
void CScore::SetNum(int nNum)
{
	m_nNum = nNum;
}

//=============================================================================
// �X�R�A�ԍ��擾����
//=============================================================================
int CScore::GetNum(void)
{
	return m_nNum;
}

//=============================================================================
// ���U���g�̃X�R�A�ݒ菈��
//=============================================================================
void CScore::SetResultScore(int nNum)
{
	m_nResultScore[nNum] = m_nScore;
}

//=============================================================================
// ���U���g�̃X�R�A�擾����
//=============================================================================
int CScore::GetResultScore(int nNum)
{
	return m_nResultScore[nNum];
}

//=============================================================================
// �n�C�X�R�A�ݒ菈��
//=============================================================================
void CScore::SetHighScore(int nHighScore)
{
	m_nScore = nHighScore;
}