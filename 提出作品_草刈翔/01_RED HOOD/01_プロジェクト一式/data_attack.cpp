//=============================================================================
// �U�����Ǘ� [data_attack.cpp]
// Aouthor : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "data_attack.h"
#include "manager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ATTACK_DATA_FILE	"data/TEXT/attack_data.txt"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CDataAttack::CDataAttack()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CDataAttack::~CDataAttack()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CDataAttack::Init(void)
{
	// �e�L�X�g�t�@�C���̓ǂݍ���
	char cString[256];
	FILE *pFile = fopen(ATTACK_DATA_FILE, "r");

	int nCntAtk = 0;

	if (pFile != NULL)
	{
		while (1)
		{
			fscanf(pFile, "%s", &cString[0]);

			if (strcmp(&cString[0], "ATTACKSET") == 0) // MOTIONSET�̕�����
			{
				while (1)
				{
					fscanf(pFile, "%s", &cString[0]);

					if (strcmp(&cString[0], "WAIT") == 0) //�U���\�܂ł̃t���[��
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%d", &m_Data[nCntAtk].nWait);
					}
					if (strcmp(&cString[0], "BEGIN_COLLISION") == 0) //�R���W����������t���[��
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%d", &m_Data[nCntAtk].nBeginCollision);
					}
					if (strcmp(&cString[0], "BEGIN_EFFECT") == 0) //�G�t�F�N�g������t���[��
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%d", &m_Data[nCntAtk].nBeginEffect);
					}
					if (strcmp(&cString[0], "TIME_COLLISION") == 0) //�R���W�����̎�������
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%f", &m_Data[nCntAtk].fTimeCollision);
					}
					if (strcmp(&cString[0], "TIME_EFFECT") == 0) //�G�t�F�N�g�̎�������
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%f", &m_Data[nCntAtk].fTimeEffect);
					}
					if (strcmp(&cString[0], "DAMAGE") == 0) //�_���[�W�{��
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%f", &m_Data[nCntAtk].fDamage);
					}

					if (strcmp(&cString[0], "END_ATTACKSET") == 0)	// END_MOTIONSET�̕�����
					{
						nCntAtk++;
						break;
					}
				}
			}

			if (strcmp(&cString[0], "END_SCRIPT") == 0) //�I���
			{
				break;
			}
		}
		// �t�@�C�������
		fclose(pFile);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CDataAttack::Uninit(void)
{
	
}

//=============================================================================
// ��������
//=============================================================================
CDataAttack *CDataAttack::Create(void)
{
	// �N���X�̐���
	CDataAttack *pAttackData = NULL;
	pAttackData = new CDataAttack;
	
	if (pAttackData != NULL)
	{
		// ����������
		pAttackData->Init();
	}
	return pAttackData;
}

//=============================================================================
// ���擾����
//=============================================================================
float CDataAttack::GetAttackData(ATTACK_TYPE atkType, ATTACK_DATA_TYPE type)
{
	int nNumber = (int)atkType;

	// �w�肳�ꂽ�ԍ����U���̎�ނ̍ő吔��菭�Ȃ�������
	if (nNumber < MAX_ATTACK)
	{
		switch (type)
		{
		case ATTACK_DATA_TYPE_WAIT:
			return (float)m_Data[nNumber].nWait;
			break;

		case ATTACK_DATA_TYPE_BEGIN_COLLISION:
			return (float)m_Data[nNumber].nBeginCollision;
			break;

		case ATTACK_DATA_TYPE_BEGIN_EFFECT:
			return (float)m_Data[nNumber].nBeginEffect;
			break;

		case ATTACK_DATA_TYPE_TIME_COLLISION:
			return m_Data[nNumber].fTimeCollision;
			break;

		case ATTACK_DATA_TYPE_TIME_EFFECT:
			return m_Data[nNumber].fTimeEffect;
			break;

		case ATTACK_DATA_TYPE_DAMAGE:
			return m_Data[nNumber].fDamage;
			break;

		default:
			return false;
			break;
		}
	}
	else
	{
		return false;
	}
}