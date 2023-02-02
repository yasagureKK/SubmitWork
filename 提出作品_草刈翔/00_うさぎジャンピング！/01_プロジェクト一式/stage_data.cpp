//=============================================================================
// �X�e�[�W�f�[�^ [stage_data.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "stage_data.h"
#include "manager.h"
#include "texture.h"
#include "player.h"
#include "model_single.h"
#include "billboard.h"
#include "texture.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
int CStageData::m_nModelNum = 0;
int CStageData::m_nBillboardNum = 0;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CStageData::CStageData()
{
	// �ϐ��̃N���A
	memset(&aStageModel, NULL, sizeof(aStageModel));
	memset(&aStageBillboard, NULL, sizeof(aStageBillboard));
	m_nModelNum = 0;
	m_nBillboardNum = 0;
	m_nNumPlayer = 0;
	m_nPattern = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CStageData::~CStageData()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CStageData::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �ϐ��̏�����
	m_pos = pos;
	m_size = size;

	int nRandumPattern = rand() % MAX_PATTERN;

	// �X�e�[�W�̃��[�h
	LoadStage(nRandumPattern);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CStageData::Uninit(void)
{
	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CStageData::Update(void)
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
					// �I�u�W�F�N�g��������
					DeleteObject();

					// �ʒu�����炷
					m_pos.x += m_size.x * 2;

					Init(m_pos, m_size);
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
void CStageData::Draw(void)
{

}

//=============================================================================
// ��������
//=============================================================================
CStageData *CStageData::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nNumPlayer)
{
	// �C���X�^���X�̐���
	CStageData *pStageData = NULL;

	// �k���`�F�b�N
	if (pStageData == NULL)
	{
		// �N���X�̐���
		pStageData = new CStageData;
		if (pStageData != NULL)
		{
			// �ϐ��̏�����
			pStageData->m_nNumPlayer = nNumPlayer;

			// ����������
			pStageData->Init(pos, size);
		}
	}

	return pStageData;
}

//=============================================================================
// �X�e�[�W���[�h����
//=============================================================================
void CStageData::LoadStage(int nPattern)
{
	// �e�L�X�g�ۑ��p�̕ϐ�
	char cString[MAX_STRING];

	// �e�L�X�g�t�@�C���̓ǂݍ���
	FILE *pFile = fopen(STAGE_DATA_FILE, "r");

	if (pFile != NULL)
	{
		for (int nNumPattern = 0; nNumPattern < MAX_PATTERN; nNumPattern++)
		{
			// �e�L�X�g�t�@�C���̍Ō�(NULL)�܂œǂݍ���
			while (fgets(cString, MAX_STRING, pFile) != NULL)
			{
				// �������ۑ�
				fscanf(pFile, "%s", cString);

				// ������̒���"OBJECTSET"����������
				if (strncmp("OBJECTSET", cString, 10) == 0)
				{
					// �p�^�[���̔ԍ����擾
					fscanf(pFile, "%s%*s%d%*s%*s", cString, &m_nPattern);
					// ���f���̍ő吔���擾
					fscanf(pFile, "%s%*s%d%*s%*s", cString, &m_nModelNum);
					// �r���{�[�h�̍ő吔���擾
					fscanf(pFile, "%s%*s%d", cString, &m_nBillboardNum);
					break;
				}
			}

			// �ő吔�𒴂��Ȃ��悤�ɂ���
			if (m_nModelNum > MAX_STAGE_OBJECT)
			{
				m_nModelNum = MAX_STAGE_OBJECT - 1;
			}

			if (m_nBillboardNum > MAX_STAGE_OBJECT)
			{
				m_nBillboardNum = MAX_STAGE_OBJECT - 1;
			}

			// �����_���Ō��܂��������ƃp�^�[���ԍ���������������z�u���J�n
			if (nPattern == m_nPattern)
			{
				/******************************/
				/* �擾�������f���̐��Ԃ�z�u */
				/******************************/
				for (int nCntStageModel = 0; nCntStageModel < m_nModelNum; nCntStageModel++)
				{
					while (fgets(cString, MAX_STRING, pFile) != NULL)
					{
						// �������ۑ�
						fscanf(pFile, "%s", cString);

						// ������̒���"MODELSET"����������
						if (strncmp("MODELSET", cString, 9) == 0)
						{
							break;
						}
					}

					while (fgets(cString, MAX_STRING, pFile) != NULL)
					{
						// �������ۑ�
						fscanf(pFile, "%s", cString);

						// ������̒���"TYPE"����������
						if (strncmp("TYPE", cString, 5) == 0)
						{
							// ��ނ��擾����
							fscanf(pFile, "%s%d%*s%*s", cString, &aStageModel[nCntStageModel].nType);

							// �ʒu���擾
							fscanf(pFile, "%s%*s%f%f%f%*s", cString,
								&aStageModel[nCntStageModel].pos.x,
								&aStageModel[nCntStageModel].pos.y,
								&aStageModel[nCntStageModel].pos.z);

							// �������擾
							fscanf(pFile, "%s%*s%*s%f%f%f", cString,
								&aStageModel[nCntStageModel].rot.x,
								&aStageModel[nCntStageModel].rot.y,
								&aStageModel[nCntStageModel].rot.z);
							break;
						}
					}

					// int�^��enum�^�ɃL���X�g
					CModelLoad::MODEL_TYPE type = (CModelLoad::MODEL_TYPE)aStageModel[nCntStageModel].nType;

					// ���f���𐶐�
					aStageModel[nCntStageModel].pModelSingle = CModelSingle::Create
					(m_pos + aStageModel[nCntStageModel].pos, aStageModel[nCntStageModel].rot,
						type, CModelSingle::MODEL_SINGLE_TYPE_NONE);
					// �e��ݒ�
					aStageModel[nCntStageModel].pModelSingle->SetShadow(true, 0.0f);

					while (fgets(cString, MAX_STRING, pFile) != NULL)
					{
						// �������ۑ�
						fscanf(pFile, "%s", cString);

						// ������̒���"END_MODELSET"���������炻���ŏI��
						if (strncmp("END_MODELSET", cString, 13) == 0)
						{
							break;
						}
					}
				}

				/**********************************/
				/* �擾�����r���{�[�h�̐��Ԃ�z�u */
				/**********************************/
				for (int nCntStageBillboard = 0; nCntStageBillboard < m_nBillboardNum; nCntStageBillboard++)
				{
					while (fgets(cString, MAX_STRING, pFile) != NULL)
					{
						// �������ۑ�
						fscanf(pFile, "%s", cString);

						// ������̒���"BILLBOARDSET"����������
						if (strncmp("BILLBOARDSET", cString, 13) == 0)
						{
							break;
						}
					}

					while (fgets(cString, MAX_STRING, pFile) != NULL)
					{
						// �������ۑ�
						fscanf(pFile, "%s", cString);

						// ������̒���"TEXTYPE"����������
						if (strncmp("TEXTYPE", cString, 8) == 0)
						{
							// �e�N�X�`���̎�ނ��擾����
							fscanf(pFile, "%s%d%*s%*s", cString, &aStageBillboard[nCntStageBillboard].nTexType);

							// �ʒu���擾
							fscanf(pFile, "%s%*s%f%f%f%*s", cString,
								&aStageBillboard[nCntStageBillboard].pos.x,
								&aStageBillboard[nCntStageBillboard].pos.y,
								&aStageBillboard[nCntStageBillboard].pos.z);

							// �T�C�Y���擾
							fscanf(pFile, "%s%*s%*s%f%f", cString,
								&aStageBillboard[nCntStageBillboard].size.x,
								&aStageBillboard[nCntStageBillboard].size.y);
							break;
						}
					}

					// int�^��enum�^�ɃL���X�g
					CTexture::TEX_TYPE type = (CTexture::TEX_TYPE)aStageBillboard[nCntStageBillboard].nTexType;

					// �r���{�[�h�𐶐�
					aStageBillboard[nCntStageBillboard].pBillboard = CBillboard::Create
					(m_pos + aStageBillboard[nCntStageBillboard].pos,
						D3DXVECTOR3(aStageBillboard[nCntStageBillboard].size.x, aStageBillboard[nCntStageBillboard].size.y, 0.0f),
						type, PRIORITY_EFFECT);
					if (type == CTexture::TEX_TYPE_TREE)
					{
						aStageBillboard[nCntStageBillboard].pBillboard->SetShadow(true);
					}
					else if (type == CTexture::TEX_TYPE_BUSH_00)
					{
						aStageBillboard[nCntStageBillboard].pBillboard->SetShadow(true);
					}


					while (fgets(cString, MAX_STRING, pFile) != NULL)
					{
						// �������ۑ�
						fscanf(pFile, "%s", cString);

						// ������̒���"END_BILLBOARDSET"���������炻���ŏI��
						if (strncmp("END_BILLBOARDSET", cString, 17) == 0)
						{
							break;
						}
					}
				}

				break;
			}

			while (fgets(cString, MAX_STRING, pFile) != NULL)
			{
				// �������ۑ�
				fscanf(pFile, "%s", cString);

				// ������̒���"END_OBJECTSET"���������炻���Ń��[�V�����̎擾���I��
				if (strncmp("END_OBJECTSET", cString, 14) == 0)
				{
					break;
				}
			}
		}
	}

	// �t�@�C�������
	fclose(pFile);
}

//=============================================================================
// �I�u�W�F�N�g�̑S�̏�������
//=============================================================================
void CStageData::DeleteObject()
{
	for (int nCntData = 0; nCntData < MAX_STAGE_OBJECT; nCntData++)
	{
		if (aStageModel[nCntData].pModelSingle != NULL)
		{
			// ���f��������
			aStageModel[nCntData].pModelSingle->Uninit();
			delete aStageModel[nCntData].pModelSingle;
			aStageModel[nCntData].pModelSingle = NULL;
		}
	}

	for (int nCntData = 0; nCntData < MAX_STAGE_OBJECT; nCntData++)
	{
		if (aStageBillboard[nCntData].pBillboard != NULL)
		{
			// �r���{�[�h������
			aStageBillboard[nCntData].pBillboard->DeleteShadow();
			aStageBillboard[nCntData].pBillboard->Uninit();
			delete aStageBillboard[nCntData].pBillboard;
			aStageBillboard[nCntData].pBillboard = NULL;

		}
	}
}