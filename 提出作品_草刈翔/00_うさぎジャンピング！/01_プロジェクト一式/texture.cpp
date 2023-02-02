//=============================================================================
// �e�N�X�`������ [texture.cpp]
// Authore : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "texture.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTexture::CTexture()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTexture::~CTexture()
{

}

//=============================================================================
// ����������
//=============================================================================
void CTexture::Init(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	//**************************
	//*  �e�N�X�`���̓ǂݍ���  *
	//**************************
	// �t�B�[���h00(����)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/field00.jpg",
		&m_apTexture[TEX_TYPE_FIELD00]);
	// �t�B�[���h02(�y�G�X�^�[�g)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/field01.png",
		&m_apTexture[TEX_TYPE_FIELD01]);
	// �t�B�[���h03(�y)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/field02.png",
		&m_apTexture[TEX_TYPE_FIELD02]);

	// �E�H�[��00(�X)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/woods00.png",
		&m_apTexture[TEX_TYPE_WALL_FOREST]);
	// �E�H�[��(�R0)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/mountains00.png",
		&m_apTexture[TEX_TYPE_WALL_MOUNTAIN_00]);
	// �E�H�[��(�R1)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/mountains01.png",
		&m_apTexture[TEX_TYPE_WALL_MOUNTAIN_01]);
	// �_00
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/cloud000.png",
		&m_apTexture[TEX_TYPE_CLOUD00]);
	// �_01
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/cloud001.png",
		&m_apTexture[TEX_TYPE_CLOUD01]);
	// ��
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/moon.png",
		&m_apTexture[TEX_TYPE_MOON]);
	// �G�t�F�N�g/�e
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/effect000.jpg",
		&m_apTexture[TEX_TYPE_EFFECT]);
	// ��
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/smoke.png",
		&m_apTexture[TEX_TYPE_SMOKE]);
	// �i���o�[
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/number00.png",
		&m_apTexture[TEX_TYPE_NUMBER]);
	// �����_
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/comma.png",
		&m_apTexture[TEX_TYPE_COMMA]);
	// ���[�g��
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/meter.png",
		&m_apTexture[TEX_TYPE_METER]);

	// �w�i�I�u�W�F�N�g�F��00
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/flower00.png",
		&m_apTexture[TEX_TYPE_FLOWER_00]);
	// �w�i�I�u�W�F�N�g�F��01
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/flower01.png",
		&m_apTexture[TEX_TYPE_FLOWER_01]);
	// �w�i�I�u�W�F�N�g�F��02
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/flower02.png",
		&m_apTexture[TEX_TYPE_FLOWER_02]);
	// �w�i�I�u�W�F�N�g�F��03
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/flower03.png",
		&m_apTexture[TEX_TYPE_FLOWER_03]);
	// �w�i�I�u�W�F�N�g�F��04
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/flower04.png",
		&m_apTexture[TEX_TYPE_FLOWER_04]);
	// �w�i�I�u�W�F�N�g�F��05
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/flower05.png",
		&m_apTexture[TEX_TYPE_FLOWER_05]);
	// �w�i�I�u�W�F�N�g�F��
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/tree.png",
		&m_apTexture[TEX_TYPE_TREE]);
	// �w�i�I�u�W�F�N�g�F�΂�
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bush00.png",
		&m_apTexture[TEX_TYPE_BUSH_00]);

	//+----------------+
	//|  �V�[������UI  |
	//+----------------+
	// �^�C�g���F���S
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_title_logo.png",
		&m_apTexture[TEX_TYPE_TITLE_LOGO]);
	// �^�C�g���F�G���^�[����
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_title_enter.png",
		&m_apTexture[TEX_TYPE_TITLE_ENTER]);
	// �^�C�g���F�w�i00
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/title_bg00.jpg",
		&m_apTexture[TEX_TYPE_TITLE_BG00]);
	// �^�C�g���F�w�i01
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/title_bg01.png",
		&m_apTexture[TEX_TYPE_TITLE_BG01]);

	// ���j���[�F�g�b�v
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_menu_top.png",
		&m_apTexture[TEX_TYPE_MENU_TOP]);
	// ���j���[�F�`���[�g���A�����[�h
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_menu_tutorial.png",
		&m_apTexture[TEX_TYPE_MENU_TUTORIAL]);
	// ���j���[�F�ЂƂ�悤���[�h
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_menu_singlemode.png",
		&m_apTexture[TEX_TYPE_MENU_SINGLE]);
	// ���j���[�F�ӂ���悤���[�h
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_menu_pvpmode.png",
		&m_apTexture[TEX_TYPE_MENU_PVP]);
	// ���j���[�F�����L���O���[�h
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_menu_ranking.png",
		&m_apTexture[TEX_TYPE_MENU_RANKING]);
	// ���j���[�F�T�E���h�e�X�g
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_menu_soundtest.png",
		&m_apTexture[TEX_TYPE_MENU_SOUNDTEST]);
	// ���j���[�F�K�C�h�c�`���[�g���A�����[�h
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_menu_guide_tutorial.png",
		&m_apTexture[TEX_TYPE_MENU_GUIDE_TUTORIAL]);
	// ���j���[�F�K�C�h�c�ЂƂ�悤���[�h
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_menu_guide_game.png",
		&m_apTexture[TEX_TYPE_MENU_GUIDE_SINGLE]);
	// ���j���[�F�K�C�h�c�ӂ���悤���[�h
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_menu_guide_pvp.png",
		&m_apTexture[TEX_TYPE_MENU_GUIDE_PVP]);
	// ���j���[�F�K�C�h�c�����L���O���[�h
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_menu_guide_ranking.png",
		&m_apTexture[TEX_TYPE_MENU_GUIDE_RANKING]);
	// ���j���[�F�K�C�h�c�T�E���h�e�X�g
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_menu_guide_soundtest.png",
		&m_apTexture[TEX_TYPE_MENU_GUIDE_SOUNDTEST]);
	// ���j���[�F�w�i
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/menu_bg.jpg",
		&m_apTexture[TEX_TYPE_MENU_BG]);

	// �`���[�g���A���F�I��
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_tutorial_end.png",
		&m_apTexture[TEX_TYPE_TUTORIAL_END]);
	// �`���[�g���A���F�X�L�b�v
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_tutorial_skip.png",
		&m_apTexture[TEX_TYPE_TUTORIAL_SKIP]);
	// �`���[�g���A���F�{�^��
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_tutorial_button.png",
		&m_apTexture[TEX_TYPE_TUTORIAL_BUTTON]);

	// �\�����j���[�F�g�b�v
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_solomenu_top.png",
		&m_apTexture[TEX_TYPE_SOLO_MENU_TOP]);
	// �\�����j���[�FCPU�Ƃ�������
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_solomenu_cpu.png",
		&m_apTexture[TEX_TYPE_SOLO_MENU_CPU]);
	// �\�����j���[�F�X�R�A�A�^�b�N
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_solomenu_score.png",
		&m_apTexture[TEX_TYPE_SOLO_MENU_SCORE]);
	// �\�����j���[�GCPU�̂悳(�����o��)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_difficulty00.png",
		&m_apTexture[TEX_TYPE_SOLO_MENU_DIFFICULTY_00]);
	// �\�����j���[�GCPU�̂悳(����)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_difficulty01.png",
		&m_apTexture[TEX_TYPE_SOLO_MENU_DIFFICULTY_01]);

	// �Q�[���F�J�E���g�_�E��
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_game_countdown.png",
		&m_apTexture[TEX_TYPE_GAME_COUNTDOWN]);
	// �Q�[���F�X�R�A����
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_score_sentence.png",
		&m_apTexture[TEX_TYPE_GAME_SCORE_SENTENCE]);
	// �Q�[���F�n�C�X�R�A����
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_highscore_sentence.png",
		&m_apTexture[TEX_TYPE_GAME_HIGHSCORE_SENTENCE]);
	// �Q�[���F1P
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_1p.png",
		&m_apTexture[TEX_TYPE_GAME_1P]);
	// �Q�[���F2P
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_2p.png",
		&m_apTexture[TEX_TYPE_GAME_2P]);
	// �Q�[���FCPU
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_cpu.png",
		&m_apTexture[TEX_TYPE_GAME_CPU]);
	// �Q�[���F�W�����v�n�_�܂ł��Ɖ�m��
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_jump_guide00.png",
		&m_apTexture[TEX_TYPE_GAME_JUMP_GUIDE00]);
	// �Q�[���F���n�܂ł��Ɖ�m��
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_jump_guide01.png",
		&m_apTexture[TEX_TYPE_GAME_JUMP_GUIDE01]);

	// ���U���g�F�g�b�v
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_result_top.png",
		&m_apTexture[TEX_TYPE_RESULT_TOP]);
	// ���U���g�F����
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_result_sentence.png",
		&m_apTexture[TEX_TYPE_RESULT_SENTENCE]);
	// ���U���g�F��
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_result_band.png",
		&m_apTexture[TEX_TYPE_RESULT_BAND]);
	// ���U���g�F�w�i
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/result_bg.jpg",
		&m_apTexture[TEX_TYPE_RESULT_BG]);
	// ���U���g�F���g���C
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_result_retry.png",
		&m_apTexture[TEX_TYPE_RESULT_RETRY]);
	// ���U���g�F�����I
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_result_win.png",
		&m_apTexture[TEX_TYPE_RESULT_WIN]);
	// ���U���g�F�Ђ��킯�c
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_result_draw.png",
		&m_apTexture[TEX_TYPE_RESULT_DRAW]);

	// �����L���O�F�g�b�v
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_ranking_top.png",
		&m_apTexture[TEX_TYPE_RANKING_TOP]);
	// �����L���O�F1�ʂ̘g
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_ranking_one.png",
		&m_apTexture[TEX_TYPE_RANKING_ONE]);
	// �����L���O�F2�ʂ̘g
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_ranking_two.png",
		&m_apTexture[TEX_TYPE_RANKING_TWO]);
	// �����L���O�F3�ʂ̘g
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_ranking_three.png",
		&m_apTexture[TEX_TYPE_RANKING_THREE]);
	// �����L���O�F4�ʂ̘g
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_ranking_four.png",
		&m_apTexture[TEX_TYPE_RANKING_FOUR]);
	// �����L���O�F5�ʂ̘g
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_ranking_five.png",
		&m_apTexture[TEX_TYPE_RANKING_FIVE]);

	// ���փ{�^��
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_next.png",
		&m_apTexture[TEX_TYPE_NEXT]);
	// ���ǂ�{�^��
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_back.png",
		&m_apTexture[TEX_TYPE_BACK]);

	// �|�[�Y�G�g�b�v
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_pause_top.png",
		&m_apTexture[TEX_TYPE_PAUSE_TOP]);
	// �|�[�Y�G�Â���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_pause_continue.png",
		&m_apTexture[TEX_TYPE_PAUSE_CONTINUE]);
	// �|�[�Y�G���Ȃ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_pause_retry.png",
		&m_apTexture[TEX_TYPE_PAUSE_RETRY]);
	// �|�[�Y�G��߂�
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_pause_quit.png",
		&m_apTexture[TEX_TYPE_PAUSE_QUIT]);

	// �T�E���h�e�X�g�F�g�b�v
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_soundtest_top.png",
		&m_apTexture[TEX_TYPE_SOUNDTEST_TOP]);
	// �T�E���h�e�X�g�F���C��
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_soundtest_main.png",
		&m_apTexture[TEX_TYPE_SOUNDTEST_MAIN]);
	// �T�E���h�e�X�g�F�Đ�
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui_soundtest_play.png",
		&m_apTexture[TEX_TYPE_SOUNDTEST_PLAY]);

	//+------------------------+
	//|  �`���[�g���A���̐���  |
	//+------------------------+
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui_tutorial_guide_00.png", &m_apGuideTex[GUIDE_TEX_TYPE_TUTORIAL_GUIDE00]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui_tutorial_guide_01.png", &m_apGuideTex[GUIDE_TEX_TYPE_TUTORIAL_GUIDE01]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui_tutorial_guide_02.png", &m_apGuideTex[GUIDE_TEX_TYPE_TUTORIAL_GUIDE02]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui_tutorial_guide_03.png", &m_apGuideTex[GUIDE_TEX_TYPE_TUTORIAL_GUIDE03]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui_tutorial_guide_04.png", &m_apGuideTex[GUIDE_TEX_TYPE_TUTORIAL_GUIDE04]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui_tutorial_guide_05.png", &m_apGuideTex[GUIDE_TEX_TYPE_TUTORIAL_GUIDE05]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui_tutorial_guide_06.png", &m_apGuideTex[GUIDE_TEX_TYPE_TUTORIAL_GUIDE06]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui_tutorial_guide_07.png", &m_apGuideTex[GUIDE_TEX_TYPE_TUTORIAL_GUIDE07]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui_tutorial_guide_08.png", &m_apGuideTex[GUIDE_TEX_TYPE_TUTORIAL_GUIDE08]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui_tutorial_guide_09.png", &m_apGuideTex[GUIDE_TEX_TYPE_TUTORIAL_GUIDE09]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui_tutorial_guide_10.png", &m_apGuideTex[GUIDE_TEX_TYPE_TUTORIAL_GUIDE10]);

	// �e�N�X�`���̃p�X�▼�O���Ԉ���Ă���ƁA�I�������ŃG���[����������̂Œ���!!
}

//=============================================================================
// �I������
//=============================================================================
void CTexture::Uninit(void)
{
	// �e�N�X�`���̑����Ԃ�J��Ԃ�
	for (int nCntTexture = 0; nCntTexture < TEX_TYPE_MAX; nCntTexture++)
	{
		// �e�N�X�`���̔j��
		if (m_apTexture[nCntTexture] != NULL)
		{
			m_apTexture[nCntTexture]->Release();
			m_apTexture[nCntTexture] = NULL;
		}
	}

	for (int nCntTexture = 0; nCntTexture < GUIDE_TEX_TYPE_MAX; nCntTexture++)
	{
		// �e�N�X�`���̔j��
		if (m_apGuideTex[nCntTexture] != NULL)
		{
			m_apGuideTex[nCntTexture]->Release();
			m_apGuideTex[nCntTexture] = NULL;
		}
	}
}

//=============================================================================
// �e�N�X�`���̎擾����
//=============================================================================
LPDIRECT3DTEXTURE9 CTexture::GetTexture(TEX_TYPE type)
{
	return m_apTexture[type];
}

//=============================================================================
// �e�N�X�`���̎擾����
//=============================================================================
LPDIRECT3DTEXTURE9 CTexture::GetGuideTex(GUIDE_TEX_TYPE type)
{
	return m_apGuideTex[type];
}