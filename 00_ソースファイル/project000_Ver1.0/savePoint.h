//============================================================
//
//	セーブポイントヘッダー [savePoint.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _SAVEPOINT_H_
#define _SAVEPOINT_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "objectModel.h"

//************************************************************
//	クラス定義
//************************************************************
// セーブポイントクラス
class CSavePoint : public CObjectModel
{
public:
	// コンストラクタ
	CSavePoint();

	// デストラクタ
	~CSavePoint();

	// セーブポイント情報構造体
	struct SInfo
	{
		D3DXVECTOR3 pos;	// 位置
		D3DXVECTOR3 rot;	// 向き
	};

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画

	// 静的メンバ関数
	static CSavePoint *Create	// 生成
	( // 引数
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot		// 向き
	);
	static SInfo GetSavePointInfo(void);	// セーブポイント情報取得

	// メンバ関数
	void CollisionPlayer(void);	// プレイヤーとの当たり判定

private:
	// 静的メンバ変数
	static const char *mc_apModelFile[];	// モデル定数
	static CSavePoint *m_pCurrentSave;		// 現在のセーブポイントへのポインタ

	static int m_nNumAll;	// セーブポイントの総数

	// メンバ変数
	const int m_nThisSaveID;	// 自身のセーブポイントインデックス
};

#endif	// _SAVEPOINT_H_
