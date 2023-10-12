//============================================================
//
//	障害物ヘッダー [obstacle.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "objectModel.h"

//************************************************************
//	クラス定義
//************************************************************
// 障害物クラス
class CObstacle : public CObjectModel
{
public:
	// 種類列挙
	enum EType
	{
		TYPE_NORMAL = 0,	// 通常モデル
		TYPE_MAX			// この列挙型の総数
	};

	// 回避列挙
	enum EDodge
	{
		DODGE_NONE,		// 回避方法無し
		DODGE_JUMP,		// ジャンプ回避
		DODGE_SLIDE,	// スライディング回避
		DODGE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CObstacle();

	// デストラクタ
	~CObstacle();

	// モデル情報構造体
	struct SModelInfo
	{
		const char *pTextureFile;	// モデルファイル名
		EDodge dodge;	// 回避法
	};

	// オーバーライド関数
	HRESULT Init(void) override;			// 初期化
	void Uninit(void) override;				// 終了
	void Update(void) override;				// 更新
	void Draw(void) override;				// 描画
	void SetType(const int nType) override;	// 種類設定

	// 静的メンバ関数
	static CObstacle *Create	// 生成
	( // 引数
		const EType type,			// 種類
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot		// 向き
	);

private:
	// メンバ変数
	EDodge m_dodge;	// 回避法

	// 静的メンバ変数
	static SModelInfo m_aStatusInfo[];	// モデル情報
};

#endif	// _OBSTACLE_H_
