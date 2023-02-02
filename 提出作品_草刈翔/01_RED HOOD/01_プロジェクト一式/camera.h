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
// クラスの定義
//*****************************************************************************
class CCamera
{
public:
	// カメラの動きの設定
	typedef enum
	{
		CAMERA_MOVE_TYPE_PLAYER = 0,	// プレイヤーの追従
		CAMERA_MOVE_TYPE_BOSS,			// ボス戦
		CAMERA_MOVE_TYPE_CLEAR,			// クリア時
		CAMERA_MOVE_TYPE_DEBUG,			// デバッグ用
		PLAYER_STATE_MAX
	} CAMERA_MOVE_TYPE;

	CCamera();															// コンストラクタ
	~CCamera();															// デストラクタ
	HRESULT Init(D3DXVECTOR3 m_pos, D3DXVECTOR3 rot,
		float viewportX, float viewportY,
		float viewportWidth, float viewporHeight);						// 初期化処理
	void Uninit(void);													// 終了処理
	void Update(void);													// 更新処理
	static CCamera *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
		float viewportX, float viewportY,
		float viewportWidth, float viewporHeight);						// 生成処理
	void Set(void);														// カメラ設定処理
	void SetPosV(D3DXVECTOR3 pos) { m_posV = pos; }						// 視点設定処理
	D3DXVECTOR3 GetPosV(void) { return m_posV; }						// 視点取得処理
	void SetPosR(D3DXVECTOR3 pos) { m_posR = pos; }						// 注視点設定処理
	D3DXVECTOR3 GetPosR(void) { return m_posR; }						// 注視点取得処理
	void SetRotV(D3DXVECTOR3 rot) { m_rot = rot; }						// 視点の向き設定処理
	D3DXVECTOR3 GetRotV(void) { return m_rot; }							// 視点の向き取得処理
	D3DVIEWPORT9 *GetViewPort(void) { return &m_viewport; }				// ビューポート取得処理
	void SetCameraMove(CAMERA_MOVE_TYPE type);							// カメラの動きの設定処理
	static void SetControl(bool bControl) { m_bControl = bControl; }	// 操作できるか設定処理

private:
	D3DXVECTOR3 MathFront(void);			// 正面の向きを計算する処理
	void MovePlayer(void);					// プレイヤー用の移動処理
	void MoveBoss(void);					// ボス戦用の移動処理
	void MoveClear(void);					// クリア時の移動処理
#ifdef _DEBUG
	void DebugMove(void);					// デバッグで使う移動処理
#endif
	D3DXVECTOR3 m_posV;						// 現在の視点
	D3DXVECTOR3 m_posR;						// 現在の注視点
	D3DXVECTOR3 m_vecU;						// 上方向ベクトル
	D3DXVECTOR3 m_rot;						// 視点の向き
	D3DXVECTOR3 m_rotAsk;					// 目的の向き
	float m_fSpeed;							// 移動速度(主にデバッグで使用)
	float m_fRotateSpeed;					// 旋回速度
	float m_fHeight;						// 視点から注視点までの高さ
	float m_fSaveHeight;					// 視点から注視点までの高さ
	float m_fDistance_PlayerPosR;			// プレイヤーの位置と注視点の距離
	float m_fDistance_posVposR;				// 視点と注視点の距離
	float m_fSaveDistance_PlayerPosR;		// プレイヤーの位置と注視点の目的の距離
	float m_fSaveDistance_posVposR;			// 視点と注視点の目的の距離
	float m_fObjectiveRot;					// 目的の向き
	float m_fObjectiveDistance;				// 目的の距離
	float m_fNumRot;						// 向きを変える量
	D3DXMATRIX m_mtxProjection;				// プロジェクションマトリックス
	D3DXMATRIX m_mtxView;					// ビューマトリックス
	D3DVIEWPORT9 m_viewport;				// ビューポート
	bool m_bRotate;							// 回転しているかどうか
	static bool m_bControl;					// 操作できるかどうか
	CAMERA_MOVE_TYPE m_type;				// 動きの種類
};
#endif // _CAMERA_H_