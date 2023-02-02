//=============================================================================
// �Ŕɕ\��������X�R�A���� [score_board.h]
// Authore : ���� ��
//=============================================================================
#ifndef _SCORE_BOARD_H_
#define _SCORE_BOARD_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene3D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_SCORE_BOARD_DIGIT	(4)	// �Ŕɕ\��������X�R�A�̍ő包��

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CNumber3D;

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CScoreBoard : public CScene3D
{
public:
	CScoreBoard(int nPriority = PRIORITY_MODEL);	// �R���X�g���N�^
	~CScoreBoard();									// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size,
		D3DCOLOR col);								// ����������
	void Uninit(void);								// �I������
	void Update(void);								// �X�V����
	void Draw(void);								// �`�揈��
	static CScoreBoard *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 size, D3DCOLOR col);			// ��������
	void SetPos(D3DXVECTOR3 pos);					// �ʒu�ݒ菈��

private:
	CNumber3D *m_apNumber3D[MAX_SCORE_BOARD_DIGIT];	// 3D�I�u�W�F�N�g�̃i���o�[�̃|�C���^
	int m_nScore;									// �X�R�A
	D3DXVECTOR3 m_pos;								// �ʒu
	D3DXVECTOR3 m_size;								// �T�C�Y
	D3DXVECTOR3 m_rot;								// ����
};

#endif	//_SCORE_BOARD_H_