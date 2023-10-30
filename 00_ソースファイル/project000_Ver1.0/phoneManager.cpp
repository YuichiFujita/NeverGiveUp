//============================================================
//
//	スマホマネージャー処理 [phoneManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "phoneManager.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "texture.h"
#include "object2D.h"

//************************************************************
//	マクロ定義
//************************************************************
#define PHONE_PRIO	(6)	// スマホの優先順位

#define POS_PHONE	(D3DXVECTOR3(SCREEN_CENT.x, 1200.0f, 0.0f))	// スマホの初期座標
#define SIZE_PHONE	(D3DXVECTOR3(394.0f, 653.0f, 0.0f))			// スマホの大きさ

#define COL_PHONE	(XCOL_WHITE)	// スマホの色
#define COL_FADE	(XCOL_WHITE)	// フェードの色
#define PHONE_MOVE	(0.5f)			// スマホの位置の更新量
#define PHONE_STOP	(SCREEN_CENT.y)	// スマホの停止Y位置

#define FADE_CHAN	(0.02f)	// フェードのα値変更量
#define FADE_STOP	(0.6f)	// フェードの最大α値

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CPhoneManager::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\phone000.png",	// スマホのテクスチャ相対パス
	"data\\TEXTURE\\phone001.png",	// 操作表示のテクスチャ相対パス
};

//************************************************************
//	親クラス [CPhoneManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CPhoneManager::CPhoneManager()
{
	// メンバ変数をクリア
	m_pPhone	= NULL;			// スマホの情報
	m_pFade		= NULL;			// フェードの情報
	m_state		= STATE_NONE;	// 状態
	m_fMove		= 0.0f;			// スマホの移動量
	m_bDisp		= false;		// 表示状況
	m_nCounterState	= 0;		// 状態管理カウンター
}

//============================================================
//	デストラクタ
//============================================================
CPhoneManager::~CPhoneManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CPhoneManager::Init(void)
{
	// メンバ変数を初期化
	m_pPhone	= NULL;			// スマホの情報
	m_pFade		= NULL;			// フェードの情報
	m_state		= STATE_NONE;	// 状態
	m_fMove		= 0.0f;			// スマホの移動量
	m_bDisp		= false;		// 表示状況
	m_nCounterState	= 0;		// 状態管理カウンター

	//--------------------------------------------------------
	//	フェードの生成・設定
	//--------------------------------------------------------
	// フェードの生成
	m_pFade = CObject2D::Create
	( // 引数
		SCREEN_CENT,	// 位置
		SCREEN_SIZE,	// 大きさ
		VEC3_ZERO,		// 向き
		COL_FADE		// 色
	);
	if (m_pFade == NULL)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pFade->SetPriority(PHONE_PRIO);

	// 自動描画をOFFに設定
	m_pFade->SetEnableDraw(false);

	//--------------------------------------------------------
	//	スマホの生成・設定
	//--------------------------------------------------------
	// スマホの生成
	m_pPhone = CObject2D::Create
	( // 引数
		SCREEN_CENT,	// 位置
		SIZE_PHONE,		// 大きさ
		VEC3_ZERO,		// 向き
		COL_PHONE		// 色
	);
	if (m_pPhone == NULL)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// テクスチャを登録・割当
	m_pPhone->BindTexture(mc_apTextureFile[TEXTURE_PHONE]);

	// 優先順位を設定
	m_pPhone->SetPriority(PHONE_PRIO);

	// 自動描画をOFFに設定
	m_pPhone->SetEnableDraw(false);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CPhoneManager::Uninit(void)
{
	// スマホの終了
	m_pPhone->Uninit();

	// フェードの終了
	m_pFade->Uninit();
}

//============================================================
//	更新処理
//============================================================
void CPhoneManager::Update(void)
{
	switch (m_state)
	{ // 状態ごとの処理
	case STATE_NONE:			// 何もしない状態

		// 無し

		// 処理を抜ける
		break;

	case STATE_FADEIN:			// フェードイン状態

		// フェードインの更新
		UpdateFade();

		// 処理を抜ける
		break;

	case STATE_PHONE_TAKE:		// スマホの取り出し状態

		// スマホ取出の更新
		UpdatePhoneTake();

		// 処理を抜ける
		break;

	case STATE_WAIT:			// 待機状態

		// 待機の更新
		UpdateWait();

		// 処理を抜ける
		break;

	case STATE_PHONE_RETURN:	// スマホのしまい状態

		// スマホ収納の更新
		UpdatePhoneReturn();

		// 処理を抜ける
		break;

	case STATE_END:				// 終了状態

		// 無し

		// 処理を抜ける
		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	// スマホの更新
	m_pPhone->Update();

	// フェードの更新
	m_pFade->Update();
}

//============================================================
//	スマホ表示の開始処理
//============================================================
void CPhoneManager::SetLook(void)
{
	// スマホの位置を初期化
	m_pPhone->SetVec3Position(POS_PHONE);

	// ポリゴンの色情報を初期化
	m_pPhone->SetColor(COL_PHONE);	// スマホ
	m_pFade->SetColor(COL_FADE);	// フェード

	// 情報を初期化
	m_fMove = 0.0f;			// スマホの移動量
	m_nCounterState = 0;	// 状態管理カウンター

	// 表示をONにする
	SetEnableDisp(true);

	// 状態を設定
	m_state = STATE_FADEIN;	// フェードイン状態
}

//============================================================
//	状態の設定処理
//============================================================
void CPhoneManager::SetState(const STATE state)
{
	// 引数の状態を設定
	m_state = state;
}

//============================================================
//	状態取得処理
//============================================================
CPhoneManager::STATE CPhoneManager::GetState(void) const
{
	// 状態を返す
	return m_state;
}

//============================================================
//	表示の設定処理
//============================================================
void CPhoneManager::SetEnableDisp(const bool bDisp)
{
	// 引数の表示状況を設定
	m_bDisp = bDisp;

	// 描画状況を表示状況と一致させる
	m_pFade->SetEnableDraw(bDisp);
	m_pPhone->SetEnableDraw(bDisp);
}

//============================================================
//	表示取得処理
//============================================================
bool CPhoneManager::IsDisp(void) const
{
	// 表示状況を返す
	return m_bDisp;
}

//============================================================
//	生成処理
//============================================================
CPhoneManager *CPhoneManager::Create(void)
{
	// ポインタを宣言
	CPhoneManager *pPhoneManager = NULL;	// スマホマネージャー生成用

	if (pPhoneManager == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pPhoneManager = new CPhoneManager;	// スマホマネージャー
	}
	else { assert(false); return NULL; }	// 使用中

	if (pPhoneManager != NULL)
	{ // 使用されている場合
		
		// スマホマネージャーの初期化
		if (FAILED(pPhoneManager->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pPhoneManager;
			pPhoneManager = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pPhoneManager;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
HRESULT CPhoneManager::Release(CPhoneManager *&prPhoneManager)
{
	if (prPhoneManager != NULL)
	{ // 使用中の場合

		// スマホマネージャーの終了
		prPhoneManager->Uninit();

		// メモリ開放
		delete prPhoneManager;
		prPhoneManager = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}

//============================================================
//	フェードインの更新処理
//============================================================
void CPhoneManager::UpdateFade(void)
{
	// 変数を宣言
	D3DXCOLOR colFade = m_pFade->GetColor();	// フェードの色

	// フェードのα値を加算
	colFade.a += FADE_CHAN;
	
	if (colFade.a >= FADE_STOP)
	{ // フェードのα値が一定値以上の場合

		// フェードのα値を補正
		colFade.a = FADE_STOP;

		// 便箋の取り出し状態にする
		m_state = STATE_PHONE_TAKE;

		// サウンドの再生
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_PAPER);	// 便箋めくり音
	}

	// フェードの色を反映
	m_pFade->SetColor(colFade);
}

//============================================================
//	スマホ取出の更新処理
//============================================================
void CPhoneManager::UpdatePhoneTake(void)
{
	// 変数を宣言
	D3DXVECTOR3 posPhone = m_pPhone->GetVec3Position();	// スマホの位置

	// スマホの位置減算量を加算
	m_fMove += PHONE_MOVE;

	// スマホの位置を減算
	posPhone.y -= m_fMove;

	if (posPhone.y <= PHONE_STOP)
	{ // スマホの位置が一定値以下の場合

		// スマホの位置を補正
		posPhone.y = PHONE_STOP;

		// スマホの移動量を初期化
		m_fMove = 0.0f;

		// 待機状態にする
		m_state = STATE_WAIT;
	}

	// スマホの位置を反映
	m_pPhone->SetVec3Position(posPhone);
}

//============================================================
//	待機の更新処理
//============================================================
void CPhoneManager::UpdateWait(void)
{
	//m_state = STATE_PHONE_RETURN;
}

//============================================================
//	スマホ収納の更新処理
//============================================================
void CPhoneManager::UpdatePhoneReturn(void)
{
	// 変数を宣言
	D3DXVECTOR3 posPhone = m_pPhone->GetVec3Position();	// 便箋の位置
	D3DXCOLOR colFade = m_pFade->GetColor();			// フェードの色

	//--------------------------------------------------------
	//	フェードの透明化
	//--------------------------------------------------------
	if (colFade.a > 0.0f)
	{ // フェードのα値が一定値より大きい場合

		// フェードのα値を減算
		colFade.a -= FADE_CHAN;

		if (colFade.a <= 0.0f)
		{ // フェードのα値が一定値以下の場合

			// フェードのα値を補正
			colFade.a = 0.0f;
		}

		// フェードの色を反映
		m_pFade->SetColor(colFade);
	}

	//--------------------------------------------------------
	//	スマホの移動
	//--------------------------------------------------------
	if (posPhone.y < POS_PHONE.y)
	{ // スマホの位置が一定値より小さい場合

		// スマホの移動量を加算
		m_fMove += PHONE_MOVE;

		// スマホの位置を加算
		posPhone.y += m_fMove;
	}
	else if (posPhone.y >= POS_PHONE.y)
	{ // スマホの位置が一定値以上の場合

		// スマホの位置を補正
		posPhone.y = POS_PHONE.y;

		// スマホの移動量を初期化
		m_fMove = 0.0f;

		if (colFade.a <= 0.0f)
		{ // α値が下がり切っている場合

			// 終了状態にする
			m_state = STATE_END;

			// 表示をOFFにする
			SetEnableDisp(false);
		}
	}

	// スマホの位置を反映
	m_pPhone->SetVec3Position(posPhone);
}
