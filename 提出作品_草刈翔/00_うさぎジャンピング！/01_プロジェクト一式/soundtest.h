//=============================================================================
// �T�E���h�e�X�g��� [soundtest.h]
// Author : ���� ��
//=============================================================================
#ifndef _SOUNDTEST_H_
#define _SOUNDTEST_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SOUNDTEST_MAX_UI			(4)			// UI�̍ő吔
#define SOUNDTEST_UI_MAIN_SIZE_X	(950.0f)	// ���C��UI�̃T�C�YX
#define SOUNDTEST_UI_MAIN_SIZE_Y	(400.0f)	// ���C��UI�̃T�C�YY
#define SOUNDTEST_UI_NUMBER_POS_X	(530.0f)	// �T�E���h�i���o�[�̈ʒu���WX
#define SOUNDTEST_UI_NUMBER_POS_Y	(440.0f)	// �T�E���h�i���o�[�̈ʒu���WY
#define SOUNDTEST_UI_NUMBER_SIZE_X	(150.0f)	// �T�E���h�i���o�[�����̃T�C�YX
#define SOUNDTEST_UI_NUMBER_SIZE_Y	(200.0f)	// �T�E���h�i���o�[�����̃T�C�YY

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScene2D;
class CUi;
class CNumberSound;

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CSoundTest : public CScene
{
public:
	CSoundTest();										// �R���X�g���N�^
	~CSoundTest();										// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// ����������
	void Uninit(void);									// �I������
	void Update(void);									// �X�V����
	void Draw(void);									// �`�揈��

private:
	static int m_nSelect;								// �ǂ��I��ł��邩
	static float m_fScroll;								// �w�i�X�N���[���p�̕ϐ�
	CScene2D *m_pScene2D;								// 2D�I�u�W�F�N�g�̃|�C���^
	CUi *m_apUi[SOUNDTEST_MAX_UI];						// UI�̃|�C���^
	CNumberSound *m_pSoundNumber;						// �T�E���h�i���o�[�̃|�C���^
};

#endif	// _SOUNDTEST_H_