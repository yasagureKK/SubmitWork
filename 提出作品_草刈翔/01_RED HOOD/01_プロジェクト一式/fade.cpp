//=============================================================================
// �t�F�[�h���� [fade.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "fade.h"
#include "renderer.h"
#include "keyboard.h"
#include "sound.h"
#include "scene2D.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
CFade::FADE CFade::m_fade = FADE_NONE;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CFade::CFade(int nPriority) : CScene(nPriority)
{
	// �ϐ��̃N���A
	m_pVtxBuff = NULL;
	m_fade = FADE_NONE;
	m_color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CFade::~CFade()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CFade::Init(D3DXVECTOR3 pos)
{
	// �ϐ��̏�����
	m_fade = FADE_IN;
	m_color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

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
void CFade::Uninit(void)
{
	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CFade::Update(void)
{
#ifdef _DEBUG
	CKeyboard *pKeyboard;
	pKeyboard = CManager::GetKeyboard();

	//�T�E���h�擾����
	CSound *pSound = CManager::GetSound();

	// �f�o�b�O�p�̉�ʑJ��
	if (GetFade() == CFade::FADE_NONE)
	{
		if (pKeyboard->GetTrigger(DIK_F6) == true)
		{
			// �^�C�g����ʂɑJ��
			SetFade(CManager::MODE_TITLE, D3DCOLOR_RGBA(0, 0, 0, 0));
			pSound->Stop();
		}
		else if (pKeyboard->GetTrigger(DIK_F7) == true)
		{
			// �Q�[����ʂɑJ��
			SetFade(CManager::MODE_GAME, D3DCOLOR_RGBA(0, 0, 0, 0));
			pSound->Stop();
		}
	}
#endif

	if (m_fade != FADE_NONE)
	{
		// �t�F�[�h�C��
		if (m_fade == FADE_IN)
		{
			//�����x���グ��
			m_color.a -= 0.025f;

			if (m_color.a <= 0.0f)
			{
				m_color.a = 0.0f;
				m_fade = FADE_NONE;
			}
		}
		// �t�F�[�h�A�E�g
		else if (m_fade == FADE_OUT)
		{
			//�����x��������
			m_color.a += 0.025f;

			if (m_color.a >= 1.0f)
			{
				m_color.a = 1.0f;
				m_fade = FADE_IN;

				//���[�h�̐ݒ�
				CManager::SetMode(m_modeNext);
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
}

//=============================================================================
// �`�揈��
//=============================================================================
void CFade::Draw(void)
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
// �ݒ菈��
//=============================================================================
void CFade::SetFade(CManager::MODE modeNext, D3DXCOLOR col)
{
	m_fade = FADE_OUT;
	m_modeNext = modeNext;
	m_color = D3DXCOLOR(col.r, col.g, col.b, 0.0f);
}