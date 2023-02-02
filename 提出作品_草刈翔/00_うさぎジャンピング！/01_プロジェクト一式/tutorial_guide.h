//=============================================================================
// ���K���[�h�̃K�C�h���� [tutorial_guide.h]
// Author : ���� ��
//=============================================================================
#ifndef _TUTORIAL_GUIDE_H_
#define _TUTORIAL_GUIDE_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TUTORIAL_GUIDE_UI_FRAME_POS_Y (610.0f)		// ��b�t���[���̈ʒu���WY
#define TUTORIAL_GUIDE_UI_FRAME_SIZE_X (1230.0f)	// ��b�t���[���̃T�C�YX
#define TUTORIAL_GUIDE_UI_FRAME_SIZE_Y (200.0f)		// ��b�t���[���̃T�C�YY
#define TUTORIAL_GUIDE_UI_BUTTON_POS_X (1200.0f)	// �{�^���̈ʒu���WX
#define TUTORIAL_GUIDE_UI_BUTTON_POS_Y (660.0f)		// �{�^���̈ʒu���WY
#define TUTORIAL_GUIDE_UI_BUTTON_SIZE (80.0f)		// �{�^���̃T�C�Y

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CUi;
class CScene2D;

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CTutorialGuide : public CScene
{
public:
	CTutorialGuide(int nPriority = PRIORITY_PAUSE);						// �R���X�g���N�^
	~CTutorialGuide();													// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);					// ����������
	void Uninit(void);													// �I������
	void Update(void);													// �X�V����
	void Draw(void);													// �`�揈��
	static CTutorialGuide *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// ��������
	static void SetPage(int nNum);										// �y�[�W���ݒ菈��
	static int GetPage(void);											// �y�[�W���擾����
	void AdvancePage(void);												// �y�[�W��i�߂鏈��
	void AddPageAndDestroyWindow(void);									// �y�[�W��i�߁A���b�Z�[�W�E�B���h�E��j�����鏈��

private:
	void CreateMessageWindow(void);										// ���b�Z�[�W�E�B���h�E����鏈��
	CScene2D *m_pScene2D[2];											// 2D�I�u�W�F�N�g�̃|�C���^
	static int m_nNowPage;												// ���݂̃y�[�W
	bool m_bDisplay;													// �K�C�h���\�����Ă邩�ǂ���
	bool m_bDeleteDisplay;												// �K�C�h�̕\�����������ǂ���
};

#endif	// _TUTORIAL_GUIDE_H_