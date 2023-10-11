//============================================================
//
//	チュートリアルマネージャー処理 [tutorialManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "tutorialManager.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "texture.h"
#include "object2D.h"

//************************************************************
//	マクロ定義
//************************************************************
#define TUTORIAL_PRIO	(14)	// チュートリアルの優先順位

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CTutorialManager::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\tutorial000.png",	// 説明テクスチャ
};

//************************************************************
//	親クラス [CTutorialManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CTutorialManager::CTutorialManager()
{
	// メンバ変数をクリア
	m_pExplain	= NULL;			// 説明表示の情報
	m_state		= STATE_NONE;	// 状態
	m_nCounterState = 0;		// 状態管理カウンター
	m_nCounterExplain = 0;		// 説明管理カウンター
}

//============================================================
//	デストラクタ
//============================================================
CTutorialManager::~CTutorialManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CTutorialManager::Init(void)
{
	// ポインタを宣言
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// テクスチャへのポインタ

	// メンバ変数を初期化
	m_pExplain	= NULL;					// 説明表示の情報
	m_state		= STATE_NORMAL;			// 状態
	m_nCounterState = 0;				// 状態管理カウンター
	m_nCounterExplain = EXPLAIN_NORMAL;	// 説明管理カウンター

	// 選択背景の生成
	m_pExplain = CObject2D::Create
	( // 引数
		SCREEN_CENT,	// 位置
		SCREEN_SIZE		// 大きさ
	);
	if (m_pExplain == NULL)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pExplain->SetPriority(TUTORIAL_PRIO);

	// 説明テクスチャを登録・割当
	m_pExplain->BindTexture(pTexture->Regist(mc_apTextureFile[m_nCounterExplain]));

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
HRESULT CTutorialManager::Uninit(void)
{
	// 説明表示の終了
	m_pExplain->Uninit();

	// 成功を返す
	return S_OK;
}

//============================================================
//	更新処理
//============================================================
void CTutorialManager::Update(void)
{
	switch (m_state)
	{ // 状態ごとの処理
	case STATE_NONE:	// 何もしない状態

		// 無し

		break;

	case STATE_NORMAL:	// 通常状態

		if (CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_RETURN))
		{
			// シーンの設定
			CManager::GetInstance()->SetScene(CScene::MODE_TITLE);	// タイトル画面
		}

		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	// 説明表示の更新
	m_pExplain->Update();
}

//============================================================
//	生成処理
//============================================================
CTutorialManager *CTutorialManager::Create(void)
{
	// ポインタを宣言
	CTutorialManager *pTutorialManager = NULL;	// チュートリアルマネージャー生成用

	if (pTutorialManager == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pTutorialManager = new CTutorialManager;	// チュートリアルマネージャー
	}
	else { assert(false); return NULL; }	// 使用中

	if (pTutorialManager != NULL)
	{ // 使用されている場合
		
		// チュートリアルマネージャーの初期化
		if (FAILED(pTutorialManager->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pTutorialManager;
			pTutorialManager = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pTutorialManager;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
HRESULT CTutorialManager::Release(CTutorialManager *&prTutorialManager)
{
	if (prTutorialManager != NULL)
	{ // 使用中の場合

		// チュートリアルマネージャーの終了
		if (FAILED(prTutorialManager->Uninit()))
		{ // 破棄に失敗した場合

			// メモリ開放
			delete prTutorialManager;
			prTutorialManager = NULL;

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// メモリ開放
		delete prTutorialManager;
		prTutorialManager = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}
