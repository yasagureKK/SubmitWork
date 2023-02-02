//=============================================================================
// �T�E���h�i���o�[���� [number_sound.cpp]
// Authore : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "number_sound.h"
#include "number.h"
#include "manager.h"
#include "scene.h"
#include "player.h"
#include "ui.h"
#include "input_keyboard.h"
#include "input_pad.h"
#include "sound.h"
#include "fade.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CNumberSound::CNumberSound(int nPriority) : CScene(nPriority)
{
	memset(&m_apNumber, NULL, sizeof(m_apNumber));
	m_nSoundNumber = 0;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CNumberSound::~CNumberSound()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CNumberSound::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �ϐ��̏�����
	m_pos = pos;
	m_size = size;

	for (int nCntScore = 0; nCntScore < MAX_NUMBER_SOUND_DIGIT; nCntScore++)
	{
		// �i���o�[�̐���
		m_apNumber[nCntScore] = CNumber::Create(D3DXVECTOR3(m_pos.x + (nCntScore * (m_size.x / 5 * 4)), m_pos.y, m_pos.z), m_size, m_col);
	}

	// �ʒu�E�T�C�Y�̐ݒ�
	SetPos(m_pos);
	SetSize(m_size);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CNumberSound::Uninit(void)
{
	// �i���o�[�̔j��
	for (int nCntScore = 0; nCntScore < MAX_NUMBER_SOUND_DIGIT; nCntScore++)
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
void CNumberSound::Update(void)
{
	//�L�[�{�[�h�擾����
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	// �Q�[���p�b�h�擾����
	CInputPad *pInputPad;
	pInputPad = CManager::GetInputPad();

	//�T�E���h�擾����
	CSound *pSound;
	pSound = CManager::GetSound();

	//�t�F�[�h�擾����
	CFade *pFade;
	pFade = CManager::GetFade();

	for (int nCntNumber = 0; nCntNumber < MAX_NUMBER_SOUND_DIGIT; nCntNumber++)
	{
		if (nCntNumber == MAX_NUMBER_SOUND_DIGIT - 1)
		{
			// �Ō��10��n�悪�g���Ȃ��Ȃ邽�߁A���ߑł��ŏ���
			m_apNumber[nCntNumber]->SetNumber(m_nSoundNumber % 10 / 1);
		}
		else
		{
			// ���X�R�A % 10�~n / 10�~(n-1)�� �ŏo���l�����ꂼ��̃i���o�[�ɐݒ�
			//��powf(float x, float y) �c x��y��̒l
			m_apNumber[nCntNumber]->SetNumber(m_nSoundNumber % (int)powf(10, MAX_NUMBER_SOUND_DIGIT - (float)nCntNumber) / (int)powf(10, MAX_NUMBER_SOUND_DIGIT - ((float)nCntNumber + 1)));
		}
	}

	// �t�F�[�h���Ă��Ȃ����̂ݑ���ł���
	if (pFade->GetFade() == CFade::FADE_NONE)
	{
		for (int nNum = 0; nNum < MAX_PLAYER_NUM; nNum++)
		{
			int nLeft, nRight;

			if (nNum == 0)
			{
				nLeft = DIK_A;
				nRight = DIK_D;
			}
			else if (nNum == 1)
			{
				nLeft = DIK_LEFT;
				nRight = DIK_RIGHT;
			}

			//***********************************************************************
			// �I�� (1P:W/S�A2P:��/�� �܂��� �p�b�h���X�e�B�b�N/�\���L�[)
			//***********************************************************************
			//��
			if (pInputKeyboard->GetTrigger(nLeft) == true ||
				pInputPad->GetTrigger(CInputPad::PAD_INPUTTYPE_LSTICK_LEFT, nNum) == true ||
				pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_DPAD_LEFT, nNum) == true)
			{
				// SE�̍Đ�
				pSound->Play(CSound::SOUND_LABEL_CURSOR);
				pSound->ControllVoice(CSound::SOUND_LABEL_CURSOR, 0.4f);

				m_nSoundNumber--;
			}
			//�E
			else if (pInputKeyboard->GetTrigger(nRight) == true ||
				pInputPad->GetTrigger(CInputPad::PAD_INPUTTYPE_LSTICK_RIGHT, nNum) == true ||
				pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_DPAD_RIGHT, nNum) == true)
			{
				// SE�̍Đ�
				pSound->Play(CSound::SOUND_LABEL_CURSOR);
				pSound->ControllVoice(CSound::SOUND_LABEL_CURSOR, 0.4f);

				m_nSoundNumber++;
			}

			// �I�����̐���
			if (m_nSoundNumber < 0)
			{
				m_nSoundNumber = 10;
			}
			else if (m_nSoundNumber > 10)
			{
				m_nSoundNumber = 0;
			}

			//***********************************************************************
			// ���� (Enter/Space �܂��� �p�b�hA�{�^��)
			//***********************************************************************
			if (pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
				pInputKeyboard->GetTrigger(DIK_SPACE) == true ||
				pInputPad->GetButtonTrigger(XINPUT_GAMEPAD_A, nNum) == true)
			{
				// �Đ��̑O��BGM���X�g�b�v
				pSound->Stop();

				// BGM���Đ�����
				PlaySound(m_nSoundNumber);
			}
		}
	}

	// �i���o�[���X�V
	SetNumber(m_nSoundNumber);
}

//=============================================================================
// �`�揈��
//=============================================================================
void CNumberSound::Draw(void)
{
	for (int nCntScore = 0; nCntScore < MAX_NUMBER_SOUND_DIGIT; nCntScore++)
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
CNumberSound *CNumberSound::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col)
{
	// �C���X�^���X�̐���
	CNumberSound *pNumberSound = NULL;

	// �k���`�F�b�N
	if (pNumberSound == NULL)
	{
		pNumberSound = new CNumberSound;
		if (pNumberSound != NULL)
		{
			// �ϐ��̐ݒ�
			pNumberSound->m_col = col;

			// ����������
			pNumberSound->Init(pos, size);
		}
	}
	return pNumberSound;
}

//=============================================================================
// �X�R�A�ݒ菈��
//=============================================================================
void CNumberSound::SetNumber(int nNumber)
{
	m_nSoundNumber = nNumber;
}

//=============================================================================
// �T�E���h�Đ�����
//=============================================================================
void CNumberSound::PlaySound(int nNum)
{
	//�T�E���h�擾����
	CSound *pSound;
	pSound = CManager::GetSound();

	// �w�肳�ꂽ����(int�^)��񋓌^�ɃL���X�g����
	CSound::SOUND_LABEL num = (CSound::SOUND_LABEL)nNum;

	// BGM�̍Đ�
	pSound->Play(num);
	pSound->ControllVoice(num, 0.4f);
}