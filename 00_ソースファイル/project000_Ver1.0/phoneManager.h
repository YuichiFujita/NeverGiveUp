//============================================================
//
//	スマホマネージャーヘッダー [phoneManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _PHONE_MANAGER_H_
#define _PHONE_MANAGER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"

//************************************************************
//	前方宣言
//************************************************************
class CObject2D;	// オブジェクト2Dクラス

//************************************************************
//	クラス定義
//************************************************************
// スマホマネージャークラス
class CPhoneManager
{
public:
	// テクスチャ列挙
	enum TEXTURE
	{
		TEXTURE_PHONE = 0,	// スマホのテクスチャ
		TEXTURE_CONTROL,	// 操作表示のテクスチャ
		TEXTURE_MAX			// この列挙型の総数
	};

	// 状態列挙
	enum STATE
	{
		STATE_NONE = 0,		// 何もしない状態
		STATE_FADEIN,		// フェードイン状態
		STATE_PHONE_TAKE,	// スマホの取り出し状態
		STATE_WAIT,			// 待機状態
		STATE_PHONE_RETURN,	// スマホのしまい状態
		STATE_END,			// 終了状態
		STATE_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CPhoneManager();

	// デストラクタ
	~CPhoneManager();

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void SetLook(void);	// スマホ表示開始
	void SetState(const STATE state);			// 状態設定
	STATE GetState(void) const;					// 状態取得
	void SetEnableDisp(const bool bDisp);		// 表示設定
	bool IsDisp(void) const;					// 表示取得
	//void SetMessage(const EMassage massage);	// メッセージ設定

	// 静的メンバ関数
	static CPhoneManager *Create(void);	// 生成
	static HRESULT Release(CPhoneManager *&prPhoneManager);	// 破棄

private:
	// メンバ関数
	void UpdateFade(void);			// フェードイン
	void UpdatePhoneTake(void);		// スマホ取出
	void UpdateWait(void);			// 待機
	void UpdatePhoneReturn(void);	// スマホ収納

	// 静的メンバ変数
	static const char *mc_apTextureFile[];	// テクスチャ定数

	// メンバ変数
	CObject2D *m_pPhone;	// スマホの情報
	CObject2D *m_pFade;		// フェードの情報
	STATE m_state;			// 状態
	int m_nCounterState;	// 状態管理カウンター
	float m_fMove;			// スマホの移動量
	bool m_bDisp;			// 表示状況
};

#endif	// _PHONE_MANAGER_H_
