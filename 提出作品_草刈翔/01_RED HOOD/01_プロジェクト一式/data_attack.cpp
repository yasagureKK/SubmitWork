//=============================================================================
// 攻撃情報管理 [data_attack.cpp]
// Aouthor : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "data_attack.h"
#include "manager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ATTACK_DATA_FILE	"data/TEXT/attack_data.txt"

//=============================================================================
// コンストラクタ
//=============================================================================
CDataAttack::CDataAttack()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CDataAttack::~CDataAttack()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CDataAttack::Init(void)
{
	// テキストファイルの読み込み
	char cString[256];
	FILE *pFile = fopen(ATTACK_DATA_FILE, "r");

	int nCntAtk = 0;

	if (pFile != NULL)
	{
		while (1)
		{
			fscanf(pFile, "%s", &cString[0]);

			if (strcmp(&cString[0], "ATTACKSET") == 0) // MOTIONSETの文字列
			{
				while (1)
				{
					fscanf(pFile, "%s", &cString[0]);

					if (strcmp(&cString[0], "WAIT") == 0) //攻撃可能までのフレーム
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%d", &m_Data[nCntAtk].nWait);
					}
					if (strcmp(&cString[0], "BEGIN_COLLISION") == 0) //コリジョンをつけるフレーム
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%d", &m_Data[nCntAtk].nBeginCollision);
					}
					if (strcmp(&cString[0], "BEGIN_EFFECT") == 0) //エフェクトをつけるフレーム
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%d", &m_Data[nCntAtk].nBeginEffect);
					}
					if (strcmp(&cString[0], "TIME_COLLISION") == 0) //コリジョンの持続時間
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%f", &m_Data[nCntAtk].fTimeCollision);
					}
					if (strcmp(&cString[0], "TIME_EFFECT") == 0) //エフェクトの持続時間
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%f", &m_Data[nCntAtk].fTimeEffect);
					}
					if (strcmp(&cString[0], "DAMAGE") == 0) //ダメージ倍率
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%f", &m_Data[nCntAtk].fDamage);
					}

					if (strcmp(&cString[0], "END_ATTACKSET") == 0)	// END_MOTIONSETの文字列
					{
						nCntAtk++;
						break;
					}
				}
			}

			if (strcmp(&cString[0], "END_SCRIPT") == 0) //終わり
			{
				break;
			}
		}
		// ファイルを閉じる
		fclose(pFile);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CDataAttack::Uninit(void)
{
	
}

//=============================================================================
// 生成処理
//=============================================================================
CDataAttack *CDataAttack::Create(void)
{
	// クラスの生成
	CDataAttack *pAttackData = NULL;
	pAttackData = new CDataAttack;
	
	if (pAttackData != NULL)
	{
		// 初期化処理
		pAttackData->Init();
	}
	return pAttackData;
}

//=============================================================================
// 情報取得処理
//=============================================================================
float CDataAttack::GetAttackData(ATTACK_TYPE atkType, ATTACK_DATA_TYPE type)
{
	int nNumber = (int)atkType;

	// 指定された番号が攻撃の種類の最大数より少なかったら
	if (nNumber < MAX_ATTACK)
	{
		switch (type)
		{
		case ATTACK_DATA_TYPE_WAIT:
			return (float)m_Data[nNumber].nWait;
			break;

		case ATTACK_DATA_TYPE_BEGIN_COLLISION:
			return (float)m_Data[nNumber].nBeginCollision;
			break;

		case ATTACK_DATA_TYPE_BEGIN_EFFECT:
			return (float)m_Data[nNumber].nBeginEffect;
			break;

		case ATTACK_DATA_TYPE_TIME_COLLISION:
			return m_Data[nNumber].fTimeCollision;
			break;

		case ATTACK_DATA_TYPE_TIME_EFFECT:
			return m_Data[nNumber].fTimeEffect;
			break;

		case ATTACK_DATA_TYPE_DAMAGE:
			return m_Data[nNumber].fDamage;
			break;

		default:
			return false;
			break;
		}
	}
	else
	{
		return false;
	}
}