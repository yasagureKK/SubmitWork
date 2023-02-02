//=============================================================================
// �����L���O��� [ranking.h]
// Author : ���� ��
//=============================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene.h"
#include "score.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define RANKING_FADE_COUNT (1800)		// �����J�ڂ܂ł̃J�E���g
#define RANKING_MAX_DATA (5)			// �����L���O�f�[�^�̍ő吔
#define RANKING_MAX_UI	(7)				// UI�̍ő吔
#define RANKING_TOP_POS_X (250.0f)		// �����L���O�g�b�v��X���W�ʒu
#define RANKING_TOP_POS_Y (70.0f)		// �����L���O�g�b�v��Y���W�ʒu
#define RANKING_TOP_SIZE_X (750.0f)		// �����L���O�g�b�v�̃T�C�YX
#define RANKING_TOP_SIZE_Y (120.0f)		// �����L���O�g�b�v�̃T�C�YY
#define RANKING_FRAME_POS_Y (180.0f)	// ���ʖ��̘g�̊�ʒuY
#define RANKING_FRAME_SIZE_X (900.0f)	// ���ʖ��̘g�̃T�C�YX
#define RANKING_FRAME_SIZE_Y (100.0f)	// ���ʖ��̘g�̃T�C�YY
#define RANKING_DATA_POS_X (520.0f)		// �����L���O�̐�����X���W�ʒu

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScene2D;
class CUi;

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CRanking : public CScene
{
public:
	CRanking();											// �R���X�g���N�^
	~CRanking();										// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// ����������
	void Uninit(void);									// �I������
	void Update(void);									// �X�V����
	void Draw(void);									// �`�揈��

private:
	static float m_fScroll;								// �w�i�X�N���[���p�̕ϐ�
	static CScore *m_apScore[MAX_SCORE_DATA];			// �X�R�A�̃|�C���^
	static int m_nRankingData[RANKING_MAX_DATA];		// �����L���O�̃X�R�A
	int m_nFadeCount;									// ��ʑJ�ڗp�̃J�E���g
	CScene2D *m_pScene2D;								// 2D�I�u�W�F�N�g�̃|�C���^
	CUi *m_apUi[RANKING_MAX_UI];						// UI�̃|�C���^
};

#endif	// _RANKING_H_