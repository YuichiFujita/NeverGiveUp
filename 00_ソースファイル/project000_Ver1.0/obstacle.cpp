//============================================================
//
//	��Q������ [obstacle.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "obstacle.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* SETUP_TXT = "data\\TXT\\obstacle.txt";	// ��Q���Z�b�g�A�b�v�e�L�X�g

	const int PRIORITY = 1;	// ��Q���̗D�揇��
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CObstacle::mc_apModelFile[] =	// ���f���萔
{
	"data\\MODEL\\OBSTACLE\\obstacle000.x",	// ��
	"data\\MODEL\\OBSTACLE\\obstacle001.x",	// �x���`
};
CObstacle::SStatusInfo CObstacle::m_aStatusInfo[TYPE_MAX] = {};	// �X�e�[�^�X���

//************************************************************
//	�q�N���X [CObstacle] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CObstacle::CObstacle(const EType type) : CObjectModel(CObject::LABEL_OBSTACLE, PRIORITY), m_type(type)
{
	// �����o�ϐ����N���A
	memset(&m_status, 0, sizeof(m_status));	// �X�e�[�^�X
	m_fAngle = 0.0f;	// �Ίp���̊p�x
	m_fLength = 0.0f;	// �Ίp���̒���
}

//============================================================
//	�f�X�g���N�^
//============================================================
CObstacle::~CObstacle()
{

}

//============================================================
//	����������
//============================================================
HRESULT CObstacle::Init(void)
{
	// �����o�ϐ���������
	memset(&m_status, 0, sizeof(m_status));	// �X�e�[�^�X
	m_fAngle = 0.0f;	// �Ίp���̊p�x
	m_fLength = 0.0f;	// �Ίp���̒���

	// �I�u�W�F�N�g���f���̏�����
	if (FAILED(CObjectModel::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CObstacle::Uninit(void)
{
	// �I�u�W�F�N�g���f���̏I��
	CObjectModel::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CObstacle::Update(void)
{
	// �I�u�W�F�N�g���f���̍X�V
	CObjectModel::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CObstacle::Draw(void)
{
	// �I�u�W�F�N�g���f���̕`��
	CObjectModel::Draw();
}

//============================================================
//	��ގ擾����
//============================================================
int CObstacle::GetType(void) const
{
	// ��Q���̉��@��Ԃ�
	return m_status.dodge;
}

//============================================================
//	�p�x�擾����
//============================================================
float CObstacle::GetAngle(void) const
{
	// �Ίp���̊p�x��Ԃ�
	return m_fAngle;
}

//============================================================
//	�����擾����
//============================================================
float CObstacle::GetLength(void) const
{
	// �Ίp���̒�����Ԃ�
	return m_fLength;
}

//============================================================
//	�傫���擾����
//============================================================
D3DXVECTOR3 CObstacle::GetVec3Sizing(void) const
{
	// ��Q���̑傫����Ԃ�
	return m_status.size;
}

//============================================================
//	��������
//============================================================
CObstacle *CObstacle::Create
(
	const EType type,			// ���
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot		// ����
)
{
	// �|�C���^��錾
	CObstacle *pObstacle = NULL;	// ��Q�������p

	if (pObstacle == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pObstacle = new CObstacle(type);	// ��Q��
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pObstacle != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// ��Q���̏�����
		if (FAILED(pObstacle->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pObstacle;
			pObstacle = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// ��ނ�ݒ�
		pObstacle->SetType(type);

		// �ʒu��ݒ�
		pObstacle->SetVec3Position(rPos);

		// ������ݒ�
		pObstacle->SetVec3Rotation(rRot);

		// �m�ۂ����A�h���X��Ԃ�
		return pObstacle;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	��ނ̐ݒ菈��
//============================================================
void CObstacle::SetType(const EType type)
{
	// �����̎�ނ̏���ݒ�
	if (type < TYPE_MAX)
	{ // ��ނ�����ꍇ

		// ���f����o�^�E����
		BindModel(mc_apModelFile[type]);

		// �X�e�[�^�X��ݒ�
		m_status = m_aStatusInfo[type];

		// �Ίp���̊p�x��ݒ�
		m_fAngle = atan2f(m_aStatusInfo[m_type].size.x, m_aStatusInfo[m_type].size.z);

		// �Ίp���̒�����ݒ�
		m_fLength = sqrtf(m_aStatusInfo[m_type].size.x * m_aStatusInfo[m_type].size.x
				  + m_aStatusInfo[m_type].size.z * m_aStatusInfo[m_type].size.z) * 0.5f;
	}
	else { assert(false); }	// ��ރI�[�o�[
}

//============================================================
//	�Z�b�g�A�b�v����
//============================================================
void CObstacle::LoadSetup(void)
{
	// �ϐ���錾
	int nType = 0;	// ��ނ̑���p
	int nEnd = 0;	// �e�L�X�g�ǂݍ��ݏI���̊m�F�p

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	// �|�C���^��錾
	FILE *pFile;	// �t�@�C���|�C���^

	// �ÓI�����o�ϐ��̏����N���A
	memset(&m_aStatusInfo[0], 0, sizeof(m_aStatusInfo));	// ���f�����

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(SETUP_TXT, "r");

	if (pFile != NULL)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			// �X�e�[�^�X�̐ݒ�
			if (strcmp(&aString[0], "STATUSSET") == 0)
			{ // �ǂݍ��񂾕����� STATUSSET �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_STATUSSET �ł͂Ȃ��ꍇ���[�v

				  // �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "OBSTACLESET") == 0)
					{ // �ǂݍ��񂾕����� OBSTACLESET �̏ꍇ

						do
						{ // �ǂݍ��񂾕����� END_OBSTACLESET �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "TYPE") == 0)
							{ // �ǂݍ��񂾕����� TYPE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &nType);		// ��ނ�ǂݍ���
							}
							else if (strcmp(&aString[0], "SIZE") == 0)
							{ // �ǂݍ��񂾕����� SIZE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].size.x);	// �傫��X��ǂݍ���
								fscanf(pFile, "%f", &m_aStatusInfo[nType].size.y);	// �傫��Y��ǂݍ���
								fscanf(pFile, "%f", &m_aStatusInfo[nType].size.z);	// �傫��Z��ǂݍ���
							}
							else if (strcmp(&aString[0], "DODGE") == 0)
							{ // �ǂݍ��񂾕����� DODGE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &m_aStatusInfo[nType].dodge);	// ���@��ǂݍ���
							}
						} while (strcmp(&aString[0], "END_OBSTACLESET") != 0);	// �ǂݍ��񂾕����� END_OBSTACLESET �ł͂Ȃ��ꍇ���[�v
					}
				} while (strcmp(&aString[0], "END_STATUSSET") != 0);	// �ǂݍ��񂾕����� END_STATUSSET �ł͂Ȃ��ꍇ���[�v
			}
		} while (nEnd != EOF);	// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v
		
		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "��Q���Z�b�g�A�b�v�t�@�C���̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}
}
