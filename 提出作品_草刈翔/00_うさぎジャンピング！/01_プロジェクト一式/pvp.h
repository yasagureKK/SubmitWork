//=============================================================================
// 2人用ゲーム画面 [pvp.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _PVP_H_
#define _PVP_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PVP_MAX_MODEL (100)			// モデルの最大数
#define PVP_MAX_MODEL_BOARD (10)	// 看板モデルの最大数
#define PVP_MAX_MESH_FIELD	(7)		// メッシュフィールドの最大数
#define PVP_MAX_MESH_CYLINDER (3)	// メッシュシリンダーの最大数
#define PVP_MAX_MESH_WALL (6)		// メッシュウォールの最大数
#define PVP_MAX_UI (6)				// UIの最大数
#define PVP_UI_1P_POS_X (110.0f)	// 1P表示の位置座標X
#define PVP_UI_1P_SIZE_X (190.0f)	// 1P表示のサイズX
#define PVP_UI_1P_SIZE_Y (90.0f)	// 1P表示のサイズY
#define PVP_2P_START (20000.0f)		// 2PのスタートZ座標

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
class CScene2D;
class CStageData;
class CGameSignboard;
class CBillboardMoon;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CPvP : public CScene
{
public:
	// 2人用ゲームのフェイズ
	typedef enum
	{
		PVP_PHASE_NONE = 0,		// なし
		PVP_PHASE_COUNTDOWN,	// スタート前のカウントダウン
		PVP_PHASE_ACTION,		// アクション中
		PVP_PHASE_ACTION_END,	// アクション終了
		PVP_PHASE_MAX
	} PVP_PHASE;

	CPvP();												// コンストラクタ
	~CPvP();											// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// 初期化処理
	void Uninit(void);									// 終了処理
	void Update(void);									// 更新処理
	void Draw(void);									// 描画処理
	void SetPhase(PVP_PHASE phase);						// フェイズ設定処理
	PVP_PHASE GetPhase(void);							// フェイズ取得処理

private:
	void Phase();										// フェイズ処理

	CUi *m_apUi[PVP_MAX_UI];												// UIのポインタ
	CPlayer *m_apPlayer[MAX_PLAYER_NUM];									// プレイヤーのポインタ
	CMeshField *m_pMeshField[MAX_PLAYER_NUM][PVP_MAX_MESH_FIELD];			// メッシュフィールドのポインタ
	CMeshWall *m_pMeshWall[MAX_PLAYER_NUM][PVP_MAX_MESH_WALL];				// メッシュウォールのポインタ
	CMeshSphere *m_pMeshSphere;												// メッシュスフィアのポインタ
	CMeshCylinder *m_pMeshCylinder[MAX_PLAYER_NUM][PVP_MAX_MESH_CYLINDER];	// メッシュシリンダーのポインタ
	CScore *m_pScore[3];													// スコアのポインタ
	CJumpGuide *m_pJumpGuide[MAX_PLAYER_NUM];								// ジャンプガイドのポインタ
	CPause *m_pPause;														// ポーズのポインタ
	CScene2D *m_pScene2D;													// 2Dオブジェクトのポインタ
	CStageData *m_pStageData[MAX_PLAYER_NUM][2];							// ステージデータのポインタ
	CGameSignboard *m_apGameSignboard[MAX_PLAYER_NUM][2];					// 看板オブジェクトのポインタ
	CBillboardMoon *m_apBillboardMoon[MAX_PLAYER_NUM];						// 月のピルボードのポインタ
	PVP_PHASE m_phase;														// 現在のフェイズ
	int m_nInterval;														// フェイズ切り替え時のインターバル
};

#endif	// _PVP_H_