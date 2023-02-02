//=============================================================================
// �`���[�g���A����� [tutorial.h]
// Author : ���� ��
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TUTORIAL_MAX_MODEL	(100)					// ���f���̍ő吔
#define TUTORIAL_PLAYER_START (-5000.0f)			// �v���C���[�̃X�^�[�g�ʒu���WX
#define TUTORIAL_MAX_MESH_FIELD	(7)					// ���b�V���t�B�[���h�̍ő吔
#define TUTORIAL_MAX_MESH_CYLINDER	(3)				// ���b�V���V�����_�[�̍ő吔
#define TUTORIAL_MAX_MESH_WALL	(6)					// ���b�V���E�H�[���̍ő吔
#define TUTORIAL_MAX_UI	(4)							// UI�̍ő吔
#define TUTORIAL_UI_SKIP_SENTENCE_POS_X (1180.0f)	// �X�L�b�v�����̈ʒu���WX
#define TUTORIAL_UI_SKIP_SENTENCE_POS_Y (35.0f)		// �X�L�b�v�����̈ʒu���WY
#define TUTORIAL_UI_SKIP_SENTENCE_SIZE_X (270.0f)	// �X�L�b�v�����̃T�C�YX
#define TUTORIAL_UI_SKIP_SENTENCE_SIZE_Y (60.0f)	// �X�L�b�v�����̃T�C�YY

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
class CTutorialGuide;
class CScene2D;
class CTutorialPause;
class CStageData;
class CGameSignboard;
class CBillboardMoon;

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CTutorial : public CScene
{
public:
	// �`���[�g���A���̃t�F�C�Y
	typedef enum
	{
		TUTORIAL_PHASE_NONE = 0,	// �Ȃ�
		TUTORIAL_PHASE_COUNTDOWN,	// �X�^�[�g�O�̃J�E���g�_�E��
		TUTORIAL_PHASE_ACTION,		// �A�N�V������
		TUTORIAL_PHASE_ACTION_END,	// �A�N�V�����I��
		TUTORIAL_PHASE_MAX
	} TUTORIAL_PHASE;

	CTutorial();												// �R���X�g���N�^
	~CTutorial();												// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);			// ����������
	void Uninit(void);											// �I������
	void Update(void);											// �X�V����
	void Draw(void);											// �`�揈��
	void SetPhase(TUTORIAL_PHASE phase);						// �t�F�C�Y�ݒ菈��
	TUTORIAL_PHASE GetPhase(void);								// �t�F�C�Y�擾����
	static bool GetExplanation(void);							// ���������ǂ����擾����

private:
	void TimeLine(void);										// �^�C�����C���Ǘ�
	void Phase(void);											// �t�F�C�Y����

	CUi *m_apUi[TUTORIAL_MAX_UI];								// UI�̃|�C���^
	CPlayer *m_pPlayer;											// �v���C���[�̃|�C���^
	CMeshField *m_pMeshField[TUTORIAL_MAX_MESH_FIELD];			// ���b�V���t�B�[���h�̃|�C���^
	CMeshWall *m_pMeshWall[TUTORIAL_MAX_MESH_WALL];				// ���b�V���E�H�[���̃|�C���^
	CMeshSphere *m_pMeshSphere;									// ���b�V���X�t�B�A�̃|�C���^
	CMeshCylinder *m_pMeshCylinder[TUTORIAL_MAX_MESH_CYLINDER];	// ���b�V���V�����_�[�̃|�C���^
	CScore *m_pScore[2];										// �X�R�A�̃|�C���^
	CJumpGuide *m_pJumpGuide;									// �W�����v�K�C�h�̃|�C���^
	CTutorialGuide *m_pTutorialGuide;							// �`���[�g���A���̃K�C�h�̃|�C���^
	CScene2D *m_pScene2D[2];									// 2D�I�u�W�F�N�g�̃|�C���^
	CTutorialPause *m_pTutorialPause;							// �`���[�g���A���ł̃|�[�Y�̃|�C���^
	CStageData *m_pStageData[2];								// �X�e�[�W�f�[�^�̃|�C���^
	CGameSignboard *m_pGameSignboard[2];						// �ŔI�u�W�F�N�g�̃|�C���^
	CBillboardMoon *m_pBillboardMoon;							// ���̃s���{�[�h�̃|�C���^
	TUTORIAL_PHASE m_phase;										// ���݂̃t�F�C�Y
	int m_nInterval;											// �t�F�C�Y�؂�ւ����̃C���^�[�o��
	int m_nTime;												// �^�C��
	int m_nPlayerCommandCount;									// �v���C���[�Ɏw�����o�����߂Ɏg���J�E���g
	static bool m_bStop;										// �X�g�b�v���Ă邩�ǂ���
	static bool m_bExplanation;									// ��������n�߂邩�ǂ���
};

#endif	// _TUTORIAL_H_