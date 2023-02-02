//=============================================================================
// �N���A���� [clear.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "data_clear.h"
#include "manager.h"
#include "ui.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SAVEFILENAME "data/TEXT/clear_save.txt"	// �t�@�C���̃p�X

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CDataClear::CDataClear()
{
	// �ϐ��̃N���A
	m_nScore = 0;
	m_nTimeBonus = 0;
	m_nLevelBonus = 0;
	memset(&m_data, NULL, sizeof(m_data));
	memset(&m_newData, NULL, sizeof(m_newData));
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CDataClear::~CDataClear()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CDataClear::Init(void)
{
	// �ϐ��̏�����
	m_nScore = 0;
	m_nTimeBonus = 0;
	m_nLevelBonus = 0;
	memset(&m_data, NULL, sizeof(m_data));
	memset(&m_newData, NULL, sizeof(m_newData));

	Load();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CDataClear::Uninit(void)
{
	
}

//=============================================================================
// ��������
//=============================================================================
CDataClear *CDataClear::Create(void)
{
	// �N���X�̐���
	CDataClear *pDataClear = NULL;
	pDataClear = new CDataClear;
	
	if (pDataClear != NULL)
	{
		// ����������
		pDataClear->Init();
	}
	return pDataClear;
}

//=============================================================================
// �Z�[�u����
//=============================================================================
void CDataClear::Save(void)
{
	// �t�@�C�����J��
	FILE *pFile;
	pFile = fopen(SAVEFILENAME, "w");

	// �k���`�F�b�N
	if (pFile != NULL)
	{
		for (int nCount = 0; nCount < MAX_CLEAR_DATA; nCount++)
		{
			// �����N�̃p�����[�^��NULL��������
			if (m_data[nCount].cRank[0] == NULL)
			{
				// "C"�̕���������Ă���
				strcpy(&m_data[nCount].cRank[0], "C");
			}

			// �����L���O�f�[�^���e�L�X�g�t�@�C���ɃR�s�[
			fprintf(pFile, "%s\n", "RANK");
			fprintf(pFile, "%s%d\n", "	SCORE = ", m_data[nCount].nScore);
			fprintf(pFile, "%s%s\n", "	RANK = ", m_data[nCount].cRank);
			fprintf(pFile, "%s\n\n", "RANK_END");
		}
		fprintf(pFile, "%s", "END_SCRIPT		# ���̍s�͐�Ώ����Ȃ����ƁI");

		// �t�@�C�������
		fclose(pFile);
	}
}

//=============================================================================
// ���[�h����
//=============================================================================
void CDataClear::Load(void)
{
	// �e�L�X�g�t�@�C���̓ǂݍ���
	char cString[256];
	FILE *pFile = fopen(SAVEFILENAME, "r");

	if (pFile != NULL)
	{
		int nCntData = 0;

		while (1)
		{
			fscanf(pFile, "%s", &cString[0]);

			if (strcmp(&cString[0], "RANK") == 0)	//RANK�̕�����
			{
				while (1)
				{
					fscanf(pFile, "%s", &cString[0]);

					if (strcmp(&cString[0], "SCORE") == 0) //�X�R�A
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%d", &m_data[nCntData].nScore);
					}
					if (strcmp(&cString[0], "RANK") == 0) //�����N
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%s", &m_data[nCntData].cRank[0]);
					}
					if (strcmp(&cString[0], "RANK_END") == 0)
					{
						break;
					}
				}

				nCntData++;
			}

			// �f�[�^���ő吔�ɂȂ�����ǂݍ��݂��I��
			if (nCntData >= MAX_CLEAR_DATA)
			{
				break;
			}

			if (strcmp(&cString[0], "END_SCRIPT") == 0) //�I���
			{
				break;
			}
		}
		// �t�@�C�������
		fclose(pFile);
	}
}

//=============================================================================
// �f�[�^�ݒ菈��
//=============================================================================
void CDataClear::SetNewScore(int nTime, int nLevel)
{
	m_newData.nTime = nTime;
	m_newData.nLevel = nLevel;

	// �^�C���{�[�i�X�A���x���{�[�i�X��ݒ�
	m_nTimeBonus = (999 - nTime) * 10;
	m_nLevelBonus = nLevel * 100;

	// �����̃X�R�A�ƍ��v���āA�V�����X�R�A��ݒ�
	m_newData.nScore = m_nScore + m_nTimeBonus + m_nLevelBonus;

	// �X�R�A��18000�_�ȏ��S�����N
	if (m_newData.nScore >= CLEAR_RANK_S)
	{
		strcpy(&m_newData.cRank[0], "S");
	}
	// �X�R�A��13000�_�ȏ�A18000�_������A�����N
	else if (m_newData.nScore >= CLEAR_RANK_A && m_newData.nScore < CLEAR_RANK_S)
	{
		strcpy(&m_newData.cRank[0], "A");
	}
	// �X�R�A��6000�_�ȏ�A13000�_������B�����N
	else if (m_newData.nScore >= CLEAR_RANK_B && m_newData.nScore < CLEAR_RANK_A)
	{
		strcpy(&m_newData.cRank[0], "B");
	}
	// �X�R�A��6000�_������C�����N
	else if (m_newData.nScore < CLEAR_RANK_B)
	{
		strcpy(&m_newData.cRank[0], "C");
	}

	SetRanking(m_newData);
}

//=============================================================================
// �����L���O�ݒ菈��
//=============================================================================
void CDataClear::SetRanking(CLEAR_DATA data)
{
	// �����L���O�f�[�^�̍X�V
	for (int nCnt = 0; nCnt < MAX_CLEAR_DATA; nCnt++)
	{
		// �V�����X�R�A�������̃f�[�^��荂��������
		if (data.nScore > m_data[nCnt].nScore)
		{
			// �Y�������f�[�^�܂ň�Ԍ�납�烋�[�v
			for (int nCnt2 = MAX_CLEAR_DATA - 1; nCnt2 > nCnt; nCnt2--)
			{
				// ���̃f�[�^��1�O�̃f�[�^�ŏ㏑��
				m_data[nCnt2] = m_data[nCnt2 - 1];
				strcpy(&m_data[nCnt2].cRank[0], &m_data[nCnt2 - 1].cRank[0]);
			}

			// �����L���O�f�[�^�ɐV�����f�[�^������
			m_data[nCnt] = data;
			strcpy(&m_data[nCnt].cRank[0], &data.cRank[0]);

			break;
		}
	}

	// �f�[�^�̃Z�[�u
	Save();
}

//=============================================================================
// �����L���O�擾����
//=============================================================================
CDataClear::CLEAR_DATA CDataClear::GetRankingData(int nNum)
{
	return m_data[nNum];
}