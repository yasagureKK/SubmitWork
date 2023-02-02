//=============================================================================
// ���f���̃��[�h���� [model_load.cpp]
// Authore: ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "model_load.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CModelLoad::CModelLoad()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CModelLoad::~CModelLoad()
{

}

//=============================================================================
// ����������
//=============================================================================
void CModelLoad::Init(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	//**********************//
	//*  ���f���̓ǂݍ���  *//
	//**********************//
	//+---------------+
	//|  �v���C���[1  |
	//+---------------+
	// �E�T�M(��)
	D3DXLoadMeshFromX("data/MODEL/rabbit/00_body.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_BODY], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_BODY], &m_apMesh[MODEL_TYPE_RABBIT_BODY]);
	// �E�T�M(��)
	D3DXLoadMeshFromX("data/MODEL/rabbit/01_head.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_HEAD], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_HEAD], &m_apMesh[MODEL_TYPE_RABBIT_HEAD]);
	// �E�T�M(�E��0)
	D3DXLoadMeshFromX("data/MODEL/rabbit/02_earR_0.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_EAR_RIGHT_0], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_EAR_RIGHT_0], &m_apMesh[MODEL_TYPE_RABBIT_EAR_RIGHT_0]);
	// �E�T�M(�E��1)
	D3DXLoadMeshFromX("data/MODEL/rabbit/03_earR_1.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_EAR_RIGHT_1], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_EAR_RIGHT_1], &m_apMesh[MODEL_TYPE_RABBIT_EAR_RIGHT_1]);
	// �E�T�M(����0)
	D3DXLoadMeshFromX("data/MODEL/rabbit/04_earL_0.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_EAR_LEFT_0], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_EAR_LEFT_0], &m_apMesh[MODEL_TYPE_RABBIT_EAR_LEFT_0]);
	// �E�T�M(����1)
	D3DXLoadMeshFromX("data/MODEL/rabbit/05_earL_1.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_EAR_LEFT_1], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_EAR_LEFT_1], &m_apMesh[MODEL_TYPE_RABBIT_EAR_LEFT_1]);
	// �E�T�M(�E��)
	D3DXLoadMeshFromX("data/MODEL/rabbit/06_handR.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_HAND_RIGHT], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_HAND_RIGHT], &m_apMesh[MODEL_TYPE_RABBIT_HAND_RIGHT]);
	// �E�T�M(����)
	D3DXLoadMeshFromX("data/MODEL/rabbit/07_handL.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_HAND_LEFT], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_HAND_LEFT], &m_apMesh[MODEL_TYPE_RABBIT_HAND_LEFT]);
	// �E�T�M(�E��)
	D3DXLoadMeshFromX("data/MODEL/rabbit/08_legR.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_LEG_RIGHT], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_LEG_RIGHT], &m_apMesh[MODEL_TYPE_RABBIT_LEG_RIGHT]);
	// �E�T�M(�E��)
	D3DXLoadMeshFromX("data/MODEL/rabbit/09_footR.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_FOOT_RIGHT], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_FOOT_RIGHT], &m_apMesh[MODEL_TYPE_RABBIT_FOOT_RIGHT]);
	// �E�T�M(����)
	D3DXLoadMeshFromX("data/MODEL/rabbit/10_legL.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_LEG_LEFT], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_LEG_LEFT], &m_apMesh[MODEL_TYPE_RABBIT_LEG_LEFT]);
	// �E�T�M(����)
	D3DXLoadMeshFromX("data/MODEL/rabbit/11_footL.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_FOOT_LEFT], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_FOOT_LEFT], &m_apMesh[MODEL_TYPE_RABBIT_FOOT_LEFT]);
	//+---------------+
	//|  �v���C���[2  |
	//+---------------+
	// �E�T�M2(��)
	D3DXLoadMeshFromX("data/MODEL/rabbit2/00_body.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_2_BODY], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_2_BODY], &m_apMesh[MODEL_TYPE_RABBIT_2_BODY]);
	// �E�T�M2(��)
	D3DXLoadMeshFromX("data/MODEL/rabbit2/01_head.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_2_HEAD], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_2_HEAD], &m_apMesh[MODEL_TYPE_RABBIT_2_HEAD]);
	// �E�T�M2(�E��0)
	D3DXLoadMeshFromX("data/MODEL/rabbit2/02_earR_0.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_2_EAR_RIGHT_0], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_2_EAR_RIGHT_0], &m_apMesh[MODEL_TYPE_RABBIT_2_EAR_RIGHT_0]);
	// �E�T�M2(�E��1)
	D3DXLoadMeshFromX("data/MODEL/rabbit2/03_earR_1.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_2_EAR_RIGHT_1], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_2_EAR_RIGHT_1], &m_apMesh[MODEL_TYPE_RABBIT_2_EAR_RIGHT_1]);
	// �E�T�M2(����0)
	D3DXLoadMeshFromX("data/MODEL/rabbit2/04_earL_0.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_2_EAR_LEFT_0], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_2_EAR_LEFT_0], &m_apMesh[MODEL_TYPE_RABBIT_2_EAR_LEFT_0]);
	// �E�T�M2(����1)
	D3DXLoadMeshFromX("data/MODEL/rabbit2/05_earL_1.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_2_EAR_LEFT_1], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_2_EAR_LEFT_1], &m_apMesh[MODEL_TYPE_RABBIT_2_EAR_LEFT_1]);
	// �E�T�M2(�E��)
	D3DXLoadMeshFromX("data/MODEL/rabbit2/06_handR.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_2_HAND_RIGHT], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_2_HAND_RIGHT], &m_apMesh[MODEL_TYPE_RABBIT_2_HAND_RIGHT]);
	// �E�T�M2(����)
	D3DXLoadMeshFromX("data/MODEL/rabbit2/07_handL.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_2_HAND_LEFT], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_2_HAND_LEFT], &m_apMesh[MODEL_TYPE_RABBIT_2_HAND_LEFT]);
	// �E�T�M2(�E��)
	D3DXLoadMeshFromX("data/MODEL/rabbit2/08_legR.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_2_LEG_RIGHT], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_2_LEG_RIGHT], &m_apMesh[MODEL_TYPE_RABBIT_2_LEG_RIGHT]);
	// �E�T�M2(�E��)
	D3DXLoadMeshFromX("data/MODEL/rabbit2/09_footR.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_2_FOOT_RIGHT], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_2_FOOT_RIGHT], &m_apMesh[MODEL_TYPE_RABBIT_2_FOOT_RIGHT]);
	// �E�T�M2(����)
	D3DXLoadMeshFromX("data/MODEL/rabbit2/10_legL.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_2_LEG_LEFT], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_2_LEG_LEFT], &m_apMesh[MODEL_TYPE_RABBIT_2_LEG_LEFT]);
	// �E�T�M2(����)
	D3DXLoadMeshFromX("data/MODEL/rabbit2/11_footL.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_RABBIT_2_FOOT_LEFT], NULL, &m_aNumMat[MODEL_TYPE_RABBIT_2_FOOT_LEFT], &m_apMesh[MODEL_TYPE_RABBIT_2_FOOT_LEFT]);

	//+----------------+
	//|  �I�u�W�F�N�g  |
	//+----------------+
	// �Ŕ�
	D3DXLoadMeshFromX("data/MODEL/object/signboard.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_SIGNBOARD], NULL, &m_aNumMat[MODEL_TYPE_SIGNBOARD], &m_apMesh[MODEL_TYPE_SIGNBOARD]);
	// �j���W��
	D3DXLoadMeshFromX("data/MODEL/object/carrot.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_CARROT], NULL, &m_aNumMat[MODEL_TYPE_CARROT], &m_apMesh[MODEL_TYPE_CARROT]);
	// ��
	D3DXLoadMeshFromX("data/MODEL/object/rock.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_apBuffMat[MODEL_TYPE_ROCK], NULL, &m_aNumMat[MODEL_TYPE_ROCK], &m_apMesh[MODEL_TYPE_ROCK]);
}

//=============================================================================
// �I������
//=============================================================================
void CModelLoad::Uninit(void)
{
	for (int nCntModel = 0; nCntModel < MODEL_TYPE_MAX; nCntModel++)
	{
		// ���b�V���̔j��
		if (m_apMesh[nCntModel] != NULL)
		{
			m_apMesh[nCntModel]->Release();
			m_apMesh[nCntModel] = NULL;
		}

		// �}�e���A���̔j��
		if (m_apBuffMat[nCntModel] != NULL)
		{
			m_apBuffMat[nCntModel]->Release();
			m_apBuffMat[nCntModel] = NULL;
		}
	}
}

//=============================================================================
// ���b�V����������
//=============================================================================
LPD3DXMESH CModelLoad::GetMesh(MODEL_TYPE type)
{
	return m_apMesh[type];
}

//=============================================================================
// �}�e���A����������
//=============================================================================
LPD3DXBUFFER CModelLoad::GetBuffMat(MODEL_TYPE type)
{
	return m_apBuffMat[type];
}

//=============================================================================
// �}�e���A������������
//=============================================================================
DWORD CModelLoad::GetNumMat(MODEL_TYPE type)
{
	return m_aNumMat[type];
}
