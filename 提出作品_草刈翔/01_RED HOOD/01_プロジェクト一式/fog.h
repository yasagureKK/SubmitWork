//=============================================================================
// フォグ(霧)処理 [fog.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _FOG_H_
#define _FOG_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
//クラスの定義
//*****************************************************************************
class CFog : public CScene
{
public:
	CFog(int nPriority = PRIORITY_NONE);									// コンストラクタ
	~CFog();																// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos);											// 初期化処理
	void Uninit(void);														// 終了処理
	void Update(void);														// 更新処理
	void Draw(void);														// 描画処理
	static CFog *Create(D3DXCOLOR col, float fStart, float fEnd);			// 生成処理
	void SetFogLine(float fStart, float fEnd);								// 計算用の変数の設定

	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_NONE; }				// オブジェクトの種類の取得処理
	void SetPos(D3DXVECTOR3 pos) { pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	// 位置設定処理
	D3DXVECTOR3 GetPos(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// 位置取得処理
	D3DXVECTOR3 GetPosOld(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	// 1フレーム前の位置取得処理
	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// サイズ取得処理
	D3DXVECTOR3 GetRot(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// 向き取得処理

private:
	D3DXCOLOR m_col;									// カラー
	float m_fStartFog;									// 霧効果が開始される距離
	float m_fEngFog;									// 霧効果が増加しなくなる距離
};
#endif // _FOG_H_