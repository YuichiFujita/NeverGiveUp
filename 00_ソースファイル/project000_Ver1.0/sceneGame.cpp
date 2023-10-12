//============================================================
//
//	ゲーム画面処理 [sceneGame.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "sceneGame.h"
#include "manager.h"
#include "sound.h"
#include "input.h"

#include "gameManager.h"
#include "timerManager.h"
#include "stage.h"
#include "pause.h"
#include "score.h"

#include "wall.h"
#include "scenery.h"
#include "sky.h"
#include "building.h"
#include "obstacle.h"

//************************************************************
//	マクロ定義
//************************************************************
#define TIME_POS		(D3DXVECTOR3(40.0f, 50.0f, 0.0f))			// タイマー位置
#define TIME_VAL_SIZE	(D3DXVECTOR3(60.0f, 80.0f, 0.0f))			// タイマー数字大きさ
#define TIME_PART_SIZE	(D3DXVECTOR3(35.0f, 80.0f, 0.0f))			// タイマー区切り大きさ
#define TIME_VAL_SPACE	(D3DXVECTOR3(TIME_VAL_SIZE.x, 0.0f, 0.0f))	// タイマー数字空白
#define TIME_PART_SPACE	(D3DXVECTOR3(TIME_PART_SIZE.x, 0.0f, 0.0f))	// タイマー区切り空白

#define SCO_POS		(D3DXVECTOR3(825.0f, 50.0f, 0.0f))		// スコア位置
#define SCO_SIZE	(D3DXVECTOR3(60.0f, 80.0f, 0.0f))		// スコア大きさ
#define SCO_SPACE	(D3DXVECTOR3(SCO_SIZE.x, 0.0f, 0.0f))	// スコア空白

#define WARNING_SIZE	(D3DXVECTOR3(1000.0f, 500.0f, 0.0f))	// 警告大きさ

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CGameManager	*CSceneGame::m_pGameManager  = NULL;	// ゲームマネージャー
CTimerManager	*CSceneGame::m_pTimerManager = NULL;	// タイマーマネージャー
CWarningSpawn	*CSceneGame::m_pWarningSpawn = NULL;	// 出現警告表示オブジェクト
CPause	*CSceneGame::m_pPause	= NULL;					// ポーズ
CScore	*CSceneGame::m_pScore	= NULL;					// スコアオブジェクト

bool CSceneGame::m_bControlCamera = false;	// カメラの操作状況
bool CSceneGame::m_bDrawUI = true;			// UIの描画状況
bool CSceneGame::m_bDrawPause = true;		// ポーズの描画状況

//************************************************************
//	子クラス [CSceneGame] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CSceneGame::CSceneGame(const EMode mode) : CScene(mode)
{

}

//============================================================
//	デストラクタ
//============================================================
CSceneGame::~CSceneGame()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CSceneGame::Init(void)
{
	// タイマーマネージャーの生成
	m_pTimerManager = CTimerManager::Create
	( // 引数
		TIME_POS,		// 位置
		TIME_VAL_SIZE,	// 数字の大きさ
		TIME_PART_SIZE,	// 区切りの大きさ
		TIME_VAL_SPACE,	// 数字の空白
		TIME_PART_SPACE	// 区切りの空白
	);
	if (m_pTimerManager == NULL)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// スコアオブジェクトの生成
	m_pScore = CScore::Create
	( // 引数
		SCO_POS,	// 位置
		SCO_SIZE,	// 大きさ
		SCO_SPACE	// 空白
	);
	if (m_pScore == NULL)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// シーンの初期化
	CScene::Init();		// ステージ・地面・プレイヤーの生成

	// ゲームマネージャーの生成
	m_pGameManager = CGameManager::Create();
	if (m_pGameManager == NULL)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// ポーズの生成
	m_pPause = CPause::Create();
	if (m_pPause == NULL)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

#if 1	// TODO：壁

	// 壁オブジェクトの生成
	CWall::Create(CWall::TEXTURE_NORMAL, D3DXVECTOR3( 0.0f,    0.0f, -3000.0f), D3DXToRadian(D3DXVECTOR3(0.0f, 180.0f, 0.0f)), D3DXVECTOR2(6000.0f, 400.0f), XCOL_WHITE, POSGRID2(18, 1));
	CWall::Create(CWall::TEXTURE_NORMAL, D3DXVECTOR3(-3000.0f, 0.0f,  0.0f),    D3DXToRadian(D3DXVECTOR3(0.0f, 270.0f, 0.0f)), D3DXVECTOR2(6000.0f, 400.0f), XCOL_WHITE, POSGRID2(18, 1));
	CWall::Create(CWall::TEXTURE_NORMAL, D3DXVECTOR3( 0.0f,    0.0f,  3000.0f), D3DXToRadian(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),   D3DXVECTOR2(6000.0f, 400.0f), XCOL_WHITE, POSGRID2(18, 1));
	CWall::Create(CWall::TEXTURE_NORMAL, D3DXVECTOR3( 3000.0f, 0.0f,  0.0f),    D3DXToRadian(D3DXVECTOR3(0.0f, 90.0f, 0.0f)),  D3DXVECTOR2(6000.0f, 400.0f), XCOL_WHITE, POSGRID2(18, 1));

#endif

	// 景色オブジェクトの生成
	CScenery::Create(CScenery::TEXTURE_NORMAL, VEC3_ZERO, VEC3_ZERO,                                    XCOL_WHITE,                        POSGRID2(32, 1), 12000.0f, 1000.0f, D3DCULL_CW, false);
	CScenery::Create(CScenery::TEXTURE_NORMAL, VEC3_ZERO, D3DXToRadian(D3DXVECTOR3(0.0f, 85.0f, 0.0f)), D3DXCOLOR(0.7f, 1.0f, 1.0f, 1.0f), POSGRID2(32, 1), 14000.0f, 1600.0f, D3DCULL_CW, false);
	CScenery::Create(CScenery::TEXTURE_NORMAL, VEC3_ZERO, D3DXToRadian(D3DXVECTOR3(0.0f, 35.0f, 0.0f)), D3DXCOLOR(0.4f, 1.0f, 0.7f, 1.0f), POSGRID2(32, 1), 16000.0f, 2200.0f, D3DCULL_CW, false);

	// 空オブジェクトの生成
	CSky::Create(CSky::TEXTURE_NORMAL, VEC3_ZERO, VEC3_ZERO, XCOL_WHITE, POSGRID2(32, 6), 18000.0f, D3DCULL_CW, false);

#if 1	// TODO：ビル

	// ビルオブジェクトの生成
	CBuilding::Create(CBuilding::TYPE_NORMAL, D3DXVECTOR3(0.0f,     0.0f, 0.0f), VEC3_ZERO, D3DXVECTOR3(180.0f, 360.0f, 180.0f), XCOL_WHITE);
	CBuilding::Create(CBuilding::TYPE_NORMA,  D3DXVECTOR3(-360.0f,  0.0f, 0.0f), VEC3_ZERO, D3DXVECTOR3(180.0f, 360.0f, 180.0f), XCOL_WHITE);
	CBuilding::Create(CBuilding::TYPE_NORM,   D3DXVECTOR3(-720.0f,  0.0f, 0.0f), VEC3_ZERO, D3DXVECTOR3(180.0f, 360.0f, 180.0f), XCOL_WHITE);
	CBuilding::Create(CBuilding::TYPE_NORMAL, D3DXVECTOR3(-1080.0f, 0.0f, 0.0f), VEC3_ZERO, D3DXVECTOR3(180.0f, 360.0f, 180.0f), XCOL_WHITE);
	CBuilding::Create(CBuilding::TYPE_NORM,   D3DXVECTOR3(-1440.0f, 0.0f, 0.0f), VEC3_ZERO, D3DXVECTOR3(180.0f, 360.0f, 180.0f), XCOL_WHITE);
	CBuilding::Create(CBuilding::TYPE_NORMA,  D3DXVECTOR3(-1800.0f, 0.0f, 0.0f), VEC3_ZERO, D3DXVECTOR3(180.0f, 360.0f, 180.0f), XCOL_WHITE);

#endif

#if 1	// TODO：障害物

	// 障害物オブジェクトの生成
	CObstacle::Create(CObstacle::TYPE_NORMAL, D3DXVECTOR3(-1200.0f, 720.0f, 0.0f), D3DXToRadian(D3DXVECTOR3(0.0f, 90.0f, 0.0f)));

#endif

	//--------------------------------------------------------
	//	初期設定
	//--------------------------------------------------------
	// カメラを設定
	CManager::GetInstance()->GetCamera()->SetState(CCamera::STATE_FOLLOW);	// カメラを追従状態に設定
	CManager::GetInstance()->GetCamera()->SetDestFollow();	// 目標位置を設定

	// タイムを計測開始
	m_pTimerManager->Start();	// 計測を開始

	// UIの描画状況を設定
	SetEnableDrawUI(m_bDrawUI);

	// ポーズの描画状況を設定
	SetEnableDrawPause(m_bDrawPause);

	// BGMの再生
	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_GAME);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
HRESULT CSceneGame::Uninit(void)
{
	// ゲームマネージャーの破棄
	if (FAILED(CGameManager::Release(m_pGameManager)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// タイマーマネージャーの破棄
	if (FAILED(CTimerManager::Release(m_pTimerManager)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// ポーズの破棄
	if (FAILED(CPause::Release(m_pPause)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 終了済みのオブジェクトポインタをNULLにする
	m_pScore = NULL;	// スコアオブジェクト

	// シーンの終了
	CScene::Uninit();

	// 成功を返す
	return S_OK;
}

//============================================================
//	更新処理
//============================================================
void CSceneGame::Update(void)
{
#if _DEBUG

	if (CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_F2))
	{
		// UIの描画状況を反転
		SetEnableDrawUI((!m_bDrawUI) ? true : false);
	}
	else if (CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_F3))
	{
		// ポーズの描画状況を反転
		SetEnableDrawPause((!m_bDrawPause) ? true : false);
	}
	else if (CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_F4))
	{
		// カメラの操作状況を反転
		SetEnableControlCamera((!m_bControlCamera) ? true : false);
	}
	else if (CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_F5))
	{
		// リザルトに遷移
		CManager::GetInstance()->SetScene(CScene::MODE_RESULT);	// リザルト画面
	}
	// デバッグ表示
	CManager::GetInstance()->GetDebugProc()->Print("[F2]：UI描画のON/OFF\n");
	CManager::GetInstance()->GetDebugProc()->Print("[F3]：ポーズ描画のON/OFF\n");
	CManager::GetInstance()->GetDebugProc()->Print("[F4]：カメラ操作のON/OFF\n");
	CManager::GetInstance()->GetDebugProc()->Print("[F5]：リザルト遷移\n");

#endif

	if (m_pTimerManager != NULL)
	{ // 使用中の場合

		// タイマーマネージャーの更新
		m_pTimerManager->Update();
	}
	else { assert(false); }	// 非使用中

	if (m_pPause != NULL)
	{ // 使用中の場合

		// ポーズの更新
		m_pPause->Update();
	}
	else { assert(false); }	// 非使用中

	if (!m_pPause->IsPause())
	{ // ポーズ中ではない場合

		if (m_pGameManager != NULL)
		{ // 使用中の場合

			// ゲームマネージャーの更新
			m_pGameManager->Update();
		}
		else { assert(false); }	// 非使用中

		// シーンの更新
		CScene::Update();
	}

#if _DEBUG

	else
	{ // ポーズ中の場合

		if (CManager::GetInstance()->GetCamera()->GetState() == CCamera::STATE_CONTROL)
		{ // カメラが操作状態の場合

			// カメラの更新
			CManager::GetInstance()->GetCamera()->Update();
		}
	}

#endif
}

//============================================================
//	描画処理
//============================================================
void CSceneGame::Draw(void)
{

}

//============================================================
//	ゲームマネージャー取得処理
//============================================================
CGameManager *CSceneGame::GetGameManager(void)
{
	// ゲームマネージャーのポインタを返す
	return m_pGameManager;
}

//============================================================
//	タイマーマネージャー取得処理
//============================================================
CTimerManager *CSceneGame::GetTimerManager(void)
{
	// タイマーマネージャーのポインタを返す
	return m_pTimerManager;
}

//============================================================
//	ポーズ取得処理
//============================================================
CPause *CSceneGame::GetPause(void)
{
	// ポーズのポインタを返す
	return m_pPause;
}

//============================================================
//	スコア取得処理
//============================================================
CScore *CSceneGame::GetScore(void)
{
	// スコアのポインタを返す
	return m_pScore;
}

//============================================================
//	出現警告表示取得処理
//============================================================
CWarningSpawn *CSceneGame::GetWarningSpawn(void)
{
	// 出現警告表示のポインタを返す
	return m_pWarningSpawn;
}

//============================================================
//	UIの描画状況の設定処理
//============================================================
void CSceneGame::SetEnableDrawUI(const bool bDraw)
{
	// 引数のUIの描画状況を設定
	m_bDrawUI = bDraw;

	// タイマーの描画状況を設定
	m_pTimerManager->SetEnableDraw(bDraw);

	// スコアの描画状況を設定
	m_pScore->SetEnableDraw(bDraw);
}

//============================================================
//	UIの描画状況取得処理
//============================================================
bool CSceneGame::IsDrawUI(void)
{
	// UIの描画状況を返す
	return m_bDrawUI;
}

//============================================================
//	ポーズの描画状況の設定処理
//============================================================
void CSceneGame::SetEnableDrawPause(const bool bDraw)
{
	// 引数のポーズの描画状況を設定
	m_bDrawPause = bDraw;

	// ポーズの描画状況を設定
	m_pPause->SetEnableDraw(m_pPause->IsPause());
}

//============================================================
//	ポーズの描画状況取得処理
//============================================================
bool CSceneGame::IsDrawPause(void)
{
	// ポーズの描画状況を返す
	return m_bDrawPause;
}

//============================================================
//	カメラの操作状況の設定処理
//============================================================
void CSceneGame::SetEnableControlCamera(const bool bControl)
{
	// 引数のカメラ操作状況を設定
	m_bControlCamera = bControl;

	// カメラの操作状況を設定
	if (bControl)
	{ // 操作する状況の場合

		// 操作状態に変更
		CManager::GetInstance()->GetCamera()->SetState(CCamera::STATE_CONTROL);
	}
	else
	{ // 操作しない状況の場合

		// 追従状態に変更
		CManager::GetInstance()->GetCamera()->SetState(CCamera::STATE_FOLLOW);
	}
}
