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
#include "editBuilding.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* SETUP_TXT	= "data\\TXT\\save_stage.txt";	// ステージセーブテキスト
	const float INIT_MOVE	= 40.0f;	// 配置物の移動量
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
	m_pBuilding = NULL;			// エディットビルの情報
	m_pos	= VEC3_ZERO;		// 位置
	m_rot	= VEC3_ZERO;		// 向き
	m_fMove	= 0.0f;				// 位置移動量
	m_thing	= THING_BUILDING;	// 配置物

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
	m_pBuilding = NULL;			// エディットビルの情報
	m_pos	= VEC3_ZERO;		// 位置
	m_rot	= VEC3_ZERO;		// 向き
	m_fMove	= INIT_MOVE;		// 位置移動量
	m_thing	= THING_BUILDING;	// 配置物

	// エディットビルの生成
	m_pBuilding = CEditBuilding::Create(this);
	if (m_pBuilding == NULL)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

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

	if (m_pBuilding != NULL)
	{ // エディットビルが使用されている場合

		// エディットビルの破棄
		CEditBuilding::Release(m_pBuilding);
	}
	else { assert(false); }	// 非使用中

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

		if (m_pBuilding != NULL)
		{ // エディットビルが使用されている場合

			// エディットビルの更新
			m_pBuilding->Update();
		}
		else { assert(false); }	// 非使用中

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
//	位置取得処理
//============================================================
D3DXVECTOR3 CEditStageManager::GetVec3Position(void) const
{
	// 位置を返す
	return m_pos;
}

//============================================================
//	向き取得処理
//============================================================
D3DXVECTOR3 CEditStageManager::GetVec3Rotation(void) const
{
	// 向きを返す
	return m_rot;
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
