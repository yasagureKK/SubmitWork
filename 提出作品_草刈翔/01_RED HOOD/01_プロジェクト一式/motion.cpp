//=============================================================================
// モーション処理 [motion.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "motion.h"
#include "player.h"
#include "enemy.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MOTION_PLAYER_FILE	"data/TEXT/motion_player.txt"
#define MOTION_WOLF00_FILE	"data/TEXT/motion_wolf00.txt"
#define MOTION_WOLF01_FILE	"data/TEXT/motion_wolf01.txt"
#define MOTION_BOSS_FILE	"data/TEXT/motion_boss.txt"

//=============================================================================
// コンストラクタ
//=============================================================================
CMotion::CMotion(void)
{
	// 変数のクリア
	for (int nCntMotion = 0; nCntMotion < MAX_MOTION_NUM; nCntMotion++)
	{
		m_aInfo[nCntMotion].nLoop = 0;
		m_aInfo[nCntMotion].nNumKey = 0;
		for (int nCntKey = 0; nCntKey < MAX_KEY_NUM; nCntKey++)
		{
			m_aInfo[nCntMotion].aKeyInfo[nCntKey].nFrame = 0;
			for (int nCntModel = 0; nCntModel < MAX_MODEL_NUM; nCntModel++)
			{
				m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
		}
	}

	m_nMaxModelNum = 0;
	m_nMaxMotionNum = 0;
	m_nNumKey = 0;
	m_nKey = 0;
	m_nKeyOld = 0;
	m_nType = 0;
	m_nTypeNext = 0;
	m_fCounter = 0;
	m_bConnect = false;
	m_bStop = false;
	m_type = MOTION_TYPE_PLAYER;
}

//=============================================================================
// デストラクタ
//=============================================================================
CMotion::~CMotion()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMotion::Init(CScene *pScene, MOTION_TYPE type)
{
	// 引数のポインタをプレイヤークラスのポインタにキャスト
	CPlayer *pPlayer = NULL;
	CEnemy *pEnemy = NULL;

	// テキストファイルの読み込み
	char cString[256];
	FILE *pFile = NULL;

	m_type = type;
	switch (m_type)
	{
	case MOTION_TYPE_PLAYER:
		// プレイヤークラスのポインタにキャスト
		pPlayer = (CPlayer*)pScene;
		pFile = fopen(MOTION_PLAYER_FILE, "r");
		break;

	case MOTION_TYPE_WOLF00:
		// エネミークラスのポインタにキャスト
		pEnemy = (CEnemy*)pScene;
		pFile = fopen(MOTION_WOLF00_FILE, "r");
		break;

	case MOTION_TYPE_WOLF01:
		// エネミークラスのポインタにキャスト
		pEnemy = (CEnemy*)pScene;
		pFile = fopen(MOTION_WOLF01_FILE, "r");
		break;

	case MOTION_TYPE_BOSS:
		// エネミークラスのポインタにキャスト
		pEnemy = (CEnemy*)pScene;
		pFile = fopen(MOTION_BOSS_FILE, "r");
		break;
	default:
		break;
	}

	int nKey = 0, nModel = 0;
	bool bMotion = false;

	if (pFile != NULL)
	{
		while (1)
		{
			fscanf(pFile, "%s", &cString[0]);

			// モデルの最大数を取得
			if (strcmp(&cString[0], "NUM_MODEL") == 0)
			{
				fscanf(pFile, "%s", &cString[0]);
				fscanf(pFile, "%d", &m_nMaxModelNum);
			}

			if (strcmp(&cString[0], "MOTIONSET") == 0) // MOTIONSETの文字列
			{
				bMotion = true;
			}
			if (strcmp(&cString[0], "END_MOTIONSET") == 0)	// END_MOTIONSETの文字列
			{
				m_nMaxMotionNum++;
				bMotion = false;
			}

			if (bMotion == true)
			{
				if (strcmp(&cString[0], "LOOP") == 0) //ループするかどうか [0 : ループしない / 1 : ループする]
				{
					fscanf(pFile, "%s", &cString[0]);
					fscanf(pFile, "%d", &m_aInfo[m_nMaxMotionNum].nLoop);
				}
				if (strcmp(&cString[0], "NUM_KEY") == 0) //キー数
				{
					fscanf(pFile, "%s", &cString[0]);
					fscanf(pFile, "%d", &m_aInfo[m_nMaxMotionNum].nNumKey);
				}

				if (strcmp(&cString[0], "KEYSET") == 0) // KEYSETの文字列
				{
					// キー数ぶん回す
					while (1)
					{
						fscanf(pFile, "%s", &cString[0]);

						if (strcmp(&cString[0], "FRAME") == 0) //フレーム数
						{
							// フレーム数を取得する
							fscanf(pFile, "%s", &cString[0]);
							fscanf(pFile, "%d", &m_aInfo[m_nMaxMotionNum].aKeyInfo[nKey].nFrame);

							// モデル数ぶん回す
							while (1)
							{
								fscanf(pFile, "%s", &cString[0]);

								if (strcmp(&cString[0], "POS") == 0) //位置
								{
									// 位置を取得する
									fscanf(pFile, "%s", &cString[0]);
									fscanf(pFile, "%f%f%f", &m_aInfo[m_nMaxMotionNum].aKeyInfo[nKey].aKey[nModel].pos.x,
										&m_aInfo[m_nMaxMotionNum].aKeyInfo[nKey].aKey[nModel].pos.y,
										&m_aInfo[m_nMaxMotionNum].aKeyInfo[nKey].aKey[nModel].pos.z);
								}
								if (strcmp(&cString[0], "ROT") == 0) //向き
								{
									// 向きを取得する
									fscanf(pFile, "%s", &cString[0]);
									fscanf(pFile, "%f%f%f", &m_aInfo[m_nMaxMotionNum].aKeyInfo[nKey].aKey[nModel].rot.x,
										&m_aInfo[m_nMaxMotionNum].aKeyInfo[nKey].aKey[nModel].rot.y,
										&m_aInfo[m_nMaxMotionNum].aKeyInfo[nKey].aKey[nModel].rot.z);

									nModel++;
								}

								// モデルの最大数まで読み込んだら
								if (nModel >= m_nMaxModelNum)
								{
									nModel = 0;
									break;
								}
							}

							nKey++;
						}

						// キーの最大数まで読み込んだら
						if (nKey >= m_aInfo[m_nMaxMotionNum].nNumKey)
						{
							nKey = 0;
							break;
						}
					}
				}
			}

			if (strcmp(&cString[0], "END_SCRIPT") == 0) //終わり
			{
				break;
			}
		}
		fclose(pFile);
	}

	// 親子関係による位置ずれの修正
	for (int nCntMotion = 0; nCntMotion < m_nMaxMotionNum; nCntMotion++)
	{
		for (int nCntKey = 0; nCntKey < m_aInfo[nCntMotion].nNumKey; nCntKey++)
		{
			for (int nCntModel = 0; nCntModel < m_nMaxModelNum; nCntModel++)
			{
				// モデルの位置と向きを取得
				D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				switch (m_type)
				{
				case MOTION_TYPE_PLAYER:
					pos = pPlayer->GetModelPos(nCntModel);
					rot = pPlayer->GetModelRot(nCntModel);
					break;

				default:
					pos = pEnemy->GetModelPos(nCntModel);
					rot = pEnemy->GetModelRot(nCntModel);
					break;
				}

				m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].pos.x += pos.x;
				m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].pos.y += pos.y;
				m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].pos.z += pos.z;
				m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].rot.x += rot.x;
				m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].rot.y += rot.y;
				m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].rot.z += rot.z;
			}
		}
	}

	// 変数の初期化
	m_nNumKey = 0;
	m_nKey = 0;
	m_nKeyOld = 0;
	m_nType = 0;
	m_nTypeNext = 0;
	m_fCounter = 0;
	m_bConnect = false;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CMotion::Uninit(void)
{
	delete this;
}

//=============================================================================
// 更新処理
//=============================================================================
void CMotion::Update(CScene *pScene)
{
	CPlayer *pPlayer = NULL;
	CEnemy *pEnemy = NULL;
	switch (m_type)
	{
	case MOTION_TYPE_PLAYER:
		pPlayer = (CPlayer*)pScene;
		break;
	default:
		pEnemy = (CEnemy*)pScene;
		break;
	}

	D3DXVECTOR3 posAsk = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 求めたい位置の値
	D3DXVECTOR3 rotAsk = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 求めたい向きの値
	D3DXVECTOR3 posDiffer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置の差分
	D3DXVECTOR3 rotDiffer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向きの差分
	int nKeyNext = 0;			// 次のキー

								// 現在のキーが最大値以上になったらキーを最初に戻す
	if (m_nKey >= m_aInfo[m_nType].nNumKey - 1)
	{
		nKeyNext = 0;

	}
	// 現在のキーが最大値より小さかったらキーを次に進める
	else
	{
		nKeyNext = m_nKey + 1;
	}

	// モーション結合中の場合
	if (m_bConnect == true)
	{
		// 次のキーを0にする
		nKeyNext = 0;
	}

	for (int nCntModel = 0; nCntModel < m_nMaxModelNum; nCntModel++)
	{
		// フレーム数
		int nFrame = 10;

		if (m_bConnect == true)
		{
			// ループするなら
			if (m_aInfo[m_nType].nLoop == 1)
			{
				nFrame = m_aInfo[m_nType].aKeyInfo[0].nFrame;
			}
		}
		else
		{
			nFrame = m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame;
		}

		// モーションをつなげる場合
		if (m_bConnect == true)
		{
			// モデルの位置と向きを取得
			D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			switch (m_type)
			{
			case MOTION_TYPE_PLAYER:
				pos = pPlayer->GetModelPos(nCntModel);
				rot = pPlayer->GetModelRot(nCntModel);
				break;

			default:
				pos = pEnemy->GetModelPos(nCntModel);
				rot = pEnemy->GetModelRot(nCntModel);
				break;
			}

			// 現在のキーと次のキーの位置の差分を求める
			posDiffer.x = m_aInfo[m_nTypeNext].aKeyInfo[0].aKey[nCntModel].pos.x - pos.x;
			posDiffer.y = m_aInfo[m_nTypeNext].aKeyInfo[0].aKey[nCntModel].pos.y - pos.y;
			posDiffer.z = m_aInfo[m_nTypeNext].aKeyInfo[0].aKey[nCntModel].pos.z - pos.z;

			// 現在のキーと次のキーの回転の差分を求める
			rotDiffer.x = m_aInfo[m_nTypeNext].aKeyInfo[0].aKey[nCntModel].rot.x - rot.x;
			rotDiffer.y = m_aInfo[m_nTypeNext].aKeyInfo[0].aKey[nCntModel].rot.y - rot.y;
			rotDiffer.z = m_aInfo[m_nTypeNext].aKeyInfo[0].aKey[nCntModel].rot.z - rot.z;

			// 現在のキーから次のキーに動かした先の位置を求める
			posAsk.x = pos.x + posDiffer.x * (m_fCounter / nFrame);
			posAsk.y = pos.y + posDiffer.y * (m_fCounter / nFrame);
			posAsk.z = pos.z + posDiffer.z * (m_fCounter / nFrame);

			// 現在のキーから次のキーに動かした先の向きを求める
			rotAsk.x = rot.x + rotDiffer.x * (m_fCounter / nFrame);
			rotAsk.y = rot.y + rotDiffer.y * (m_fCounter / nFrame);
			rotAsk.z = rot.z + rotDiffer.z * (m_fCounter / nFrame);
		}
		// モーションをつなげない場合
		else
		{
			// 現在のキーと次のキーの位置の差分を求める
			posDiffer.x = m_aInfo[m_nTypeNext].aKeyInfo[nKeyNext].aKey[nCntModel].pos.x - m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].pos.x;
			posDiffer.y = m_aInfo[m_nTypeNext].aKeyInfo[nKeyNext].aKey[nCntModel].pos.y - m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].pos.y;
			posDiffer.z = m_aInfo[m_nTypeNext].aKeyInfo[nKeyNext].aKey[nCntModel].pos.z - m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].pos.z;

			// 現在のキーと次のキーの回転の差分を求める
			rotDiffer.x = m_aInfo[m_nTypeNext].aKeyInfo[nKeyNext].aKey[nCntModel].rot.x - m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].rot.x;
			rotDiffer.y = m_aInfo[m_nTypeNext].aKeyInfo[nKeyNext].aKey[nCntModel].rot.y - m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].rot.y;
			rotDiffer.z = m_aInfo[m_nTypeNext].aKeyInfo[nKeyNext].aKey[nCntModel].rot.z - m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].rot.z;

			// 向きの差分を3.14から-3.14の値の範囲内に収める
			if (rotDiffer.x < -D3DX_PI)
			{
				rotDiffer.x += D3DX_PI * 2.0f;
			}
			else if (rotDiffer.x > D3DX_PI)
			{
				rotDiffer.x -= D3DX_PI * 2.0f;
			}

			if (rotDiffer.y < -D3DX_PI)
			{
				rotDiffer.y += D3DX_PI * 2.0f;
			}
			else if (rotDiffer.y > D3DX_PI)
			{
				rotDiffer.y -= D3DX_PI * 2.0f;
			}

			if (rotDiffer.z < -D3DX_PI)
			{
				rotDiffer.z += D3DX_PI * 2.0f;
			}
			else if (rotDiffer.z > D3DX_PI)
			{
				rotDiffer.z -= D3DX_PI * 2.0f;
			}

			// 現在のキーから次のキーに動かした先の位置を求める
			posAsk.x = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].pos.x + posDiffer.x * (m_fCounter / nFrame);
			posAsk.y = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].pos.y + posDiffer.y * (m_fCounter / nFrame);
			posAsk.z = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].pos.z + posDiffer.z * (m_fCounter / nFrame);

			// 現在のキーから次のキーに動かした先の向きを求める
			rotAsk.x = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].rot.x + rotDiffer.x * (m_fCounter / nFrame);
			rotAsk.y = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].rot.y + rotDiffer.y * (m_fCounter / nFrame);
			rotAsk.z = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].rot.z + rotDiffer.z * (m_fCounter / nFrame);
		}

		// モデルの位置と向きに反映
		switch (m_type)
		{
		case MOTION_TYPE_PLAYER:
			pPlayer->SetModelPos(nCntModel, posAsk);
			pPlayer->SetModelRot(nCntModel, rotAsk);
			break;

		default:
			pEnemy->SetModelPos(nCntModel, posAsk);
			pEnemy->SetModelRot(nCntModel, rotAsk);
			break;
		}
	}

	if (m_type == MOTION_TYPE_PLAYER)
	{
		if (pPlayer->GetLand() == true)
		{
			m_bStop = false;
		}
	}

	if (m_bStop == false)
	{
		// カウンターを加算
		m_fCounter += 1.0f;
	}

	// モーション結合中ではない場合
	if (m_bConnect == false)
	{
		// カウンターがフレーム数の最大値を超えたら
		if (m_fCounter >= m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame)
		{
			// キーを進める
			m_nKey++;

			// 現在のキーが(キー数 - 1)以上だったら
			if (m_nKey >= m_aInfo[m_nType].nNumKey - 1)
			{
				// ループしないモーションのとき
				if (m_aInfo[m_nType].nLoop == 0)
				{
					if (m_type == MOTION_TYPE_PLAYER)
					{
						// ジャンプモーション
						if (m_nType == 2)
						{
							if (pPlayer->GetLand() == false)
							{
								m_bStop = true;
							}
						}
						// クリアモーション
						else if (m_nType == 11)
						{
							SetMotion(12);
						}
						else
						{
							SetMotion(0);
						}
					}
					else if (m_type == MOTION_TYPE_WOLF00 || m_type == MOTION_TYPE_WOLF01)
					{
						// ダウンモーション
						if (m_nType == 4)
						{
							m_bStop = true;
						}
						else
						{
							SetMotion(0);
						}
					}
					else if (m_type == MOTION_TYPE_BOSS)
					{
						// ダウンモーション
						if (m_nType == 3)
						{
							m_bStop = true;
						}
						// ジャンプ(溜め)モーション
						else if (m_nType == 4)
						{
							m_bStop = true;
						}
						// ジャンプモーション
						else if (m_nType == 5)
						{
							m_bStop = true;
						}
						else
						{
							SetMotion(0);
						}
					}
					else
					{
						// 次のモーションをニュートラルモーションにする
						SetMotion(0);
					}
				}
				//ループするモーションのとき
				else
				{
					// キーが最後までいったら0に戻してモーションを繰り返す
					if (m_nKey > m_aInfo[m_nType].nNumKey - 1)
					{
						m_nKey = 0;
					}
				}
			}

			// カウンターをリセット
			m_fCounter = 0.0f;
		}
	}
	// モーション結合中の場合
	else
	{
		// 10フレームかけてモーション移行
		int nMaxFrame = 10;

		if (m_fCounter >= nMaxFrame)
		{
			m_bConnect = false;
			m_fCounter = 0.0f;
			m_nKey = 0;
			//現在のモーションの種類を次のモーションの種類にする
			m_nType = m_nTypeNext;
		}
	}
}

//=============================================================================
// 生成処理
//=============================================================================
CMotion *CMotion::Create(CScene *pScene, MOTION_TYPE type)
{
	// クラスの生成
	CMotion *pMotion = NULL;
	pMotion = new CMotion;

	if (pMotion != NULL)
	{
		pMotion->Init(pScene, type);
	}
	return pMotion;
}

//=============================================================================
// モーション設定処理
//=============================================================================
void CMotion::SetMotion(int nType)
{
	m_nTypeNext = nType;
	m_bConnect = true;
	m_bStop = false;
	m_fCounter = 0.0f;

	// ■ プレイヤーのモーション
	/*　[0] ニュートラル
	/*	[1] 移動
	/*	[2] ジャンプ
	/*	[3] 着地
	/*	[4] ダッシュ
	/*	[5] 攻撃：斧 (1段目)
	/*	[6] 攻撃：斧 (2段目)
	/*	[7] 攻撃：斧 (3段目)
	/*	[8] 攻撃：ボウガン
	/*	[9] 被ダメージ
	/*	[10]ダウン
	/*	[11]クリア
	/*	[12]クリア(待機)		*/

	// ■ 狼00のモーション
	/*　[0] ニュートラル
	/*	[1] 移動
	/*	[2] 攻撃
	/*	[3] 被ダメージ			
	/*	[4]	ダウン				*/

	// ■ 狼01のモーション
	/*　[0] ニュートラル
	/*	[1] 移動
	/*	[2] 攻撃
	/*	[3] 被ダメージ			
	/*	[4]	ダウン				*/

	// ■ ボスのモーション
	/*　[0] ニュートラル
	/*	[1] 移動
	/*	[2] 攻撃
	/*	[3] ダウン				
	/*	[4] ジャンプ(溜め)
	/*	[5] ジャンプ
	/*	[6] 着地
	/*	[7] チャージ
	/*	[8] ダッシュ			*/
}