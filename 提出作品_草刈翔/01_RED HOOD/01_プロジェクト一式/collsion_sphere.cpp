//=============================================================================
// ���̃R���W�������� [collision_sphere.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "collision_sphere.h"
#include "manager.h"
#include "renderer.h"
#include "scene3D.h"
#include "model.h"
#include "keyboard.h"
#include "fade.h"
#include "sound.h"
#include "particle_hit.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
bool CCollisionSphere::m_bMngVisual = false;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCollisionSphere::CCollisionSphere(int nPriority) :CScene3D(nPriority)
{
	// �ϐ��̃N���A
	m_pVtxBuff = NULL;
	m_pIdxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posSource = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pParent = NULL;
	m_col = D3DCOLOR_RGBA(0, 0, 0, 0);
	m_collisionType = COLLISION_S_TYPE_NONE;
	m_saveCollisionType = COLLISION_S_TYPE_NONE;
	m_nVertical = 0;
	m_nSide = 0;
	m_fTime = 0.0f;
	m_fDamage = 0.0f;
	m_fObjectiveRot = 0.0f;
	m_bContact = false;
	m_bContactOld = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCollisionSphere::~CCollisionSphere()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CCollisionSphere::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �ϐ��̏�����
	m_pos = pos;
	m_posSource = pos;
	m_size = size;
	m_nVertical = 16;
	m_nSide = 16;
	m_fObjectiveRot = 0.0f;
	m_bContact = false;
	m_bContactOld = false;

	// ���_�o�b�t�@�̐���
	//��(�c�̕������{�P)�~(���̕������{�P)�̒l�̒��_�𐶐�����
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * ((m_nVertical + 1) * (m_nSide + 1)),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_�o�b�t�@�ւ̃|�C���^
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nCntVtx = 0;

	// ���_���W�̐ݒ�
	for (int nCntSide = m_nSide + 1; nCntSide > 0; nCntSide--)
	{
		for (int nCntVertical = 0; nCntVertical < m_nVertical + 1; nCntVertical++, nCntVtx++)
		{
			pVtx[nCntVtx].pos = D3DXVECTOR3(cosf((D3DX_PI * 2.0f / m_nVertical) * nCntVertical) * (sinf(((D3DX_PI * 2.0f / m_nVertical) * nCntSide) + ((D3DX_PI * 2.0f / m_nVertical) * m_nSide)) * (m_size.x / 2.0f)),
											cosf((D3DX_PI * 2.0f / m_nVertical) * nCntSide) * (m_size.x / 2.0f),
											sinf((D3DX_PI * 2.0f / m_nVertical) * nCntVertical) * (sinf(((D3DX_PI * 2.0f / m_nVertical) * nCntSide) + ((D3DX_PI * 2.0f / m_nVertical) * m_nSide)) * (m_size.x / 2.0f)));

			// �@��
			pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

			// �J���[
			pVtx[nCntVtx].col = m_col;

			// �e�N�X�`��
			pVtx[nCntVtx].tex = D3DXVECTOR2(0.0f + -(1.0f / m_nVertical * nCntVertical), 0.0f + -(1.0f / m_nSide * nCntSide));
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * ((m_nSide - 1) * (m_nVertical + 3) + (m_nVertical + 1) * (m_nSide + 1)),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// �C���f�b�N�X���ւ̃|�C���^
	WORD *pIdx;

	// �C���f�b�N�X�o�b�t�@�����b�N���A�ԍ��f�[�^�ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// �ԍ��f�[�^�̐ݒ�
	for (int nCntSide = 0; nCntSide < m_nSide; nCntSide++)
	{
		for (int nCntVertical = 0; nCntVertical < m_nVertical + 1; nCntVertical++)
		{
			pIdx[(nCntVertical * 2) + 0 + (m_nVertical + 2) * 2 * nCntSide] = (m_nVertical + 1) + nCntVertical + (m_nVertical + 1) * nCntSide;
			pIdx[(nCntVertical * 2) + 1 + (m_nVertical + 2) * 2 * nCntSide] = 0 + nCntVertical + (m_nVertical + 1) * nCntSide;
		}
	}

	for (int nCntSide = 0; nCntSide < m_nSide - 1; nCntSide++)
	{
		pIdx[((m_nVertical + 1) * 2 + 0) * (nCntSide + 1) + (2 * nCntSide)] = m_nVertical + (m_nVertical + 1) * nCntSide;
		pIdx[((m_nVertical + 1) * 2 + 1) * (nCntSide + 1) + (1 * nCntSide)] = m_nVertical * 2 + 2 + (m_nVertical + 1) * nCntSide;
	}

	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	m_pIdxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CCollisionSphere::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// �C���f�b�N�X�o�b�t�@�̔j��
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CCollisionSphere::Update(void)
{
	// �O�̃t���[���ɓ����������ǂ����̏�����
	m_bContactOld = m_bContact;

	m_bContact = false;

	// �����m�̓����蔻��
	Collision(this);

#ifdef _DEBUG
	// ����̉�������
	//VisualChange();
#endif

	// ���������炷
	if (m_fTime > -1.0f)
	{
		m_fTime -= 1.0f;

		// ���Ԍo�߂ŏ���
		if (m_fTime <= 0.0f)
		{
			Uninit();
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CCollisionSphere::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans, mtxScale, mtxParent;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// �e�̃��f���̃}�g���b�N�X�̐ݒ�
	if (m_pParent != NULL)
	{
		mtxParent = m_pParent->GetMtx();

		// ���[���h�}�g���b�N�X�Ɛe�̃��[���h�}�g���b�N�X���|�����킹��
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);
	}
	// �e�̃��f���������ꍇ
	else
	{
		// ���[���h�}�g���b�N�X���擾
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

#ifdef _DEBUG
	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	if (m_bMngVisual == true)
	{
		// �|���S���̕`��
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
			0,
			0,
			((m_nVertical + 1) * (m_nSide + 1)),				//���_�̐�
			0,													//�J�n���钸�_�̃C���f�b�N�X
			(m_nSide * m_nVertical * 2) + (m_nSide * 4) - 4);	//�`�悷��v���~�e�B�u��
	}
#endif

	//���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// ��������
//=============================================================================
CCollisionSphere* CCollisionSphere::Create(D3DXVECTOR3 pos, float fSize, COLLISION_S_TYPE type, float fTime)
{
	// �N���X�̐���
	CCollisionSphere *pCollisionS = NULL;
	pCollisionS = new CCollisionSphere;

	if (pCollisionS != NULL)
	{
		// �ϐ��̏�����
		pCollisionS->m_fTime = fTime;
		pCollisionS->m_collisionType = type;

		// ���[���h�}�g���b�N�X�ݒ�O�ɏՓ˔�����s�����߁A��Ɉ����̈ʒu�����[���h�}�g���b�N�X�ɐݒ肵�Ă���
		pCollisionS->m_mtxWorld._41 = pos.x;
		pCollisionS->m_mtxWorld._42 = pos.y;
		pCollisionS->m_mtxWorld._43 = pos.z;

		switch (type)
		{
			// �Ȃ�
		case COLLISION_S_TYPE_NONE:
			pCollisionS->m_col = D3DCOLOR_RGBA(255, 255, 255, 0);
			break;
			// �v���C���[�F��
		case COLLISION_S_TYPE_PLAYER:
			pCollisionS->m_col = D3DCOLOR_RGBA(255, 255, 255, 153);
			break;
			// ����F��
		case COLLISION_S_TYPE_WEAPON:
			pCollisionS->m_col = D3DCOLOR_RGBA(0, 255, 0, 153);
			break;
			// ����[��]�F��
		case COLLISION_S_TYPE_WEAPON_P:
			pCollisionS->m_col = D3DCOLOR_RGBA(0, 255, 0, 153);
			break;
			// �G�F��
		case COLLISION_S_TYPE_ENEMY:
			pCollisionS->m_col = D3DCOLOR_RGBA(0, 0, 255, 153);
			break;
			// �G�̍U���F��
		case COLLISION_S_TYPE_ENEMY_ATTACK:
			pCollisionS->m_col = D3DCOLOR_RGBA(255, 0, 0, 153);
			break;
		default:
			break;
		}

		// ����������
		pCollisionS->Init(pos, D3DXVECTOR3(fSize, fSize, fSize));
	}
	return pCollisionS;
}

//=============================================================================
// �Փˏ���
//=============================================================================
void CCollisionSphere::Collision(CScene *pScene)
{
	// �Փ˂��Ă��邩�ǂ���
	m_saveCollisionType = COLLISION_S_TYPE_NONE;

	//�I�u�W�F�N�g��������|�C���^
	CScene *pObject = NULL;
	//�I�u�W�F�N�g����ۑ�����|�C���^�ϐ�
	CScene *pSaveObject = NULL;

	//�擪�̃|�C���^����
	pObject = pObject->GetTopObj(CScene::PRIORITY_EFFECT);

	while (pObject != NULL)
	{
		//���݂̃I�u�W�F�N�g�̃|�C���^��ۑ�
		pSaveObject = pObject;

		if (pObject->GetObjType() == CScene::OBJECT_TYPE_NONE)
		{
			CCollisionSphere *pCollisionS = NULL;
			pCollisionS = (CCollisionSphere*)pObject;

			// �����ȊO�̋��̃R���W��������������
			if (pObject != this)
			{
				D3DXVECTOR3 pos = D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);	//���g�̈ʒu
				D3DXVECTOR3 size = pScene->GetSize();											//���g�T�C�Y
				D3DXVECTOR3 posCollisionS = pCollisionS->GetMtxPos();							//�Ώۂ̃R���W�����̈ʒu
				D3DXVECTOR3 sizeCollisionS = pCollisionS->GetSize();							//�Ώۂ̃R���W�����̃T�C�Y
				COLLISION_S_TYPE collisionType = pCollisionS->GetCollisionType();				//�Ώۂ̃R���W�����̎��

				// �R���W�������m�̋����Ɣ��a�����߂�
				float fDistance = sqrtf((posCollisionS.x - pos.x) * (posCollisionS.x - pos.x) + (posCollisionS.y - pos.y) * (posCollisionS.y - pos.y) + (posCollisionS.z - pos.z) * (posCollisionS.z - pos.z));
				float fRadius = sqrtf((sizeCollisionS.x / 2 + size.x / 2) * (sizeCollisionS.x / 2 + size.x / 2));

				// ���������a��菬�����Ȃ�����Փ�
				if (fDistance < fRadius)
				{
					// �v���C���[�E�G�̍U��
					if ((m_collisionType == COLLISION_S_TYPE_PLAYER && collisionType == COLLISION_S_TYPE_ENEMY_ATTACK) ||
						(m_collisionType == COLLISION_S_TYPE_ENEMY_ATTACK && collisionType == COLLISION_S_TYPE_PLAYER))
					{
						// �Ώۂ̃R���W�����̎�ނ�ۑ�
						m_saveCollisionType = collisionType;

						// �G��Ă����Ԃɂ���
						m_bContact = true;

						// �Ώۂ̃R���W���������ʒu����A�Ώۂ̃R���W�����̌�����ݒ�
						D3DXVECTOR3 posSource = pCollisionS->GetPosSource();
						m_fObjectiveRot = (float)atan2((posSource.x - pos.x), (posSource.z - pos.z)) - D3DX_PI;

						if (m_collisionType == COLLISION_S_TYPE_ENEMY_ATTACK)
						{
							// �_���[�W��ݒ�
							pCollisionS->SetDamageValue(m_fDamage);
						}
					}
					// ����E�G
					else if ((m_collisionType == COLLISION_S_TYPE_WEAPON && collisionType == COLLISION_S_TYPE_ENEMY) ||
						(m_collisionType == COLLISION_S_TYPE_ENEMY && collisionType == COLLISION_S_TYPE_WEAPON))
					{
						// �Ώۂ̃R���W�����̎�ނ�ۑ�
						m_saveCollisionType = collisionType;

						// �G��Ă����Ԃɂ���
						m_bContact = true;

						if (m_collisionType == COLLISION_S_TYPE_WEAPON)
						{
							// �_���[�W��ݒ�
							pCollisionS->SetDamageValue(m_fDamage);
						}
					}
					// ����[��]�E�G
					else if ((m_collisionType == COLLISION_S_TYPE_WEAPON_P && collisionType == COLLISION_S_TYPE_ENEMY) ||
						(m_collisionType == COLLISION_S_TYPE_ENEMY && collisionType == COLLISION_S_TYPE_WEAPON_P))
					{
						// �Ώۂ̃R���W�����̎�ނ�ۑ�
						m_saveCollisionType = collisionType;

						// �G��Ă����Ԃɂ���
						m_bContact = true;

						if (m_collisionType == COLLISION_S_TYPE_WEAPON_P)
						{
							// �_���[�W��ݒ�
							pCollisionS->SetDamageValue(m_fDamage);
						}
					}
				}
			}
		}
		pObject = pSaveObject->GetObjNext(pSaveObject);
	}
}

//=============================================================================
// ���[���h�}�g���b�N�X���W�擾����
//=============================================================================
D3DXVECTOR3 CCollisionSphere::GetMtxPos(void)
{
	return D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);
}

//=============================================================================
// �Փ˂����u�Ԃ̏���
//=============================================================================
bool CCollisionSphere::OnCollisionEnter(COLLISION_S_TYPE type)
{
	if (m_saveCollisionType == type && (m_bContactOld == false && m_bContact == true))
	{
		return true;
	}
	else
	{
		return false;
	}
}

//=============================================================================
// �R���W�����̎�ސݒ菈��
//=============================================================================
void CCollisionSphere::SetCollisionType(COLLISION_S_TYPE type)
{
	m_collisionType = type;

	switch (type)
	{
		// �Ȃ�
	case COLLISION_S_TYPE_NONE:
		m_col = D3DCOLOR_RGBA(255, 255, 255, 0);
		break;
		// �v���C���[�F��
	case COLLISION_S_TYPE_PLAYER:
		m_col = D3DCOLOR_RGBA(255, 255, 255, 153);
		break;
		// ����F��
	case COLLISION_S_TYPE_WEAPON:
		m_col = D3DCOLOR_RGBA(0, 255, 0, 153);
		break; 
		// ����[��]�F��
	case COLLISION_S_TYPE_WEAPON_P:
		m_col = D3DCOLOR_RGBA(0, 255, 0, 153);
		break;
		// �G�F��
	case COLLISION_S_TYPE_ENEMY:
		m_col = D3DCOLOR_RGBA(0, 0, 255, 153);
		break;
		// �G�̍U���F��
	case COLLISION_S_TYPE_ENEMY_ATTACK:
		m_col = D3DCOLOR_RGBA(255, 0, 0, 153);
		break;
	default:
		break;
	}

	if (m_pVtxBuff != NULL)
	{
		VERTEX_3D *pVtx;
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		int nCntVtx = 0;
		for (int nCntSide = m_nSide + 1; nCntSide > 0; nCntSide--)
		{
			for (int nCntVertical = 0; nCntVertical < m_nVertical + 1; nCntVertical++, nCntVtx++)
			{
				// �J���[
				pVtx[nCntVtx].col = m_col;
			}
		}
		m_pVtxBuff->Unlock();
	}
}

#ifdef _DEBUG
//=============================================================================
// ����̉����ݒ菈��
//=============================================================================
void CCollisionSphere::SetVisual(void)
{
	m_bMngVisual = !m_bMngVisual;
}

//=============================================================================
// ����̉�������
//=============================================================================
void CCollisionSphere::VisualChange(void)
{
	//�L�[�{�[�h�擾����
	CKeyboard *pKeyboard = CManager::GetKeyboard();

	if (pKeyboard->GetTrigger(DIK_F2) == true)
	{
		m_bMngVisual = !m_bMngVisual;
	}
}
#endif