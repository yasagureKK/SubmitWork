//=============================================================================
// ステージデータ [data_stage.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "data_stage.h"
#include "manager.h"
#include "texture.h"
#include "model_single.h"
#include "mesh_field.h"
#include "mesh_wall.h"
#include "mesh_sky.h"
#include "enemy.h"
#include "texture.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXT_FILE_PATH "data/TEXT/stage_data_"	// 基本となるテキストファイルのパス

//=============================================================================
// コンストラクタ
//=============================================================================
CDataStage::CDataStage()
{
	// 変数のクリア
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nType = 0;
	m_nRow = 0;
	m_nLine = 0;
	m_nMaxStage = 0;
	m_pMeshSky = NULL;
	m_type = STAGE_TYPE_GAME;
}

//=============================================================================
// デストラクタ
//=============================================================================
CDataStage::~CDataStage()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CDataStage::Init(D3DXVECTOR3 pos)
{
	WIN32_FIND_DATA win32fd;	// ファイルの情報
	HANDLE hFind;				// ハンドル

	// 検索するデータファイルのパス
	hFind = FindFirstFile("data\\TEXT\\stage_data*", &win32fd);

	// 検索が失敗しなかったら 
	if (hFind != INVALID_HANDLE_VALUE)
	{
		// ファイルの数を取得
		do
		{
			// ステージの最大数を増やす
			m_nMaxStage++;
		} while (FindNextFile(hFind, &win32fd));

		// ファイルを閉じる
		FindClose(hFind);
	}

	// ステージのロード
	LoadStage((int)m_type);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CDataStage::Uninit(void)
{
	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CDataStage::Update(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void CDataStage::Draw(void)
{

}

//=============================================================================
// 生成処理
//=============================================================================
CDataStage *CDataStage::Create(D3DXVECTOR3 pos, STAGE_TYPE type)
{
	// クラスの生成
	CDataStage *pStageData = NULL;
	pStageData = new CDataStage;

	if (pStageData != NULL)
	{
		// 変数の設定
		pStageData->m_type = type;

		// 初期化処理
		pStageData->Init(pos);
	}
	return pStageData;
}

//=============================================================================
// ステージロード処理
//=============================================================================
void CDataStage::LoadStage(int nNum)
{
	char aFillName[128];
	char aChar[128];
	int nStageNum = nNum + 1;

	// ファイル名に基本となるパスの文字列を入れる
	strcpy(&aFillName[0], TEXT_FILE_PATH);

	// ステージの番号を文字列にして保存し、パスの文字列とステージ番号を連結
	sprintf(&aChar[0], "%d", nStageNum);
	strcat(&aFillName[0], &aChar[0]);

	// ".txt"の文字列を保存し、連結してパスが完成
	sprintf(&aChar[0], ".txt");
	strcat(&aFillName[0], &aChar[0]);

	// テキストファイルの読み込み
	char cString[256];
	FILE *pFile = fopen(&aFillName[0], "r");

	if (pFile != NULL)
	{
		int nMaxField = 0, nMaxWall = 0, nMaxModel = 0, nMaxEnemy = 0;
		int nCntField = 0, nCntWall = 0, nCntModel = 0, nCntEnemy = 0;

		while (1)
		{
			fscanf(pFile, "%s", &cString[0]);

			/************/
			/* 空の配置 */
			/************/
			if (strcmp(&cString[0], "SKYSET") == 0)	//SKYSETの文字列
			{
				int nTexType = 0;
				float fSize = 0.0f, fSpin = 0.0f;

				while (1)
				{
					fscanf(pFile, "%s", &cString[0]);

					if (strcmp(&cString[0], "TEXTYPE") == 0) //テクスチャの種類
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%d", &nTexType);
					}
					if (strcmp(&cString[0], "SIZE") == 0) //サイズ
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%f", &fSize);
					}
					if (strcmp(&cString[0], "SPIN") == 0) //回転数
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%f", &fSpin);
					}
					if (strcmp(&cString[0], "END_SKYSET") == 0)
					{
						break;
					}
				}

				// メッシュスフィアを生成
				//※今回、フォグの影響で背景が見えないため空は生成しない
				//CMeshSky::Create(D3DXVECTOR3(0.0f, -1000.0f, 0.0f), D3DXVECTOR3(fSize, 0.0f, fSize), fSpin, nTexType);
			}

			// それぞれのオブジェクトの最大数を取得
			if (strcmp(&cString[0], "NUM_FIELD") == 0)
			{
				fscanf(pFile, "%s%d", cString, &nMaxField);
			}
			if (strcmp(&cString[0], "NUM_WALL") == 0)
			{
				fscanf(pFile, "%s%d", cString, &nMaxWall);
			}
			if (strcmp(&cString[0], "NUM_MODEL") == 0)
			{
				fscanf(pFile, "%s%d", cString, &nMaxModel);
			}
			if (strcmp(&cString[0], "NUM_ENEMY") == 0)
			{
				fscanf(pFile, "%s%d", cString, &nMaxEnemy);
			}

			/**************/
			/* 地面の配置 */
			/**************/
			if (strcmp(&cString[0], "FIELDSET") == 0 && nCntField < nMaxField)
			{
				int nTexType = 0, nRow = 0, nLine = 0;
				D3DXVECTOR3 pos, rot, size;

				while (1)
				{
					fscanf(pFile, "%s", &cString[0]);

					if (strcmp(&cString[0], "TEXTYPE") == 0) //テクスチャの種類
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%d", &nTexType);
					}
					if (strcmp(&cString[0], "POS") == 0) //位置
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%f%f%f", &pos.x, &pos.y, &pos.z);
					}
					if (strcmp(&cString[0], "ROT") == 0) //向き
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%f%f%f", &rot.x, &rot.y, &rot.z);
					}
					if (strcmp(&cString[0], "BLOCK") == 0) //分割数
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%d%d", &nLine, &nRow);
					}
					if (strcmp(&cString[0], "SIZE") == 0) //サイズ
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%f%f", &size.x, &size.z);
						size.y = 0.0f;
					}
					if (strcmp(&cString[0], "END_FIELDSET") == 0)
					{
						break;
					}
				}

				// 地面の生成
				CMeshField::Create(pos, size, rot, nRow, nLine, nTexType);

				nCntField++;
			}

			/************/
			/* 壁の配置 */
			/************/
			if (strcmp(&cString[0], "WALLSET") == 0 && nCntWall < nMaxWall)
			{
				int nTexType = 0, nRow = 0, nLine = 0;
				D3DXVECTOR3 pos, rot, size;

				// モデルのパーツ数だけ回す
				while (1)
				{
					fscanf(pFile, "%s", &cString[0]);

					if (strcmp(&cString[0], "TEXTYPE") == 0) //テクスチャの種類
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%d", &nTexType);
					}
					if (strcmp(&cString[0], "POS") == 0) //位置
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%f%f%f", &pos.x, &pos.y, &pos.z);
					}
					if (strcmp(&cString[0], "ROT") == 0) //向き
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%f%f%f", &rot.x, &rot.y, &rot.z);
					}
					if (strcmp(&cString[0], "BLOCK") == 0) //分割数
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%d%d", &nRow, &nLine);
					}
					if (strcmp(&cString[0], "SIZE") == 0) //サイズ
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%f%f", &size.x, &size.y);
						size.z = 0.0f;
					}
					if (strcmp(&cString[0], "END_WALLSET") == 0)
					{
						break;
					}
				}

				// 壁の生成
				CMeshWall::Create(pos, size, rot, nRow, nLine, nTexType);

				nCntWall++;
			}

			/****************/
			/* モデルの配置 */
			/****************/
			if (strcmp(&cString[0], "MODELSET") == 0 && nCntModel < nMaxModel)
			{
				int nType = 0;
				D3DXVECTOR3 pos, rot;

				// モデルのパーツ数だけ回す
				while (1)
				{
					fscanf(pFile, "%s", &cString[0]);

					if (strcmp(&cString[0], "TYPE") == 0) //種類
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%d", &nType);
					}
					if (strcmp(&cString[0], "POS") == 0) //位置
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%f%f%f", &pos.x, &pos.y, &pos.z);
					}
					if (strcmp(&cString[0], "ROT") == 0) //向き
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%f%f%f", &rot.x, &rot.y, &rot.z);
					}
					if (strcmp(&cString[0], "END_MODELSET") == 0)
					{
						break;
					}
				}

				// モデルの生成
				CModelSingle::Create(pos, rot, nType);

				nCntModel++;
			}

			/************/
			/* 敵の配置 */
			/************/
			if (strcmp(&cString[0], "ENEMYSET") == 0 && nCntEnemy < nMaxEnemy)
			{
				int nType = 0;
				D3DXVECTOR3 pos, rot;
				float fLife = 0.0f;

				// モデルのパーツ数だけ回す
				while (1)
				{
					fscanf(pFile, "%s", &cString[0]);

					if (strcmp(&cString[0], "TYPE") == 0) //インデックス番号
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%d", &nType);
					}
					if (strcmp(&cString[0], "POS") == 0) //位置
					{
						// 位置を取得する
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%f%f%f", &pos.x, &pos.y, &pos.z);
					}
					if (strcmp(&cString[0], "ROT") == 0) //向き
					{
						// 向きを取得する
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%f%f%f", &rot.x, &rot.y, &rot.z);
					}
					if (strcmp(&cString[0], "LIFE") == 0) //ライフ
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%f", &fLife);
					}
					if (strcmp(&cString[0], "END_ENEMYSET") == 0)
					{
						break;
					}
				}

				// int型をenum型にキャスト
				CEnemy::ENEMY_TYPE type = (CEnemy::ENEMY_TYPE)nType;

				// 敵の生成
				CEnemy::Create(pos, rot, type, fLife);
				nCntEnemy++;
			}

			if (strcmp(&cString[0], "END_SCRIPT") == 0) // END_SCRIPTの文字列
			{
				break;
			}
		}

		// ファイルを閉じる
		fclose(pFile);
	}
}