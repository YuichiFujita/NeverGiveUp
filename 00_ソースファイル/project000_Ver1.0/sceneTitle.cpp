//============================================================
//
//	タイトル画面処理 [sceneTitle.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "sceneTitle.h"
#include "manager.h"
#include "texture.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "titleManager.h"

#include "stage.h"
#include "field.h"
#include "wall.h"
#include "scenery.h"
#include "sky.h"

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CTitleManager *CSceneTitle::m_pTitleManager = NULL;	// タイトルマネージャー

//************************************************************
//	子クラス [CSceneTitle] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CSceneTitle::CSceneTitle(const EMode mode) : CScene(mode)
{

}

//============================================================
//	デストラクタ
//============================================================
CSceneTitle::~CSceneTitle()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CSceneTitle::Init(void)
{
	//--------------------------------------------------------
	//	タイトルの初期化
	//--------------------------------------------------------
	// タイトルマネージャーの生成
	m_pTitleManager = CTitleManager::Create();
	if (m_pTitleManager == NULL)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// シーンの初期化
	CScene::Init();

	//--------------------------------------------------------
	//	オブジェクト生成・初期化
	//--------------------------------------------------------
#if 0	// TODO：壁

	// 壁オブジェクトの生成
	CWall::Create(CWall::TEXTURE_NORMAL, D3DXVECTOR3( 0.0f,    0.0f, -3000.0f), D3DXToRadian(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),   D3DXVECTOR2(6000.0f, 400.0f), XCOL_WHITE, POSGRID2(18, 1));
	CWall::Create(CWall::TEXTURE_NORMAL, D3DXVECTOR3(-3000.0f, 0.0f,  0.0f),    D3DXToRadian(D3DXVECTOR3(0.0f, 90.0f, 0.0f)),  D3DXVECTOR2(6000.0f, 400.0f), XCOL_WHITE, POSGRID2(18, 1));
	CWall::Create(CWall::TEXTURE_NORMAL, D3DXVECTOR3( 0.0f,    0.0f,  3000.0f), D3DXToRadian(D3DXVECTOR3(0.0f, 180.0f, 0.0f)), D3DXVECTOR2(6000.0f, 400.0f), XCOL_WHITE, POSGRID2(18, 1));
	CWall::Create(CWall::TEXTURE_NORMAL, D3DXVECTOR3( 3000.0f, 0.0f,  0.0f),    D3DXToRadian(D3DXVECTOR3(0.0f, 270.0f, 0.0f)), D3DXVECTOR2(6000.0f, 400.0f), XCOL_WHITE, POSGRID2(18, 1));

#endif

	// 景色オブジェクトの生成
	CScenery::Create(CScenery::TEXTURE_BILL_WHITE,     VEC3_ZERO, VEC3_ZERO,                                    XCOL_WHITE, POSGRID2(14, 1), 12000.0f, 800.0f,  D3DCULL_CW, false);
	CScenery::Create(CScenery::TEXTURE_BILL_LIGHTBLUE, VEC3_ZERO, D3DXToRadian(D3DXVECTOR3(0.0f, 85.0f, 0.0f)), XCOL_WHITE, POSGRID2(14, 1), 14000.0f, 1400.0f, D3DCULL_CW, false);
	CScenery::Create(CScenery::TEXTURE_BILL_BLUE,      VEC3_ZERO, D3DXToRadian(D3DXVECTOR3(0.0f, 35.0f, 0.0f)), XCOL_WHITE, POSGRID2(14, 1), 16000.0f, 2000.0f, D3DCULL_CW, false);

	// 空オブジェクトの生成
	CSky::Create(CSky::TEXTURE_NORMAL, VEC3_ZERO, VEC3_ZERO, XCOL_WHITE, POSGRID2(32, 6), 18000.0f, D3DCULL_CW, false);

	//--------------------------------------------------------
	//	初期設定
	//--------------------------------------------------------
	// カメラを設定
	CManager::GetInstance()->GetCamera()->SetState(CCamera::STATE_ROTATE);	// カメラを回転状態に設定
	CManager::GetInstance()->GetCamera()->SetDestRotate();					// 目標位置を設定
	CManager::GetInstance()->GetCamera()->SetEnableUpdate(false);			// 更新を停止

	// BGMの再生
	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_TITLE_RESULT);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
HRESULT CSceneTitle::Uninit(void)
{
	// タイトルマネージャーの破棄
	if (FAILED(CTitleManager::Release(m_pTitleManager)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// シーンの終了
	CScene::Uninit();

	// 成功を返す
	return S_OK;
}

//============================================================
//	更新処理
//============================================================
void CSceneTitle::Update(void)
{
	// デバッグ表示
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "======================================\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "　[デバッグ情報]\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "======================================\n");

	if (m_pTitleManager != NULL)
	{ // 使用中の場合

		// タイトルマネージャーの更新
		m_pTitleManager->Update();
	}
	else { assert(false); }	// 非使用中

	// シーンの更新
	CScene::Update();
}

//============================================================
//	描画処理
//============================================================
void CSceneTitle::Draw(void)
{

}

//============================================================
//	タイトルマネージャー取得処理
//============================================================
CTitleManager *CSceneTitle::GetTitleManager(void)
{
	// タイトルマネージャーを返す
	return m_pTitleManager;
}
