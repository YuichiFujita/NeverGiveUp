//============================================================
//
//	障害物処理 [obstacle.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "obstacle.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	マクロ定義
//************************************************************
#define OBSTACLE_PRIO	(1)	// 障害物の優先順位

#define SIZE_OBSTACLE	(D3DXVECTOR3(60.0f, 0.0f, 160.0f))	// 障害物の大きさ	// TODO：仮

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CObstacle::SModelInfo CObstacle::m_aStatusInfo[] =	// モデル情報
{
	{ // 箱モデル
		"data\\MODEL\\OBSTACLE\\obstacle000.x",	// モデルファイル名
		{ // ステータス情報
			SIZE_OBSTACLE,	// 大きさ
			DODGE_JUMP,		// 回避法
		},
	},

	{ // ベンチモデル
		"data\\MODEL\\OBSTACLE\\obstacle001.x",	// モデルファイル名
		{ // ステータス情報
			SIZE_OBSTACLE,	// 大きさ
			DODGE_SLIDE,	// 回避法
		},
	},
};

//************************************************************
//	子クラス [CObstacle] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CObstacle::CObstacle(const EType type) : CObjectModel(CObject::LABEL_OBSTACLE, OBSTACLE_PRIO), m_status(m_aStatusInfo[type].status), m_type(type)
{

}

//============================================================
//	デストラクタ
//============================================================
CObstacle::~CObstacle()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CObstacle::Init(void)
{
	// オブジェクトモデルの初期化
	if (FAILED(CObjectModel::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CObstacle::Uninit(void)
{
	// オブジェクトモデルの終了
	CObjectModel::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CObstacle::Update(void)
{
	// オブジェクトモデルの更新
	CObjectModel::Update();
}

//============================================================
//	描画処理
//============================================================
void CObstacle::Draw(void)
{
	// オブジェクトモデルの描画
	CObjectModel::Draw();
}

//============================================================
//	種類取得処理
//============================================================
int CObstacle::GetType(void) const
{
	// 障害物の回避法を返す
	return m_status.dodge;
}

//============================================================
//	大きさ取得処理
//============================================================
D3DXVECTOR3 CObstacle::GetVec3Sizing(void) const
{
	// 障害物の大きさを返す
	return m_status.size;
}

//============================================================
//	生成処理
//============================================================
CObstacle *CObstacle::Create
(
	const EType type,			// 種類
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot		// 向き
)
{
	// ポインタを宣言
	CObstacle *pObstacle = NULL;	// 障害物生成用

	if (pObstacle == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pObstacle = new CObstacle(type);	// 障害物
	}
	else { assert(false); return NULL; }	// 使用中

	if (pObstacle != NULL)
	{ // 確保に成功している場合

		// 障害物の初期化
		if (FAILED(pObstacle->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pObstacle;
			pObstacle = NULL;

			// 失敗を返す
			return NULL;
		}

		// 種類を設定
		pObstacle->SetType(type);

		// 位置を設定
		pObstacle->SetVec3Position(rPos);

		// 向きを設定
		pObstacle->SetVec3Rotation(rRot);

		// 確保したアドレスを返す
		return pObstacle;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	種類の設定処理
//============================================================
void CObstacle::SetType(const EType type)
{
	// 引数の種類の情報を設定
	if (type < TYPE_MAX)
	{ // 種類がある場合

		// モデルを登録・割当
		BindModel(m_aStatusInfo[type].pTextureFile);
	}
	else { assert(false); }	// 種類オーバー
}
