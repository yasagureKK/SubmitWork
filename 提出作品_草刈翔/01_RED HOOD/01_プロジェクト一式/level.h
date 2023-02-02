//=============================================================================
// ���x������ [level.h]
// Author : ���� ��
//=============================================================================
#ifndef _LEVEL_H_
#define _LEVEL_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "ui.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CNumber;

//*****************************************************************************
//�}�N����`
//*****************************************************************************
#define MAX_LEVEL_UI		(4)		// UI�̍ő吔
#define MAX_LEVEL			(10)	// �ő僌�x��
#define MAX_LEVEL_DIGIT		(2)		// ���x���̍ő包��
#define MAX_LEVEL_ICON_UI	(3)		// UI�̍ő吔

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CLevel : public CUI
{
public:
	CLevel();																	// �R���X�g���N�^
	~CLevel();																	// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR2 pos, D3DXVECTOR2 size);							// ����������
	void Uninit(void);															// �I������
	void Update(void);															// �X�V����
	void Draw(void);															// �`�揈��
	static CLevel *Create(D3DXVECTOR2 pos, D3DXVECTOR2 size);					// ��������
	void SetExp(int nValue) { m_nExp = nValue; }								// �o���l�ݒ菈��
	void AddExp(int nValue) { m_nExp += nValue; }								// �o���l��������
	int GetLevel(void) { return m_nLevel; }										// ���x���擾����
	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_UI; }						// �I�u�W�F�N�g�̎�ނ̎擾����
	void SetPos(D3DXVECTOR2 pos) { m_pos = pos; }								// �ʒu�ݒ菈��
	D3DXVECTOR3 GetPos(void) { return D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f); }	// �ʒu�擾����
	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(m_size.x, m_size.y, 0.0f); }	// �T�C�Y�擾����
	bool GetMoment(void) { return m_bMoment; }

private:
	void SetEmptyBar();															// �󕔕��̐ݒ菈��

	int m_nLevel;																// ���x��
	int m_nExp;																	// �o���l
	int m_nNextLevelExp;														// ���̃��x���܂ł̍��v�o���l
	int m_nNeedExp[MAX_LEVEL];													// ���x�����Ƃ̕K�v�o���l
	float m_fObjectiveSize;														// �ړI�̃T�C�Y
	float m_fMaxSize;															// �ő�̃T�C�Y
	bool m_bMoment;																// ���x�����オ�����u��
	bool m_bDrawNumber;
	D3DXVECTOR2 m_pos;															// �ʒu
	D3DXVECTOR2	m_size;															// �T�C�Y
	D3DXVECTOR2 m_emptyPos;														// �󕔕��̈ʒu
	D3DXVECTOR2 m_emptySize;													// �󕔕��̃T�C�Y
	D3DXVECTOR2 m_levelPos;														// �����̈ʒu
	CUI *m_apUI[MAX_LEVEL_UI];													// UI�̃|�C���^
	CUI *m_apNumber[MAX_LEVEL_DIGIT];											// ����UI�̃|�C���^
	CUI *m_apIconUI[MAX_LEVEL_ICON_UI];											// �A�C�R���̃|�C���^
	CUI *m_apIconNumber[MAX_LEVEL_ICON_UI];										// �A�C�R���p�̐���UI�̃|�C���^
};
#endif	//_LEVEL_H_