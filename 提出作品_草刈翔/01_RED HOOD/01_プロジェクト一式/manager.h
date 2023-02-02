//=============================================================================
// �}�l�[�W���[ [manager.h]
// Author : ���� ��
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CRenderer;
class CTitle;
class CGame;
class CKeyboard;
class CGamePad;
class CMouse;
class CTexture;
class CModelLoad;
class CCamera;
class CFade;
class CSound;
class CDataAttack;
class CDataClear;

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CManager
{
public:
	// ���[�h�̎��
	typedef enum
	{
		MODE_TITLE = 0,	// �^�C�g��
		MODE_GAME,		// �Q�[��
		MODE_MAX,
	} MODE;

	CManager();														// �R���X�g���N�^
	~CManager();													// �f�X�g���N�^
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);		// ����������
	void Uninit(void);												// �I������
	void Update(void);												// �X�V����
	void Draw(void);												// �`�揈��

	static void SetMode(MODE mode);									// ���[�h�ݒ菈��
	static MODE GetMode(void) { return m_mode; }					// ���[�h�擾����
	static CTitle *GetTitle(void) { return m_pTitle; }				// �^�C�g���擾����
	static CGame *GetGame(void) { return m_pGame; }					// �Q�[���擾����
	static CRenderer *GetRenderer(void) { return m_pRenderer; }		// �����_���[�擾����
	static CCamera *GetCamera(void) { return m_pCamera; }			// �J�����擾����
	static CKeyboard *GetKeyboard(void) { return m_pKeyboard; }		// �L�[�{�[�h�擾����
	static CGamePad *GetGamePad(void) { return m_pGamePad; }		// �Q�[���p�b�h�擾����
	static CMouse *GetMouse(void) { return m_pMouse; }				// �}�E�X�擾����
	static CTexture *GetTexture(void) { return m_pTexture; }		// �e�N�X�`���擾����
	static CModelLoad *GetModelLoad(void) { return m_pModelLoad; }	// ���f�����[�h�擾����
	static CFade *GetFade(void) { return m_pFade; }					// �t�F�[�h�擾����
	static CSound *GetSound(void) { return m_pSound; }				// �T�E���h�擾����
	static CDataAttack *GetDataAtk(void) { return m_pDataAtk; }		// �U�����̎擾����
	static CDataClear *GetDataClear(void) { return m_pDataClear; }	// �N���A���̎擾����

	static void SetPause(bool bPause) { m_bPause = bPause; }		// �|�[�Y�ݒ菈��
	static bool GetPause() { return m_bPause; };					// �|�[�Y�擾����

private:
	static void GameSetUp();										// �Q�[����ʂ̃Z�b�g�A�b�v����

	static MODE m_mode;												// ���݂̃��[�h
	static CTitle *m_pTitle;										// �^�C�g���̃|�C���^
	static CGame *m_pGame;											// �Q�[���̃|�C���^
	static CRenderer *m_pRenderer;									// �����_���[�N���X�̃|�C���^
	static CCamera *m_pCamera;										// �J�����̃|�C���^
	static CKeyboard *m_pKeyboard;									// �L�[�{�[�h�N���X�̃|�C���^
	static CGamePad *m_pGamePad;									// �Q�[���p�b�h�N���X�̃|�C���^
	static CMouse *m_pMouse;										// �}�E�X�̃|�C���^
	static CTexture *m_pTexture;									// �e�N�X�`���̃|�C���^
	static CModelLoad *m_pModelLoad;								// ���f�����[�h�̃|�C���^
	static CFade *m_pFade;											// �t�F�[�h�̃|�C���^
	static CSound *m_pSound;										// �T�E���h�̃|�C���^
	static CDataAttack *m_pDataAtk;									// �U�����̃|�C���^
	static CDataClear *m_pDataClear;								// �N���A���̃|�C���^
	static bool m_bPause;											// �|�[�Y���Ă��邩�ǂ���
};
#endif // _MANAGER_H_