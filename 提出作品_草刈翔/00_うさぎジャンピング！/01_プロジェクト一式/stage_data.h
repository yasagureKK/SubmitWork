//=============================================================================
// �X�e�[�W�f�[�^ [stage_data.h]
// Author : ���� ��
//=============================================================================
#ifndef _STAGRE_DATA_
#define _STAGRE_DATA_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_STAGE_OBJECT	(64)						// �I�u�W�F�N�g�̍ő吔
#define MAX_PATTERN			(4)							// �z�u�p�^�[���̍ő吔
#define STAGE_DATA_FILE "data/TEXT/stage_data.txt"		// �ǂݍ��ރt�@�C���̃p�X

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CModelSingle;
class CBillboard;

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CStageData : public CScene
{
public:
	// ���f�����̍\����
	typedef struct
	{
		int nType;						// ���f���̎��
		D3DXVECTOR3 pos;				// �ʒu
		D3DXVECTOR3 rot;				// ����
		CModelSingle *pModelSingle;		// ���f��(�P��)�̃|�C���^
	} STAGE_MODEL;

	// �r���{�[�h���̍\����
	typedef struct
	{
		int nTexType;					// �e�N�X�`���̎��
		D3DXVECTOR3 pos;				// �ʒu
		D3DXVECTOR3 size;				// �T�C�Y
		CBillboard *pBillboard;			// �r���{�[�h�̃|�C���^
	} STAGE_BILLBOARD;

	CStageData();										// �R���X�g���N�^
	~CStageData();										// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// ����������
	void Uninit(void);									// �I������
	void Update(void);									// �X�V����
	void Draw(void);									// �`�揈��
	static CStageData *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 size, int nNumPlayer);				// ��������
	void LoadStage(int nPattern);						// �X�e�[�W���[�h����
	void DeleteObject(void);							// �I�u�W�F�N�g�̑S�̏�������

private:
	D3DXVECTOR3 m_pos;									// �ʒu
	D3DXVECTOR3 m_size;									// �T�C�Y
	STAGE_MODEL aStageModel[MAX_STAGE_OBJECT];			// ���f��
	STAGE_BILLBOARD aStageBillboard[MAX_STAGE_OBJECT];	// �r���{�[�h
	static int m_nModelNum;								// ���f���̐�
	static int m_nBillboardNum;							// �r���{�[�h�̐�
	int m_nNumPlayer;									// �ǂ̃v���C���[�ɑ΂�����̂�
	int m_nPattern;										// �ǂ̔z�u�p�^�[����
};

#endif	// _GAME_OBJECT_H_