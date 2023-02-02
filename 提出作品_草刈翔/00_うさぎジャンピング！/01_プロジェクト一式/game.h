//=============================================================================
// �Q�[����� [game.h]
// Author : ���� ��
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define GAME_MAX_MESH_FIELD	(7)						// ���b�V���t�B�[���h�̍ő吔
#define GAME_MAX_MESH_CYLINDER	(3)					// ���b�V���V�����_�[�̍ő吔
#define GAME_MAX_MESH_WALL	(6)						// ���b�V���E�H�[���̍ő吔
#define GAME_MAX_UI	(3)								// UI�̍ő吔
#define GAME_PLAYER_START (-5000.0f)				// �v���C���[�̃X�^�[�g�ʒu���WX
#define GAME_UI_SCORE_POS_X (60.0f)					// �X�R�A�̈ʒu���WX
#define GAME_UI_SCORE_POS_Y (130.0f)				// �X�R�A�̈ʒu���WY
#define GAME_UI_SCORE_SENTENCE_POS_X (150.0f)		// �X�R�A�����̈ʒu���WX
#define GAME_UI_SCORE_SENTENCE_POS_Y (50.0f)		// �X�R�A�����̈ʒu���WY
#define GAME_UI_SCORE_SENTENCE_SIZE_X (250.0f)		// �X�R�A�����̃T�C�YX
#define GAME_UI_SCORE_SENTENCE_SIZE_Y (90.0f)		// �X�R�A�����̃T�C�YY
#define GAME_UI_HIGHSCORE_POS_X (1000.0f)			// �n�C�X�R�A�̈ʒu���WX
#define GAME_UI_HIGHSCORE_POS_Y (120.0f)			// �n�C�X�R�A�̈ʒu���WY
#define GAME_UI_HIGHSCORE_SENTENCE_POS_X (1050.0f)	// �n�C�X�R�A�����̈ʒu���WX
#define GAME_UI_HIGHSCORE_SENTENCE_POS_Y (50.0f)	// �n�C�X�R�A�����̈ʒu���WY
#define GAME_UI_HIGHSCORE_SENTENCE_SIZE_X (450.0f)	// �n�C�X�R�A�����̃T�C�YX
#define GAME_UI_HIGHSCORE_SENTENCE_SIZE_Y (80.0f)	// �n�C�X�R�A�����̃T�C�YY
#define GAME_UI_JUMP_GUIDE_POS_X (570.0f)			// �W�����v�K�C�h�̈ʒu���WX
#define GAME_UI_JUMP_GUIDE_POS_Y (110.0f)			// �W�����v�K�C�h�̈ʒu���WY
#define GAME_UI_COUNTDOWN_SIZE_X (300.0f)			// �J�E���g�_�E���̃T�C�YX
#define GAME_UI_COUNTDOWN_SIZE_Y (150.0f)			// �J�E���g�_�E���̃T�C�YY

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CUi;
class CPlayer;
class CMeshField;
class CMeshWall;
class CMeshSphere;
class CMeshCylinder;
class CScore;
class CJumpGuide;
class CPause;
class CStageData;
class CGameSignboard;
class CBillboardMoon;

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CGame : public CScene
{
public:
	// �Q�[���̃t�F�C�Y
	typedef enum
	{
		GAME_PHASE_NONE = 0,	// �Ȃ�
		GAME_PHASE_COUNTDOWN,	// �X�^�[�g�O�̃J�E���g�_�E��
		GAME_PHASE_ACTION,		// �A�N�V������
		GAME_PHASE_ACTION_END,	// �A�N�V�����I��
		GAME_PHASE_MAX
	} GAME_PHASE;

	CGame();												// �R���X�g���N�^
	~CGame();												// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);		// ����������
	void Uninit(void);										// �I������
	void Update(void);										// �X�V����
	void Draw(void);										// �`�揈��
	void SetPhase(GAME_PHASE phase);						// �t�F�C�Y�ݒ菈��
	GAME_PHASE GetPhase(void);								// �t�F�C�Y�擾����

private:
	void Phase();											// �t�F�C�Y����

	CUi *m_apUi[GAME_MAX_UI];								// UI�̃|�C���^
	CPlayer *m_pPlayer;										// �v���C���[�̃|�C���^
	CMeshField *m_pMeshField[GAME_MAX_MESH_FIELD];			// ���b�V���t�B�[���h�̃|�C���^
	CMeshWall *m_pMeshWall[GAME_MAX_MESH_WALL];				// ���b�V���E�H�[���̃|�C���^
	CMeshSphere *m_pMeshSphere;								// ���b�V���X�t�B�A�̃|�C���^
	CMeshCylinder *m_pMeshCylinder[GAME_MAX_MESH_CYLINDER];	// ���b�V���V�����_�[�̃|�C���^
	CScore *m_pScore[2];									// �X�R�A�̃|�C���^
	CJumpGuide *m_pJumpGuide;								// �W�����v�K�C�h�̃|�C���^
	CPause *m_pPause;										// �|�[�Y�̃|�C���^
	CStageData *m_pStageData[2];							// �X�e�[�W�f�[�^�̃|�C���^
	CGameSignboard *m_pGameSignboard[2];					// �ŔI�u�W�F�N�g�̃|�C���^
	CBillboardMoon *m_pBillboardMoon;						// ���̃s���{�[�h�̃|�C���^
	GAME_PHASE m_phase;										// ���݂̃t�F�C�Y
	int m_nInterval;										// �t�F�C�Y�؂�ւ����̃C���^�[�o��
};

#endif	// _GAME_H_