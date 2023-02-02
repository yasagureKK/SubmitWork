//=============================================================================
// ���x���A�b�v���� [level_up.h]
// Author : ���� ��
//=============================================================================
#ifndef _LEVEL_UP_H_
#define _LEVEL_UP_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define LEVELUP_MAX_UI		(6)	// UI�̍ő吔
#define LEVELUP_MAX_SELECT	(3)	// �I�����̍ő吔

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CUI;

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CLevelUp : public CScene
{
public:
	// ���x���A�b�v�̃t�F�[�Y
	typedef enum
	{
		LEVELUP_PHASE_BG = 0,	// �w�i
		LEVELUP_PHASE_SELECT,	// �I��
		LEVELUP_PHASE_FINISH,	// �I��
		LEVELUP_PHASE_MAX,
	} LEVELUP_PHASE;

	CLevelUp(int nPriority = PRIORITY_PAUSE);								// �R���X�g���N�^
	~CLevelUp();															// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos);											// ����������
	void Uninit(void);														// �I������
	void Update(void);														// �X�V����
	void Draw(void);														// �`�揈��
	static CLevelUp *Create(D3DXVECTOR3 pos);								// ��������

	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_NONE; }				// �I�u�W�F�N�g�̎�ނ̎擾����
	void SetPos(D3DXVECTOR3 pos) { pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	// �ʒu�ݒ菈��
	D3DXVECTOR3 GetPos(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// �ʒu�擾����
	D3DXVECTOR3 GetPosOld(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	// 1�t���[���O�̈ʒu�擾����
	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// �T�C�Y�擾����
	D3DXVECTOR3 GetRot(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// �����擾����
	static void SetCorrectionDef(int nDef) { m_nCorrectionDef = nDef; }		// �h��␳�l�̐ݒ菈��
	static void SetCorrectionAtk(int nAtk) { m_nCorrectionAtk = nAtk; }		// �U���␳�l�̐ݒ菈��
	static void SetCorrectionSpd(int nSpeed) { m_nCorrectionSpd = nSpeed; }	// ���x�␳�l�̐ݒ菈��
	static int GetCorrectionDef(void) { return m_nCorrectionDef; }			// �h��␳�l�̎擾����
	static int GetCorrectionAtk(void) { return m_nCorrectionAtk; }			// �U���␳�l�̎擾����
	static int GetCorrectionSpd(void) { return m_nCorrectionSpd; }			// ���x�␳�l�̎擾����

private:
	CUI *m_apUI[LEVELUP_MAX_UI];			// UI�̃|�C���^
	CUI *m_apSelectUI[LEVELUP_MAX_SELECT];	// �I������UI�̃|�C���^
	int m_nAlpha;							// �A���t�@�x
	int m_nSelect;							// �ǂ̍��ڂ�I��ł��邩
	int m_nFlash;							// �_�ŗp�̃J�E���g
	static int m_nCorrectionDef;			// �h��̕␳�l
	static int m_nCorrectionAtk;			// �U���̕␳�l
	static int m_nCorrectionSpd;			// ���x�̕␳�l
	LEVELUP_PHASE m_Phase;					// ���݂̃t�F�[�Y
};
#endif	// _LEVEL_UP_H_