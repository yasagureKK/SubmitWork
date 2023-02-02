//=============================================================================
// �e�N�X�`���Ǘ� [texture.cpp]
// Author : ���� ��
//=============================================================================
//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "texture.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FILE_TEX_2D "data/TEXT/texture_2D.txt"	// �ǂݍ��ރt�@�C����
#define FILE_TEX_3D "data/TEXT/texture_3D.txt"	// �ǂݍ��ރt�@�C����

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTexture::CTexture()
{
	// �ϐ��̃N���A
	memset(&m_apTexture2D, NULL, sizeof(m_apTexture2D));
	memset(&m_apTexture3D, NULL, sizeof(m_apTexture3D));
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTexture::~CTexture()
{

}

//=============================================================================
// ����������
//=============================================================================
void CTexture::Init(void)
{
	// �e�N�X�`���ƃe�N�X�`���̍ő吔��ǂݍ���
	m_nMaxTex2D = LoadTexture(m_apTexture2D);
	m_nMaxTex3D = LoadTexture(m_apTexture3D);
}

//=============================================================================
// �I������
//=============================================================================
void CTexture::Uninit(void)
{
	// �e�N�X�`���̔j��
	for (int nCntTexture = 0; nCntTexture < MAX_TEXTURE_2D; nCntTexture++)
	{
		if (m_apTexture2D[nCntTexture] != NULL)
		{
			m_apTexture2D[nCntTexture]->Release();
			m_apTexture2D[nCntTexture] = NULL;
		}
	}

	for (int nCntTexture = 0; nCntTexture < MAX_TEXTURE_3D; nCntTexture++)
	{
		if (m_apTexture3D[nCntTexture] != NULL)
		{
			m_apTexture3D[nCntTexture]->Release();
			m_apTexture3D[nCntTexture] = NULL;
		}
	}
}

//=============================================================================
// �e�N�X�`���̎擾����
//=============================================================================
LPDIRECT3DTEXTURE9 CTexture::GetTexture2D(int nTexture)
{
	if (nTexture >= 0 && nTexture < MAX_TEXTURE_2D)
	{
		return m_apTexture2D[nTexture];
	}
	else
	{
		return NULL;
	}
}

LPDIRECT3DTEXTURE9 CTexture::GetTexture3D(int nTexture)
{
	if (nTexture >= 0 && nTexture < MAX_TEXTURE_3D)
	{
		return m_apTexture3D[nTexture];
	}
	else
	{
		return NULL;
	}
}

//=============================================================================
// �e�N�X�`�����[�h����
//=============================================================================
int CTexture::LoadTexture(LPDIRECT3DTEXTURE9 tex[])
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�t�@�C���ǂݍ���
	char cString[256];
	FILE *pFile = NULL;

	if (tex == m_apTexture2D)
	{
		pFile = fopen(FILE_TEX_2D, "r");
	}
	else if (tex == m_apTexture3D)
	{
		pFile = fopen(FILE_TEX_3D, "r");
	}

	int nCntTex = 0, nMaxTex = 0;

	if (pFile != NULL)
	{
		while (1)
		{
			fscanf(pFile, "%s", &cString[0]);

			if (strcmp(&cString[0], "NUM_TEXTURE") == 0)	// NUM_TEXTURE�̕�����
			{
				// �e�N�X�`���̍ő吔���擾
				fscanf(pFile, "%s", &cString[0]);
				fscanf(pFile, "%d", &nMaxTex);

				if (tex == m_apTexture2D && nMaxTex >= MAX_TEXTURE_2D)
				{
					nMaxTex = MAX_TEXTURE_2D;
				}
				else if (tex == m_apTexture3D && nMaxTex >= MAX_TEXTURE_3D)
				{
					nMaxTex = MAX_TEXTURE_3D;
				}
			}

			if (strcmp(&cString[0], "TEXTURE_FILENAME") == 0) // TEXTURE_FILENAME�̕�����
			{
				// �e�N�X�`����ǂݍ���
				fscanf(pFile, "%s", &cString[0]);
				fscanf(pFile, "%s", &cString[0]);
				D3DXCreateTextureFromFile(pDevice, &cString[0], &tex[nCntTex]);
				nCntTex++;
			}

			if (strcmp(&cString[0], "END_SCRIPT") == 0) // END_SCRIPT�̕�����
			{
				break;
			}
		}
		fclose(pFile);
	}
	return nMaxTex;
}