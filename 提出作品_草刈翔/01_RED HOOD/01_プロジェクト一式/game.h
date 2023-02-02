//=============================================================================
// ゲーム画面 [game.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_LIGHT	(4)		// ライトの最大数
#define MAX_ENEMYBOX (4)	// エネミーボックスの最大数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CLight;
class CPlayer;
class CDataStage;
class CCollisionSphere;
class CEnemyBox;
class CPause;
class CFog;
class CScore;
class CTime;
class CUIStart;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CGame : public CScene
{
public:
	CGame();																// コンストラクタ
	~CGame();																// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos);											// 初期化処理
	void Uninit(void);														// 終了処理
	void Update(void);														// 更新処理
	void Draw(void);														// 描画処理
	static CGame *Create(void);												// 生成処理
	CPlayer *GetPlayer(void);												// プレイヤー取得処理
	static void SetDefeatBoss(bool bDefeat) { m_bDefeatBoss = bDefeat; }	// ボスを倒したか設定処理
	static bool GetDefeatBoss(void) { return m_bDefeatBoss; }				// ボスを倒したか取得処理
	void SetBossBattle(bool bDefeat);										// ボス戦かどうか設定処理
	bool GetBossBattle(void) { return m_bBossBattle; }						// ボス戦かどうか取得処理
	static bool GetClear(void) { return m_bClear; }							// クリアしたか取得処理

	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_NONE; }				// オブジェクトの種類の取得処理
	void SetPos(D3DXVECTOR3 pos) { pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	// 位置設定処理
	D3DXVECTOR3 GetPos(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); };		// 位置取得処理
	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// サイズ取得処理
	D3DXVECTOR3 GetPosOld(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	// 1フレーム前の位置取得処理
	D3DXVECTOR3 GetRot(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// 向き取得処理

private:
	CPlayer *m_pPlayer;						// プレイヤーのポインタ
	CDataStage *m_pDataStage;				// ステージデータのポインタ
	CEnemyBox *m_pEnemyBox[MAX_ENEMYBOX];	// エネミーボックスのポインタ
	CPause *m_pPause;						// ポーズのポインタ
	CFog *m_pFog;							// フォグのポインタ
	CScore *m_pScore;						// スコアのポインタ
	CTime *m_pTime;							// タイムのポインタ
	CLight *m_apLight[MAX_LIGHT];			// ライトのポインタ
	CUIStart *m_pStart;						// スタートUIのポインタ
	static bool m_bBossBattle;				// ボス戦かどうか
	static bool m_bDefeatBoss;				// ボスを倒したか
	static bool m_bClear;					// クリアしたか
	int nCntGame;							// スタート時やボス撃破後に使うカウント
};

#endif	// _GAME_H_