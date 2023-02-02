//=============================================================================
// �X�R�A���� [score.h]
// Authore : ���� ��
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene2D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_SCORE_DIGIT	(6)					// �X�R�A�̍ő包��
#define MAX_SCORE_DATA	(5)					// �f�[�^�̍ő吔
#define SAVEFILENAME "data/TEXT/score.txt"	// �t�@�C���̃p�X

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CNumber;
class CUi;

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CScore : public CScene
{
public:
	CScore(int nPriority = PRIORITY_UI);				// �R���X�g���N�^
	~CScore();											// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// ����������
	void Uninit(void);									// �I������
	void Update(void);									// �X�V����
	void Draw(void);									// �`�揈��
	static CScore *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 size, D3DXCOLOR col);				// ��������
	void SetScore(int nScore);							// �X�R�A�ݒ菈��
	void AddScore(int nValue);							// �X�R�A�ǉ�����
	int GetScore(void);									// �X�R�A�擾����
	static void SaveScore(void);						// �Z�[�u����
	static void LoadScore(void);						// ���[�h����
	static void SetRankScore(int nNum);					// �����L���O�ݒ菈��
	void SetScorePos(D3DXVECTOR3 pos);					// �X�R�A�ʒu�ݒ菈��
	void SetNum(int nNum);								// �X�R�A�ԍ��ݒ菈��
	int GetNum(void);									// �X�R�A�ԍ��擾����
	void SetResultScore(int nNum);						// ���U���g�̃X�R�A�ݒ菈��
	int GetResultScore(int nNum);						// ���U���g�̃X�R�A�擾����
	void SetHighScore(int nHighScore);					// �n�C�X�R�A�ݒ菈��

private:
	CNumber *m_apNumber[MAX_SCORE_DIGIT];				// �i���o�[�̃|�C���^
	CUi *m_apUi[2];										// UI�̃|�C���^
	int m_nScore;										// �X�R�A
	static int m_nResultScore[2];						// ���U���g�X�R�A
	static int m_aScore[MAX_SCORE_DATA];				// �����L���O�ɕۑ�����X�R�A
	D3DXVECTOR3 m_pos;									// �ʒu
	D3DXVECTOR3 m_size;									// �T�C�Y
	D3DXCOLOR m_col;									// �J���[
	int m_nNum;											// �X�R�A�ԍ�
};

#endif	//_SCORE_H_