//=============================================================================
// エネミー処理 [enemy.cpp]
// Author : 草刈 翔
//=============================================================================
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "enemy.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "model.h"
#include "model_single.h"
#include "mesh_field.h"
#include "mesh_wall.h"
#include "motion.h"
#include "control_wolf00.h"
#include "control_wolf01.h"
#include "control_boss.h"
#include "shadow.h"
#include "life_billboard.h"
#include "life.h"
#include "sound.h"
#include "collision_sphere.h"
#include "player.h"
#include "data_clear.h"
#include "particle_hit.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CEnemy::CEnemy(int nPriority) :CScene3D(nPriority)
{
	// 変数のクリア
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	for (int nCntModel = 0; nCntModel < MAX_ENEMY_MODEL; nCntModel++)
	{
		m_apModel[nCntModel] = NULL;
	}
	m_pParent = NULL;
	m_pMotion = NULL;
	m_pControl = NULL;
	m_pShadow = NULL;
	m_pLifeBill = NULL;
	m_pLife = NULL;
	memset(&m_pCollisionS, NULL, sizeof(m_pCollisionS));
	m_nModelNum = 0;
	m_nDeathCount = 0;
	m_nScore = 0;
	m_fLife = 0.0f;
	m_bLand = false;
	m_bKnockDown = false;
	m_bTouchWall = false;
	m_bCanControl = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CEnemy::~CEnemy()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, float fLife)
{
	// 変数の初期化
	m_pos = pos;
	m_posOld = pos;
	m_fLife = fLife;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nDeathCount = 0;
	m_bLand = false;
	m_bKnockDown = false;
	m_bTouchWall = false;
	m_bCanControl = false;

	// モデルの生成処理
	CreateModel(m_type);

	// 位置の設定
	SetPos(m_pos);

	// サイズを取得
	m_size = m_apModel[0]->GetSize();

	if (m_size.x > m_size.z)
	{
		m_size.z = m_size.x;
	}
	else if (m_size.z > m_size.x)
	{
		m_size.x = m_size.z;
	}

	// サイズの設定
	SetSize(m_size);

	CModel *pModel = NULL;
	D3DXVECTOR3 modelPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// コントロール、モーション、スコア、コリジョンの生成
	switch (m_type)
	{
	case ENEMY_TYPE_WOLF_0:
		m_pControl = CControlWolf00::Create();
		m_pMotion = CMotion::Create(this, CMotion::MOTION_TYPE_WOLF00);
		m_nScore = 200;
		// 体にあたるモデルの位置や情報を取得し、親子付け
		modelPos = GetModelPos(0);
		pModel = GetModel(0);
		m_pCollisionS[0] = CCollisionSphere::Create(modelPos, m_size.x * 1.2f, CCollisionSphere::COLLISION_S_TYPE::COLLISION_S_TYPE_ENEMY, -1.0f);
		m_pCollisionS[0]->SetParent(pModel);
		break;

	case ENEMY_TYPE_WOLF_1:
		m_pControl = CControlWolf01::Create();
		m_pMotion = CMotion::Create(this, CMotion::MOTION_TYPE_WOLF01);
		m_nScore = 300;
		// 体にあたるモデルの位置や情報を取得し、親子付け
		modelPos = GetModelPos(0);
		pModel = GetModel(0);
		m_pCollisionS[0] = CCollisionSphere::Create(D3DXVECTOR3(modelPos.x, modelPos.y - m_size.y, modelPos.z), m_size.x * 1.2f, CCollisionSphere::COLLISION_S_TYPE::COLLISION_S_TYPE_ENEMY, -1.0f);
		m_pCollisionS[0]->SetParent(pModel);
		break;

	case ENEMY_TYPE_BOSS:
		m_pControl = CControlBoss::Create();
		m_pMotion = CMotion::Create(this, CMotion::MOTION_TYPE_BOSS);
		m_nScore = 3000;
		m_fLife = BEGIN_BOSS_LIFE;
		
		// 体にあたるモデルの位置や情報を取得し、親子付け
		modelPos = GetModelPos(1);
		pModel = GetModel(1);
		m_pCollisionS[0] = CCollisionSphere::Create(D3DXVECTOR3(modelPos.x, modelPos.y, modelPos.z), m_size.x, CCollisionSphere::COLLISION_S_TYPE::COLLISION_S_TYPE_ENEMY, -1.0f);
		m_pCollisionS[0]->SetParent(pModel);
		// 頭にあたるモデルの情報を取得し、親子付け
		modelPos = GetModelPos(2);
		pModel = GetModel(2);
		m_pCollisionS[1] = CCollisionSphere::Create(D3DXVECTOR3(modelPos.x, modelPos.y - 100.0f, modelPos.z + 40.0f), m_size.x * 0.6f, CCollisionSphere::COLLISION_S_TYPE::COLLISION_S_TYPE_ENEMY, -1.0f);
		m_pCollisionS[1]->SetParent(pModel);
		// 右手にあたるモデルの情報を取得し、親子付け
		modelPos = GetModelPos(5);
		pModel = GetModel(5);
		m_pCollisionS[2] = CCollisionSphere::Create(D3DXVECTOR3(modelPos.x + 60.0f, modelPos.y, modelPos.z), m_size.x * 0.4f, CCollisionSphere::COLLISION_S_TYPE::COLLISION_S_TYPE_ENEMY, -1.0f);
		m_pCollisionS[2]->SetParent(pModel);
		// 左手にあたるモデルの情報を取得し、親子付け
		modelPos = GetModelPos(8);
		pModel = GetModel(8);
		m_pCollisionS[3] = CCollisionSphere::Create(D3DXVECTOR3(modelPos.x - 60.0f, modelPos.y, modelPos.z), m_size.x * 0.4f, CCollisionSphere::COLLISION_S_TYPE::COLLISION_S_TYPE_ENEMY, -1.0f);
		m_pCollisionS[3]->SetParent(pModel);
		break;

	default:
		break;
	}
	m_pMotion->Update(this);

	modelPos = m_apModel[0]->GetPos();
	m_size.y = modelPos.y + m_size.y * 2;

	// 影の生成し、影を使用している状態に設定
	m_pShadow = CShadow::Create(D3DXVECTOR3(m_pos.x, m_pos.y - m_size.y, m_pos.z), D3DXVECTOR3(m_size.x, 0.0f, m_size.x));
	m_pShadow->SetUseShadow(true);

	if (m_type == ENEMY_TYPE_BOSS)
	{
		// ライフを生成し、現在のライフと最大ライフを設定
		m_pLife = CLife::Create(D3DXVECTOR2(SCREEN_WIDTH / 2 + 180.0f, SCREEN_HEIGHT - 70.0f), D3DXVECTOR2(SCREEN_WIDTH - 480.0f, 35.0f), CLife::LIFE_TYPE_BOSS);
		m_pLife->SetLife((float)m_fLife);
		m_pLife->SetMaxLife((float)BEGIN_BOSS_LIFE);
	}
	else
	{
		// ライフと最大ライフを設定
		m_pLifeBill = CLifeBillboard::Create(D3DXVECTOR3(m_pos.x, m_pos.y + m_size.y / 2, m_pos.z), 130.0f, 10.0f);
		m_pLifeBill->SetMaxLife(m_fLife);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEnemy::Uninit(void)
{
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
	for (int nCntPlayer = 0; nCntPlayer < MAX_ENEMY_MODEL; nCntPlayer++)
	{
		if (m_apModel[nCntPlayer] != NULL)
		{
			m_apModel[nCntPlayer]->Uninit();
		}
	}

	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEnemy::Update(void)
{
	if (this != NULL)
	{
		// 動作させるかどうか
		CanControl(this);

		if (m_bCanControl == true)
		{
			// サウンド取得処理
			CSound *pSound = CManager::GetSound();

			// 影の追従
			TrackingShadow();

			// 位置の取得
			D3DXVECTOR3 pos = GetPos();
			m_pos = pos;
			m_posOld = pos;

			// 倒されていない場合
			if (m_bKnockDown == false)
			{
				// 移動処理
				Move();
			}
			// 倒された場合
			else
			{
				m_move.x = 0.0f;
				m_move.z = 0.0f;
			}

			// 移動量反映
			m_pos += m_move;

			// 位置反映
			SetPos(m_pos);

			// 敵同士の押し出し処理
			CollisionEnemy(this);

			// モデルとの当たり判定
			m_bLand = CModelSingle::Collision(this);

			// 壁との当たり判定
			m_bTouchWall = CMeshWall::Collision(this);

			if (m_bLand == false)
			{
				// 地面との当たり判定
				m_bLand = CMeshField::Collision(this);
			}

			if (m_bLand == true)
			{
				// 着地したらY方向の移動量を0に
				m_move.y = 0.0f;
			}

			// モーション
			m_pMotion->Update(this);

			// 位置取得
			m_pos = GetPos();

			// 他のコリジョンと接触した時の処理
			TouchCollision();
		}

		if (m_pLifeBill != NULL)
		{
			// 位置を更新し、現在のライフを設定
			m_pLifeBill->SetPos(m_pos);
			m_pLifeBill->SetLife(m_fLife);
		}

		if (m_pLife != NULL)
		{
			// 現在のライフを設定
			m_pLife->SetLife(m_fLife);
		}

		// ライフがなくなったら
		if (m_fLife <= 0)
		{
			// 倒された時の処理
			KnockDown();
		}

		// クリアの状態になったら
		if (CGame::GetClear() == true)
		{
			// ライフのUIを消す
			if (m_pLife != NULL)
			{
				m_pLife->Uninit();
				delete m_pLife;
				m_pLife = NULL;
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CEnemy::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

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
	for (int nCntModel = 0; nCntModel < m_nModelNum; nCntModel++)
	{
		m_apModel[nCntModel]->Draw();
	}
}

//=============================================================================
// 生成処理
//=============================================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, ENEMY_TYPE type, float fLife)
{
	// クラスの生成
	CEnemy *pEnemy = NULL;
	pEnemy = new CEnemy;

	if (pEnemy != NULL)
	{
		// 変数の初期化
		pEnemy->m_rot = rot;
		pEnemy->m_type = type;

		switch (pEnemy->m_type)
		{
		case ENEMY_TYPE_WOLF_0:
			pEnemy->m_nModelNum = MAX_ENEMY_MODEL_WOLF00;
			break;
		case ENEMY_TYPE_WOLF_1:
			pEnemy->m_nModelNum = MAX_ENEMY_MODEL_WOLF01;
			break;
		case ENEMY_TYPE_BOSS:
			pEnemy->m_nModelNum = MAX_ENEMY_MODEL_BOSS;
			break;
		default:
			break;
		}

		// 初期化処理
		pEnemy->Init(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), fLife);
	}
	return pEnemy;
}

//=============================================================================
// 移動処理
//=============================================================================
void CEnemy::Move(void)
{
	// ヌルチェック
	if (m_pControl != NULL)
	{
		// エネミー操作のクラスにプレイヤーのポインタを入れ、移動量を取得
		m_pControl->Update(this);
		m_move = m_pControl->GetMove();
	}
}

//=============================================================================
// 影の表示のためのオブジェクトとの当たり判定
//=============================================================================
void CEnemy::TrackingShadow(void)
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
void CEnemy::TouchCollision(void)
{
	for (int nCollision = 0; nCollision < MAX_ENEMY_COLLSION; nCollision++)
	{
		if (m_pCollisionS[nCollision] != NULL)
		{
			// 体のコリジョンが何らかの武器判定のコリジョンと触れた瞬間なら
			if (m_pCollisionS[nCollision]->OnCollisionEnter(CCollisionSphere::COLLISION_S_TYPE_WEAPON) == true || m_pCollisionS[nCollision]->OnCollisionEnter(CCollisionSphere::COLLISION_S_TYPE_WEAPON_P) == true)
			{
				// ライフを減らす
				m_fLife -= m_pCollisionS[nCollision]->GetDamageValue();

				// 斧のヒットSEの再生
				CSound *pSound = CManager::GetSound();
				pSound->Play(8);
				pSound->ControllVoice(8, 0.8f);

				// パーティクルの生成
				D3DXVECTOR3 collpos = m_pCollisionS[nCollision]->GetMtxPos();
				CParticleHit::Create(collpos, D3DXVECTOR3(50.0f, 50.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DCOLOR_RGBA(255, 50, 50, 255), 20, 8.0f, 10);

				// ザコ敵は被ダメージモーションにする
				if (m_type == ENEMY_TYPE_WOLF_0 || m_type == ENEMY_TYPE_WOLF_1)
				{
					// ライフが残っていたら
					if (m_pCollisionS[nCollision]->OnCollisionEnter(CCollisionSphere::COLLISION_S_TYPE_WEAPON_P) == true && m_fLife > 0)
					{
						// ダメージモーションにする
						m_pMotion->SetMotion(3);
					}
				}
			}
		}
	}
}

//=============================================================================
// 敵との(押し出しのための)衝突処理
//=============================================================================
void CEnemy::CollisionEnemy(CEnemy *pEnemy)
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
			// 自分以外の敵がいたら
			if (pObject != this)
			{
				//エネミーにキャスト
				CEnemy *pObjEnemy = NULL;
				pObjEnemy = (CEnemy*)pObject;

				D3DXVECTOR3 posEne = pObjEnemy->GetPos();		// 対象の敵の位置を取得
				D3DXVECTOR3 posEneOld = pObjEnemy->GetPosOld();	// 対象の敵の1フレーム前の位置を取得
				D3DXVECTOR3 sizeEne = pObjEnemy->GetSize();		// 対象の敵のサイズを取得
				float fEneLife = pObjEnemy->GetLife();			// 対象の敵のライフ

				float fDistance = sqrtf((pEnemy->m_pos.x - posEne.x) * (pEnemy->m_pos.x - posEne.x) + (pEnemy->m_pos.y - posEne.y) * (pEnemy->m_pos.y - posEne.y) + (pEnemy->m_pos.z - posEne.z) * (pEnemy->m_pos.z - posEne.z));
				float fRot = (float)atan2((posEne.x - pEnemy->m_pos.x), (posEne.z - pEnemy->m_pos.z)) - D3DX_PI;

				// 距離がエネミーのサイズより小さいかつ、対象の敵のライフが残っていたら
				if (fDistance <= pEnemy->m_size.x && fEneLife > 0.0f)
				{
					pEnemy->m_pos.x = posEne.x + (sinf(fRot) * m_size.x);
					pEnemy->m_pos.z = posEne.z + (cosf(fRot) * m_size.z);

					// 位置を設定
					SetPos(pEnemy->m_pos);
				}
			}
		}
		pObject = pSaveObject->GetObjNext(pSaveObject);
	}
}

//=============================================================================
// モデル生成時の処理
//=============================================================================
void CEnemy::CreateModel(ENEMY_TYPE type)
{
	char cString[256];		// テキスト保存用の変数
	FILE *pFile = NULL;		// テキストファイルの読み込み

	// 狼00
	if (type == ENEMY_TYPE_WOLF_0)
	{
		pFile = fopen("data/TEXT/motion_wolf00.txt", "r");
		//nModelType = 17; // [狼00：体]のモデル番号
	}
	// 狼01
	else if (type == ENEMY_TYPE_WOLF_1)
	{
		pFile = fopen("data/TEXT/motion_wolf01.txt", "r");
		//nModelType = 29; // [狼01：体]のモデル番号
	}
	// ボス
	else if (type == ENEMY_TYPE_BOSS)
	{
		pFile = fopen("data/TEXT/motion_boss.txt", "r");
		//nModelType = 45; // [ボス：腰]のモデル番号
	}

	if (pFile != NULL)
	{
		int nCntModel = 0;
		char cFileName[MAX_ENEMY_MODEL][128];

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
			if (nCntModel >= m_nModelNum)
			{
				nCntModel = 0;
				break;
			}
		}

		int nParents = 0;
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
			if (nCntModel >= m_nModelNum)
			{
				break;
			}
		}
		// ファイルを閉じる
		fclose(pFile);
	}
}

//=============================================================================
// 経験値受け渡し処理
//=============================================================================
void CEnemy::SendExp(void)
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

		if (pObject->GetObjType() == CScene::OBJECT_TYPE_PLAYER)
		{
			// 自分以外の敵がいたら
			if (pObject != NULL)
			{
				//プレイヤーにキャスト
				CPlayer *pPlayer = NULL;
				pPlayer = (CPlayer*)pObject;

				int nExp = 0;
				switch (m_type)
				{
				case ENEMY_TYPE_WOLF_0:
					nExp = 2;
					break;

				case ENEMY_TYPE_WOLF_1:
					nExp = 3;
					break;

				default:
					break;
				}

				// 位置を設定
				pPlayer->AddExp(nExp);
			}
		}
		pObject = pSaveObject->GetObjNext(pSaveObject);
	}
}

//=============================================================================
// 倒された時の処理
//=============================================================================
void CEnemy::KnockDown(void)
{
	if (m_bKnockDown == false)
	{
		// プレイヤーに経験値を渡す
		SendExp();

		// スコアを加算
		CManager::GetDataClear()->AddScore(m_nScore);

		// ライフを0に設定
		if (m_pLife != NULL)
		{
			m_fLife = 0;
			m_pLife->SetLife(m_fLife);
		}

		// コリジョンを消す
		for (int nCollision = 0; nCollision < MAX_ENEMY_COLLSION; nCollision++)
		{
			if (m_pCollisionS[nCollision] != NULL)
			{
				m_pCollisionS[nCollision]->Uninit();
				m_pCollisionS[nCollision] = NULL;
			}
		}

		// 影の終了処理
		if (m_pShadow != NULL)
		{
			m_pShadow->Uninit();
			m_pShadow = NULL;
		}

		m_bKnockDown = true;
	}

	switch (m_type)
	{
		// ボス
	case ENEMY_TYPE_BOSS:
		if (m_pMotion->GetConnect() == false && m_pMotion->GetMotion() != 3)
		{
			// ダウンモーションにする
			m_pMotion->SetMotion(3);
		}
		break;
		// ザコ敵
	default:
		if (m_pMotion->GetConnect() == false && m_pMotion->GetMotion() != 4)
		{
			// ダウンモーションにする
			m_pMotion->SetMotion(4);
		}

		// ライフを消す
		if (m_pLifeBill != NULL)
		{
			m_pLifeBill->Uninit();
			m_pLifeBill = NULL;
		}

		// カウントを進める
		m_nDeathCount++;

		// 2秒経ったら消滅
		if (m_nDeathCount >= 120)
		{
			Uninit();
		}
		break;
	}
}

//=============================================================================
// 動作させるどうかの設定処理
//=============================================================================
void CEnemy::CanControl(CEnemy *pEnemy)
{
	//※敵の数が多いと、全ての敵が動作する際に処理落ちが発生するため、プレイヤーに近い敵を動作させる
	m_bCanControl = false;

	// オブジェクト情報を入れるポインタ・その情報を保存するポインタ
	CScene *pObject = NULL;
	CScene *pSaveObject = NULL;

	// 先頭のポインタを代入
	pObject = pObject->GetTopObj(CScene::PRIORITY_MODEL);

	while (pObject != NULL)
	{
		// 現在のオブジェクトのポインタを保存
		pSaveObject = pObject;
		if (pObject->GetObjType() == CScene::OBJECT_TYPE_PLAYER)
		{
			// プレイヤークラスにキャスト
			CPlayer *pPlayer = NULL;
			pPlayer = (CPlayer*)pObject;

			D3DXVECTOR3 plyPos = pPlayer->GetPos();		// プレイヤーの位置
			D3DXVECTOR3 enePos = pEnemy->GetPos();		// 敵の位置

			float fRadius = sqrtf((plyPos.x - enePos.x) * (plyPos.x - enePos.x) + (plyPos.z - enePos.z) * (plyPos.z - enePos.z));

			// プレイヤーとの距離が4000.0f以上離れていたら
			if (fRadius >= 4000.0f)
			{
				// 動作させない
				m_bCanControl = false;
			}
			else
			{
				// 動作させる
				m_bCanControl = true;
			}
		}
		// 次のオブジェクトを取得
		pObject = pSaveObject->GetObjNext(pSaveObject);
	}
}

//=============================================================================
// モデル毎の位置設定処理
//=============================================================================
void CEnemy::SetModelPos(int nCntModel, D3DXVECTOR3 pos)
{
	if (m_apModel[nCntModel] != NULL)
	{
		m_apModel[nCntModel]->SetPos(pos);
	}
}

//=============================================================================
// モデル毎の位置取得処理
//=============================================================================
D3DXVECTOR3 CEnemy::GetModelPos(int nCntModel)
{
	if (m_apModel[nCntModel] != NULL)
	{
		return m_apModel[nCntModel]->GetPos();
	}
	else
	{
		return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//=============================================================================
// モデル毎の向き設定処理
//=============================================================================
void CEnemy::SetModelRot(int nCntModel, D3DXVECTOR3 rot)
{
	if (m_apModel[nCntModel] != NULL)
	{
		m_apModel[nCntModel]->SetRot(rot);
	}
}

//=============================================================================
// モデル毎の向き取得処理
//=============================================================================
D3DXVECTOR3 CEnemy::GetModelRot(int nCntModel)
{
	if (m_apModel[nCntModel] != NULL)
	{
		return m_apModel[nCntModel]->GetRot();
	}
	else
	{
		return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//=============================================================================
// エネミーのモデル取得処理
//=============================================================================
CModel *CEnemy::GetModel(int nCntModel)
{
	if (m_apModel[nCntModel] != NULL)
	{
		return m_apModel[nCntModel];
	}
	else
	{
		return false;
	}
}