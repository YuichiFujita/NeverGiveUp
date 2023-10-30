//============================================================
//
//	�^�C�}�[�}�l�[�W���[���� [timerManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "timerManager.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "value.h"
#include "object2D.h"
#include "player.h"

//************************************************************
//	�}�N����`
//************************************************************
#define TIMER_PRIO	(6)	// �^�C�}�[�̗D�揇��

#define TIME_NUMMIN	(DWORD)(0)			// �ŏ��^�C��
#define TIME_NUMMAX	(DWORD)(99 * 60000)	// �ő�^�C��

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CTimerManager::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\timer000.png",	// ��؂�\��
};

//************************************************************
//	�q�N���X [CTimerManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CTimerManager::CTimerManager()
{
	// �����o�ϐ����N���A
	memset(&m_apValue[0], 0, sizeof(m_apValue));	// ���l�̏��
	memset(&m_apPart[0], 0, sizeof(m_apPart));		// ��؂�̏��
	m_pos				= VEC3_ZERO;	// �ʒu
	m_sizeValue			= VEC3_ZERO;	// �����̑傫��
	m_sizePart			= VEC3_ZERO;	// ��؂�̑傫��
	m_spaceValue		= VEC3_ZERO;	// �����̋�
	m_spacePart			= VEC3_ZERO;	// ��؂�̋�
	m_dwStartTime		= 0;			// �J�n����
	m_dwTime			= 0;			// �o�ߎ���
	m_dwStopStartTime	= 0;			// ��~�J�n����
	m_dwStopTime		= 0;			// ��~����
	m_dwTempTime		= 0;			// �o�ߎ��Ԃ̌v�Z�p
	m_state				= STATE_NONE;	// �v�����
	m_bStop				= true;			// �v����~��
	m_nLimit			= 0;			// ��������
}

//============================================================
//	�f�X�g���N�^
//============================================================
CTimerManager::~CTimerManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CTimerManager::Init(void)
{
	// �|�C���^��錾
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// �e�N�X�`���ւ̃|�C���^

	// �����o�ϐ���������
	memset(&m_apValue[0], 0, sizeof(m_apValue));	// ���l�̏��
	memset(&m_apPart[0], 0, sizeof(m_apPart));		// ��؂�̏��
	m_pos				= VEC3_ZERO;	// �ʒu
	m_sizeValue			= VEC3_ZERO;	// �����̑傫��
	m_sizePart			= VEC3_ZERO;	// ��؂�̑傫��
	m_spaceValue		= VEC3_ZERO;	// �����̋�
	m_spacePart			= VEC3_ZERO;	// ��؂�̋�
	m_dwStartTime		= 0;			// �J�n����
	m_dwTime			= 0;			// �o�ߎ���
	m_dwStopStartTime	= 0;			// ��~�J�n����
	m_dwStopTime		= 0;			// ��~����
	m_dwTempTime		= 0;			// �o�ߎ��Ԃ̌v�Z�p
	m_state				= STATE_NONE;	// �v�����
	m_bStop				= true;			// �v����~��
	m_nLimit			= 0;			// ��������

	for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{ // �^�C�}�[�̌������J��Ԃ�

		// �����̐���
		m_apValue[nCntTimer] = CValue::Create(CValue::TEXTURE_NORMAL);
		if (m_apValue[nCntTimer] == NULL)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �D�揇�ʂ�ݒ�
		m_apValue[nCntTimer]->SetPriority(TIMER_PRIO);
	}

	for (int nCntTimer = 0; nCntTimer < MAX_PART; nCntTimer++)
	{ // ��؂�̐����J��Ԃ�

		// ��؂�̐���
		m_apPart[nCntTimer] = CObject2D::Create(VEC3_ZERO);
		if (m_apPart[nCntTimer] == NULL)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �e�N�X�`����o�^�E����
		m_apPart[nCntTimer]->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_NORMAL]));

		// �D�揇�ʂ�ݒ�
		m_apPart[nCntTimer]->SetPriority(TIMER_PRIO);
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CTimerManager::Uninit(void)
{
	// �����I�u�W�F�N�g��j��
	for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{ // �^�C�}�[�̌������J��Ԃ�

		// �����̏I��
		m_apValue[nCntTimer]->Uninit();
	}

	// ��؂�I�u�W�F�N�g��j��
	for (int nCntTimer = 0; nCntTimer < MAX_PART; nCntTimer++)
	{ // ��؂�̐����J��Ԃ�

		// ��؂�̏I��
		m_apPart[nCntTimer]->Uninit();
	}
}

//============================================================
//	�X�V����
//============================================================
void CTimerManager::Update(void)
{
	switch (m_state)
	{ // �v����Ԃ��Ƃ̏���
	case STATE_NONE:

		// ����

		break;

	case STATE_MEASURE:

		if (m_bStop == false)
		{ // �v�����̏ꍇ

			if (m_nLimit <= 0)
			{ // �������Ԃ� 0�ȉ��̏ꍇ

				// ���݂̌v���~���b��ݒ�
				m_dwTime = timeGetTime() - m_dwTempTime;
			}
			else
			{ // �������Ԃ� 0���傫���ꍇ

				// �ϐ���錾
				long nTime = m_nLimit - (timeGetTime() - m_dwTempTime);	// ���݃^�C��

				if (nTime > 0)
				{ // �^�C���� 0���傫���ꍇ

					// ���݂̌v���~���b��ݒ�
					m_dwTime = nTime;
				}
				else
				{  // �^�C���� 0�ȉ��̏ꍇ

					// �|�C���^��錾
					CPlayer *pPlayer = CScene::GetPlayer();	// �v���C���[���
					if (pPlayer == NULL)
					{ // �v���C���[�����݂��Ȃ��ꍇ

						// �����𔲂���
						assert(false);
						return;
					}

					// ���݂̌v���~���b��ݒ�
					m_dwTime = 0;

					// �v�����I������
					End();

					// �v���C���[���_���[�W��Ԃɂ���
					pPlayer->SetState(CPlayer::STATE_DAMAGE);
				}
			}
		}
		else
		{ // �v����~���̏ꍇ

			// ���݂̒�~�~���b��ݒ�
			m_dwStopTime = timeGetTime() - m_dwStopStartTime;
		}

		break;

	case STATE_END:

		// ����

		break;

	default:	// ��O����
		assert(false);
		break;
	}

	// �����̃e�N�X�`�����W�̐ݒ�
	SetTexNum();

	for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{ // �^�C�}�[�̌������J��Ԃ�

		// �����̍X�V
		m_apValue[nCntTimer]->Update();
	}

	for (int nCntTimer = 0; nCntTimer < MAX_PART; nCntTimer++)
	{ // ��؂�̐����J��Ԃ�

		// ��؂�̍X�V
		m_apPart[nCntTimer]->Update();
	}

	// �f�o�b�O�\��
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "�^�C�}�[�F[%d:%d:%d]\n", m_dwTime / 60000, (m_dwTime / 1000) % 60, m_dwTime % 1000);
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "��~�^�C�}�[�F[%d:%d:%d]\n", m_dwStopTime / 60000, (m_dwStopTime / 1000) % 60, m_dwStopTime % 1000);
}

//============================================================
//	��������
//============================================================
CTimerManager *CTimerManager::Create
(
	const TIME time,				// �ݒ�^�C��
	const long nTime,				// ��������
	const D3DXVECTOR3& rPos,		// �ʒu
	const D3DXVECTOR3& rSizeValue,	// �����̑傫��
	const D3DXVECTOR3& rSizePart,	// ��؂�̑傫��
	const D3DXVECTOR3& rSpaceValue,	// �����̋�
	const D3DXVECTOR3& rSpacePart	// ��؂�̋�
)
{
	// �|�C���^��錾
	CTimerManager *pTimerManager = NULL;	// �^�C�}�[�}�l�[�W���[�����p

	if (pTimerManager == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pTimerManager = new CTimerManager;	// �^�C�}�[�}�l�[�W���[
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pTimerManager != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// �^�C�}�[�}�l�[�W���[�̏�����
		if (FAILED(pTimerManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pTimerManager;
			pTimerManager = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �������Ԃ�ݒ�
		pTimerManager->SetLimit(time, nTime);

		// �ʒu��ݒ�
		pTimerManager->SetPosition(rPos);

		// �����̑傫����ݒ�
		pTimerManager->SetScalingValue(rSizeValue);

		// ��؂�̑傫����ݒ�
		pTimerManager->SetScalingPart(rSizePart);

		// �����̋󔒂�ݒ�
		pTimerManager->SetSpaceValue(rSpaceValue);

		// ��؂�̋󔒂�ݒ�
		pTimerManager->SetSpacePart(rSpacePart);

		// �m�ۂ����A�h���X��Ԃ�
		return pTimerManager;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
HRESULT CTimerManager::Release(CTimerManager *&prTimerManager)
{
	if (prTimerManager != NULL)
	{ // �g�p���̏ꍇ

		// �^�C�}�[�}�l�[�W���[�̏I��
		prTimerManager->Uninit();

		// �������J��
		delete prTimerManager;
		prTimerManager = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}

//============================================================
//	�ŏ��^�C���擾����
//============================================================
DWORD CTimerManager::GetMinTime(void)
{
	// �ŏ��^�C����Ԃ�
	return TIME_NUMMIN;
}

//============================================================
//	�ő�^�C���擾����
//============================================================
DWORD CTimerManager::GetMaxTime(void)
{
	// �ő�^�C����Ԃ�
	return TIME_NUMMAX;
}

//============================================================
//	�v���J�n����
//============================================================
void CTimerManager::Start(void)
{
	if (m_state != STATE_MEASURE)
	{ // �^�C���̌v�����ł͂Ȃ��ꍇ

		// �J�n�������擾
		m_dwStartTime = timeGetTime();
		m_dwTempTime  = m_dwStartTime;

		// ���~��Ԃɂ���
		EnableStop(false);

		// �v���J�n��Ԃɂ���
		m_state = STATE_MEASURE;
	}
}

//============================================================
//	�v���I������
//============================================================
void CTimerManager::End(void)
{
	// �v���I����Ԃɂ���
	m_state = STATE_END;
}

//============================================================
//	�v����~�̗L�������̐ݒ菈��
//============================================================
void CTimerManager::EnableStop(const bool bStop)
{
	// �����̒�~�󋵂���
	m_bStop = bStop;

	if (bStop)
	{ // ��~����ꍇ

		// ��~�J�n�������擾
		m_dwStopStartTime = timeGetTime();
	}
	else
	{ // �ĊJ����ꍇ

		// ��~���Ԃ����Z
		m_dwTempTime += m_dwStopTime;

		// ��~�֘A�̎��Ԃ�������
		m_dwStopStartTime = 0;	// ��~�J�n����
		m_dwStopTime = 0;		// ��~����
	}
}

//============================================================
//	�v����Ԏ擾����
//============================================================
CTimerManager::STATE CTimerManager::GetState(void)
{
	// �v����Ԃ�Ԃ�
	return m_state;
}

//============================================================
//	�~���b�̉��Z����
//============================================================
bool CTimerManager::AddMSec(long nMSec)
{
	// �ϐ���錾
	bool bAdd = true;	// ���Z��

	if (m_state == STATE_MEASURE)
	{ // �^�C���̌v�����̏ꍇ

		if (m_bStop)
		{ // ��~���̏ꍇ

			// ���Z�ł��Ȃ���Ԃɂ���
			bAdd = false;
		}

		if (m_nLimit > 0)
		{ // �������Ԃ� 0���傫���ꍇ

			// ���Z�ł��Ȃ���Ԃɂ���
			bAdd = false;
		}
	}

	if (bAdd)
	{ // ���Z�ł���ꍇ

		// ���Z�ʂ̕␳
		useful::LimitNum(nMSec, -(long)m_dwTime, (long)TIME_NUMMAX);

		// �����̃~���b�����Z
		m_dwTempTime -= (DWORD)nMSec;

		// ���݂̌v���~���b��ݒ�
		m_dwTime = timeGetTime() - m_dwTempTime;

		// �����̃e�N�X�`�����W�̐ݒ�
		SetTexNum();
	}

	// ���Z�̐������s��Ԃ�
	return bAdd;
}

//============================================================
//	�b�̉��Z����
//============================================================
bool CTimerManager::AddSec(long nSec)
{
	// �ϐ���錾
	bool bAdd = true;	// ���Z��

	if (m_state == STATE_MEASURE)
	{ // �^�C���̌v�����̏ꍇ

		if (m_bStop)
		{ // ��~���̏ꍇ

			// ���Z�ł��Ȃ���Ԃɂ���
			bAdd = false;
		}

		if (m_nLimit > 0)
		{ // �������Ԃ� 0���傫���ꍇ

			// ���Z�ł��Ȃ���Ԃɂ���
			bAdd = false;
		}
	}

	if (bAdd)
	{ // ���Z�ł���ꍇ

		// �������~���b�ɕϊ�
		nSec *= 1000;

		// ���Z�ʂ̕␳
		useful::LimitNum(nSec, -(long)m_dwTime, (long)TIME_NUMMAX);

		// �����̕b�����Z
		m_dwTempTime -= (DWORD)nSec;

		// ���݂̌v���~���b��ݒ�
		m_dwTime = timeGetTime() - m_dwTempTime;

		// �����̃e�N�X�`�����W�̐ݒ�
		SetTexNum();
	}

	// ���Z�̐������s��Ԃ�
	return bAdd;
}

//============================================================
//	���̉��Z����
//============================================================
bool CTimerManager::AddMin(long nMin)
{
	// �ϐ���錾
	bool bAdd = true;	// ���Z��

	if (m_state == STATE_MEASURE)
	{ // �^�C���̌v�����̏ꍇ

		if (m_bStop)
		{ // ��~���̏ꍇ

			// ���Z�ł��Ȃ���Ԃɂ���
			bAdd = false;
		}

		if (m_nLimit > 0)
		{ // �������Ԃ� 0���傫���ꍇ

			// ���Z�ł��Ȃ���Ԃɂ���
			bAdd = false;
		}
	}

	if (bAdd)
	{ // ���Z�ł���ꍇ

		// �������~���b�ɕϊ�
		nMin *= 60000;

		// ���Z�ʂ̕␳
		useful::LimitNum(nMin, -(long)m_dwTime, (long)TIME_NUMMAX);

		// �����̕������Z
		m_dwTempTime -= (DWORD)nMin;

		// ���݂̌v���~���b��ݒ�
		m_dwTime = timeGetTime() - m_dwTempTime;

		// �����̃e�N�X�`�����W�̐ݒ�
		SetTexNum();
	}

	// ���Z�̐������s��Ԃ�
	return bAdd;
}

//============================================================
//	�~���b�̐ݒ菈��
//============================================================
bool CTimerManager::SetMSec(long nMSec)
{
	// �ϐ���錾
	bool bSet = true;	// �ݒ��

	if (m_state == STATE_MEASURE)
	{ // �^�C���̌v�����̏ꍇ

		if (m_bStop)
		{ // ��~���̏ꍇ

			// �ݒ�ł��Ȃ���Ԃɂ���
			bSet = false;
		}

		if (m_nLimit > 0)
		{ // �������Ԃ� 0���傫���ꍇ

			// �ݒ�ł��Ȃ���Ԃɂ���
			bSet = false;
		}
	}

	if (bSet)
	{ // �ݒ�ł���ꍇ

		// ���Z�ʂ̕␳
		useful::LimitNum(nMSec, -(long)m_dwTime, (long)TIME_NUMMAX);

		// ������ݒ�
		m_dwStartTime = timeGetTime() - nMSec;
		m_dwTempTime = m_dwStartTime;

		// ���݂̌v���~���b��ݒ�
		m_dwTime = timeGetTime() - m_dwTempTime;

		// �l��������
		m_dwStopStartTime	= 0;	// ��~�J�n����
		m_dwStopTime		= 0;	// ��~����

		// �����̃e�N�X�`�����W�̐ݒ�
		SetTexNum();
	}

	// �ݒ�̐������s��Ԃ�
	return bSet;
}

//============================================================
//	�b�̐ݒ菈��
//============================================================
bool CTimerManager::SetSec(long nSec)
{
	// �ϐ���錾
	bool bSet = true;	// �ݒ��

	if (m_state == STATE_MEASURE)
	{ // �^�C���̌v�����̏ꍇ

		if (m_bStop)
		{ // ��~���̏ꍇ

			// �ݒ�ł��Ȃ���Ԃɂ���
			bSet = false;
		}
		
		if (m_nLimit > 0)
		{ // �������Ԃ� 0���傫���ꍇ

			// �ݒ�ł��Ȃ���Ԃɂ���
			bSet = false;
		}
	}

	if (bSet)
	{ // �ݒ�ł���ꍇ

		// �������~���b�ɕϊ�
		nSec *= 1000;

		// ���Z�ʂ̕␳
		useful::LimitNum(nSec, -(long)m_dwTime, (long)TIME_NUMMAX);

		// ������ݒ�
		m_dwStartTime = timeGetTime() - nSec;
		m_dwTempTime = m_dwStartTime;

		// ���݂̌v���~���b��ݒ�
		m_dwTime = timeGetTime() - m_dwTempTime;

		// �l��������
		m_dwStopStartTime	= 0;	// ��~�J�n����
		m_dwStopTime		= 0;	// ��~����

		// �����̃e�N�X�`�����W�̐ݒ�
		SetTexNum();
	}

	// �ݒ�̐������s��Ԃ�
	return bSet;
}

//============================================================
//	���̐ݒ菈��
//============================================================
bool CTimerManager::SetMin(long nMin)
{
	// �ϐ���錾
	bool bSet = true;	// �ݒ��

	if (m_state == STATE_MEASURE)
	{ // �^�C���̌v�����̏ꍇ

		if (m_bStop)
		{ // ��~���̏ꍇ

			// �ݒ�ł��Ȃ���Ԃɂ���
			bSet = false;
		}

		if (m_nLimit > 0)
		{ // �������Ԃ� 0���傫���ꍇ

			// �ݒ�ł��Ȃ���Ԃɂ���
			bSet = false;
		}
	}

	if (bSet)
	{ // �ݒ�ł���ꍇ

		// �������~���b�ɕϊ�
		nMin *= 60000;

		// ���Z�ʂ̕␳
		useful::LimitNum(nMin, -(long)m_dwTime, (long)TIME_NUMMAX);

		// ������ݒ�
		m_dwStartTime = timeGetTime() - nMin;
		m_dwTempTime = m_dwStartTime;

		// ���݂̌v���~���b��ݒ�
		m_dwTime = timeGetTime() - m_dwTempTime;

		// �l��������
		m_dwStopStartTime	= 0;	// ��~�J�n����
		m_dwStopTime		= 0;	// ��~����

		// �����̃e�N�X�`�����W�̐ݒ�
		SetTexNum();
	}

	// �ݒ�̐������s��Ԃ�
	return bSet;
}

//============================================================
//	�^�C���̎擾����
//============================================================
int CTimerManager::Get(void)
{
	// �^�C����Ԃ�
	return m_dwTime;
}

//============================================================
//	�~���b�̎擾����
//============================================================
int CTimerManager::GetMSec(void)
{
	// �~���b��Ԃ�
	return m_dwTime % 1000;
}

//============================================================
//	�b�̎擾����
//============================================================
int CTimerManager::GetSec(void)
{
	// �b��Ԃ�
	return (m_dwTime / 1000) % 60;
}

//============================================================
//	���̎擾����
//============================================================
int CTimerManager::GetMin(void)
{
	// ����Ԃ�
	return m_dwTime / 60000;
}

//============================================================
//	�������Ԃ̎擾����
//============================================================
long CTimerManager::GetLimit(void)
{
	// �������Ԃ�Ԃ�
	return m_nLimit;
}

//============================================================
//	�������Ԃ̐ݒ菈��
//============================================================
void CTimerManager::SetLimit(const TIME time, const long nTime)
{
	// �ϐ���錾
	long nMSec = 0;	// �~���b�ϊ��p

	// �����̐������Ԃ�ݒ�
	switch (time)
	{ // �^�C�����Ƃ̏���
	case TIME_MSEC:	// �~���b

		m_nLimit = nTime;	// �������Ԃ�ݒ�

		break;

	case TIME_SEC:	// �b

		// �������~���b�ɕϊ�
		nMSec = nTime * 1000;
		m_nLimit = nMSec;	// �������Ԃ�ݒ�

		break;

	case TIME_MIN:	// ��

		// �������~���b�ɕϊ�
		nMSec = nTime * 60000;
		m_nLimit = nMSec;	// �������Ԃ�ݒ�

		break;

	default:
		assert(false);
		break;
	}
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CTimerManager::SetPosition(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_pos = rPos;

	// �����̕\���ݒ�
	SetDrawValue();
}

//============================================================
//	�����̑傫���̐ݒ菈��
//============================================================
void CTimerManager::SetScalingValue(const D3DXVECTOR3& rSize)
{
	// �����̐����̑傫����ݒ�
	m_sizeValue = rSize;

	// �����̕\���ݒ�
	SetDrawValue();
}

//============================================================
//	��؂�̑傫���̐ݒ菈��
//============================================================
void CTimerManager::SetScalingPart(const D3DXVECTOR3& rSize)
{
	// �����̋�؂�̑傫����ݒ�
	m_sizePart = rSize;

	// �����̕\���ݒ�
	SetDrawValue();
}

//============================================================
//	�����̋󔒂̐ݒ菈��
//============================================================
void CTimerManager::SetSpaceValue(const D3DXVECTOR3& rSpace)
{
	// �����̐����̋󔒂�ݒ�
	m_spaceValue = rSpace;

	// �����̕\���ݒ�
	SetDrawValue();
}

//============================================================
//	��؂�̋󔒂̐ݒ菈��
//============================================================
void CTimerManager::SetSpacePart(const D3DXVECTOR3& rSpace)
{
	// �����̋�؂�̋󔒂�ݒ�
	m_spacePart = rSpace;

	// �����̕\���ݒ�
	SetDrawValue();
}

//============================================================
//	�D�揇�ʂ̐ݒ菈��
//============================================================
void CTimerManager::SetPriority(const int nPriority)
{
	if (m_apValue[0] != NULL)
	{ // �X�R�A�̐擪�̐��l���g�p����Ă���ꍇ

		// �����I�u�W�F�N�g�̗D�揇�ʂ�ݒ�
		for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
		{ // �^�C�}�[�̌������J��Ԃ�

			m_apValue[nCntTimer]->SetPriority(nPriority);
		}

		// ��؂�I�u�W�F�N�g�̗D�揇�ʂ�ݒ�
		for (int nCntTimer = 0; nCntTimer < MAX_PART; nCntTimer++)
		{ // ��؂�̐����J��Ԃ�

			m_apPart[nCntTimer]->SetPriority(nPriority);
		}
	}
	else { assert(false); }	// ��g�p��
}

//============================================================
//	�X�V�󋵂̐ݒ菈��
//============================================================
void CTimerManager::SetEnableUpdate(const bool bUpdate)
{
	if (m_apValue[0] != NULL)
	{ // �X�R�A�̐擪�̐��l���g�p����Ă���ꍇ

		// �����I�u�W�F�N�g�̍X�V�󋵂�ݒ�
		for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
		{ // �^�C�}�[�̌������J��Ԃ�

			m_apValue[nCntTimer]->SetEnableUpdate(bUpdate);
		}

		// ��؂�I�u�W�F�N�g�̍X�V�󋵂�ݒ�
		for (int nCntTimer = 0; nCntTimer < MAX_PART; nCntTimer++)
		{ // ��؂�̐����J��Ԃ�

			m_apPart[nCntTimer]->SetEnableUpdate(bUpdate);
		}
	}
	else { assert(false); }	// ��g�p��
}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CTimerManager::SetEnableDraw(const bool bDraw)
{
	if (m_apValue[0] != NULL)
	{ // �X�R�A�̐擪�̐��l���g�p����Ă���ꍇ

		// �����I�u�W�F�N�g�̕`��󋵂�ݒ�
		for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
		{ // �^�C�}�[�̌������J��Ԃ�

			m_apValue[nCntTimer]->SetEnableDraw(bDraw);
		}

		// ��؂�I�u�W�F�N�g�̕`��󋵂�ݒ�
		for (int nCntTimer = 0; nCntTimer < MAX_PART; nCntTimer++)
		{ // ��؂�̐����J��Ԃ�

			m_apPart[nCntTimer]->SetEnableDraw(bDraw);
		}
	}
	else { assert(false); }	// ��g�p��
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CTimerManager::GetPosition(void) const
{
	// �ʒu��Ԃ�
	return m_pos;
}

//============================================================
//	�����̑傫���擾����
//============================================================
D3DXVECTOR3 CTimerManager::GetScalingValue(void) const
{
	// �����̑傫����Ԃ�
	return m_sizeValue;
}

//============================================================
//	��؂�̑傫���擾����
//============================================================
D3DXVECTOR3 CTimerManager::GetScalingPart(void) const
{
	// ��؂�̑傫����Ԃ�
	return m_sizePart;
}

//============================================================
//	�����̋󔒎擾����
//============================================================
D3DXVECTOR3 CTimerManager::GetSpaceValue(void) const
{
	// �����̋󔒂�Ԃ�
	return m_spaceValue;
}

//============================================================
//	��؂�̋󔒎擾����
//============================================================
D3DXVECTOR3 CTimerManager::GetSpacePart(void) const
{
	// ��؂�̋󔒂�Ԃ�
	return m_spacePart;
}

//============================================================
//	�����̕\���ݒ菈��
//============================================================
void CTimerManager::SetDrawValue(void)
{
	// �ϐ���錾
	D3DXVECTOR3 spaceValue = m_spaceValue * 0.5f;	// �����̋�
	D3DXVECTOR3 spacePart = m_spacePart * 0.5f;		// ��؂�̋�
	D3DXVECTOR3 posPoly = m_pos - spaceValue;		// �|���S�������ʒu
	int nNumValue = 0;	// �����̐�����
	int nNumPart = 0;	// ��؂�̐�����

	if (m_apValue[0] != NULL && m_apPart[0] != NULL)
	{ // �^�C�}�[�̐擪���l�A�^�C�}�[�̐擪��؂肪�g�p����Ă���ꍇ

		for (int nCntTimer = 0; nCntTimer < MAX_TIMER + MAX_PART; nCntTimer++)
		{ // �^�C�}�[�̌��� + �^�C�}�[�̋�؂萔���J��Ԃ�

			if (nCntTimer == MAX_MIN || nCntTimer == MAX_MIN + MAX_SEC + 1)
			{ // ��؂�̐����^�C�~���O�������ꍇ

				// �|���S�������ʒu�����炷
				posPoly += spacePart;

				// ��؂�̈ʒu��ݒ�
				m_apPart[nNumValue]->SetVec3Position(posPoly);

				// ��؂�̑傫����ݒ�
				m_apPart[nNumValue]->SetVec3Sizing(m_sizePart);

				// ��؂萶���������Z
				nNumValue++;

				// �|���S�������ʒu�����炷
				posPoly += spacePart;
			}
			else
			{ // ��؂�̐����^�C�~���O�ł͂Ȃ��ꍇ

				// �|���S�������ʒu�����炷
				posPoly += spaceValue;

				// �����̈ʒu��ݒ�
				m_apValue[nNumPart]->SetVec3Position(posPoly);

				// �����̑傫����ݒ�
				m_apValue[nNumPart]->SetVec3Sizing(m_sizeValue);

				// ���������������Z
				nNumPart++;

				// �|���S�������ʒu�����炷
				posPoly += spaceValue;
			}
		}
	}
	else { assert(false); }	// ��g�p��
}

//============================================================
//	�����̃e�N�X�`�����W�̐ݒ菈��
//============================================================
void CTimerManager::SetTexNum(void)
{
	// �ϐ���錾
	int aNumDivide[MAX_TIMER];	// ���̌������Ƃ̕���p

	// �����������Ƃɕ���
	useful::DivideDigitNum(&aNumDivide[0], GetMin(), MAX_MIN);

	// �b���������Ƃɕ���
	useful::DivideDigitNum(&aNumDivide[MAX_MIN], GetSec(), MAX_SEC);

	// �~���b���������Ƃɕ���
	useful::DivideDigitNum(&aNumDivide[MAX_MIN + MAX_SEC], GetMSec(), MAX_MSEC);

	for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{ // �^�C�}�[�̌������J��Ԃ�

		// �����̐ݒ�
		m_apValue[nCntTimer]->SetNumber(aNumDivide[nCntTimer]);
	}
}
