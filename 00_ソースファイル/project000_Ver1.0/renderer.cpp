//============================================================
//
//	�����_���[���� [renderer.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "renderer.h"
#include "object.h"
#include "manager.h"
#include "camera.h"

//************************************************************
//	�e�N���X [CRenderer] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CRenderer::CRenderer()
{
	// �����o�ϐ����N���A
	m_pD3D			= NULL;		// Direct3D�I�u�W�F�N�g
	m_pD3DDevice	= NULL;		// Direct3D�f�o�C�X
}

//============================================================
//	�f�X�g���N�^
//============================================================
CRenderer::~CRenderer()
{

}

//============================================================
//	����������
//============================================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	// �����o�ϐ���������
	m_pD3D			= NULL;		// Direct3D�I�u�W�F�N�g
	m_pD3DDevice	= NULL;		// Direct3D�f�o�C�X

	// �ϐ���錾
	D3DDISPLAYMODE			d3ddm;	// �f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS	d3dpp;	// �v���[���e�[�V�����p�����[�^

	// Direct3D�I�u�W�F�N�g�̐���
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{ // �I�u�W�F�N�g�̐����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{ // �f�B�X�v���C���[�h�̎擾�Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));			// �p�����[�^�̃[���N���A
	d3dpp.BackBufferWidth	= SCREEN_WIDTH;		// �Q�[����ʃT�C�Y (��)
	d3dpp.BackBufferHeight	= SCREEN_HEIGHT;	// �Q�[����ʃT�C�Y (����)
	d3dpp.BackBufferFormat	= d3ddm.Format;		// �o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount	= 1;				// �o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect		= D3DSWAPEFFECT_DISCARD;	// �_�u���o�b�t�@�̐؂�ւ� (�f���M���ɓ���)
	d3dpp.EnableAutoDepthStencil	= TRUE;				// �f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;		// �f�v�X�o�b�t�@�Ƃ��� 16bit ���g��
	d3dpp.Windowed					= bWindow;			// �E�C���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;		// ���t���b�V�����[�g
	d3dpp.PresentationInterval			= D3DPRESENT_INTERVAL_DEFAULT;	// �C���^�[�o��

	// Direct3D�f�o�C�X�̐���
	if (FAILED(CreateDevice(hWnd, d3dpp)))
	{ // �f�o�C�X�̐����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �����_�[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �T���v���[�X�e�[�g�̐ݒ� (�e�N�X�`���̊g�k��Ԃ̐ݒ�)
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ� (�e�N�X�`���̃A���t�@�u�����h�̐ݒ�)
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CRenderer::Uninit(void)
{
	// Direct3D�f�o�C�X�̔j��
	if (m_pD3DDevice != NULL)
	{ // Direct3D�f�o�C�X���g�p���̏ꍇ

		// �������J��
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	// Direct3D�I�u�W�F�N�g�̔j��
	if (m_pD3D != NULL)
	{ // Direct3D�I�u�W�F�N�g���g�p���̏ꍇ

		// �������J��
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

//============================================================
//	�X�V����
//============================================================
void CRenderer::Update(void)
{
	// �I�u�W�F�N�g�̑S�X�V
	CObject::UpdateAll();
}

//============================================================
//	�`�揈��
//============================================================
void CRenderer::Draw(void)
{
	// �ϐ���錾
	HRESULT			hr;				// �ُ�I���̊m�F�p
	D3DVIEWPORT9	viewportDef;	// �J�����̃r���[�|�[�g�ۑ��p

	// �o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A
	hr = m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
	if (FAILED(hr)) { assert(false); }

	// �`��J�n
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{ // �`��J�n�����������ꍇ

		// ���݂̃r���[�|�[�g���擾
		m_pD3DDevice->GetViewport(&viewportDef);

		// �J�����̐ݒ�
		CManager::GetInstance()->GetCamera()->SetCamera(CCamera::TYPE_MAIN);

		// �I�u�W�F�N�g�̑S�`��
		CObject::DrawAll();

		// �f�o�b�O�\���̕`��
		CManager::GetInstance()->GetDebugProc()->Draw();

		// �r���[�|�[�g�����ɖ߂�
		m_pD3DDevice->SetViewport(&viewportDef);

		// �`��I��
		hr = m_pD3DDevice->EndScene();
		if (FAILED(hr)) { assert(false); }
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	hr = m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
	if (FAILED(hr)) { assert(false); }
}

//============================================================
//	�f�o�C�X�擾����
//============================================================
LPDIRECT3DDEVICE9 CRenderer::GetDevice(void) const
{
	// �f�o�C�X�̃|�C���^��Ԃ�
	return m_pD3DDevice;
}

//============================================================
//	��������
//============================================================
CRenderer *CRenderer::Create(HWND hWnd, BOOL bWindow)
{
	// �|�C���^��錾
	CRenderer *pRenderer = NULL;	// �����_���[�����p

	if (pRenderer == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// ���������m��
		pRenderer = new CRenderer;	// �����_���[
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pRenderer != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// �����_���[�̏�����
		if (FAILED(pRenderer->Init(hWnd, bWindow)))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pRenderer;
			pRenderer = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pRenderer;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
HRESULT CRenderer::Release(CRenderer *&prRenderer)
{
	if (prRenderer != NULL)
	{ // �g�p���̏ꍇ

		// �����_���[�̏I��
		prRenderer->Uninit();

		// �������J��
		delete prRenderer;
		prRenderer = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}

//============================================================
//	�f�o�C�X��������
//============================================================
HRESULT CRenderer::CreateDevice(HWND hWnd, D3DPRESENT_PARAMETERS d3dpp)
{
	// �`�揈���ƒ��_�������n�[�h�E�F�A�ōs���ݒ�ɂ���
	if (FAILED(m_pD3D->CreateDevice
	( // ����
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice
	)))
	{ // CPU�̐��\���Ⴉ�����ꍇ

		// �`�揈���̓n�[�h�E�F�A�A���_������CPU���s���ݒ�ɂ���
		if (FAILED(m_pD3D->CreateDevice
		( // ����
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pD3DDevice
		)))
		{ // �n�[�h�E�F�A�̐��\���Ⴉ�����ꍇ

			// �`�揈���ƒ��_������CPU�ōs���ݒ�ɂ���
			if (FAILED(m_pD3D->CreateDevice
			( // ����
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_pD3DDevice
			)))
			{ // CPU�ƃn�[�h�E�F�A�̐��\���g�����ɂȂ�Ȃ������ꍇ

				// ���s��Ԃ�
				assert(false);
				return E_FAIL;
			}
		}
	}

	// ������Ԃ�
	return S_OK;
}
