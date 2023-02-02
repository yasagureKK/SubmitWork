//=============================================================================
// チュートリアル画面 [tutorial.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TUTORIAL_MAX_MODEL	(100)					// モデルの最大数
#define TUTORIAL_PLAYER_START (-5000.0f)			// プレイヤーのスタート位置座標X
#define TUTORIAL_MAX_MESH_FIELD	(7)					// メッシュフィールドの最大数
#define TUTORIAL_MAX_MESH_CYLINDER	(3)				// メッシュシリンダーの最大数
#define TUTORIAL_MAX_MESH_WALL	(6)					// メッシュウォールの最大数
#define TUTORIAL_MAX_UI	(4)							// UIの最大数
#define TUTORIAL_UI_SKIP_SENTENCE_POS_X (1180.0f)	// スキップ文字の位置座標X
#define TUTORIAL_UI_SKIP_SENTENCE_POS_Y (35.0f)		// スキップ文字の位置座標Y
#define TUTORIAL_UI_SKIP_SENTENCE_SIZE_X (270.0f)	// スキップ文字のサイズX
#define TUTORIAL_UI_SKIP_SENTENCE_SIZE_Y (60.0f)	// スキップ文字のサイズY

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
class CTutorialGuide;
class CScene2D;
class CTutorialPause;
class CStageData;
class CGameSignboard;
class CBillboardMoon;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CTutorial : public CScene
{
public:
	// チュートリアルのフェイズ
	typedef enum
	{
		TUTORIAL_PHASE_NONE = 0,	// なし
		TUTORIAL_PHASE_COUNTDOWN,	// スタート前のカウントダウン
		TUTORIAL_PHASE_ACTION,		// アクション中
		TUTORIAL_PHASE_ACTION_END,	// アクション終了
		TUTORIAL_PHASE_MAX
	} TUTORIAL_PHASE;

	CTutorial();												// コンストラクタ
	~CTutorial();												// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);			// 初期化処理
	void Uninit(void);											// 終了処理
	void Update(void);											// 更新処理
	void Draw(void);											// 描画処理
	void SetPhase(TUTORIAL_PHASE phase);						// フェイズ設定処理
	TUTORIAL_PHASE GetPhase(void);								// フェイズ取得処理
	static bool GetExplanation(void);							// 説明中かどうか取得処理

private:
	void TimeLine(void);										// タイムライン管理
	void Phase(void);											// フェイズ処理

	CUi *m_apUi[TUTORIAL_MAX_UI];								// UIのポインタ
	CPlayer *m_pPlayer;											// プレイヤーのポインタ
	CMeshField *m_pMeshField[TUTORIAL_MAX_MESH_FIELD];			// メッシュフィールドのポインタ
	CMeshWall *m_pMeshWall[TUTORIAL_MAX_MESH_WALL];				// メッシュウォールのポインタ
	CMeshSphere *m_pMeshSphere;									// メッシュスフィアのポインタ
	CMeshCylinder *m_pMeshCylinder[TUTORIAL_MAX_MESH_CYLINDER];	// メッシュシリンダーのポインタ
	CScore *m_pScore[2];										// スコアのポインタ
	CJumpGuide *m_pJumpGuide;									// ジャンプガイドのポインタ
	CTutorialGuide *m_pTutorialGuide;							// チュートリアルのガイドのポインタ
	CScene2D *m_pScene2D[2];									// 2Dオブジェクトのポインタ
	CTutorialPause *m_pTutorialPause;							// チュートリアルでのポーズのポインタ
	CStageData *m_pStageData[2];								// ステージデータのポインタ
	CGameSignboard *m_pGameSignboard[2];						// 看板オブジェクトのポインタ
	CBillboardMoon *m_pBillboardMoon;							// 月のピルボードのポインタ
	TUTORIAL_PHASE m_phase;										// 現在のフェイズ
	int m_nInterval;											// フェイズ切り替え時のインターバル
	int m_nTime;												// タイム
	int m_nPlayerCommandCount;									// プレイヤーに指示を出すために使うカウント
	static bool m_bStop;										// ストップしてるかどうか
	static bool m_bExplanation;									// 説明から始めるかどうか
};

#endif	// _TUTORIAL_H_