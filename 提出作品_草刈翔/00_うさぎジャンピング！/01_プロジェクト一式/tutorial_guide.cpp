//=============================================================================
// ���K���[�h�̃K�C�h���� [tutorial_guide.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "tutorial_guide.h"
#include "manager.h"
#include "texture.h"
#include "fade.h"
#include "input_keyboard.h"
#include "input_pad.h"
#include "sound.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
int CTutorialGuide::m_nNowPage = 0;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTutorialGuide::CTutorialGuide(int nPriority) :CScene(nPriority)
{
	// �ϐ��̃N���A
	memset(&m_pScene2D, NULL, sizeof(m_pScene2D));
	m_bDisplay = false;
	m_bDeleteDisplay = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTutorialGuide::~CTutorialGuide()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTutorialGuide::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �ϐ��̏�����
	m_nNowPage = 0;
	m_bDisplay = false;
	m_bDeleteDisplay = false;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTutorialGuide::Uninit(void)
{
	// �j������O�ɕ\����Ԃ�����
	m_bDisplay = false;

	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CTutorialGuide::Update(void)
{
	//�L�[�{�[�h�擾����
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//�p�b�h�擾����
	CInputPad *pInputPad;
	pInputPad = CManager::GetInputPad();

	//�T�E���h�擾����
	CSound *pSound;
	pSound = CManager::GetSound();

	//�t�F�[�h�擾����
	CFade *pFade;
	pFade = CManager::GetFade();

	// �t�F�[�h���Ă��Ȃ����̂ݑ���ł���
	if (pFade->GetFade() == CFade::FADE_NONE)
	{
		// �K�C�h��\�����Ă��Ȃ����A�܂������Ȃ��ꍇ�ɏ���
		if (m_bDisplay == false && m_bDeleteDisplay == false)
		{
			// ������true�ɂ���1�񂾂�����������
			m_bDisplay = true;

			// ���b�Z�[�W�E�B���h�E�̐���
			CreateMessageWindow();
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTutorialGuide::Draw(void)
{

}

//=============================================================================
// ��������
//=============================================================================
CTutorialGuide *CTutorialGuide::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �C���X�^���X�̐���
	CTutorialGuide *pPause = NULL;

	// �k���`�F�b�N
	if (pPause == NULL)
	{
		// �N���X�̐���
		pPause = new CTutorialGuide;
		if (pPause != NULL)
		{
			// ����������
			pPause->Init(pos, size);
		}
	}

	return pPause;
}

//=============================================================================
// �y�[�W���ݒ菈��
//=============================================================================
void CTutorialGuide::SetPage(int nNum)
{
	m_nNowPage = nNum;
}

//=============================================================================
// �y�[�W���擾����
//=============================================================================
int CTutorialGuide::GetPage(void)
{
	return m_nNowPage;
}

//=============================================================================
// ���b�Z�[�W�E�B���h�E����鏈��
//=============================================================================
void CTutorialGuide::CreateMessageWindow(void)
{
	// ���b�Z�[�W�E�B���h�E���܂������ꍇ
	if (m_pScene2D[0] == NULL)
	{
		// ���b�Z�[�W�E�B���h�E�����
		m_pScene2D[0] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, TUTORIAL_GUIDE_UI_FRAME_POS_Y, 0.0f), D3DXVECTOR3(TUTORIAL_GUIDE_UI_FRAME_SIZE_X, TUTORIAL_GUIDE_UI_FRAME_SIZE_Y, 0.0f), PRIORITY_UI);
		m_pScene2D[0]->SetColor(D3DCOLOR_RGBA(0, 0, 0, 155));
	}

	// ���݂̃y�[�W��int�^��񋓌^�ɃL���X�g����
	CTexture::GUIDE_TEX_TYPE num = (CTexture::GUIDE_TEX_TYPE)m_nNowPage;

	// ����
	m_pScene2D[1] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, TUTORIAL_GUIDE_UI_FRAME_POS_Y, 0.0f), D3DXVECTOR3(TUTORIAL_GUIDE_UI_FRAME_SIZE_X - 30.0f, TUTORIAL_GUIDE_UI_FRAME_SIZE_Y - 35.0f, 0.0f), PRIORITY_UI);
	m_pScene2D[1]->BindTexture(CManager::GetTexture()->GetGuideTex(num));
}

//=============================================================================
// �y�[�W��i�߂鏈��
//=============================================================================
void CTutorialGuide::AdvancePage(void)
{
	//�T�E���h�擾����
	CSound *pSound;
	pSound = CManager::GetSound();

	// ������false�ɂ���1�񂾂�����������
	m_bDisplay = false;

	// �񋓌^�ł���y�[�W�̍ő�l��int�^�ɃL���X�g����
	int nNum = (int)CTexture::GUIDE_TEX_TYPE_MAX;

	// ���݂̃y�[�W���y�[�W�̍ő�l-2��艺��������
	if (m_nNowPage < nNum - 2)
	{
		// �y�[�W��i�߂�
		m_nNowPage++;
	}

	// 2D�I�u�W�F�N�g�̕����̂ݔj��
	m_pScene2D[1]->Uninit();
	delete m_pScene2D[1];
	m_pScene2D[1] = NULL;
}

//=============================================================================
// �y�[�W��i�߁A���b�Z�[�W�E�B���h�E��j�����鏈��
//=============================================================================
void CTutorialGuide::AddPageAndDestroyWindow(void)
{
	//�T�E���h�擾����
	CSound *pSound;
	pSound = CManager::GetSound();

	// ������false�ɂ���1�񂾂�����������
	m_bDisplay = false;

	// �񋓌^�ł���y�[�W�̍ő�l��int�^�ɃL���X�g����
	int nNum = (int)CTexture::GUIDE_TEX_TYPE_MAX;

	// ���݂̃y�[�W���y�[�W�̍ő�l-2��艺��������
	if (m_nNowPage < nNum - 2)
	{
		// �y�[�W��i�߂�
		m_nNowPage++;
	}

	// 2D�I�u�W�F�N�g�̔j��
	for (int nCntScene2D = 0; nCntScene2D < 2; nCntScene2D++)
	{
		m_pScene2D[nCntScene2D]->Uninit();
		delete m_pScene2D[nCntScene2D];
		m_pScene2D[nCntScene2D] = NULL;
	}

	// �K�C�h�̕\��������
	m_bDeleteDisplay = true;
}