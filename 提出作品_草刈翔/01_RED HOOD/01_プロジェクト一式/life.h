//=============================================================================
// ���C�t���� [life.h]
// Author : ���� ��
//=============================================================================
#ifndef _LIFE_H_
#define _LIFE_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "ui.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_LIFE_POLYGON	(3)	// �|���S���̐�
#define MAX_LIFE_DIGIT		(3)	// �ő包��

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CLife : public CUI
{
public:
	// ���C�t�̎��
	typedef enum
	{
		LIFE_TYPE_PLAYER = 0,	// �v���C���[
		LIFE_TYPE_BOSS,			// �{�X
		LIFE_TYPE_MAX
	} LIFE_TYPE;

	CLife();																	// �R���X�g���N�^
	~CLife();																	// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR2 pos, D3DXVECTOR2 size, LIFE_TYPE type);			// ����������
	void Uninit(void);															// �I������
	void Update(void);															// �X�V����
	void Draw(void);															// �`�揈��
	static CLife *Create(D3DXVECTOR2 pos, D3DXVECTOR2 size, LIFE_TYPE type);	// ��������
	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_UI; }						// �I�u�W�F�N�g�̎�ނ̎擾����
	void SetLife(float fLife) { m_fLife = fLife; }								// ���C�t�ݒ菈��
	void SetMaxLife(float fLife) { m_fMaxLife = fLife; }						// �ő僉�C�t�ݒ菈��

private:
	void SetLifeBar(void);			// ���C�t�o�[�̐ݒ菈��

	float m_fLife;					// ���C�t
	float m_fMaxLife;				// �ő僉�C�t
	float m_fObjectiveSize;			// �ړI�̃T�C�Y
	float m_fMaxSize;				// �ő�̃T�C�Y
	D3DXVECTOR2 m_pos;				// �ʒu
	D3DXVECTOR2 m_size;				// �T�C�Y
	D3DXVECTOR2 m_emptyPos;			// �󕔕��̈ʒu
	D3DXVECTOR2 m_emptySize;		// �󕔕��̃T�C�Y
	CUI *m_apUI[MAX_LIFE_POLYGON];	// UI�̃|�C���^
};
#endif	//_LIFE_H_