//=============================================================================
// �K�C�h���� [jump_guide.h]
// Authore : ���� ��
//=============================================================================
#ifndef _JUMP_GUIDE_H_
#define _JUMP_GUIDE_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene2D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_SCORE_JUMP_GUIDE_DIGIT (3)	// �ő包��

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CNumber;
class CUi;

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CJumpGuide : public CScene
{
public:
	CJumpGuide(int nPriority = PRIORITY_UI);			// �R���X�g���N�^
	~CJumpGuide();										// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// ����������
	void Uninit(void);									// �I������
	void Update(void);									// �X�V����
	void Draw(void);									// �`�揈��
	static CJumpGuide *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 size, D3DXCOLOR col);				// ��������
	void SetPosValue(int nPos);							// �ʒu���W�̒l�ݒ菈��
	int GetPosValue(void);								// �ʒu���W�̒l�擾����
	void SetNum(int nNum);								// �X�R�A�ԍ��ݒ菈��
	int GetNum(void);									// �X�R�A�ԍ��擾����

private:
	CNumber *m_apNumber[MAX_SCORE_JUMP_GUIDE_DIGIT];	// 2D�I�u�W�F�N�g�̃i���o�[�̃|�C���^
	CUi *m_apUi[2];										// UI�̃|�C���^
	int m_nPos;											// ���݂̈ʒu���WX
	D3DXVECTOR3 m_pos;									// �ʒu
	D3DXVECTOR3 m_size;									// �T�C�Y
	D3DXCOLOR m_col;									// �J���[
	int m_nNum;											// �ԍ�
	bool m_bChangeTex;									// �摜��؂�ւ������ǂ���
};

#endif	//_JUMP_GUIDE_H_