//=============================================================================
// フェード処理 [fade.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "fade.h"
#include "renderer.h"
#include "keyboard.h"
#include "sound.h"
#include "scene2D.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CFade::FADE CFade::m_fade = FADE_NONE;

//=============================================================================
// コンストラクタ
//=============================================================================
CFade::CFade(int nPriority) : CScene(nPriority)
{
	// 変数のクリア
	m_pVtxBuff = NULL;
	m_fade = FADE_NONE;
	m_color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//=============================================================================
// デストラクタ
//=============================================================================
CFade::~CFade()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CFade::Init(D3DXVECTOR3 pos)
{
	// 変数の初期化
	m_fade = FADE_IN;
	m_color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

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
void CFade::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CFade::Update(void)
{
#ifdef _DEBUG
	CKeyboard *pKeyboard;
	pKeyboard = CManager::GetKeyboard();

	//サウンド取得処理
	CSound *pSound = CManager::GetSound();

	// デバッグ用の画面遷移
	if (GetFade() == CFade::FADE_NONE)
	{
		if (pKeyboard->GetTrigger(DIK_F6) == true)
		{
			// タイトル画面に遷移
			SetFade(CManager::MODE_TITLE, D3DCOLOR_RGBA(0, 0, 0, 0));
			pSound->Stop();
		}
		else if (pKeyboard->GetTrigger(DIK_F7) == true)
		{
			// ゲーム画面に遷移
			SetFade(CManager::MODE_GAME, D3DCOLOR_RGBA(0, 0, 0, 0));
			pSound->Stop();
		}
	}
#endif

	if (m_fade != FADE_NONE)
	{
		// フェードイン
		if (m_fade == FADE_IN)
		{
			//透明度を上げる
			m_color.a -= 0.025f;

			if (m_color.a <= 0.0f)
			{
				m_color.a = 0.0f;
				m_fade = FADE_NONE;
			}
		}
		// フェードアウト
		else if (m_fade == FADE_OUT)
		{
			//透明度を下げる
			m_color.a += 0.025f;

			if (m_color.a >= 1.0f)
			{
				m_color.a = 1.0f;
				m_fade = FADE_IN;

				//モードの設定
				CManager::SetMode(m_modeNext);
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
}

//=============================================================================
// 描画処理
//=============================================================================
void CFade::Draw(void)
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
// 設定処理
//=============================================================================
void CFade::SetFade(CManager::MODE modeNext, D3DXCOLOR col)
{
	m_fade = FADE_OUT;
	m_modeNext = modeNext;
	m_color = D3DXCOLOR(col.r, col.g, col.b, 0.0f);
}