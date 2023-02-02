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

//=============================================================================
// コンストラクタ
//=============================================================================
CScene2D::CScene2D(int nPriority) : CScene(nPriority)
{
	// 変数のクリア
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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
HRESULT CScene2D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// 位置・サイズの初期化
	m_pos = pos;
	m_size = size;

	// 位置・サイズ設定処理
	CScene::SetPos(m_pos);
	CScene::SetSize(m_size);

	// 半径・角度の設定
	//m_fRadius = sqrtf(fSizeX * fSizeX + fSizeY * fSizeY);
	//m_fAngle = atan2f(fSizeX, fSizeY);
	//m_fAddAngle = 0;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,  //確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

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
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

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
CScene2D *CScene2D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nPriority)
{
	// インスタンスの生成
	CScene2D *pScene2D = NULL;

	if (pScene2D == NULL)
	{
		// 優先順位を持たせてクラスを生成
		pScene2D = new CScene2D(nPriority);

		// ヌルチェック
		if (pScene2D != NULL)
		{
			// 初期化処理
			pScene2D->Init(pos, size);
		}
	}

	return pScene2D;
}

//=============================================================================
// 位置設定処理
//=============================================================================
void CScene2D::SetPos(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// 位置・サイズに代入
	m_pos = pos;
	m_size = size;

	// 位置・サイズ設定処理
	CScene::SetPos(m_pos);
	CScene::SetSize(m_size);

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
// 位置取得処理
//=============================================================================
D3DXVECTOR3 CScene2D::GetPos(void)
{
	return m_pos;
}

//=============================================================================
// サイズ取得処理
//=============================================================================
D3DXVECTOR3 CScene2D::GetSize(void)
{
	return m_size;
}

//=============================================================================
// テクスチャ設定処理
//=============================================================================
void CScene2D::SetTexture(float fU, float fV, int nCutU, int nCutV)
{
	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点データのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

	//テクスチャ設定(U座標の位置、V座標の位置、横の分割数、縦の分割数を入れる)
	pVtx[0].tex = D3DXVECTOR2(fU * (1.0f / nCutU)				  , fV * (1.0f / nCutV));
	pVtx[1].tex = D3DXVECTOR2(fU * (1.0f / nCutU) + (1.0f / nCutU), fV * (1.0f / nCutV));
	pVtx[2].tex = D3DXVECTOR2(fU * (1.0f / nCutU)				  , fV * (1.0f / nCutV) + (1.0f / nCutV));
	pVtx[3].tex = D3DXVECTOR2(fU * (1.0f / nCutU) + (1.0f / nCutU), fV * (1.0f / nCutV) + (1.0f / nCutV));

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// テクスチャ割当処理
//=============================================================================
void CScene2D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	// テクスチャの割り当て
	m_pTexture = pTexture;
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