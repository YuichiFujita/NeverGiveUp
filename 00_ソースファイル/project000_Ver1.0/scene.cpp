//============================================================
//
//	シーン処理 [scene.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "scene.h"
#include "manager.h"
#include "renderer.h"
#include "light.h"
#include "camera.h"
#include "sceneTitle.h"
#include "sceneTutorial.h"
#include "sceneGame.h"
#include "sceneResult.h"

#include "stage.h"
#include "player.h"
#include "field.h"

//************************************************************
//	マクロ定義
//************************************************************
#define FIELD_SIZE	(D3DXVECTOR2(6000.0f, 6000.0f))	// 地面の大きさ
#define FIELD_PART	(POSGRID2(120, 120))			// 地面の分割数

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CStage *CScene::m_pStage = NULL;	// ステージ
CPlayer	*CScene::m_pPlayer = NULL;	// プレイヤーオブジェクト
CField *CScene::m_pField = NULL;	// 地面オブジェクト

//************************************************************
//	親クラス [CScene] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CScene::CScene(const EMode mode)
{
	// メンバ変数をクリア
	m_mode = mode;	// モード
}

//============================================================
//	デストラクタ
//============================================================
CScene::~CScene()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CScene::Init(void)
{
	// ステージの生成
	m_pStage = CStage::Create();
	if (m_pStage == NULL)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 地面オブジェクトの生成
	m_pField = CField::Create(CField::TEXTURE_FIELD, VEC3_ZERO, VEC3_ZERO, FIELD_SIZE, XCOL_WHITE, FIELD_PART);
	if (m_pField == NULL)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// プレイヤーオブジェクトの生成
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(-2000.0f, 480.0f, 0.0f), VEC3_ZERO);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
HRESULT CScene::Uninit(void)
{
	// ステージの破棄
	if (FAILED(CStage::Release(m_pStage)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 終了済みのオブジェクトポインタをNULLにする
	m_pPlayer = NULL;	// プレイヤーオブジェクト
	m_pField = NULL;	// 地面オブジェクト

	// 成功を返す
	return S_OK;
}

//============================================================
//	更新処理
//============================================================
void CScene::Update(void)
{
	if (m_pStage != NULL)
	{ // 使用中の場合

		// ステージの更新
		m_pStage->Update();
	}
	else { assert(false); }	// 非使用中

	if (CManager::GetInstance()->GetLight() != NULL)
	{ // 使用中の場合

		// ライトの更新
		CManager::GetInstance()->GetLight()->Update();
	}
	else { assert(false); }	// 非使用中

	if (CManager::GetInstance()->GetCamera() != NULL)
	{ // 使用中の場合

		// カメラの更新
		CManager::GetInstance()->GetCamera()->Update();
	}
	else { assert(false); }	// 非使用中

	if (CManager::GetInstance()->GetRenderer() != NULL)
	{ // 使用中の場合

		// レンダラーの更新
		CManager::GetInstance()->GetRenderer()->Update();
	}
	else { assert(false); }	// 非使用中
}

//============================================================
//	生成処理
//============================================================
CScene *CScene::Create(EMode mode)
{
	// ポインタを宣言
	CScene *pScene = NULL;	// シーン生成用

	if (pScene == NULL)
	{ // 使用されていない場合

		// メモリ確保
		switch (mode)
		{ // モードごとの処理
		case MODE_TITLE:

			// タイトル画面を生成
			pScene = new CSceneTitle(mode);

			break;

		case MODE_TUTORIAL:

			// チュートリアル画面を生成
			pScene = new CSceneTutorial(mode);

			break;

		case MODE_GAME:

			// ゲーム画面を生成
			pScene = new CSceneGame(mode);

			break;

		case MODE_RESULT:

			// リザルト画面を生成
			pScene = new CSceneResult(mode);

			break;

		default:	// 例外処理
			assert(false);
			break;
		}
	}
	else { assert(false); return NULL; }	// 使用中

	if (pScene != NULL)
	{ // 確保に成功している場合

		// シーンの初期化
		if (FAILED(pScene->Init()))
		{ // 初期化に失敗した場合

			// シーンの終了
			if (FAILED(pScene->Uninit()))
			{ // シーンの終了に失敗した場合

				// メモリ開放
				delete pScene;
				pScene = NULL;

				// 失敗を返す
				assert(false);
				return NULL;
			}

			// メモリ開放
			delete pScene;
			pScene = NULL;

			// 失敗を返す
			assert(false);
			return NULL;
		}

		// 確保したアドレスを返す
		return pScene;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
HRESULT CScene::Release(CScene *&prScene)
{
	if (prScene != NULL)
	{ // 使用中の場合

		// シーンの終了
		if (FAILED(prScene->Uninit()))
		{ // シーンの終了に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// メモリ開放
		delete prScene;
		prScene = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}

//============================================================
//	ステージ取得処理
//============================================================
CStage *CScene::GetStage(void)
{
	// ステージのポインタを返す
	return m_pStage;
}

//============================================================
//	プレイヤー取得処理
//============================================================
CPlayer *CScene::GetPlayer(void)
{
	// プレイヤーのポインタを返す
	return m_pPlayer;
}

//============================================================
//	地面取得処理
//============================================================
CField *CScene::GetField(void)
{
	// 地面のポインタを返す
	return m_pField;
}

//============================================================
//	モードの設定処理
//============================================================
void CScene::SetMode(const EMode mode)
{
	// 引数のモードを設定
	m_mode = mode;
}

//============================================================
//	モード取得処理
//============================================================
CScene::EMode CScene::GetMode(void) const
{
	// 現在のモードを返す
	return m_mode;
}
