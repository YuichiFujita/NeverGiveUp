//============================================================
//
//	�X�e�[�W���� [stage.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
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
//	�}�N����`
//************************************************************
#define STAGE_SETUP_TXT	"data\\TXT\\stage.txt"	// �Z�b�g�A�b�v�e�L�X�g���΃p�X

//************************************************************
//	�e�N���X [CStage] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CStage::CStage()
{
	// �����o�ϐ����N���A
	memset(&m_stageLimit, 0, sizeof(m_stageLimit));	// �͈�
}

//============================================================
//	�f�X�g���N�^
//============================================================
CStage::~CStage()
{

}

//============================================================
//	����������
//============================================================
HRESULT CStage::Init(void)
{
	// �����o�ϐ���������
	memset(&m_stageLimit, 0, sizeof(m_stageLimit));	// �͈�

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CStage::Uninit(void)
{

}

//============================================================
//	�X�V����
//============================================================
void CStage::Update(void)
{

}

//============================================================
//	�X�e�[�W�͈͂̐ݒ菈��
//============================================================
void CStage::SetStageLimit(const SStageLimit& rLimit)
{
	// �����̃X�e�[�W�͈͂�ݒ�
	m_stageLimit = rLimit;
}

//============================================================
//	�X�e�[�W�͈͎擾����
//============================================================
CStage::SStageLimit CStage::GetStageLimit(void) const
{
	// �X�e�[�W�͈͂�Ԃ�
	return m_stageLimit;
}

//============================================================
//	�ʒu�̕␳����
//============================================================
void CStage::LimitPosition(D3DXVECTOR3& rPos, const float fRadius)
{
	switch (m_stageLimit.mode)
	{ // �������[�h���Ƃ̏���
	case LIMIT_BOX:		// ��`�͈�

		// �p���̓�������
		collision::InBoxPillar
		( // ����
			rPos,				// ����ʒu
			VEC3_ZERO,			// ���茴�_�ʒu
			VEC3_ALL(fRadius),	// ����T�C�Y(�E�E��E��)
			VEC3_ALL(fRadius),	// ����T�C�Y(���E���E�O)
			D3DXVECTOR3(fabsf(m_stageLimit.fRight), 0.0f, fabsf(m_stageLimit.fFar)),	// ���茴�_�T�C�Y(�E�E��E��)
			D3DXVECTOR3(fabsf(m_stageLimit.fLeft),  0.0f, fabsf(m_stageLimit.fNear))	// ���茴�_�T�C�Y(���E���E�O)
		);

		break;

	case LIMIT_CIRCLE:	// �~�͈�

		// �~���̓�������
		collision::InCirclePillar
		( // ����
			rPos,					// ����ʒu
			VEC3_ZERO,				// ���茴�_�ʒu
			fRadius,				// ���蔼�a
			m_stageLimit.fRadius	// ���茴�_���a
		);

		break;

	default:	// ��O����
		assert(false);
		break;
	}
}

//============================================================
//	�͈͊O�̒��n����
//============================================================
bool CStage::LandPosition(D3DXVECTOR3& rPos, D3DXVECTOR3& rMove, const float fHeight)
{
	if (rPos.y - fHeight < m_stageLimit.fField)
	{ // �ʒu���n�ʂ�艺�̏ꍇ

		// �ʒu��␳
		rPos.y = m_stageLimit.fField + fHeight;

		// �ړ��ʂ�������
		rMove.y = 0.0f;

		// ���n���Ă���󋵂�Ԃ�
		return true;
	}

	// ���n���Ă��Ȃ��󋵂�Ԃ�
	return false;
}

//============================================================
//	��������
//============================================================
CStage *CStage::Create(void)
{
	// �|�C���^��錾
	CStage *pStage = NULL;		// �X�e�[�W�����p

	if (pStage == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pStage = new CStage;	// �X�e�[�W
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pStage != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// �X�e�[�W�̏�����
		if (FAILED(pStage->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ���s��Ԃ�
			return NULL;
		}

		// �Z�b�g�A�b�v�̓ǂݍ���
		LoadSetup(pStage);

		// �m�ۂ����A�h���X��Ԃ�
		return pStage;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
HRESULT CStage::Release(CStage *&prStage)
{
	if (prStage != NULL)
	{ // �g�p���̏ꍇ

		// �X�e�[�W�̏I��
		prStage->Uninit();

		// �������J��
		delete prStage;
		prStage = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}

//============================================================
//	�Z�b�g�A�b�v����
//============================================================
void CStage::LoadSetup(CStage *pStage)
{
	// �ϐ���錾
	SStageLimit stageLimit;			// �X�e�[�W�͈͂̑���p
	D3DXVECTOR3 pos = VEC3_ZERO;	// �ʒu�̑���p
	D3DXVECTOR3 rot = VEC3_ZERO;	// �����̑���p
	D3DXCOLOR col = XCOL_WHITE;		// �F�̑���p
	POSGRID2 part = GRID2_ZERO;		// �������̑���p

	float fRadius = 0.0f;	// ���a�̑���p
	float fHeight = 0.0f;	// �c���̑���p
	int nTextureID = 0;		// �e�N�X�`���C���f�b�N�X�̑���p
	int nEnd = 0;			// �e�L�X�g�ǂݍ��ݏI���̊m�F�p

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	// �|�C���^��錾
	FILE *pFile;	// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(STAGE_SETUP_TXT, "r");

	if (pFile != NULL)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			// �X�e�[�W�͈͂̐ݒ�
			if (strcmp(&aString[0], "LIMITSET") == 0)
			{ // �ǂݍ��񂾕����� LIMITSET �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_LIMITSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "CENTER") == 0)
					{ // �ǂݍ��񂾕����� CENTER �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &stageLimit.center.x);	// ���S���WX��ǂݍ���
						fscanf(pFile, "%f", &stageLimit.center.y);	// ���S���WY��ǂݍ���
						fscanf(pFile, "%f", &stageLimit.center.z);	// ���S���WZ��ǂݍ���
					}
					else if (strcmp(&aString[0], "NEAR") == 0)
					{ // �ǂݍ��񂾕����� NEAR �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &stageLimit.fNear);		// �O�ʒu��ǂݍ���

						// �������[�h����`�͈͂ɐݒ�
						stageLimit.mode = LIMIT_BOX;
					}
					else if (strcmp(&aString[0], "FAR") == 0)
					{ // �ǂݍ��񂾕����� FAR �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &stageLimit.fFar);		// ��ʒu��ǂݍ���

						// �������[�h����`�͈͂ɐݒ�
						stageLimit.mode = LIMIT_BOX;
					}
					else if (strcmp(&aString[0], "RIGHT") == 0)
					{ // �ǂݍ��񂾕����� RIGHT �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &stageLimit.fRight);	// �E�ʒu��ǂݍ���

						// �������[�h����`�͈͂ɐݒ�
						stageLimit.mode = LIMIT_BOX;
					}
					else if (strcmp(&aString[0], "LEFT") == 0)
					{ // �ǂݍ��񂾕����� LEFT �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &stageLimit.fLeft);		// ���ʒu��ǂݍ���

						// �������[�h����`�͈͂ɐݒ�
						stageLimit.mode = LIMIT_BOX;
					}
					else if (strcmp(&aString[0], "RADIUS") == 0)
					{ // �ǂݍ��񂾕����� RADIUS �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &stageLimit.fRadius);	// ���a��ǂݍ���

						// �������[�h���~�͈͂ɐݒ�
						stageLimit.mode = LIMIT_CIRCLE;
					}
					else if (strcmp(&aString[0], "FIELD") == 0)
					{ // �ǂݍ��񂾕����� FIELD �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &stageLimit.fField);	// �n�ʈʒu��ǂݍ���
					}
				} while (strcmp(&aString[0], "END_LIMITSET") != 0);	// �ǂݍ��񂾕����� END_LIMITSET �ł͂Ȃ��ꍇ���[�v

				// �X�e�[�W�͈͂̐ݒ�
				pStage->SetStageLimit(stageLimit);
			}

			// ��̐ݒ�
			else if (strcmp(&aString[0], "STAGE_SKYSET") == 0)
			{ // �ǂݍ��񂾕����� STAGE_SKYSET �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_STAGE_SKYSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "SKYSET") == 0)
					{ // �ǂݍ��񂾕����� SKYSET �̏ꍇ
		
						do
						{ // �ǂݍ��񂾕����� END_SKYSET �ł͂Ȃ��ꍇ���[�v
		
							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);
		
							if (strcmp(&aString[0], "TEXTURE_ID") == 0)
							{ // �ǂݍ��񂾕����� TEXTURE_ID �̏ꍇ
		
								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &nTextureID);	// �e�N�X�`���C���f�b�N�X��ǂݍ���
							}
							else if (strcmp(&aString[0], "POS") == 0)
							{ // �ǂݍ��񂾕����� POS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &pos.x);		// �ʒuX��ǂݍ���
								fscanf(pFile, "%f", &pos.y);		// �ʒuY��ǂݍ���
								fscanf(pFile, "%f", &pos.z);		// �ʒuZ��ǂݍ���
							}
							else if (strcmp(&aString[0], "ROT") == 0)
							{ // �ǂݍ��񂾕����� ROT �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &rot.x);		// ����X��ǂݍ���
								fscanf(pFile, "%f", &rot.y);		// ����Y��ǂݍ���
								fscanf(pFile, "%f", &rot.z);		// ����Z��ǂݍ���
							}
							else if (strcmp(&aString[0], "COL") == 0)
							{ // �ǂݍ��񂾕����� COL �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &col.r);		// �FR��ǂݍ���
								fscanf(pFile, "%f", &col.g);		// �FG��ǂݍ���
								fscanf(pFile, "%f", &col.b);		// �FB��ǂݍ���
								fscanf(pFile, "%f", &col.a);		// �FA��ǂݍ���
							}
							else if (strcmp(&aString[0], "PARTWIDTH") == 0)
							{ // �ǂݍ��񂾕����� PARTWIDTH �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &part.x);		// ����������ǂݍ���
							}
							else if (strcmp(&aString[0], "PARTHEIGHT") == 0)
							{ // �ǂݍ��񂾕����� PARTHEIGHT �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &part.y);		// �c��������ǂݍ���
							}
							else if (strcmp(&aString[0], "RADIUS") == 0)
							{ // �ǂݍ��񂾕����� RADIUS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &fRadius);		// ���a��ǂݍ���
							}
						} while (strcmp(&aString[0], "END_SKYSET") != 0);	// �ǂݍ��񂾕����� END_SKYSET �ł͂Ȃ��ꍇ���[�v

						// ��I�u�W�F�N�g�̐���
						CSky::Create((CSky::ETexture)nTextureID, pos, D3DXToRadian(rot), col, part, fRadius);
					}
				} while (strcmp(&aString[0], "END_STAGE_SKYSET") != 0);	// �ǂݍ��񂾕����� END_STAGE_SKYSET �ł͂Ȃ��ꍇ���[�v
			}

			// �i�F�̐ݒ�
			else if (strcmp(&aString[0], "STAGE_SCENERYSET") == 0)
			{ // �ǂݍ��񂾕����� STAGE_SCENERYSET �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_STAGE_SCENERYSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "SCENERYSET") == 0)
					{ // �ǂݍ��񂾕����� SCENERYSET �̏ꍇ
		
						do
						{ // �ǂݍ��񂾕����� END_SCENERYSET �ł͂Ȃ��ꍇ���[�v
		
							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);
		
							if (strcmp(&aString[0], "TEXTURE_ID") == 0)
							{ // �ǂݍ��񂾕����� TEXTURE_ID �̏ꍇ
		
								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &nTextureID);	// �e�N�X�`���C���f�b�N�X��ǂݍ���
							}
							else if (strcmp(&aString[0], "POS") == 0)
							{ // �ǂݍ��񂾕����� POS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &pos.x);		// �ʒuX��ǂݍ���
								fscanf(pFile, "%f", &pos.y);		// �ʒuY��ǂݍ���
								fscanf(pFile, "%f", &pos.z);		// �ʒuZ��ǂݍ���
							}
							else if (strcmp(&aString[0], "ROT") == 0)
							{ // �ǂݍ��񂾕����� ROT �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &rot.x);		// ����X��ǂݍ���
								fscanf(pFile, "%f", &rot.y);		// ����Y��ǂݍ���
								fscanf(pFile, "%f", &rot.z);		// ����Z��ǂݍ���
							}
							else if (strcmp(&aString[0], "COL") == 0)
							{ // �ǂݍ��񂾕����� COL �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &col.r);		// �FR��ǂݍ���
								fscanf(pFile, "%f", &col.g);		// �FG��ǂݍ���
								fscanf(pFile, "%f", &col.b);		// �FB��ǂݍ���
								fscanf(pFile, "%f", &col.a);		// �FA��ǂݍ���
							}
							else if (strcmp(&aString[0], "PARTWIDTH") == 0)
							{ // �ǂݍ��񂾕����� PARTWIDTH �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &part.x);		// ����������ǂݍ���
							}
							else if (strcmp(&aString[0], "PARTHEIGHT") == 0)
							{ // �ǂݍ��񂾕����� PARTHEIGHT �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &part.y);		// �c��������ǂݍ���
							}
							else if (strcmp(&aString[0], "RADIUS") == 0)
							{ // �ǂݍ��񂾕����� RADIUS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &fRadius);		// ���a��ǂݍ���
							}
							else if (strcmp(&aString[0], "HEIGHT") == 0)
							{ // �ǂݍ��񂾕����� HEIGHT �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &fHeight);		// �c����ǂݍ���
							}
						} while (strcmp(&aString[0], "END_SCENERYSET") != 0);	// �ǂݍ��񂾕����� END_SCENERYSET �ł͂Ȃ��ꍇ���[�v

						// �i�F�I�u�W�F�N�g�̐���
						CScenery::Create((CScenery::ETexture)nTextureID, pos, D3DXToRadian(rot), col, part, fRadius, fHeight);
					}
				} while (strcmp(&aString[0], "END_STAGE_SCENERYSET") != 0);	// �ǂݍ��񂾕����� END_STAGE_SCENERYSET �ł͂Ȃ��ꍇ���[�v
			}
		} while (nEnd != EOF);	// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v
		
		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�X�e�[�W�Z�b�g�A�b�v�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}
}
