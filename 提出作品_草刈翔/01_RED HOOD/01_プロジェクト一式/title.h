//=============================================================================
// �^�C�g����� [title.h]
// Author : ���� ��
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TITLE_MAX_SELECT (4)		// UI�̑I�����̍ő吔
#define TITLE_MAX_UI (3)			// UI�̍ő吔

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CUI;
class CRanking;

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CTitle : public CScene
{
public:
	// �^�C�g���̃t�F�[�Y
	typedef enum
	{
		TITLE_PHASE_PRESS_START = 0,	// �X�^�[�g�҂�
		TITLE_PHASE_MODE_SELECT,		// ���ڑI��
		TITLE_PHASE_HOWTO,				// �V�ѕ�
		TITLE_PHASE_GUIDE,				// ����K�C�h
		TITLE_PHASE_RANKING,			// �����L���O
		TITLE_PHASE_MAX,
	} TITLE_PHASE;

	CTitle();																// �R���X�g���N�^
	~CTitle();																// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos);											// ����������
	void Uninit(void);														// �I������
	void Update(void);														// �X�V����
	void Draw(void);														// �`�揈��
	static CTitle *Create(void);											// ��������
	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_NONE; }				// �I�u�W�F�N�g�̎�ނ̎擾����
	void SetPos(D3DXVECTOR3 pos) { pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	// �ʒu�ݒ菈��
	D3DXVECTOR3 GetPos(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// �ʒu�擾����
	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// �T�C�Y�擾����
	D3DXVECTOR3 GetPosOld(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	// 1�t���[���O�̈ʒu�擾����
	D3DXVECTOR3 GetRot(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// �����擾����

private:
	void UiCreate(TITLE_PHASE phase, bool bPad);	// UI��������

	CUI *m_apUI[TITLE_MAX_UI];						// UI�̃|�C���^
	CUI *m_apSelectUI[TITLE_MAX_SELECT];			// �I������UI�̃|�C���^
	CRanking *m_pRanking;							// �����L���O�̃|�C���^
	static int m_nSelect;							// �I����
	int m_nHowtoTex;								// �V�ѕ��̃e�N�X�`���ԍ�
	TITLE_PHASE m_Phase;							// ���݂̃t�F�[�Y
};

#endif	// _TITLE_H_