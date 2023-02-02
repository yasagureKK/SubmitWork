//=============================================================================
// 2Dオブジェクト処理 [scene2D.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene2D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CScene2D::CScene2D(int nPriority) : CScene(nPriority)
{
	// 変数のクリア
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_fRadius = 0.0f;
	m_fTilt = 0.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CScene2D::~CScene2D()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CScene2D::Init(D3DXVECTOR3 pos)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 変数の初期化
	m_fRadius = 0.0f;
	m_fTilt = 0.0f;

	// 位置の設定
	m_pos = pos;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	// 頂点情報へのポインタ
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - (m_size.x / 2), m_pos.y - (m_size.y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (m_size.x / 2), m_pos.y - (m_size.y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - (m_size.x / 2), m_pos.y + (m_size.y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (m_size.x / 2), m_pos.y + (m_size.y / 2), 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

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
void CScene2D::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)  //ヌルチェック
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
void CScene2D::Update(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void CScene2D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// フォグの影響を受けてしまうのでフォグを無効にする
	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// 生成処理
//=============================================================================
CScene2D *CScene2D::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nPriority)
{
	// クラスの生成
	CScene2D *pScene2D = NULL;
	pScene2D = new CScene2D(nPriority);

	if (pScene2D != NULL)
	{
		// 初期化処理
		pScene2D->Init(pos);
	}
	return pScene2D;
}

//=============================================================================
// サイズ設定処理
//=============================================================================
void CScene2D::SetPos(D3DXVECTOR3 pos)
{
	// 位置を更新
	m_pos = pos;

	// 頂点情報へのポインタ
	VERTEX_2D *pVtx;

	// 位置を更新
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - (m_size.x / 2), m_pos.y - (m_size.y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (m_size.x / 2), m_pos.y - (m_size.y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - (m_size.x / 2), m_pos.y + (m_size.y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (m_size.x / 2), m_pos.y + (m_size.y / 2), 0.0f);
	m_pVtxBuff->Unlock();
}

//=============================================================================
// サイズ設定処理
//=============================================================================
void CScene2D::SetSize(D3DXVECTOR2 size)
{
	// サイズを更新
	m_size = size;

	// 頂点情報へのポインタ
	VERTEX_2D *pVtx;

	// 位置を更新
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - (m_size.x / 2), m_pos.y - (m_size.y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (m_size.x / 2), m_pos.y - (m_size.y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - (m_size.x / 2), m_pos.y + (m_size.y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (m_size.x / 2), m_pos.y + (m_size.y / 2), 0.0f);
	m_pVtxBuff->Unlock();
}

//=============================================================================
// テクスチャ座標設定処理
//=============================================================================
void CScene2D::SetTexture(float fU, float fV, int nCutU, int nCutV)
{
	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	// テクスチャ座標を更新
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);
	pVtx[0].tex = D3DXVECTOR2(fU * (1.0f / nCutU),					fV * (1.0f / nCutV));
	pVtx[1].tex = D3DXVECTOR2(fU * (1.0f / nCutU) + (1.0f / nCutU), fV * (1.0f / nCutV));
	pVtx[2].tex = D3DXVECTOR2(fU * (1.0f / nCutU),					fV * (1.0f / nCutV) + (1.0f / nCutV));
	pVtx[3].tex = D3DXVECTOR2(fU * (1.0f / nCutU) + (1.0f / nCutU), fV * (1.0f / nCutV) + (1.0f / nCutV));
	m_pVtxBuff->Unlock();
}

//=============================================================================
// テクスチャ割当処理
//=============================================================================
void CScene2D::BindTexture(int nTexType)
{
	// テクスチャの割り当て
	m_pTexture = CManager::GetTexture()->GetTexture2D(nTexType);
}

//=============================================================================
// カラー設定処理
//=============================================================================
void CScene2D::SetColor(D3DXCOLOR col)
{
	// 頂点情報へのポインタ
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点データのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

	// カラーを更新
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 頂点座標位置設定処理
//=============================================================================
void CScene2D::SetBesideBarScale(float fScale, bool bReverse)
{
	m_size.x = fScale;

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (bReverse == false)
	{
		//頂点の座標
		pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y - (m_size.y / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y - (m_size.y / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + (m_size.y / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + (m_size.y / 2), 0.0f);
	}
	else if (bReverse == true)
	{
		//頂点の座標
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y - (m_size.y / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x, m_pos.y - (m_size.y / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y + (m_size.y / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x, m_pos.y + (m_size.y / 2), 0.0f);
	}

	m_pVtxBuff->Unlock();
}

//=============================================================================
// 傾き設定処理
//=============================================================================
void CScene2D::SetTilt(float fAngle)
{
	m_fRadius = sqrtf(m_size.x * m_size.x + m_size.y * m_size.y);
	m_fTilt = atan2f(m_size.x, m_size.y);

	// 頂点情報へのポインタ
	VERTEX_2D *pVtx;

	// 画像を傾ける
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fRadius * cosf(m_fTilt - fAngle), m_pos.y - m_fRadius * sinf(m_fTilt - fAngle), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fRadius * cosf(m_fTilt + fAngle), m_pos.y - m_fRadius * sinf(m_fTilt + fAngle), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fRadius * cosf(m_fTilt + fAngle), m_pos.y + m_fRadius * sinf(m_fTilt + fAngle), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fRadius * cosf(m_fTilt - fAngle), m_pos.y + m_fRadius * sinf(m_fTilt - fAngle), 0.0f);
	m_pVtxBuff->Unlock();
}