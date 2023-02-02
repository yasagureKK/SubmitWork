//=============================================================================
// フェード処理 [fade.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "manager.h"
#include "scene.h"

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CFade : public CScene
{
public:
	// フェードの状態
	typedef enum
	{
		FADE_NONE = 0,		// 何もしていない
		FADE_IN,			// フェードイン
		FADE_OUT,			// フェードアウト
		FADE_MAX
	} FADE;

	CFade(int nPriority = PRIORITY_FADE);					// コンストラクタ
	~CFade();												// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos);							// 初期化処理
	void Uninit(void);										// 終了処理
	void Update(void);										// 更新処理
	void Draw(void);										// 描画処理
	void SetFade(CManager::MODE modeNext, D3DXCOLOR col);	// 設定処理
	static FADE GetFade(void) { return m_fade; }			// 取得処理
	OBJECT_TYPE GetObjType(void) { return OBJECT_TYPE_NONE; }				// オブジェクトの種類の取得処理
	void SetPos(D3DXVECTOR3 pos) { pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	// 位置設定処理
	D3DXVECTOR3 GetPos(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// 位置取得処理
	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// サイズ取得処理
	D3DXVECTOR3 GetPosOld(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	// 1フレーム前の位置取得処理
	D3DXVECTOR3 GetRot(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// 向き取得処理

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;						// 頂点バッファへのポインタ
	CManager::MODE m_modeNext;								// 次の画面
	D3DXCOLOR m_color;										// フェードの色
	static FADE m_fade;										// フェードの状態
};
#endif // _FADE_H_