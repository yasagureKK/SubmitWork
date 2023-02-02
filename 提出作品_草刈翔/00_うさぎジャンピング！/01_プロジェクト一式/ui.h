//=============================================================================
// UI���� [ui.h]
// Author : ���� ��
//=============================================================================
#ifndef _UI_H_
#define _UI_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene2D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define UI_TOP_POS_X (250.0f)	// �g�b�v�̈ʒuX
#define UI_TOP_POS_Y (60.0f)	// �g�b�v�̈ʒuY
#define UI_TOP_SIZE_X (750.0f)	// �g�b�v�̃T�C�YX
#define UI_TOP_SIZE_Y (120.0f)	// �g�b�v�̃T�C�YY
#define UI_BACK_POS_X (100.0f)	// ���ǂ�{�^���̈ʒuX
#define UI_BACK_POS_Y (680.0f)	// ���ǂ�{�^���̈ʒuY
#define UI_BACK_SIZE_X (300.0f)	// ���ǂ�{�^���̃T�C�YX
#define UI_BACK_SIZE_Y (100.0f)	// ���ǂ�{�^���̃T�C�YY

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CControl;

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CUi : public CScene2D
{
public:
	// UI�̎��
	typedef enum
	{
		UI_TYPE_NEXT = 0,				// ���փ{�^��
		UI_TYPE_BACK,					// ���ǂ�{�^��
		UI_TYPE_COMMA,					// �����_
		UI_TYPE_METER,					// ���[�g��
		UI_TYPE_TITLE_LOGO,				// �^�C�g���F���S
		UI_TYPE_TITLE_ENTER,			// �^�C�g���F�G���^�[
		UI_TYPE_MENU_TOP,				// ���j���[�F�g�b�v
		UI_TYPE_MENU_TUTORIAL,			// ���j���[�F�`���[�g���A�����[�h
		UI_TYPE_MENU_SINGLE,			// ���j���[�F�ЂƂ�悤���[�h
		UI_TYPE_MENU_PVP,				// ���j���[�F�ӂ���悤���[�h
		UI_TYPE_MENU_RANKING,			// ���j���[�F�����L���O���[�h
		UI_TYPE_MENU_SOUNDTEST,			// ���j���[�G�T�E���h�e�X�g
		UI_TYPE_MENU_GUIDE_TUTORIAL,	// ���j���[�F�K�C�h�c�`���[�g���A�����[�h
		UI_TYPE_MENU_GUIDE_SINGLE,		// ���j���[�F�K�C�h�c�ЂƂ�悤���[�h
		UI_TYPE_MENU_GUIDE_PVP,			// ���j���[�F�K�C�h�c�ӂ���悤���[�h
		UI_TYPE_MENU_GUIDE_RANKING,		// ���j���[�F�K�C�h�c�����L���O���[�h
		UI_TYPE_MENU_GUIDE_SOUNDTEST,	// ���j���[�G�K�C�h�c�T�E���h�e�X�g
		UI_TYPE_TUTORIAL,				// �`���[�g���A���F
		UI_TYPE_TUTORIAL_END,			// �`���[�g���A���F�I��
		UI_TYPE_TUTORIAL_SKIP,			// �`���[�g���A���F�X�L�b�v
		UI_TYPE_TUTORIAL_BUTTON,		// �`���[�g���A���F�{�^��
		UI_TYPE_SOLO_MENU_TOP,			// �\�����j���[�F�g�b�v
		UI_TYPE_SOLO_MENU_CPU,			// �\�����j���[�FCPU�Ƃ�������
		UI_TYPE_SOLO_MENU_SCORE,		// �\�����j���[�F�X�R�A�A�^�b�N
		UI_TYPE_SOLO_MENU_DIFFICULTY_00,// �\�����j���[�GCPU�̂悳(�����o��)
		UI_TYPE_SOLO_MENU_DIFFICULTY_01,// �\�����j���[�GCPU�̂悳(����)
		UI_TYPE_GAME_COUNTDOWN,			// �Q�[���F�J�E���g�_�E��
		UI_TYPE_GAME_SCORE,				// �Q�[���F�X�R�A����
		UI_TYPE_GAME_HIGHSCORE,			// �Q�[���F�n�C�X�R�A����
		UI_TYPE_GAME_1P,				// �Q�[���F1P
		UI_TYPE_GAME_2P,				// �Q�[���F2P
		UI_TYPE_GAME_CPU,				// �Q�[���FCPU
		UI_TYPE_GAME_JUMP_GUIDE,		// �Q�[���F�W�����v�n�_�܂ł��Ɖ�m��
		UI_TYPE_RESULT_TOP,				// ���U���g�F�g�b�v
		UI_TYPE_RESULT_SENTENCE,		// ���U���g�F����
		UI_TYPE_RESULT_BAND,			// ���U���g�F��
		UI_TYPE_RESULT_RETRY,			// ���U���g�F���g���C
		UI_TYPE_RESULT_WIN,				// ���U���g�F�����I
		UI_TYPE_RESULT_DRAW,			// ���U���g�F�Ђ��킯�c
		UI_TYPE_RANKING_TOP,			// �����L���O�F�g�b�v
		UI_TYPE_RANKING_ONE,			// �����L���O�F1�ʂ̘g
		UI_TYPE_RANKING_TWO,			// �����L���O�F2�ʂ̘g
		UI_TYPE_RANKING_THREE,			// �����L���O�F3�ʂ̘g
		UI_TYPE_RANKING_FOUR,			// �����L���O�F4�ʂ̘g
		UI_TYPE_RANKING_FIVE,			// �����L���O�F5�ʂ̘g
		UI_TYPE_PAUSE_TOP,				// �|�[�Y�G�g�b�v
		UI_TYPE_PAUSE_CONTINUE,			// �|�[�Y�G�Â���
		UI_TYPE_PAUSE_RETRY,			// �|�[�Y�G���Ȃ���
		UI_TYPE_PAUSE_QUIT,				// �|�[�Y�G��߂�
		UI_TYPE_SOUNDTEST_TOP,			// �T�E���h�e�X�g�F�g�b�v
		UI_TYPE_SOUNDTEST_MAIN,			// �T�E���h�e�X�g�F���C��
		UI_TYPE_SOUNDTEST_PLAY,			// �T�E���h�e�X�g�F�Đ�
		UI_TYPE_MAX
	}UI_TYPE;

	CUi(int nPriority = PRIORITY_UI);						// �R���X�g���N�^
	~CUi();													// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);		// ����������
	void Uninit(void);										// �I������
	void Update(void);										// �X�V����
	void Draw(void);										// �`�揈��
	static CUi *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size,
		int nPriolty, UI_TYPE type);						// ��������
	UI_TYPE GetUIType(void);								// ��ގ擾����

private:
	UI_TYPE m_type;											// UI�̎��
	LPDIRECT3DTEXTURE9 m_pTexture;							// �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3 m_size;										// �T�C�Y
	CControl *m_pControl;									// �R���g���[���N���X�̃|�C���^
};

#endif // _UI_H_