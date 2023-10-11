//============================================================
//
//	�Q�[����ʃw�b�_�[ [sceneGame.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SCENE_GAME_H_
#define _SCENE_GAME_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "scene.h"
#include "camera.h"

//************************************************************
//	�O���錾
//************************************************************
class CGameManager;		// �Q�[���}�l�[�W���[�N���X
class CTimerManager;	// �^�C�}�[�}�l�[�W���[�N���X
class CPause;			// �|�[�Y�N���X
class CScore;			// �X�R�A�N���X
class CWarningSpawn;	// �o���x���\���N���X

//************************************************************
//	�N���X��`
//************************************************************
// �Q�[����ʃN���X
class CSceneGame : public CScene
{
public:
	// �R���X�g���N�^
	CSceneGame(const EMode mode);

	// �f�X�g���N�^
	~CSceneGame();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	HRESULT Uninit(void) override;	// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��

	// �ÓI�����o�֐�
	static CGameManager		*GetGameManager(void);		// �Q�[���}�l�[�W���[�擾
	static CTimerManager	*GetTimerManager(void);		// �^�C�}�[�}�l�[�W���[�擾
	static CPause			*GetPause(void);			// �|�[�Y�擾
	static CScore			*GetScore(void);			// �X�R�A�擾
	static CWarningSpawn	*GetWarningSpawn(void);		// �o���x���\���擾

	static void SetEnableDrawUI(const bool bDraw);		// UI�̕`��󋵐ݒ�
	static bool IsDrawUI(void);							// UI�̕`��󋵎擾
	static void SetEnableDrawPause(const bool bDraw);	// �|�[�Y�̕`��󋵐ݒ�
	static bool IsDrawPause(void);						// �|�[�Y�̕`��󋵎擾
	static void SetEnableControlCamera(const bool bControl);	// �J�����̑���󋵐ݒ�

private:
	// �ÓI�����o�ϐ�
	static CGameManager		*m_pGameManager;	// �Q�[���}�l�[�W���[
	static CTimerManager	*m_pTimerManager;	// �^�C�}�[�}�l�[�W���[
	static CPause			*m_pPause;			// �|�[�Y
	static CScore			*m_pScore;			// �X�R�A�I�u�W�F�N�g
	static CWarningSpawn	*m_pWarningSpawn;	// �o���x���\���I�u�W�F�N�g

	static bool m_bControlCamera;	// �J�����̑����
	static bool m_bDrawUI;			// UI�̕`���
	static bool m_bDrawPause;		// �|�[�Y�̕`���
};

#endif	// _SCENE_GAME_H_
