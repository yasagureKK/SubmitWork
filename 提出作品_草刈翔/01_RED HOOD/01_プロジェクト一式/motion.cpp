//=============================================================================
// ���[�V�������� [motion.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "motion.h"
#include "player.h"
#include "enemy.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MOTION_PLAYER_FILE	"data/TEXT/motion_player.txt"
#define MOTION_WOLF00_FILE	"data/TEXT/motion_wolf00.txt"
#define MOTION_WOLF01_FILE	"data/TEXT/motion_wolf01.txt"
#define MOTION_BOSS_FILE	"data/TEXT/motion_boss.txt"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CMotion::CMotion(void)
{
	// �ϐ��̃N���A
	for (int nCntMotion = 0; nCntMotion < MAX_MOTION_NUM; nCntMotion++)
	{
		m_aInfo[nCntMotion].nLoop = 0;
		m_aInfo[nCntMotion].nNumKey = 0;
		for (int nCntKey = 0; nCntKey < MAX_KEY_NUM; nCntKey++)
		{
			m_aInfo[nCntMotion].aKeyInfo[nCntKey].nFrame = 0;
			for (int nCntModel = 0; nCntModel < MAX_MODEL_NUM; nCntModel++)
			{
				m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
		}
	}

	m_nMaxModelNum = 0;
	m_nMaxMotionNum = 0;
	m_nNumKey = 0;
	m_nKey = 0;
	m_nKeyOld = 0;
	m_nType = 0;
	m_nTypeNext = 0;
	m_fCounter = 0;
	m_bConnect = false;
	m_bStop = false;
	m_type = MOTION_TYPE_PLAYER;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMotion::~CMotion()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CMotion::Init(CScene *pScene, MOTION_TYPE type)
{
	// �����̃|�C���^���v���C���[�N���X�̃|�C���^�ɃL���X�g
	CPlayer *pPlayer = NULL;
	CEnemy *pEnemy = NULL;

	// �e�L�X�g�t�@�C���̓ǂݍ���
	char cString[256];
	FILE *pFile = NULL;

	m_type = type;
	switch (m_type)
	{
	case MOTION_TYPE_PLAYER:
		// �v���C���[�N���X�̃|�C���^�ɃL���X�g
		pPlayer = (CPlayer*)pScene;
		pFile = fopen(MOTION_PLAYER_FILE, "r");
		break;

	case MOTION_TYPE_WOLF00:
		// �G�l�~�[�N���X�̃|�C���^�ɃL���X�g
		pEnemy = (CEnemy*)pScene;
		pFile = fopen(MOTION_WOLF00_FILE, "r");
		break;

	case MOTION_TYPE_WOLF01:
		// �G�l�~�[�N���X�̃|�C���^�ɃL���X�g
		pEnemy = (CEnemy*)pScene;
		pFile = fopen(MOTION_WOLF01_FILE, "r");
		break;

	case MOTION_TYPE_BOSS:
		// �G�l�~�[�N���X�̃|�C���^�ɃL���X�g
		pEnemy = (CEnemy*)pScene;
		pFile = fopen(MOTION_BOSS_FILE, "r");
		break;
	default:
		break;
	}

	int nKey = 0, nModel = 0;
	bool bMotion = false;

	if (pFile != NULL)
	{
		while (1)
		{
			fscanf(pFile, "%s", &cString[0]);

			// ���f���̍ő吔���擾
			if (strcmp(&cString[0], "NUM_MODEL") == 0)
			{
				fscanf(pFile, "%s", &cString[0]);
				fscanf(pFile, "%d", &m_nMaxModelNum);
			}

			if (strcmp(&cString[0], "MOTIONSET") == 0) // MOTIONSET�̕�����
			{
				bMotion = true;
			}
			if (strcmp(&cString[0], "END_MOTIONSET") == 0)	// END_MOTIONSET�̕�����
			{
				m_nMaxMotionNum++;
				bMotion = false;
			}

			if (bMotion == true)
			{
				if (strcmp(&cString[0], "LOOP") == 0) //���[�v���邩�ǂ��� [0 : ���[�v���Ȃ� / 1 : ���[�v����]
				{
					fscanf(pFile, "%s", &cString[0]);
					fscanf(pFile, "%d", &m_aInfo[m_nMaxMotionNum].nLoop);
				}
				if (strcmp(&cString[0], "NUM_KEY") == 0) //�L�[��
				{
					fscanf(pFile, "%s", &cString[0]);
					fscanf(pFile, "%d", &m_aInfo[m_nMaxMotionNum].nNumKey);
				}

				if (strcmp(&cString[0], "KEYSET") == 0) // KEYSET�̕�����
				{
					// �L�[���Ԃ��
					while (1)
					{
						fscanf(pFile, "%s", &cString[0]);

						if (strcmp(&cString[0], "FRAME") == 0) //�t���[����
						{
							// �t���[�������擾����
							fscanf(pFile, "%s", &cString[0]);
							fscanf(pFile, "%d", &m_aInfo[m_nMaxMotionNum].aKeyInfo[nKey].nFrame);

							// ���f�����Ԃ��
							while (1)
							{
								fscanf(pFile, "%s", &cString[0]);

								if (strcmp(&cString[0], "POS") == 0) //�ʒu
								{
									// �ʒu���擾����
									fscanf(pFile, "%s", &cString[0]);
									fscanf(pFile, "%f%f%f", &m_aInfo[m_nMaxMotionNum].aKeyInfo[nKey].aKey[nModel].pos.x,
										&m_aInfo[m_nMaxMotionNum].aKeyInfo[nKey].aKey[nModel].pos.y,
										&m_aInfo[m_nMaxMotionNum].aKeyInfo[nKey].aKey[nModel].pos.z);
								}
								if (strcmp(&cString[0], "ROT") == 0) //����
								{
									// �������擾����
									fscanf(pFile, "%s", &cString[0]);
									fscanf(pFile, "%f%f%f", &m_aInfo[m_nMaxMotionNum].aKeyInfo[nKey].aKey[nModel].rot.x,
										&m_aInfo[m_nMaxMotionNum].aKeyInfo[nKey].aKey[nModel].rot.y,
										&m_aInfo[m_nMaxMotionNum].aKeyInfo[nKey].aKey[nModel].rot.z);

									nModel++;
								}

								// ���f���̍ő吔�܂œǂݍ��񂾂�
								if (nModel >= m_nMaxModelNum)
								{
									nModel = 0;
									break;
								}
							}

							nKey++;
						}

						// �L�[�̍ő吔�܂œǂݍ��񂾂�
						if (nKey >= m_aInfo[m_nMaxMotionNum].nNumKey)
						{
							nKey = 0;
							break;
						}
					}
				}
			}

			if (strcmp(&cString[0], "END_SCRIPT") == 0) //�I���
			{
				break;
			}
		}
		fclose(pFile);
	}

	// �e�q�֌W�ɂ��ʒu����̏C��
	for (int nCntMotion = 0; nCntMotion < m_nMaxMotionNum; nCntMotion++)
	{
		for (int nCntKey = 0; nCntKey < m_aInfo[nCntMotion].nNumKey; nCntKey++)
		{
			for (int nCntModel = 0; nCntModel < m_nMaxModelNum; nCntModel++)
			{
				// ���f���̈ʒu�ƌ������擾
				D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				switch (m_type)
				{
				case MOTION_TYPE_PLAYER:
					pos = pPlayer->GetModelPos(nCntModel);
					rot = pPlayer->GetModelRot(nCntModel);
					break;

				default:
					pos = pEnemy->GetModelPos(nCntModel);
					rot = pEnemy->GetModelRot(nCntModel);
					break;
				}

				m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].pos.x += pos.x;
				m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].pos.y += pos.y;
				m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].pos.z += pos.z;
				m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].rot.x += rot.x;
				m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].rot.y += rot.y;
				m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].rot.z += rot.z;
			}
		}
	}

	// �ϐ��̏�����
	m_nNumKey = 0;
	m_nKey = 0;
	m_nKeyOld = 0;
	m_nType = 0;
	m_nTypeNext = 0;
	m_fCounter = 0;
	m_bConnect = false;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CMotion::Uninit(void)
{
	delete this;
}

//=============================================================================
// �X�V����
//=============================================================================
void CMotion::Update(CScene *pScene)
{
	CPlayer *pPlayer = NULL;
	CEnemy *pEnemy = NULL;
	switch (m_type)
	{
	case MOTION_TYPE_PLAYER:
		pPlayer = (CPlayer*)pScene;
		break;
	default:
		pEnemy = (CEnemy*)pScene;
		break;
	}

	D3DXVECTOR3 posAsk = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ���߂����ʒu�̒l
	D3DXVECTOR3 rotAsk = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ���߂��������̒l
	D3DXVECTOR3 posDiffer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu�̍���
	D3DXVECTOR3 rotDiffer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �����̍���
	int nKeyNext = 0;			// ���̃L�[

								// ���݂̃L�[���ő�l�ȏ�ɂȂ�����L�[���ŏ��ɖ߂�
	if (m_nKey >= m_aInfo[m_nType].nNumKey - 1)
	{
		nKeyNext = 0;

	}
	// ���݂̃L�[���ő�l��菬����������L�[�����ɐi�߂�
	else
	{
		nKeyNext = m_nKey + 1;
	}

	// ���[�V�����������̏ꍇ
	if (m_bConnect == true)
	{
		// ���̃L�[��0�ɂ���
		nKeyNext = 0;
	}

	for (int nCntModel = 0; nCntModel < m_nMaxModelNum; nCntModel++)
	{
		// �t���[����
		int nFrame = 10;

		if (m_bConnect == true)
		{
			// ���[�v����Ȃ�
			if (m_aInfo[m_nType].nLoop == 1)
			{
				nFrame = m_aInfo[m_nType].aKeyInfo[0].nFrame;
			}
		}
		else
		{
			nFrame = m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame;
		}

		// ���[�V�������Ȃ���ꍇ
		if (m_bConnect == true)
		{
			// ���f���̈ʒu�ƌ������擾
			D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			switch (m_type)
			{
			case MOTION_TYPE_PLAYER:
				pos = pPlayer->GetModelPos(nCntModel);
				rot = pPlayer->GetModelRot(nCntModel);
				break;

			default:
				pos = pEnemy->GetModelPos(nCntModel);
				rot = pEnemy->GetModelRot(nCntModel);
				break;
			}

			// ���݂̃L�[�Ǝ��̃L�[�̈ʒu�̍��������߂�
			posDiffer.x = m_aInfo[m_nTypeNext].aKeyInfo[0].aKey[nCntModel].pos.x - pos.x;
			posDiffer.y = m_aInfo[m_nTypeNext].aKeyInfo[0].aKey[nCntModel].pos.y - pos.y;
			posDiffer.z = m_aInfo[m_nTypeNext].aKeyInfo[0].aKey[nCntModel].pos.z - pos.z;

			// ���݂̃L�[�Ǝ��̃L�[�̉�]�̍��������߂�
			rotDiffer.x = m_aInfo[m_nTypeNext].aKeyInfo[0].aKey[nCntModel].rot.x - rot.x;
			rotDiffer.y = m_aInfo[m_nTypeNext].aKeyInfo[0].aKey[nCntModel].rot.y - rot.y;
			rotDiffer.z = m_aInfo[m_nTypeNext].aKeyInfo[0].aKey[nCntModel].rot.z - rot.z;

			// ���݂̃L�[���玟�̃L�[�ɓ���������̈ʒu�����߂�
			posAsk.x = pos.x + posDiffer.x * (m_fCounter / nFrame);
			posAsk.y = pos.y + posDiffer.y * (m_fCounter / nFrame);
			posAsk.z = pos.z + posDiffer.z * (m_fCounter / nFrame);

			// ���݂̃L�[���玟�̃L�[�ɓ���������̌��������߂�
			rotAsk.x = rot.x + rotDiffer.x * (m_fCounter / nFrame);
			rotAsk.y = rot.y + rotDiffer.y * (m_fCounter / nFrame);
			rotAsk.z = rot.z + rotDiffer.z * (m_fCounter / nFrame);
		}
		// ���[�V�������Ȃ��Ȃ��ꍇ
		else
		{
			// ���݂̃L�[�Ǝ��̃L�[�̈ʒu�̍��������߂�
			posDiffer.x = m_aInfo[m_nTypeNext].aKeyInfo[nKeyNext].aKey[nCntModel].pos.x - m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].pos.x;
			posDiffer.y = m_aInfo[m_nTypeNext].aKeyInfo[nKeyNext].aKey[nCntModel].pos.y - m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].pos.y;
			posDiffer.z = m_aInfo[m_nTypeNext].aKeyInfo[nKeyNext].aKey[nCntModel].pos.z - m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].pos.z;

			// ���݂̃L�[�Ǝ��̃L�[�̉�]�̍��������߂�
			rotDiffer.x = m_aInfo[m_nTypeNext].aKeyInfo[nKeyNext].aKey[nCntModel].rot.x - m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].rot.x;
			rotDiffer.y = m_aInfo[m_nTypeNext].aKeyInfo[nKeyNext].aKey[nCntModel].rot.y - m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].rot.y;
			rotDiffer.z = m_aInfo[m_nTypeNext].aKeyInfo[nKeyNext].aKey[nCntModel].rot.z - m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].rot.z;

			// �����̍�����3.14����-3.14�̒l�͈͓̔��Ɏ��߂�
			if (rotDiffer.x < -D3DX_PI)
			{
				rotDiffer.x += D3DX_PI * 2.0f;
			}
			else if (rotDiffer.x > D3DX_PI)
			{
				rotDiffer.x -= D3DX_PI * 2.0f;
			}

			if (rotDiffer.y < -D3DX_PI)
			{
				rotDiffer.y += D3DX_PI * 2.0f;
			}
			else if (rotDiffer.y > D3DX_PI)
			{
				rotDiffer.y -= D3DX_PI * 2.0f;
			}

			if (rotDiffer.z < -D3DX_PI)
			{
				rotDiffer.z += D3DX_PI * 2.0f;
			}
			else if (rotDiffer.z > D3DX_PI)
			{
				rotDiffer.z -= D3DX_PI * 2.0f;
			}

			// ���݂̃L�[���玟�̃L�[�ɓ���������̈ʒu�����߂�
			posAsk.x = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].pos.x + posDiffer.x * (m_fCounter / nFrame);
			posAsk.y = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].pos.y + posDiffer.y * (m_fCounter / nFrame);
			posAsk.z = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].pos.z + posDiffer.z * (m_fCounter / nFrame);

			// ���݂̃L�[���玟�̃L�[�ɓ���������̌��������߂�
			rotAsk.x = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].rot.x + rotDiffer.x * (m_fCounter / nFrame);
			rotAsk.y = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].rot.y + rotDiffer.y * (m_fCounter / nFrame);
			rotAsk.z = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].rot.z + rotDiffer.z * (m_fCounter / nFrame);
		}

		// ���f���̈ʒu�ƌ����ɔ��f
		switch (m_type)
		{
		case MOTION_TYPE_PLAYER:
			pPlayer->SetModelPos(nCntModel, posAsk);
			pPlayer->SetModelRot(nCntModel, rotAsk);
			break;

		default:
			pEnemy->SetModelPos(nCntModel, posAsk);
			pEnemy->SetModelRot(nCntModel, rotAsk);
			break;
		}
	}

	if (m_type == MOTION_TYPE_PLAYER)
	{
		if (pPlayer->GetLand() == true)
		{
			m_bStop = false;
		}
	}

	if (m_bStop == false)
	{
		// �J�E���^�[�����Z
		m_fCounter += 1.0f;
	}

	// ���[�V�����������ł͂Ȃ��ꍇ
	if (m_bConnect == false)
	{
		// �J�E���^�[���t���[�����̍ő�l�𒴂�����
		if (m_fCounter >= m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame)
		{
			// �L�[��i�߂�
			m_nKey++;

			// ���݂̃L�[��(�L�[�� - 1)�ȏゾ������
			if (m_nKey >= m_aInfo[m_nType].nNumKey - 1)
			{
				// ���[�v���Ȃ����[�V�����̂Ƃ�
				if (m_aInfo[m_nType].nLoop == 0)
				{
					if (m_type == MOTION_TYPE_PLAYER)
					{
						// �W�����v���[�V����
						if (m_nType == 2)
						{
							if (pPlayer->GetLand() == false)
							{
								m_bStop = true;
							}
						}
						// �N���A���[�V����
						else if (m_nType == 11)
						{
							SetMotion(12);
						}
						else
						{
							SetMotion(0);
						}
					}
					else if (m_type == MOTION_TYPE_WOLF00 || m_type == MOTION_TYPE_WOLF01)
					{
						// �_�E�����[�V����
						if (m_nType == 4)
						{
							m_bStop = true;
						}
						else
						{
							SetMotion(0);
						}
					}
					else if (m_type == MOTION_TYPE_BOSS)
					{
						// �_�E�����[�V����
						if (m_nType == 3)
						{
							m_bStop = true;
						}
						// �W�����v(����)���[�V����
						else if (m_nType == 4)
						{
							m_bStop = true;
						}
						// �W�����v���[�V����
						else if (m_nType == 5)
						{
							m_bStop = true;
						}
						else
						{
							SetMotion(0);
						}
					}
					else
					{
						// ���̃��[�V�������j���[�g�������[�V�����ɂ���
						SetMotion(0);
					}
				}
				//���[�v���郂�[�V�����̂Ƃ�
				else
				{
					// �L�[���Ō�܂ł�������0�ɖ߂��ă��[�V�������J��Ԃ�
					if (m_nKey > m_aInfo[m_nType].nNumKey - 1)
					{
						m_nKey = 0;
					}
				}
			}

			// �J�E���^�[�����Z�b�g
			m_fCounter = 0.0f;
		}
	}
	// ���[�V�����������̏ꍇ
	else
	{
		// 10�t���[�������ă��[�V�����ڍs
		int nMaxFrame = 10;

		if (m_fCounter >= nMaxFrame)
		{
			m_bConnect = false;
			m_fCounter = 0.0f;
			m_nKey = 0;
			//���݂̃��[�V�����̎�ނ����̃��[�V�����̎�ނɂ���
			m_nType = m_nTypeNext;
		}
	}
}

//=============================================================================
// ��������
//=============================================================================
CMotion *CMotion::Create(CScene *pScene, MOTION_TYPE type)
{
	// �N���X�̐���
	CMotion *pMotion = NULL;
	pMotion = new CMotion;

	if (pMotion != NULL)
	{
		pMotion->Init(pScene, type);
	}
	return pMotion;
}

//=============================================================================
// ���[�V�����ݒ菈��
//=============================================================================
void CMotion::SetMotion(int nType)
{
	m_nTypeNext = nType;
	m_bConnect = true;
	m_bStop = false;
	m_fCounter = 0.0f;

	// �� �v���C���[�̃��[�V����
	/*�@[0] �j���[�g����
	/*	[1] �ړ�
	/*	[2] �W�����v
	/*	[3] ���n
	/*	[4] �_�b�V��
	/*	[5] �U���F�� (1�i��)
	/*	[6] �U���F�� (2�i��)
	/*	[7] �U���F�� (3�i��)
	/*	[8] �U���F�{�E�K��
	/*	[9] ��_���[�W
	/*	[10]�_�E��
	/*	[11]�N���A
	/*	[12]�N���A(�ҋ@)		*/

	// �� �T00�̃��[�V����
	/*�@[0] �j���[�g����
	/*	[1] �ړ�
	/*	[2] �U��
	/*	[3] ��_���[�W			
	/*	[4]	�_�E��				*/

	// �� �T01�̃��[�V����
	/*�@[0] �j���[�g����
	/*	[1] �ړ�
	/*	[2] �U��
	/*	[3] ��_���[�W			
	/*	[4]	�_�E��				*/

	// �� �{�X�̃��[�V����
	/*�@[0] �j���[�g����
	/*	[1] �ړ�
	/*	[2] �U��
	/*	[3] �_�E��				
	/*	[4] �W�����v(����)
	/*	[5] �W�����v
	/*	[6] ���n
	/*	[7] �`���[�W
	/*	[8] �_�b�V��			*/
}