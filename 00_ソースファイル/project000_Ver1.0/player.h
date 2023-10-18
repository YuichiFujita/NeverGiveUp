//============================================================
//
//	�v���C���[�w�b�_�[ [player.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "objectChara.h"

//************************************************************
//	�O���錾
//************************************************************
class CShadow;	// �e�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �v���C���[�N���X
class CPlayer : public CObjectChara
{
public:
	// ���f����
	enum EModel
	{
		MODEL_WAIST,	// ��
		MODEL_BODY,		// ��
		MODEL_HEAD,		// ��
		MODEL_ARMUL,	// ����r
		MODEL_ARMUR,	// �E��r
		MODEL_ARMDL,	// �����r
		MODEL_ARMDR,	// �E���r
		MODEL_HANDL,	// ����
		MODEL_HANDR,	// �E��
		MODEL_LEGUL,	// ��������
		MODEL_LEGUR,	// �E������
		MODEL_LEGDL,	// ����
		MODEL_LEGDR,	// �E��
		MODEL_FOOTL,	// ����
		MODEL_FOOTR,	// �E��
		MODEL_MAX		// ���̗񋓌^�̑���
	};

	// ���[�V������
	enum EMotion
	{
		MOTION_IDOL = 0,	// �ҋ@���[�V����
		MOTION_MOVE,		// �ړ����[�V����
		MOTION_SLIDE,		// �X���C�f�B���O���[�V����
		MOTION_MAX			// ���̗񋓌^�̑���
	};

	// ��ԗ�
	enum EState
	{
		STATE_NONE = 0,	// �������Ȃ����
		STATE_NORMAL,	// �ʏ���
		STATE_DEATH,	// ���S���
		STATE_MAX		// ���̗񋓌^�̑���
	};

	// ����
	enum EAxis
	{
		AXIS_X = 0,	// X��
		AXIS_Y,		// Y��
		AXIS_Z,		// Z��
		AXIS_MAX	// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CPlayer();

	// �f�X�g���N�^
	~CPlayer();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;		// ������
	void Uninit(void) override;			// �I��
	void Update(void) override;			// �X�V
	void Draw(void) override;			// �`��
	void Hit(const int nDmg) override;	// �q�b�g

	void SetState(const int nState) override;	// ��Ԑݒ�
	int GetState(void) const override;			// ��Ԏ擾
	float GetRadius(void) const override;		// ���a�擾

	void SetEnableUpdate(const bool bUpdate) override;	// �X�V�󋵐ݒ�
	void SetEnableDraw(const bool bDraw) override;		// �`��󋵐ݒ�
	D3DXMATRIX GetMtxWorld(void) const override;		// �}�g���b�N�X�擾

	// �ÓI�����o�֐�
	static CPlayer *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot		// ����
	);

private:
	// �����o�֐�
	void LoadSetup(void);		// �Z�b�g�A�b�v
	EMotion UpdateNormal(void);	// �ʏ��Ԏ��̍X�V

	void UpdateOldPosition(void);			// �ߋ��ʒu�̍X�V
	EMotion UpdateMove(void);				// �ړ��ʁE�ڕW�����̍X�V
	void UpdateJump(void);					// �W�����v�̍X�V
	void UpdateSliding(void);				// �X���C�f�B���O�̍X�V
	bool UpdateLanding(D3DXVECTOR3& rPos);	// ���n�󋵂̍X�V
	void UpdateRotation(D3DXVECTOR3& rRot);	// �����̍X�V
	void UpdateMotion(int nMotion);			// ���[�V�����E�I�u�W�F�N�g�L�����N�^�[�̍X�V

	bool ResponseSingleBuilding(const EAxis axis, D3DXVECTOR3& rPos);	// �r���Ƃ̈ꎲ���Ƃ̓����蔻��
	bool CollisionBuilding(D3DXVECTOR3& rPos);	// �r���Ƃ̓����蔻��
	bool CollisionObstacle(D3DXVECTOR3& rPos);	// ��Q���Ƃ̓����蔻��

	// �ÓI�����o�ϐ�
	static const char *mc_apModelFile[];	// ���f���萔

	// �����o�ϐ�
	CShadow *m_pShadow;			// �e�̏��
	D3DXVECTOR3	m_oldPos;		// �ߋ��ʒu
	D3DXVECTOR3	m_move;			// �ړ���
	D3DXVECTOR3	m_destRot;		// �ڕW����
	EState	m_state;			// ���
	int		m_nCounterState;	// ��ԊǗ��J�E���^�[
	int		m_nCounterSlide;	// �X���C�f�B���O�Ǘ��J�E���^�[
	float	m_fMove;			// �ړ���
	bool	m_bJump;			// �W�����v��
	bool	m_bSlide;			// �X���C�f�B���O��
	bool	m_bSlideControl;	// �X���C�f�B���O�����
};

#endif	// _PLAYER_H_
