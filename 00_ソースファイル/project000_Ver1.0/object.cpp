//============================================================
//
//	オブジェクト処理 [object.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"
#include "manager.h"

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CObject *CObject::m_apTop[MAX_PRIO] = {};	// 先頭のオブジェクトへのポインタ
CObject *CObject::m_apCur[MAX_PRIO] = {};	// 最後尾のオブジェクトへのポインタ
DWORD CObject::m_dwNextID = 0;				// 次のユニークID
int CObject::m_nNumAll = 0;					// オブジェクトの総数

//************************************************************
//	親クラス [CObject] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CObject::CObject()
{
	if (m_apCur[DEFAULT_PRIO] != NULL)
	{ // 最後尾が存在する場合

		// 現在の最後尾オブジェクトの次オブジェクトを自身に設定
		m_apCur[DEFAULT_PRIO]->m_pNext = this;

		// 前オブジェクトを設定
		m_pPrev = m_apCur[DEFAULT_PRIO];	// 現在の最後尾オブジェクト

		// 次オブジェクトをクリア
		m_pNext = NULL;

		// 自身の情報アドレスを最後尾に設定
		m_apCur[DEFAULT_PRIO] = this;
	}
	else
	{ // 最後尾が存在しない場合

		// 自身の情報アドレスを先頭に設定
		m_apTop[DEFAULT_PRIO] = this;

		// 自身の情報アドレスを最後尾に設定
		m_apCur[DEFAULT_PRIO] = this;

		// 前オブジェクトのクリア
		m_pPrev = NULL;

		// 次オブジェクトのクリア
		m_pNext = NULL;
	}

	// 自身の情報をクリア
	m_label		= LABEL_NONE;	// オブジェクトラベル
	m_nPriority	= DEFAULT_PRIO;	// 優先順位
	m_dwID		= m_dwNextID;	// 自身のユニークID
	m_bUpdate	= true;			// 自身の更新状況
	m_bDraw		= true;			// 自身の描画状況
	m_bDeath	= false;		// 自身の死亡フラグ

	// ユニークIDを加算
	m_dwNextID++;

	// オブジェクトの総数を加算
	m_nNumAll++;
}

//============================================================
//	オーバーロードコンストラクタ
//============================================================
CObject::CObject(const ELabel label, const int nPriority)
{
	if (m_apCur[nPriority] != NULL)
	{ // 最後尾が存在する場合

		// 現在の最後尾オブジェクトの次オブジェクトを自身に設定
		m_apCur[nPriority]->m_pNext = this;

		// 前オブジェクトを設定
		m_pPrev = m_apCur[nPriority];	// 現在の最後尾オブジェクト

		// 次オブジェクトをクリア
		m_pNext = NULL;

		// 自身の情報アドレスを最後尾に設定
		m_apCur[nPriority] = this;
	}
	else
	{ // 最後尾が存在しない場合

		// 自身の情報アドレスを先頭に設定
		m_apTop[nPriority] = this;

		// 自身の情報アドレスを最後尾に設定
		m_apCur[nPriority] = this;

		// 前オブジェクトのクリア
		m_pPrev = NULL;

		// 次オブジェクトのクリア
		m_pNext = NULL;
	}

	// 自身の情報を設定
	m_label		= label;		// オブジェクトラベル
	m_nPriority	= nPriority;	// 優先順位
	m_dwID		= m_dwNextID;	// 自身のユニークID
	m_bUpdate	= true;			// 自身の更新状況
	m_bDraw		= true;			// 自身の描画状況
	m_bDeath	= false;		// 自身の死亡フラグ

	// ユニークIDを加算
	m_dwNextID++;

	// オブジェクトの総数を加算
	m_nNumAll++;
}

//============================================================
//	デストラクタ
//============================================================
CObject::~CObject()
{
	// オブジェクトの総数を減算
	m_nNumAll--;
}

//============================================================
//	ヒット処理
//============================================================
void CObject::Hit(void)
{
	// 例外処理
	assert(false);
}

//============================================================
//	ダメージヒット処理
//============================================================
void CObject::Hit(const int /*nDmg*/)
{
	// 例外処理
	assert(false);
}

//============================================================
//	ノックバックヒット処理
//============================================================
void CObject::HitKnockBack(const int /*nDmg*/, const D3DXVECTOR3& /*vec*/)
{
	// 例外処理
	assert(false);
}

//============================================================
//	テクスチャ割当処理 (インデックス)
//============================================================
void CObject::BindTexture(const int /*nTextureID*/)
{
	// 例外処理
	assert(false);
}

//============================================================
//	テクスチャ割当処理 (パス)
//============================================================
void CObject::BindTexture(const char * /*pTexturePass*/)
{
	// 例外処理
	assert(false);
}

//============================================================
//	モデル割当処理 (インデックス)
//============================================================
void CObject::BindModel(const int /*nModelID*/)
{
	// 例外処理
	assert(false);
}

//============================================================
//	モデル割当処理 (パス)
//============================================================
void CObject::BindModel(const char * /*pModelPass*/)
{
	// 例外処理
	assert(false);
}

//============================================================
//	状態の設定処理
//============================================================
void CObject::SetState(const int /*nState*/)
{
	// 例外処理
	assert(false);
}

//============================================================
//	状態取得処理
//============================================================
int CObject::GetState(void) const
{
	// 例外処理
	assert(false);

	// NONE_IDXを返す
	return NONE_IDX;
}

//============================================================
//	種類の設定処理
//============================================================
void CObject::SetType(const int /*nType*/)
{
	// 例外処理
	assert(false);
}

//============================================================
//	種類取得処理
//============================================================
int CObject::GetType(void) const
{
	// 例外処理
	assert(false);

	// NONE_IDXを返す
	return NONE_IDX;
}

//============================================================
//	角度設定の設定処理
//============================================================
void CObject::SetAngle(const float /*fAngle*/)
{
	// 例外処理
	assert(false);
}

//============================================================
//	角度取得処理
//============================================================
float CObject::GetAngle(void) const
{
	// 例外処理
	assert(false);

	// 初期値を返す
	return 0.0f;
}

//============================================================
//	長さ設定の設定処理
//============================================================
void CObject::SetLength(const float /*fLength*/)
{
	// 例外処理
	assert(false);
}

//============================================================
//	長さ取得処理
//============================================================
float CObject::GetLength(void) const
{
	// 例外処理
	assert(false);

	// 初期値を返す
	return 0.0f;
}

//============================================================
//	半径の設定処理
//============================================================
void CObject::SetRadius(const float /*fRadius*/)
{
	// 例外処理
	assert(false);
}

//============================================================
//	半径取得処理
//============================================================
float CObject::GetRadius(void) const
{
	// 例外処理
	assert(false);

	// 初期値を返す
	return 0.0f;
}

//============================================================
//	縦幅の設定処理
//============================================================
void CObject::SetHeight(const float /*fHeight*/)
{
	// 例外処理
	assert(false);
}

//============================================================
//	縦幅取得処理
//============================================================
float CObject::GetHeight(void) const
{
	// 例外処理
	assert(false);

	// 初期値を返す
	return 0.0f;
}

//============================================================
//	拡大率の設定処理
//============================================================
void CObject::SetScale(const float /*fScale*/)
{
	// 例外処理
	assert(false);
}

//============================================================
//	拡大率取得処理
//============================================================
float CObject::GetScale(void) const
{
	// 例外処理
	assert(false);

	// 初期値を返す
	return 0.0f;
}

//============================================================
//	二軸の位置の設定処理
//============================================================
void CObject::SetVec2Position(const D3DXVECTOR2& /*rPos*/)
{
	// 例外処理
	assert(false);
}

//============================================================
//	二軸の位置取得処理
//============================================================
D3DXVECTOR2 CObject::GetVec2Position(void) const
{
	// 例外処理
	assert(false);

	// 初期値を返す
	return VEC2_ZERO;
}

//============================================================
//	三軸の位置の設定処理
//============================================================
void CObject::SetVec3Position(const D3DXVECTOR3& /*rPos*/)
{
	// 例外処理
	assert(false);
}

//============================================================
//	三軸の位置取得処理
//============================================================
D3DXVECTOR3 CObject::GetVec3Position(void) const
{
	// 例外処理
	assert(false);

	// 初期値を返す
	return VEC3_ZERO;
}

//============================================================
//	向きの設定処理
//============================================================
void CObject::SetVec3Rotation(const D3DXVECTOR3& /*rRot*/)
{
	// 例外処理
	assert(false);
}

//============================================================
//	向き取得処理
//============================================================
D3DXVECTOR3 CObject::GetVec3Rotation(void) const
{
	// 例外処理
	assert(false);

	// 初期値を返す
	return VEC3_ZERO;
}

//============================================================
//	二軸の大きさの設定処理
//============================================================
void CObject::SetVec2Sizing(const D3DXVECTOR2& /*rSize*/)
{
	// 例外処理
	assert(false);
}

//============================================================
//	二軸の大きさ取得処理
//============================================================
D3DXVECTOR2 CObject::GetVec2Sizing(void) const
{
	// 例外処理
	assert(false);

	// 初期値を返す
	return VEC2_ZERO;
}

//============================================================
//	三軸の大きさの設定処理
//============================================================
void CObject::SetVec3Sizing(const D3DXVECTOR3& /*rSize*/)
{
	// 例外処理
	assert(false);
}

//============================================================
//	三軸の大きさ取得処理
//============================================================
D3DXVECTOR3 CObject::GetVec3Sizing(void) const
{
	// 例外処理
	assert(false);

	// 初期値を返す
	return VEC3_ZERO;
}

//============================================================
//	拡大率の設定処理
//============================================================
void CObject::SetVec3Scaling(const D3DXVECTOR3& /*rScale*/)
{
	// 例外処理
	assert(false);
}

//============================================================
//	拡大率取得処理
//============================================================
D3DXVECTOR3 CObject::GetVec3Scaling(void) const
{
	// 例外処理
	assert(false);

	// 初期値を返す
	return VEC3_ZERO;
}

//============================================================
//	色の設定処理
//============================================================
void CObject::SetColor(const D3DXCOLOR& /*rCol*/)
{
	// 例外処理
	assert(false);
}

//============================================================
//	色取得処理
//============================================================
D3DXCOLOR CObject::GetColor(void) const
{
	// 例外処理
	assert(false);

	// 初期値を返す
	return XCOL_WHITE;
}

//============================================================
//	マテリアルの全設定処理
//============================================================
void CObject::SetAllMaterial(const D3DXMATERIAL& /*rMat*/)
{
	// 例外処理
	assert(false);
}

//============================================================
//	マテリアルの再設定処理
//============================================================
void CObject::ResetMaterial(void)
{
	// 例外処理
	assert(false);
}

//============================================================
//	優先順位の設定処理
//============================================================
void CObject::SetPriority(const int nPriority)
{
	//--------------------------------------------------------
	//	リストの再接続
	//--------------------------------------------------------
	// 前のオブジェクトをつなぎなおす
	if (m_pNext != NULL)
	{ // 次のオブジェクトが存在する場合

		// 前のオブジェクトを変更
		m_pNext->m_pPrev = m_pPrev;
	}

	// 次のオブジェクトをつなぎなおす
	if (m_pPrev != NULL)
	{ // 前のオブジェクトが存在する場合

		// 次のオブジェクトを変更
		m_pPrev->m_pNext = m_pNext;
	}

	// 先頭オブジェクトの変更
	if (m_apTop[m_nPriority] == this)
	{ // 先頭オブジェクトが破棄するオブジェクトだった場合

		// 次のオブジェクトを先頭に指定
		m_apTop[m_nPriority] = m_pNext;
	}

	// 最後尾オブジェクトの変更
	if (m_apCur[m_nPriority] == this)
	{ // 最後尾オブジェクトが破棄するオブジェクトだった場合

		// 前のオブジェクトを最後尾に指定
		m_apCur[m_nPriority] = m_pPrev;
	}

	//--------------------------------------------------------
	//	優先順位の設定・リストへの追加
	//--------------------------------------------------------
	// 引数の優先順位を設定
	m_nPriority = nPriority;

	// 自身のオブジェクトを引数の優先順位リストに変更
	if (m_apCur[nPriority] != NULL)
	{ // 最後尾が存在する場合

		// 現在の最後尾オブジェクトの次オブジェクトを自身に設定
		m_apCur[nPriority]->m_pNext = this;

		// 前オブジェクトを設定
		m_pPrev = m_apCur[nPriority];	// 現在の最後尾オブジェクト

		// 次オブジェクトをクリア
		m_pNext = NULL;

		// 自身の情報アドレスを最後尾に設定
		m_apCur[nPriority] = this;
	}
	else
	{ // 最後尾が存在しない場合

		// 自身の情報アドレスを先頭に設定
		m_apTop[nPriority] = this;

		// 自身の情報アドレスを最後尾に設定
		m_apCur[nPriority] = this;

		// 前オブジェクトのクリア
		m_pPrev = NULL;

		// 次オブジェクトのクリア
		m_pNext = NULL;
	}
}

//============================================================
//	更新状況の設定処理
//============================================================
void CObject::SetEnableUpdate(const bool bUpdate)
{
	// 引数の更新状況を設定
	m_bUpdate = bUpdate;
}

//============================================================
//	描画状況の設定処理
//============================================================
void CObject::SetEnableDraw(const bool bDraw)
{
	// 引数の描画状況を設定
	m_bDraw = bDraw;
}

//============================================================
//	マトリックスポインタ取得処理
//============================================================
D3DXMATRIX *CObject::GetPtrMtxWorld(void)
{
	// 例外処理
	assert(false);

	// NULLを返す
	return NULL;
}

//============================================================
//	マトリックス取得処理
//============================================================
D3DXMATRIX CObject::GetMtxWorld(void) const
{
	// 変数を宣言
	D3DXMATRIX mtxNone;	// 返り値用

	// マトリックスの初期化
	D3DXMatrixIdentity(&mtxNone);

	// 例外処理
	assert(false);

	// 単位マトリックスを返す
	return mtxNone;
}

//============================================================
//	全破棄処理
//============================================================
void CObject::ReleaseAll(void)
{
	// ポインタを宣言
	CObject *pObject;	// オブジェクト代入用

	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // 優先順位の総数分繰り返す

		if (m_apTop[nCntPri] != NULL)
		{ // 先頭が存在する場合

			// オブジェクトの先頭を代入
			pObject = m_apTop[nCntPri];

			while (pObject != NULL)
			{ // オブジェクトが使用されている場合繰り返す

				// ポインタを宣言
				CObject *pObjectNext = pObject->m_pNext;	// 次のオブジェクトへのポインタ

				if (pObject->m_label != LABEL_NONE)
				{ // オブジェクトラベルが設定されている場合

					if (!pObject->m_bDeath)
					{ // 死亡していない場合

						// オブジェクトの終了
						pObject->Uninit();
					}
				}

				// 次のオブジェクトへのポインタを代入
				pObject = pObjectNext;
			}
		}
	}

	// 全死亡処理
	DeathAll();
}

//============================================================
//	全更新処理
//============================================================
void CObject::UpdateAll(void)
{
	// ポインタを宣言
	CObject *pObject;	// オブジェクト代入用

	// デバッグ表示
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[オブジェクト数]：%d\n", m_nNumAll);

	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // 優先順位の総数分繰り返す

		if (m_apTop[nCntPri] != NULL)
		{ // 先頭が存在する場合

			// オブジェクトの先頭を代入
			pObject = m_apTop[nCntPri];

			while (pObject != NULL)
			{ // オブジェクトが使用されている場合繰り返す

				// ポインタを宣言
				CObject *pObjectNext = pObject->m_pNext;	// 次のオブジェクトへのポインタ

				if (pObject->m_label != LABEL_NONE)
				{ // オブジェクトラベルが設定されている場合

					if (pObject->m_bUpdate)
					{ // 更新する場合

						if (!pObject->m_bDeath)
						{ // 死亡していない場合

							// オブジェクトの更新
							pObject->Update();
						}
					}
				}

				// 次のオブジェクトへのポインタを代入
				pObject = pObjectNext;
			}
		}
	}

	// 全死亡処理
	DeathAll();
}

//============================================================
//	全描画処理
//============================================================
void CObject::DrawAll(void)
{
	// ポインタを宣言
	CObject *pObject;	// オブジェクト代入用

	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // 優先順位の総数分繰り返す

		if (m_apTop[nCntPri] != NULL)
		{ // 先頭が存在する場合

			// オブジェクトの先頭を代入
			pObject = m_apTop[nCntPri];

			while (pObject != NULL)
			{ // オブジェクトが使用されている場合繰り返す

				// ポインタを宣言
				CObject *pObjectNext = pObject->m_pNext;	// 次のオブジェクトへのポインタ

				if (pObject->m_bDraw)
				{ // 描画する場合

					if (!pObject->m_bDeath)
					{ // 死亡していない場合

						// オブジェクトの描画
						pObject->Draw();
					}
				}

				// 次のオブジェクトへのポインタを代入
				pObject = pObjectNext;
			}
		}
	}
}

//============================================================
//	全死亡処理
//============================================================
void CObject::DeathAll(void)
{
	// ポインタを宣言
	CObject *pObject;	// オブジェクト代入用

	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // 優先順位の総数分繰り返す

		if (m_apTop[nCntPri] != NULL)
		{ // 先頭が存在する場合

			// オブジェクトの先頭を代入
			pObject = m_apTop[nCntPri];

			while (pObject != NULL)
			{ // オブジェクトが使用されている場合繰り返す

				// ポインタを宣言
				CObject *pObjectNext = pObject->m_pNext;	// 次のオブジェクトへのポインタ

				if (pObject->m_bDeath)
				{ // 死亡フラグが立っている場合

					if (pObject != NULL)
					{ // 使用されている場合

						// 前のオブジェクトをつなぎなおす
						if (pObject->m_pNext != NULL)
						{ // 次のオブジェクトが存在する場合

							// 前のオブジェクトを変更
							pObject->m_pNext->m_pPrev = pObject->m_pPrev;
						}

						// 次のオブジェクトをつなぎなおす
						if (pObject->m_pPrev != NULL)
						{ // 前のオブジェクトが存在する場合

							// 次のオブジェクトを変更
							pObject->m_pPrev->m_pNext = pObject->m_pNext;
						}

						// 先頭オブジェクトの変更
						if (m_apTop[pObject->m_nPriority] == pObject)
						{ // 先頭オブジェクトが破棄するオブジェクトだった場合

							// 次のオブジェクトを先頭に指定
							m_apTop[pObject->m_nPriority] = pObject->m_pNext;
						}

						// 最後尾オブジェクトの変更
						if (m_apCur[pObject->m_nPriority] == pObject)
						{ // 最後尾オブジェクトが破棄するオブジェクトだった場合

							// 前のオブジェクトを最後尾に指定
							m_apCur[pObject->m_nPriority] = pObject->m_pPrev;
						}

						// メモリ開放
						delete pObject;
					}
				}

				// 次のオブジェクトへのポインタを代入
				pObject = pObjectNext;
			}
		}
	}
}

//============================================================
//	総数取得処理
//============================================================
int CObject::GetNumAll(void)
{
	// 現在のオブジェクトの総数を返す
	return m_nNumAll;
}

//============================================================
//	先頭オブジェクト取得処理
//============================================================
CObject *CObject::GetTop(const int nPriority)
{
	// 引数の優先順位の先頭オブジェクトを返す
	return m_apTop[nPriority];
}

//============================================================
//	最後尾オブジェクト取得処理
//============================================================
CObject *CObject::GetCur(const int nPriority)
{
	// 引数の優先順位の最後尾オブジェクトを返す
	return m_apCur[nPriority];
}

//============================================================
//	ラベルの設定処理
//============================================================
void CObject::SetLabel(const ELabel label)
{
	if (label < LABEL_MAX)
	{ // 引数のラベルが使用可能な場合

		// 引数のラベルを設定
		m_label = label;
	}
	else { assert(false); }	// 範囲外
}

//============================================================
//	ラベルの取得処理
//============================================================
CObject::ELabel CObject::GetLabel(void) const
{
	// ラベルを返す
	return m_label;
}

//============================================================
//	優先順位の取得処理
//============================================================
int CObject::GetPriority(void) const
{
	// 優先順位を返す
	return m_nPriority;
}

//============================================================
//	ユニークIDの取得処理
//============================================================
DWORD CObject::GetID(void) const
{
	// ユニークIDを返す
	return m_dwID;
}

//============================================================
//	更新状況の取得処理
//============================================================
bool CObject::IsUpdate(void) const
{
	// 更新状況を返す
	return m_bUpdate;
}

//============================================================
//	描画状況の取得処理
//============================================================
bool CObject::IsDraw(void) const
{
	// 描画状況を返す
	return m_bDraw;
}

//============================================================
//	死亡フラグ取得処理
//============================================================
bool CObject::IsDeath(void) const
{
	// 死亡フラグを返す
	return m_bDeath;
}

//============================================================
//	オブジェクト取得処理
//============================================================
CObject *CObject::GetObject(void)
{
	// 自身のポインタを返す
	return this;
}

//============================================================
//	前オブジェクト取得処理
//============================================================
CObject *CObject::GetPrev(void) const
{
	// 前オブジェクトを返す
	return m_pPrev;
}

//============================================================
//	次オブジェクト取得処理
//============================================================
CObject *CObject::GetNext(void) const
{
	// 次オブジェクトを返す
	return m_pNext;
}

//============================================================
//	破棄処理
//============================================================
void CObject::Release(void)
{
	if (this != NULL)
	{ // 使用されている場合

		if (!m_bDeath)
		{ // 死亡フラグが立っていない場合

			// 死亡フラグを立てる
			m_bDeath = true;
		}
		else { assert(false); }	// 死亡済み
	}
}
