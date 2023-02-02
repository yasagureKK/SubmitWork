//=============================================================================
// プレイヤーのモーション処理 [motion_player.h]
// Authore : 草刈 翔
//=============================================================================
#ifndef _MOTION_PLAYER_H_
#define _MOTION_PLAYER_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_MOTION_PLAYER_KEY_INFO	(255)					// キーの情報の最大値
#define MOTION_PLAYER_FILE "data/TEXT/motion_player.txt"	// ファイルのパス

//*****************************************************************************
// 構造体の定義
//*****************************************************************************
//キーの構造体
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 rot;	//向き
} KEY;

//キー情報の構造体
typedef struct
{
	int nFrame;						//再生フレーム
	KEY aKey[MAX_PLAYER_MODEL];		//各モデルのキー要素(パーツの最大数)
} KEY_INFO;

//モーション情報の構造体
typedef struct
{
	int nLoop;				//ループするかどうか
	int nNumKey;			//キーの総数
	KEY_INFO aKeyInfo[MAX_MOTION_PLAYER_KEY_INFO];	//キーの情報
} MOTION_INFO;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CMotionPlayer
{
public:
	// モーションの種類
	typedef enum
	{
		MOTION_PLAYER_TYPE_NEUTRAL = 0,	// ニュートラル
		MOTION_PLAYER_TYPE_MOVE,		// 移動
		MOTION_PLAYER_TYPE_JUMP,		// ジャンプ
		MOTION_PLAYER_TYPE_JUMP_FALL,	// ジャンプ(落下中)
		MOTION_PLAYER_TYPE_LANDING,		// 着地
		MOTION_PLAYER_TYPE_DOUBLE_JUMP,	// 2段ジャンプ
		MOTION_PLAYER_TYPE_1P_WIN,		// 1P勝利アクション
		MOTION_PLAYER_TYPE_1P_WIN_STAND,// 1P勝利時待機
		MOTION_PLAYER_TYPE_1P_LOSE,		// 1P敗北
		MOTION_PLAYER_TYPE_2P_WIN,		// 2P勝利アクション
		MOTION_PLAYER_TYPE_2P_WIN_STAND,// 2P勝利時待機
		MOTION_PLAYER_TYPE_2P_LOSE,		// 2P敗北
		MOTION_PLAYER_TYPE_MAX
		//※テキストファイルのモーション数をこの種類の最大数に合わせること!!
	} MOTION_PLAYER_TYPE;

	CMotionPlayer();									// コンストラクタ
	~CMotionPlayer();									// デストラクタ
	HRESULT Init(CPlayer *pPlayer);						// 初期化処理
	void Uninit(void);									// 終了処理
	void Update(CPlayer *pPlayer);						// 更新処理
	static CMotionPlayer *Create(CPlayer *pPlayer);		// 生成処理
	void SetMotion(MOTION_PLAYER_TYPE type);			// モーション設定処理
	MOTION_PLAYER_TYPE GetMotion(void);					// モーション取得処理
	bool GetConnect(void);								// モーション結合取得処理

private:
	MOTION_INFO m_aInfo[MOTION_PLAYER_TYPE_MAX];		// モーション情報
	MOTION_PLAYER_TYPE m_type;							// 現在のモーションタイプ
	MOTION_PLAYER_TYPE m_typeNext;						// 次のモーションタイプ
	int m_nNumKey;										// キー数
	int m_nKey;											// 現在のキー
	int m_nKeyNext;										// 次のキー
	int m_nKeyOld;										// 1フレーム前のキー
	float m_fCounter;									// モーションのカウンター
	bool m_bConnect;									// モーション結合中かどうか
};

#endif // _MOTION_PLAYER_H_