//============================================================
//
//	ステージ処理 [stage.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "stage.h"
#include "manager.h"
#include "texture.h"
#include "scene.h"
#include "sceneGame.h"
#include "player.h"
#include "field.h"
#include "collision.h"

#include "wall.h"
#include "scenery.h"
#include "sky.h"
#include "building.h"
#include "signboard.h"
#include "obstacle.h"
#include "savePoint.h"
#include "goalPoint.h"

//************************************************************
//	マクロ定義
//************************************************************
#define STAGE_SETUP_TXT	"data\\TXT\\stage.txt"	// セットアップテキスト相対パス

//************************************************************
//	親クラス [CStage] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CStage::CStage()
{
	// メンバ変数をクリア
	memset(&m_stageLimit, 0, sizeof(m_stageLimit));	// 範囲
}

//============================================================
//	デストラクタ
//============================================================
CStage::~CStage()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CStage::Init(void)
{
	// メンバ変数を初期化
	memset(&m_stageLimit, 0, sizeof(m_stageLimit));	// 範囲

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CStage::Uninit(void)
{

}

//============================================================
//	更新処理
//============================================================
void CStage::Update(void)
{

}

//============================================================
//	ステージ範囲の設定処理
//============================================================
void CStage::SetStageLimit(const SStageLimit& rLimit)
{
	// 引数のステージ範囲を設定
	m_stageLimit = rLimit;
}

//============================================================
//	ステージ範囲取得処理
//============================================================
CStage::SStageLimit CStage::GetStageLimit(void) const
{
	// ステージ範囲を返す
	return m_stageLimit;
}

//============================================================
//	位置の補正処理
//============================================================
void CStage::LimitPosition(D3DXVECTOR3& rPos, const float fRadius)
{
	switch (m_stageLimit.mode)
	{ // 制限モードごとの処理
	case LIMIT_BOX:		// 矩形範囲

		// 角柱の内側制限
		collision::InBoxPillar
		( // 引数
			rPos,				// 判定位置
			VEC3_ZERO,			// 判定原点位置
			VEC3_ALL(fRadius),	// 判定サイズ(右・上・後)
			VEC3_ALL(fRadius),	// 判定サイズ(左・下・前)
			D3DXVECTOR3(fabsf(m_stageLimit.fRight), 0.0f, fabsf(m_stageLimit.fFar)),	// 判定原点サイズ(右・上・後)
			D3DXVECTOR3(fabsf(m_stageLimit.fLeft),  0.0f, fabsf(m_stageLimit.fNear))	// 判定原点サイズ(左・下・前)
		);

		break;

	case LIMIT_CIRCLE:	// 円範囲

		// 円柱の内側制限
		collision::InCirclePillar
		( // 引数
			rPos,					// 判定位置
			VEC3_ZERO,				// 判定原点位置
			fRadius,				// 判定半径
			m_stageLimit.fRadius	// 判定原点半径
		);

		break;

	default:	// 例外処理
		assert(false);
		break;
	}
}

//============================================================
//	範囲外の着地処理
//============================================================
bool CStage::LandPosition(D3DXVECTOR3& rPos, D3DXVECTOR3& rMove, const float fHeight)
{
	if (rPos.y - fHeight < m_stageLimit.fField)
	{ // 位置が地面より下の場合

		// 位置を補正
		rPos.y = m_stageLimit.fField + fHeight;

		// 移動量を初期化
		rMove.y = 0.0f;

		// 着地している状況を返す
		return true;
	}

	// 着地していない状況を返す
	return false;
}

//============================================================
//	生成処理
//============================================================
CStage *CStage::Create(void)
{
	// ポインタを宣言
	CStage *pStage = NULL;		// ステージ生成用

	if (pStage == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pStage = new CStage;	// ステージ
	}
	else { assert(false); return NULL; }	// 使用中

	if (pStage != NULL)
	{ // 確保に成功している場合

		// ステージの初期化
		if (FAILED(pStage->Init()))
		{ // 初期化に失敗した場合

			// 失敗を返す
			return NULL;
		}

		// セットアップの読み込み
		LoadSetup(pStage);

		// 確保したアドレスを返す
		return pStage;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
HRESULT CStage::Release(CStage *&prStage)
{
	if (prStage != NULL)
	{ // 使用中の場合

		// ステージの終了
		prStage->Uninit();

		// メモリ開放
		delete prStage;
		prStage = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}

//============================================================
//	セットアップ処理
//============================================================
void CStage::LoadSetup(CStage *pStage)
{
	// 変数を宣言
	SStageLimit stageLimit;			// ステージ範囲の代入用
	D3DXVECTOR3 pos = VEC3_ZERO;	// 位置の代入用
	D3DXVECTOR3 rot = VEC3_ZERO;	// 向きの代入用
	D3DXCOLOR col = XCOL_WHITE;		// 色の代入用
	POSGRID2 part = GRID2_ZERO;		// 分割数の代入用

	float fRadius = 0.0f;	// 半径の代入用
	float fHeight = 0.0f;	// 縦幅の代入用
	int nTextureID = 0;		// テクスチャインデックスの代入用
	int nEnd = 0;			// テキスト読み込み終了の確認用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	// ポインタを宣言
	FILE *pFile;	// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(STAGE_SETUP_TXT, "r");

	if (pFile != NULL)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			// ステージ範囲の設定
			if (strcmp(&aString[0], "LIMITSET") == 0)
			{ // 読み込んだ文字列が LIMITSET の場合

				do
				{ // 読み込んだ文字列が END_LIMITSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "CENTER") == 0)
					{ // 読み込んだ文字列が CENTER の場合

						fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
						fscanf(pFile, "%f", &stageLimit.center.x);	// 中心座標Xを読み込む
						fscanf(pFile, "%f", &stageLimit.center.y);	// 中心座標Yを読み込む
						fscanf(pFile, "%f", &stageLimit.center.z);	// 中心座標Zを読み込む
					}
					else if (strcmp(&aString[0], "NEAR") == 0)
					{ // 読み込んだ文字列が NEAR の場合

						fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
						fscanf(pFile, "%f", &stageLimit.fNear);		// 前位置を読み込む

						// 制限モードを矩形範囲に設定
						stageLimit.mode = LIMIT_BOX;
					}
					else if (strcmp(&aString[0], "FAR") == 0)
					{ // 読み込んだ文字列が FAR の場合

						fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
						fscanf(pFile, "%f", &stageLimit.fFar);		// 後位置を読み込む

						// 制限モードを矩形範囲に設定
						stageLimit.mode = LIMIT_BOX;
					}
					else if (strcmp(&aString[0], "RIGHT") == 0)
					{ // 読み込んだ文字列が RIGHT の場合

						fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
						fscanf(pFile, "%f", &stageLimit.fRight);	// 右位置を読み込む

						// 制限モードを矩形範囲に設定
						stageLimit.mode = LIMIT_BOX;
					}
					else if (strcmp(&aString[0], "LEFT") == 0)
					{ // 読み込んだ文字列が LEFT の場合

						fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
						fscanf(pFile, "%f", &stageLimit.fLeft);		// 左位置を読み込む

						// 制限モードを矩形範囲に設定
						stageLimit.mode = LIMIT_BOX;
					}
					else if (strcmp(&aString[0], "RADIUS") == 0)
					{ // 読み込んだ文字列が RADIUS の場合

						fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
						fscanf(pFile, "%f", &stageLimit.fRadius);	// 半径を読み込む

						// 制限モードを円範囲に設定
						stageLimit.mode = LIMIT_CIRCLE;
					}
					else if (strcmp(&aString[0], "FIELD") == 0)
					{ // 読み込んだ文字列が FIELD の場合

						fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
						fscanf(pFile, "%f", &stageLimit.fField);	// 地面位置を読み込む
					}
				} while (strcmp(&aString[0], "END_LIMITSET") != 0);	// 読み込んだ文字列が END_LIMITSET ではない場合ループ

				// ステージ範囲の設定
				pStage->SetStageLimit(stageLimit);
			}

			// 空の設定
			else if (strcmp(&aString[0], "STAGE_SKYSET") == 0)
			{ // 読み込んだ文字列が STAGE_SKYSET の場合

				do
				{ // 読み込んだ文字列が END_STAGE_SKYSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "SKYSET") == 0)
					{ // 読み込んだ文字列が SKYSET の場合
		
						do
						{ // 読み込んだ文字列が END_SKYSET ではない場合ループ
		
							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);
		
							if (strcmp(&aString[0], "TEXTURE_ID") == 0)
							{ // 読み込んだ文字列が TEXTURE_ID の場合
		
								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%d", &nTextureID);	// テクスチャインデックスを読み込む
							}
							else if (strcmp(&aString[0], "POS") == 0)
							{ // 読み込んだ文字列が POS の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%f", &pos.x);		// 位置Xを読み込む
								fscanf(pFile, "%f", &pos.y);		// 位置Yを読み込む
								fscanf(pFile, "%f", &pos.z);		// 位置Zを読み込む
							}
							else if (strcmp(&aString[0], "ROT") == 0)
							{ // 読み込んだ文字列が ROT の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%f", &rot.x);		// 向きXを読み込む
								fscanf(pFile, "%f", &rot.y);		// 向きYを読み込む
								fscanf(pFile, "%f", &rot.z);		// 向きZを読み込む
							}
							else if (strcmp(&aString[0], "COL") == 0)
							{ // 読み込んだ文字列が COL の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%f", &col.r);		// 色Rを読み込む
								fscanf(pFile, "%f", &col.g);		// 色Gを読み込む
								fscanf(pFile, "%f", &col.b);		// 色Bを読み込む
								fscanf(pFile, "%f", &col.a);		// 色Aを読み込む
							}
							else if (strcmp(&aString[0], "PARTWIDTH") == 0)
							{ // 読み込んだ文字列が PARTWIDTH の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%d", &part.x);		// 横分割数を読み込む
							}
							else if (strcmp(&aString[0], "PARTHEIGHT") == 0)
							{ // 読み込んだ文字列が PARTHEIGHT の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%d", &part.y);		// 縦分割数を読み込む
							}
							else if (strcmp(&aString[0], "RADIUS") == 0)
							{ // 読み込んだ文字列が RADIUS の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%f", &fRadius);		// 半径を読み込む
							}
						} while (strcmp(&aString[0], "END_SKYSET") != 0);	// 読み込んだ文字列が END_SKYSET ではない場合ループ

						// 空オブジェクトの生成
						CSky::Create((CSky::ETexture)nTextureID, pos, D3DXToRadian(rot), col, part, fRadius);
					}
				} while (strcmp(&aString[0], "END_STAGE_SKYSET") != 0);	// 読み込んだ文字列が END_STAGE_SKYSET ではない場合ループ
			}

			// 景色の設定
			else if (strcmp(&aString[0], "STAGE_SCENERYSET") == 0)
			{ // 読み込んだ文字列が STAGE_SCENERYSET の場合

				do
				{ // 読み込んだ文字列が END_STAGE_SCENERYSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "SCENERYSET") == 0)
					{ // 読み込んだ文字列が SCENERYSET の場合
		
						do
						{ // 読み込んだ文字列が END_SCENERYSET ではない場合ループ
		
							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);
		
							if (strcmp(&aString[0], "TEXTURE_ID") == 0)
							{ // 読み込んだ文字列が TEXTURE_ID の場合
		
								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%d", &nTextureID);	// テクスチャインデックスを読み込む
							}
							else if (strcmp(&aString[0], "POS") == 0)
							{ // 読み込んだ文字列が POS の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%f", &pos.x);		// 位置Xを読み込む
								fscanf(pFile, "%f", &pos.y);		// 位置Yを読み込む
								fscanf(pFile, "%f", &pos.z);		// 位置Zを読み込む
							}
							else if (strcmp(&aString[0], "ROT") == 0)
							{ // 読み込んだ文字列が ROT の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%f", &rot.x);		// 向きXを読み込む
								fscanf(pFile, "%f", &rot.y);		// 向きYを読み込む
								fscanf(pFile, "%f", &rot.z);		// 向きZを読み込む
							}
							else if (strcmp(&aString[0], "COL") == 0)
							{ // 読み込んだ文字列が COL の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%f", &col.r);		// 色Rを読み込む
								fscanf(pFile, "%f", &col.g);		// 色Gを読み込む
								fscanf(pFile, "%f", &col.b);		// 色Bを読み込む
								fscanf(pFile, "%f", &col.a);		// 色Aを読み込む
							}
							else if (strcmp(&aString[0], "PARTWIDTH") == 0)
							{ // 読み込んだ文字列が PARTWIDTH の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%d", &part.x);		// 横分割数を読み込む
							}
							else if (strcmp(&aString[0], "PARTHEIGHT") == 0)
							{ // 読み込んだ文字列が PARTHEIGHT の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%d", &part.y);		// 縦分割数を読み込む
							}
							else if (strcmp(&aString[0], "RADIUS") == 0)
							{ // 読み込んだ文字列が RADIUS の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%f", &fRadius);		// 半径を読み込む
							}
							else if (strcmp(&aString[0], "HEIGHT") == 0)
							{ // 読み込んだ文字列が HEIGHT の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%f", &fHeight);		// 縦幅を読み込む
							}
						} while (strcmp(&aString[0], "END_SCENERYSET") != 0);	// 読み込んだ文字列が END_SCENERYSET ではない場合ループ

						// 景色オブジェクトの生成
						CScenery::Create((CScenery::ETexture)nTextureID, pos, D3DXToRadian(rot), col, part, fRadius, fHeight);
					}
				} while (strcmp(&aString[0], "END_STAGE_SCENERYSET") != 0);	// 読み込んだ文字列が END_STAGE_SCENERYSET ではない場合ループ
			}
		} while (nEnd != EOF);	// 読み込んだ文字列が EOF ではない場合ループ
		
		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(NULL, "ステージセットアップの読み込みに失敗！", "警告！", MB_ICONWARNING);
	}
}
