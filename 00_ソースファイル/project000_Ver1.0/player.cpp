//============================================================
//
//	プレイヤー処理 [player.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "player.h"
#include "manager.h"
#include "scene.h"
#include "sceneGame.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "texture.h"
#include "collision.h"

#include "multiModel.h"
#include "shadow.h"
#include "stage.h"
#include "field.h"
#include "building.h"
#include "obstacle.h"

#include "effect3D.h"
#include "particle3D.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	// セットアップテキスト相対パス
	const char* SETUP_TXT = "data\\TXT\\player.txt";	// プレイヤーセットアップ

	// プレイヤー基本情報
	namespace basic
	{
		const float MOVE		= 2.8f;		// 移動量
		const float	JUMP		= 20.0f;	// ジャンプ上昇量
		const float	GRAVITY		= 1.0f;		// 重力
		const float	RADIUS		= 20.0f;	// 半径
		const float HEIGHT		= 100.0f;	// 縦幅
		const float	JUMP_REV	= 0.22f;	// 空中の移動量の減衰係数
		const float	LAND_REV	= 0.2f;		// 地上の移動量の減衰係数
		const float	REV_ROTA	= 0.15f;	// 向き変更の補正係数
	}

	// スライディング情報
	namespace slide
	{
		const float	SLIDE_CONTROL_MIN	= 2.4f;		// スライディングが可能になる移動量
		const float	PLUS_SUB_MOVE		= 0.0035f;	// 移動量の最大追加減算量

		const int	MIN_END_CNT	= 30;		// スライディングの解除操作ができるようになるカウント
		const int	MAX_END_CNT	= 80;		// スライディングが強制終了するカウント
		const float	MIN_MOVE	= 1.25f;	// 移動量の最低速度
		const float	SUB_MOVE	= 0.01f;	// スライディング時の速度減算量
		const float	ADD_MOVE	= 0.08f;	// 非スライディング時の速度加算量
	}

	// プレイヤー他クラス情報
	namespace other
	{
		const D3DXVECTOR3 SHADOW_SIZE = D3DXVECTOR3(80.0f, 0.0f, 80.0f);	// 影の大きさ
	}
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CPlayer::mc_apModelFile[] =	// モデル定数
{
	"data\\MODEL\\PLAYER\\00_waist.x",	// 腰
	"data\\MODEL\\PLAYER\\01_body.x",	// 体
	"data\\MODEL\\PLAYER\\02_head.x",	// 頭
	"data\\MODEL\\PLAYER\\03_armUL.x",	// 左上腕
	"data\\MODEL\\PLAYER\\04_armUR.x",	// 右上腕
	"data\\MODEL\\PLAYER\\05_armDL.x",	// 左下腕
	"data\\MODEL\\PLAYER\\06_armDR.x",	// 右下腕
	"data\\MODEL\\PLAYER\\07_handL.x",	// 左手
	"data\\MODEL\\PLAYER\\08_handR.x",	// 右手
	"data\\MODEL\\PLAYER\\09_legUL.x",	// 左太もも
	"data\\MODEL\\PLAYER\\10_legUR.x",	// 右太もも
	"data\\MODEL\\PLAYER\\11_legDL.x",	// 左脛
	"data\\MODEL\\PLAYER\\12_legDR.x",	// 右脛
	"data\\MODEL\\PLAYER\\13_footL.x",	// 左足
	"data\\MODEL\\PLAYER\\14_footR.x",	// 右足
};

//************************************************************
//	子クラス [CPlayer] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CPlayer::CPlayer() : CObjectChara(CObject::LABEL_PLAYER)
{
	// メンバ変数をクリア
	m_pShadow		= NULL;			// 影の情報
	m_oldPos		= VEC3_ZERO;	// 過去位置
	m_move			= VEC3_ZERO;	// 移動量
	m_destRot		= VEC3_ZERO;	// 目標向き
	m_state			= STATE_NONE;	// 状態
	m_nCounterState	= 0;			// 状態管理カウンター
	m_nCounterSlide	= 0;			// スライディング管理カウンター
	m_fMove			= 0.0f;			// 移動量
	m_bJump			= false;		// ジャンプ状況
	m_bSlide		= false;		// スライディング状況
	m_bSlideControl	= false;		// スライディング操作状況
}

//============================================================
//	デストラクタ
//============================================================
CPlayer::~CPlayer()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CPlayer::Init(void)
{
	// メンバ変数を初期化
	m_pShadow		= NULL;			// 影の情報
	m_oldPos		= VEC3_ZERO;	// 過去位置
	m_move			= VEC3_ZERO;	// 移動量
	m_destRot		= VEC3_ZERO;	// 目標向き
	m_state			= STATE_NORMAL;	// 状態
	m_nCounterState	= 0;			// 状態管理カウンター
	m_nCounterSlide = 0;			// スライディング管理カウンター
	m_fMove			= basic::MOVE;	// 移動量
	m_bJump			= true;			// ジャンプ状況
	m_bSlide		= false;		// スライディング状況
	m_bSlideControl	= false;		// スライディング操作状況

	// オブジェクトキャラクターの初期化
	if (FAILED(CObjectChara::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// セットアップの読み込み
	LoadSetup();

	// モデル情報の設定
	SetModelInfo();

	// 影の生成
	m_pShadow = CShadow::Create(CShadow::TEXTURE_NORMAL, other::SHADOW_SIZE, this);
	if (m_pShadow == NULL)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 待機モーションを設定
	SetMotion(MOTION_IDOL);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CPlayer::Uninit(void)
{
	// 影の終了
	m_pShadow->Uninit();

	// オブジェクトキャラクターの終了
	CObjectChara::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CPlayer::Update(void)
{
	// 変数を宣言
	EMotion currentMotion = MOTION_IDOL;	// 現在のモーション

	// 過去位置の更新
	UpdateOldPosition();

	switch (m_state)
	{ // 状態ごとの処理
	case STATE_NONE:
		break;

	case STATE_NORMAL:

		// 通常状態の更新
		currentMotion = UpdateNormal();

		break;

	case STATE_DEATH:
		break;

	default:
		assert(false);
		break;
	}

	// 影の更新
	m_pShadow->Update();

	// モーション・オブジェクトキャラクターの更新
	UpdateMotion(currentMotion);
}

//============================================================
//	描画処理
//============================================================
void CPlayer::Draw(void)
{
	// オブジェクトキャラクターの描画
	CObjectChara::Draw();
}

//============================================================
//	ヒット処理
//============================================================
void CPlayer::Hit(const int /*nDmg*/)
{

#if 0	// TODO：Hit処理

	// 変数を宣言
	D3DXVECTOR3 pos = GetVec3Position();	// プレイヤー位置

	if (IsDeath() != true)
	{ // 死亡フラグが立っていない場合

		if (m_state == STATE_NORMAL)
		{ // 通常状態の場合

			// 体力からダメージ分減算
			m_pLife->AddNum(-nDmg);

			if (m_pLife->GetNum() > 0)
			{ // 生きている場合

				// カウンターを初期化
				m_nCounterState = 0;

				// 状態を変更
				m_state = STATE_DAMAGE;	// ダメージ状態
			}
			else
			{ // 死んでいる場合

				// カウンターを初期化
				m_nCounterState = 0;

				// 状態を変更
				m_state = STATE_DEATH;	// 死亡状態

				// モーションの設定
				SetMotion(MOTION_DEATH);	// 死亡モーション

				// サウンドの再生
				CManager::GetSound()->Play(CSound::LABEL_SE_BREAK);	// 破壊音
			}
		}
	}

#endif

}

//============================================================
//	状態の設定処理
//============================================================
void CPlayer::SetState(const int nState)
{
	// 引数の状態を設定
	m_state = (EState)nState;
}

//============================================================
//	状態取得処理
//============================================================
int CPlayer::GetState(void) const
{
	// 状態を返す
	return m_state;
}

//============================================================
//	半径取得処理
//============================================================
float CPlayer::GetRadius(void) const
{
	// 半径を返す
	return basic::RADIUS;
}

//============================================================
//	更新状況の設定処理
//============================================================
void CPlayer::SetEnableUpdate(const bool bUpdate)
{
	// 引数の更新状況を設定
	CObject::SetEnableUpdate(bUpdate);		// 自身
	m_pShadow->SetEnableUpdate(bUpdate);	// 影
}

//============================================================
//	描画状況の設定処理
//============================================================
void CPlayer::SetEnableDraw(const bool bDraw)
{
	// 引数の描画状況を設定
	CObject::SetEnableDraw(bDraw);		// 自身
	m_pShadow->SetEnableDraw(bDraw);	// 影
}

//============================================================
//	マトリックス取得処理
//============================================================
D3DXMATRIX CPlayer::GetMtxWorld(void) const
{
	// 変数を宣言
	D3DXMATRIX  mtxRot, mtxTrans, mtxWorld;		// 計算用マトリックス
	D3DXVECTOR3 posPlayer = GetVec3Position();	// プレイヤー位置
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// プレイヤー向き

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rotPlayer.y, rotPlayer.x, rotPlayer.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, posPlayer.x, posPlayer.y, posPlayer.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ワールドマトリックスを返す
	return mtxWorld;
}

//============================================================
//	生成処理
//============================================================
CPlayer *CPlayer::Create
(
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot		// 向き
)
{
	// 変数を宣言
	D3DXVECTOR3 pos = rPos;		// 座標設定用

	// ポインタを宣言
	CPlayer *pPlayer = NULL;	// プレイヤー生成用

	if (pPlayer == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pPlayer = new CPlayer;	// プレイヤー
	}
	else { assert(false); return NULL; }	// 使用中

	if (pPlayer != NULL)
	{ // 使用されている場合

		// プレイヤーの初期化
		if (FAILED(pPlayer->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pPlayer;
			pPlayer = NULL;

			// 失敗を返す
			return NULL;
		}

		// 位置を設定
		CScene::GetStage()->LimitPosition(pos, basic::RADIUS);	// ステージ範囲内補正
		pPlayer->SetVec3Position(pos);

		// 向きを設定
		pPlayer->SetVec3Rotation(rRot);
	}

	// 確保したアドレスを返す
	return pPlayer;
}

//============================================================
//	通常状態時の更新処理
//============================================================
CPlayer::EMotion CPlayer::UpdateNormal(void)
{
	// 変数を宣言
	EMotion currentMotion = MOTION_IDOL;		// 現在のモーション
	D3DXVECTOR3 posPlayer = GetVec3Position();	// プレイヤー位置
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// プレイヤー向き

	// ポインタを宣言
	CStage *pStage = CScene::GetStage();	// ステージ情報
	if (pStage == NULL)
	{ // ステージが使用されていない場合

		// 処理を抜ける
		assert(false);
		return MOTION_IDOL;
	}

	// 移動操作
	currentMotion = UpdateMove();

	// ジャンプ操作
	UpdateJump();

	// スライディング操作
	UpdateSliding();

	// 向き更新
	UpdateRotation(rotPlayer);

	// 着地判定
	UpdateLanding(posPlayer);

	// ステージ範囲外の補正
	pStage->LimitPosition(posPlayer, basic::RADIUS);

	// 障害物との当たり判定
	if (CollisionObstacle(posPlayer))
	{ // 当たった場合

		// TODO：死亡状態にする

		CParticle3D::Create(CParticle3D::TYPE_HEAL, posPlayer);
	}

	// 位置を反映
	SetVec3Position(posPlayer);

	// 向きを反映
	SetVec3Rotation(rotPlayer);

	// デバッグ表示
	CManager::GetInstance()->GetDebugProc()->Print("プレイヤー移動量：%f\n", m_fMove);
	CManager::GetInstance()->GetDebugProc()->Print((m_bJump) ? "ジャンプ：ON\n" : "ジャンプ：OFF\n");
	CManager::GetInstance()->GetDebugProc()->Print((m_bSlide) ? "スライディング：ON\n" : "スライディング：OFF\n");

	// 現在のモーションを返す
	return currentMotion;
}

//============================================================
//	過去位置の更新処理
//============================================================
void CPlayer::UpdateOldPosition(void)
{
	// 過去位置を更新
	m_oldPos = GetVec3Position();
}

//============================================================
//	移動量・目標向きの更新処理
//============================================================
CPlayer::EMotion CPlayer::UpdateMove(void)
{
	// 変数を宣言
	D3DXVECTOR3 rotCamera = CManager::GetInstance()->GetCamera()->GetVec3Rotation();	// カメラの向き

	// ポインタを宣言
	CInputKeyboard	*pKeyboard	= CManager::GetInstance()->GetKeyboard();	// キーボード
	CInputPad		*pPad		= CManager::GetInstance()->GetPad();		// パッド

#if 0
	if (pKeyboard->IsPress(DIK_W))
	{ // 奥移動の操作が行われた場合

		if (pKeyboard->IsPress(DIK_A))
		{ // 左移動の操作も行われた場合 (左奥移動)

			// 移動量を更新
			m_move.x += sinf(rotCamera.y - (D3DX_PI * 0.25f)) * 2.0f;
			m_move.z += cosf(rotCamera.y - (D3DX_PI * 0.25f)) * 2.0f;

			// 目標向きを更新
			m_destRot.y = D3DXToRadian(135) + rotCamera.y;
		}
		else if (pKeyboard->IsPress(DIK_D))
		{ // 右移動の操作も行われた場合 (右奥移動)

			// 移動量を更新
			m_move.x -= sinf(rotCamera.y - (D3DX_PI * 0.75f)) * 2.0f;
			m_move.z -= cosf(rotCamera.y - (D3DX_PI * 0.75f)) * 2.0f;

			// 目標向きを更新
			m_destRot.y = D3DXToRadian(225) + rotCamera.y;
		}
		else
		{ // 奥移動の操作だけが行われた場合 (奥移動)

			// 移動量を更新
			m_move.x += sinf(rotCamera.y) * 2.0f;
			m_move.z += cosf(rotCamera.y) * 2.0f;

			// 目標向きを更新
			m_destRot.y = D3DXToRadian(180) + rotCamera.y;
		}
	}
	else if (pKeyboard->IsPress(DIK_S))
	{ // 手前移動の操作が行われた場合

		if (pKeyboard->IsPress(DIK_A))
		{ // 左移動の操作も行われた場合 (左手前移動)

			// 移動量を更新
			m_move.x += sinf(rotCamera.y - (D3DX_PI * 0.75f)) * 2.0f;
			m_move.z += cosf(rotCamera.y - (D3DX_PI * 0.75f)) * 2.0f;

			// 目標向きを更新
			m_destRot.y = D3DXToRadian(45) + rotCamera.y;
		}
		else if (pKeyboard->IsPress(DIK_D))
		{ // 右移動の操作も行われた場合 (右手前移動)

			// 移動量を更新
			m_move.x -= sinf(rotCamera.y - (D3DX_PI * 0.25f)) * 2.0f;
			m_move.z -= cosf(rotCamera.y - (D3DX_PI * 0.25f)) * 2.0f;

			// 目標向きを更新
			m_destRot.y = D3DXToRadian(315) + rotCamera.y;
		}
		else
		{ // 手前移動の操作だけが行われた場合 (手前移動)

			// 移動量を更新
			m_move.x -= sinf(rotCamera.y) * 2.0f;
			m_move.z -= cosf(rotCamera.y) * 2.0f;

			// 目標向きを更新
			m_destRot.y = D3DXToRadian(0) + rotCamera.y;
		}
	}
	else if (pKeyboard->IsPress(DIK_A))
	{ // 左移動の操作が行われた場合

		// 移動量を更新
		m_move.x += sinf(rotCamera.y - (D3DX_PI * 0.5f)) * 2.0f;
		m_move.z += cosf(rotCamera.y - (D3DX_PI * 0.5f)) * 2.0f;

		// 目標向きを更新
		m_destRot.y = D3DXToRadian(90) + rotCamera.y;
	}
	else if (pKeyboard->IsPress(DIK_D))
	{ // 右移動の操作が行われた場合

		// 移動量を更新
		m_move.x -= sinf(rotCamera.y - (D3DX_PI * 0.5f)) * 2.0f;
		m_move.z -= cosf(rotCamera.y - (D3DX_PI * 0.5f)) * 2.0f;

		// 目標向きを更新
		m_destRot.y = D3DXToRadian(270) + rotCamera.y;
	}
#else
	// 移動量を更新
	m_move.x += m_fMove;

	// 目標向きを更新
	m_destRot.y = atan2f(-m_move.x, -m_move.z);

	if (!m_bSlide)
	{ // スライディング中ではない場合

		if (m_fMove < basic::MOVE)
		{ // 移動量が最高速度ではない場合

			// 移動量を加算
			m_fMove += slide::ADD_MOVE;

			if (m_fMove > basic::MOVE)
			{ // 移動量が最高速度を超えた場合

				// 移動量を補正
				m_fMove = basic::MOVE;
			}
		}
	}
#endif

	// 移動モーションを返す
	return MOTION_MOVE;
}

//============================================================
//	ジャンプの更新処理
//============================================================
void CPlayer::UpdateJump(void)
{
	// ポインタを宣言
	CInputKeyboard	*pKeyboard	= CManager::GetInstance()->GetKeyboard();	// キーボード
	CInputPad		*pPad		= CManager::GetInstance()->GetPad();		// パッド

	if (!m_bJump && !m_bSlide)
	{ // ジャンプとスライディングをしていない場合

		if (pKeyboard->IsTrigger(DIK_SPACE) || pPad->IsTrigger(CInputPad::KEY_B))
		{ // ジャンプの操作が行われた場合

			// 上移動量
			m_move.y += basic::JUMP;

			// ジャンプしている状態にする
			m_bJump = true;

			CEffect3D::Create(GetVec3Position(), 80.0f, CEffect3D::TYPE_NORMAL, 20);
		}
	}
}

//============================================================
//	スライディングの更新処理
//============================================================
void CPlayer::UpdateSliding(void)
{
	// ポインタを宣言
	CInputKeyboard	*pKeyboard	= CManager::GetInstance()->GetKeyboard();	// キーボード
	CInputPad		*pPad		= CManager::GetInstance()->GetPad();		// パッド

	if (!m_bJump && !m_bSlide)
	{ // ジャンプとスライディングをしていない場合

		if (pKeyboard->IsTrigger(DIK_RETURN) || pPad->IsTrigger(CInputPad::KEY_A))
		{ // スライディングの操作が行われた場合

			// スライディング操作が行われた情報を保存
			m_bSlideControl = true;
		}

		if (m_fMove >= slide::SLIDE_CONTROL_MIN)
		{ // スライディングが可能な移動速度の場合

			if (m_bSlideControl)
			{ // スライディングの操作が行われた場合

				// スライディング操作が行われた情報を初期化
				m_bSlideControl = false;

				// スライディングしている状態にする
				m_bSlide = true;

				// スライディングモーションを設定
				SetMotion(MOTION_SLIDE);
			}
		}
	}

	if (m_bSlide)
	{ // スライディングしている場合

		// 変数を宣言
		float fPlusSubMove = (slide::PLUS_SUB_MOVE / (float)slide::MAX_END_CNT) * (float)(slide::MAX_END_CNT - m_nCounterSlide);	// 移動量の追加減算量

		// カウンターを加算
		m_nCounterSlide++;

		if (m_fMove > slide::MIN_MOVE)
		{ // 移動量が最低速度ではない場合

			// 移動量を減算
			m_fMove -= slide::SUB_MOVE + fPlusSubMove;

			if (m_fMove < slide::MIN_MOVE)
			{ // 移動量が最低速度を超えた場合

				// 移動量を補正
				m_fMove = slide::MIN_MOVE;
			}
		}

		if (m_nCounterSlide > slide::MIN_END_CNT)
		{ // スライディングの解除操作ができるカウントに到達した場合

			if (!(pKeyboard->IsPress(DIK_RETURN) || pPad->IsPress(CInputPad::KEY_A)))
			{ // スライディング解除の操作が行われた場合

				// カウンターを初期化
				m_nCounterSlide = 0;

				// スライディングしていない状態にする
				m_bSlide = false;
			}
		}

		if (m_nCounterSlide > slide::MAX_END_CNT)
		{ // スライディングが強制終了するカウントに到達した場合

			// カウンターを初期化
			m_nCounterSlide = 0;

			// スライディングしていない状態にする
			m_bSlide = false;
		}
	}
}

//============================================================
//	着地状況の更新処理
//============================================================
bool CPlayer::UpdateLanding(D3DXVECTOR3& rPos)
{
	// 変数を宣言
	bool bLand = false;	// 着地状況

	// 着地判定
	if (CollisionBuilding(rPos))
	{ // ブロックに着地していた場合

		// 着地している状態にする
		bLand = true;

		// ジャンプしていない状態にする
		m_bJump = false;
	}
	else if (CScene::GetField()->LandPosition(rPos, m_move)
	||       CScene::GetStage()->LandPosition(rPos, m_move, 0.0f))
	{ // プレイヤーが着地していた場合

		// 着地している状態にする
		bLand = true;

		// ジャンプしていない状態にする
		m_bJump = false;
	}
	else
	{ // 着地していない場合

		// ジャンプしている状態にする
		m_bJump = true;
	}

	// 着地状況を返す
	return bLand;
}

//============================================================
//	向きの更新処理
//============================================================
void CPlayer::UpdateRotation(D3DXVECTOR3& rRot)
{
	// 変数を宣言
	float fDiffRot = 0.0f;	// 差分向き

	// 目標向きの正規化
	useful::NormalizeRot(m_destRot.y);

	// 目標向きまでの差分を計算
	fDiffRot = m_destRot.y - rRot.y;

	// 差分向きの正規化
	useful::NormalizeRot(fDiffRot);

	// 向きの更新
	rRot.y += fDiffRot * basic::REV_ROTA;

	// 向きの正規化
	useful::NormalizeRot(rRot.y);
}

//============================================================
//	モーション・オブジェクトキャラクターの更新処理
//============================================================
void CPlayer::UpdateMotion(int nMotion)
{
	// 変数を宣言
	int nAnimMotion = GetMotionType();	// 現在再生中のモーション

	if (nMotion != NONE_IDX)
	{ // モーションが設定されている場合

		if (IsMotionLoop(nAnimMotion))
		{ // ループするモーションだった場合

			if (nAnimMotion != nMotion)
			{ // 現在のモーションが再生中のモーションと一致しない場合

				// 現在のモーションの設定
				SetMotion(nMotion);
			}
		}
	}

	// オブジェクトキャラクターの更新
	CObjectChara::Update();

	// モーションの遷移
	if (IsMotionFinish())
	{ // モーションが終了していた場合

		switch (GetMotionType())
		{ // モーションの種類ごとの処理
		case MOTION_SLIDE:	// スライディングモーション

			if (!m_bSlide)
			{ // スライディング中ではない場合

				// 現在のモーションの設定
				SetMotion(nMotion);
			}

			// 処理を抜ける
			break;
		}
	}
}

//============================================================
//	ビルとの一軸ごとの当たり判定
//============================================================
bool CPlayer::ResponseSingleBuilding(const EAxis axis, D3DXVECTOR3& rPos)
{
	// 変数を宣言
	D3DXVECTOR3 sizeMinPlayer = D3DXVECTOR3(basic::RADIUS, 0.0f, basic::RADIUS);			// プレイヤー最小大きさ
	D3DXVECTOR3 sizeMaxPlayer = D3DXVECTOR3(basic::RADIUS, basic::HEIGHT, basic::RADIUS);	// プレイヤー最大大きさ
	bool bMin = false;	// 不の方向の判定状況
	bool bMax = false;	// 正の方向の判定状況
	bool bHit = false;	// 各軸の判定情報

	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // 優先順位の総数分繰り返す

		// ポインタを宣言
		CObject *pObjectTop = CObject::GetTop(nCntPri);	// 先頭オブジェクト

		if (pObjectTop != NULL)
		{ // 先頭が存在する場合

			// ポインタを宣言
			CObject *pObjCheck = pObjectTop;	// オブジェクト確認用

			while (pObjCheck != NULL)
			{ // オブジェクトが使用されている場合繰り返す

				// 変数を宣言
				D3DXVECTOR3 posBuild = VEC3_ZERO;		// ビル位置
				D3DXVECTOR3 sizeMinBuild = VEC3_ZERO;	// ビル最小大きさ
				D3DXVECTOR3 sizeMaxBuild = VEC3_ZERO;	// ビル最大大きさ

				// ポインタを宣言
				CObject *pObjectNext = pObjCheck->GetNext();	// 次オブジェクト

				if (pObjCheck->GetLabel() != CObject::LABEL_BUILDING)
				{ // オブジェクトラベルがビルではない場合

					// 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;

					// 次の繰り返しに移行
					continue;
				}

				// ビルの位置を設定
				posBuild = pObjCheck->GetVec3Position();

				// ビルの最小の大きさを設定
				sizeMinBuild = pObjCheck->GetVec3Sizing();
				sizeMinBuild.y = 0.0f;	// 縦の大きさを初期化

				// ビルの最大の大きさを設定
				sizeMaxBuild = pObjCheck->GetVec3Sizing();
				sizeMaxBuild.y *= 2.0f;	// 縦の大きさを倍にする

				switch (axis)
				{ // 判定軸ごとの処理
				case AXIS_X:	// X軸

					// X軸の衝突判定
					collision::ResponseSingleX
					( // 引数
						rPos,			// 判定位置
						m_oldPos,		// 判定過去位置
						posBuild,		// 判定目標位置
						sizeMaxPlayer,	// 判定サイズ(右・上・後)
						sizeMinPlayer,	// 判定サイズ(左・下・前)
						sizeMaxBuild,	// 判定目標サイズ(右・上・後)
						sizeMinBuild,	// 判定目標サイズ(左・下・前)
						&m_move,		// 移動量
						&bMin,			// 左からの判定
						&bMax			// 右からの判定
					);

					if (bMin || bMax)
					{ // 横から当たっていた場合

						// ぶつかり状況にする
						bHit = true;
					}

					break;

				case AXIS_Y:	// Y軸

					// Y軸の衝突判定
					collision::ResponseSingleY
					( // 引数
						rPos,			// 判定位置
						m_oldPos,		// 判定過去位置
						posBuild,		// 判定目標位置
						sizeMaxPlayer,	// 判定サイズ(右・上・後)
						sizeMinPlayer,	// 判定サイズ(左・下・前)
						sizeMaxBuild,	// 判定目標サイズ(右・上・後)
						sizeMinBuild,	// 判定目標サイズ(左・下・前)
						&m_move,		// 移動量
						&bMin,			// 下からの判定
						&bMax			// 上からの判定
					);

					if (bMax)
					{ // 上から当たっていた場合

						// 着地している状況にする
						bHit = true;
					}

					break;

				case AXIS_Z:	// Z軸

					// Z軸の衝突判定
					collision::ResponseSingleZ
					( // 引数
						rPos,			// 判定位置
						m_oldPos,		// 判定過去位置
						posBuild,		// 判定目標位置
						sizeMaxPlayer,	// 判定サイズ(右・上・後)
						sizeMinPlayer,	// 判定サイズ(左・下・前)
						sizeMaxBuild,	// 判定目標サイズ(右・上・後)
						sizeMinBuild,	// 判定目標サイズ(左・下・前)
						&m_move,		// 移動量
						&bMin,			// 前からの判定
						&bMax			// 後からの判定
					);

					if (bMin || bMax)
					{ // 横から当たっていた場合

						// ぶつかり状況にする
						bHit = true;
					}

					break;

				default:	// 例外処理
					assert(false);
					break;
				}

				// 次のオブジェクトへのポインタを代入
				pObjCheck = pObjectNext;
			}
		}
	}

	// 各軸の判定情報を返す
	return bHit;
}

//============================================================
//	ビルとの当たり判定
//============================================================
bool CPlayer::CollisionBuilding(D3DXVECTOR3& rPos)
{
	// 変数を宣言
	bool bLand = false;	// 着地状況

	// 重力を加算
	m_move.y -= basic::GRAVITY;

	// 移動量を加算
	rPos.x += m_move.x;

	// X軸の当たり判定
	if (ResponseSingleBuilding(AXIS_X, rPos))
	{ // 横に当たっていた場合

		// TODO：死亡状態にする

		CEffect3D::Create(rPos, 40.0f, CEffect3D::TYPE_NORMAL, 10);
	}

	// 移動量を加算
	rPos.y += m_move.y;

	// Y軸の当たり判定
	if (ResponseSingleBuilding(AXIS_Y, rPos))
	{ // 上に当たっていた場合

		// 着地している状況にする
		bLand = true;
	}

	// 移動量を加算
	rPos.z += m_move.z;

	// Z軸の当たり判定
	if (ResponseSingleBuilding(AXIS_Z, rPos))
	{ // 横に当たっていた場合

		// TODO：死亡状態にする

		CEffect3D::Create(rPos, 40.0f, CEffect3D::TYPE_NORMAL, 10);
	}

	// 移動量を減衰
	if (m_bJump)
	{ // 空中の場合

		m_move.x += (0.0f - m_move.x) * basic::JUMP_REV;
		m_move.z += (0.0f - m_move.z) * basic::JUMP_REV;
	}
	else
	{ // 地上の場合

		m_move.x += (0.0f - m_move.x) * basic::LAND_REV;
		m_move.z += (0.0f - m_move.z) * basic::LAND_REV;
	}

	// 着地状況を返す
	return bLand;
}

//============================================================
//	障害物との当たり判定
//============================================================
bool CPlayer::CollisionObstacle(D3DXVECTOR3& rPos)
{
	// 変数を宣言
	D3DXVECTOR3 sizeMinPlayer = D3DXVECTOR3(basic::RADIUS, 0.0f, basic::RADIUS);			// プレイヤー最小大きさ
	D3DXVECTOR3 sizeMaxPlayer = D3DXVECTOR3(basic::RADIUS, basic::HEIGHT, basic::RADIUS);	// プレイヤー最大大きさ

	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // 優先順位の総数分繰り返す

		// ポインタを宣言
		CObject *pObjectTop = CObject::GetTop(nCntPri);	// 先頭オブジェクト

		if (pObjectTop != NULL)
		{ // 先頭が存在する場合

			// ポインタを宣言
			CObject *pObjCheck = pObjectTop;	// オブジェクト確認用

			while (pObjCheck != NULL)
			{ // オブジェクトが使用されている場合繰り返す

				// 変数を宣言
				D3DXVECTOR3 posBuild = VEC3_ZERO;		// ビル位置
				D3DXVECTOR3 sizeMinBuild = VEC3_ZERO;	// ビル最小大きさ
				D3DXVECTOR3 sizeMaxBuild = VEC3_ZERO;	// ビル最大大きさ
				bool bHit = false;	// 判定情報

				// ポインタを宣言
				CObject *pObjectNext = pObjCheck->GetNext();	// 次オブジェクト

				if (pObjCheck->GetLabel() != CObject::LABEL_OBSTACLE)
				{ // オブジェクトラベルが障害物ではない場合

					// 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;

					// 次の繰り返しに移行
					continue;
				}

				// 障害物の位置を設定
				posBuild = pObjCheck->GetVec3Position();

				// 障害物の最小の大きさを設定
				sizeMinBuild = pObjCheck->GetVec3Sizing();
				sizeMinBuild.y = 0.0f;	// 縦の大きさを初期化

				// 障害物の最大の大きさを設定
				sizeMaxBuild = pObjCheck->GetVec3Sizing();
				sizeMaxBuild.y *= 2.0f;	// 縦の大きさを倍にする

				switch (pObjCheck->GetType())
				{ // 回避法ごとの処理
				case CObstacle::DODGE_JUMP:		// ジャンプ回避

					if (!m_bJump)
					{ // ジャンプ中ではない場合

						// 二軸の矩形の衝突判定
						bHit = collision::Box2D
						( // 引数
							rPos,			// 判定位置
							posBuild,		// 判定目標位置
							sizeMaxPlayer,	// 判定サイズ(右・上・後)
							sizeMinPlayer,	// 判定サイズ(左・下・前)
							sizeMaxBuild,	// 判定目標サイズ(右・上・後)
							sizeMinBuild	// 判定目標サイズ(左・下・前)
						);
					}

					break;

				case CObstacle::DODGE_SLIDE:	// スライディング回避

					if (!m_bSlide)
					{ // スライディング中ではない場合

						// 二軸の矩形の衝突判定
						bHit = collision::Box2D
						( // 引数
							rPos,			// 判定位置
							posBuild,		// 判定目標位置
							sizeMaxPlayer,	// 判定サイズ(右・上・後)
							sizeMinPlayer,	// 判定サイズ(左・下・前)
							sizeMaxBuild,	// 判定目標サイズ(右・上・後)
							sizeMinBuild	// 判定目標サイズ(左・下・前)
						);
					}

					break;

				default:	// 例外処理
					assert(false);
					break;
				}

				if (bHit)
				{ // 当たっていた場合

					// 当たっている情報を返す
					return true;
				}

				// 次のオブジェクトへのポインタを代入
				pObjCheck = pObjectNext;
			}
		}
	}

	// 当たっていない情報を返す
	return false;
}

//============================================================
//	セットアップ処理
//============================================================
void CPlayer::LoadSetup(void)
{
	// 変数を宣言
	CMotion::SMotionInfo info;		// ポーズの代入用
	D3DXVECTOR3 pos = VEC3_ZERO;	// 位置の代入用
	D3DXVECTOR3 rot = VEC3_ZERO;	// 向きの代入用
	int nID			= 0;	// インデックスの代入用
	int nParentID	= 0;	// 親インデックスの代入用
	int nNowPose	= 0;	// 現在のポーズ番号
	int nNowKey		= 0;	// 現在のキー番号
	int nLoop		= 0;	// ループのON/OFFの変換用
	int nEnd		= 0;	// テキスト読み込み終了の確認用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	// ポインタを宣言
	FILE *pFile;	// ファイルポインタ

	// ポーズ代入用の変数を初期化
	memset(&info, 0, sizeof(info));

	// ファイルを読み込み形式で開く
	pFile = fopen(SETUP_TXT, "r");

	if (pFile != NULL)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			// キャラクターの設定
			if (strcmp(&aString[0], "CHARACTERSET") == 0)
			{ // 読み込んだ文字列が CHARACTERSET の場合

				do
				{ // 読み込んだ文字列が END_CHARACTERSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "PARTSSET") == 0)
					{ // 読み込んだ文字列が PARTSSET の場合

						do
						{ // 読み込んだ文字列が END_PARTSSET ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "INDEX") == 0)
							{ // 読み込んだ文字列が INDEX の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%d", &nID);			// モデルのインデックスを読み込む
							}
							else if (strcmp(&aString[0], "PARENT") == 0)
							{ // 読み込んだ文字列が PARENT の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%d", &nParentID);	// モデルの親のインデックスを読み込む
							}
							else if (strcmp(&aString[0], "POS") == 0)
							{ // 読み込んだ文字列が POS の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%f", &pos.x);		// X座標を読み込む
								fscanf(pFile, "%f", &pos.y);		// Y座標を読み込む
								fscanf(pFile, "%f", &pos.z);		// Z座標を読み込む
							}
							else if (strcmp(&aString[0], "ROT") == 0)
							{ // 読み込んだ文字列が ROT の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%f", &rot.x);		// X向きを読み込む
								fscanf(pFile, "%f", &rot.y);		// Y向きを読み込む
								fscanf(pFile, "%f", &rot.z);		// Z向きを読み込む
							}
						} while (strcmp(&aString[0], "END_PARTSSET") != 0);	// 読み込んだ文字列が END_PARTSSET ではない場合ループ

						// パーツ情報の設定
						CObjectChara::SetPartsInfo(nID, nParentID, pos, rot, mc_apModelFile[nID]);
					}
				} while (strcmp(&aString[0], "END_CHARACTERSET") != 0);		// 読み込んだ文字列が END_CHARACTERSET ではない場合ループ
			}

			// モーションの設定
			else if (strcmp(&aString[0], "MOTIONSET") == 0)
			{ // 読み込んだ文字列が MOTIONSET の場合

				// 現在のポーズ番号を初期化
				nNowPose = 0;

				do
				{ // 読み込んだ文字列が END_MOTIONSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "LOOP") == 0)
					{ // 読み込んだ文字列が LOOP の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &nLoop);		// ループのON/OFFを読み込む

						// 読み込んだ値をbool型に変換
						info.bLoop = (nLoop == 0) ? false : true;
					}
					else if (strcmp(&aString[0], "NUM_KEY") == 0)
					{ // 読み込んだ文字列が NUM_KEY の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &info.nNumKey);	// キーの総数を読み込む
					}
					else if (strcmp(&aString[0], "KEYSET") == 0)
					{ // 読み込んだ文字列が KEYSET の場合

						// 現在のキー番号を初期化
						nNowKey = 0;

						do
						{ // 読み込んだ文字列が END_KEYSET ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "FRAME") == 0)
							{ // 読み込んだ文字列が FRAME の場合

								fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
								fscanf(pFile, "%d", &info.aKeyInfo[nNowPose].nFrame);	// キーが切り替わるまでのフレーム数を読み込む
							}
							else if (strcmp(&aString[0], "KEY") == 0)
							{ // 読み込んだ文字列が KEY の場合

								do
								{ // 読み込んだ文字列が END_KEY ではない場合ループ

									// ファイルから文字列を読み込む
									fscanf(pFile, "%s", &aString[0]);

									if (strcmp(&aString[0], "POS") == 0)
									{ // 読み込んだ文字列が POS の場合

										fscanf(pFile, "%s", &aString[0]);									// = を読み込む (不要)
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].pos.x);	// X位置を読み込む
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].pos.y);	// Y位置を読み込む
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].pos.z);	// Z位置を読み込む

										// 読み込んだ位置にパーツの初期位置を加算
										info.aKeyInfo[nNowPose].aKey[nNowKey].pos += GetPartsPosition(nNowKey);
									}
									else if (strcmp(&aString[0], "ROT") == 0)
									{ // 読み込んだ文字列が ROT の場合

										fscanf(pFile, "%s", &aString[0]);									// = を読み込む (不要)
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].rot.x);	// X向きを読み込む
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].rot.y);	// Y向きを読み込む
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].rot.z);	// Z向きを読み込む

										// 読み込んだ向きにパーツの初期向きを加算
										info.aKeyInfo[nNowPose].aKey[nNowKey].rot += GetPartsRotation(nNowKey);

										// 初期向きを正規化
										useful::NormalizeRot(info.aKeyInfo[nNowPose].aKey[nNowKey].rot.x);
										useful::NormalizeRot(info.aKeyInfo[nNowPose].aKey[nNowKey].rot.y);
										useful::NormalizeRot(info.aKeyInfo[nNowPose].aKey[nNowKey].rot.z);
									}

								} while (strcmp(&aString[0], "END_KEY") != 0);	// 読み込んだ文字列が END_KEY ではない場合ループ

								// 現在のキー番号を加算
								nNowKey++;
							}
						} while (strcmp(&aString[0], "END_KEYSET") != 0);	// 読み込んだ文字列が END_KEYSET ではない場合ループ

						// 現在のポーズ番号を加算
						nNowPose++;
					}
				} while (strcmp(&aString[0], "END_MOTIONSET") != 0);	// 読み込んだ文字列が END_MOTIONSET ではない場合ループ

				// モーション情報の設定
				CObjectChara::SetMotionInfo(info);
			}
		} while (nEnd != EOF);	// 読み込んだ文字列が EOF ではない場合ループ
		
		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(NULL, "プレイヤーセットアップファイルの読み込みに失敗！", "警告！", MB_ICONWARNING);
	}
}
