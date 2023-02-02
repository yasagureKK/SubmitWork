//=============================================================================
// �X�R�A���� [score.h]
// Author : ���� ��
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

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
#define MAX_SCORE_DIGIT	(5)		// �ő包��

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CScore : public CUI
{
public:
	CScore();																	// �R���X�g���N�^
	~CScore();																	// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR2 pos, D3DXVECTOR2 size);							// ����������
	void Uninit(void);															// �I������
	void Update(void);															// �X�V����
	void Draw(void);															// �`�揈��
	static CScore *Create(D3DXVECTOR2 pos, D3DXVECTOR2 size);					// ��������
	int GetTime(void) { return m_nScore; }										// �^�C���擾����
	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_UI; }						// �I�u�W�F�N�g�̎�ނ̎擾����
	void SetPos(D3DXVECTOR2 pos) { m_pos = pos; }								// �ʒu�ݒ菈��
	D3DXVECTOR3 GetPos(void) { return D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f); }	// �ʒu�擾����
	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(m_size.x, m_size.y, 0.0f); }	// �T�C�Y�擾����

private:
	int m_nScore;																// �^�C��
	D3DXVECTOR2 m_pos;															// �ʒu
	D3DXVECTOR2	m_size;															// �T�C�Y
	CUI *m_apUI;																// UI�̃|�C���^
	CUI *m_apNumber[MAX_SCORE_DIGIT];											// ����������UI�̃|�C���^
};
#endif	//_SCORE_H_