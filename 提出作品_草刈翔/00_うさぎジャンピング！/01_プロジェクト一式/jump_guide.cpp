//=============================================================================
// ガイド処理 [jump_guide.cpp]
// Authore : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "jump_guide.h"
#include "number.h"
#include "manager.h"
#include "scene.h"
#include "player.h"
#include "ui.h"
#include "texture.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CJumpGuide::CJumpGuide(int nPriority) : CScene(nPriority)
{
	memset(&m_apNumber, NULL, sizeof(m_apNumber));
	memset(&m_apUi, NULL, sizeof(m_apUi));
	m_nPos = 0;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	m_nNum = 0;
	m_bChangeTex = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CJumpGuide::~CJumpGuide()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CJumpGuide::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// 変数の初期化
	m_pos = pos;
	m_size = size;
	m_bChangeTex = false;

	// 文字の生成
	m_apUi[0] = CUi::Create(D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y - m_size.y / 4 * 3, 0.0f),
		D3DXVECTOR3(200.0f, 60.0f, 0.0f), PRIORITY_UI, CUi::UI_TYPE_GAME_JUMP_GUIDE);

	for (int nCntScore = 0; nCntScore < MAX_SCORE_JUMP_GUIDE_DIGIT; nCntScore++)
	{
		// ナンバーの生成
		m_apNumber[nCntScore] = CNumber::Create(D3DXVECTOR3(m_pos.x + (nCntScore * (m_size.x / 5 * 4)), m_pos.y, m_pos.z), m_size, m_col);

		// 最後の時だけメートルをつける
		if (nCntScore == MAX_SCORE_JUMP_GUIDE_DIGIT - 1)
		{
			// m(メートル)の生成
			m_apUi[1] = CUi::Create(D3DXVECTOR3(m_pos.x + (nCntScore * m_size.x) + (m_size.x / 5 * 2), m_pos.y + (m_size.y / 8), 0.0f),
				D3DXVECTOR3(m_size.x * 0.8f, m_size.x * 0.8f, 0.0f), PRIORITY_UI, CUi::UI_TYPE_METER);
			m_apUi[1]->SetColor(m_col);
		}
	}

	// 位置・サイズの設定
	SetPos(m_pos);
	SetSize(m_size);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CJumpGuide::Uninit(void)
{
	//ナンバーの破棄
	for (int nCntScore = 0; nCntScore < MAX_SCORE_JUMP_GUIDE_DIGIT; nCntScore++)
	{
		// ヌルチェック
		if (m_apNumber[nCntScore] != NULL)
		{
			m_apNumber[nCntScore]->Uninit();
			delete m_apNumber[nCntScore];
			m_apNumber[nCntScore] = NULL;
		}
	}

	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CJumpGuide::Update(void)
{
	for (int nCntNumber = 0; nCntNumber < MAX_SCORE_JUMP_GUIDE_DIGIT; nCntNumber++)
	{
		if (nCntNumber == MAX_SCORE_JUMP_GUIDE_DIGIT - 1)
		{
			// 最後は10のn乗が使えなくなるため、決め打ちで処理
			m_apNumber[nCntNumber]->SetNumber(m_nPos % 10 / 1);
		}
		else
		{
			// ＜スコア % 10×n / 10×(n-1)＞ で出た値をそれぞれのナンバーに設定
			//※powf(float x, float y) … xのy乗の値
			m_apNumber[nCntNumber]->SetNumber(m_nPos % (int)powf(10, MAX_SCORE_JUMP_GUIDE_DIGIT - (float)nCntNumber) / (int)powf(10, MAX_SCORE_JUMP_GUIDE_DIGIT - ((float)nCntNumber + 1)));
		}
	}

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

			// スコアの番号がプレイヤーの番号と一致していたら
			if (m_nNum == pPlayer->GetNum() && pPlayer->GetLandFinish() == false)
			{
				// プレイヤーの位置を取得
				D3DXVECTOR3 pos = pPlayer->GetPos();

				// プレイヤーがジャンプする前
				if (pos.x < 0)
				{
					// -1をかけ、プレイヤーのX方向の位置を設定
					m_nPos = -(int)pos.x / 20;

					// 位置座標Xを更新
					SetPosValue(m_nPos);
				}
				// 踏み切りを過ぎて、ジャンプしていたら
				else if (pos.x >= 0 && pPlayer->GetJump() == true)
				{
					if (m_bChangeTex == false)
					{
						// 画像を切り替え
						m_apUi[0]->BindTexture(CManager::GetTexture()->GetTexture(CTexture::TEX_TYPE_GAME_JUMP_GUIDE01));
						m_bChangeTex = true;
					}
					
					// プレイヤーのY方向の位置を設定
					m_nPos = (int)pos.y / 20;

					// 位置座標Yを更新
					SetPosValue(m_nPos);
				}
			}
		}

		// 次のオブジェクトを取得
		pObject = pSaveObject->GetObjNext(pSaveObject);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CJumpGuide::Draw(void)
{
	for (int nCntScore = 0; nCntScore < MAX_SCORE_JUMP_GUIDE_DIGIT; nCntScore++)
	{
		// ヌルチェック
		if (m_apNumber[nCntScore] != NULL)
		{
			m_apNumber[nCntScore]->Draw();
		}
	}
}

//=============================================================================
// 生成処理
//=============================================================================
CJumpGuide *CJumpGuide::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col)
{
	// インスタンスの生成
	CJumpGuide *pJumpGuide = NULL;

	// ヌルチェック
	if (pJumpGuide == NULL)
	{
		pJumpGuide = new CJumpGuide;
		if (pJumpGuide != NULL)
		{
			// 変数の設定
			pJumpGuide->m_col = col;

			// 初期化処理
			pJumpGuide->Init(pos, size);
		}
	}
	return pJumpGuide;
}

//=============================================================================
// 位置座標の値設定処理
//=============================================================================
void CJumpGuide::SetPosValue(int nPos)
{
	m_nPos = nPos;
}

//=============================================================================
// 位置座標の値取得処理
//=============================================================================
int CJumpGuide::GetPosValue(void)
{
	return m_nPos;
}

//=============================================================================
// スコア番号設定処理
//=============================================================================
void CJumpGuide::SetNum(int nNum)
{
	m_nNum = nNum;
}

//=============================================================================
// スコア番号取得処理
//=============================================================================
int CJumpGuide::GetNum(void)
{
	return m_nNum;
}