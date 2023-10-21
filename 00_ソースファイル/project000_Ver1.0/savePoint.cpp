//============================================================
//
//	セーブポイント処理 [savePoint.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "savePoint.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "collision.h"
#include "scene.h"
#include "player.h"

#include "particle3D.h"

//************************************************************
//	マクロ定義
//************************************************************
#define SAVEPOINT_PRIO	(1)	// セーブポイントの優先順位

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const D3DXVECTOR3 COLL_SIZE = D3DXVECTOR3(280.0f, 0.0f, 280.0f);	// セーブ判定の大きさ
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CSavePoint::mc_apModelFile[] =	// モデル定数
{
	"data\\MODEL\\POINT\\save000.x",	// セーブポイントモデル
};
CSavePoint *CSavePoint::m_pCurrentSave = NULL;	// 現在のセーブポイントへのポインタ
int CSavePoint::m_nNumAll = 0;	// セーブポイントの総数

//************************************************************
//	子クラス [CSavePoint] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CSavePoint::CSavePoint() : CObjectModel(CObject::LABEL_SAVEPOINT, SAVEPOINT_PRIO), m_nThisSaveID(m_nNumAll)
{
	// セーブポイントの総数を加算
	m_nNumAll++;
}

//============================================================
//	デストラクタ
//============================================================
CSavePoint::~CSavePoint()
{
	// セーブポイントの総数を減算
	m_nNumAll--;
}

//============================================================
//	初期化処理
//============================================================
HRESULT CSavePoint::Init(void)
{
	// オブジェクトモデルの初期化
	if (FAILED(CObjectModel::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	if (m_nThisSaveID == 0)
	{ // 自身のセーブポイントが一つ目の場合

		// 自身のセーブポイントを保存
		m_pCurrentSave = this;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CSavePoint::Uninit(void)
{
	// オブジェクトモデルの終了
	CObjectModel::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CSavePoint::Update(void)
{
	// プレイヤーとの当たり判定
	CollisionPlayer();

	// オブジェクトモデルの更新
	CObjectModel::Update();

	// デバッグ表示
	if (m_nThisSaveID == 0)
	{
		CManager::GetInstance()->GetDebugProc()->Print("今のセーブポイント：%d\n", m_pCurrentSave->m_nThisSaveID);
	}
}

//============================================================
//	描画処理
//============================================================
void CSavePoint::Draw(void)
{
	// オブジェクトモデルの描画
	CObjectModel::Draw();
}

//============================================================
//	生成処理
//============================================================
CSavePoint *CSavePoint::Create
(
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot		// 向き
)
{
	// ポインタを宣言
	CSavePoint *pSavePoint = NULL;	// セーブポイント生成用

	if (pSavePoint == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pSavePoint = new CSavePoint;	// セーブポイント
	}
	else { assert(false); return NULL; }	// 使用中

	if (pSavePoint != NULL)
	{ // 確保に成功している場合

		// セーブポイントの初期化
		if (FAILED(pSavePoint->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pSavePoint;
			pSavePoint = NULL;

			// 失敗を返す
			return NULL;
		}

		// 位置を設定
		pSavePoint->SetVec3Position(rPos);

		// 向きを設定
		pSavePoint->SetVec3Rotation(rRot);

		// 確保したアドレスを返す
		return pSavePoint;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	セーブポイント情報取得処理
//============================================================
CSavePoint::SInfo CSavePoint::GetSavePointInfo(void)
{
	// 変数を宣言
	SInfo info = { VEC3_ZERO, VEC3_ZERO };	// セーブポイント情報

	if (m_pCurrentSave != NULL)
	{ // セーブポイントが存在する場合

		// セーブポイントの情報を設定
		info.pos = m_pCurrentSave->GetVec3Position();	// 位置
		info.rot = m_pCurrentSave->GetVec3Rotation();	// 向き
	}
	else { assert(false); }	// セーブポイント無し

	// セーブポイント情報を返す
	return info;
}

//============================================================
//	プレイヤーとの当たり判定
//============================================================
void CSavePoint::CollisionPlayer(void)
{
	// ポインタを宣言
	CPlayer *pPlayer = CScene::GetPlayer();	// プレイヤー情報

	// 変数を宣言
	bool  bHit = false;	// 判定状況
	float fPlayerRadius = pPlayer->GetRadius();			// プレイヤー半径
	D3DXVECTOR3 posPlayer = pPlayer->GetVec3Position();	// プレイヤー位置
	D3DXVECTOR3 posSave = GetVec3Position();			// セーブ位置
	D3DXVECTOR3 sizePlayer = D3DXVECTOR3(fPlayerRadius, 0.0f, fPlayerRadius);	// プレイヤー大きさ

	if (pPlayer == NULL)
	{ // プレイヤーが存在しない場合

		// 処理を抜ける
		assert(false);
		return;
	}

	// プレイヤーとの判定
	bHit = collision::Box2D
	( // 引数
		posPlayer,	// 判定位置
		posSave,	// 判定目標位置
		sizePlayer,	// 判定サイズ(右・上・後)
		sizePlayer,	// 判定サイズ(左・下・前)
		COLL_SIZE,	// 判定目標サイズ(右・上・後)
		COLL_SIZE	// 判定目標サイズ(左・下・前)
	);
	if (bHit)
	{ // プレイヤーが判定内の場合

		// セーブポイントを自身に変更
		m_pCurrentSave = this;

		// デバッグパーティクル
		CParticle3D::Create(CParticle3D::TYPE_HEAL, pPlayer->GetVec3Position());
	}
}
