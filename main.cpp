//
// TETRIS Programmed by 2009/06/22 Imoimo
// 

#include "tetris.h"

#define	DEFAULT_FPS		60
#define WINDOW_WIDTH	640
#define WINDOW_HEIGHT	480
#define STR_WINTTL		"TETRIS"

// �֐��v���g�^�C�v�錾(��{�t���[�����[�N�p)
void MainLoop(void);
void BeforeInit(void);
void AfterInit(void);
void UpdateGame(int GameTime);
void GameDraw(int GameTime);
void WaitLoop(LONGLONG intTime);
void DestroyGame(void);

// �O���[�o���ϐ���`�E������(��{�t���[�����[�N�p)
int WaitVsyncFlag;

/////////////////////////////////////////////////////////////////////////
//	�e�Q�[���Ŏg�p����N���X�C���X�^���X��O���[�o���ϐ��͂����ɋL�q
//

// �\���́E�N���X�C���X�^���X��`
GameStatus	Gs;

// �O���[�o���ϐ���`


//
// ��`�����܂ŁB
/////////////////////////////////////////////////////////////////////////

// WinMain�֐�
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
						 LPSTR lpCmdLine, int nCmdShow )
{

	BeforeInit();		// DirectX�������O����
	if ( DxLib_Init() == -1 ) return -1;	// �G���[���N�����璼���ɏI��
	SetDrawScreen(DX_SCREEN_BACK);
	SetWaitVSyncFlag(TRUE);
	AfterInit();		// DirectX�������㏈��
	MainLoop();			// �Q�[���{��(���C�����[�v)
	DestroyGame();
	DxLib_End();		// �c�w���C�u�����g�p�̏I������

	return 0;			// �\�t�g�̏I��
}

// �Q�[���t���[�����[�N(��{�I�ɕҏW���Ȃ����j�ŁB)
void MainLoop(void)
{
	unsigned int gameTime = 0;
	int stick,pzFlag = 0,kf = 0;
	LONGLONG startTime,frmTime;

	startTime = GetNowHiPerformanceCount();
	while (ProcessMessage() == 0)
	{
		frmTime = GetNowHiPerformanceCount();
		if(pzFlag == 0) UpdateGame(gameTime);
		GameDraw(gameTime);
		if(pzFlag == 1) DrawString(0,0, "PAUSE!" ,GetColor(255,0,0));
		//if(WaitVsyncFlag == FALSE){
		//	frmTime = GetNowHiPerformanceCount() - frmTime;
		//	WaitLoop(frmTime);
		//}
		ScreenFlip();

		// ESC�L�[�����͏�ɊĎ��B�����ꂽ�璼���ɏI���I
		stick = CheckHitKey(KEY_INPUT_ESCAPE);
		if (stick == 1) break;
		// SPACE�L�[����������PAUSE(�g�O������)
		stick = CheckHitKey(KEY_INPUT_SPACE);
		if (stick == 1){
			if(kf == 0){
				kf = 1;
				if(pzFlag == 0) pzFlag = 1;
				else pzFlag = 0;
				PlaySoundMem(Gs.SoundHandle[SOUND_KEYH],DX_PLAYTYPE_BACK);
			}
		}else kf = 0;
		gameTime++;
	}
}

// �Q�[���E�F�C�g����
void WaitLoop(LONGLONG intTime)
{
	LONGLONG stTime,nowTime;
	int lpFlag = 1;
	intTime = DEFAULT_FPS - intTime;
	stTime = GetNowHiPerformanceCount();
	while(lpFlag){
		nowTime = GetNowHiPerformanceCount();
		if(nowTime - stTime > intTime){
			lpFlag = 0;
		}
	}
}

// DirectX�������O����
void BeforeInit( void )
{
	int mbResult;

	mbResult = MessageBox(NULL,"�t���X�N���[���ŋN�����܂����H",
												"�N������",MB_YESNO);
	if (mbResult == IDYES){
		ChangeWindowMode(FALSE);
		//WaitVsyncFlag = TRUE;
	}else{
		ChangeWindowMode(TRUE);
		//WaitVsyncFlag = FALSE;
	}
	SetMainWindowText(STR_WINTTL);
}

//////////////////////////////////////////////////////////////////////////
//
//	���̉�����e�Q�[���ŗL�̏�����ǉ�����B
//
//////////////////////////////////////////////////////////////////////////

// �Q�[���J�n�O����
void AfterInit( void )
{
	GameInitFirst(&Gs);
}

// �Q�[���X�V����
void UpdateGame(int GameTime)
{
	void (*pf[])(GameStatus*) =
	{
		GameMainUpdate,
		GameTitleUpdate,
		GameOverUpdate,
		GameRankingUpdate,
		GameInputRankingUpdate,
		GameOptionUpdate
	};

	pf[Gs.GameMode](&Gs);
}

// �Q�[���`�揈��
void GameDraw(int GameTime)
{
	void (*pf[])(GameStatus*) =
	{
		GameMainDraw,
		GameTitleDraw,
		GameMainDraw,
		GameRankingDraw,
		GameMainDraw,
		GameOptionDraw
	};

	pf[Gs.GameMode](&Gs);
}

void DestroyGame(void){
	ResetReplay(&Gs);
}