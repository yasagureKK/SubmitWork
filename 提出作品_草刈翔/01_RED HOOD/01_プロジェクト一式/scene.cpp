//=============================================================================
// オブジェクト管理 [scene.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene.h"
#include "manager.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CScene *CScene::m_pTop[PRIORITY_MAX] = {};
CScene *CScene::m_pCur[PRIORITY_MAX] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CScene::CScene(int nPriority)
{
	// 一番最初のオブジェクト
	if (m_pTop[nPriority] == NULL && m_pCur[nPriority] == NULL)
	{
		// 先頭のオブジェクトを設定
		m_pTop[nPriority] = this;

		// 次のオブジェクト情報がなければ最後尾に設定
		m_pCur[nPriority] = this;

		// 前の情報・次の情報はNULL
		m_pPrev = NULL;
		m_pNext = NULL;
	}
	// 二つ目以降のオブジェクト
	else
	{
		// 前のオブジェクトに最後尾の情報を入れる
		m_pPrev = m_pCur[nPriority];

		// 前のオブジェクトの次のオブジェクトをこのオブジェクトに設定
		m_pCur[nPriority]->m_pNext = this;

		// 生成するたびに最後尾を設定
		m_pCur[nPriority] = this;
		m_pNext = NULL;
	}

	// 変数の初期化
	m_nPriority = nPriority;
	m_bDeath = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CScene::~CScene()
{
	CScene *pScene = this;
	CScene *pSceneNext = pScene->m_pNext;
	CScene *pScenePrev = pScene->m_pPrev;

	// オブジェクトをリストから解除

	// 先頭でも最後尾でもない場合
	if (pScene != m_pTop[m_nPriority] && pScene != m_pCur[m_nPriority])
	{
		// 前のオブジェクトが持っている次のオブジェクト情報を自分の次のオブジェクト情報にする
		pScenePrev->m_pNext = pSceneNext;
		// 次のオブジェクトが持っている前のオブジェクト情報を自分の前のオブジェクト情報にする
		pSceneNext->m_pPrev = pScenePrev;
	}

	// 先頭だった場合
	if (pScene == m_pTop[m_nPriority])
	{
		m_pTop[m_nPriority] = pSceneNext;

		// 次のオブジェクトがあったら
		if (pSceneNext != NULL)
		{
			// 次のオブジェクトが持っている前のオブジェクト情報をNULLにする
			pSceneNext->m_pPrev = NULL;
		}
	}

	// 最後尾だった場合
	if (pScene == m_pCur[m_nPriority])
	{
		// 最後尾を前のオブジェクトにずらす
		m_pCur[m_nPriority] = pScenePrev;

		// 前のオブジェクトがあったら
		if (pScenePrev != NULL)
		{
			// 前のオブジェクトが持っている次のオブジェクト情報をNULLにする
			pScenePrev->m_pNext = NULL;
		}
	}
}

//=============================================================================
// 全てのオブジェクトの開放処理
//=============================================================================
void CScene::ReleaseAll(void)
{
	CScene *pScene = NULL;

	// 描画優先順に処理
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		// フェードは処理させない
		if (nCntPriority != PRIORITY_FADE)
		{
			pScene = m_pTop[nCntPriority];

			// pSceneがNULLじゃないならループ
			while (pScene)
			{
				// 次のオブジェクトのポインタを保存
				CScene *pSceneSave = pScene->m_pNext;

				// 終了処理
				pScene->Uninit();

				if (pScene->m_bDeath == true)
				{
					// 死亡フラグがtrueのオブジェクトを破棄
					delete pScene;
					pScene = NULL;
				}

				// 次のオブジェクトのポインタにする
				pScene = pSceneSave;
			}
		}
	}
}

//=============================================================================
// 全てのオブジェクトの更新処理
//=============================================================================
void CScene::UpdateAll(void)
{
	CScene *pScene = NULL;

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		// 先頭のオブジェクトのポインタ
		pScene = m_pTop[nCntPriority];

		// pSceneがNULLじゃないならループ
		while (pScene)
		{
			// ポーズ中じゃないなら更新
			if (CManager::GetPause() == false)
			{
				pScene->Update();
			}
			// ポーズ中なら
			else
			{
				// フェードとポーズのみ更新させる
				if (nCntPriority == PRIORITY_FADE || nCntPriority == PRIORITY_PAUSE)
				{
					pScene->Update();
				}
			}

			//次のオブジェクトのポインタにする
			pScene = pScene->m_pNext;
		}
	}

	//オブジェクト情報を保存するポインタ変数
	CScene *pSaveObject = NULL;
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//先頭のポインタを代入
		pScene = m_pTop[nCntPriority];
		while (pScene != NULL)
		{
			// 次のオブジェクトのポインタを保存
			pSaveObject = pScene->m_pNext;

			//死亡フラグチェック
			if (pScene->m_bDeath == true)
			{
				//破棄
				delete pScene;
				pScene = NULL;
			}
			//次のオブジェクトのポインタにする
			pScene = pSaveObject;
		}
	}
}

//=============================================================================
// 全てのオブジェクトの描画処理
//=============================================================================
void CScene::DrawAll(void)
{
	// 描画優先順位の低いものから描画させていく
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		// 最初に先頭を設定
		CScene *pScene = m_pTop[nCntPriority];

		while (pScene != NULL)
		{
			// 次のオブジェクタのポインタを保存
			CScene *pSceneNext = pScene->m_pNext;

			// 描画処理
			pScene->Draw();

			//次のオブジェクトのポインタにする
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
// 単体の開放処理
//=============================================================================
void CScene::Release()
{
	// 死亡フラグをtrueにする
	m_bDeath = true;
}

//=============================================================================
// 引数のオブジェクトの次のオブジェクトの情報取得処理
//=============================================================================
CScene *CScene::GetObjNext(CScene *pObject)
{
	return pObject->m_pNext;
}

//=============================================================================
// 引数のプライオリティの先頭オブジェクトの取得処理
//=============================================================================
CScene *CScene::GetTopObj(int nPriority)
{
	return m_pTop[nPriority];
}