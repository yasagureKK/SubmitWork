//=============================================================================
// �G�l�~�[�{�b�N�X���� [enemy_box.h]
// Author : ���� ��
//=============================================================================
#ifndef _ENEMY_BOX_H_
#define _ENEMY_BOX_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene3D.h"

//*****************************************************************************
//�}�N����`
//*****************************************************************************
#define MAX_ENEBOX_ENEMY	(6)	// �G�̍ő吔
#define MAX_ENEBOX_WALL		(8)	// �ǃ|���S���̍ő吔

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CEnemy;
class CMeshWall;

//*****************************************************************************
//�N���X�̒�`
//*****************************************************************************
class CEnemyBox : public CScene3D
{
public:
	// ���
	typedef enum
	{
		ENEMY_BOX_TYPE_0 = 0,	// �����[�g�ɔz�u����
		ENEMY_BOX_TYPE_1,		// �E���[�g�ɔz�u����
		ENEMY_BOX_TYPE_2,		// �����n�_�Ŕz�u����
		ENEMY_BOX_TYPE_BOSS,	// �ŉ����ɔz�u����
		ENEMY_BOX_TYPE_MAX
	} ENEMY_BOX_TYPE;

	CEnemyBox(int nPriority = PRIORITY_NONE);								// �R���X�g���N�^
	~CEnemyBox();															// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);						// ����������
	void Uninit(void);														// �I������
	void Update(void);														// �X�V����
	void Draw(void);														// �`�揈��
	static CEnemyBox *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size,
		int nType);															// ��������
	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_NONE; }				// �I�u�W�F�N�g�̎�ނ̎擾����

private:
	void InsidePlayer(void);												// �v���C���[�����ɓ��������𔻕ʂ��鏈��
	void SetObject(void);													// �G�E�ǂ̔z�u����

	D3DXVECTOR3 m_pos;														// �ʒu
	D3DXVECTOR3	m_size;														// �T�C�Y
	D3DXVECTOR3 m_rot;														// ����
	D3DXMATRIX m_mtxWorld;													// ���[���h�}�g���b�N�X
	ENEMY_BOX_TYPE m_type;													// ���
	int m_nEnemyNum;														// �G�̐�
	bool m_bInside;															// �v���C���[�����ɓ�������
	bool m_bDefeat;															// �G���S�ł�����
	CEnemy *m_pEnemy[MAX_ENEBOX_ENEMY];										// �G�l�~�[�̃|�C���^
	CMeshWall *m_pMeshWall[MAX_ENEBOX_WALL];								// ���b�V���E�H�[���̃|�C���^
};
#endif // _ENEMY_BOX_H_