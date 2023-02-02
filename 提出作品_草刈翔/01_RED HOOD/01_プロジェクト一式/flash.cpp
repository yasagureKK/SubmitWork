//=============================================================================
// �t���b�V������ [flash.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "flash.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CFlash::CFlash(int nPriority) : CScene2D(nPriority)
{
	// �ϐ��̃N���A
	m_pVtxBuff = NULL;
	m_flash = FLASH_NONE;
	m_color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_TargetColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_fOnSpeed = 0.0f;
	m_fOffSpeed = 0.0f;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CFlash::~CFlash()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CFlash::Init(D3DXCOLOR col, float fOnSpeed, float fOffSpeed)
{
	// �ϐ��̏�����
	m_flash = FLASH_ON;
	m_TargetColor = D3DXCOLOR(col.r, col.g, col.b, col.a);
	m_color = D3DXCOLOR(m_TargetColor.r, m_TargetColor.g, m_TargetColor.b, 0.0f);
	m_fOnSpeed = fOnSpeed;
	m_fOffSpeed = fOffSpeed;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	// ���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = m_color;
	pVtx[1].col = m_color;
	pVtx[2].col = m_color;
	pVtx[3].col = m_color;

	// �e�N�X�`���̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CFlash::Uninit(void)
{
	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CFlash::Update(void)
{
	if (m_flash != FLASH_NONE)
	{
		// �t���b�V���I��
		if (m_flash == FLASH_ON)
		{
			//�����x��������
			m_color.a += m_fOnSpeed;

			if (m_color.a >= m_TargetColor.a)
			{
				m_color.a = m_TargetColor.a;
				m_flash = FLASH_OFF;
			}
		}
		// �t���b�V���I�t
		else if (m_flash == FLASH_OFF)
		{
			//�����x���グ��
			m_color.a -= m_fOffSpeed;

			if (m_color.a <= 0.0f)
			{
				m_color.a = 0.0f;
				m_flash = FLASH_NONE;
			}
		}

		VERTEX_2D *pVtx; //���_���ւ̃|�C���^
		m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);
		pVtx[0].col = m_color;
		pVtx[1].col = m_color;
		pVtx[2].col = m_color;
		pVtx[3].col = m_color;
		m_pVtxBuff->Unlock();
	}
	else if (m_flash == FLASH_NONE)
	{
		Uninit();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CFlash::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// ��������
//=============================================================================
CFlash *CFlash::Create(D3DXCOLOR col, float fOnSpeed, float fOffSpeed)
{
	// �N���X�̐���
	CFlash *pFlash = NULL;
	pFlash = new CFlash;

	if (pFlash != NULL)
	{
		// ����������
		pFlash->Init(col, fOnSpeed, fOffSpeed);
	}
	return pFlash;
}