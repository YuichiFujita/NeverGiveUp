//============================================================
//
//	エディット障害物処理 [editObstacle.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "editObstacle.h"
#include "manager.h"
#include "input.h"
#include "collision.h"
#include "editStageManager.h"
#include "stage.h"

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
	const float	INIT_ALPHA = 0.5f;	// 配置前のα値
}

//************************************************************
//	親クラス [CEditObstacle] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEditObstacle::CEditObstacle()
{
#if _DEBUG

	// メンバ変数をクリア
	m_pEdit = NULL;	// エディットステージの情報
	memset(&m_obstacle, 0, sizeof(m_obstacle));	// 障害物配置情報

#endif	// _DEBUG
}

//============================================================
//	デストラクタ
//============================================================
CEditObstacle::~CEditObstacle()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	初期化処理
//============================================================
HRESULT CEditObstacle::Init(void)
{
#if _DEBUG

	// メンバ変数を初期化
	m_pEdit = NULL;	// エディットステージの情報

	m_obstacle.pObstacle = NULL;			// 障害物情報
	m_obstacle.type = CObstacle::TYPE_BOX;	// 障害物種類

	// 障害物の生成
	m_obstacle.pObstacle = CObstacle::Create(m_obstacle.type, VEC3_ZERO, VEC3_ZERO);
	if (m_obstacle.pObstacle == NULL)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 透明度を設定
	m_obstacle.pObstacle->SetAlpha(INIT_ALPHA);

	// 表示をOFFにする
	SetDisp(false);

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
void CEditObstacle::Uninit(void)
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	更新処理
//============================================================
void CEditObstacle::Update(void)
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

	// 障害物の生成
	CreateObstacle();

	// 障害物の破棄
	ReleaseObstacle();

	// 位置を反映
	m_obstacle.pObstacle->SetVec3Position(m_pEdit->GetVec3Position());

	// 向きを反映
	m_obstacle.pObstacle->SetVec3Rotation(m_pEdit->GetVec3Rotation());

#endif	// _DEBUG
}

//============================================================
//	表示の設定処理
//============================================================
void CEditObstacle::SetDisp(const bool bDisp)
{
	// 自動更新・自動描画を表示状況に合わせる
	m_obstacle.pObstacle->SetEnableUpdate(bDisp);	// 更新
	m_obstacle.pObstacle->SetEnableDraw(bDisp);		// 描画

	if (bDisp)
	{ // 表示ONの場合

		// 位置を反映
		m_obstacle.pObstacle->SetVec3Position(m_pEdit->GetVec3Position());
	}
	else
	{ // 表示OFFの場合

		// 障害物の色の全初期化
		InitAllColorObstacle();

		// 位置をステージの範囲外に設定
		D3DXVECTOR3 sizeObs = m_obstacle.pObstacle->GetVec3Sizing();
		D3DXVECTOR3 outLimit = D3DXVECTOR3(0.0f, 0.0f, CScene::GetStage()->GetStageLimit().fNear - ((sizeObs.z < sizeObs.x) ? sizeObs.x : sizeObs.z) * 0.5f);
		m_obstacle.pObstacle->SetVec3Position(outLimit);
	}
}

//============================================================
//	操作表示の描画処理
//============================================================
void CEditObstacle::DrawDebugControl(void)
{
	// ポインタを宣言
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();	// デバッグプロックの情報

	pDebug->Print(CDebugProc::POINT_RIGHT, "種類変更：[%s]\n", NAME_TYPE);
	pDebug->Print(CDebugProc::POINT_RIGHT, "削除：[%s]\n", NAME_RELEASE);
	pDebug->Print(CDebugProc::POINT_RIGHT, "設置：[%s]\n", NAME_CREATE);
}

//============================================================
//	情報表示の描画処理
//============================================================
void CEditObstacle::DrawDebugInfo(void)
{
	// ポインタを宣言
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();	// デバッグプロックの情報

	pDebug->Print(CDebugProc::POINT_RIGHT, "%d：[種類]\n", m_obstacle.type);
}

//============================================================
//	保存処理
//============================================================
void CEditObstacle::Save(FILE *pFile)
{
#if _DEBUG

	if (pFile != NULL)
	{ // ファイルが存在する場合

		// 見出しを書き出し
		fprintf(pFile, "#------------------------------------------------------------------------------\n");
		fprintf(pFile, "#	障害物の配置情報\n");
		fprintf(pFile, "#------------------------------------------------------------------------------\n");

		// 情報開始地点を書き出し
		fprintf(pFile, "STAGE_OBSTACLESET\n\n");

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
		
					// ポインタを宣言
					CObject *pObjectNext = pObjCheck->GetNext();	// 次オブジェクト
	
					if (pObjCheck->GetLabel() != CObject::LABEL_OBSTACLE)
					{ // オブジェクトラベルが障害物ではない場合
	
						// 次のオブジェクトへのポインタを代入
						pObjCheck = pObjectNext;
	
						// 次の繰り返しに移行
						continue;
					}
	
					if (pObjCheck == (CObject*)m_obstacle.pObstacle)
					{ // 同じアドレスだった場合
	
						// 次のオブジェクトへのポインタを代入
						pObjCheck = pObjectNext;
	
						// 次の繰り返しに移行
						continue;
					}

					// 障害物の情報を取得
					D3DXVECTOR3 posBuild = pObjCheck->GetVec3Position();	// 位置
					D3DXVECTOR3 rotBuild = pObjCheck->GetVec3Rotation();	// 向き
					int nType = pObjCheck->GetType();	// 種類
	
					// 情報を書き出し
					fprintf(pFile, "	OBSTACLESET\n");
					fprintf(pFile, "		TYPE = %d\n", nType);
					fprintf(pFile, "		POS = %.2f %.2f %.2f\n", posBuild.x, posBuild.y, posBuild.z);
					fprintf(pFile, "		ROT = %.2f %.2f %.2f\n", rotBuild.x, rotBuild.y, rotBuild.z);
					fprintf(pFile, "	END_OBSTACLESE\n\n");

					// 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;
				}
			}
		}

		// 情報終了地点を書き出し
		fprintf(pFile, "END_STAGE_OBSTACLESET\n\n");
	}

#endif	// _DEBUG
}

//============================================================
//	生成処理
//============================================================
CEditObstacle *CEditObstacle::Create(CEditStageManager *pEdit)
{
#if _DEBUG

	// ポインタを宣言
	CEditObstacle *pEditObstacle = NULL;	// エディット障害物生成用

	if (pEditObstacle == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pEditObstacle = new CEditObstacle;	// エディット障害物
	}
	else { assert(false); return NULL; }	// 使用中

	if (pEditObstacle != NULL)
	{ // 使用されている場合
		
		// エディット障害物の初期化
		if (FAILED(pEditObstacle->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pEditObstacle;
			pEditObstacle = NULL;

			// 失敗を返す
			return NULL;
		}

		// エディットステージの情報を設定
		pEditObstacle->m_pEdit = pEdit;

		// 確保したアドレスを返す
		return pEditObstacle;
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
HRESULT CEditObstacle::Release(CEditObstacle *&prEditObstacle)
{
#if _DEBUG

	if (prEditObstacle != NULL)
	{ // 使用中の場合

		// エディット障害物の終了
		prEditObstacle->Uninit();

		// メモリ開放
		delete prEditObstacle;
		prEditObstacle = NULL;

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
void CEditObstacle::UpdateChangeType(void)
{
	// ポインタを宣言
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// キーボード情報

	// 種類を変更
	if (m_pKeyboard->IsTrigger(KEY_TYPE))
	{
		// 種類を変更
		m_obstacle.type = (CObstacle::EType)((m_obstacle.type + 1) % CObstacle::TYPE_MAX);

		// 種類を反映
		m_obstacle.pObstacle->SetType(m_obstacle.type);
		m_obstacle.pObstacle->SetAlpha(INIT_ALPHA);	// 透明度を再設定
	}
}

//============================================================
//	障害物の生成処理
//============================================================
void CEditObstacle::CreateObstacle(void)
{
	// 変数を宣言
	D3DXVECTOR3 posEdit = m_pEdit->GetVec3Position();	// エディットの位置
	D3DXVECTOR3 rotEdit = m_pEdit->GetVec3Rotation();	// エディットの向き

	// ポインタを宣言
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// キーボード情報

	// 障害物を配置
	if (m_pKeyboard->IsTrigger(KEY_CREATE))
	{
		//----------------------------------------------------
		//	障害物の情報を配置用に変更
		//----------------------------------------------------
		// 自動更新・自動描画をONにする
		m_obstacle.pObstacle->SetEnableUpdate(true);
		m_obstacle.pObstacle->SetEnableDraw(true);

		// マテリアルを再設定
		m_obstacle.pObstacle->ResetMaterial();

		// 未保存を設定
		m_pEdit->UnSave();

		//----------------------------------------------------
		//	新しい障害物の生成
		//----------------------------------------------------
		// 障害物の生成
		m_obstacle.pObstacle = CObstacle::Create(m_obstacle.type, posEdit, rotEdit);
		assert(m_obstacle.pObstacle != NULL);

		// 透明度を設定
		m_obstacle.pObstacle->SetAlpha(INIT_ALPHA);
	}
}

//============================================================
//	障害物の破棄処理
//============================================================
void CEditObstacle::ReleaseObstacle(void)
{
	// 変数を宣言
	bool bRelease = false;	// 破棄状況

	// ポインタを宣言
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// キーボード情報

	// 障害物を削除
	if (m_pKeyboard->IsTrigger(KEY_RELEASE))
	{
		// 破棄する状態を設定
		bRelease = true;
	}

	// 障害物の削除判定
	DeleteCollisionObstacle(bRelease);
}

//============================================================
//	障害物の削除判定
//============================================================
void CEditObstacle::DeleteCollisionObstacle(const bool bRelase)
{
	// 変数を宣言
	D3DXVECTOR3 posEdit = m_pEdit->GetVec3Position();				// エディットの位置
	D3DXVECTOR3 sizeEdit = m_obstacle.pObstacle->GetVec3Sizing();	// エディット障害物の大きさ

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
				D3DXVECTOR3 posBuild = VEC3_ZERO;	// 障害物位置
				D3DXVECTOR3 sizeBuild = VEC3_ZERO;	// 障害物大きさ

				// ポインタを宣言
				CObject *pObjectNext = pObjCheck->GetNext();	// 次オブジェクト

				if (pObjCheck->GetLabel() != CObject::LABEL_OBSTACLE)
				{ // オブジェクトラベルが障害物ではない場合

					// 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;

					// 次の繰り返しに移行
					continue;
				}

				if (pObjCheck == (CObject*)m_obstacle.pObstacle)
				{ // 同じアドレスだった場合

					// 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;

					// 次の繰り返しに移行
					continue;
				}

				// 障害物の位置を取得
				posBuild = pObjCheck->GetVec3Position();

				// 障害物の大きさを取得
				sizeBuild = pObjCheck->GetVec3Sizing();

				// 球体の当たり判定
				if (collision::Circle3D
				( // 引数
					posEdit,	// 判定位置
					posBuild,	// 判定目標位置
					((sizeBuild.x + sizeBuild.z) * 0.5f) * 0.5f,	// 判定半径
					((sizeEdit.x + sizeEdit.z) * 0.5f) * 0.5f		// 判定目標半径
				))
				{ // 判定内だった場合

					if (bRelase)
					{ // 破棄する場合

						// 終了処理
						pObjCheck->Uninit();

						// 未保存を設定
						m_pEdit->UnSave();
					}
					else
					{ // 破棄しない場合

						// 赤マテリアルを全設定
						pObjCheck->SetAllMaterial(material::Red());
					}
				}
				else
				{ // 判定外だった場合

					// マテリアルを再設定
					pObjCheck->ResetMaterial();
				}

				// 次のオブジェクトへのポインタを代入
				pObjCheck = pObjectNext;
			}
		}
	}
}

//============================================================
//	障害物の色の全初期化処理
//============================================================
void CEditObstacle::InitAllColorObstacle(void)
{
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

				// ポインタを宣言
				CObject *pObjectNext = pObjCheck->GetNext();	// 次オブジェクト

				if (pObjCheck->GetLabel() != CObject::LABEL_OBSTACLE)
				{ // オブジェクトラベルが障害物ではない場合

					// 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;

					// 次の繰り返しに移行
					continue;
				}

				if (pObjCheck == (CObject*)m_obstacle.pObstacle)
				{ // 同じアドレスだった場合

					// 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;

					// 次の繰り返しに移行
					continue;
				}

				// マテリアルを再設定
				pObjCheck->ResetMaterial();

				// 次のオブジェクトへのポインタを代入
				pObjCheck = pObjectNext;
			}
		}
	}
}
