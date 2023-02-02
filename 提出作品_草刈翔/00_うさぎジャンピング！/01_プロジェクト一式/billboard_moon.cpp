//=============================================================================
// ビルボード(ムーン)処理 [billboard_moon.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "billboard_moon.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "player.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CBillboardMoon::CBillboardMoon()
{
	// 変数のクリア
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// デストラクタ
//=============================================================================
CBillboardMoon::~CBillboardMoon()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBillboardMoon::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nNum)
{
	// 継承元クラスの初期化処理
	CBillboard::Init(pos, size);

	// 変数の初期化
	m_pos = pos;
	m_size = size;
	m_nNum = nNum;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBillboardMoon::Uninit(void)
{
	// 継承元クラスの終了処理
	CBillboard::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CBillboardMoon::Update(void)
{
	// 継承元クラスの更新処理
	CBillboard::Update();

	// オブジェクト情報を入れるポインタ・その情報を保存するポインタ
	CScene *pObject = NULL;
	CScene *pSaveObject = NULL;

	// 先頭のポインタを代入
	pObject = pObject->GetTopObj(CScene::PRIORITY_MODEL);

	while (pObject != NULL)
	{
		// 現在のオブジェクトのポインタを保存
		pSaveObject = pObject;

		if (pObject->GetObjType() == CScene::OBJECT_TYPE_PLAYER)
		{
			// プレイヤークラスにキャスト
			CPlayer *pPlayer = NULL;
			pPlayer = (CPlayer*)pObject;

			// プレイヤーの位置を取得
			D3DXVECTOR3 pos = pPlayer->GetPos();

			// 番号がプレイヤーの番号と一致していたら
			if (m_nNum == pPlayer->GetNum())
			{
				m_pos.x = pos.x + 500.0f;

				SetPos(m_pos, m_size);
			}
		}
		// 次のオブジェクトを取得
		pObject = pSaveObject->GetObjNext(pSaveObject);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CBillboardMoon::Draw(void)
{
	// 継承元クラスの更新処理
	CBillboard::Draw();
}

//=============================================================================
// 生成処理
//=============================================================================
CBillboardMoon *CBillboardMoon::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nNum)
{
	//インスタンスの生成
	CBillboardMoon *pBillboardMoon = NULL;

	// ヌルチェック
	if (pBillboardMoon == NULL)
	{
		// クラスの生成
		pBillboardMoon = new CBillboardMoon;
		if (pBillboardMoon != NULL)
		{
			// テクスチャの割当
			pBillboardMoon->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_MOON);
			pBillboardMoon->BindTexture(pBillboardMoon->m_pTexture);

			// 初期化処理
			pBillboardMoon->Init(pos, size, nNum);
		}
	}
	return pBillboardMoon;
}