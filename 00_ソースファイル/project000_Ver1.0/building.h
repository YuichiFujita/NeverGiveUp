//============================================================
//
//	ビルヘッダー [building.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _BUILDING_H_
#define _BUILDING_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "objectMeshCube.h"

//************************************************************
//	クラス定義
//************************************************************
// ビルクラス
class CBuilding : public CObjectMeshCube
{
public:
	// 種類列挙
	enum EType
	{
		TYPE_00 = 0,	// ビル00テクスチャ
		TYPE_01,		// ビル01テクスチャ
		TYPE_02,		// ビル02テクスチャ
		TYPE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CBuilding();

	// デストラクタ
	~CBuilding();

	// オーバーライド関数
	HRESULT Init(void) override;			// 初期化
	void Uninit(void) override;				// 終了
	void Update(void) override;				// 更新
	void Draw(void) override;				// 描画
	void SetType(const int nType) override;	// 種類設定

	// 静的メンバ関数
	static CBuilding *Create	// 生成
	( // 引数
		const EType type,			// 種類
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot,	// 向き
		const D3DXVECTOR3& rSize,	// 大きさ
		const D3DXCOLOR& rCol		// 色
	);

private:
	// 静的メンバ変数
	static const char *mc_apTextureFile[][6];	// テクスチャ定数
};

#endif	// _BUILDING_H_
