//============================================================
//
//	影処理 [shadow.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "shadow.h"
#include "manager.h"
#include "scene.h"
#include "renderer.h"
#include "texture.h"
#include "collision.h"
#include "field.h"
#include "stage.h"
#include "building.h"
#include "obstacle.h"

//************************************************************
//	マクロ定義
//************************************************************
#define SHADOW_PRIO	(2)		// 影の優先順位
#define PLUS_POSY	(0.01f)	// ちらつき防止用の縦座標加算量

#define MAX_DIS_HEIGHT	(200.0f)	// 影と親の縦の距離の最大値
#define MAX_PLUS_SIZE	(120.0f)	// 影の大きさ加算量の最大値

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CShadow::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\shadow000.jpg",	// 通常テクスチャ
};

//************************************************************
//	子クラス [CShadow] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CShadow::CShadow(const float fMinAlpha, const float fMaxAlpha) : CObject3D(CObject::LABEL_NONE, SHADOW_PRIO), m_fMinAlpha(fMinAlpha), m_fMaxAlpha(fMaxAlpha)
{
	// メンバ変数をクリア
	m_pParentObject = NULL;	// 親オブジェクト
}

//============================================================
//	デストラクタ
//============================================================
CShadow::~CShadow()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CShadow::Init(void)
{
	// メンバ変数を初期化
	m_pParentObject = NULL;	// 親オブジェクト

	// オブジェクト3Dの初期化
	if (FAILED(CObject3D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// Zテストの設定
	SetFunc(D3DCMP_LESSEQUAL);

	// Zバッファの使用状況の設定
	SetZEnable(false);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CShadow::Uninit(void)
{
	// 親オブジェクトの削除
	DeleteObjectParent();

	// オブジェクト3Dの終了
	CObject3D::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CShadow::Update(void)
{
	// 描画情報を設定
	if (FAILED(SetDrawInfo()))
	{ // 描画情報の設定に失敗した場合

		// オブジェクトの終了
		Uninit();

		// 関数を抜ける
		return;
	}

	// オブジェクト3Dの更新
	CObject3D::Update();
}

//============================================================
//	描画処理
//============================================================
void CShadow::Draw(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ

	// αブレンディングを減算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// オブジェクト3Dの描画
	CObject3D::Draw();

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//============================================================
//	生成処理
//============================================================
CShadow *CShadow::Create
(
	const ETexture texture,		// 種類
	const D3DXVECTOR3& rSize,	// 大きさ
	CObject *pObject,			// 親オブジェクト
	const float fMinAlpha,		// 透明度の最小値
	const float fMaxAlpha		// 透明度の最大値
)
{
	// 変数を宣言
	int nTextureID;	// テクスチャインデックス

	// ポインタを宣言
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// テクスチャへのポインタ
	CShadow *pShadow = NULL;	// 影生成用

	if (pShadow == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pShadow = new CShadow(fMinAlpha, fMaxAlpha);	// 影
	}
	else { assert(false); return NULL; }	// 使用中

	if (pShadow != NULL)
	{ // 確保に成功している場合

		// 影の初期化
		if (FAILED(pShadow->Init()))
		{ // 初期化に失敗した場合

			// 失敗を返す
			assert(false);
			return NULL;
		}

		// テクスチャを登録
		nTextureID = pTexture->Regist(mc_apTextureFile[texture]);

		// テクスチャを割当
		pShadow->BindTexture(nTextureID);

		// 元の大きさを設定
		pShadow->SetScalingOrigin(rSize);

		// 親オブジェクトを設定
		pShadow->SetParentObject(pObject);

		// 描画情報を設定
		if (FAILED(pShadow->SetDrawInfo()))
		{ // 描画情報の設定に失敗した場合

			// メモリ開放
			delete pShadow;
			pShadow = NULL;

			// 失敗を返す
			assert(false);
			return NULL;
		}

		// 確保したアドレスを返す
		return pShadow;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	元の大きさの設定処理
//============================================================
void CShadow::SetScalingOrigin(const D3DXVECTOR3& rSize)
{
	// 引数の大きさを代入
	m_sizeOrigin = rSize;

	// 大きさの設定
	CObject3D::SetVec3Sizing(rSize);
}

//============================================================
//	描画情報の設定処理
//============================================================
HRESULT CShadow::SetDrawInfo(void)
{
	if (m_pParentObject != NULL)
	{ // 親オブジェクトが使用されていた場合

		// 変数を宣言
		D3DXVECTOR3 posParent = m_pParentObject->GetVec3Position();	// 親オブジェクト位置
		D3DXVECTOR3 posShadow  = VEC3_ZERO;	// 影位置
		D3DXVECTOR3 sizeShadow = VEC3_ZERO;	// 影大きさ
		float fDis = 0.0f;		// 影と親の距離
		float fAlpha = 0.0f;	// 影の透明度

		// 描画位置の設定
		posShadow = SetDrawPosition();

		// 影と親の縦座標の距離を求める
		fDis = fabsf(posParent.y - posShadow.y);		// 縦の距離を求める
		useful::LimitNum(fDis, 0.0f, MAX_DIS_HEIGHT);	// 縦の距離を制限
		fDis *= 1.0f / MAX_DIS_HEIGHT;					// 距離を割合化

		// 影の大きさを求める
		sizeShadow = D3DXVECTOR3(m_sizeOrigin.x + (MAX_PLUS_SIZE * fDis), 0.0f, m_sizeOrigin.z + (MAX_PLUS_SIZE * fDis));

		// α値を求める
		fAlpha = fabsf(fDis - 1.0f);	// α値を設定
		useful::LimitNum(fAlpha, m_fMinAlpha, m_fMaxAlpha);	// α値を制限

		// 影の描画情報を設定
		SetVec3Position(posShadow);	// 位置
		SetVec3Sizing(sizeShadow);	// 大きさ
		SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha));	// 色

		// 成功を返す
		return S_OK;
	}
	else
	{ // 親オブジェクトが使用されていなかった場合

		// 失敗を返す
		return E_FAIL;
	}
}

//============================================================
//	描画位置の設定処理
//============================================================
D3DXVECTOR3 CShadow::SetDrawPosition(void)
{
	// 変数を宣言
	D3DXVECTOR3 posParent = m_pParentObject->GetVec3Position();	// 親オブジェクト位置
	D3DXVECTOR3 posShadow = VEC3_ZERO;	// 影位置
	float fPosHeight = 0.0f;	// ビルの上座標

	// ポインタを宣言
	CStage *pStage = CScene::GetStage();	// ステージの情報
	if (pStage == NULL)
	{ // ステージが存在しない場合

		// 関数を抜ける
		return VEC3_ZERO;
	}

	// 影の位置を求める
	posShadow = posParent;	// 親オブジェクトの座標代入

	if (CollisionObstacle(posParent, fPosHeight))
	{ // 障害物の範囲内の場合

		// 高さを障害物の上に設定
		posShadow.y = fPosHeight + PLUS_POSY;
	}
	else if (CollisionBuilding(posParent, fPosHeight))
	{ // ビルの範囲内の場合

		// 高さをビルの上に設定
		posShadow.y = fPosHeight + PLUS_POSY;
	}
	else if (pStage->IsFieldPositionRange(posParent))
	{ // 地面の範囲内の場合

		// 高さを地面に設定
		posShadow.y = pStage->GetFieldPositionHeight(posParent) + PLUS_POSY;
	}
	else
	{ // 全ての範囲外の場合

		// 高さを制限位置に設定
		posShadow.y = pStage->GetStageLimit().fField + PLUS_POSY;
	}

	// 影位置を返す
	return posShadow;
}

//============================================================
//	親オブジェクトの設定処理
//============================================================
void CShadow::SetParentObject(CObject *pObject)
{
	// 引数のオブジェクトを設定
	m_pParentObject = pObject;
}

//============================================================
//	親オブジェクトの削除処理
//============================================================
void CShadow::DeleteObjectParent(void)
{
	// 親オブジェクトをNULLにする
	m_pParentObject = NULL;
}

//============================================================
//	障害物との当たり判定
//============================================================
bool CShadow::CollisionObstacle(D3DXVECTOR3& rPos, float& rDrawPos)
{
	// 変数を宣言
	float fDisPosY = 0.0f;	// プレイヤーとビルのY距離
	bool bInitDis = false;	// Y距離の初期化状況

	// ポインタを宣言
	CObject *pCurrentObj = NULL;	// 現在の影乗っかりオブジェクト

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
				D3DXVECTOR3 pos0, pos1, pos2, pos3;	// 頂点座標代入用
				D3DXVECTOR3 posObs  = VEC3_ZERO;	// 障害物位置
				D3DXVECTOR3 rotObs  = VEC3_ZERO;	// 障害物向き
				D3DXVECTOR3 sizeObs = VEC3_ZERO;	// 障害物大きさ
				float fAngle  = 0.0f;	// 対角線の角度
				float fLength = 0.0f;	// 対角線の長さ
				bool  bHit = false;		// 判定情報

				// ポインタを宣言
				CObject *pObjectNext = pObjCheck->GetNext();	// 次オブジェクト

				if (pObjCheck->GetLabel() != CObject::LABEL_OBSTACLE)
				{ // オブジェクトラベルが障害物ではない場合

					// 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;

					// 次の繰り返しに移行
					continue;
				}

				if (pObjCheck->GetState() != CObstacle::STATE_LAND)
				{ // 特殊状態が着地できるではない場合

					// 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;

					// 次の繰り返しに移行
					continue;
				}

				// 障害物の情報を取得
				posObs  = pObjCheck->GetVec3Position();	// 位置
				rotObs  = pObjCheck->GetVec3Rotation();	// 向き
				sizeObs = pObjCheck->GetVec3Sizing();	// 大きさ
				fAngle  = pObjCheck->GetAngle();		// 対角線角度
				fLength = pObjCheck->GetLength();		// 対角線長さ

				// 頂点座標を計算
				pos0.x = posObs.x + sinf(rotObs.y + (D3DX_PI + fAngle)) * fLength;
				pos0.y = 0.0f;
				pos0.z = posObs.z + cosf(rotObs.y + (D3DX_PI + fAngle)) * fLength;
				pos1.x = posObs.x + sinf(rotObs.y + (D3DX_PI - fAngle)) * fLength;
				pos1.y = 0.0f;
				pos1.z = posObs.z + cosf(rotObs.y + (D3DX_PI - fAngle)) * fLength;
				pos3.x = posObs.x + sinf(rotObs.y - fAngle) * fLength;
				pos3.y = 0.0f;
				pos3.z = posObs.z + cosf(rotObs.y - fAngle) * fLength;
				pos2.x = posObs.x + sinf(rotObs.y + fAngle) * fLength;
				pos2.y = 0.0f;
				pos2.z = posObs.z + cosf(rotObs.y + fAngle) * fLength;

				// 障害物との判定
				bHit = collision::BoxOuterPillar
				( // 引数
					pos0,	// 四角の各頂点
					pos1,	// 四角の各頂点
					pos2,	// 四角の各頂点
					pos3,	// 四角の各頂点
					rPos	// 判定位置
				);

				if (bHit)
				{ // 当たっていた場合

					// 変数を宣言
					float fDis = rPos.y - (posObs.y + sizeObs.y);	// 影と障害物のY距離

					if (fDis >= 0.0f)
					{ // プレイヤーと障害物のY距離がプラスの場合

						if (!bInitDis)
						{ // 初期化していない場合

							// 現在の距離を代入
							fDisPosY = fDis;

							// 現在の影乗っかりオブジェクトを代入
							pCurrentObj = pObjCheck;

							// 初期化済みにする
							bInitDis = true;
						}
						else
						{ // 初期化している場合

							if (fDis < fDisPosY)
							{ // より近い障害物の場合

								// 現在の距離を代入
								fDisPosY = fDis;

								// 現在の影乗っかりオブジェクトを代入
								pCurrentObj = pObjCheck;
							}
						}
					}
				}

				// 次のオブジェクトへのポインタを代入
				pObjCheck = pObjectNext;
			}
		}
	}

	if (pCurrentObj != NULL)
	{ // 障害物があった場合

		// 変数を宣言
		D3DXVECTOR3 posObs = VEC3_ZERO;		// 障害物位置
		D3DXVECTOR3 sizeObs = VEC3_ZERO;	// 障害物大きさ
		
		// 障害物の位置を取得
		posObs = pCurrentObj->GetVec3Position();

		// 障害物の大きさを取得
		sizeObs = pCurrentObj->GetVec3Sizing();

		// 描画位置を設定
		rDrawPos = posObs.y + sizeObs.y;

		// 当たっている判定を返す
		return true;
	}
	else
	{ // 障害物がなかった場合

		// 当たっていない判定を返す
		return false;
	}
}

//============================================================
//	ビルとの当たり判定
//============================================================
bool CShadow::CollisionBuilding(D3DXVECTOR3& rPos, float& rDrawPos)
{
	// 変数を宣言
	float fDisPosY = 0.0f;	// プレイヤーとビルのY距離
	bool bInitDis = false;	// Y距離の初期化状況

	// ポインタを宣言
	CObject *pCurrentObj = NULL;	// 現在の影乗っかりオブジェクト

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
				D3DXVECTOR3 posBuild  = VEC3_ZERO;	// ビル位置
				D3DXVECTOR3 sizeBuild = VEC3_ZERO;	// ビル大きさ
				bool bHit = false;	// 判定状況

				// ポインタを宣言
				CObject *pObjectNext = pObjCheck->GetNext();	// 次オブジェクト

				if (pObjCheck->GetLabel() != CObject::LABEL_BUILDING)
				{ // オブジェクトラベルがビルではない場合

					// 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;

					// 次の繰り返しに移行
					continue;
				}

				// ビルの位置を取得
				posBuild = pObjCheck->GetVec3Position();

				// ビルの大きさを取得
				sizeBuild = pObjCheck->GetVec3Sizing();

				// ビルとの当たり判定
				bHit = collision::Box2D
				( // 引数
					rPos,		// 判定位置
					posBuild,	// 判定目標位置
					VEC3_ZERO,	// 判定サイズ(右・上・後)
					VEC3_ZERO,	// 判定サイズ(左・下・前)
					sizeBuild,	// 判定目標サイズ(右・上・後)
					sizeBuild	// 判定目標サイズ(左・下・前)
				);

				if (bHit)
				{ // 当たっていた場合

					// 変数を宣言
					float fDis = rPos.y - (posBuild.y + (sizeBuild.y * 2.0f));	// 影とビルのY距離

					if (fDis >= 0.0f)
					{ // プレイヤーとビルのY距離がプラスの場合

						if (!bInitDis)
						{ // 初期化していない場合

							// 現在の距離を代入
							fDisPosY = fDis;

							// 現在の影乗っかりオブジェクトを代入
							pCurrentObj = pObjCheck;

							// 初期化済みにする
							bInitDis = true;
						}
						else
						{ // 初期化している場合

							if (fDis < fDisPosY)
							{ // より近いビルの場合

								// 現在の距離を代入
								fDisPosY = fDis;

								// 現在の影乗っかりオブジェクトを代入
								pCurrentObj = pObjCheck;
							}
						}
					}
				}

				// 次のオブジェクトへのポインタを代入
				pObjCheck = pObjectNext;
			}
		}
	}

	if (pCurrentObj != NULL)
	{ // ビルがあった場合

		// 変数を宣言
		D3DXVECTOR3 posBuild = VEC3_ZERO;	// ビル位置
		D3DXVECTOR3 sizeBuild = VEC3_ZERO;	// ビル大きさ
		
		// ビルの位置を取得
		posBuild = pCurrentObj->GetVec3Position();

		// ビルの大きさを取得
		sizeBuild = pCurrentObj->GetVec3Sizing();

		// 描画位置を設定
		rDrawPos = posBuild.y + (sizeBuild.y * 2.0f);

		// 当たっている判定を返す
		return true;
	}
	else
	{ // ビルがなかった場合

		// 当たっていない判定を返す
		return false;
	}
}
