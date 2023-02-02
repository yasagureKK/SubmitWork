//=============================================================================
// フラッシュ処理 [flash.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "flash.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CFlash::CFlash(int nPriority) : CScene2D(nPriority)
{
	// 変数のクリア
	m_pVtxBuff = NULL;
	m_flash = FLASH_NONE;
	m_color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_TargetColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_fOnSpeed = 0.0f;
	m_fOffSpeed = 0.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CFlash::~CFlash()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CFlash::Init(D3DXCOLOR col, float fOnSpeed, float fOffSpeed)
{
	// 変数の初期化
	m_flash = FLASH_ON;
	m_TargetColor = D3DXCOLOR(col.r, col.g, col.b, col.a);
	m_color = D3DXCOLOR(m_TargetColor.r, m_TargetColor.g, m_TargetColor.b, 0.0f);
	m_fOnSpeed = fOnSpeed;
	m_fOffSpeed = fOffSpeed;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	// 頂点情報へのポインタ
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = m_color;
	pVtx[1].col = m_color;
	pVtx[2].col = m_color;
	pVtx[3].col = m_color;

	// テクスチャの設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CFlash::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CFlash::Update(void)
{
	if (m_flash != FLASH_NONE)
	{
		// フラッシュオン
		if (m_flash == FLASH_ON)
		{
			//透明度を下げる
			m_color.a += m_fOnSpeed;

			if (m_color.a >= m_TargetColor.a)
			{
				m_color.a = m_TargetColor.a;
				m_flash = FLASH_OFF;
			}
		}
		// フラッシュオフ
		else if (m_flash == FLASH_OFF)
		{
			//透明度を上げる
			m_color.a -= m_fOffSpeed;

			if (m_color.a <= 0.0f)
			{
				m_color.a = 0.0f;
				m_flash = FLASH_NONE;
			}
		}

		VERTEX_2D *pVtx; //頂点情報へのポインタ
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
// 描画処理
//=============================================================================
void CFlash::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// 生成処理
//=============================================================================
CFlash *CFlash::Create(D3DXCOLOR col, float fOnSpeed, float fOffSpeed)
{
	// クラスの生成
	CFlash *pFlash = NULL;
	pFlash = new CFlash;

	if (pFlash != NULL)
	{
		// 初期化処理
		pFlash->Init(col, fOnSpeed, fOffSpeed);
	}
	return pFlash;
}