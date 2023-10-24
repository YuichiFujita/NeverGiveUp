//============================================================
//
//	ゴールポイントヘッダー [goalPoint.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _GOALPOINT_H_
#define _GOALPOINT_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	クラス定義
//************************************************************
// ゴールポイントクラス
class CGoalPoint : public CObject
{
public:
	// コンストラクタ
	CGoalPoint();

	// デストラクタ
	~CGoalPoint();

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// 位置設定
	D3DXVECTOR3 GetVec3Position(void) const override;		// 位置取得
	void SetVec3Sizing(const D3DXVECTOR3& rSize) override;	// 判定大きさ設定
	D3DXVECTOR3 GetVec3Sizing(void) const override;			// 判定大きさ取得

	// 静的メンバ関数
	static CGoalPoint *Create(const D3DXVECTOR3& rPos);	// 生成

private:
	// メンバ関数
	void CollisionPlayer(void);	// プレイヤーとの当たり判定

	// メンバ変数
	D3DXVECTOR3 m_pos;	// 位置
	D3DXVECTOR3 m_size;	// 大きさ
};

#endif	// _GOALPOINT_H_
