//============================================================
//
//	ビル処理 [building.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "building.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	マクロ定義
//************************************************************
#define BUILDING_PRIO	(1)	// ビルの優先順位

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CBuilding::mc_apTextureFile[][6] =	// テクスチャ定数
{
	{ // ビル00テクスチャ
		"data\\TEXTURE\\buildingSide000.png",	// 左テクスチャ
		"data\\TEXTURE\\buildingSide000.png",	// 右テクスチャ
		"data\\TEXTURE\\buildingRoof000.png",	// 下テクスチャ
		"data\\TEXTURE\\buildingRoof000.png",	// 上テクスチャ
		"data\\TEXTURE\\buildingSide000.png",	// 前テクスチャ
		"data\\TEXTURE\\buildingSide000.png",	// 後テクスチャ
	},

	{ // ビル01テクスチャ
		"data\\TEXTURE\\buildingSide001.png",	// 左テクスチャ
		"data\\TEXTURE\\buildingSide001.png",	// 右テクスチャ
		"data\\TEXTURE\\buildingRoof001.png",	// 下テクスチャ
		"data\\TEXTURE\\buildingRoof001.png",	// 上テクスチャ
		"data\\TEXTURE\\buildingSide001.png",	// 前テクスチャ
		"data\\TEXTURE\\buildingSide001.png",	// 後テクスチャ
	},

	{ // ビル02テクスチャ
		"data\\TEXTURE\\buildingSide002.png",	// 左テクスチャ
		"data\\TEXTURE\\buildingSide002.png",	// 右テクスチャ
		"data\\TEXTURE\\buildingRoof002.png",	// 下テクスチャ
		"data\\TEXTURE\\buildingRoof002.png",	// 上テクスチャ
		"data\\TEXTURE\\buildingSide002.png",	// 前テクスチャ
		"data\\TEXTURE\\buildingSide002.png",	// 後テクスチャ
	},
};

//************************************************************
//	子クラス [CBuilding] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CBuilding::CBuilding() : CObjectMeshCube(CObject::LABEL_BUILDING, BUILDING_PRIO)
{
	// メンバ変数をクリア
	m_collision = COLLISION_NONE;	// 当たり判定
}

//============================================================
//	デストラクタ
//============================================================
CBuilding::~CBuilding()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CBuilding::Init(void)
{
	// メンバ変数を初期化
	m_collision = COLLISION_NONE;	// 当たり判定

	// オブジェクトメッシュキューブの初期化
	if (FAILED(CObjectMeshCube::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 原点を設定
	SetOrigin(CObjectMeshCube::ORIGIN_DOWN);

	// 縁取り色を設定
	SetBorderColor(XCOL_WHITE);

	// 縁取りの状態を設定
	if (FAILED(SetBorderState(CObjectMeshCube::BORDER_OFF)))
	{ // 分割数の設定に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 縁取りの太さを設定
	SetBorderThick(0.0f);

	// テクスチャの状態を設定
	SetTextureState(CObjectMeshCube::TEXSTATE_SELECT);

	// テクスチャの分割数Xを設定
	SetTexturePatternX(VEC2_ONE);

	// テクスチャの分割数Yを設定
	SetTexturePatternY(VEC2_ONE);

	// テクスチャの分割数Zを設定
	SetTexturePatternZ(VEC2_ONE);

	// カリングを設定
	SetCulling(D3DCULL_CCW);

	// ライティングを設定
	SetLighting(true);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CBuilding::Uninit(void)
{
	// オブジェクトメッシュキューブの終了
	CObjectMeshCube::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CBuilding::Update(void)
{
	// オブジェクトメッシュキューブの更新
	CObjectMeshCube::Update();
}

//============================================================
//	描画処理
//============================================================
void CBuilding::Draw(void)
{
	// オブジェクトメッシュキューブの描画
	CObjectMeshCube::Draw();
}

//============================================================
//	種類の設定処理
//============================================================
void CBuilding::SetType(const int nType)
{
	// 変数を宣言
	SFaceTex faceTex;	// テクスチャインデックス設定用

	// ポインタを宣言
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// テクスチャへのポインタ
	if (pTexture == NULL)
	{ // 使用されていない場合

		// 処理を抜ける
		assert(false);
		return;
	}

	if (nType < TYPE_MAX)
	{ // 種類がある場合

		// 引数の種類のテクスチャを登録
		faceTex = SFaceTex
		( // 引数
			pTexture->Regist(mc_apTextureFile[nType][0]),	// 左
			pTexture->Regist(mc_apTextureFile[nType][1]),	// 右
			pTexture->Regist(mc_apTextureFile[nType][2]),	// 下
			pTexture->Regist(mc_apTextureFile[nType][3]),	// 上
			pTexture->Regist(mc_apTextureFile[nType][4]),	// 前
			pTexture->Regist(mc_apTextureFile[nType][5])	// 後
		);

		// テクスチャを割当
		BindTexture(faceTex);
	}
	else { assert(false); }	// 種類オーバー
}

//============================================================
//	状態取得処理
//============================================================
int CBuilding::GetState(void) const
{
	// 当たり判定を返す
	return m_collision;
}

//============================================================
//	生成処理
//============================================================
CBuilding *CBuilding::Create
(
	const EType type,			// 種類
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot,	// 向き
	const D3DXVECTOR3& rSize,	// 大きさ
	const D3DXCOLOR& rCol,		// 色
	const ECollision collision	// 当たり判定
)
{
	// ポインタを宣言
	CBuilding *pBuilding = NULL;	// ビル生成用

	if (pBuilding == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pBuilding = new CBuilding;	// ビル
	}
	else { assert(false); return NULL; }	// 使用中

	if (pBuilding != NULL)
	{ // 確保に成功している場合

		// ビルの初期化
		if (FAILED(pBuilding->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pBuilding;
			pBuilding = NULL;

			// 失敗を返す
			return NULL;
		}

		// 種類を設定
		pBuilding->SetType(type);

		// 位置を設定
		pBuilding->SetVec3Position(rPos);

		// 向きを設定
		pBuilding->SetVec3Rotation(rRot);

		// 大きさを設定
		pBuilding->SetVec3Sizing(rSize);

		// キューブ色を設定
		pBuilding->SetCubeColor(rCol);

		// 当たり判定を設定
		pBuilding->SetCollision(collision);

		// 確保したアドレスを返す
		return pBuilding;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	当たり判定の設定処理
//============================================================
void CBuilding::SetCollision(const ECollision collision)
{
	if (collision > NONE_IDX && collision < COLLISION_MAX)
	{ // インデックスが範囲内の場合

		// 引数の当たり判定を設定
		m_collision = collision;
	}
	else { assert(false); }	// 範囲外
}
