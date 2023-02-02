//=============================================================================
// ステージデータ [stage_data.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _STAGRE_DATA_
#define _STAGRE_DATA_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_STAGE_OBJECT	(64)						// オブジェクトの最大数
#define MAX_PATTERN			(4)							// 配置パターンの最大数
#define STAGE_DATA_FILE "data/TEXT/stage_data.txt"		// 読み込むファイルのパス

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CModelSingle;
class CBillboard;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CStageData : public CScene
{
public:
	// モデル情報の構造体
	typedef struct
	{
		int nType;						// モデルの種類
		D3DXVECTOR3 pos;				// 位置
		D3DXVECTOR3 rot;				// 向き
		CModelSingle *pModelSingle;		// モデル(単体)のポインタ
	} STAGE_MODEL;

	// ビルボード情報の構造体
	typedef struct
	{
		int nTexType;					// テクスチャの種類
		D3DXVECTOR3 pos;				// 位置
		D3DXVECTOR3 size;				// サイズ
		CBillboard *pBillboard;			// ビルボードのポインタ
	} STAGE_BILLBOARD;

	CStageData();										// コンストラクタ
	~CStageData();										// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// 初期化処理
	void Uninit(void);									// 終了処理
	void Update(void);									// 更新処理
	void Draw(void);									// 描画処理
	static CStageData *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 size, int nNumPlayer);				// 生成処理
	void LoadStage(int nPattern);						// ステージロード処理
	void DeleteObject(void);							// オブジェクトの全体消去処理

private:
	D3DXVECTOR3 m_pos;									// 位置
	D3DXVECTOR3 m_size;									// サイズ
	STAGE_MODEL aStageModel[MAX_STAGE_OBJECT];			// モデル
	STAGE_BILLBOARD aStageBillboard[MAX_STAGE_OBJECT];	// ビルボード
	static int m_nModelNum;								// モデルの数
	static int m_nBillboardNum;							// ビルボードの数
	int m_nNumPlayer;									// どのプレイヤーに対するものか
	int m_nPattern;										// どの配置パターンか
};

#endif	// _GAME_OBJECT_H_