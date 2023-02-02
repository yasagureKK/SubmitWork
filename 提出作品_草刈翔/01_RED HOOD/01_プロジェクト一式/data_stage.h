//=============================================================================
// ステージデータ [data_stage.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _DATA_STAGE_
#define _DATA_STAGE_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CMeshSky;
class CMeshField;
class CMeshWall;
class CModelSingle;
class CEnemy;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CDataStage : public CScene
{
public:
	// ステージの種類
	typedef enum
	{
		STAGE_TYPE_GAME = 0,		// ゲーム
		STAGE_TYPE_DEBUG,			// デバッグ
		STAGE_TYPE_MAX
	} STAGE_TYPE;

	CDataStage();															// コンストラクタ
	~CDataStage();															// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos);											// 初期化処理
	void Uninit(void);														// 終了処理
	void Update(void);														// 更新処理
	void Draw(void);														// 描画処理
	static CDataStage *Create(D3DXVECTOR3 pos, STAGE_TYPE type);			// 生成処理
	void LoadStage(int nNum);												// ステージロード処理
	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_NONE; }				// オブジェクトの種類の取得処理
	void SetPos(D3DXVECTOR3 pos) { pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	// 位置設定処理
	D3DXVECTOR3 GetPos(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); };		// 位置取得処理
	D3DXVECTOR3 GetPosOld(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	// 1フレーム前の位置取得処理
	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// サイズ取得処理
	D3DXVECTOR3 GetRot(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// 向き取得処理

private:
	D3DXVECTOR3 m_pos;														// 位置
	D3DXVECTOR3 m_rot;														// 向き
	D3DXVECTOR3 m_size;														// サイズ
	int m_nType;															// 種類
	int m_nRow;																// 行数
	int m_nLine;															// 列数
	int m_nMaxStage;														// ステージの最大数
	CMeshSky *m_pMeshSky;													// メッシュスカイのポインタ
	STAGE_TYPE m_type;														// ステージの種類
};
#endif	// _DATA_STAGE_