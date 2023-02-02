//=============================================================================
// プレイヤー処理 [player.h]
// Authore : 草刈 翔
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CModel;
class CMotionPlayer;
class CControl;
class CShadow;
class CParticle;

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define MAX_PLAYER_MODEL	(12)	// モデルの数

//*****************************************************************************
//クラスの定義
//*****************************************************************************
class CPlayer : public CScene
{
public:
	// プレイヤーの操作の種類
	typedef enum
	{
		PLAYER_COMMAND_NONE = 0,	// なし
		PLAYER_COMMAND_DASH,		// ダッシュ
		PLAYER_COMMAND_JUMP,		// ジャンプ
		PLAYER_COMMAND_DOUBLE_JUMP,	// 2段ジャンプ
		PLAYER_COMMAND_HOVERING,	// ホバリング
		PLAYER_COMMAND_MAX
	} PLAYER_COMMAND;

	CPlayer(int nPriority = PRIORITY_MODEL);					// コンストラクタ
	~CPlayer();													// デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);			// 初期化処理
	void Uninit(void);											// 終了処理
	void Update(void);											// 更新処理
	void Draw(void);											// 描画処理
	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3	rot,
		int nNumPlayer, bool bCpu);								// 生成処理
	D3DXVECTOR3 GetPosOld(void);								// 1フレーム前の位置取得処理
	D3DXVECTOR3 GetRot(void);									// 向き取得処理
	void SetRot(D3DXVECTOR3 rot);								// 向き設定処理
	void SetMove(D3DXVECTOR3 move);								// 移動量設定処理
	D3DXVECTOR3 GetMove(void);									// 移動量取得処理
	void SetJump(bool bJump);									// ジャンプ設定処理
	bool GetJump(void);											// ジャンプ取得処理
	void SetLand(bool bLand);									// 着地設定処理
	bool GetLand(void);											// 着地取得処理
	void SetLandFinish(bool bLandFinish);						// 着地でフィニッシュしたかどうか設定処理
	bool GetLandFinish(void);									// 着地でフィニッシュしたかどうか取得処理
	void SetStop(bool bStop);									// 止まっているかどうか設定処理
	bool GetStop(void);											// 止まっているかどうか取得処理
	void SetModelPos(int nCntModel, D3DXVECTOR3 pos);			// モデル毎の位置設定処理
	D3DXVECTOR3 GetModelPos(int nCntModel);						// モデル毎の位置取得処理
	void SetModelRot(int nCntModel, D3DXVECTOR3 rot);			// モデル毎の向き設定処理
	D3DXVECTOR3 GetModelRot(int nCntModel);						// モデル毎の向き取得処理
	CMotionPlayer *GetMotionPlayer(void);						// プレイヤーのモーション取得処理
	void Collision(CScene *pSCene);								// オブジェクトとの当たり判定
	void ShadowCollision(void);									// 影の表示のためのオブジェクトとの当たり判定
	void SetNum(int nNum);										// プレイヤーの番号設定処理
	int GetNum(void);											// プレイヤーの番号取得処理
	void SetCommand(PLAYER_COMMAND command);					// プレイヤーの操作設定処理
	PLAYER_COMMAND GetCommand(void);							// プレイヤーの操作取得処理
	bool GetCpu(void);											// CPUかどうか取得処理

private:
	void ModelCreate(int nNum);									// モデル生成処理
	void Move(void);											// 移動処理
	bool Collision(void);										// 当たり判定
	void CpuMove();												// CPU操作処理

	D3DXVECTOR3 m_pos;											// 位置
	D3DXVECTOR3 m_posOld;										// 1フレーム前の位置
	D3DXVECTOR3 m_move;											// ポリゴンの移動量
	D3DXVECTOR3	m_size;											// サイズ
	D3DXVECTOR3 m_rot;											// 向き
	CModel *m_apModel[MAX_PLAYER_MODEL];						// モデルのポインタ
	D3DXMATRIX m_mtxWorld;										// ワールドマトリックス
	CModel *m_pParent;											// 親モデルへのポインタ
	CControl *m_pControl;										// コントロールのポインタ
	CShadow *m_pShadow;											// シャドウのポインタ
	CMotionPlayer *m_pMotionPlayer;								// プレイヤーモーションのポインタ
	CParticle *m_pParticle;										// パーティクルのポインタ
	bool m_bLand;												// 着地しているかどうか
	bool m_bJump;												// ジャンプしているかどうか
	bool m_bJumpOld;											// 前フレームジャンプしているかどうか
	bool m_bLandFinish;											// 着地でフィニッシュしたかどうか
	bool m_bStop;												// 止まっているかどうか
	bool m_bCpu;												// CPUかどうか
	int m_nNum;													// プレイヤーの番号
	int m_nCpuCount;											// CPU操作のカウント
	PLAYER_COMMAND m_command;									// 操作の種類
};

#endif // _PLAYER_H_