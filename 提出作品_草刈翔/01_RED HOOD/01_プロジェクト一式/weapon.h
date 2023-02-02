//=============================================================================
// ����UI���� [weapon.h]
// Author : ���� ��
//=============================================================================
#ifndef _WEAPON_H_
#define _WEAPON_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "ui.h"

//*****************************************************************************
//�}�N����`
//*****************************************************************************
#define MAX_WEAPON_UI_POLYGON	(3)	// �ő�|���S����
#define MAX_WEAPON_ICON			(2)	// �A�C�R���̐�

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CWeapon : public CUI
{
public:
	CWeapon();																// �R���X�g���N�^
	~CWeapon();																// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR2 pos);												// ����������
	void Uninit(void);															// �I������
	void Update(void);															// �X�V����
	void Draw(void);															// �`�揈��
	static CWeapon *Create(D3DXVECTOR2 pos);									// ��������
	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_UI; }						// �I�u�W�F�N�g�̎�ނ̎擾����
	void SetPos(D3DXVECTOR2 pos) { m_pos = pos; }								// �ʒu�ݒ菈��
	D3DXVECTOR3 GetPos(void) { return D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f); }	// �ʒu�擾����
	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(m_size.x, m_size.y, 0.0f); }	// �T�C�Y�擾����
	void SetUIChange(int nType);												// UI�ύX����

private:
	D3DXVECTOR2 m_pos;															// �ʒu
	D3DXVECTOR2	m_size;															// �T�C�Y
	CUI *m_apUI[MAX_WEAPON_UI_POLYGON];											// UI�̃|�C���^
	float m_fTiltCircle;														// �~��UI�̌X��
	D3DXVECTOR2 m_posIcon[MAX_WEAPON_ICON];										// �A�C�R���̈ʒu
	float m_fTiltIcon[MAX_WEAPON_ICON];											// �A�C�R���ɐݒ肷��X��
	float m_fAddTilt;															// �A�C�R���ɒǉ�����X��
	float m_fObjectiveTilt;														// �A�C�R���̖ړI�̌X��
};
#endif	//_WEAPON_H_