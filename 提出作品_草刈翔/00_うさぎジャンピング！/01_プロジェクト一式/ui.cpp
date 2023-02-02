//=============================================================================
// UI���� [ui.h]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "ui.h"
#include "manager.h"
#include "texture.h"
#include "scene.h"
#include "control.h"
#include "control_ui_title_logo.h"
#include "control_ui_title_enter.h"
#include "control_ui_game_countdown.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CUi::CUi(int nPriority) :CScene2D(nPriority)
{
	// �ϐ��̃N���A
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pControl = NULL;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CUi::~CUi()
{

}

//=============================================================================
//����������
//=============================================================================
HRESULT CUi::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �p�����N���X�̏���������
	CScene2D::Init(pos, size);

	// �ϐ��̏�����
	m_size = size;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CUi::Uninit(void)
{
	// �p�����N���X�̏I������
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CUi::Update(void)
{
	// �R���g���[���N���X���k������Ȃ�������
	if (m_pControl != NULL)
	{
		// ���g�̃|�C���^�����A�X�V����
		m_pControl->Update(this);

		// Scene2D����ʒu���擾
		D3DXVECTOR3 pos;
		pos = GetPos();

		// �擾�����ʒu�Ɉړ��ʂ����Z
		pos += m_pControl->GetMove();

		// �ʒu�A�T�C�Y��ݒ�
		SetPos(pos, GetSize());
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CUi::Draw(void)
{
	// �p�����N���X�̕`�揈��
	CScene2D::Draw();
}

//=============================================================================
// ��������
//=============================================================================
CUi* CUi::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nPriolity, UI_TYPE type)
{
	// �C���X�^���X�̐���
	CUi *pUi = NULL;

	// �k���`�F�b�N
	if (pUi == NULL)
	{
		// �N���X�̐���
		pUi = new CUi(nPriolity);
		if (pUi != NULL)
		{
			// �w�肵��UI�̎�ނŃe�N�X�`����ς��A���蓖�Ă�
			switch (type)
			{
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// �ėp�n
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// ���փ{�^��
			case UI_TYPE_NEXT:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_NEXT);
				break;
				// ���ǂ�{�^��
			case UI_TYPE_BACK:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_BACK);
				break;
			case UI_TYPE_COMMA:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_COMMA);
				break;
				// ���[�g��
			case UI_TYPE_METER:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_METER);
				break;
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// �^�C�g���n
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// �^�C�g���F���S
			case UI_TYPE_TITLE_LOGO:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_TITLE_LOGO);
				pUi->m_pControl = CControlUiTitleLogo::Create();
				break;
				// �^�C�g���F�G���^�[����
			case UI_TYPE_TITLE_ENTER:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_TITLE_ENTER);
				pUi->m_pControl = CControlUiTitleEnter::Create();
				break;
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// ���j���[�n
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// ���j���[�F�g�b�v
			case UI_TYPE_MENU_TOP:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_MENU_TOP);
				break;
				// ���j���[�F�`���[�g���A�����[�h
			case UI_TYPE_MENU_TUTORIAL:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_MENU_TUTORIAL);
				break;
				// ���j���[�F�ЂƂ�悤���[�h
			case UI_TYPE_MENU_SINGLE:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_MENU_SINGLE);
				break;
				// ���j���[�F�ӂ���悤���[�h
			case UI_TYPE_MENU_PVP:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_MENU_PVP);
				break;
				// ���j���[�F�����L���O���[�h
			case UI_TYPE_MENU_RANKING:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_MENU_RANKING);
				break;
				// ���j���[�F�T�E���h�e�X�g
			case UI_TYPE_MENU_SOUNDTEST:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_MENU_SOUNDTEST);
				break;
				// ���j���[�F�K�C�h�c�`���[�g���A�����[�h
			case UI_TYPE_MENU_GUIDE_TUTORIAL:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_MENU_GUIDE_TUTORIAL);
				break;
				// ���j���[�F�K�C�h�c�ЂƂ�悤���[�h
			case UI_TYPE_MENU_GUIDE_SINGLE:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_MENU_GUIDE_SINGLE);
				break;
				// ���j���[�F�K�C�h�c�ӂ���悤���[�h
			case UI_TYPE_MENU_GUIDE_PVP:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_MENU_GUIDE_PVP);
				break;
				// ���j���[�F�K�C�h�c�����L���O���[�h
			case UI_TYPE_MENU_GUIDE_RANKING:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_MENU_GUIDE_RANKING);
				break;
				// ���j���[�F�K�C�h�c�T�E���h�e�X�g
			case UI_TYPE_MENU_GUIDE_SOUNDTEST:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_MENU_GUIDE_SOUNDTEST);
				break;
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// �`���[�g���A���n
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// �`���[�g���A���F�I��
			case UI_TYPE_TUTORIAL_END:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_TUTORIAL_END);
				break;
				// �`���[�g���A���F�X�L�b�v
			case UI_TYPE_TUTORIAL_SKIP:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_TUTORIAL_SKIP);
				break;
				// �`���[�g���A���F�{�^��
			case UI_TYPE_TUTORIAL_BUTTON:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_TUTORIAL_BUTTON);
				break;
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// 1�l�p�Q�[�����j���[�n
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// �\�����j���[�F�g�b�v
			case UI_TYPE_SOLO_MENU_TOP:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_SOLO_MENU_TOP);
				break;
				// �\�����j���[�FCPU�Ƃ�������
			case UI_TYPE_SOLO_MENU_CPU:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_SOLO_MENU_CPU);
				break;
				// �\�����j���[�F�X�R�A�A�^�b�N
			case UI_TYPE_SOLO_MENU_SCORE:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_SOLO_MENU_SCORE);
				break;
				// �\�����j���[�F�g�b�v
			case UI_TYPE_SOLO_MENU_DIFFICULTY_00:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_SOLO_MENU_DIFFICULTY_00);
				break;
				// �\�����j���[�FCPU�Ƃ�������
			case UI_TYPE_SOLO_MENU_DIFFICULTY_01:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_SOLO_MENU_DIFFICULTY_01);
				break;
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// �Q�[���n
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			case UI_TYPE_GAME_COUNTDOWN:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_GAME_COUNTDOWN);
				pUi->m_pControl = CControlUiGameCountdown::Create();
				break;
			case UI_TYPE_GAME_SCORE:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_GAME_SCORE_SENTENCE);
				break;
			case UI_TYPE_GAME_HIGHSCORE:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_GAME_HIGHSCORE_SENTENCE);
				break;
			case UI_TYPE_GAME_1P:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_GAME_1P);
				break;
			case UI_TYPE_GAME_2P:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_GAME_2P);
				break;
			case UI_TYPE_GAME_CPU:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_GAME_CPU);
				break;
			case UI_TYPE_GAME_JUMP_GUIDE:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_GAME_JUMP_GUIDE00);
				break;
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// ���U���g�n
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// ���U���g�F�g�b�v
			case UI_TYPE_RESULT_TOP:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_RESULT_TOP);
				break;
				// ���U���g�F����
			case UI_TYPE_RESULT_SENTENCE:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_RESULT_SENTENCE);
				break;
				// ���U���g�F��
			case UI_TYPE_RESULT_BAND:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_RESULT_BAND);
				break;
				// ���U���g�F���g���C
			case UI_TYPE_RESULT_RETRY:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_RESULT_RETRY);
				break;
				// ���U���g�F�����I
			case UI_TYPE_RESULT_WIN:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_RESULT_WIN);
				break;
				// ���U���g�F�Ђ��킯�c
			case UI_TYPE_RESULT_DRAW:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_RESULT_DRAW);
				break;
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// �����L���O�n
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// �����L���O�F�g�b�v
			case UI_TYPE_RANKING_TOP:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_RANKING_TOP);
				break;
				// �����L���O�F1�ʂ̘g
			case UI_TYPE_RANKING_ONE:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_RANKING_ONE);
				break;
				// �����L���O�F2�ʂ̘g
			case UI_TYPE_RANKING_TWO:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_RANKING_TWO);
				break;
				// �����L���O�F3�ʂ̘g
			case UI_TYPE_RANKING_THREE:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_RANKING_THREE);
				break;
				// �����L���O�F4�ʂ̘g
			case UI_TYPE_RANKING_FOUR:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_RANKING_FOUR);
				break;
				// �����L���O�F5�ʂ̘g
			case UI_TYPE_RANKING_FIVE:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_RANKING_FIVE);
				break;
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// �|�[�Y�n
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// �|�[�Y�G�g�b�v
			case UI_TYPE_PAUSE_TOP:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_PAUSE_TOP);
				break;
				// �|�[�Y�G�Â���
			case UI_TYPE_PAUSE_CONTINUE:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_PAUSE_CONTINUE);
				break;
				// �|�[�Y�G���Ȃ���
			case UI_TYPE_PAUSE_RETRY:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_PAUSE_RETRY);
				break;
				// �|�[�Y�G��߂�
			case UI_TYPE_PAUSE_QUIT:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_PAUSE_QUIT);
				break;
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// �|�[�Y�n
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// �T�E���h�e�X�g�F�g�b�v
			case UI_TYPE_SOUNDTEST_TOP:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_SOUNDTEST_TOP);
				break;
				// �T�E���h�e�X�g�F���C��
			case UI_TYPE_SOUNDTEST_MAIN:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_SOUNDTEST_MAIN);
				break;
				// �T�E���h�e�X�g�F�Đ�
			case UI_TYPE_SOUNDTEST_PLAY:
				pUi->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_SOUNDTEST_PLAY);
				break;

			default:
				break;
			}
			pUi->BindTexture(pUi->m_pTexture);

			// �ϐ��̏�����
			pUi->m_type = type;

			// ����������
			pUi->Init(pos, size);
		}
	}

	return pUi;
}

//=============================================================================
// ��ނ̎擾����
//=============================================================================
CUi::UI_TYPE CUi::GetUIType(void)
{
	return m_type;
}