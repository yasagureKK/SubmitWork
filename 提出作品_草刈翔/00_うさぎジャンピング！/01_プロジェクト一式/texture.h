//=============================================================================
// �e�N�X�`������ [texture.cpp]
// Authore : ���� ��
//=============================================================================
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CTexture
{
public:
	// �e�N�X�`���̎��
	typedef enum
	{
											//*****���Q�[���{�Ҍn��*****//
		TEX_TYPE_FIELD00 = 0,				// �t�B�[���h00(����)
		TEX_TYPE_FIELD01,					// �t�B�[���h01(�y�G�X�^�[�g)
		TEX_TYPE_FIELD02,					// �t�B�[���h02(�y)
		TEX_TYPE_WALL_FOREST,				// �E�H�[��(�X)
		TEX_TYPE_WALL_MOUNTAIN_00,			// �E�H�[��(�R0)[�X�e�[�W�̈�ԉ��ɔz�u����]
		TEX_TYPE_WALL_MOUNTAIN_01,			// �E�H�[��(�R1)[�R0�̎�O�ɔz�u����]
		TEX_TYPE_CLOUD00,					// �_00
		TEX_TYPE_CLOUD01,					// �_01
		TEX_TYPE_MOON,						// ��
		TEX_TYPE_EFFECT,					// �G�t�F�N�g/�e
		TEX_TYPE_SMOKE,						// ��
		TEX_TYPE_NUMBER,					// �i���o�[
		TEX_TYPE_COMMA,						// �����_
		TEX_TYPE_METER,						// ���[�g��
		TEX_TYPE_FLOWER_00,					// �w�i�I�u�W�F�N�g�F��00
		TEX_TYPE_FLOWER_01,					// �w�i�I�u�W�F�N�g�F��01
		TEX_TYPE_FLOWER_02,					// �w�i�I�u�W�F�N�g�F��02
		TEX_TYPE_FLOWER_03,					// �w�i�I�u�W�F�N�g�F��03
		TEX_TYPE_FLOWER_04,					// �w�i�I�u�W�F�N�g�F��04
		TEX_TYPE_FLOWER_05,					// �w�i�I�u�W�F�N�g�F��05
		TEX_TYPE_TREE,						// �w�i�I�u�W�F�N�g�F��
		TEX_TYPE_BUSH_00,					// �w�i�I�u�W�F�N�g�F�΂�
											//*********��UI�n��*********//
		TEX_TYPE_TITLE_LOGO,				// �^�C�g���F���S
		TEX_TYPE_TITLE_ENTER,				// �^�C�g���F�G���^�[����
		TEX_TYPE_TITLE_BG00,				// �^�C�g���F�w�i00
		TEX_TYPE_TITLE_BG01,				// �^�C�g���F�w�i01
		TEX_TYPE_MENU_TOP,					// ���j���[�F�g�b�v
		TEX_TYPE_MENU_TUTORIAL,				// ���j���[�F�`���[�g���A�����[�h
		TEX_TYPE_MENU_SINGLE,				// ���j���[�F�ЂƂ�悤���[�h
		TEX_TYPE_MENU_PVP,					// ���j���[�F�ӂ���悤���[�h
		TEX_TYPE_MENU_RANKING,				// ���j���[�F�����L���O���[�h
		TEX_TYPE_MENU_SOUNDTEST,			// ���j���[�G�T�E���h�e�X�g
		TEX_TYPE_MENU_GUIDE_TUTORIAL,		// ���j���[�F�K�C�h�c�`���[�g���A�����[�h
		TEX_TYPE_MENU_GUIDE_SINGLE,			// ���j���[�F�K�C�h�c�ЂƂ�悤���[�h
		TEX_TYPE_MENU_GUIDE_PVP,			// ���j���[�F�K�C�h�c�ӂ���悤���[�h
		TEX_TYPE_MENU_GUIDE_RANKING,		// ���j���[�F�K�C�h�c�����L���O���[�h
		TEX_TYPE_MENU_GUIDE_SOUNDTEST,		// ���j���[�G�K�C�h�c�T�E���h�e�X�g
		TEX_TYPE_MENU_BG,					// ���j���[�F�w�i
		TEX_TYPE_TUTORIAL_END,				// �`���[�g���A���F�I��
		TEX_TYPE_TUTORIAL_SKIP,				// �`���[�g���A���F�X�L�b�v
		TEX_TYPE_TUTORIAL_BUTTON,			// �`���[�g���A���F�{�^��
		TEX_TYPE_SOLO_MENU_TOP,				// �\�����j���[�G�g�b�v
		TEX_TYPE_SOLO_MENU_CPU,				// �\�����j���[�GCPU�Ƃ�������
		TEX_TYPE_SOLO_MENU_SCORE,			// �\�����j���[�G�X�R�A�A�^�b�N
		TEX_TYPE_SOLO_MENU_DIFFICULTY_00,	// �\�����j���[�GCPU�̂悳(�����o��)
		TEX_TYPE_SOLO_MENU_DIFFICULTY_01,	// �\�����j���[�GCPU�̂悳(����)
		TEX_TYPE_GAME_COUNTDOWN,			// �Q�[���F�J�E���g�_�E��
		TEX_TYPE_GAME_SCORE_SENTENCE,		// �Q�[���F�X�R�A����
		TEX_TYPE_GAME_HIGHSCORE_SENTENCE,	// �Q�[���F�n�C�X�R�A����
		TEX_TYPE_GAME_1P,					// �Q�[���F1P
		TEX_TYPE_GAME_2P,					// �Q�[���F2P
		TEX_TYPE_GAME_CPU,					// �Q�[���FCPU
		TEX_TYPE_GAME_JUMP_GUIDE00,			// �Q�[���F�W�����v�n�_�܂ł��Ɖ�m��
		TEX_TYPE_GAME_JUMP_GUIDE01,			// �Q�[���F���n�܂ł��Ɖ�m��
		TEX_TYPE_RESULT_TOP,				// ���U���g�F�g�b�v
		TEX_TYPE_RESULT_SENTENCE,			// ���U���g�F����
		TEX_TYPE_RESULT_BAND,				// ���U���g�F��
		TEX_TYPE_RESULT_BG,					// ���U���g�F�w�i
		TEX_TYPE_RESULT_RETRY,				// ���U���g�F���g���C
		TEX_TYPE_RESULT_WIN,				// ���U���g�F�����I
		TEX_TYPE_RESULT_DRAW,				// ���U���g�F�Ђ��킯�c
		TEX_TYPE_RANKING_TOP,				// �����L���O�F�g�b�v
		TEX_TYPE_RANKING_ONE,				// �����L���O�F1�ʂ̘g
		TEX_TYPE_RANKING_TWO,				// �����L���O�F2�ʂ̘g
		TEX_TYPE_RANKING_THREE,				// �����L���O�F3�ʂ̘g
		TEX_TYPE_RANKING_FOUR,				// �����L���O�F4�ʂ̘g
		TEX_TYPE_RANKING_FIVE,				// �����L���O�F5�ʂ̘g
		TEX_TYPE_NEXT,						// ���փ{�^��
		TEX_TYPE_BACK,						// ���ǂ�{�^��
		TEX_TYPE_PAUSE_TOP,					// �|�[�Y�G�g�b�v
		TEX_TYPE_PAUSE_CONTINUE,			// �|�[�Y�G�Â���
		TEX_TYPE_PAUSE_RETRY,				// �|�[�Y�G���Ȃ���
		TEX_TYPE_PAUSE_QUIT,				// �|�[�Y�G��߂�
		TEX_TYPE_SOUNDTEST_TOP,				// �T�E���h�e�X�g�F�g�b�v
		TEX_TYPE_SOUNDTEST_MAIN,			// �T�E���h�e�X�g�F���C��
		TEX_TYPE_SOUNDTEST_PLAY,			// �T�E���h�e�X�g�F�Đ�
		TEX_TYPE_MAX
	} TEX_TYPE;

	// ���K���[�h�ł̐����e�N�X�`���̎��
	typedef enum
	{
											//***�����K���[�h�̐�����***//
		GUIDE_TEX_TYPE_TUTORIAL_GUIDE00,	// ����0 [�悤����]
		GUIDE_TEX_TYPE_TUTORIAL_GUIDE01,	// ����1 [���������Ă��������܂�]
		GUIDE_TEX_TYPE_TUTORIAL_GUIDE02,	// ����2 [���������đ�W�����v]
		GUIDE_TEX_TYPE_TUTORIAL_GUIDE03,	// ����3 [���E�ŃX�s�[�h�A�b�v]
		GUIDE_TEX_TYPE_TUTORIAL_GUIDE04,	// ����4 [�ǂ�ǂ�J��Ԃ���]
		GUIDE_TEX_TYPE_TUTORIAL_GUIDE05,	// ����5 [A�ŃW�����v]
		GUIDE_TEX_TYPE_TUTORIAL_GUIDE06,	// ����6 [������x�W�����v]
		GUIDE_TEX_TYPE_TUTORIAL_GUIDE07,	// ����7 [�z�o�����O]
		GUIDE_TEX_TYPE_TUTORIAL_GUIDE08,	// ����8 [�L�^�����܂�]
		GUIDE_TEX_TYPE_TUTORIAL_GUIDE09,	// ����9 [�ǂ��܂łƂׂ邩]
		GUIDE_TEX_TYPE_TUTORIAL_GUIDE10,	// ����10[������@]
		GUIDE_TEX_TYPE_MAX
	} GUIDE_TEX_TYPE;

	CTexture();												// �R���X�g���N�^
	~CTexture();											// �f�X�g���N�^
	void Init(void);										// ����������
	void Uninit(void);										// �I������
	LPDIRECT3DTEXTURE9 GetTexture(TEX_TYPE Type);			// �e�N�X�`���擾����
	LPDIRECT3DTEXTURE9 GetGuideTex(GUIDE_TEX_TYPE Type);	// �����e�N�X�`���擾����

private:
	LPDIRECT3DTEXTURE9	m_apTexture[TEX_TYPE_MAX];			// �e�N�X�`���̃|�C���^
	LPDIRECT3DTEXTURE9	m_apGuideTex[GUIDE_TEX_TYPE_MAX];	// �����e�N�X�`���̃|�C���^
};

#endif