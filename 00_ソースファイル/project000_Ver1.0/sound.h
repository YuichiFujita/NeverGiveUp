//============================================================
//
//	�T�E���h�w�b�_�[ [sound.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SOUND_H_
#define _SOUND_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"

//************************************************************
//	�N���X��`
//************************************************************
// �T�E���h�N���X
class CSound
{
public:
	// �R���X�g���N�^
	CSound();

	// �f�X�g���N�^
	~CSound();

	// �T�E���h���x����
	enum ELabel
	{
		LABEL_BGM_GENERAL = 0,	// BGM (�ėp)
		LABEL_BGM_TUTORIAL,		// BGM (�`���[�g���A��)
		LABEL_BGM_GAME,			// BGM (�Q�[��)

		LABEL_SE_SELECT_000,	// �I�𑀍쉹00
		LABEL_SE_SELECT_001,	// �I�𑀍쉹01
		LABEL_SE_DECISION_000,	// ���艹00
		LABEL_SE_DECISION_001,	// ���艹01
		LABEL_SE_DECISION_002,	// ���艹02
		LABEL_SE_PAPER,			// ��Ⳃ߂��艹
		LABEL_SE_SPAWN,			// ������
		LABEL_SE_PLAYER_SHOT,	// �v���C���[�ˌ��� (�A)
		LABEL_SE_ENEMY_SHOT000,	// �G�ˌ��� (�e)
		LABEL_SE_ENEMY_SHOT001,	// �G�ˌ��� (��C)
		LABEL_SE_STICK,			// �A���e��
		LABEL_SE_HIT,			// �e���e��
		LABEL_SE_BREAK,			// �j��
		LABEL_SE_HURRICANE,		// �n���P�[���������܂ꉹ
		LABEL_SE_FLY,			// ���V��
		LABEL_SE_STOMP,			// �A�����݂���
		LABEL_SE_WARNING,		// �x����
		LABEL_SE_LEVELUP,		// ���x���A�b�v��
		LABEL_MAX,				// ���̗񋓌^�̑���
	};

	// �T�E���h���\����
	struct SSoundInfo
	{
		char *pFilename;	// �t�@�C����
		int   nCntLoop;		// ���[�v�J�E���g
	};

	// �����o�֐�
	HRESULT Init(HWND hWnd);	// ������
	void Uninit(void);			// �I��
	HRESULT Play(ELabel label);	// �Đ�
	void Stop(ELabel label);	// ��~
	void Stop(void);			// �S��~

	// �ÓI�����o�֐�
	static CSound *Create(HWND hWnd);			// ����
	static HRESULT Release(CSound *&pSound);	// �j��

private:
	// �����o�֐�
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);	// �`�����N�`�F�b�N
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);	// �`�����N�f�[�^�Ǎ�

	// �����o�ϐ�
	IXAudio2 *m_pXAudio2;								// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice *m_pMasteringVoice;			// �}�X�^�[�{�C�X
	IXAudio2SourceVoice    *m_apSourceVoice[LABEL_MAX];	// �\�[�X�{�C�X
	BYTE *m_apDataAudio[LABEL_MAX];						// �I�[�f�B�I�f�[�^
	DWORD m_aSizeAudio[LABEL_MAX];						// �I�[�f�B�I�f�[�^�T�C�Y
	static const SSoundInfo mc_aSoundInfo[LABEL_MAX];	// �T�E���h���萔
};

#endif	// _SOUND_H_
