//=============================================================================
// �X�^�[�gUI���� [start.h]
// Author : ���� ��
//=============================================================================
#ifndef _START_H_
#define _START_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "ui.h"

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CUIStart : public CUI
{
public:
	// �X�^�[�g�̃t�F�[�Y
	typedef enum
	{
		START_PHASE_READY = 0,	// Ready
		START_PHASE_GO,			// Go
		START_PHASE_MAX,
	} START_PHASE;

	CUIStart();																	// �R���X�g���N�^
	~CUIStart();																// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR2 pos, D3DXVECTOR2 size);							// ����������
	void Uninit(void);															// �I������
	void Update(void);															// �X�V����
	void Draw(void);															// �`�揈��
	static CUIStart *Create(D3DXVECTOR2 pos, D3DXVECTOR2 size);					// ��������
	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_UI; }						// �I�u�W�F�N�g�̎�ނ̎擾����
	void SetPos(D3DXVECTOR2 pos) { m_pos = pos; }								// �ʒu�ݒ菈��
	D3DXVECTOR3 GetPos(void) { return D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f); }	// �ʒu�擾����
	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(m_size.x, m_size.y, 0.0f); }	// �T�C�Y�擾����
	bool GetStartMoment(void) { return m_bStartMoment; }

private:
	D3DXVECTOR2 m_pos;				// �ʒu
	D3DXVECTOR2	m_size;				// �T�C�Y
	D3DXVECTOR2	m_expansionSize;	// �g�債���摜�T�C�Y
	CUI *m_apUI;					// UI�̃|�C���^
	bool m_bStartMoment;			// �X�^�[�g�����u��
	int m_nWaitTime;				// �ҋ@����
	float m_fAlpha;					// �A���t�@�x
	float m_fExpansion;				// �g�嗦
	START_PHASE m_phase;			// �t�F�[�Y
};
#endif	//_START_H_