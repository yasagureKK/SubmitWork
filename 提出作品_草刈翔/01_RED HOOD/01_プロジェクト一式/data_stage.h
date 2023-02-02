//=============================================================================
// �X�e�[�W�f�[�^ [data_stage.h]
// Author : ���� ��
//=============================================================================
#ifndef _DATA_STAGE_
#define _DATA_STAGE_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CMeshSky;
class CMeshField;
class CMeshWall;
class CModelSingle;
class CEnemy;

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CDataStage : public CScene
{
public:
	// �X�e�[�W�̎��
	typedef enum
	{
		STAGE_TYPE_GAME = 0,		// �Q�[��
		STAGE_TYPE_DEBUG,			// �f�o�b�O
		STAGE_TYPE_MAX
	} STAGE_TYPE;

	CDataStage();															// �R���X�g���N�^
	~CDataStage();															// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos);											// ����������
	void Uninit(void);														// �I������
	void Update(void);														// �X�V����
	void Draw(void);														// �`�揈��
	static CDataStage *Create(D3DXVECTOR3 pos, STAGE_TYPE type);			// ��������
	void LoadStage(int nNum);												// �X�e�[�W���[�h����
	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_NONE; }				// �I�u�W�F�N�g�̎�ނ̎擾����
	void SetPos(D3DXVECTOR3 pos) { pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	// �ʒu�ݒ菈��
	D3DXVECTOR3 GetPos(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); };		// �ʒu�擾����
	D3DXVECTOR3 GetPosOld(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	// 1�t���[���O�̈ʒu�擾����
	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// �T�C�Y�擾����
	D3DXVECTOR3 GetRot(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// �����擾����

private:
	D3DXVECTOR3 m_pos;														// �ʒu
	D3DXVECTOR3 m_rot;														// ����
	D3DXVECTOR3 m_size;														// �T�C�Y
	int m_nType;															// ���
	int m_nRow;																// �s��
	int m_nLine;															// ��
	int m_nMaxStage;														// �X�e�[�W�̍ő吔
	CMeshSky *m_pMeshSky;													// ���b�V���X�J�C�̃|�C���^
	STAGE_TYPE m_type;														// �X�e�[�W�̎��
};
#endif	// _DATA_STAGE_