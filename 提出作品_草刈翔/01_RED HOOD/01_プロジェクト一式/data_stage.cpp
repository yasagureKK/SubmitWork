//=============================================================================
// �X�e�[�W�f�[�^ [data_stage.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "data_stage.h"
#include "manager.h"
#include "texture.h"
#include "model_single.h"
#include "mesh_field.h"
#include "mesh_wall.h"
#include "mesh_sky.h"
#include "enemy.h"
#include "texture.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXT_FILE_PATH "data/TEXT/stage_data_"	// ��{�ƂȂ�e�L�X�g�t�@�C���̃p�X

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CDataStage::CDataStage()
{
	// �ϐ��̃N���A
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nType = 0;
	m_nRow = 0;
	m_nLine = 0;
	m_nMaxStage = 0;
	m_pMeshSky = NULL;
	m_type = STAGE_TYPE_GAME;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CDataStage::~CDataStage()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CDataStage::Init(D3DXVECTOR3 pos)
{
	WIN32_FIND_DATA win32fd;	// �t�@�C���̏��
	HANDLE hFind;				// �n���h��

	// ��������f�[�^�t�@�C���̃p�X
	hFind = FindFirstFile("data\\TEXT\\stage_data*", &win32fd);

	// ���������s���Ȃ������� 
	if (hFind != INVALID_HANDLE_VALUE)
	{
		// �t�@�C���̐����擾
		do
		{
			// �X�e�[�W�̍ő吔�𑝂₷
			m_nMaxStage++;
		} while (FindNextFile(hFind, &win32fd));

		// �t�@�C�������
		FindClose(hFind);
	}

	// �X�e�[�W�̃��[�h
	LoadStage((int)m_type);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CDataStage::Uninit(void)
{
	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CDataStage::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CDataStage::Draw(void)
{

}

//=============================================================================
// ��������
//=============================================================================
CDataStage *CDataStage::Create(D3DXVECTOR3 pos, STAGE_TYPE type)
{
	// �N���X�̐���
	CDataStage *pStageData = NULL;
	pStageData = new CDataStage;

	if (pStageData != NULL)
	{
		// �ϐ��̐ݒ�
		pStageData->m_type = type;

		// ����������
		pStageData->Init(pos);
	}
	return pStageData;
}

//=============================================================================
// �X�e�[�W���[�h����
//=============================================================================
void CDataStage::LoadStage(int nNum)
{
	char aFillName[128];
	char aChar[128];
	int nStageNum = nNum + 1;

	// �t�@�C�����Ɋ�{�ƂȂ�p�X�̕����������
	strcpy(&aFillName[0], TEXT_FILE_PATH);

	// �X�e�[�W�̔ԍ��𕶎���ɂ��ĕۑ����A�p�X�̕�����ƃX�e�[�W�ԍ���A��
	sprintf(&aChar[0], "%d", nStageNum);
	strcat(&aFillName[0], &aChar[0]);

	// ".txt"�̕������ۑ����A�A�����ăp�X������
	sprintf(&aChar[0], ".txt");
	strcat(&aFillName[0], &aChar[0]);

	// �e�L�X�g�t�@�C���̓ǂݍ���
	char cString[256];
	FILE *pFile = fopen(&aFillName[0], "r");

	if (pFile != NULL)
	{
		int nMaxField = 0, nMaxWall = 0, nMaxModel = 0, nMaxEnemy = 0;
		int nCntField = 0, nCntWall = 0, nCntModel = 0, nCntEnemy = 0;

		while (1)
		{
			fscanf(pFile, "%s", &cString[0]);

			/************/
			/* ��̔z�u */
			/************/
			if (strcmp(&cString[0], "SKYSET") == 0)	//SKYSET�̕�����
			{
				int nTexType = 0;
				float fSize = 0.0f, fSpin = 0.0f;

				while (1)
				{
					fscanf(pFile, "%s", &cString[0]);

					if (strcmp(&cString[0], "TEXTYPE") == 0) //�e�N�X�`���̎��
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%d", &nTexType);
					}
					if (strcmp(&cString[0], "SIZE") == 0) //�T�C�Y
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%f", &fSize);
					}
					if (strcmp(&cString[0], "SPIN") == 0) //��]��
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%f", &fSpin);
					}
					if (strcmp(&cString[0], "END_SKYSET") == 0)
					{
						break;
					}
				}

				// ���b�V���X�t�B�A�𐶐�
				//������A�t�H�O�̉e���Ŕw�i�������Ȃ����ߋ�͐������Ȃ�
				//CMeshSky::Create(D3DXVECTOR3(0.0f, -1000.0f, 0.0f), D3DXVECTOR3(fSize, 0.0f, fSize), fSpin, nTexType);
			}

			// ���ꂼ��̃I�u�W�F�N�g�̍ő吔���擾
			if (strcmp(&cString[0], "NUM_FIELD") == 0)
			{
				fscanf(pFile, "%s%d", cString, &nMaxField);
			}
			if (strcmp(&cString[0], "NUM_WALL") == 0)
			{
				fscanf(pFile, "%s%d", cString, &nMaxWall);
			}
			if (strcmp(&cString[0], "NUM_MODEL") == 0)
			{
				fscanf(pFile, "%s%d", cString, &nMaxModel);
			}
			if (strcmp(&cString[0], "NUM_ENEMY") == 0)
			{
				fscanf(pFile, "%s%d", cString, &nMaxEnemy);
			}

			/**************/
			/* �n�ʂ̔z�u */
			/**************/
			if (strcmp(&cString[0], "FIELDSET") == 0 && nCntField < nMaxField)
			{
				int nTexType = 0, nRow = 0, nLine = 0;
				D3DXVECTOR3 pos, rot, size;

				while (1)
				{
					fscanf(pFile, "%s", &cString[0]);

					if (strcmp(&cString[0], "TEXTYPE") == 0) //�e�N�X�`���̎��
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%d", &nTexType);
					}
					if (strcmp(&cString[0], "POS") == 0) //�ʒu
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%f%f%f", &pos.x, &pos.y, &pos.z);
					}
					if (strcmp(&cString[0], "ROT") == 0) //����
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%f%f%f", &rot.x, &rot.y, &rot.z);
					}
					if (strcmp(&cString[0], "BLOCK") == 0) //������
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%d%d", &nLine, &nRow);
					}
					if (strcmp(&cString[0], "SIZE") == 0) //�T�C�Y
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%f%f", &size.x, &size.z);
						size.y = 0.0f;
					}
					if (strcmp(&cString[0], "END_FIELDSET") == 0)
					{
						break;
					}
				}

				// �n�ʂ̐���
				CMeshField::Create(pos, size, rot, nRow, nLine, nTexType);

				nCntField++;
			}

			/************/
			/* �ǂ̔z�u */
			/************/
			if (strcmp(&cString[0], "WALLSET") == 0 && nCntWall < nMaxWall)
			{
				int nTexType = 0, nRow = 0, nLine = 0;
				D3DXVECTOR3 pos, rot, size;

				// ���f���̃p�[�c��������
				while (1)
				{
					fscanf(pFile, "%s", &cString[0]);

					if (strcmp(&cString[0], "TEXTYPE") == 0) //�e�N�X�`���̎��
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%d", &nTexType);
					}
					if (strcmp(&cString[0], "POS") == 0) //�ʒu
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%f%f%f", &pos.x, &pos.y, &pos.z);
					}
					if (strcmp(&cString[0], "ROT") == 0) //����
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%f%f%f", &rot.x, &rot.y, &rot.z);
					}
					if (strcmp(&cString[0], "BLOCK") == 0) //������
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%d%d", &nRow, &nLine);
					}
					if (strcmp(&cString[0], "SIZE") == 0) //�T�C�Y
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%f%f", &size.x, &size.y);
						size.z = 0.0f;
					}
					if (strcmp(&cString[0], "END_WALLSET") == 0)
					{
						break;
					}
				}

				// �ǂ̐���
				CMeshWall::Create(pos, size, rot, nRow, nLine, nTexType);

				nCntWall++;
			}

			/****************/
			/* ���f���̔z�u */
			/****************/
			if (strcmp(&cString[0], "MODELSET") == 0 && nCntModel < nMaxModel)
			{
				int nType = 0;
				D3DXVECTOR3 pos, rot;

				// ���f���̃p�[�c��������
				while (1)
				{
					fscanf(pFile, "%s", &cString[0]);

					if (strcmp(&cString[0], "TYPE") == 0) //���
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%d", &nType);
					}
					if (strcmp(&cString[0], "POS") == 0) //�ʒu
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%f%f%f", &pos.x, &pos.y, &pos.z);
					}
					if (strcmp(&cString[0], "ROT") == 0) //����
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%f%f%f", &rot.x, &rot.y, &rot.z);
					}
					if (strcmp(&cString[0], "END_MODELSET") == 0)
					{
						break;
					}
				}

				// ���f���̐���
				CModelSingle::Create(pos, rot, nType);

				nCntModel++;
			}

			/************/
			/* �G�̔z�u */
			/************/
			if (strcmp(&cString[0], "ENEMYSET") == 0 && nCntEnemy < nMaxEnemy)
			{
				int nType = 0;
				D3DXVECTOR3 pos, rot;
				float fLife = 0.0f;

				// ���f���̃p�[�c��������
				while (1)
				{
					fscanf(pFile, "%s", &cString[0]);

					if (strcmp(&cString[0], "TYPE") == 0) //�C���f�b�N�X�ԍ�
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%d", &nType);
					}
					if (strcmp(&cString[0], "POS") == 0) //�ʒu
					{
						// �ʒu���擾����
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%f%f%f", &pos.x, &pos.y, &pos.z);
					}
					if (strcmp(&cString[0], "ROT") == 0) //����
					{
						// �������擾����
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%f%f%f", &rot.x, &rot.y, &rot.z);
					}
					if (strcmp(&cString[0], "LIFE") == 0) //���C�t
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%f", &fLife);
					}
					if (strcmp(&cString[0], "END_ENEMYSET") == 0)
					{
						break;
					}
				}

				// int�^��enum�^�ɃL���X�g
				CEnemy::ENEMY_TYPE type = (CEnemy::ENEMY_TYPE)nType;

				// �G�̐���
				CEnemy::Create(pos, rot, type, fLife);
				nCntEnemy++;
			}

			if (strcmp(&cString[0], "END_SCRIPT") == 0) // END_SCRIPT�̕�����
			{
				break;
			}
		}

		// �t�@�C�������
		fclose(pFile);
	}
}