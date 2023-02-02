//=============================================================================
// エネミーボックス処理 [enemy_box.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _ENEMY_BOX_H_
#define _ENEMY_BOX_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene3D.h"

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define MAX_ENEBOX_ENEMY	(6)	// 敵の最大数
#define MAX_ENEBOX_WALL		(8)	// 壁ポリゴンの最大数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CEnemy;
class CMeshWall;

//*****************************************************************************
//クラスの定義
//*****************************************************************************
class CEnemyBox : public CScene3D
{
public:
	// 種類
	typedef enum
	{
		ENEMY_BOX_TYPE_0 = 0,	// 左ルートに配置する
		ENEMY_BOX_TYPE_1,		// 右ルートに配置する
		ENEMY_BOX_TYPE_2,		// 合流地点で配置する
		ENEMY_BOX_TYPE_BOSS,	// 最奥部に配置する
		ENEMY_BOX_TYPE_MAX
	} ENEMY_BOX_TYPE;

	CEnemyBox(int nPriority = PRIORITY_NONE);								// コンストラクタ
	~CEnemyBox();															// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);						// 初期化処理
	void Uninit(void);														// 終了処理
	void Update(void);														// 更新処理
	void Draw(void);														// 描画処理
	static CEnemyBox *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size,
		int nType);															// 生成処理
	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_NONE; }				// オブジェクトの種類の取得処理

private:
	void InsidePlayer(void);												// プレイヤーが中に入ったかを判別する処理
	void SetObject(void);													// 敵・壁の配置処理

	D3DXVECTOR3 m_pos;														// 位置
	D3DXVECTOR3	m_size;														// サイズ
	D3DXVECTOR3 m_rot;														// 向き
	D3DXMATRIX m_mtxWorld;													// ワールドマトリックス
	ENEMY_BOX_TYPE m_type;													// 種類
	int m_nEnemyNum;														// 敵の数
	bool m_bInside;															// プレイヤーが中に入ったか
	bool m_bDefeat;															// 敵が全滅したか
	CEnemy *m_pEnemy[MAX_ENEBOX_ENEMY];										// エネミーのポインタ
	CMeshWall *m_pMeshWall[MAX_ENEBOX_WALL];								// メッシュウォールのポインタ
};
#endif // _ENEMY_BOX_H_