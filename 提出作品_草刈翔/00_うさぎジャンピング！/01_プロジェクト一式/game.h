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
#define GAME_MAX_MESH_FIELD	(7)						// メッシュフィールドの最大数
#define GAME_MAX_MESH_CYLINDER	(3)					// メッシュシリンダーの最大数
#define GAME_MAX_MESH_WALL	(6)						// メッシュウォールの最大数
#define GAME_MAX_UI	(3)								// UIの最大数
#define GAME_PLAYER_START (-5000.0f)				// プレイヤーのスタート位置座標X
#define GAME_UI_SCORE_POS_X (60.0f)					// スコアの位置座標X
#define GAME_UI_SCORE_POS_Y (130.0f)				// スコアの位置座標Y
#define GAME_UI_SCORE_SENTENCE_POS_X (150.0f)		// スコア文字の位置座標X
#define GAME_UI_SCORE_SENTENCE_POS_Y (50.0f)		// スコア文字の位置座標Y
#define GAME_UI_SCORE_SENTENCE_SIZE_X (250.0f)		// スコア文字のサイズX
#define GAME_UI_SCORE_SENTENCE_SIZE_Y (90.0f)		// スコア文字のサイズY
#define GAME_UI_HIGHSCORE_POS_X (1000.0f)			// ハイスコアの位置座標X
#define GAME_UI_HIGHSCORE_POS_Y (120.0f)			// ハイスコアの位置座標Y
#define GAME_UI_HIGHSCORE_SENTENCE_POS_X (1050.0f)	// ハイスコア文字の位置座標X
#define GAME_UI_HIGHSCORE_SENTENCE_POS_Y (50.0f)	// ハイスコア文字の位置座標Y
#define GAME_UI_HIGHSCORE_SENTENCE_SIZE_X (450.0f)	// ハイスコア文字のサイズX
#define GAME_UI_HIGHSCORE_SENTENCE_SIZE_Y (80.0f)	// ハイスコア文字のサイズY
#define GAME_UI_JUMP_GUIDE_POS_X (570.0f)			// ジャンプガイドの位置座標X
#define GAME_UI_JUMP_GUIDE_POS_Y (110.0f)			// ジャンプガイドの位置座標Y
#define GAME_UI_COUNTDOWN_SIZE_X (300.0f)			// カウントダウンのサイズX
#define GAME_UI_COUNTDOWN_SIZE_Y (150.0f)			// カウントダウンのサイズY

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CUi;
class CPlayer;
class CMeshField;
class CMeshWall;
class CMeshSphere;
class CMeshCylinder;
class CScore;
class CJumpGuide;
class CPause;
class CStageData;
class CGameSignboard;
class CBillboardMoon;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CGame : public CScene
{
public:
	// ゲームのフェイズ
	typedef enum
	{
		GAME_PHASE_NONE = 0,	// なし
		GAME_PHASE_COUNTDOWN,	// スタート前のカウントダウン
		GAME_PHASE_ACTION,		// アクション中
		GAME_PHASE_ACTION_END,	// アクション終了
		GAME_PHASE_MAX
	} GAME_PHASE;

	CGame();												// コンストラクタ
	~CGame();												// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);		// 初期化処理
	void Uninit(void);										// 終了処理
	void Update(void);										// 更新処理
	void Draw(void);										// 描画処理
	void SetPhase(GAME_PHASE phase);						// フェイズ設定処理
	GAME_PHASE GetPhase(void);								// フェイズ取得処理

private:
	void Phase();											// フェイズ処理

	CUi *m_apUi[GAME_MAX_UI];								// UIのポインタ
	CPlayer *m_pPlayer;										// プレイヤーのポインタ
	CMeshField *m_pMeshField[GAME_MAX_MESH_FIELD];			// メッシュフィールドのポインタ
	CMeshWall *m_pMeshWall[GAME_MAX_MESH_WALL];				// メッシュウォールのポインタ
	CMeshSphere *m_pMeshSphere;								// メッシュスフィアのポインタ
	CMeshCylinder *m_pMeshCylinder[GAME_MAX_MESH_CYLINDER];	// メッシュシリンダーのポインタ
	CScore *m_pScore[2];									// スコアのポインタ
	CJumpGuide *m_pJumpGuide;								// ジャンプガイドのポインタ
	CPause *m_pPause;										// ポーズのポインタ
	CStageData *m_pStageData[2];							// ステージデータのポインタ
	CGameSignboard *m_pGameSignboard[2];					// 看板オブジェクトのポインタ
	CBillboardMoon *m_pBillboardMoon;						// 月のピルボードのポインタ
	GAME_PHASE m_phase;										// 現在のフェイズ
	int m_nInterval;										// フェイズ切り替え時のインターバル
};

#endif	// _GAME_H_