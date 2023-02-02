//=============================================================================
// �r���{�[�h�̃��C�t���� [life_billboard.h]
// Author : ���� ��
//=============================================================================
#ifndef _LIFE_BILLBOARD_H_
#define _LIFE_BILLBOARD_H_

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "billboard.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CBillboard;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_LIFE_BILLBOARD	(3)	// �r���{�[�h�̐�

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CLifeBillboard : public CBillboard
{
public:
	CLifeBillboard();															// �R���X�g���N�^
	~CLifeBillboard();															// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);							// ����������
	void Uninit(void);															// �I������
	void Update(void);															// �X�V����
	void Draw(void);															// �`�揈��
	static CLifeBillboard *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY);	// ��������
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }								// �ʒu�ݒ菈��
	void SetSize(D3DXVECTOR3 size) { m_size = size; }							// �T�C�Y�ݒ菈��
	void SetLife(float fValue) { m_fLife = fValue; }							// ���C�t�ݒ菈��
	void DecLife(int nValue) {m_fLife -= nValue; }								// ���C�t��������
	void SetMaxLife(float nValue) { m_fMaxLife = nValue; }						// �ő僉�C�t�ݒ菈��

private:
	void SetLifeBar(void);														// ���C�t�o�[�̐ݒ菈��

	float m_fLife;																// ���C�t
	float m_fMaxLife;															// �ő僉�C�t
	float m_fObjectiveSize;														// �ړI�̃T�C�Y
	float m_fMaxSize;															// �ő�̃T�C�Y
	D3DXVECTOR3 m_pos;															// �ʒu
	D3DXVECTOR3	m_size;															// �T�C�Y
	D3DXVECTOR3 m_emptySize;													// �󕔕��̃T�C�Y
	CBillboard *m_apBillboard[MAX_LIFE_BILLBOARD];								// �r���{�[�h�̃|�C���^
};
#endif	//_LIFE_BILLBOARD_H_