//=============================================================================
// カメラ処理 [camera.h]
// Author : 草刈 翔
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_CAMERA				(2)			// カメラの最大数
#define CAMERA_HEIGHT			(80.0f)		// カメラの高さ
#define CAMERA_MOVE_SPEED		(3.0f)		// カメラの移動速度
#define CAMERA_ROTATE_SPEED		(0.03f)		// カメラの旋回速度
#define CAMERA_DISTANCE			(250.0f)	// 視点と注視点の距離
#define CAMERA_MIN_RENDERER		(10.0f)		// 描画範囲の最小値
#define CAMERA_MAX_RENDERER		(8000.0f)	// 描画範囲の最大値

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CCamera
{
public:
	CCamera();													// コンストラクタ
	~CCamera();													// デストラクタ
	HRESULT Init(D3DXVECTOR3 m_pos, D3DXVECTOR3 rot,
		float viewportX, float viewportY,
		float viewportWidth, float viewporHeight);				// 初期化処理
	void Uninit(void);											// 終了処理
	void Update(void);											// 更新処理
	static CCamera *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
		float viewportX, float viewportY,
		float viewportWidth, float viewporHeight);				// 生成処理
	void Set(void);												// カメラ設定処理
	void SetPosV(D3DXVECTOR3 pos);								// 視点設定処理
	D3DXVECTOR3 GetPosV(void);									// 視点取得処理
	void SetPosR(D3DXVECTOR3 pos);								// 注視点設定処理
	D3DXVECTOR3 GetPosR(void);									// 注視点取得処理
	void SetRotV(D3DXVECTOR3 rot);								// 視点の向き設定処理
	D3DXVECTOR3 GetRotV(void);									// 視点の向き取得処理
	void SetNum(int nNum);										// カメラの番号設定処理
	int GetNum(void);											// カメラの番号取得処理
	D3DVIEWPORT9 *GetViewPort(void) { return &m_viewport; }		// ビューポート取得処理

private:
	void Move();												// カメラの移動

#ifdef _DEBUG
	void DebugMove();											// デバッグ用のカメラ
#endif

	D3DXVECTOR3 m_posV;											// 現在の視点
	D3DXVECTOR3 m_posVDest;										// 目的の視点
	D3DXVECTOR3 m_posR;											// 現在の注視点
	D3DXVECTOR3 m_posRDest;										// 目的の注視点
	D3DXVECTOR3 m_vecU;											// 上方向ベクトル
	D3DXVECTOR3 m_rot;											// 視点の向き
	D3DXVECTOR3 m_rotAsk;										// 目的の向き
	float m_fHeight;											// 視点から注視点の高さ
	float m_fRotDiffer;											// 目的の向きとの差
	float m_fDistance;											// 視点と注視点の距離
	D3DXMATRIX m_mtxProjection;									// プロジェクションマトリックス
	D3DXMATRIX m_mtxView;										// ビューマトリックス
	D3DVIEWPORT9 m_viewport;									// ビューポート
	D3DXMATRIX m_mtxWorldCamera;								// ワールドマトリックス
	bool m_bTurn;												// カメラが回転中かどうか
	int m_nNum;													// カメラの番号
};

#endif // _CAMERA_H_