//=============================================================================
// �N���A���� [data_clear.h]
// Author : ���� ��
//=============================================================================
#ifndef _DATA_CLEAR_H_
#define _DATA_CLEAR_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_CLEAR_DATA	(3)		// �f�[�^�̍ő吔
#define CLEAR_RANK_S	(18000)	// S�����N�̊�l
#define CLEAR_RANK_A	(13000)	// A�����N�̊�l
#define CLEAR_RANK_B	(6000)	// B�����N�̊�l

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CDataClear
{
public:
	// �N���A���̍\����
	typedef struct
	{
		int nScore;		// �N���A�X�R�A
		char cRank[64];	// �N���A�����N
		int nTime;		// �N���A�^�C��
		int nLevel;		// �N���A���x��
	} CLEAR_DATA;

	CDataClear();										// �R���X�g���N�^
	~CDataClear();										// �f�X�g���N�^
	HRESULT Init(void);									// ����������
	void Uninit(void);									// �I������
	static CDataClear *Create(void);					// ��������
	void AddScore(int nValue) { m_nScore += nValue; }	// �X�R�A���Z����
	void SetScore(int nScore) { m_nScore = nScore; }	// �X�R�A�ݒ菈��
	int GetScore(void) { return m_nScore; }				// �X�R�A�擾����
	int GetTimeBonus(void) { return m_nTimeBonus; }		// �^�C���{�[�i�X�擾����
	int GetLevelBonus(void) { return m_nLevelBonus; }	// ���x���{�[�i�X�擾����
	void SetNewScore(int nTime, int nLevel);			// �V�����X�R�A�̐ݒ�
	void SetRanking(CLEAR_DATA data);					// �����L���O�̐ݒ�
	CLEAR_DATA GetRankingData(int nNum);				// �����L���O�̎擾

private:
	void Save(void);						// �Z�[�u����
	void Load(void);						// ���[�h����

	int m_nScore;							// �X�R�A
	int m_nTimeBonus;						// �^�C���{�[�i�X
	int m_nLevelBonus;						// ���x���{�[�i�X
	CLEAR_DATA m_data[MAX_CLEAR_DATA];		// �N���A�f�[�^
	CLEAR_DATA m_newData;					// �N���A�f�[�^
};
#endif	// _DATA_CLEAR_H_