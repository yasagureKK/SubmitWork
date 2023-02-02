//=============================================================================
// ���f���̃��[�h���� [model_load.h]
// Authore: ���� ��
//=============================================================================
#ifndef _MODEL_LOAD_H_
#define _MODEL_LOAD_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CModelLoad
{
public:
	// ���f���̎��
	typedef enum
	{
										//*********���v���C���[1��********//
		MODEL_TYPE_RABBIT_BODY = 0,		// �E�T�M(��)
		MODEL_TYPE_RABBIT_HEAD,			// �E�T�M(��)
		MODEL_TYPE_RABBIT_EAR_RIGHT_0,	// �E�T�M(�E��0)
		MODEL_TYPE_RABBIT_EAR_RIGHT_1,	// �E�T�M(�E��1)
		MODEL_TYPE_RABBIT_EAR_LEFT_0,	// �E�T�M(����0)
		MODEL_TYPE_RABBIT_EAR_LEFT_1,	// �E�T�M(����0)
		MODEL_TYPE_RABBIT_HAND_RIGHT,	// �E�T�M(�E��)
		MODEL_TYPE_RABBIT_HAND_LEFT,	// �E�T�M(����)
		MODEL_TYPE_RABBIT_LEG_RIGHT,	// �E�T�M(�E��)
		MODEL_TYPE_RABBIT_FOOT_RIGHT,	// �E�T�M(�E��)
		MODEL_TYPE_RABBIT_LEG_LEFT,		// �E�T�M(����)
		MODEL_TYPE_RABBIT_FOOT_LEFT,	// �E�T�M(����)
										//*********���v���C���[2��********//
		MODEL_TYPE_RABBIT_2_BODY,		// �E�T�M2(��)
		MODEL_TYPE_RABBIT_2_HEAD,		// �E�T�M2(��)
		MODEL_TYPE_RABBIT_2_EAR_RIGHT_0,// �E�T�M2(�E��0)
		MODEL_TYPE_RABBIT_2_EAR_RIGHT_1,// �E�T�M2(�E��1)
		MODEL_TYPE_RABBIT_2_EAR_LEFT_0,	// �E�T�M2(����0)
		MODEL_TYPE_RABBIT_2_EAR_LEFT_1,	// �E�T�M2(����0)
		MODEL_TYPE_RABBIT_2_HAND_RIGHT,	// �E�T�M2(�E��)
		MODEL_TYPE_RABBIT_2_HAND_LEFT,	// �E�T�M2(����)
		MODEL_TYPE_RABBIT_2_LEG_RIGHT,	// �E�T�M2(�E��)
		MODEL_TYPE_RABBIT_2_FOOT_RIGHT,	// �E�T�M2(�E��)
		MODEL_TYPE_RABBIT_2_LEG_LEFT,	// �E�T�M2(����)
		MODEL_TYPE_RABBIT_2_FOOT_LEFT,	// �E�T�M2(����)
										//*********���I�u�W�F�N�g��*********//
		MODEL_TYPE_SIGNBOARD,			// �Ŕ�
		MODEL_TYPE_CARROT,				// �j���W��
		MODEL_TYPE_ROCK,				// ��
		MODEL_TYPE_MAX
	} MODEL_TYPE;

	CModelLoad();								// �R���X�g���N�^
	~CModelLoad();								// �f�X�g���N�^
	void Init(void);							// ����������
	void Uninit(void);							// �I������
	LPD3DXMESH GetMesh(MODEL_TYPE type);		// ���b�V����������
	LPD3DXBUFFER GetBuffMat(MODEL_TYPE type);	// �}�e���A����������
	DWORD GetNumMat(MODEL_TYPE type);			// �}�e���A�����̊�������

private:
	LPD3DXMESH m_apMesh[MODEL_TYPE_MAX];		// ���b�V��(���_���)�ւ̃|�C���^
	LPD3DXBUFFER m_apBuffMat[MODEL_TYPE_MAX];	// �}�e���A��(�ގ����)�ւ̃|�C���^
	DWORD m_aNumMat[MODEL_TYPE_MAX];			// �}�e���A����
};

#endif // _MODEL_LOAD_H_