//=============================================================================
// �r���{�[�h�̃G�t�F�N�g���� [billboard_effect.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "effect_billboard.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "model.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CEffectB::CEffectB()
{
	// �ϐ��̃N���A
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_defSize = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_mtxPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_pParent = NULL;
	m_type = EFFECT_TYPE_NORMAL;
	m_nLife = 0;
	m_nBeginLife = 0;
	m_fDefAlpha = 0.0f;
	m_bModel = true;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEffectB::~CEffectB()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CEffectB::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR3 move, int nLife)
{
	// �p�����N���X�̏���������
	CBillboard::Init(pos, size);

	// �ϐ��̏�����
	m_pos = pos;
	m_size = size;
	m_col = col;
	m_move = move;
	m_nLife = nLife;
	m_nBeginLife = m_nLife;
	m_defSize = D3DXVECTOR3(m_size.x / m_nBeginLife, m_size.y / m_nBeginLife, 0.0f);
	m_fDefAlpha = col.a / m_nBeginLife;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEffectB::Uninit(void)
{
	// �p�����N���X�̏I������
	CBillboard::Uninit();

	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CEffectB::Update(void)
{
	// �p�����N���X�̍X�V����
	CBillboard::Update();

	// ���X�ɏ�����^�C�v�Ȃ�
	if (m_type == EFFECT_TYPE_VANISH)
	{
		m_col.a -= m_fDefAlpha;

		if (m_col.a <= 0)
		{
			m_col.a = 0;
		}
	}
	// ���X�ɏ������Ȃ�^�C�v�Ȃ�
	else if (m_type == EFFECT_TYPE_SMALL)
	{
		// �����̎��Ԃɍ��킹�āA�T�C�Y�����������炷
		m_size.x -= m_defSize.x;
		m_size.y -= m_defSize.y;
	}

	// �T�C�Y�̐ݒ�
	CBillboard::SetSize(m_size);

	// �J���[�̐ݒ�
	CBillboard::SetColor(m_col);

	// ���������炷
	m_nLife--;

	// �������Ȃ��Ȃ�����
	if (m_nLife < 0)
	{
		// ���ł�����
		Uninit();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CEffectB::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//Z�o�b�t�@�̍X�V
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	if (m_bModel == true)
	{
		//���C�e�B���O�𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		//�A���t�@�e�X�g��L����
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 100);

		// �v�Z�p�}�g���b�N�X
		D3DXMATRIX mtxRot, mtxTrans, mtxScale, mtxParent;

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&m_mtxWorld);

		//�r���[�}�g���b�N�X���擾
		D3DXMATRIX mtxView;
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
		D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);
		m_mtxWorld._41 = 0.0f;
		m_mtxWorld._42 = 0.0f;
		m_mtxWorld._43 = 0.0f;

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		// �e�̃��f���̃}�g���b�N�X�̐ݒ�
		if (m_pParent != NULL)
		{
			mtxParent = m_pParent->GetMtx();

			// �Z�o�����p�[�c�̃��[���h�}�g���b�N�X�Ɛe�̃��[���h�}�g���b�N�X���|�����킹��
			D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

			// ���[���h�}�g���b�N�X���W��ۑ�
			m_mtxPos = D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);

			// �ʒu��ۑ��������[���h�}�g���b�N�X���W�ɍĐݒ�
			m_pos = m_mtxPos;

			// �e���f���̏��������A����ȏ�v�Z�����Ȃ��悤�ɂ���
			m_pParent = NULL;
		}

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, m_pTexture);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
			0,											//�J�n���钸�_�̃C���f�b�N�X
			2);											//�`�悷��v���~�e�B�u��

		//�A���t�@�e�X�g�𖳌���
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);

		//���C�e�B���O��L���ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
	else if (m_bModel == false)
	{
		// �p�����N���X�̕`�揈��
		CBillboard::Draw();
	}

	//Z�o�b�t�@�̍X�V
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ���u�����f�B���O��ʏ�ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// ��������
//=============================================================================
CEffectB *CEffectB::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR3 move,
	int nLife, EFFECT_TYPE type, CModel *pModel)
{
	// �N���X�̐���
	CEffectB *pEffectB = NULL;
	pEffectB = new CEffectB;

	if (pEffectB != NULL)
	{
		// �e�N�X�`���̊���
		pEffectB->m_pTexture = CManager::GetTexture()->GetTexture3D(0);

		// �ϐ��̐ݒ�
		pEffectB->m_type = type;
		pEffectB->m_pParent = pModel;

		// ���f����񂪂�������ϐ���true�ɂ��Ă���
		if (pEffectB->m_pParent != NULL)
		{
			pEffectB->m_bModel = true;
		}
		else
		{
			pEffectB->m_bModel = false;
		}

		// ����������
		pEffectB->Init(pos, size, col, move, nLife);
	}
	return pEffectB;
}