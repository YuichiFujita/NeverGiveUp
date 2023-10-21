//============================================================
//
//	エディットステージマネージャーヘッダー [editStageManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _EDIT_STAGEMANAGER_H_
#define _EDIT_STAGEMANAGER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "building.h"

//************************************************************
//	前方宣言
//************************************************************
class CBuilding;	// ビルクラス
class CSignboard;	// 看板クラス
class CObstacle;	// 障害物クラス
class CSavePoint;	// セーブポイントクラス
class CGoalPoint;	// ゴールポイントクラス

//************************************************************
//	クラス定義
//************************************************************
// エディットステージマネージャークラス
class CEditStageManager
{
public:
	// 配置物列挙
	enum EThing
	{
		THING_BUILDING = 0,	// ビル
		THING_SIGNBOARD,	// 看板
		THING_OBSTACLE,		// 障害物
		THING_SAVEPOINT,	// セーブポイント
		THING_GOALPOINT,	// ゴールポイント
		THING_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CEditStageManager();

	// デストラクタ
	~CEditStageManager();

	// ビル情報構造体
	struct SBuildingInfo
	{
		CBuilding *pBuilding;	// ビル情報
		CBuilding::EType type;	// ビル種類
	};

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新

	// 静的メンバ関数
	static CEditStageManager *Create(void);	// 生成
	static HRESULT Release(CEditStageManager *&prEditStageManager);	// 破棄

private:
	// メンバ関数
	void UpdateBuilding(void);	// ビル配置更新

	// メンバ変数
	D3DXVECTOR3 m_pos;	// 位置
	D3DXVECTOR3 m_rot;	// 向き
	EThing m_thing;		// 配置物
	SBuildingInfo m_building;	// ビル配置情報
	float m_fMove;	// 位置移動量
};

#endif	// _EDIT_STAGEMANAGER_H_
