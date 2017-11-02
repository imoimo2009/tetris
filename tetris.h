// tetris.h

#pragma once

// インクルード
#include "dxlib.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// DXLib関数の戻り値
#define DXLIB_TRUE		0
#define DXLIB_FALSE		(-1)

// ゲーム内部パラメータ
#define FIELD_X			12
#define FIELD_Y			23
#define NEXT_NUM		4
#define TETRI_SIZE		4
#define TETRI_NUM		7
#define BLOCK_WALL		9
#define BLOCK_OFF		0
#define BLOCK_ENABLE	1
#define ROTATE_RIGHT	0
#define ROTATE_LEFT		1
#define ROTATE_NOT		9
#define SPEED_ADD		6
#define SPEED_MAX		10
#define SPEED_TABLE		"59,44,29,14,09,05,04,02,01,00"
#define LEVEL_UP_LINES	10
#define WAIT_FLAME		60
#define KEY_REPEAT_TIME 10
#define HISCORE			2000
#define RANK_MAX		10
#define GOVER_HEIGHT	20
#define PINCH_HEIGHT	14
#define START_POS_X		5
#define START_POS_Y		0
#define C_LINE_EMPTY	0
#define C_LINE_FULL		10
#define L_FLASH_TIME	80
#define L_DROP_TIME		20
#define L_ERASE_TIME	50
#define ANIME_TIME		90
#define BLOCK_RECAST	20
#define GAMEMODE_MAIN	0
#define GAMEMODE_TITLE  1
#define GAMEMODE_GOV	2
#define GAMEMODE_RANK	3
#define GAMEMODE_RANKIN 4
#define GAMEMODE_OPTION	5
#define REPLAYMODE_PLAY	1
#define REPLAYMODE_REC	0
#define GOVERDEMO_TIME	3
#define GOVERDEMO_END	21
#define TSELECT_START	0
#define TSELECT_REPLAY	1
#define TSELECT_RANKING	2
#define TSELECT_OPTION	3
#define CURSOL_SPEED	4
#define RANKING_NAME	5
#define DEFAULT_YEAR	2009
#define DEFAULT_MONTH	7
#define DEFAULT_DAY		11
#define DEFAULT_HOUR	0
#define DEFAULT_MINIT	0
#define RANKING_FILE	"ranking.txt"
#define REPLAY_FILE		"replay.dat"
#define OPTION_FILE		"tetris.ini"
#define OPTION_MAX		9
#define OPTION_BGMV		0
#define OPTION_SEV		1
#define OPTION_HDROP	2
#define OPTION_HOLD		3
#define OPTION_GHOST	4
#define OPTION_NEXT		5
#define OPTION_KEYR		6
#define OPTION_KEYL		7
#define OPTION_KEYH		8
#define OPT_BGM_NUM		5
#define OPT_SE_NUM		5
#define OPT_NEXT_NUM	3
#define OPT_KEY_NUM		2
#define OPT_DEF_NUM		1

// INIファイル関連
#define INI_COMMENT		'#'
#define INI_TOKEN		'='
#define INI_STR_BGMV	"BGMVOLUME="
#define INI_STR_SEV		"SEVOLUME="
#define INI_STR_HDROP	"HARDDROP="
#define INI_STR_HOLD	"HOLD="
#define INI_STR_GHOST	"GHOST="
#define INI_STR_NEXT	"NEXTNUM="
#define INI_STR_KEYL	"KEY_LEFT_ASSIGN="
#define INI_STR_KEYR	"KEY_RIGHT_ASSIGN="
#define INI_STR_KEYH	"KEY_HOLD_ASSIGN="
#define INI_DEFAULT		"#TETRIS用設定ファイルです。"

// 描画関係パラメータ
#define STAGE_POS_X		224
#define STAGE_POS_Y		64
#define STAGE_SIZE_X	14
#define STAGE_SIZE_Y	24
#define	STAGE_CLIP		2
#define STAGE_ENTER_R	9
#define STAGE_ENTER_L	4
#define NEXT_POS_X		236
#define NEXT_POS_Y		0
#define HOLD_POS_X		150
#define HOLD_POS_Y		80
#define HSCORE_POS_X	420
#define HSCORE_POS_Y	80
#define SCORE_POS_X		436
#define SCORE_POS_Y		120
#define LEVEL_POS_X		80
#define LEVEL_POS_Y		300
#define LINES_POS_X		80
#define LINES_POS_Y		340
#define GOVER_POS_X		(STAGE_POS_X + CHAR_SIZE * 2)
#define GOVER_POS_Y		(STAGE_POS_Y + CHAR_SIZE * 10)
#define TMENU_POS_X		300
#define TMENU_POS_Y		300
#define TMENU_DIST		20
#define TMENU_CUR_X		280
#define MASK_OFF		0
#define MASK_ON			1
#define G_BOX_MARGIN	4
#define G_BOX_ALPHA		150
#define GHOST_ALPHA		80
#define STAGETOP_ALPHA	45
#define FADEOUT_START	75
#define FADEOUT_END		255
#define GOVFADE_END		200
#define FADE_SPEED		3
#define BG_CHIP_WIDTH	8
#define BG_CHIP_HEIGHT	8
#define BG_CHIP_NUM		3
#define BG_SIZE_X		6
#define BG_SIZE_Y		5
#define RANK_TITLE_X	288
#define RANK_TITLE_Y	32
#define RANK_CAPTION_Y	80
#define R_CAP_RANK_X	32
#define R_CAP_DATE_X	176
#define R_CAP_SCORE_X	352
#define R_CAP_LINE_X	464
#define R_CAP_NAME_X	552
#define R_LINE_DATE_X	72
#define R_LINE_SCORE_X	312
#define R_LINE_LINE_X	440
#define R_LINE_NAME_X	520
#define R_LINE_BOT		432
#define R_DATE_POS_X	48
#define R_SCORE_POS_X	288
#define R_LINE_POS_X	416
#define R_NAME_POS_X	496
#define RANK_TBOX_X		16
#define RANK_TBOX_Y		16
#define RANK_LIST_POS_X	32
#define RANK_LIST_POS_Y	112
#define R_INP_POS_LX	64	
#define R_INP_POS_RX	288
#define R_INP_POS_AX	480
#define R_INP_POS_LRY	16
#define R_INP_POS_CX	224
#define R_INP_POS_CY	300
#define R_INP_SIZE1		32
#define R_INP_SIZE2		64
#define R_INPUT_WIDTH	15
#define R_INPUT_CENTER	7
#define R_INPSTR_X		280
#define R_INPSTR_Y		400
#define R_INP_CAP1_X	192
#define R_INP_CAP2_X	384
#define R_INP_CAP_Y		117
#define R_INP_CNUM_X	304
#define R_INP_CNUM_Y	112
#define R_INP_CNUM_DIG	2
#define R_INP_COPT_X	352
#define R_INP_COPT_Y	128
#define R_INP_SCORE_X	236
#define R_INP_SCORE_Y	176
#define R_INP_LINES_X	260
#define R_INP_LINES_Y	224

// キャラクタ関係
#define TITLEBG_FNAME	"titlebg.bmp"
#define OPTIONCH_FNAME	"option.bmp"
#define BG_FNAME		"bg.jpg"
#define FONT_FNAME		"gamefont.bmp"
#define CHAR_FNAME		"tetris.bmp"
#define CHAR1_FNAME		"tetris1.bmp"
#define CHAR_SIZE		16
#define CHARSET_SIZE_X	10
#define CHARSET_SIZE_Y	15
#define CHARSET1_SIZE_Y	3
#define FONT_SIZE		16
#define FONTSET_SIZE_X	10
#define FONTSET_SIZE_Y	6
#define LINE_LT			30
#define LINE_RT			31
#define LINE_LB			32
#define LINE_RB			33
#define LINE_TOP		34
#define LINE_BOT		35
#define LINE_LM			36
#define LINE_RM			37
#define LINE_ENT_L		38
#define LINE_ENT_R		39
#define G_FLASH_START	40
#define G_FLASH_END		49
#define G_LEVEL_START	20
#define G_LEVEL_WIDTH	4
#define G_LEVEL_DIGIT	3
#define G_SCORE_START	24
#define G_SCORE_WIDTH	3
#define G_SCORE_DIGIT	7
#define G_HSCORE_START	76
#define G_HSCORE_WIDTH  4
#define G_HSCORE_DIGIT	7
#define G_NEXT_START	27
#define G_NEXT_WIDTH	3
#define G_TITLE_START	50
#define G_TITLE_WIDTH	8
#define G_TITLE_HEIGHT	2
#define G_HOLD_START	70
#define G_HOLD_WIDTH	3
#define G_LINES_START	73
#define G_LINES_WIDTH	3
#define G_LINES_DIGIT	4
#define G_GOVER_START	110
#define G_GOVER_WIDTH	8
#define G_GOVER_HEIGHT	2
#define G_H_A_K_START	80
#define G_H_A_K_WIDTH	6
#define G_REPLAY_START	86
#define G_REPLAY_WIDTH	4
#define G_START_START	90
#define G_START_WIDTH	6
#define G_OPTION_START	130
#define G_OPTION_WIDTH	4
#define G_RANKING_START	134
#define G_RANKING_WIDTH	4
#define G_CURSOL_CODE	58
#define G_GRADATION		128
#define G_RANK_LINE_S	138
#define G_RANK_LINE_W	2
#define G_RANK_RANK_S	140
#define G_RANK_RANK_W	2
#define G_RANK_DATE_S	142
#define G_RANK_DATE_W	2
#define G_RANK_SCORE_S	144
#define G_RANK_SCORE_W	3
#define G_RANK_NAME_S	147
#define G_RANK_NAME_W	3
#define G_CORNER_LU		108 
#define G_CORNER_RU		109 
#define G_CORNER_LD		118 
#define G_CORNER_RD		119 
#define G_UGET1_START	0
#define G_UGET1_WIDTH	6
#define G_UGET1_HEIGHT	2
#define G_UGET2_START	6
#define G_UGET2_WIDTH	4
#define G_UGET2_HEIGHT	2
#define G_1ST_START		20
#define G_2ND_START		22
#define G_3RD_START		24
#define G_4TH_START		26
#define G_NOPT_WIDTH	2

// サウンド関係
#define S_FN_BGM		"bgm.ogg"
#define S_FN_BLIN		"blockin.wav"
#define S_FN_BHIT		"blockhit.wav"
#define S_FN_DROP		"linedrop.wav"
#define S_FN_HOLD		"hold.wav"
#define S_FN_ROTE		"rotate.wav"
#define S_FN_FLSH		"flash.wav"
#define S_FN_KEYH		"keyhit.wav"
#define S_FN_HDRP		"hdrop.wav"
#define S_FN_LVUP		"lvup.wav"
#define S_FN_GOVR		"gover.wav"
#define SOUND_NUM		11
#define SOUND_BGM		0
#define SOUND_BLIN		1
#define SOUND_BHIT		2
#define SOUND_DROP		3
#define SOUND_HOLD		4
#define SOUND_ROTE		5
#define SOUND_FLSH		6
#define SOUND_KEYH		7
#define SOUND_HDRP		8
#define SOUND_LVUP		9
#define SOUND_GOVR		10
#define BGM_LOOP_POINT	30480
#define PINCH_FREQ		48000
#define NORMAL_FREQ		44100
#define VOL_PITCH		51
#define VOL_BGM			0
#define VOL_SE			1

// その他
#define STRING_BUFFER	256

// ブロック定義
#define PIECE_1_1		(2*TETRI_SIZE+0)	// 0000
#define PIECE_1_2		(2*TETRI_SIZE+1)	// 0000
#define PIECE_1_3		(2*TETRI_SIZE+2)	// 1111
#define PIECE_1_4		(2*TETRI_SIZE+3)	// 0000
#define PIECE_2_1		(1*TETRI_SIZE+1)	// 0000
#define PIECE_2_2		(1*TETRI_SIZE+2)	// 0110
#define PIECE_2_3		(2*TETRI_SIZE+1)	// 0110
#define PIECE_2_4		(2*TETRI_SIZE+2)	// 0000
#define PIECE_3_1		(1*TETRI_SIZE+1)	// 0000
#define PIECE_3_2		(1*TETRI_SIZE+2)	// 0110
#define PIECE_3_3		(2*TETRI_SIZE+0)	// 1100
#define PIECE_3_4		(2*TETRI_SIZE+1)	// 0000
#define PIECE_4_1		(1*TETRI_SIZE+0)	// 0000
#define PIECE_4_2		(1*TETRI_SIZE+1)	// 1100
#define PIECE_4_3		(2*TETRI_SIZE+1)	// 0110
#define PIECE_4_4		(2*TETRI_SIZE+2)	// 0000
#define PIECE_5_1		(1*TETRI_SIZE+1)	// 0000
#define PIECE_5_2		(2*TETRI_SIZE+1)	// 0100
#define PIECE_5_3		(2*TETRI_SIZE+2)	// 0111
#define PIECE_5_4		(2*TETRI_SIZE+3)	// 0000
#define PIECE_6_1		(1*TETRI_SIZE+2)	// 0000
#define PIECE_6_2		(2*TETRI_SIZE+0)	// 0010
#define PIECE_6_3		(2*TETRI_SIZE+1)	// 1110
#define PIECE_6_4		(2*TETRI_SIZE+2)	// 0000
#define PIECE_7_1		(1*TETRI_SIZE+1)	// 0000
#define PIECE_7_2		(2*TETRI_SIZE+0)	// 0100
#define PIECE_7_3		(2*TETRI_SIZE+1)	// 1110
#define PIECE_7_4		(2*TETRI_SIZE+2)	// 0000
#define PIECE_TYPE_1	1
#define PIECE_TYPE_2	2
#define PIECE_TYPE_3	3
#define PIECE_TYPE_4	4
#define PIECE_TYPE_5	5
#define PIECE_TYPE_6	6
#define PIECE_TYPE_7	7
#define PIECE_CENTER_1	0
#define PIECE_CENTER_2	0
#define PIECE_CENTER_3	0
#define PIECE_CENTER_4	0
#define PIECE_CENTER_5	2
#define PIECE_CENTER_6	1
#define PIECE_CENTER_7	1

// 構造体定義

struct Replay{
	int GameTime;
	int Stick;
	Replay *Next;
};

struct Ranking{
	DATEDATA Date;
	int Score;
	int Lines;
	int Name[RANKING_NAME];
};

struct Option{
	int BgmVolume;
	int SeVolume;
	int HardDrop;
	int Hold;
	int NextNum;
	int Ghost;
	int KeyRight;
	int KeyLeft;
	int KeyHold;
};

struct GameStatus{
	// ポインタ
	Replay *ReplayStart;
	Replay *CurrentReplay;
	// 配列
	int Next[NEXT_NUM];
	int Piece[TETRI_SIZE * TETRI_SIZE];
	int Stage[STAGE_SIZE_X * STAGE_SIZE_Y];
	int CharHandle[CHARSET_SIZE_X * CHARSET_SIZE_Y];
	int CharHandle1[CHARSET_SIZE_X * CHARSET1_SIZE_Y];
	int FontHandle[FONTSET_SIZE_X * FONTSET_SIZE_Y];
	int SoundHandle[SOUND_NUM];
	int EraseLines[STAGE_SIZE_Y];
	int BgChipHandle[BG_CHIP_NUM];
	int SpeedTable[SPEED_MAX];
	// 構造体インスタンス
	Option Op;
	Ranking PlayerRanking[RANK_MAX];
	// ゲームパラメータ
	int Color;
	int Bl_X;
	int Bl_Y;
	int Ghost_Y;
	int Score;
	int HiScore;
	int Level;
	int Speed;
	int GetLines;
	int WaitTime;
	int AnimeTimer;
	int RotateStatus;
	int PieceCenter;
	int EraseLineNum;
	int BgHandle;
	int TitleBgHandle;
	int OptionCharHandle;
	int GameMode;
	int StackHeight;
	int Hold;
	int HardDrop;
	int GameTime;
	int ReplayMode;
	int RecordIndex;
	int FadeTimer;
	int FadeStart;
	int GovCounter;
	int RandSeed;
	int SelectVar;
	int Cursol_Pos;
	int Cursol_Pos2;
	int BG_X;
	int BG_Y;
	int Rank;
	int GroundFlag;
	// 入力キーフラグ
	int KeyFlag_A;
	int KeyFlag_B;
	int KeyFlag_C;
	int KeyFlag_L;
	int KeyFlag_R;
	int KeyFlag_UP;
	int KeyFlag_DOWN;
	int KeyRepeat_L;
	int KeyRepeat_R;
};

struct TitleStatus{
	int SelectVar;
	int Cursol_Pos;
};


// 列挙体定義
enum CharGraph{
	CHAR_0,CHAR_1,CHAR_2,CHAR_3,CHAR_4,CHAR_5,CHAR_6,CHAR_7,CHAR_8,CHAR_9,
	CHAR_A,CHAR_B,CHAR_C,CHAR_D,CHAR_E,CHAR_F,CHAR_G,CHAR_H,CHAR_I,CHAR_J,
	CHAR_K,CHAR_L,CHAR_M,CHAR_N,CHAR_O,CHAR_P,CHAR_Q,CHAR_R,CHAR_S,CHAR_T,
	CHAR_U,CHAR_V,CHAR_W,CHAR_X,CHAR_Y,CHAR_Z,CHAR_CHAR_COMMA,CHAR_DOT,CHAR_MINUS,CHAR_ECALL,
	CHAR_PLUS,CHAR_SLASH,CHAR_AT,CHAR_INC,CHAR_DEC,CHAR_SHARP,CHAR_COLON,CHAR_EXC,CHAR_HEART,CHAR_CAT,
	CHAR_UBAR,CHAR_SPACE,CHAR_BACK,CHAR_END
};

// 関数プロトタイプ宣言

// 親関数
void GameMainDraw(GameStatus *gs);
void GameTitleDraw(GameStatus *gs);
void GameRankingDraw(GameStatus *gs);
void GameOptionDraw(GameStatus *gs);
void GameMainUpdate(GameStatus *gs);
void GameTitleUpdate(GameStatus *gs);
void GameRankingUpdate(GameStatus *gs);
void GameOverUpdate(GameStatus *gs);
void GameInputRankingUpdate(GameStatus *gs);
void GameOptionUpdate(GameStatus *gs);

// 子関数・更新処理
int LoadCharacters(int *gHandle,const char *str,int width,int height,int size);
int LoadSoundFiles(int *sHandle);
void InitStatus(GameStatus *gs);
void MoveTetriDrop(GameStatus *gs);
int UserControl(GameStatus *gs);
void ResetTetriBlocks(GameStatus *gs);
void EraseBlockLine(int *stage,int *eraseLines);
void DropBlockLine(int *stage,int row);
int UpdateGameParam(GameStatus *gs);
void RecordReplay(GameStatus *gs,int stick);
int PlaybackReplay(GameStatus *gs);
void ResetReplay(GameStatus *gs);
void GameInitFirst(GameStatus *gs);

// 子関数・描画処理
void DrawStage(int *gHandle,int *stage,int x,int y);
void DrawStageFlame(int *gHandle,int x,int y);
void DrawPlayerTetriBlocks(GameStatus *gs,int x,int y);
void DrawHighScore(GameStatus *gs,int x,int y);
void DrawScore(GameStatus *gs,int x,int y);
void DrawLevel(GameStatus *gs,int x,int y);
void DrawLines(GameStatus *gs,int x,int y);
void DrawGhostTetriBlocks(GameStatus *gs);
void DrawHoldTetriBlock(GameStatus *gs,int x,int y);
void DrawNextTetriBlocks(GameStatus *gs,int x,int y);
void DrawFlashLine(GameStatus *gs);
void DrawGameOver(GameStatus *gs);
void DrawScrollBG(GameStatus *gs);
void DrawRankingLine(GameStatus *gs,int x,int y,int rank);
void DrawRankingInput(GameStatus *gs);

// 内部関数・更新処理
int SetPattern(int *piece,int type);
int RotateNext(GameStatus *gs);
int PieceOffsetX(int *piece,int *stage,int x,int y);
int PieceOffsetY(int *piece,int *stage,int x,int y);
int GetDigitNum(int num,int digit);
void CopyBlockLine(int *stage,int orgLine,int copyLine);
int CheckBlockLine(int *stage,int checkLine);
int GetCollision(int *piece,int *stage,int x,int y);
void StackBlock(GameStatus *gs,int x,int y);
int ScanBlockLine(int *stage,int *eraseLines,int row);
int GetRandomPiece(int *next);
int GetHoldBlock(GameStatus *gs);
int GetGhostPos(int *stage,int *piece,int x,int y);
int TetriRotate(GameStatus *gs,int rotate);
void InitTitle(GameStatus *gs);
void GameStart(GameStatus *gs);
void ReplayStart(GameStatus *gs);
void RankingStart(GameStatus *gs);
void OptionStart(GameStatus *gs);
void SetBlockMask(int *mask,int x,int y);
void GameOverInit(GameStatus *gs);
void RankingInit(GameStatus *gs);
int CheckRanking(Ranking *rp,int score,int lines);
void UpdateRanking(GameStatus *gs);
void StartRankingInput(GameStatus *gs);
void DestroyBGHandle(int *gHandle);
int LoadRankingFile(Ranking *rp,char *str);
int SaveRankingFile(Ranking *rp,char *str);
int LoadReplayFile(GameStatus *gs,char *str);
int SaveReplayFile(GameStatus *gs,char *str);
int GetStrParam(char *str,int start,int digit);
void OptionInit(Option *op);
int LoadIniFile(Option *op,char *str);
int SaveIniFile(Option *op,char *str);
int GetIniParam(char *buf);
int GetIniState(char *buf);
void ChangeOptionValue(Option *op,int index,int inc);
void swap(int *a,int *b);
void SetVolume(GameStatus *gs,int mode,int volume);

// 内部関数・描画処理
void DrawTetriBlocks(int *gHandle,int *piece,int x,int y,int *mask);
void DrawTileChar(int *gHandle,int x,int y,int start,int width,int height);
void DrawExtendTileChar(int *gHandle,int x,int y,int x1,int y1,int start,int width,int height);
void DrawTransBox(int x,int y,int x1,int y1,int color,int blendPalam);
void DrawNumber(int *gHandle,int x,int y,int num,int digit_max);
void Draw2xNumber(int *gHandle,int x,int y,int num,int digit_max);
int MakeBGChip(int *gHandle,int logo);
void DrawNameString(int *gHandle,int x,int y,int *str);
void DrawInputName(int *gHandle,int x,int y,int count,int code);
void DrawRankingName(int *gHandle,int x,int y,int pos,int chr,int *name);

// グローバル変数宣言
//extern GameStatus	Gs;
