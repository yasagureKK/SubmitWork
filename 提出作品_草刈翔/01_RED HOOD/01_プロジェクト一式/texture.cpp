//=============================================================================
// テクスチャ管理 [texture.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "texture.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FILE_TEX_2D "data/TEXT/texture_2D.txt"	// 読み込むファイル名
#define FILE_TEX_3D "data/TEXT/texture_3D.txt"	// 読み込むファイル名

//=============================================================================
// コンストラクタ
//=============================================================================
CTexture::CTexture()
{
	// 変数のクリア
	memset(&m_apTexture2D, NULL, sizeof(m_apTexture2D));
	memset(&m_apTexture3D, NULL, sizeof(m_apTexture3D));
}

//=============================================================================
// デストラクタ
//=============================================================================
CTexture::~CTexture()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
void CTexture::Init(void)
{
	// テクスチャとテクスチャの最大数を読み込む
	m_nMaxTex2D = LoadTexture(m_apTexture2D);
	m_nMaxTex3D = LoadTexture(m_apTexture3D);
}

//=============================================================================
// 終了処理
//=============================================================================
void CTexture::Uninit(void)
{
	// テクスチャの破棄
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
// テクスチャの取得処理
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
// テクスチャロード処理
//=============================================================================
int CTexture::LoadTexture(LPDIRECT3DTEXTURE9 tex[])
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//ファイル読み込み
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

			if (strcmp(&cString[0], "NUM_TEXTURE") == 0)	// NUM_TEXTUREの文字列
			{
				// テクスチャの最大数を取得
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

			if (strcmp(&cString[0], "TEXTURE_FILENAME") == 0) // TEXTURE_FILENAMEの文字列
			{
				// テクスチャを読み込む
				fscanf(pFile, "%s", &cString[0]);
				fscanf(pFile, "%s", &cString[0]);
				D3DXCreateTextureFromFile(pDevice, &cString[0], &tex[nCntTex]);
				nCntTex++;
			}

			if (strcmp(&cString[0], "END_SCRIPT") == 0) // END_SCRIPTの文字列
			{
				break;
			}
		}
		fclose(pFile);
	}
	return nMaxTex;
}