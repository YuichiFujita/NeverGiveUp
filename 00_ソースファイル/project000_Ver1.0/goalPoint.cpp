//============================================================
//
//	ゴールポイント処理 [goalPoint.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "goalPoint.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "collision.h"
#include "scene.h"
#include "player.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY = 1;	// ゴールポイントの優先順位
}

//************************************************************
//	子クラス [CGoalPoint] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CGoalPoint::CGoalPoint() : CObject(CObject::LABEL_GOALPOINT, PRIORITY)
{
	// メンバ変数をクリア
	m_pos = VEC3_ZERO;	// 位置
	m_size = VEC3_ZERO;	// 大きさ
}

//============================================================
//	デストラクタ
//============================================================
CGoalPoint::~CGoalPoint()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CGoalPoint::Init(void)
{
	// メンバ変数を初期化
	m_pos = VEC3_ZERO;	// 位置
	m_size = VEC3_ZERO;	// 大きさ

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CGoalPoint::Uninit(void)
{
	// ゴールポイントを破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CGoalPoint::Update(void)
{
	// プレイヤーとの当たり判定
	CollisionPlayer();
}

//============================================================
//	描画処理
//============================================================
void CGoalPoint::Draw(void)
{

}

//============================================================
//	位置の設定処理
//============================================================
void CGoalPoint::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// 引数の位置を設定
	m_pos = rPos;
}

//============================================================
//	位置取得処理
//============================================================
D3DXVECTOR3 CGoalPoint::GetVec3Position(void) const
{
	// 位置を返す
	return m_pos;
}

//============================================================
//	判定大きさ設定の設定処理
//============================================================
void CGoalPoint::SetVec3Sizing(const D3DXVECTOR3& rSize)
{
	// 引数の大きさを設定
	m_size = rSize;
}

//============================================================
//	判定大きさ取得処理
//============================================================
D3DXVECTOR3 CGoalPoint::GetVec3Sizing(void) const
{
	// 大きさを返す
	return m_size;
}

//============================================================
//	生成処理
//============================================================
CGoalPoint *CGoalPoint::Create	// 生成
(
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rSize	// 大きさ
)
{
	// ポインタを宣言
	CGoalPoint *pGoalPoint = NULL;	// ゴールポイント生成用

	if (pGoalPoint == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pGoalPoint = new CGoalPoint;	// ゴールポイント
	}
	else { assert(false); return NULL; }	// 使用中

	if (pGoalPoint != NULL)
	{ // 確保に成功している場合

		// ゴールポイントの初期化
		if (FAILED(pGoalPoint->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pGoalPoint;
			pGoalPoint = NULL;

			// 失敗を返す
			return NULL;
		}

		// 位置を設定
		pGoalPoint->SetVec3Position(rPos);

		// 大きさを設定
		pGoalPoint->SetVec3Sizing(rSize);

		// 確保したアドレスを返す
		return pGoalPoint;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	プレイヤーとの当たり判定
//============================================================
void CGoalPoint::CollisionPlayer(void)
{
	if (CManager::GetInstance()->GetMode() == CScene::MODE_GAME)
	{ // ゲームモードの場合

		// ポインタを宣言
		CPlayer *pPlayer = CScene::GetPlayer();	// プレイヤー情報
		if (pPlayer == NULL)
		{ // プレイヤーが存在しない場合

			// 処理を抜ける
			assert(false);
			return;
		}

		// 変数を宣言
		bool  bHit = false;	// 判定状況
		float fPlayerRadius    = pPlayer->GetRadius();			// プレイヤー半径
		D3DXVECTOR3 posPlayer  = pPlayer->GetVec3Position();	// プレイヤー位置
		D3DXVECTOR3 posGoal    = GetVec3Position();				// ゴール位置
		D3DXVECTOR3 sizePlayer = D3DXVECTOR3(fPlayerRadius, 0.0f, fPlayerRadius);	// プレイヤー大きさ

		if (pPlayer->GetState() != CPlayer::STATE_CLEAR
		&&  pPlayer->GetState() != CPlayer::STATE_OVER)
		{ // ゲーム終了に関する状態ではない場合

			// プレイヤーとの判定
			bHit = collision::Box2D
			( // 引数
				posPlayer,	// 判定位置
				posGoal,	// 判定目標位置
				sizePlayer,	// 判定サイズ(右・上・後)
				sizePlayer,	// 判定サイズ(左・下・前)
				m_size,		// 判定目標サイズ(右・上・後)
				m_size		// 判定目標サイズ(左・下・前)
			);
			if (bHit)
			{ // プレイヤーが判定内の場合

				// プレイヤーをクリア成功状態にする
				pPlayer->SetState(CPlayer::STATE_CLEAR);
			}
		}
	}
}

