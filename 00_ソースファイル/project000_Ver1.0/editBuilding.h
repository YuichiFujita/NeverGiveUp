//============================================================
//
//	エディットビルヘッダー [editBuilding.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _EDIT_BUILDING_H_
#define _EDIT_BUILDING_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "building.h"

//************************************************************
//	前方宣言
//************************************************************
class CEditStageManager;	// エディットステージマネージャー

//************************************************************
//	クラス定義
//************************************************************
// エディットビルクラス
class CEditBuilding
{
public:
	// コンストラクタ
	CEditBuilding();

	// デストラクタ
	~CEditBuilding();

	// ビル情報構造体
	struct SInfo
	{
		CBuilding *pBuilding;	// ビル情報
		CBuilding::EType type;	// ビル種類
	};

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新

	// 静的メンバ関数
	static CEditBuilding *Create(CEditStageManager *pEdit);	// 生成
	static HRESULT Release(CEditBuilding *&prEditBuilding);	// 破棄

private:
	// メンバ変数
	CEditStageManager *m_pEdit;	// エディットステージの情報
	SInfo m_building;	// ビル配置情報
};

#endif	// _EDIT_BUILDING_H_
