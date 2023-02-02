//=============================================================================
// �^�C������ [time.h]
// Author : ���� ��
//=============================================================================
#ifndef _TIME_H_
#define _TIME_H_

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
#define MAX_TIME		(999)	// ���Ԃ̍ő吔
#define MAX_TIME_DIGIT	(3)		// �ő包��

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CTime : public CUI
{
public:
	CTime();																	// �R���X�g���N�^
	~CTime();																	// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR2 pos, D3DXVECTOR2 size);							// ����������
	void Uninit(void);															// �I������
	void Update(void);															// �X�V����
	void Draw(void);															// �`�揈��
	static CTime *Create(D3DXVECTOR2 pos, D3DXVECTOR2 size);					// ��������
	int GetTime(void) { return m_nTime; }										// �^�C���擾����
	void SetTimeStop(bool bStop) { m_bTimeStop = bStop; }						// �^�C���X�g�b�v�ݒ菈��
	bool GetTimeStop(void) { return m_bTimeStop; }								// �^�C���X�g�b�v�擾����
	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_UI; }						// �I�u�W�F�N�g�̎�ނ̎擾����
	void SetPos(D3DXVECTOR2 pos) { m_pos = pos; }								// �ʒu�ݒ菈��
	D3DXVECTOR3 GetPos(void) { return D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f); }	// �ʒu�擾����
	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(m_size.x, m_size.y, 0.0f); }	// �T�C�Y�擾����

private:
	int m_nTime;																// �^�C��
	int m_nNumber;																// �^�C���v�Z�p�̕ϐ�
	bool m_bTimeStop;															// �^�C���i�s���~�߂邩�ǂ���
	D3DXVECTOR2 m_pos;															// �ʒu
	D3DXVECTOR2	m_size;															// �T�C�Y
	CUI *m_apUI;																// UI�̃|�C���^
	CUI *m_apNumber[MAX_TIME_DIGIT];											// ����������UI�̃|�C���^
};
#endif	//_TIME_H_