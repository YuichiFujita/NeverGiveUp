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
		TYPE_BOX = 0,	// 箱モデル
		TYPE_BENCH,		// ベンチモデル
		TYPE_MAX		// この列挙型の総数
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
	CObstacle(const EType type);

	// デストラクタ
	~CObstacle();

	// ステータス構造体
	struct SStatusInfo
	{
		D3DXVECTOR3 size;	// 大きさ
		EDodge dodge;		// 回避法
	};

	// モデル情報構造体
	struct SModelInfo
	{
		const char *pTextureFile;	// モデルファイル名
		SStatusInfo status;			// ステータス情報
	};

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画
	int GetType(void) const override;		// 種類取得
	float GetAngle(void) const override;	// 角度取得
	float GetLength(void) const override;	// 長さ取得
	D3DXVECTOR3 GetVec3Sizing(void) const override;	// 大きさ取得

	// 静的メンバ関数
	static CObstacle *Create	// 生成
	( // 引数
		const EType type,			// 種類
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot		// 向き
	);

	// メンバ関数
	void SetType(const EType type);	// 種類設定

private:
	// 静的メンバ変数
	static SModelInfo m_aStatusInfo[];	// モデル情報

	// メンバ変数
	const SStatusInfo m_status;	// ステータス定数
	const EType m_type;	// 種類定数
	float m_fAngle;		// 対角線の角度
	float m_fLength;	// 対角線の長さ
};

#endif	// _OBSTACLE_H_
