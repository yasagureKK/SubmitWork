//=============================================================================
// �v���C���[�̃��[�V�������� [motion_player.h]
// Authore : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "motion_player.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CMotionPlayer::CMotionPlayer()
{
	for (int nCntMotion = 0; nCntMotion < MOTION_PLAYER_TYPE_MAX; nCntMotion++)
	{
		m_aInfo[nCntMotion].nLoop = 0;
		m_aInfo[nCntMotion].nNumKey = 0;
		for (int nCntKey = 0; nCntKey < MAX_MOTION_PLAYER_KEY_INFO; nCntKey++)
		{
			m_aInfo[nCntMotion].aKeyInfo[nCntKey].nFrame = 0;
			for (int nCntModel = 0; nCntModel < MAX_PLAYER_MODEL; nCntModel++)
			{
				m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
		}
	}

	// �ϐ��̃N���A
	m_type = MOTION_PLAYER_TYPE_NEUTRAL;
	m_typeNext = MOTION_PLAYER_TYPE_NEUTRAL;
	m_nNumKey = 0;
	m_nKeyOld = 0;
	m_fCounter = 0.0f;
	m_nKey = 0;
	m_bConnect = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMotionPlayer::~CMotionPlayer()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CMotionPlayer::Init(CPlayer *pPlayer)
{
	// �e�L�X�g�t�@�C���̓ǂݍ���
	FILE *pfile = fopen(MOTION_PLAYER_FILE, "r");

	// �e�L�X�g�ۑ��p�̕ϐ�
	char cString[MAX_STRING];

	// ���[�V�����̓ǂݍ���
	for (int nCntMotion = 0; nCntMotion < MOTION_PLAYER_TYPE_MAX; nCntMotion++)
	{
		// �e�L�X�g�t�@�C���̍Ō�(NULL)�܂œǂݍ���
		while (fgets(cString, MAX_STRING, pfile) != NULL)
		{
			// �������ۑ�
			fscanf(pfile, "%s", cString);

			// ������̒���"MOTIONSET"����������
			if (strncmp("MOTIONSET", cString, 10) == 0)
			{
				//���[�v���ƃL�[�����擾
				fscanf(pfile, "%s%*s%d%*s%*s%*s%*s", cString, &m_aInfo[nCntMotion].nLoop);
				fscanf(pfile, "%s%*s%d", cString, &m_aInfo[nCntMotion].nNumKey);
				break;
			}
		}

		// �擾�����L�[���Ԃ�J��Ԃ�
		for (int nCntKey = 0; nCntKey < m_aInfo[nCntMotion].nNumKey; nCntKey++)
		{
			while (fgets(cString, MAX_STRING, pfile) != NULL)
			{
				// �������ۑ�
				fscanf(pfile, "%s", cString);

				// ������̒���"FRAME"����������
				if (strncmp("FRAME", cString, 6) == 0)
				{
					// �t���[�������擾����
					fscanf(pfile, "%s%d", cString, &m_aInfo[nCntMotion].aKeyInfo[nCntKey].nFrame);
					break;
				}
			}

			// �v���C���[�̃��f���̐������J��Ԃ�
			for (int nCntModel = 0; nCntModel < MAX_PLAYER_MODEL; nCntModel++)
			{
				while (fgets(cString, MAX_STRING, pfile) != NULL)
				{
					// �������ۑ�
					fscanf(pfile, "%s", cString);

					// ������̒���"POS"����������
					if (strncmp("POS", cString, 4) == 0)
					{
						//�ʒu�E��]�����擾
						fscanf(pfile, "%s%f%f%f", cString,
							&m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].pos.x,
							&m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].pos.y,
							&m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].pos.z);
						fscanf(pfile, "%s%*s%f%f%f%*s", cString,
							&m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].rot.x,
							&m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].rot.y,
							&m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].rot.z);
						break;
					}
				}
			}

			while (fgets(cString, MAX_STRING, pfile) != NULL)
			{
				// �������ۑ�
				fscanf(pfile, "%s", cString);

				// ������̒���"END_KEYSET"���������炻���ŃL�[���̎擾���I��
				if (strncmp("END_KEYSET", cString, 11) == 0)
				{
					break;
				}
			}
		}

		while (fgets(cString, MAX_STRING, pfile) != NULL)
		{
			// �������ۑ�
			fscanf(pfile, "%s", cString);

			// ������̒���"END_MOTIONSET"���������炻���Ń��[�V�����̎擾���I��
			if (strncmp("END_MOTIONSET", cString, 14) == 0)
			{
				break;
			}
		}
	}

	// �t�@�C�������
	fclose(pfile);

	// �e�q�֌W�ɂ��ʒu����̏C��
	for (int nCntMotion = 0; nCntMotion < MOTION_PLAYER_TYPE_MAX; nCntMotion++)
	{
		// �L�[���Ԃ�J��Ԃ�
		for (int nCntKey = 0; nCntKey < m_aInfo[nCntMotion].nNumKey; nCntKey++)
		{
			// �v���C���[�̃��f���̐������J��Ԃ�
			for (int nCntModel = 0; nCntModel < MAX_PLAYER_MODEL; nCntModel++)
			{
				// ���f���̈ʒu�ƌ������擾
				D3DXVECTOR3 pos = pPlayer->GetModelPos(nCntModel);
				D3DXVECTOR3 rot = pPlayer->GetModelRot(nCntModel);

				m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].pos.x = pos.x + m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].pos.x;
				m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].pos.y = pos.y + m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].pos.y;
				m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].pos.z = pos.z + m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].pos.z;
				m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].rot.x = rot.x + m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].rot.x;
				m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].rot.y = rot.y + m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].rot.y;
				m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].rot.z = rot.z + m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].rot.z;
			}
		}
	}

	// �ϐ��̏�����
	m_type = MOTION_PLAYER_TYPE_NEUTRAL;
	m_typeNext = MOTION_PLAYER_TYPE_NEUTRAL;
	m_fCounter = 0.0f;
	m_nKey = 0;
	m_bConnect = false;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CMotionPlayer::Uninit(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void CMotionPlayer::Update(CPlayer *pPlayer)
{
	D3DXVECTOR3 posAsk;		// ���߂����ʒu�̒l
	D3DXVECTOR3 rotAsk;		// ���߂��������̒l
	D3DXVECTOR3 posDiffer;	// �ʒu�̍���
	D3DXVECTOR3 rotDiffer;	// �����̍���
	int nKeyNext = 0;							// ���̃L�[

	// ���݂̃L�[���ő�l�ȏ�ɂȂ�����L�[���ŏ��ɖ߂�
	if (m_nKey >= m_aInfo[m_type].nNumKey - 1)
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

	for (int nCntModel = 0; nCntModel < MAX_PLAYER_MODEL; nCntModel++)
	{
		// �t���[����
		int nFrame = 0;
		if (m_bConnect == true)
		{
			nFrame = 10;

			// ���[�v����Ȃ�
			if (m_aInfo[m_type].nLoop == 1)
			{
				nFrame = m_aInfo[m_type].aKeyInfo[0].nFrame;
			}
		}
		else
		{
			nFrame = m_aInfo[m_type].aKeyInfo[m_nKey].nFrame;
		}

		// ���[�V�������Ȃ���ꍇ
		if (m_bConnect == true)
		{
			// ���f���̈ʒu�ƌ������擾
			D3DXVECTOR3 pos = pPlayer->GetModelPos(nCntModel);
			D3DXVECTOR3 rot = pPlayer->GetModelRot(nCntModel);

			// ���݂̃L�[�Ǝ��̃L�[�̈ʒu�̍��������߂�
			posDiffer.x = m_aInfo[m_typeNext].aKeyInfo[0].aKey[nCntModel].pos.x - pos.x;
			posDiffer.y = m_aInfo[m_typeNext].aKeyInfo[0].aKey[nCntModel].pos.y - pos.y;
			posDiffer.z = m_aInfo[m_typeNext].aKeyInfo[0].aKey[nCntModel].pos.z - pos.z;

			// ���݂̃L�[�Ǝ��̃L�[�̉�]�̍��������߂�
			rotDiffer.x = m_aInfo[m_typeNext].aKeyInfo[0].aKey[nCntModel].rot.x - rot.x;
			rotDiffer.y = m_aInfo[m_typeNext].aKeyInfo[0].aKey[nCntModel].rot.y - rot.y;
			rotDiffer.z = m_aInfo[m_typeNext].aKeyInfo[0].aKey[nCntModel].rot.z - rot.z;

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
			posDiffer.x = m_aInfo[m_typeNext].aKeyInfo[nKeyNext].aKey[nCntModel].pos.x - m_aInfo[m_type].aKeyInfo[m_nKey].aKey[nCntModel].pos.x;
			posDiffer.y = m_aInfo[m_typeNext].aKeyInfo[nKeyNext].aKey[nCntModel].pos.y - m_aInfo[m_type].aKeyInfo[m_nKey].aKey[nCntModel].pos.y;
			posDiffer.z = m_aInfo[m_typeNext].aKeyInfo[nKeyNext].aKey[nCntModel].pos.z - m_aInfo[m_type].aKeyInfo[m_nKey].aKey[nCntModel].pos.z;

			// ���݂̃L�[�Ǝ��̃L�[�̉�]�̍��������߂�
			rotDiffer.x = m_aInfo[m_typeNext].aKeyInfo[nKeyNext].aKey[nCntModel].rot.x - m_aInfo[m_type].aKeyInfo[m_nKey].aKey[nCntModel].rot.x;
			rotDiffer.y = m_aInfo[m_typeNext].aKeyInfo[nKeyNext].aKey[nCntModel].rot.y - m_aInfo[m_type].aKeyInfo[m_nKey].aKey[nCntModel].rot.y;
			rotDiffer.z = m_aInfo[m_typeNext].aKeyInfo[nKeyNext].aKey[nCntModel].rot.z - m_aInfo[m_type].aKeyInfo[m_nKey].aKey[nCntModel].rot.z;

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
			posAsk.x = m_aInfo[m_type].aKeyInfo[m_nKey].aKey[nCntModel].pos.x + posDiffer.x * (m_fCounter / nFrame);
			posAsk.y = m_aInfo[m_type].aKeyInfo[m_nKey].aKey[nCntModel].pos.y + posDiffer.y * (m_fCounter / nFrame);
			posAsk.z = m_aInfo[m_type].aKeyInfo[m_nKey].aKey[nCntModel].pos.z + posDiffer.z * (m_fCounter / nFrame);

			// ���݂̃L�[���玟�̃L�[�ɓ���������̌��������߂�
			rotAsk.x = m_aInfo[m_type].aKeyInfo[m_nKey].aKey[nCntModel].rot.x + rotDiffer.x * (m_fCounter / nFrame);
			rotAsk.y = m_aInfo[m_type].aKeyInfo[m_nKey].aKey[nCntModel].rot.y + rotDiffer.y * (m_fCounter / nFrame);
			rotAsk.z = m_aInfo[m_type].aKeyInfo[m_nKey].aKey[nCntModel].rot.z + rotDiffer.z * (m_fCounter / nFrame);
		}

		// ���f���̈ʒu�ƌ����ɔ��f
		pPlayer->SetModelPos(nCntModel, posAsk);
		pPlayer->SetModelRot(nCntModel, rotAsk);
	}

	// �J�E���^�[�����Z
	m_fCounter += 1.0f;

	// ���[�V�����������ł͂Ȃ��ꍇ
	if (m_bConnect == false)
	{
		// �J�E���^�[���t���[�����̍ő�l�𒴂�����
		if (m_fCounter >= m_aInfo[m_type].aKeyInfo[m_nKey].nFrame)
		{
			// �L�[��i�߂�
			m_nKey++;

			// ���݂̃L�[��(�L�[�� - 1)��菬����������
			if (m_nKey < m_aInfo[m_type].nNumKey - 1)
			{
				// �J�E���^�[�����Z�b�g
				m_fCounter = 0.0f;
			}
			// ���݂̃L�[��(�L�[�� - 1)�ȏゾ������
			else if (m_nKey >= m_aInfo[m_type].nNumKey - 1)
			{
				// ���[�v���Ȃ����[�V�����̂Ƃ�
				//�����[�v���Ȃ����[�V�����͔O�̂��߂ɑS�ď����Ă�������
				if (m_aInfo[m_type].nLoop == 0)
				{
					// ���n���[�V����
					if (m_type == MOTION_PLAYER_TYPE_LANDING)
					{
						D3DXVECTOR3 move = pPlayer->GetMove();

						// �����Ă��Ȃ��Ȃ�
						//�����0�ɂ���Ə��Ȃ��ړ��ʂł���u�����ړ����[�V�����Ɉڍs���邽�ߍ���͔͈͂�ݒ�
						if ((move.x <= 1.0f && move.x >= -1.0f) &&
							(move.z <= 1.0f && move.z >= -1.0f))
						{
							// ���̃��[�V�������j���[�g�������[�V�����ɂ���
							SetMotion(MOTION_PLAYER_TYPE_NEUTRAL);
						}
						// �����Ă���Ȃ�
						else
						{
							// ���̃��[�V�������ړ����[�V�����ɂ���
							SetMotion(MOTION_PLAYER_TYPE_MOVE);
						}
					}
					else if (m_type == MOTION_PLAYER_TYPE_1P_WIN)
					{
						// ���̃��[�V�������������ҋ@���[�V�����ɂ���
						SetMotion(MOTION_PLAYER_TYPE_1P_WIN_STAND);
					}
					else if (m_type == MOTION_PLAYER_TYPE_2P_WIN)
					{
						// ���̃��[�V�������������ҋ@���[�V�����ɂ���
						SetMotion(MOTION_PLAYER_TYPE_2P_WIN_STAND);
					}
					else
					{
						// ���̃��[�V�������j���[�g�������[�V�����ɂ���
						SetMotion(MOTION_PLAYER_TYPE_NEUTRAL);
					}
				}
				//���[�v���郂�[�V�����̂Ƃ�
				else
				{
					// �L�[���Ō�܂ł�������0�ɖ߂��ă��[�V�������J��Ԃ�
					if (m_nKey > m_aInfo[m_type].nNumKey - 1)
					{
						m_nKey = 0;
					}
				}

				m_fCounter = 0.0f;
			}
		}
	}
	// ���[�V�����������̏ꍇ
	else
	{
		// �t���[�����̍ő�l�𒴂�����
		float nMaxFrame = 10.0f;

		if (m_fCounter >= nMaxFrame)
		{
			m_bConnect = false;
			m_fCounter = 0.0f;
			m_nKey = 0;
			//���݂̃��[�V�����̎�ނ����̃��[�V�����̎�ނɂ���
			m_type = m_typeNext;
		}
	}
}

//=============================================================================
// ��������
//=============================================================================
CMotionPlayer *CMotionPlayer::Create(CPlayer *pPlayer)
{
	CMotionPlayer *pMotionPlayer = NULL;

	if (pMotionPlayer == NULL)
	{
		pMotionPlayer = new CMotionPlayer;
		if (pMotionPlayer != NULL)
		{
			pMotionPlayer->Init(pPlayer);
		}
	}

	return pMotionPlayer;
}


//=============================================================================
// ���[�V�����ݒ菈��
//=============================================================================
void CMotionPlayer::SetMotion(MOTION_PLAYER_TYPE type)
{
	m_typeNext = type;
	m_bConnect = true;
	m_fCounter = 0.0f;
}

//=============================================================================
// ���[�V�����擾����
//=============================================================================
CMotionPlayer::MOTION_PLAYER_TYPE CMotionPlayer::GetMotion(void)
{
	return m_type;
}

//=============================================================================
// ���[�V���������擾����
//=============================================================================
bool CMotionPlayer::GetConnect(void)
{
	return m_bConnect;
}