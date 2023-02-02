//=============================================================================
// ステージデータ [stage_data.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "stage_data.h"
#include "manager.h"
#include "texture.h"
#include "player.h"
#include "model_single.h"
#include "billboard.h"
#include "texture.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
int CStageData::m_nModelNum = 0;
int CStageData::m_nBillboardNum = 0;

//=============================================================================
// コンストラクタ
//=============================================================================
CStageData::CStageData()
{
	// 変数のクリア
	memset(&aStageModel, NULL, sizeof(aStageModel));
	memset(&aStageBillboard, NULL, sizeof(aStageBillboard));
	m_nModelNum = 0;
	m_nBillboardNum = 0;
	m_nNumPlayer = 0;
	m_nPattern = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CStageData::~CStageData()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CStageData::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// 変数の初期化
	m_pos = pos;
	m_size = size;

	int nRandumPattern = rand() % MAX_PATTERN;

	// ステージのロード
	LoadStage(nRandumPattern);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CStageData::Uninit(void)
{
	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CStageData::Update(void)
{
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

			// 番号がプレイヤーの番号と一致していたら
			if (m_nNumPlayer == pPlayer->GetNum())
			{
				// プレイヤーの位置を取得
				D3DXVECTOR3 pos = pPlayer->GetPos();

				// 位置＋サイズの位置にプレイヤーが来たら消す
				if (pos.x > m_pos.x + m_size.x)
				{
					// オブジェクトを一回消去
					DeleteObject();

					// 位置をずらす
					m_pos.x += m_size.x * 2;

					Init(m_pos, m_size);
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
void CStageData::Draw(void)
{

}

//=============================================================================
// 生成処理
//=============================================================================
CStageData *CStageData::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nNumPlayer)
{
	// インスタンスの生成
	CStageData *pStageData = NULL;

	// ヌルチェック
	if (pStageData == NULL)
	{
		// クラスの生成
		pStageData = new CStageData;
		if (pStageData != NULL)
		{
			// 変数の初期化
			pStageData->m_nNumPlayer = nNumPlayer;

			// 初期化処理
			pStageData->Init(pos, size);
		}
	}

	return pStageData;
}

//=============================================================================
// ステージロード処理
//=============================================================================
void CStageData::LoadStage(int nPattern)
{
	// テキスト保存用の変数
	char cString[MAX_STRING];

	// テキストファイルの読み込み
	FILE *pFile = fopen(STAGE_DATA_FILE, "r");

	if (pFile != NULL)
	{
		for (int nNumPattern = 0; nNumPattern < MAX_PATTERN; nNumPattern++)
		{
			// テキストファイルの最後(NULL)まで読み込む
			while (fgets(cString, MAX_STRING, pFile) != NULL)
			{
				// 文字列を保存
				fscanf(pFile, "%s", cString);

				// 文字列の中に"OBJECTSET"があったら
				if (strncmp("OBJECTSET", cString, 10) == 0)
				{
					// パターンの番号を取得
					fscanf(pFile, "%s%*s%d%*s%*s", cString, &m_nPattern);
					// モデルの最大数を取得
					fscanf(pFile, "%s%*s%d%*s%*s", cString, &m_nModelNum);
					// ビルボードの最大数を取得
					fscanf(pFile, "%s%*s%d", cString, &m_nBillboardNum);
					break;
				}
			}

			// 最大数を超えないようにする
			if (m_nModelNum > MAX_STAGE_OBJECT)
			{
				m_nModelNum = MAX_STAGE_OBJECT - 1;
			}

			if (m_nBillboardNum > MAX_STAGE_OBJECT)
			{
				m_nBillboardNum = MAX_STAGE_OBJECT - 1;
			}

			// ランダムで決まった数字とパターン番号が同じだったら配置を開始
			if (nPattern == m_nPattern)
			{
				/******************************/
				/* 取得したモデルの数ぶん配置 */
				/******************************/
				for (int nCntStageModel = 0; nCntStageModel < m_nModelNum; nCntStageModel++)
				{
					while (fgets(cString, MAX_STRING, pFile) != NULL)
					{
						// 文字列を保存
						fscanf(pFile, "%s", cString);

						// 文字列の中に"MODELSET"があったら
						if (strncmp("MODELSET", cString, 9) == 0)
						{
							break;
						}
					}

					while (fgets(cString, MAX_STRING, pFile) != NULL)
					{
						// 文字列を保存
						fscanf(pFile, "%s", cString);

						// 文字列の中に"TYPE"があったら
						if (strncmp("TYPE", cString, 5) == 0)
						{
							// 種類を取得する
							fscanf(pFile, "%s%d%*s%*s", cString, &aStageModel[nCntStageModel].nType);

							// 位置を取得
							fscanf(pFile, "%s%*s%f%f%f%*s", cString,
								&aStageModel[nCntStageModel].pos.x,
								&aStageModel[nCntStageModel].pos.y,
								&aStageModel[nCntStageModel].pos.z);

							// 向きを取得
							fscanf(pFile, "%s%*s%*s%f%f%f", cString,
								&aStageModel[nCntStageModel].rot.x,
								&aStageModel[nCntStageModel].rot.y,
								&aStageModel[nCntStageModel].rot.z);
							break;
						}
					}

					// int型をenum型にキャスト
					CModelLoad::MODEL_TYPE type = (CModelLoad::MODEL_TYPE)aStageModel[nCntStageModel].nType;

					// モデルを生成
					aStageModel[nCntStageModel].pModelSingle = CModelSingle::Create
					(m_pos + aStageModel[nCntStageModel].pos, aStageModel[nCntStageModel].rot,
						type, CModelSingle::MODEL_SINGLE_TYPE_NONE);
					// 影を設定
					aStageModel[nCntStageModel].pModelSingle->SetShadow(true, 0.0f);

					while (fgets(cString, MAX_STRING, pFile) != NULL)
					{
						// 文字列を保存
						fscanf(pFile, "%s", cString);

						// 文字列の中に"END_MODELSET"があったらそこで終了
						if (strncmp("END_MODELSET", cString, 13) == 0)
						{
							break;
						}
					}
				}

				/**********************************/
				/* 取得したビルボードの数ぶん配置 */
				/**********************************/
				for (int nCntStageBillboard = 0; nCntStageBillboard < m_nBillboardNum; nCntStageBillboard++)
				{
					while (fgets(cString, MAX_STRING, pFile) != NULL)
					{
						// 文字列を保存
						fscanf(pFile, "%s", cString);

						// 文字列の中に"BILLBOARDSET"があったら
						if (strncmp("BILLBOARDSET", cString, 13) == 0)
						{
							break;
						}
					}

					while (fgets(cString, MAX_STRING, pFile) != NULL)
					{
						// 文字列を保存
						fscanf(pFile, "%s", cString);

						// 文字列の中に"TEXTYPE"があったら
						if (strncmp("TEXTYPE", cString, 8) == 0)
						{
							// テクスチャの種類を取得する
							fscanf(pFile, "%s%d%*s%*s", cString, &aStageBillboard[nCntStageBillboard].nTexType);

							// 位置を取得
							fscanf(pFile, "%s%*s%f%f%f%*s", cString,
								&aStageBillboard[nCntStageBillboard].pos.x,
								&aStageBillboard[nCntStageBillboard].pos.y,
								&aStageBillboard[nCntStageBillboard].pos.z);

							// サイズを取得
							fscanf(pFile, "%s%*s%*s%f%f", cString,
								&aStageBillboard[nCntStageBillboard].size.x,
								&aStageBillboard[nCntStageBillboard].size.y);
							break;
						}
					}

					// int型をenum型にキャスト
					CTexture::TEX_TYPE type = (CTexture::TEX_TYPE)aStageBillboard[nCntStageBillboard].nTexType;

					// ビルボードを生成
					aStageBillboard[nCntStageBillboard].pBillboard = CBillboard::Create
					(m_pos + aStageBillboard[nCntStageBillboard].pos,
						D3DXVECTOR3(aStageBillboard[nCntStageBillboard].size.x, aStageBillboard[nCntStageBillboard].size.y, 0.0f),
						type, PRIORITY_EFFECT);
					if (type == CTexture::TEX_TYPE_TREE)
					{
						aStageBillboard[nCntStageBillboard].pBillboard->SetShadow(true);
					}
					else if (type == CTexture::TEX_TYPE_BUSH_00)
					{
						aStageBillboard[nCntStageBillboard].pBillboard->SetShadow(true);
					}


					while (fgets(cString, MAX_STRING, pFile) != NULL)
					{
						// 文字列を保存
						fscanf(pFile, "%s", cString);

						// 文字列の中に"END_BILLBOARDSET"があったらそこで終了
						if (strncmp("END_BILLBOARDSET", cString, 17) == 0)
						{
							break;
						}
					}
				}

				break;
			}

			while (fgets(cString, MAX_STRING, pFile) != NULL)
			{
				// 文字列を保存
				fscanf(pFile, "%s", cString);

				// 文字列の中に"END_OBJECTSET"があったらそこでモーションの取得を終了
				if (strncmp("END_OBJECTSET", cString, 14) == 0)
				{
					break;
				}
			}
		}
	}

	// ファイルを閉じる
	fclose(pFile);
}

//=============================================================================
// オブジェクトの全体消去処理
//=============================================================================
void CStageData::DeleteObject()
{
	for (int nCntData = 0; nCntData < MAX_STAGE_OBJECT; nCntData++)
	{
		if (aStageModel[nCntData].pModelSingle != NULL)
		{
			// モデルを消去
			aStageModel[nCntData].pModelSingle->Uninit();
			delete aStageModel[nCntData].pModelSingle;
			aStageModel[nCntData].pModelSingle = NULL;
		}
	}

	for (int nCntData = 0; nCntData < MAX_STAGE_OBJECT; nCntData++)
	{
		if (aStageBillboard[nCntData].pBillboard != NULL)
		{
			// ビルボードを消去
			aStageBillboard[nCntData].pBillboard->DeleteShadow();
			aStageBillboard[nCntData].pBillboard->Uninit();
			delete aStageBillboard[nCntData].pBillboard;
			aStageBillboard[nCntData].pBillboard = NULL;

		}
	}
}