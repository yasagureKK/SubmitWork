//=============================================================================
// �P�̂̃��f������ [model_single.h]
// Authore : ���� ��
//=============================================================================
#ifndef _MODEL_SINGLE_H_
#define _MODEL_SINGLE_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene.h"
#include "model_load.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CModel;
class CScoreBoard;
class CShadow;

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CModelSingle : public CScene
{
public:
	// ���f���̓����̎��
	typedef enum
	{
		MODEL_SINGLE_TYPE_NONE = 0,
		MODEL_SINGLE_TYPE_SIGNBOARD,	// �Ŕ�
		MODEL_SINGLE_TYPE_MAX
	} MODEL_SINGLE_TYPE;

	CModelSingle(int nPriority = PRIORITY_MODEL);		// �R���X�g���N�^
	~CModelSingle();									// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// ����������
	void Uninit(void);									// �I������
	void Update(void);									// �X�V����
	void Draw(void);									// �`�揈��
	static CModelSingle *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 rot, CModelLoad::MODEL_TYPE type,
		MODEL_SINGLE_TYPE modelType);					// ��������
	static bool Collision(CScene *pScene);				// �����蔻��
	void SetShadow(bool bOnShadow, float fHeight);		// �e�̐ݒ菈��

private:
	D3DXVECTOR3 m_pos;									// �ʒu
	D3DXVECTOR3 m_posOld;								// 1�t���[���O�̈ʒu
	D3DXVECTOR3	m_size;									// �T�C�Y
	D3DXVECTOR3 m_rot;									// ����
	D3DXVECTOR3 m_move;									// �ړ���
	CModel *m_pModel;									// ���f���̃|�C���^
	D3DXMATRIX m_mtxWorld;								// ���[���h�}�g���b�N�X
	CModelLoad::MODEL_TYPE m_type;						// ���f���̎��
	MODEL_SINGLE_TYPE m_modelType;						// ���f���̓����̎��
	CScoreBoard *m_pScoreBoard;							// �X�R�A�{�[�h�̃|�C���^
	CShadow *m_pShadow;									// �V���h�E�̃|�C���^
};

#endif // _MODEL_SINGLE_H_