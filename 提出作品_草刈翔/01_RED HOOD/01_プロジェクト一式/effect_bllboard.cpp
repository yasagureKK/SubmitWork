//=============================================================================
// ビルボードのエフェクト処理 [billboard_effect.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "effect_billboard.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "model.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CEffectB::CEffectB()
{
	// 変数のクリア
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
// デストラクタ
//=============================================================================
CEffectB::~CEffectB()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEffectB::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR3 move, int nLife)
{
	// 継承元クラスの初期化処理
	CBillboard::Init(pos, size);

	// 変数の初期化
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
// 終了処理
//=============================================================================
void CEffectB::Uninit(void)
{
	// 継承元クラスの終了処理
	CBillboard::Uninit();

	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEffectB::Update(void)
{
	// 継承元クラスの更新処理
	CBillboard::Update();

	// 徐々に消えるタイプなら
	if (m_type == EFFECT_TYPE_VANISH)
	{
		m_col.a -= m_fDefAlpha;

		if (m_col.a <= 0)
		{
			m_col.a = 0;
		}
	}
	// 徐々に小さくなるタイプなら
	else if (m_type == EFFECT_TYPE_SMALL)
	{
		// 寿命の時間に合わせて、サイズを少しずつ減らす
		m_size.x -= m_defSize.x;
		m_size.y -= m_defSize.y;
	}

	// サイズの設定
	CBillboard::SetSize(m_size);

	// カラーの設定
	CBillboard::SetColor(m_col);

	// 寿命を減らす
	m_nLife--;

	// 寿命がなくなったら
	if (m_nLife < 0)
	{
		// 消滅させる
		Uninit();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CEffectB::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//Zバッファの更新
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	if (m_bModel == true)
	{
		//ライティングを無効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		//アルファテストを有効に
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 100);

		// 計算用マトリックス
		D3DXMATRIX mtxRot, mtxTrans, mtxScale, mtxParent;

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);

		//ビューマトリックスを取得
		D3DXMATRIX mtxView;
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		//ポリゴンをカメラに対して正面に向ける
		D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);
		m_mtxWorld._41 = 0.0f;
		m_mtxWorld._42 = 0.0f;
		m_mtxWorld._43 = 0.0f;

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		// 親のモデルのマトリックスの設定
		if (m_pParent != NULL)
		{
			mtxParent = m_pParent->GetMtx();

			// 算出したパーツのワールドマトリックスと親のワールドマトリックスを掛け合わせる
			D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

			// ワールドマトリックス座標を保存
			m_mtxPos = D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);

			// 位置を保存したワールドマトリックス座標に再設定
			m_pos = m_mtxPos;

			// 親モデルの情報を消し、これ以上計算させないようにする
			m_pParent = NULL;
		}

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, m_pTexture);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			0,											//開始する頂点のインデックス
			2);											//描画するプリミティブ数

		//アルファテストを無効に
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);

		//ライティングを有効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
	else if (m_bModel == false)
	{
		// 継承元クラスの描画処理
		CBillboard::Draw();
	}

	//Zバッファの更新
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// αブレンディングを通常に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// 生成処理
//=============================================================================
CEffectB *CEffectB::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR3 move,
	int nLife, EFFECT_TYPE type, CModel *pModel)
{
	// クラスの生成
	CEffectB *pEffectB = NULL;
	pEffectB = new CEffectB;

	if (pEffectB != NULL)
	{
		// テクスチャの割当
		pEffectB->m_pTexture = CManager::GetTexture()->GetTexture3D(0);

		// 変数の設定
		pEffectB->m_type = type;
		pEffectB->m_pParent = pModel;

		// モデル情報があったら変数をtrueにしておく
		if (pEffectB->m_pParent != NULL)
		{
			pEffectB->m_bModel = true;
		}
		else
		{
			pEffectB->m_bModel = false;
		}

		// 初期化処理
		pEffectB->Init(pos, size, col, move, nLife);
	}
	return pEffectB;
}