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
#define MAX_LIGHT	(4)		// ���C�g�̍ő吔
#define MAX_ENEMYBOX (4)	// �G�l�~�[�{�b�N�X�̍ő吔

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CLight;
class CPlayer;
class CDataStage;
class CCollisionSphere;
class CEnemyBox;
class CPause;
class CFog;
class CScore;
class CTime;
class CUIStart;

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CGame : public CScene
{
public:
	CGame();																// �R���X�g���N�^
	~CGame();																// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos);											// ����������
	void Uninit(void);														// �I������
	void Update(void);														// �X�V����
	void Draw(void);														// �`�揈��
	static CGame *Create(void);												// ��������
	CPlayer *GetPlayer(void);												// �v���C���[�擾����
	static void SetDefeatBoss(bool bDefeat) { m_bDefeatBoss = bDefeat; }	// �{�X��|�������ݒ菈��
	static bool GetDefeatBoss(void) { return m_bDefeatBoss; }				// �{�X��|�������擾����
	void SetBossBattle(bool bDefeat);										// �{�X�킩�ǂ����ݒ菈��
	bool GetBossBattle(void) { return m_bBossBattle; }						// �{�X�킩�ǂ����擾����
	static bool GetClear(void) { return m_bClear; }							// �N���A�������擾����

	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_NONE; }				// �I�u�W�F�N�g�̎�ނ̎擾����
	void SetPos(D3DXVECTOR3 pos) { pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	// �ʒu�ݒ菈��
	D3DXVECTOR3 GetPos(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); };		// �ʒu�擾����
	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// �T�C�Y�擾����
	D3DXVECTOR3 GetPosOld(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	// 1�t���[���O�̈ʒu�擾����
	D3DXVECTOR3 GetRot(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// �����擾����

private:
	CPlayer *m_pPlayer;						// �v���C���[�̃|�C���^
	CDataStage *m_pDataStage;				// �X�e�[�W�f�[�^�̃|�C���^
	CEnemyBox *m_pEnemyBox[MAX_ENEMYBOX];	// �G�l�~�[�{�b�N�X�̃|�C���^
	CPause *m_pPause;						// �|�[�Y�̃|�C���^
	CFog *m_pFog;							// �t�H�O�̃|�C���^
	CScore *m_pScore;						// �X�R�A�̃|�C���^
	CTime *m_pTime;							// �^�C���̃|�C���^
	CLight *m_apLight[MAX_LIGHT];			// ���C�g�̃|�C���^
	CUIStart *m_pStart;						// �X�^�[�gUI�̃|�C���^
	static bool m_bBossBattle;				// �{�X�킩�ǂ���
	static bool m_bDefeatBoss;				// �{�X��|������
	static bool m_bClear;					// �N���A������
	int nCntGame;							// �X�^�[�g����{�X���j��Ɏg���J�E���g
};

#endif	// _GAME_H_