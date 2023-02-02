//=============================================================================
// プレイヤーのモーション処理 [motion_player.h]
// Authore : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "motion_player.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CMotionPlayer::CMotionPlayer()
{
	for (int nCntMotion = 0; nCntMotion < MOTION_PLAYER_TYPE_MAX; nCntMotion++)
	{
		m_aInfo[nCntMotion].nLoop = 0;
		m_aInfo[nCntMotion].nNumKey = 0;
		for (int nCntKey = 0; nCntKey < MAX_MOTION_PLAYER_KEY_INFO; nCntKey++)
		{
			m_aInfo[nCntMotion].aKeyInfo[nCntKey].nFrame = 0;
			for (int nCntModel = 0; nCntModel < MAX_PLAYER_MODEL; nCntModel++)
			{
				m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
		}
	}

	// 変数のクリア
	m_type = MOTION_PLAYER_TYPE_NEUTRAL;
	m_typeNext = MOTION_PLAYER_TYPE_NEUTRAL;
	m_nNumKey = 0;
	m_nKeyOld = 0;
	m_fCounter = 0.0f;
	m_nKey = 0;
	m_bConnect = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CMotionPlayer::~CMotionPlayer()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMotionPlayer::Init(CPlayer *pPlayer)
{
	// テキストファイルの読み込み
	FILE *pfile = fopen(MOTION_PLAYER_FILE, "r");

	// テキスト保存用の変数
	char cString[MAX_STRING];

	// モーションの読み込み
	for (int nCntMotion = 0; nCntMotion < MOTION_PLAYER_TYPE_MAX; nCntMotion++)
	{
		// テキストファイルの最後(NULL)まで読み込む
		while (fgets(cString, MAX_STRING, pfile) != NULL)
		{
			// 文字列を保存
			fscanf(pfile, "%s", cString);

			// 文字列の中に"MOTIONSET"があったら
			if (strncmp("MOTIONSET", cString, 10) == 0)
			{
				//ループ情報とキー数を取得
				fscanf(pfile, "%s%*s%d%*s%*s%*s%*s", cString, &m_aInfo[nCntMotion].nLoop);
				fscanf(pfile, "%s%*s%d", cString, &m_aInfo[nCntMotion].nNumKey);
				break;
			}
		}

		// 取得したキー数ぶん繰り返す
		for (int nCntKey = 0; nCntKey < m_aInfo[nCntMotion].nNumKey; nCntKey++)
		{
			while (fgets(cString, MAX_STRING, pfile) != NULL)
			{
				// 文字列を保存
				fscanf(pfile, "%s", cString);

				// 文字列の中に"FRAME"があったら
				if (strncmp("FRAME", cString, 6) == 0)
				{
					// フレーム数を取得する
					fscanf(pfile, "%s%d", cString, &m_aInfo[nCntMotion].aKeyInfo[nCntKey].nFrame);
					break;
				}
			}

			// プレイヤーのモデルの数だけ繰り返す
			for (int nCntModel = 0; nCntModel < MAX_PLAYER_MODEL; nCntModel++)
			{
				while (fgets(cString, MAX_STRING, pfile) != NULL)
				{
					// 文字列を保存
					fscanf(pfile, "%s", cString);

					// 文字列の中に"POS"があったら
					if (strncmp("POS", cString, 4) == 0)
					{
						//位置・回転情報を取得
						fscanf(pfile, "%s%f%f%f", cString,
							&m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].pos.x,
							&m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].pos.y,
							&m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].pos.z);
						fscanf(pfile, "%s%*s%f%f%f%*s", cString,
							&m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].rot.x,
							&m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].rot.y,
							&m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].rot.z);
						break;
					}
				}
			}

			while (fgets(cString, MAX_STRING, pfile) != NULL)
			{
				// 文字列を保存
				fscanf(pfile, "%s", cString);

				// 文字列の中に"END_KEYSET"があったらそこでキー情報の取得を終了
				if (strncmp("END_KEYSET", cString, 11) == 0)
				{
					break;
				}
			}
		}

		while (fgets(cString, MAX_STRING, pfile) != NULL)
		{
			// 文字列を保存
			fscanf(pfile, "%s", cString);

			// 文字列の中に"END_MOTIONSET"があったらそこでモーションの取得を終了
			if (strncmp("END_MOTIONSET", cString, 14) == 0)
			{
				break;
			}
		}
	}

	// ファイルを閉じる
	fclose(pfile);

	// 親子関係による位置ずれの修正
	for (int nCntMotion = 0; nCntMotion < MOTION_PLAYER_TYPE_MAX; nCntMotion++)
	{
		// キー数ぶん繰り返す
		for (int nCntKey = 0; nCntKey < m_aInfo[nCntMotion].nNumKey; nCntKey++)
		{
			// プレイヤーのモデルの数だけ繰り返す
			for (int nCntModel = 0; nCntModel < MAX_PLAYER_MODEL; nCntModel++)
			{
				// モデルの位置と向きを取得
				D3DXVECTOR3 pos = pPlayer->GetModelPos(nCntModel);
				D3DXVECTOR3 rot = pPlayer->GetModelRot(nCntModel);

				m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].pos.x = pos.x + m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].pos.x;
				m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].pos.y = pos.y + m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].pos.y;
				m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].pos.z = pos.z + m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].pos.z;
				m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].rot.x = rot.x + m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].rot.x;
				m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].rot.y = rot.y + m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].rot.y;
				m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].rot.z = rot.z + m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].rot.z;
			}
		}
	}

	// 変数の初期化
	m_type = MOTION_PLAYER_TYPE_NEUTRAL;
	m_typeNext = MOTION_PLAYER_TYPE_NEUTRAL;
	m_fCounter = 0.0f;
	m_nKey = 0;
	m_bConnect = false;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CMotionPlayer::Uninit(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void CMotionPlayer::Update(CPlayer *pPlayer)
{
	D3DXVECTOR3 posAsk;		// 求めたい位置の値
	D3DXVECTOR3 rotAsk;		// 求めたい向きの値
	D3DXVECTOR3 posDiffer;	// 位置の差分
	D3DXVECTOR3 rotDiffer;	// 向きの差分
	int nKeyNext = 0;							// 次のキー

	// 現在のキーが最大値以上になったらキーを最初に戻す
	if (m_nKey >= m_aInfo[m_type].nNumKey - 1)
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

	for (int nCntModel = 0; nCntModel < MAX_PLAYER_MODEL; nCntModel++)
	{
		// フレーム数
		int nFrame = 0;
		if (m_bConnect == true)
		{
			nFrame = 10;

			// ループするなら
			if (m_aInfo[m_type].nLoop == 1)
			{
				nFrame = m_aInfo[m_type].aKeyInfo[0].nFrame;
			}
		}
		else
		{
			nFrame = m_aInfo[m_type].aKeyInfo[m_nKey].nFrame;
		}

		// モーションをつなげる場合
		if (m_bConnect == true)
		{
			// モデルの位置と向きを取得
			D3DXVECTOR3 pos = pPlayer->GetModelPos(nCntModel);
			D3DXVECTOR3 rot = pPlayer->GetModelRot(nCntModel);

			// 現在のキーと次のキーの位置の差分を求める
			posDiffer.x = m_aInfo[m_typeNext].aKeyInfo[0].aKey[nCntModel].pos.x - pos.x;
			posDiffer.y = m_aInfo[m_typeNext].aKeyInfo[0].aKey[nCntModel].pos.y - pos.y;
			posDiffer.z = m_aInfo[m_typeNext].aKeyInfo[0].aKey[nCntModel].pos.z - pos.z;

			// 現在のキーと次のキーの回転の差分を求める
			rotDiffer.x = m_aInfo[m_typeNext].aKeyInfo[0].aKey[nCntModel].rot.x - rot.x;
			rotDiffer.y = m_aInfo[m_typeNext].aKeyInfo[0].aKey[nCntModel].rot.y - rot.y;
			rotDiffer.z = m_aInfo[m_typeNext].aKeyInfo[0].aKey[nCntModel].rot.z - rot.z;

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
			posDiffer.x = m_aInfo[m_typeNext].aKeyInfo[nKeyNext].aKey[nCntModel].pos.x - m_aInfo[m_type].aKeyInfo[m_nKey].aKey[nCntModel].pos.x;
			posDiffer.y = m_aInfo[m_typeNext].aKeyInfo[nKeyNext].aKey[nCntModel].pos.y - m_aInfo[m_type].aKeyInfo[m_nKey].aKey[nCntModel].pos.y;
			posDiffer.z = m_aInfo[m_typeNext].aKeyInfo[nKeyNext].aKey[nCntModel].pos.z - m_aInfo[m_type].aKeyInfo[m_nKey].aKey[nCntModel].pos.z;

			// 現在のキーと次のキーの回転の差分を求める
			rotDiffer.x = m_aInfo[m_typeNext].aKeyInfo[nKeyNext].aKey[nCntModel].rot.x - m_aInfo[m_type].aKeyInfo[m_nKey].aKey[nCntModel].rot.x;
			rotDiffer.y = m_aInfo[m_typeNext].aKeyInfo[nKeyNext].aKey[nCntModel].rot.y - m_aInfo[m_type].aKeyInfo[m_nKey].aKey[nCntModel].rot.y;
			rotDiffer.z = m_aInfo[m_typeNext].aKeyInfo[nKeyNext].aKey[nCntModel].rot.z - m_aInfo[m_type].aKeyInfo[m_nKey].aKey[nCntModel].rot.z;

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
			posAsk.x = m_aInfo[m_type].aKeyInfo[m_nKey].aKey[nCntModel].pos.x + posDiffer.x * (m_fCounter / nFrame);
			posAsk.y = m_aInfo[m_type].aKeyInfo[m_nKey].aKey[nCntModel].pos.y + posDiffer.y * (m_fCounter / nFrame);
			posAsk.z = m_aInfo[m_type].aKeyInfo[m_nKey].aKey[nCntModel].pos.z + posDiffer.z * (m_fCounter / nFrame);

			// 現在のキーから次のキーに動かした先の向きを求める
			rotAsk.x = m_aInfo[m_type].aKeyInfo[m_nKey].aKey[nCntModel].rot.x + rotDiffer.x * (m_fCounter / nFrame);
			rotAsk.y = m_aInfo[m_type].aKeyInfo[m_nKey].aKey[nCntModel].rot.y + rotDiffer.y * (m_fCounter / nFrame);
			rotAsk.z = m_aInfo[m_type].aKeyInfo[m_nKey].aKey[nCntModel].rot.z + rotDiffer.z * (m_fCounter / nFrame);
		}

		// モデルの位置と向きに反映
		pPlayer->SetModelPos(nCntModel, posAsk);
		pPlayer->SetModelRot(nCntModel, rotAsk);
	}

	// カウンターを加算
	m_fCounter += 1.0f;

	// モーション結合中ではない場合
	if (m_bConnect == false)
	{
		// カウンターがフレーム数の最大値を超えたら
		if (m_fCounter >= m_aInfo[m_type].aKeyInfo[m_nKey].nFrame)
		{
			// キーを進める
			m_nKey++;

			// 現在のキーが(キー数 - 1)より小さかったら
			if (m_nKey < m_aInfo[m_type].nNumKey - 1)
			{
				// カウンターをリセット
				m_fCounter = 0.0f;
			}
			// 現在のキーが(キー数 - 1)以上だったら
			else if (m_nKey >= m_aInfo[m_type].nNumKey - 1)
			{
				// ループしないモーションのとき
				//※ループしないモーションは念のために全て書いておくこと
				if (m_aInfo[m_type].nLoop == 0)
				{
					// 着地モーション
					if (m_type == MOTION_PLAYER_TYPE_LANDING)
					{
						D3DXVECTOR3 move = pPlayer->GetMove();

						// 動いていないなら
						//※基準を0にすると少ない移動量でも一瞬だけ移動モーションに移行するため今回は範囲を設定
						if ((move.x <= 1.0f && move.x >= -1.0f) &&
							(move.z <= 1.0f && move.z >= -1.0f))
						{
							// 次のモーションをニュートラルモーションにする
							SetMotion(MOTION_PLAYER_TYPE_NEUTRAL);
						}
						// 動いているなら
						else
						{
							// 次のモーションを移動モーションにする
							SetMotion(MOTION_PLAYER_TYPE_MOVE);
						}
					}
					else if (m_type == MOTION_PLAYER_TYPE_1P_WIN)
					{
						// 次のモーションを勝利時待機モーションにする
						SetMotion(MOTION_PLAYER_TYPE_1P_WIN_STAND);
					}
					else if (m_type == MOTION_PLAYER_TYPE_2P_WIN)
					{
						// 次のモーションを勝利時待機モーションにする
						SetMotion(MOTION_PLAYER_TYPE_2P_WIN_STAND);
					}
					else
					{
						// 次のモーションをニュートラルモーションにする
						SetMotion(MOTION_PLAYER_TYPE_NEUTRAL);
					}
				}
				//ループするモーションのとき
				else
				{
					// キーが最後までいったら0に戻してモーションを繰り返す
					if (m_nKey > m_aInfo[m_type].nNumKey - 1)
					{
						m_nKey = 0;
					}
				}

				m_fCounter = 0.0f;
			}
		}
	}
	// モーション結合中の場合
	else
	{
		// フレーム数の最大値を超えたら
		float nMaxFrame = 10.0f;

		if (m_fCounter >= nMaxFrame)
		{
			m_bConnect = false;
			m_fCounter = 0.0f;
			m_nKey = 0;
			//現在のモーションの種類を次のモーションの種類にする
			m_type = m_typeNext;
		}
	}
}

//=============================================================================
// 生成処理
//=============================================================================
CMotionPlayer *CMotionPlayer::Create(CPlayer *pPlayer)
{
	CMotionPlayer *pMotionPlayer = NULL;

	if (pMotionPlayer == NULL)
	{
		pMotionPlayer = new CMotionPlayer;
		if (pMotionPlayer != NULL)
		{
			pMotionPlayer->Init(pPlayer);
		}
	}

	return pMotionPlayer;
}


//=============================================================================
// モーション設定処理
//=============================================================================
void CMotionPlayer::SetMotion(MOTION_PLAYER_TYPE type)
{
	m_typeNext = type;
	m_bConnect = true;
	m_fCounter = 0.0f;
}

//=============================================================================
// モーション取得処理
//=============================================================================
CMotionPlayer::MOTION_PLAYER_TYPE CMotionPlayer::GetMotion(void)
{
	return m_type;
}

//=============================================================================
// モーション結合取得処理
//=============================================================================
bool CMotionPlayer::GetConnect(void)
{
	return m_bConnect;
}