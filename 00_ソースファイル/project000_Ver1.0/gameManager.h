//============================================================
//
//	�Q�[���}�l�[�W���[�w�b�_�[ [gameManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"

//************************************************************
//	�O���錾
//************************************************************
class CEditStageManager;	// �G�f�B�b�g�X�e�[�W�}�l�[�W���[�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �Q�[���}�l�[�W���[�N���X
class CGameManager
{
public:
	// �R���X�g���N�^
	CGameManager();

	// �f�X�g���N�^
	~CGameManager();

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V

	// �ÓI�����o�֐�
	static CGameManager *Create(void);	// ����
	static HRESULT Release(CGameManager *&prGameManager);	// �j��

	static CEditStageManager *GetEditStage(void);	// �G�f�B�b�g�X�e�[�W�擾

private:
	// �ÓI�����o�ϐ�
	static CEditStageManager *m_pEditStage;	// �G�f�B�b�g�X�e�[�W�̏��
};

#endif	// _GAMEMANAGER_H_
