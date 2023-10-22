//============================================================
//
//	�Q�[����ʏ��� [sceneGame.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "sceneGame.h"
#include "manager.h"
#include "sound.h"
#include "input.h"

#include "gameManager.h"
#include "editStageManager.h"
#include "timerManager.h"
#include "stage.h"
#include "pause.h"
#include "score.h"
#include "player.h"

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
#define TIME_POS		(D3DXVECTOR3(40.0f, 50.0f, 0.0f))			// �^�C�}�[�ʒu
#define TIME_VAL_SIZE	(D3DXVECTOR3(60.0f, 80.0f, 0.0f))			// �^�C�}�[�����傫��
#define TIME_PART_SIZE	(D3DXVECTOR3(35.0f, 80.0f, 0.0f))			// �^�C�}�[��؂�傫��
#define TIME_VAL_SPACE	(D3DXVECTOR3(TIME_VAL_SIZE.x, 0.0f, 0.0f))	// �^�C�}�[������
#define TIME_PART_SPACE	(D3DXVECTOR3(TIME_PART_SIZE.x, 0.0f, 0.0f))	// �^�C�}�[��؂��

#define SCO_POS		(D3DXVECTOR3(825.0f, 50.0f, 0.0f))		// �X�R�A�ʒu
#define SCO_SIZE	(D3DXVECTOR3(60.0f, 80.0f, 0.0f))		// �X�R�A�傫��
#define SCO_SPACE	(D3DXVECTOR3(SCO_SIZE.x, 0.0f, 0.0f))	// �X�R�A��

#define WARNING_SIZE	(D3DXVECTOR3(1000.0f, 500.0f, 0.0f))	// �x���傫��

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CGameManager	*CSceneGame::m_pGameManager  = NULL;	// �Q�[���}�l�[�W���[
CTimerManager	*CSceneGame::m_pTimerManager = NULL;	// �^�C�}�[�}�l�[�W���[
CWarningSpawn	*CSceneGame::m_pWarningSpawn = NULL;	// �o���x���\���I�u�W�F�N�g
CPause	*CSceneGame::m_pPause	= NULL;					// �|�[�Y
CScore	*CSceneGame::m_pScore	= NULL;					// �X�R�A�I�u�W�F�N�g

bool CSceneGame::m_bControlCamera = false;	// �J�����̑����
bool CSceneGame::m_bDrawUI = true;			// UI�̕`���
bool CSceneGame::m_bDrawPause = true;		// �|�[�Y�̕`���

//************************************************************
//	�q�N���X [CSceneGame] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CSceneGame::CSceneGame(const EMode mode) : CScene(mode)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CSceneGame::~CSceneGame()
{

}

//============================================================
//	����������
//============================================================
HRESULT CSceneGame::Init(void)
{
	// �^�C�}�[�}�l�[�W���[�̐���
	m_pTimerManager = CTimerManager::Create
	( // ����
		TIME_POS,		// �ʒu
		TIME_VAL_SIZE,	// �����̑傫��
		TIME_PART_SIZE,	// ��؂�̑傫��
		TIME_VAL_SPACE,	// �����̋�
		TIME_PART_SPACE	// ��؂�̋�
	);
	if (m_pTimerManager == NULL)
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �X�R�A�I�u�W�F�N�g�̐���
	m_pScore = CScore::Create
	( // ����
		SCO_POS,	// �ʒu
		SCO_SIZE,	// �傫��
		SCO_SPACE	// ��
	);
	if (m_pScore == NULL)
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �V�[���̏�����
	CScene::Init();		// �X�e�[�W�E�n�ʁE�v���C���[�̐���

	// �Q�[���}�l�[�W���[�̐���
	m_pGameManager = CGameManager::Create();
	if (m_pGameManager == NULL)
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �|�[�Y�̐���
	m_pPause = CPause::Create();
	if (m_pPause == NULL)
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �i�F�I�u�W�F�N�g�̐���
	CScenery::Create(CScenery::TEXTURE_BILL_WHITE,     VEC3_ZERO, VEC3_ZERO,                                    XCOL_WHITE, POSGRID2(14, 1), 12000.0f, 800.0f,  D3DCULL_CW, false);
	CScenery::Create(CScenery::TEXTURE_BILL_LIGHTBLUE, VEC3_ZERO, D3DXToRadian(D3DXVECTOR3(0.0f, 85.0f, 0.0f)), XCOL_WHITE, POSGRID2(14, 1), 14000.0f, 1400.0f, D3DCULL_CW, false);
	CScenery::Create(CScenery::TEXTURE_BILL_BLUE,      VEC3_ZERO, D3DXToRadian(D3DXVECTOR3(0.0f, 35.0f, 0.0f)), XCOL_WHITE, POSGRID2(14, 1), 16000.0f, 2000.0f, D3DCULL_CW, false);

	// ��I�u�W�F�N�g�̐���
	CSky::Create(CSky::TEXTURE_NORMAL, VEC3_ZERO, VEC3_ZERO, XCOL_WHITE, POSGRID2(32, 6), 18000.0f, D3DCULL_CW, false);

#if 0

#if 0	// TODO�F�r��

	// �r���I�u�W�F�N�g�̐���	// TODO�F��ނ̐ݒ�ňʒu�ƌ����ȊO�̏��͌��܂�悤�ɕύX
	CBuilding::Create(CBuilding::TYPE_00, D3DXVECTOR3(0.0f, 0.0f, 560.0f * 7.25f), D3DXToRadian(D3DXVECTOR3(0.0f, 180.0f, 0.0f)), D3DXVECTOR3(280.0f, 560.0f, 280.0f), XCOL_WHITE, CBuilding::COLLISION_GROUND);
	CBuilding::Create(CBuilding::TYPE_00, D3DXVECTOR3(0.0f, 0.0f, 560.0f * 6.25f), D3DXToRadian(D3DXVECTOR3(0.0f, 180.0f, 0.0f)), D3DXVECTOR3(280.0f, 560.0f, 280.0f), XCOL_WHITE, CBuilding::COLLISION_GROUND);
	CBuilding::Create(CBuilding::TYPE_00, D3DXVECTOR3(0.0f, 0.0f, 560.0f * 4.5f), D3DXToRadian(D3DXVECTOR3(0.0f, 180.0f, 0.0f)), D3DXVECTOR3(280.0f, 560.0f, 280.0f), XCOL_WHITE, CBuilding::COLLISION_GROUND);
	CBuilding::Create(CBuilding::TYPE_00, D3DXVECTOR3(0.0f, 0.0f, 560.0f * 3.5f), D3DXToRadian(D3DXVECTOR3(0.0f, 180.0f, 0.0f)), D3DXVECTOR3(280.0f, 560.0f, 280.0f), XCOL_WHITE, CBuilding::COLLISION_GROUND);
	CBuilding::Create(CBuilding::TYPE_00, D3DXVECTOR3(0.0f, 0.0f, 560.0f * 2.0f), D3DXToRadian(D3DXVECTOR3(0.0f, 180.0f, 0.0f)), D3DXVECTOR3(280.0f, 560.0f, 280.0f), XCOL_WHITE, CBuilding::COLLISION_GROUND);
	CBuilding::Create(CBuilding::TYPE_00, D3DXVECTOR3(0.0f, 0.0f, 560.0f * 1.0f), D3DXToRadian(D3DXVECTOR3(0.0f, 180.0f, 0.0f)), D3DXVECTOR3(280.0f, 560.0f, 280.0f), XCOL_WHITE, CBuilding::COLLISION_GROUND);
	CBuilding::Create(CBuilding::TYPE_00, D3DXVECTOR3(0.0f, 0.0f, 560.0f * 0.0f), D3DXToRadian(D3DXVECTOR3(0.0f, 180.0f, 0.0f)), D3DXVECTOR3(280.0f, 560.0f, 280.0f), XCOL_WHITE, CBuilding::COLLISION_GROUND);

	CBuilding::Create(CBuilding::TYPE_01, D3DXVECTOR3(-560.0f,  0.0f, 0.0f), D3DXToRadian(D3DXVECTOR3(0.0f, 270.0f, 0.0f)), D3DXVECTOR3(280.0f, 560.0f, 280.0f), XCOL_WHITE, CBuilding::COLLISION_GROUND);
	CBuilding::Create(CBuilding::TYPE_02, D3DXVECTOR3(-1120.0f, 0.0f, 0.0f), D3DXToRadian(D3DXVECTOR3(0.0f, 270.0f, 0.0f)), D3DXVECTOR3(280.0f, 560.0f, 280.0f), XCOL_WHITE, CBuilding::COLLISION_GROUND);
	CBuilding::Create(CBuilding::TYPE_00, D3DXVECTOR3(-1680.0f, 0.0f, 0.0f), D3DXToRadian(D3DXVECTOR3(0.0f, 270.0f, 0.0f)), D3DXVECTOR3(280.0f, 560.0f, 280.0f), XCOL_WHITE, CBuilding::COLLISION_GROUND);
	CBuilding::Create(CBuilding::TYPE_01, D3DXVECTOR3(-2240.0f, 0.0f, 0.0f), D3DXToRadian(D3DXVECTOR3(0.0f, 270.0f, 0.0f)), D3DXVECTOR3(280.0f, 560.0f, 280.0f), XCOL_WHITE, CBuilding::COLLISION_GROUND);
	CBuilding::Create(CBuilding::TYPE_02, D3DXVECTOR3(-2800.0f, 0.0f, 0.0f), D3DXToRadian(D3DXVECTOR3(0.0f, 270.0f, 0.0f)), D3DXVECTOR3(280.0f, 560.0f, 280.0f), XCOL_WHITE, CBuilding::COLLISION_GROUND);

#else

	CBuilding::Create(CBuilding::TYPE_01, D3DXVECTOR3(-2800.0f, 0.0f, 560.0f),  D3DXToRadian(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),   D3DXVECTOR3(280.0f, 560.0f, 280.0f), XCOL_WHITE, CBuilding::COLLISION_GROUND);
	CBuilding::Create(CBuilding::TYPE_01, D3DXVECTOR3(-2800.0f, 0.0f, 1120.0f), D3DXToRadian(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),   D3DXVECTOR3(280.0f, 560.0f, 280.0f), XCOL_WHITE, CBuilding::COLLISION_GROUND);
	CBuilding::Create(CBuilding::TYPE_02, D3DXVECTOR3(-1680.0f, 0.0f, 1120.0f), D3DXToRadian(D3DXVECTOR3(0.0f, 90.0f, 0.0f)),  D3DXVECTOR3(280.0f, 560.0f, 280.0f), XCOL_WHITE, CBuilding::COLLISION_GROUND);
	CBuilding::Create(CBuilding::TYPE_02, D3DXVECTOR3(-2240.0f, 0.0f, 1120.0f), D3DXToRadian(D3DXVECTOR3(0.0f, 90.0f, 0.0f)),  D3DXVECTOR3(280.0f, 560.0f, 280.0f), XCOL_WHITE, CBuilding::COLLISION_GROUND);
	CBuilding::Create(CBuilding::TYPE_00, D3DXVECTOR3(-1680.0f, 0.0f, 560.0f),  D3DXToRadian(D3DXVECTOR3(0.0f, 180.0f, 0.0f)), D3DXVECTOR3(280.0f, 560.0f, 280.0f), XCOL_WHITE, CBuilding::COLLISION_GROUND);
	CBuilding::Create(CBuilding::TYPE_00, D3DXVECTOR3(-1680.0f, 0.0f, 0.0f),    D3DXToRadian(D3DXVECTOR3(0.0f, 180.0f, 0.0f)), D3DXVECTOR3(280.0f, 560.0f, 280.0f), XCOL_WHITE, CBuilding::COLLISION_GROUND);
	CBuilding::Create(CBuilding::TYPE_02, D3DXVECTOR3(-2800.0f, 0.0f, 0.0f),    D3DXToRadian(D3DXVECTOR3(0.0f, 270.0f, 0.0f)), D3DXVECTOR3(280.0f, 560.0f, 280.0f), XCOL_WHITE, CBuilding::COLLISION_GROUND);
	CBuilding::Create(CBuilding::TYPE_02, D3DXVECTOR3(-2240.0f, 0.0f, 0.0f),    D3DXToRadian(D3DXVECTOR3(0.0f, 270.0f, 0.0f)), D3DXVECTOR3(280.0f, 560.0f, 280.0f), XCOL_WHITE, CBuilding::COLLISION_GROUND);

#endif

#if 0	// TODO�F�Z�[�u�|�C���g

	// ��Q���I�u�W�F�N�g�̐���
	//CObstacle::Create(CObstacle::TYPE_BOX,   D3DXVECTOR3(-0.0f,    1120.0f, 0.0f), D3DXToRadian(D3DXVECTOR3(0.0f, 90.0f, 0.0f)));
	CObstacle::Create(CObstacle::TYPE_BENCH, D3DXVECTOR3(-560.0f,  1120.0f, 0.0f), D3DXToRadian(D3DXVECTOR3(0.0f, 90.0f, 0.0f)));
	CObstacle::Create(CObstacle::TYPE_BOX,   D3DXVECTOR3(-1120.0f, 1120.0f, 0.0f), D3DXToRadian(D3DXVECTOR3(0.0f, 90.0f, 0.0f)));
	CObstacle::Create(CObstacle::TYPE_BENCH, D3DXVECTOR3(-1680.0f, 1120.0f, 0.0f), D3DXToRadian(D3DXVECTOR3(0.0f, 90.0f, 0.0f)));
	CObstacle::Create(CObstacle::TYPE_BOX,   D3DXVECTOR3(-2240.0f, 1120.0f, 0.0f), D3DXToRadian(D3DXVECTOR3(0.0f, 90.0f, 0.0f)));
	//CObstacle::Create(CObstacle::TYPE_BENCH, D3DXVECTOR3(-2800.0f, 1120.0f, 0.0f), D3DXToRadian(D3DXVECTOR3(0.0f, 90.0f, 0.0f)));

#else

	// ��Q���I�u�W�F�N�g�̐���
	CObstacle::Create(CObstacle::TYPE_BOX,   D3DXVECTOR3(-2800.0f, 1120.0f, 560.0f),  D3DXToRadian(D3DXVECTOR3(0.0f, 0.0f, 0.0f))  );
	CObstacle::Create(CObstacle::TYPE_BOX,   D3DXVECTOR3(-2240.0f, 1120.0f, 1120.0f), D3DXToRadian(D3DXVECTOR3(0.0f, 90.0f, 0.0f)) );
	CObstacle::Create(CObstacle::TYPE_BENCH, D3DXVECTOR3(-1680.0f, 1120.0f, 560.0f),  D3DXToRadian(D3DXVECTOR3(0.0f, 180.0f, 0.0f)));
	CObstacle::Create(CObstacle::TYPE_BENCH, D3DXVECTOR3(-2240.0f, 1120.0f, 0.0f),    D3DXToRadian(D3DXVECTOR3(0.0f, 270.0f, 0.0f)));

#endif

	// �Ŕ̐���
	CSignboard::Create(CSignboard::TYPE_00, D3DXVECTOR3(-2240.0f, 1400.0f, 40.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1.0f);

#if 0	// TODO�F�Z�[�u�|�C���g

	// �Z�[�u�|�C���g�̐���
	CSavePoint::Create(D3DXVECTOR3(-2800.0f, 1120.0f, 0.0f), D3DXToRadian(D3DXVECTOR3(0.0f, 270.0f, 0.0f)));
	CSavePoint::Create(D3DXVECTOR3(0.0f, 1120.0f, 0.0f),     D3DXToRadian(D3DXVECTOR3(0.0f, 180.0f, 0.0f)));

#else

	// �Z�[�u�|�C���g�̐���
	CSavePoint::Create(D3DXVECTOR3(-2800.0f, 1120.0f, 1120.0f), D3DXToRadian(D3DXVECTOR3(0.0f, 0.0f, 0.0f))  );
	//CSavePoint::Create(D3DXVECTOR3(-1680.0f, 1120.0f, 1120.0f), D3DXToRadian(D3DXVECTOR3(0.0f, 90.0f, 0.0f)) );
	CSavePoint::Create(D3DXVECTOR3(-1680.0f, 1120.0f, 0.0f),    D3DXToRadian(D3DXVECTOR3(0.0f, 180.0f, 0.0f)));
	CSavePoint::Create(D3DXVECTOR3(-2800.0f, 1120.0f, 0.0f),    D3DXToRadian(D3DXVECTOR3(0.0f, 270.0f, 0.0f)));

#endif

#if 0	// TODO�F�S�[���|�C���g



#else

	// �S�[���|�C���g�̐���
	CGoalPoint::Create(D3DXVECTOR3(-1680.0f, 1120.0f, 1120.0f));

#endif

#endif

	//--------------------------------------------------------
	//	�����ݒ�
	//--------------------------------------------------------
	// �J������ݒ�
	CManager::GetInstance()->GetCamera()->SetState(CCamera::STATE_FOLLOW);	// �J������Ǐ]��Ԃɐݒ�

	// �v���C���[���o��
	CScene::GetPlayer()->SetSpawn();

	// �^�C�����v���J�n
	m_pTimerManager->Start();	// �v�����J�n

	// UI�̕`��󋵂�ݒ�
	SetEnableDrawUI(m_bDrawUI);

	// �|�[�Y�̕`��󋵂�ݒ�
	SetEnableDrawPause(m_bDrawPause);

	// BGM�̍Đ�
	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_GAME);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
HRESULT CSceneGame::Uninit(void)
{
	// �Q�[���}�l�[�W���[�̔j��
	if (FAILED(CGameManager::Release(m_pGameManager)))
	{ // �j���Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �^�C�}�[�}�l�[�W���[�̔j��
	if (FAILED(CTimerManager::Release(m_pTimerManager)))
	{ // �j���Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �|�[�Y�̔j��
	if (FAILED(CPause::Release(m_pPause)))
	{ // �j���Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �I���ς݂̃I�u�W�F�N�g�|�C���^��NULL�ɂ���
	m_pScore = NULL;	// �X�R�A�I�u�W�F�N�g

	// �V�[���̏I��
	CScene::Uninit();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�X�V����
//============================================================
void CSceneGame::Update(void)
{
#if _DEBUG

	if (CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_F2))
	{
		// �G�f�B�b�g�󋵂𔽓]
		CGameManager::GetEditStage()->SetEnableEdit((!CGameManager::GetEditStage()->IsEdit()) ? true : false);
	}
	if (CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_F3))
	{
		// UI�̕`��󋵂𔽓]
		SetEnableDrawUI((!m_bDrawUI) ? true : false);
	}
	else if (CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_F4))
	{
		// �|�[�Y�̕`��󋵂𔽓]
		SetEnableDrawPause((!m_bDrawPause) ? true : false);
	}
	else if (CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_F5))
	{
		// �J�����̑���󋵂𔽓]
		SetEnableControlCamera((!m_bControlCamera) ? true : false);
	}
	else if (CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_F6))
	{
		// �v���C���[�̏o����ݒ�
		CScene::GetPlayer()->SetSpawn();
	}
	//else if (CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_F7))
	//{
	//	// ���U���g�ɑJ��
	//	CManager::GetInstance()->SetScene(CScene::MODE_RESULT);	// ���U���g���
	//}

	// �f�o�b�O�\��
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "======================================\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "�@[�f�o�b�O����]\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "======================================\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[F1]�F�f�o�b�O�\����ON/OFF\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[F2]�F�G�f�B�b�g���[�h��ON/OFF\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[F3]�FUI�`���ON/OFF\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[F4]�F�|�[�Y�`���ON/OFF\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[F5]�F�J���������ON/OFF\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[F6]�F�v���C���[�X�|�[��\n");
	//CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[F7]�F���U���g�J��\n");

	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "======================================\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "�@[�f�o�b�O���]\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "======================================\n");

#endif

	if (m_pTimerManager != NULL)
	{ // �g�p���̏ꍇ

		// �^�C�}�[�}�l�[�W���[�̍X�V
		m_pTimerManager->Update();
	}
	else { assert(false); }	// ��g�p��

	if (m_pPause != NULL)
	{ // �g�p���̏ꍇ

		// �|�[�Y�̍X�V
		m_pPause->Update();
	}
	else { assert(false); }	// ��g�p��

	if (!m_pPause->IsPause())
	{ // �|�[�Y���ł͂Ȃ��ꍇ

		if (m_pGameManager != NULL)
		{ // �g�p���̏ꍇ

			// �Q�[���}�l�[�W���[�̍X�V
			m_pGameManager->Update();
		}
		else { assert(false); }	// ��g�p��

		// �V�[���̍X�V
		CScene::Update();
	}

#if _DEBUG

	else
	{ // �|�[�Y���̏ꍇ

		if (CManager::GetInstance()->GetCamera()->GetState() == CCamera::STATE_CONTROL)
		{ // �J�����������Ԃ̏ꍇ

			// �J�����̍X�V
			CManager::GetInstance()->GetCamera()->Update();
		}
	}

#endif
}

//============================================================
//	�`�揈��
//============================================================
void CSceneGame::Draw(void)
{

}

//============================================================
//	�Q�[���}�l�[�W���[�擾����
//============================================================
CGameManager *CSceneGame::GetGameManager(void)
{
	// �Q�[���}�l�[�W���[�̃|�C���^��Ԃ�
	return m_pGameManager;
}

//============================================================
//	�^�C�}�[�}�l�[�W���[�擾����
//============================================================
CTimerManager *CSceneGame::GetTimerManager(void)
{
	// �^�C�}�[�}�l�[�W���[�̃|�C���^��Ԃ�
	return m_pTimerManager;
}

//============================================================
//	�|�[�Y�擾����
//============================================================
CPause *CSceneGame::GetPause(void)
{
	// �|�[�Y�̃|�C���^��Ԃ�
	return m_pPause;
}

//============================================================
//	�X�R�A�擾����
//============================================================
CScore *CSceneGame::GetScore(void)
{
	// �X�R�A�̃|�C���^��Ԃ�
	return m_pScore;
}

//============================================================
//	�o���x���\���擾����
//============================================================
CWarningSpawn *CSceneGame::GetWarningSpawn(void)
{
	// �o���x���\���̃|�C���^��Ԃ�
	return m_pWarningSpawn;
}

//============================================================
//	UI�̕`��󋵂̐ݒ菈��
//============================================================
void CSceneGame::SetEnableDrawUI(const bool bDraw)
{
	// ������UI�̕`��󋵂�ݒ�
	m_bDrawUI = bDraw;

	// �^�C�}�[�̕`��󋵂�ݒ�
	m_pTimerManager->SetEnableDraw(bDraw);

	// �X�R�A�̕`��󋵂�ݒ�
	m_pScore->SetEnableDraw(bDraw);
}

//============================================================
//	UI�̕`��󋵎擾����
//============================================================
bool CSceneGame::IsDrawUI(void)
{
	// UI�̕`��󋵂�Ԃ�
	return m_bDrawUI;
}

//============================================================
//	�|�[�Y�̕`��󋵂̐ݒ菈��
//============================================================
void CSceneGame::SetEnableDrawPause(const bool bDraw)
{
	// �����̃|�[�Y�̕`��󋵂�ݒ�
	m_bDrawPause = bDraw;

	// �|�[�Y�̕`��󋵂�ݒ�
	m_pPause->SetEnableDraw(m_pPause->IsPause());
}

//============================================================
//	�|�[�Y�̕`��󋵎擾����
//============================================================
bool CSceneGame::IsDrawPause(void)
{
	// �|�[�Y�̕`��󋵂�Ԃ�
	return m_bDrawPause;
}

//============================================================
//	�J�����̑���󋵂̐ݒ菈��
//============================================================
void CSceneGame::SetEnableControlCamera(const bool bControl)
{
	// �����̃J��������󋵂�ݒ�
	m_bControlCamera = bControl;

	// �J�����̑���󋵂�ݒ�
	if (bControl)
	{ // ���삷��󋵂̏ꍇ

		// �����ԂɕύX
		CManager::GetInstance()->GetCamera()->SetState(CCamera::STATE_CONTROL);
	}
	else
	{ // ���삵�Ȃ��󋵂̏ꍇ

		// �Ǐ]��ԂɕύX
		CManager::GetInstance()->GetCamera()->SetState(CCamera::STATE_FOLLOW);
	}
}
