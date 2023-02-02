//=============================================================================
// �N���A���� [clear.h]
// Author : ���� ��
//=============================================================================
#ifndef _CLEAR_H_
#define _CLEAR_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "ui.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_CLEAR_UI		(7)	// UI�̍ő吔
#define MAX_CLEAR_SCORE_UI	(4)	// �X�R�AUI�̎�ސ�
#define MAX_CLEAR_DIGIT		(5)	// �ő包��

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CUI;

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CClear : public CUI
{
public:
	// UI�\���̒i�K
	typedef enum
	{
		CLEAR_FASE_NONE = 0,	// �Ȃ�
		CLEAR_FASE_TOP,			// �g�b�v����
		CLEAR_FASE_SCORE,		// �X�R�A
		CLEAR_FASE_TIME,		// �^�C���{�[�i�X
		CLEAR_FASE_LEVEL,		// ���x���{�[�i�X
		CLEAR_FASE_TOTAL,		// �ŏI�X�R�A
		CLEAR_FASE_RANK,		// �����N
		CLEAR_FASE_FADE,		// ��ʑJ��
		CLEAR_FASE_MAX
	} CLEAR_FASE;

	CClear();											// �R���X�g���N�^
	~CClear();											// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// ����������
	void Uninit(void);									// �I������
	void Update(void);									// �X�V����
	void Draw(void);									// �`�揈��
	static CClear *Create(void);						// ��������

private:
	CUI *m_apUI[MAX_CLEAR_UI];								// UI�̃|�C���^
	CUI *m_apNumber[MAX_CLEAR_DIGIT][MAX_CLEAR_SCORE_UI];	// ����������UI�̃|�C���^
	int m_nAlpha;											// �A���t�@�x
	int m_nClearCount;										// UI�\���̃J�E���g
	int m_nTimeBonus;										// �^�C���{�[�i�X
	int m_nLevelBonus;										// ���x���{�[�i�X
	int m_nScore;											// �X�R�A
	bool m_bDisplay;										// �摜��\�����邩�ǂ���
	CLEAR_FASE m_phase;										// UI�\���̒i�K
};
#endif	//_CLEAR_H_