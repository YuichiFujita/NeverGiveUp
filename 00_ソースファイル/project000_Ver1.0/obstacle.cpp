//============================================================
//
//	障害物処理 [obstacle.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "obstacle.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* SETUP_TXT = "data\\TXT\\obstacle.txt";	// 障害物セットアップテキスト

	const int PRIORITY = 1;	// 障害物の優先順位
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CObstacle::mc_apModelFile[] =	// モデル定数
{
	"data\\MODEL\\OBSTACLE\\obstacle000.x",	// 箱
	"data\\MODEL\\OBSTACLE\\obstacle001.x",	// ベンチ
};
CObstacle::SStatusInfo CObstacle::m_aStatusInfo[TYPE_MAX] = {};	// ステータス情報

//************************************************************
//	子クラス [CObstacle] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CObstacle::CObstacle(const EType type) : CObjectModel(CObject::LABEL_OBSTACLE, PRIORITY), m_type(type)
{
	// メンバ変数をクリア
	memset(&m_status, 0, sizeof(m_status));	// ステータス
	m_fAngle = 0.0f;	// 対角線の角度
	m_fLength = 0.0f;	// 対角線の長さ
}

//============================================================
//	デストラクタ
//============================================================
CObstacle::~CObstacle()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CObstacle::Init(void)
{
	// メンバ変数を初期化
	memset(&m_status, 0, sizeof(m_status));	// ステータス
	m_fAngle = 0.0f;	// 対角線の角度
	m_fLength = 0.0f;	// 対角線の長さ

	// オブジェクトモデルの初期化
	if (FAILED(CObjectModel::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CObstacle::Uninit(void)
{
	// オブジェクトモデルの終了
	CObjectModel::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CObstacle::Update(void)
{
	// オブジェクトモデルの更新
	CObjectModel::Update();
}

//============================================================
//	描画処理
//============================================================
void CObstacle::Draw(void)
{
	// オブジェクトモデルの描画
	CObjectModel::Draw();
}

//============================================================
//	種類取得処理
//============================================================
int CObstacle::GetType(void) const
{
	// 障害物の回避法を返す
	return m_status.dodge;
}

//============================================================
//	角度取得処理
//============================================================
float CObstacle::GetAngle(void) const
{
	// 対角線の角度を返す
	return m_fAngle;
}

//============================================================
//	長さ取得処理
//============================================================
float CObstacle::GetLength(void) const
{
	// 対角線の長さを返す
	return m_fLength;
}

//============================================================
//	大きさ取得処理
//============================================================
D3DXVECTOR3 CObstacle::GetVec3Sizing(void) const
{
	// 障害物の大きさを返す
	return m_status.size;
}

//============================================================
//	生成処理
//============================================================
CObstacle *CObstacle::Create
(
	const EType type,			// 種類
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot		// 向き
)
{
	// ポインタを宣言
	CObstacle *pObstacle = NULL;	// 障害物生成用

	if (pObstacle == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pObstacle = new CObstacle(type);	// 障害物
	}
	else { assert(false); return NULL; }	// 使用中

	if (pObstacle != NULL)
	{ // 確保に成功している場合

		// 障害物の初期化
		if (FAILED(pObstacle->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pObstacle;
			pObstacle = NULL;

			// 失敗を返す
			return NULL;
		}

		// 種類を設定
		pObstacle->SetType(type);

		// 位置を設定
		pObstacle->SetVec3Position(rPos);

		// 向きを設定
		pObstacle->SetVec3Rotation(rRot);

		// 確保したアドレスを返す
		return pObstacle;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	種類の設定処理
//============================================================
void CObstacle::SetType(const EType type)
{
	// 引数の種類の情報を設定
	if (type < TYPE_MAX)
	{ // 種類がある場合

		// モデルを登録・割当
		BindModel(mc_apModelFile[type]);

		// ステータスを設定
		m_status = m_aStatusInfo[type];

		// 対角線の角度を設定
		m_fAngle = atan2f(m_aStatusInfo[m_type].size.x, m_aStatusInfo[m_type].size.z);

		// 対角線の長さを設定
		m_fLength = sqrtf(m_aStatusInfo[m_type].size.x * m_aStatusInfo[m_type].size.x
				  + m_aStatusInfo[m_type].size.z * m_aStatusInfo[m_type].size.z) * 0.5f;
	}
	else { assert(false); }	// 種類オーバー
}

//============================================================
//	セットアップ処理
//============================================================
void CObstacle::LoadSetup(void)
{
	// 変数を宣言
	int nType = 0;	// 種類の代入用
	int nEnd = 0;	// テキスト読み込み終了の確認用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	// ポインタを宣言
	FILE *pFile;	// ファイルポインタ

	// 静的メンバ変数の情報をクリア
	memset(&m_aStatusInfo[0], 0, sizeof(m_aStatusInfo));	// モデル情報

	// ファイルを読み込み形式で開く
	pFile = fopen(SETUP_TXT, "r");

	if (pFile != NULL)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			// ステータスの設定
			if (strcmp(&aString[0], "STATUSSET") == 0)
			{ // 読み込んだ文字列が STATUSSET の場合

				do
				{ // 読み込んだ文字列が END_STATUSSET ではない場合ループ

				  // ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "OBSTACLESET") == 0)
					{ // 読み込んだ文字列が OBSTACLESET の場合

						do
						{ // 読み込んだ文字列が END_OBSTACLESET ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "TYPE") == 0)
							{ // 読み込んだ文字列が TYPE の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%d", &nType);		// 種類を読み込む
							}
							else if (strcmp(&aString[0], "SIZE") == 0)
							{ // 読み込んだ文字列が SIZE の場合

								fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].size.x);	// 大きさXを読み込む
								fscanf(pFile, "%f", &m_aStatusInfo[nType].size.y);	// 大きさYを読み込む
								fscanf(pFile, "%f", &m_aStatusInfo[nType].size.z);	// 大きさZを読み込む
							}
							else if (strcmp(&aString[0], "DODGE") == 0)
							{ // 読み込んだ文字列が DODGE の場合

								fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
								fscanf(pFile, "%d", &m_aStatusInfo[nType].dodge);	// 回避法を読み込む
							}
						} while (strcmp(&aString[0], "END_OBSTACLESET") != 0);	// 読み込んだ文字列が END_OBSTACLESET ではない場合ループ
					}
				} while (strcmp(&aString[0], "END_STATUSSET") != 0);	// 読み込んだ文字列が END_STATUSSET ではない場合ループ
			}
		} while (nEnd != EOF);	// 読み込んだ文字列が EOF ではない場合ループ
		
		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(NULL, "障害物セットアップファイルの読み込みに失敗！", "警告！", MB_ICONWARNING);
	}
}
