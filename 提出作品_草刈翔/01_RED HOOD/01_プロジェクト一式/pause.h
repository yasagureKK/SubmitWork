//=============================================================================
// �|�[�Y���� [pause.h]
// Author : ���� ��
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PAUSE_MAX_UI		(5)	// UI�̍ő吔
#define PAUSE_MAX_SELECT_UI	(3)	// �I������UI�̍ő吔

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CUI;

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CPause : public CScene
{
public:
	// �|�[�Y�̃t�F�[�Y
	typedef enum
	{
		PAUSE_PHASE_NORMAL = 0,	// �ʏ�
		PAUSE_PHASE_GUIDE,		// ����K�C�h
		PAUSE_PHASE_MAX,
	} PAUSE_PHASE;

	CPause(int nPriority = PRIORITY_PAUSE);									// �R���X�g���N�^
	~CPause();																// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos);											// ����������
	void Uninit(void);														// �I������
	void Update(void);														// �X�V����
	void Draw(void);														// �`�揈��
	static CPause *Create(D3DXVECTOR3 pos);									// ��������
	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_NONE; }				// �I�u�W�F�N�g�̎�ނ̎擾����
	void SetPos(D3DXVECTOR3 pos) { pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	// �ʒu�ݒ菈��
	D3DXVECTOR3 GetPos(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// �ʒu�擾����
	D3DXVECTOR3 GetPosOld(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	// 1�t���[���O�̈ʒu�擾����
	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// �T�C�Y�擾����
	D3DXVECTOR3 GetRot(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// �����擾����

	static void SetPossiblePause(bool bPause);								// �|�[�Y�ł��邩�ݒ菈��

private:
	void UICreate(PAUSE_PHASE phase, float fTex);	// UI��������
	void CancelPause(void);							// �|�[�Y�������̏���

	CUI *m_apUi[PAUSE_MAX_UI];						// UI�̃|�C���^
	CUI *m_apSelectUi[PAUSE_MAX_SELECT_UI];			// �I������UI�̃|�C���^
	static bool m_bCanPause;						// �|�[�Y�ł��邩�ǂ���
	bool m_bPause;									// �|�[�Y���Ă��邩�ǂ���
	static int m_nSelect;							// �ǂ̑I������I��ł��邩
	PAUSE_PHASE m_Phase;							// ���݂̃t�F�[�Y
};
#endif	// _PAUSE_H_