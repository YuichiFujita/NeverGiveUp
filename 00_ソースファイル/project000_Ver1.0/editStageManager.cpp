//============================================================
//
//	エディットステージマネージャー処理 [editStageManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "editStageManager.h"
#include "manager.h"
#include "input.h"
#include "effect3D.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* SETUP_TXT	= "data\\TXT\\save_stage.txt";	// ステージセーブテキスト
	const float INIT_MOVE	= 40.0f;	// 配置物の移動量
	const float INIT_ALPHA	= 0.5f;		// 配置前のα値
}

//************************************************************
//	親クラス [CEditStageManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEditStageManager::CEditStageManager()
{
#if _DEBUG

	// メンバ変数をクリア
	m_pos = VEC3_ZERO;	// 位置
	m_rot = VEC3_ZERO;	// 向き
	m_fMove = 0.0f;		// 位置移動量
	m_thing = THING_BUILDING;	// 配置物
	memset(&m_building, 0, sizeof(m_building));	// ビル配置情報

#endif	// _DEBUG
}

//============================================================
//	デストラクタ
//============================================================
CEditStageManager::~CEditStageManager()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	初期化処理
//============================================================
HRESULT CEditStageManager::Init(void)
{
#if _DEBUG

	// メンバ変数を初期化
	m_pos = VEC3_ZERO;	// 位置
	m_rot = VEC3_ZERO;	// 向き
	m_fMove = INIT_MOVE;	// 位置移動量
	m_thing = THING_BUILDING;	// 配置物

	// ビル配置情報を初期化
	m_building.type = CBuilding::TYPE_00;	// ビル種類

	// ビルの生成	// TODO：種類の設定で位置と向き以外の情報は決まるように変更
	m_building.pBuilding = CBuilding::Create(m_building.type, m_pos, m_rot, D3DXVECTOR3(280.0f, 560.0f, 280.0f), XCOL_WHITE, CBuilding::COLLISION_GROUND);
	if (m_building.pBuilding == NULL)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 自動更新をOFFにする
	m_building.pBuilding->SetEnableUpdate(false);

	// 色を設定
	D3DXCOLOR col = m_building.pBuilding->GetCubeColor();	// 元の色を取得
	m_building.pBuilding->SetCubeColor(D3DXCOLOR(col.r, col.g, col.b, INIT_ALPHA));

	// 成功を返す
	return S_OK;

#else	// NDEBUG

	// 成功を返す
	return S_OK;

#endif	// _DEBUG
}

//============================================================
//	終了処理
//============================================================
void CEditStageManager::Uninit(void)
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	更新処理
//============================================================
void CEditStageManager::Update(void)
{
#if _DEBUG

	// ポインタを宣言
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// キーボード情報

	// 位置を変更
	if (m_pKeyboard->IsTrigger(DIK_S))
	{
		m_pos.z += m_fMove;
	}
	if (m_pKeyboard->IsTrigger(DIK_W))
	{
		m_pos.z -= m_fMove;
	}
	if (m_pKeyboard->IsTrigger(DIK_A))
	{
		m_pos.x += m_fMove;
	}
	if (m_pKeyboard->IsTrigger(DIK_D))
	{
		m_pos.x -= m_fMove;
	}
	if (m_pKeyboard->IsTrigger(DIK_E))
	{
		m_pos.y += m_fMove;
	}
	if (m_pKeyboard->IsTrigger(DIK_Q))
	{
		m_pos.y -= m_fMove;
	}

	// 向きを変更
	if (m_pKeyboard->IsTrigger(DIK_Z))
	{
		m_rot.y += HALF_PI;
		useful::Vec3NormalizeRot(m_rot);
	}
	if (m_pKeyboard->IsTrigger(DIK_C))
	{
		m_rot.y -= HALF_PI;
		useful::Vec3NormalizeRot(m_rot);
	}

	switch (m_thing)
	{ // 配置物ごとの処理
	case THING_BUILDING:	// ビル

		// ビル配置の更新
		UpdateBuilding();

		break;

#if 0

	case THING_SIGNBOARD:	// 看板
		break;
	case THING_OBSTACLE:	// 障害物
		break;
	case THING_SAVEPOINT:	// セーブポイント
		break;
	case THING_GOALPOINT:	// ゴールポイント
		break;

#endif

	default:	// 例外処理
		assert(false);
		break;
	}

#endif	// _DEBUG
}

//============================================================
//	生成処理
//============================================================
CEditStageManager *CEditStageManager::Create(void)
{
#if _DEBUG

	// ポインタを宣言
	CEditStageManager *pEditStageManager = NULL;	// エディットステージマネージャー生成用

	if (pEditStageManager == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pEditStageManager = new CEditStageManager;	// エディットステージマネージャー
	}
	else { assert(false); return NULL; }	// 使用中

	if (pEditStageManager != NULL)
	{ // 使用されている場合
		
		// エディットステージマネージャーの初期化
		if (FAILED(pEditStageManager->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pEditStageManager;
			pEditStageManager = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pEditStageManager;
	}
	else { assert(false); return NULL; }	// 確保失敗

#else	// NDEBUG

	// NULLを返す
	return NULL;

#endif	// _DEBUG
}

//============================================================
//	破棄処理
//============================================================
HRESULT CEditStageManager::Release(CEditStageManager *&prEditStageManager)
{
#if _DEBUG

	if (prEditStageManager != NULL)
	{ // 使用中の場合

		// エディットステージマネージャーの終了
		prEditStageManager->Uninit();

		// メモリ開放
		delete prEditStageManager;
		prEditStageManager = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中

#else	// NDEBUG

	// 成功を返す
	return S_OK;

#endif	// _DEBUG
}

//============================================================
//	ビル配置の更新処理
//============================================================
void CEditStageManager::UpdateBuilding(void)
{
	// ポインタを宣言
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// キーボード情報

	//--------------------------------------------------------
	//	操作の更新
	//--------------------------------------------------------
	// 種類を変更
	if (m_pKeyboard->IsTrigger(DIK_1))
	{
		m_building.type = (CBuilding::EType)((m_building.type + 1) % CBuilding::TYPE_MAX);
	}

	// ビルを配置
	if (m_pKeyboard->IsTrigger(DIK_0))
	{
		//----------------------------------------------------
		//	ビルの情報を配置用に変更
		//----------------------------------------------------
		// 自動更新をONにする
		m_building.pBuilding->SetEnableUpdate(true);

		// 色を設定
		D3DXCOLOR colSet = m_building.pBuilding->GetCubeColor();	// 元の色を取得
		m_building.pBuilding->SetCubeColor(D3DXCOLOR(colSet.r, colSet.g, colSet.b, 1.0f));

		//----------------------------------------------------
		//	新しいビルの生成
		//----------------------------------------------------
		// ビルの生成	// TODO：種類の設定で位置と向き以外の情報は決まるように変更
		m_building.pBuilding = CBuilding::Create(m_building.type, m_pos, m_rot, D3DXVECTOR3(280.0f, 560.0f, 280.0f), XCOL_WHITE, CBuilding::COLLISION_GROUND);
		assert(m_building.pBuilding != NULL);

		// 自動更新をOFFにする
		m_building.pBuilding->SetEnableUpdate(false);

		// 色を設定
		D3DXCOLOR colNew = m_building.pBuilding->GetCubeColor();	// 元の色を取得
		m_building.pBuilding->SetCubeColor(D3DXCOLOR(colNew.r, colNew.g, colNew.b, INIT_ALPHA));
	}

	//--------------------------------------------------------
	//	表示の更新
	//--------------------------------------------------------
	// プレイヤーの進行方向にエフェクトを表示
	D3DXVECTOR3 posEffect = VEC3_ZERO;	// エフェクト位置
	D3DXVECTOR3 sizeBuilding = m_building.pBuilding->GetVec3Sizing();	// ビル大きさ

	// エフェクト位置を計算
	posEffect.x = m_pos.x + sinf(m_rot.y + D3DX_PI) * sizeBuilding.x;
	posEffect.y = m_pos.y + sizeBuilding.y * 2.0f;
	posEffect.z = m_pos.z + cosf(m_rot.y + D3DX_PI) * sizeBuilding.z;

	// エフェクト表示
	CEffect3D::Create(posEffect, 30.0f, CEffect3D::TYPE_NORMAL, 10);

	// 位置を反映
	m_building.pBuilding->SetVec3Position(m_pos);

	// 向きを反映
	m_building.pBuilding->SetVec3Rotation(m_rot);

	// 種類を反映
	m_building.pBuilding->SetType(m_building.type);
}
