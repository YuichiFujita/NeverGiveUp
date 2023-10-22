//============================================================
//
//	エディットビル処理 [editBuilding.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "editBuilding.h"
#include "manager.h"
#include "input.h"
#include "editStageManager.h"
#include "effect3D.h"

//************************************************************
//	マクロ定義
//************************************************************
#define KEY_CREATE		(DIK_0)	// 生成キー
#define NAME_CREATE		("0")	// 生成表示
#define KEY_RELEASE		(DIK_9)	// 破棄キー
#define NAME_RELEASE	("9")	// 破棄表示
#define KEY_TYPE		(DIK_2)	// 種類変更キー
#define NAME_TYPE		("2")	// 種類変更表示

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const float	INIT_ALPHA		= 0.5f;		// 配置前のα値
	const float	EFFECT_RADIUS	= 30.0f;	// 方向表示エフェクトの半径
	const int	EFFECT_LIFE		= 10;		// 方向表示エフェクトの寿命
}

//************************************************************
//	親クラス [CEditBuilding] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEditBuilding::CEditBuilding()
{
#if _DEBUG

	// メンバ変数をクリア
	m_pEdit = NULL;	// エディットステージの情報
	memset(&m_building, 0, sizeof(m_building));	// ビル配置情報

#endif	// _DEBUG
}

//============================================================
//	デストラクタ
//============================================================
CEditBuilding::~CEditBuilding()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	初期化処理
//============================================================
HRESULT CEditBuilding::Init(void)
{
#if _DEBUG

	// メンバ変数を初期化
	m_pEdit = NULL;	// エディットステージの情報
	m_building.pBuilding = NULL;			// ビル情報
	m_building.type = CBuilding::TYPE_00;	// ビル種類

	// ビルの生成	// TODO：種類の設定で位置と向き以外の情報は決まるように変更
	m_building.pBuilding = CBuilding::Create(m_building.type, VEC3_ZERO, VEC3_ZERO, D3DXVECTOR3(280.0f, 560.0f, 280.0f), XCOL_WHITE, CBuilding::COLLISION_GROUND);
	if (m_building.pBuilding == NULL)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 自動更新・自動描画をOFFにする
	m_building.pBuilding->SetEnableUpdate(false);
	m_building.pBuilding->SetEnableDraw(false);

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
void CEditBuilding::Uninit(void)
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	更新処理
//============================================================
void CEditBuilding::Update(void)
{
#if _DEBUG

	if (m_pEdit == NULL)
	{ // エディットステージが存在しない場合

		// 処理を抜ける
		assert(false);
		return;
	}

	// 種類変更の更新
	UpdateChangeType();

	// ビルの生成
	CreateBuilding();

	// ビルの破棄
	ReleaseBuilding();

	// 方向表示エフェクトの生成
	CreateRotaEffect();

	// 位置を反映
	m_building.pBuilding->SetVec3Position(m_pEdit->GetVec3Position());

	// 向きを反映
	m_building.pBuilding->SetVec3Rotation(m_pEdit->GetVec3Rotation());

	// 種類を反映
	m_building.pBuilding->SetType(m_building.type);

#endif
}

//============================================================
//	表示の設定処理
//============================================================
void CEditBuilding::SetDisp(const bool bDisp)
{
	// 自動更新・自動描画を表示状況に合わせる
	m_building.pBuilding->SetEnableUpdate(bDisp);	// 更新
	m_building.pBuilding->SetEnableDraw(bDisp);		// 描画
}

//============================================================
//	生成処理
//============================================================
CEditBuilding *CEditBuilding::Create(CEditStageManager *pEdit)
{
#if _DEBUG

	// ポインタを宣言
	CEditBuilding *pEditBuilding = NULL;	// エディットビル生成用

	if (pEditBuilding == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pEditBuilding = new CEditBuilding;	// エディットビル
	}
	else { assert(false); return NULL; }	// 使用中

	if (pEditBuilding != NULL)
	{ // 使用されている場合
		
		// エディットビルの初期化
		if (FAILED(pEditBuilding->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pEditBuilding;
			pEditBuilding = NULL;

			// 失敗を返す
			return NULL;
		}

		// エディットステージの情報を設定
		pEditBuilding->m_pEdit = pEdit;

		// 確保したアドレスを返す
		return pEditBuilding;
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
HRESULT CEditBuilding::Release(CEditBuilding *&prEditBuilding)
{
#if _DEBUG

	if (prEditBuilding != NULL)
	{ // 使用中の場合

		// エディットビルの終了
		prEditBuilding->Uninit();

		// メモリ開放
		delete prEditBuilding;
		prEditBuilding = NULL;

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
//	種類変更の更新処理
//============================================================
void CEditBuilding::UpdateChangeType(void)
{
	// ポインタを宣言
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// キーボード情報

	// 種類を変更
	if (m_pKeyboard->IsTrigger(KEY_TYPE))
	{
		m_building.type = (CBuilding::EType)((m_building.type + 1) % CBuilding::TYPE_MAX);
	}
}

//============================================================
//	ビルの生成処理
//============================================================
void CEditBuilding::CreateBuilding(void)
{
	// 変数を宣言
	D3DXVECTOR3 posEdit = m_pEdit->GetVec3Position();	// エディットの位置
	D3DXVECTOR3 rotEdit = m_pEdit->GetVec3Rotation();	// エディットの向き
	D3DXCOLOR colBuild = XCOL_WHITE;	// 色保存用

	// ポインタを宣言
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// キーボード情報

	// ビルを配置
	if (m_pKeyboard->IsTrigger(KEY_CREATE))
	{
		//----------------------------------------------------
		//	ビルの情報を配置用に変更
		//----------------------------------------------------
		// 自動更新・自動描画をONにする
		m_building.pBuilding->SetEnableUpdate(true);
		m_building.pBuilding->SetEnableDraw(true);

		// 色を設定
		colBuild = m_building.pBuilding->GetCubeColor();	// 元の色を取得
		m_building.pBuilding->SetCubeColor(D3DXCOLOR(colBuild.r, colBuild.g, colBuild.b, 1.0f));

		//----------------------------------------------------
		//	新しいビルの生成
		//----------------------------------------------------
		// ビルの生成	// TODO：種類の設定で位置と向き以外の情報は決まるように変更
		m_building.pBuilding = CBuilding::Create(m_building.type, posEdit, rotEdit, D3DXVECTOR3(280.0f, 560.0f, 280.0f), XCOL_WHITE, CBuilding::COLLISION_GROUND);
		assert(m_building.pBuilding != NULL);

		// 色を設定
		colBuild = m_building.pBuilding->GetCubeColor();	// 元の色を取得
		m_building.pBuilding->SetCubeColor(D3DXCOLOR(colBuild.r, colBuild.g, colBuild.b, INIT_ALPHA));
	}
}

//============================================================
//	ビルの破棄処理
//============================================================
void CEditBuilding::ReleaseBuilding(void)
{
	// ポインタを宣言
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// キーボード情報

	// ビルを配置
	if (m_pKeyboard->IsTrigger(KEY_RELEASE))
	{

	}
}

//============================================================
//	方向表示エフェクトの生成処理
//============================================================
void CEditBuilding::CreateRotaEffect(void)
{
	// 変数を宣言
	D3DXVECTOR3 posEffect = VEC3_ZERO;								// エフェクト位置
	D3DXVECTOR3 posEdit   = m_pEdit->GetVec3Position();				// エディットの位置
	D3DXVECTOR3 rotEdit   = m_pEdit->GetVec3Rotation();				// エディットの向き
	D3DXVECTOR3 sizeBuild = m_building.pBuilding->GetVec3Sizing();	// ビル大きさ

	// エフェクト位置を計算
	posEffect.x = posEdit.x + sinf(rotEdit.y + D3DX_PI) * sizeBuild.x;
	posEffect.y = posEdit.y + sizeBuild.y * 2.0f;
	posEffect.z = posEdit.z + cosf(rotEdit.y + D3DX_PI) * sizeBuild.z;

	// 方向表示エフェクトを生成
	CEffect3D::Create(posEffect, EFFECT_RADIUS, CEffect3D::TYPE_NORMAL, EFFECT_LIFE);
}
