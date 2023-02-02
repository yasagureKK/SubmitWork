//=============================================================================
// 2�l�p�Q�[����� [pvp.h]
// Author : ���� ��
//=============================================================================
#ifndef _PVP_H_
#define _PVP_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PVP_MAX_MODEL (100)			// ���f���̍ő吔
#define PVP_MAX_MODEL_BOARD (10)	// �Ŕ��f���̍ő吔
#define PVP_MAX_MESH_FIELD	(7)		// ���b�V���t�B�[���h�̍ő吔
#define PVP_MAX_MESH_CYLINDER (3)	// ���b�V���V�����_�[�̍ő吔
#define PVP_MAX_MESH_WALL (6)		// ���b�V���E�H�[���̍ő吔
#define PVP_MAX_UI (6)				// UI�̍ő吔
#define PVP_UI_1P_POS_X (110.0f)	// 1P�\���̈ʒu���WX
#define PVP_UI_1P_SIZE_X (190.0f)	// 1P�\���̃T�C�YX
#define PVP_UI_1P_SIZE_Y (90.0f)	// 1P�\���̃T�C�YY
#define PVP_2P_START (20000.0f)		// 2P�̃X�^�[�gZ���W

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
class CScene2D;
class CStageData;
class CGameSignboard;
class CBillboardMoon;

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CPvP : public CScene
{
public:
	// 2�l�p�Q�[���̃t�F�C�Y
	typedef enum
	{
		PVP_PHASE_NONE = 0,		// �Ȃ�
		PVP_PHASE_COUNTDOWN,	// �X�^�[�g�O�̃J�E���g�_�E��
		PVP_PHASE_ACTION,		// �A�N�V������
		PVP_PHASE_ACTION_END,	// �A�N�V�����I��
		PVP_PHASE_MAX
	} PVP_PHASE;

	CPvP();												// �R���X�g���N�^
	~CPvP();											// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// ����������
	void Uninit(void);									// �I������
	void Update(void);									// �X�V����
	void Draw(void);									// �`�揈��
	void SetPhase(PVP_PHASE phase);						// �t�F�C�Y�ݒ菈��
	PVP_PHASE GetPhase(void);							// �t�F�C�Y�擾����

private:
	void Phase();										// �t�F�C�Y����

	CUi *m_apUi[PVP_MAX_UI];												// UI�̃|�C���^
	CPlayer *m_apPlayer[MAX_PLAYER_NUM];									// �v���C���[�̃|�C���^
	CMeshField *m_pMeshField[MAX_PLAYER_NUM][PVP_MAX_MESH_FIELD];			// ���b�V���t�B�[���h�̃|�C���^
	CMeshWall *m_pMeshWall[MAX_PLAYER_NUM][PVP_MAX_MESH_WALL];				// ���b�V���E�H�[���̃|�C���^
	CMeshSphere *m_pMeshSphere;												// ���b�V���X�t�B�A�̃|�C���^
	CMeshCylinder *m_pMeshCylinder[MAX_PLAYER_NUM][PVP_MAX_MESH_CYLINDER];	// ���b�V���V�����_�[�̃|�C���^
	CScore *m_pScore[3];													// �X�R�A�̃|�C���^
	CJumpGuide *m_pJumpGuide[MAX_PLAYER_NUM];								// �W�����v�K�C�h�̃|�C���^
	CPause *m_pPause;														// �|�[�Y�̃|�C���^
	CScene2D *m_pScene2D;													// 2D�I�u�W�F�N�g�̃|�C���^
	CStageData *m_pStageData[MAX_PLAYER_NUM][2];							// �X�e�[�W�f�[�^�̃|�C���^
	CGameSignboard *m_apGameSignboard[MAX_PLAYER_NUM][2];					// �ŔI�u�W�F�N�g�̃|�C���^
	CBillboardMoon *m_apBillboardMoon[MAX_PLAYER_NUM];						// ���̃s���{�[�h�̃|�C���^
	PVP_PHASE m_phase;														// ���݂̃t�F�C�Y
	int m_nInterval;														// �t�F�C�Y�؂�ւ����̃C���^�[�o��
};

#endif	// _PVP_H_