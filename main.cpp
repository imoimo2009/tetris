//
// TETRIS Programmed by 2009/06/22 Imoimo
// 

#include "tetris.h"

#define	DEFAULT_FPS		60
#define WINDOW_WIDTH	640
#define WINDOW_HEIGHT	480
#define STR_WINTTL		"TETRIS"

// 関数プロトタイプ宣言(基本フレームワーク用)
void MainLoop(void);
void BeforeInit(void);
void AfterInit(void);
void UpdateGame(int GameTime);
void GameDraw(int GameTime);
void WaitLoop(LONGLONG intTime);
void DestroyGame(void);

// グローバル変数定義・初期化(基本フレームワーク用)
int WaitVsyncFlag;

/////////////////////////////////////////////////////////////////////////
//	各ゲームで使用するクラスインスタンスやグローバル変数はここに記述
//

// 構造体・クラスインスタンス定義
GameStatus	Gs;

// グローバル変数定義


//
// 定義ここまで。
/////////////////////////////////////////////////////////////////////////

// WinMain関数
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
						 LPSTR lpCmdLine, int nCmdShow )
{

	BeforeInit();		// DirectX初期化前処理
	if ( DxLib_Init() == -1 ) return -1;	// エラーが起きたら直ちに終了
	SetDrawScreen(DX_SCREEN_BACK);
	SetWaitVSyncFlag(TRUE);
	AfterInit();		// DirectX初期化後処理
	MainLoop();			// ゲーム本体(メインループ)
	DestroyGame();
	DxLib_End();		// ＤＸライブラリ使用の終了処理

	return 0;			// ソフトの終了
}

// ゲームフレームワーク(基本的に編集しない方針で。)
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

		// ESCキーだけは常に監視。押されたら直ちに終了！
		stick = CheckHitKey(KEY_INPUT_ESCAPE);
		if (stick == 1) break;
		// SPACEキーを押したらPAUSE(トグル動作)
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

// ゲームウェイト処理
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

// DirectX初期化前処理
void BeforeInit( void )
{
	int mbResult;

	mbResult = MessageBox(NULL,"フルスクリーンで起動しますか？",
												"起動処理",MB_YESNO);
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
//	この下から各ゲーム固有の処理を追加する。
//
//////////////////////////////////////////////////////////////////////////

// ゲーム開始前処理
void AfterInit( void )
{
	GameInitFirst(&Gs);
}

// ゲーム更新処理
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

// ゲーム描画処理
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