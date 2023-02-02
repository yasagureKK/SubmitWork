//=============================================================================
// 1�l�p�Q�[�����j���[��� [solo_menu.h]
// Author : ���� ��
//=============================================================================
#ifndef _SOLO_MENU_H_
#define _SOLO_MENU_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SOLO_MENU_MAX_UI		(8)			// UI�̍ő吔
#define SOLO_MENU_SELECT_POS_Y	(190.0f)	// ���j���[�I�����̈ʒuY
#define SOLO_MENU_SELECT_SIZE_X	(650.0f)	// ���j���[�I�����̃T�C�YX
#define SOLO_MENU_SELECT_SIZE_Y	(100.0f)	// ���j���[�I�����̃T�C�YY
#define SOLO_MENU_DIFFICULTY_00_POS_X	(1000.0f)	// ���j���[�I�����̈ʒuX
#define SOLO_MENU_DIFFICULTY_00_POS_Y	(260.0f)	// ���j���[�I�����̈ʒuY
#define SOLO_MENU_DIFFICULTY_00_SIZE_X	(440.0f)	// ���j���[�I�����̃T�C�YX
#define SOLO_MENU_DIFFICULTY_00_SIZE_Y	(200.0f)	// ���j���[�I�����̃T�C�YY

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScene2D;
class CUi;

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CSoloMenu : public CScene
{
public:
	CSoloMenu();										// �R���X�g���N�^
	~CSoloMenu();										// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// ����������
	void Uninit(void);									// �I������
	void Update(void);									// �X�V����
	void Draw(void);									// �`�揈��
	static int GetDifficulty(void);						// ��Փx�̎擾����

private:
	static float m_fScroll;								// �w�i�X�N���[���p�̃J�E���g
	static int m_nSelect;								// �ǂ̃��[�h��I��ł��邩
	static int m_nDifficulty;							// �ǂ̓�Փx��I��ł��邩
	CScene2D *m_pScene2D;								// 2D�I�u�W�F�N�g�̃|�C���^
	CUi *m_apUi[SOLO_MENU_MAX_UI];						// UI�̃|�C���^
};

#endif	// _SOLO_MENU_H_