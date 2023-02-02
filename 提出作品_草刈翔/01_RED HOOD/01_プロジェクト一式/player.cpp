//=============================================================================
// プレイヤー処理 [player.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "model.h"
#include "model_single.h"
#include "mesh_field.h"
#include "mesh_wall.h"
#include "camera.h"
#include "motion.h"
#include "control_player.h"
#include "shadow.h"
#include "sound.h"
#include "collision_sphere.h"
#include "life.h"
#include "level.h"
#include "level_up.h"
#include "enemy.h"
#include "flash.h"
#include "death.h"
#include "pause.h"
#include "weapon.h"

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define PLAYER_BEGIN_LIFE	(200.0f)	// 初期ライフ
#define INVINCIBLE_TIME		(120)		// 無敵時間
#define LIFE_SIZE_X			(250.0f)	// ライフのサイズX
#define LIFE_SIZE_Y			(20.0f)		// ライフのサイズY
#define LEVEL_SIZE_Y		(10.0f)		// レベルのサイズY

//=============================================================================
// コンストラクタ
//=============================================================================
CPlayer::CPlayer(int nPriority) :CScene3D(nPriority)
{
	// 変数のクリア
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	for (int nCntModel = 0; nCntModel < MAX_PLAYER_MODEL; nCntModel++)
	{
		m_apModel[nCntModel] = NULL;
	}
	m_pParent = NULL;
	m_pMotion = NULL;
	m_pControl = NULL;
	m_pShadow = NULL;
	m_pCollisionS = NULL;
	m_pLife = NULL;
	m_pLevel = NULL;
	m_fLife = 0.0f;
	m_fMaxLife = 0.0f;
	m_nExp = 0;
	m_nCntInvincible = 0;
	m_bLand = false;
	m_bLandOld = false;
	m_bJump = false;
	m_bWeaponChange = false;
	m_bInvincible = false;
	m_bCanControl = false;
	m_bDeath = false;
	m_state = PLAYER_STATE_NORMAL;
	m_weapon = PLAYER_WEAPON_AXE;
}

//=============================================================================
// デストラクタ
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// 変数の初期化
	m_rot = rot;
	m_pos = pos;
	m_posOld = pos;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLife = PLAYER_BEGIN_LIFE;
	m_fMaxLife = PLAYER_BEGIN_LIFE;
	m_nExp = 0;
	m_nCntInvincible = 0;
	m_bLand = false;
	m_bLandOld = false;
	m_bJump = false;
	m_bWeaponChange = false;
	m_bInvincible = false;
	m_bCanControl = false;
	m_bDeath = false;
	m_state = PLAYER_STATE_NORMAL;
	m_weapon = PLAYER_WEAPON_AXE;

	// モデル生成処理
	ModelCreate();

	// 位置の設定
	SetPos(m_pos);

	// 頭のパーツのサイズを取得
	m_size = m_apModel[2]->GetSize();

	if (m_size.x > m_size.z)
	{
		m_size.z = m_size.x;
	}
	else if (m_size.z > m_size.x)
	{
		m_size.x = m_size.z;
	}

	D3DXVECTOR3 modelPos = m_apModel[0]->GetPos();
	m_size.y = modelPos.y + m_size.y * 2;

	// モーションの生成
	//※1フレームだけＴ字ポーズになるため、更新処理も処理
	m_pMotion = CMotion::Create(this, CMotion::MOTION_TYPE_PLAYER);
	m_pMotion->Update(this);

	// 体にコリジョン(プレイヤー判定)をつける
	m_pCollisionS = CCollisionSphere::Create(m_pos, m_size.x * 1.5f, CCollisionSphere::COLLISION_S_TYPE::COLLISION_S_TYPE_PLAYER, -1.0f);

	// ライフを生成し、現在のライフと最大ライフを設定
	m_pLife = CLife::Create(D3DXVECTOR2(150.0f, 30.0f), D3DXVECTOR2(LIFE_SIZE_X, LIFE_SIZE_Y), CLife::LIFE_TYPE_PLAYER);
	m_pLife->SetLife(m_fLife);
	m_pLife->SetMaxLife(m_fMaxLife);

	// レベルを生成
	m_pLevel = CLevel::Create(D3DXVECTOR2(150.0f, 60.0f), D3DXVECTOR2(LIFE_SIZE_X, LEVEL_SIZE_Y));

	// 武器UIを生成
	m_pWepaon = CWeapon::Create(D3DXVECTOR2(0.0f, SCREEN_HEIGHT));

	// 影を生成し、影を使用している状態に設定
	m_pShadow = CShadow::Create(D3DXVECTOR3(m_pos.x, m_pos.y - m_size.y, m_pos.z), D3DXVECTOR3(m_size.x * 2, 0.0f, m_size.x * 2));
	m_pShadow->SetUseShadow(true);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPlayer::Uninit(void)
{
	// 影の終了処理
	if (m_pShadow != NULL)
	{
		m_pShadow->Uninit();
	}

	// コントロールの終了処理
	if (m_pControl != NULL)
	{
		m_pControl->Uninit();
		m_pControl = NULL;
	}

	// モーションの破棄
	if (m_pMotion != NULL)
	{
		m_pMotion->Uninit();
	}

	// モデルの終了処理
	for (int nCntModel = 0; nCntModel < MAX_PLAYER_MODEL; nCntModel++)
	{
		if (m_apModel[nCntModel] != NULL)
		{
			m_apModel[nCntModel]->Uninit();
		}
	}

	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPlayer::Update(void)
{
	if (this != NULL)
	{
		// サウンド取得処理
		CSound *pSound = CManager::GetSound();

		// 影の追従
		TrackingShadow();

		// 位置の取得
		D3DXVECTOR3 pos = GetPos();
		m_pos = pos;
		m_posOld = m_pos;

		// 1フレーム前の位置設定
		SetPosOld(m_posOld);

		// 前のフレームに着地しているかどうかの情報を代入
		m_bLandOld = m_bLand;

		// 移動処理
		Move();

		// 移動量反映
		m_pos += m_move;

		// 位置反映
		SetPos(m_pos);

		// 敵との押し出し処理
		CollisionEnemy();

		// モデルとの当たり判定
		m_bLand = CModelSingle::Collision(this);

		// 壁との当たり判定
		CMeshWall::Collision(this);

		if (m_bLand == false)
		{
			// 地面との当たり判定
			m_bLand = CMeshField::Collision(this);
		}

		// 着地中
		if (m_bLand == true)
		{
			// 着地したらY方向の移動量を0に
			m_move.y = 0.0f;

			if (m_bDeath == false)
			{
				if (m_bLandOld == true)
				{
					// ジャンプモーションかつ、モーションをつなげていなかったら
					if (m_pMotion->GetMotion() == 2 && GetMotion()->GetConnect() == false)
					{
						// 足音(2)SEの再生
						pSound->Play(6);
						pSound->ControllVoice(6, 0.3f);

						// 着地モーションにする
						m_pMotion->SetMotion(3);
					}

					m_bJump = false;
				}
			}
		}

		if (m_bDeath == false)
		{
			// 着地していないかつ、モーションをつなげていなかったら
			if (GetLand() == false && GetMotion()->GetConnect() == false)
			{
				if (GetMotion()->GetMotion() != 2 &&
					GetMotion()->GetMotion() != 4 &&
					GetMotion()->GetMotion() != 5 &&
					GetMotion()->GetMotion() != 6 &&
					GetMotion()->GetMotion() != 7 &&
					GetMotion()->GetMotion() != 8 &&
					GetMotion()->GetMotion() != 9)
				{
					// ジャンプモーションにする(落下の表現)
					GetMotion()->SetMotion(2);
				}
			}
		}

		// モーション
		m_pMotion->Update(this);

		// 位置取得
		m_pos = GetPos();

		if (m_pCollisionS != NULL)
		{
			// 球体コリジョンを追従させる
			D3DXVECTOR3 modelPos = m_apModel[0]->GetMtxPos();
			m_pCollisionS->SetPosCollision(modelPos);
		}

		// 他のコリジョンと接触した時の処理
		TouchCollision();

		if (m_pLife != NULL)
		{
			// レベルが上がった瞬間
			if (m_pLevel != NULL && m_pLevel->GetMoment() == true)
			{
				m_fLife += m_fMaxLife / 5;

				if (m_fLife > m_fMaxLife)
				{
					m_fLife = m_fMaxLife;
				}
			}

			// UIに現在のライフ・最大ライフを設定
			m_pLife->SetLife(m_fLife);
			m_pLife->SetMaxLife(m_fMaxLife);
		}

		if (m_pLevel != NULL)
		{
			// UIに現在の経験値を設定
			m_pLevel->SetExp(m_nExp);		
		}

		// 武器変更時の処理
		ChangeWeapon();

		// ライフがなくなったら
		if (m_fLife <= 0.0f)
		{
			m_fLife = 0.0f;

			// コリジョンの判定を消す
			m_pCollisionS->SetCollisionType(CCollisionSphere::COLLISION_S_TYPE_NONE);

			if (m_bDeath == false)
			{
				// 死亡判定をtrueにする
				m_bDeath = true;

				// カメラを操作不可能にする
				CCamera::SetControl(false);

				// ポーズを不可能にする
				CPause::SetPossiblePause(false);

				// ゲームオーバー画面を生成
				CDeath::Create();
			}
		}

		if (m_pCollisionS != NULL)
		{
			// 無敵中など、プレイヤーの状態処理
			State();
		}

		// クリアの状態になったら
		if (CGame::GetClear() == true)
		{
			// 操作を不可能にする
			m_bCanControl = false;

			// ポーズを不可能にする
			CPause::SetPossiblePause(false);

			// ライフのUIを消す
			if (m_pLife != NULL)
			{
				m_pLife->Uninit();
				delete m_pLife;
				m_pLife = NULL;
			}

			// レベルのUIを消す
			if (m_pLevel != NULL)
			{
				m_pLevel->Uninit();
				delete m_pLevel;
				m_pLevel = NULL;
			}

			// 武器のUIを消す
			if (m_pWepaon != NULL)
			{
				m_pWepaon->Uninit();
				delete m_pWepaon;
				m_pWepaon = NULL;
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CPlayer::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// モデルの描画
	for (int nCntModel = 0; nCntModel < MAX_PLAYER_MODEL; nCntModel++)
	{
		m_apModel[nCntModel]->Draw();
	}

}

//=============================================================================
// 生成処理
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// クラスの生成
	CPlayer *pPlayer = NULL;
	pPlayer = new CPlayer;

	if (pPlayer != NULL)
	{
		// プレイヤー操作のクラスを生成
		pPlayer->m_pControl = CControlPlayer::Create();

		// 初期化処理
		pPlayer->Init(pos, rot);
	}
	return pPlayer;
}

//=============================================================================
// モデル生成処理
//=============================================================================
void CPlayer::ModelCreate(void)
{
	// テキストファイルの読み込み
	char cString[256];
	FILE *pFile = fopen("data/TEXT/motion_player.txt", "r");

	if (pFile != NULL)
	{
		int nCntModel = 0;
		char cFileName[MAX_PLAYER_MODEL][128];

		while (1)
		{
			fscanf(pFile, "%s", &cString[0]);

			// モデルのパスを取得
			if (strcmp(&cString[0], "MODEL_FILENAME") == 0)
			{
				fscanf(pFile, "%s", &cString[0]);
				fscanf(pFile, "%s", &cFileName[nCntModel][0]);
				nCntModel++;
			}

			// モデルが最大数になったらパスの読み込みを終了
			if (nCntModel >= MAX_PLAYER_MODEL)
			{
				nCntModel = 0;
				break;
			}
		}

		int nIdx = 0, nParents = 0;
		D3DXVECTOR3 pos, rot;

		while (1)
		{
			fscanf(pFile, "%s", &cString[0]);

			if (strcmp(&cString[0], "PARTSSET") == 0)	//PARTSSETの文字列
			{
				// モデルのパーツ数だけ回す
				while (1)
				{
					fscanf(pFile, "%s", &cString[0]);

					if (strcmp(&cString[0], "INDEX") == 0) //インデックス番号
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%d", &nIdx);
					}
					if (strcmp(&cString[0], "PARENT") == 0) //親のモデル
					{
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%d", &nParents);
					}
					if (strcmp(&cString[0], "POS") == 0) //位置
					{
						// 位置を取得する
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%f%f%f", &pos.x, &pos.y, &pos.z);
					}
					if (strcmp(&cString[0], "ROT") == 0) //向き
					{
						// 向きを取得する
						fscanf(pFile, "%s", &cString[0]);
						fscanf(pFile, "%f%f%f", &rot.x, &rot.y, &rot.z);
					}
					if (strcmp(&cString[0], "END_PARTSSET") == 0)
					{
						break;
					}
				}

				// モデルを生成し、向きと位置を設定
				m_apModel[nCntModel] = CModel::Create(pos, rot, &cFileName[nCntModel][0]);

				// 親モデルを設定
				if (nParents == -1)
				{
					// -1 なら親モデル無し
					m_apModel[nCntModel]->SetParent(NULL);
				}
				else
				{
					// -1 以外なら親子付け
					m_apModel[nCntModel]->SetParent(m_apModel[nParents]);
				}

				nCntModel++;
			}

			// モデルが最大数になったら配置を終了
			if (nCntModel >= MAX_PLAYER_MODEL)
			{
				break;
			}
		}
		// ファイルを閉じる
		fclose(pFile);
	}
}

//=============================================================================
// 移動処理
//=============================================================================
void CPlayer::Move(void)
{
	// ヌルチェック
	if (m_pControl != NULL)
	{
		// プレイヤー操作のクラスにプレイヤーのポインタを入れ、移動量を取得
		m_pControl->Update(this);
		m_move = m_pControl->GetMove();
	}
}

//=============================================================================
// モデル毎の位置設定処理
//=============================================================================
void CPlayer::SetModelPos(int nCntModel, D3DXVECTOR3 pos)
{
	m_apModel[nCntModel]->SetPos(pos);
}

//=============================================================================
// モデル毎の位置取得処理
//=============================================================================
D3DXVECTOR3 CPlayer::GetModelPos(int nCntModel)
{
	return m_apModel[nCntModel]->GetPos();
}

//=============================================================================
// モデル毎の向き設定処理
//=============================================================================
void CPlayer::SetModelRot(int nCntModel, D3DXVECTOR3 rot)
{
	m_apModel[nCntModel]->SetRot(rot);
}

//=============================================================================
// モデル毎の向き取得処理
//=============================================================================
D3DXVECTOR3 CPlayer::GetModelRot(int nCntModel)
{
	return m_apModel[nCntModel]->GetRot();
}

//=============================================================================
// モデル毎のワールドマトリックス座標取得処理
//=============================================================================
D3DXVECTOR3 CPlayer::GetModelMtxPos(int nCntModel)
{
	return m_apModel[nCntModel]->GetMtxPos();
}

//=============================================================================
// レベル取得処理(level.cppを経由)
//=============================================================================
int CPlayer::GetLevel(void)
{
	if (m_pLevel != NULL)
	{
		// 現在のレベルを取得し、返す
		int nLevel = m_pLevel->GetLevel();
		return nLevel;
	}
	else
	{
		return false;
	}
}

//=============================================================================
// 影の追従処理
//=============================================================================
void CPlayer::TrackingShadow(void)
{
	// 影があったら
	if (m_pShadow != NULL)
	{
		//オブジェクト情報を入れるポインタ
		CScene *pObject = NULL;
		//オブジェクト情報を保存するポインタ変数
		CScene *pSaveObject = NULL;

		//先頭のポインタを代入
		pObject = pObject->GetTopObj(CScene::PRIORITY_NONE);

		while (pObject != NULL)
		{
			//現在のオブジェクトのポインタを保存
			pSaveObject = pObject;

			if (pObject->GetObjType() == CScene::OBJECT_TYPE_MODEL)
			{
				// メッシュフィールドクラスにキャスト
				CMeshField *pMeshField = NULL;
				pMeshField = (CMeshField*)pObject;

				D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 保存用の位置
				D3DXVECTOR3 posObj = pMeshField->GetPos();			// オブジェクトの位置を取得
				D3DXVECTOR3 posObjOld = pMeshField->GetPosOld();	// オブジェクトの1フレーム前の位置を取得
				D3DXVECTOR3 sizeObj = pMeshField->GetSize();		// オブジェクトのサイズを取得

				if (posObj.x + sizeObj.x / 2.0f > m_pos.x - m_size.x / 2.0f &&
					posObj.x - sizeObj.x / 2.0f < m_pos.x + m_size.x / 2.0f &&
					posObj.z + sizeObj.z / 2.0f > m_pos.z - m_size.x / 2.0f &&
					posObj.z - sizeObj.z / 2.0f < m_pos.z + m_size.x / 2.0f &&
					posObj.y + sizeObj.y >= m_pos.y - 500.0f &&
					posObj.y + sizeObj.y <= m_pos.y)
				{
					//位置をフィールドの上に
					pos.y = posObj.y;

					//影の追従
					m_pShadow->SetShadow(D3DXVECTOR3(m_pos.x, pos.y, m_pos.z), (m_pos.y - pos.y));

					//影を使っている状態にする
					m_pShadow->SetUseShadow(true);

					return;
				}
				else
				{
					//影を使っていない状態にする
					m_pShadow->SetUseShadow(false);
				}
			}

			pObject = pSaveObject->GetObjNext(pSaveObject);
		}
	}
}

//=============================================================================
// それぞれのコリジョンの接触処理
//=============================================================================
void CPlayer::TouchCollision(void)
{
	if (m_pCollisionS != NULL)
	{
		if (m_bInvincible == false)
		{
			// 体のコリジョンが敵の攻撃判定のコリジョンと触れた瞬間なら
			if (m_pCollisionS->OnCollisionEnter(CCollisionSphere::COLLISION_S_TYPE_ENEMY_ATTACK) == true)
			{
				// 赤くフラッシュさせる
				CFlash::Create(D3DCOLOR_RGBA(204, 0, 0, 200), 0.1f, 0.03f);

				// 防御補正値によって受けるダメージ量を変える
				float fDamage = m_pCollisionS->GetDamageValue() * (1.0f - CLevelUp::GetCorrectionDef() * 0.1f);

				if (fDamage <= 0)
				{
					fDamage = 0;
				}

				// ライフを減らす
				m_fLife -= fDamage;

				// ダメージSEの再生
				CSound *pSound = CManager::GetSound();
				pSound->Play(12);
				pSound->ControllVoice(12, 0.4f);

				// 対象のコリジョンの方向を向かせる
				m_rot.y = m_pCollisionS->GetObjectiveRot();

				// 吹っ飛び状態にする
				m_state = PLAYER_STATE_BLOWAWAY;

				// 無敵状態にする
				m_bInvincible = true;
			}
		}
	}
}

//=============================================================================
// 状態処理
//=============================================================================
void CPlayer::State(void)
{
	if (m_bInvincible == true && m_bDeath == false)
	{
		// 体のコリジョンをなしに設定
		m_pCollisionS->SetCollisionType(CCollisionSphere::COLLISION_S_TYPE_NONE);

		// カウントを進める
		m_nCntInvincible++;
		int nAlpha = 255;

		if (m_nCntInvincible % 8 == 0 || m_nCntInvincible % 8 == 1)
		{
			nAlpha = 128;
		}

		// モデルの色を薄くする
		for (int nCntModel = 0; nCntModel < MAX_PLAYER_MODEL; nCntModel++)
		{
			m_apModel[nCntModel]->SetColor(D3DCOLOR_RGBA(0, 0, 0, nAlpha));
		}

		// 無敵時間が終わったら
		if (m_nCntInvincible > INVINCIBLE_TIME)
		{
			// カラーを元に戻す
			for (int nCntModel = 0; nCntModel < MAX_PLAYER_MODEL; nCntModel++)
			{
				m_apModel[nCntModel]->SetColor(D3DCOLOR_RGBA(0, 0, 0, 255));
			}

			// カウントをリセット
			m_nCntInvincible = 0;

			// 無敵状態から戻す
			m_bInvincible = false;

			// 体のコリジョンをプレイヤーに戻す
			m_pCollisionS->SetCollisionType(CCollisionSphere::COLLISION_S_TYPE_PLAYER);
		}
	}
	else if (m_state == PLAYER_STATE_DEBUG)
	{
		// 体のコリジョンをなしに設定
		m_pCollisionS->SetCollisionType(CCollisionSphere::COLLISION_S_TYPE_NONE);
	}
}

//=============================================================================
// 敵との(押し出しのための)衝突処理
//=============================================================================
void CPlayer::CollisionEnemy(void)
{
	//オブジェクト情報を入れるポインタ
	CScene *pObject = NULL;
	//オブジェクト情報を保存するポインタ変数
	CScene *pSaveObject = NULL;

	//先頭のポインタを代入
	pObject = pObject->GetTopObj(CScene::PRIORITY_MODEL);

	while (pObject != NULL)
	{
		//現在のオブジェクトのポインタを保存
		pSaveObject = pObject;

		if (pObject->GetObjType() == CScene::OBJECT_TYPE_ENEMY)
		{
			//プレイヤーにキャスト
			CEnemy *pEnemy = NULL;
			pEnemy = (CEnemy*)pObject;

			D3DXVECTOR3 posEne = pEnemy->GetPos();		// 敵の位置を取得
			float fEneLife = pEnemy->GetLife();			// 敵のライフ

			float fDistance = sqrtf((m_pos.x - posEne.x) * (m_pos.x - posEne.x) + (m_pos.y - posEne.y) * (m_pos.y - posEne.y) + (m_pos.z - posEne.z) * (m_pos.z - posEne.z));
			float fRot = (float)atan2((posEne.x - m_pos.x), (posEne.z - m_pos.z)) - D3DX_PI;

			// 距離がプレイヤーのサイズより小さいかつ、敵のライフが残っていたら
			if (fDistance <= m_size.x && fEneLife > 0.0f)
			{
				m_pos.x = posEne.x + (sinf(fRot) * m_size.x);
				m_pos.z = posEne.z + (cosf(fRot) * m_size.z);

				// 位置を設定
				SetPos(m_pos);
			}
		}
		pObject = pSaveObject->GetObjNext(pSaveObject);
	}
}

//=============================================================================
// 武器変更時の処理
//=============================================================================
void CPlayer::ChangeWeapon(void)
{
	// 武器が変わったなら
	if (m_bWeaponChange == true)
	{
		if (m_pWepaon != NULL)
		{
			// 武器アイコンのUIを切り替える
			m_pWepaon->SetUIChange((int)m_weapon);
		}

		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		switch (m_weapon)
		{
		case PLAYER_WEAPON_AXE:
			if (m_apModel != NULL)
			{
				m_apModel[15]->Uninit();
				m_apModel[15] = NULL;
			}

			if (m_apModel[15] == NULL)
			{
				// 右手のパーツの位置を取得
				pos = m_apModel[5]->GetPos();

				// ボウガンを生成し、右手のパーツに親子関係を持たせる
				m_apModel[15] = CModel::Create(D3DXVECTOR3(pos.x - 5.0f, pos.y + 0.0f, pos.z + 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data/MODEL/Player/12_axe.x");
				m_apModel[15]->SetParent(m_apModel[5]);
			}
			break;

		case PLAYER_WEAPON_BOWGUN:
			if (m_apModel != NULL)
			{
				m_apModel[15]->Uninit();
				m_apModel[15] = NULL;
			}

			if (m_apModel[15] == NULL)
			{
				// 右手のパーツの位置を取得
				pos = m_apModel[5]->GetPos();

				// 斧を生成し、右手のパーツに親子関係を持たせる
				m_apModel[15] = CModel::Create(D3DXVECTOR3(pos.x - 5.0f, pos.y + 0.0f, pos.z + 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data/MODEL/Player/12_bowgun.x");
				m_apModel[15]->SetParent(m_apModel[5]);
			}
			break;

		default:
			break;
		}

		// 武器の切り替えを終了
		m_bWeaponChange = false;
	}
}