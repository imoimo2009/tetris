// titleupdate.cpp

#include "tetris.h"

void GameTitleUpdate(GameStatus *gs){
	int curPos;
	// キー入力検出部
	int stick = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	// フェード処理
	if(gs->FadeStart == 1){
		if(gs->FadeTimer > FADEOUT_END - FADEOUT_START){
			switch(gs->SelectVar){
				case TSELECT_START:
					GameStart(gs);
					break;
				case TSELECT_REPLAY:
					ReplayStart(gs);
					break;
				case TSELECT_RANKING:
					RankingStart(gs);
					break;
				case TSELECT_OPTION:
					OptionStart(gs);
					break;
			}
		}
		gs->FadeTimer += FADE_SPEED;
	}else{
		// 上入力
		if(stick == PAD_INPUT_UP){
			if(gs->KeyFlag_UP == 0){
				gs->SelectVar--;
				if(gs->SelectVar < TSELECT_START) gs->SelectVar = TSELECT_OPTION;
				if(gs->SelectVar == TSELECT_REPLAY){
					if(gs->ReplayStart == NULL) gs->SelectVar--;
				}
				gs->KeyFlag_UP = 1;
				PlaySoundMem(gs->SoundHandle[SOUND_KEYH],DX_PLAYTYPE_BACK);
			}
		}else gs->KeyFlag_UP = 0;
		// 下入力
		if(stick == PAD_INPUT_DOWN){
			if(gs->KeyFlag_DOWN == 0){
				gs->SelectVar++;
				if(gs->SelectVar > TSELECT_OPTION) gs->SelectVar = TSELECT_START;
				if(gs->SelectVar == TSELECT_REPLAY){
					if(gs->ReplayStart == NULL) gs->SelectVar++;
				}
				gs->KeyFlag_DOWN = 1;
				PlaySoundMem(gs->SoundHandle[SOUND_KEYH],DX_PLAYTYPE_BACK);
			}
		}else gs->KeyFlag_DOWN = 0;
		// 決定キー入力
		if(stick == PAD_INPUT_A){
			if(gs->KeyFlag_A == 0){
				gs->FadeStart = 1;
				gs->KeyFlag_A = 1;
				PlaySoundMem(gs->SoundHandle[SOUND_KEYH],DX_PLAYTYPE_BACK);
			}
		}else gs->KeyFlag_A = 0;
		// カーソル移動
		curPos = TMENU_POS_Y + gs->SelectVar * TMENU_DIST;
		if(gs->Cursol_Pos > curPos) gs->Cursol_Pos -= CURSOL_SPEED;
		else if(gs->Cursol_Pos < curPos) gs->Cursol_Pos += CURSOL_SPEED;
	}
}

void InitTitle(GameStatus *gs){
	gs->GameMode = GAMEMODE_TITLE;
	gs->FadeTimer = 0;
	gs->FadeStart = 0;
	gs->KeyFlag_A = 1;
	gs->KeyFlag_UP = 1;
	gs->KeyFlag_DOWN = 1;
	gs->SelectVar = TSELECT_START;
	gs->Cursol_Pos = TMENU_POS_Y;
	if(gs->ReplayStart != NULL) SaveReplayFile(gs,REPLAY_FILE);
}

void GameStart(GameStatus *gs){
	gs->GameMode = GAMEMODE_MAIN;
	gs->ReplayMode = REPLAYMODE_REC;
	InitStatus(gs);
}

void ReplayStart(GameStatus *gs){
	gs->GameMode = GAMEMODE_MAIN;
	gs->ReplayMode = REPLAYMODE_PLAY;
	InitStatus(gs);
}

void OptionStart(GameStatus *gs){
	gs->BG_X = 0;
	gs->BG_Y = 0;
	gs->GameMode = GAMEMODE_OPTION;
	gs->BgChipHandle[0] = MakeBGChip(gs->CharHandle,0);
	gs->BgChipHandle[1] = MakeBGChip(gs->CharHandle,1);
	gs->Color = GetColor(255,255,0);
	gs->FadeTimer = 0;
	gs->FadeStart = 0;
	gs->KeyFlag_A = 1;
	gs->KeyFlag_UP = 1;
	gs->KeyFlag_DOWN = 1;
	gs->KeyFlag_L = 1;
	gs->KeyFlag_R = 1;
	gs->SelectVar = 0;
	SetFrequencySoundMem(NORMAL_FREQ,gs->SoundHandle[SOUND_BGM]);
}

// Option更新処理
void GameOptionUpdate(GameStatus *gs){
	int stick = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	const int cpWidth = -BG_CHIP_WIDTH * CHAR_SIZE;
	const int cpHeight = -BG_CHIP_HEIGHT * CHAR_SIZE;
	gs->BG_X--;
	if(gs->BG_X <= cpWidth) gs->BG_X = 0;
	gs->BG_Y--;
	if(gs->BG_Y <= cpHeight) gs->BG_Y = 0;
	// フェード処理
	if(gs->FadeStart == 1){
		if(gs->FadeTimer > FADEOUT_END - FADEOUT_START){
			gs->GameMode = GAMEMODE_TITLE;
			DestroyBGHandle(gs->BgChipHandle);
			SaveIniFile(&gs->Op,OPTION_FILE);
			StopSoundMem(gs->SoundHandle[SOUND_BGM]);
			InitTitle(gs);
		}
		gs->FadeTimer += FADE_SPEED;
	}else{
		// 上入力
		if(stick == PAD_INPUT_UP){
			if(gs->KeyFlag_UP == 0){
				gs->SelectVar--;
				if(gs->SelectVar < OPTION_BGMV) gs->SelectVar = OPTION_KEYH;
				gs->KeyFlag_UP = 1;
				PlaySoundMem(gs->SoundHandle[SOUND_KEYH],DX_PLAYTYPE_BACK);
			}
		}else gs->KeyFlag_UP = 0;
		// 下入力
		if(stick == PAD_INPUT_DOWN){
			if(gs->KeyFlag_DOWN == 0){
				gs->SelectVar++;
				if(gs->SelectVar > OPTION_KEYH) gs->SelectVar = OPTION_BGMV;
				gs->KeyFlag_DOWN = 1;
				PlaySoundMem(gs->SoundHandle[SOUND_KEYH],DX_PLAYTYPE_BACK);
			}
		}else gs->KeyFlag_DOWN = 0;
		// 右入力
		if(stick == PAD_INPUT_RIGHT){
			if(gs->KeyFlag_R == 0){
				ChangeOptionValue(&gs->Op,gs->SelectVar,1);
				gs->KeyFlag_R = 1;
				PlaySoundMem(gs->SoundHandle[SOUND_KEYH],DX_PLAYTYPE_BACK);
			}
		}else gs->KeyFlag_R = 0;
		// 左入力
		if(stick == PAD_INPUT_LEFT){
			if(gs->KeyFlag_L == 0){
				ChangeOptionValue(&gs->Op,gs->SelectVar,-1);
				gs->KeyFlag_L = 1;
				PlaySoundMem(gs->SoundHandle[SOUND_KEYH],DX_PLAYTYPE_BACK);
			}
		}else gs->KeyFlag_L = 0;
		// 決定キー入力
		if(stick == PAD_INPUT_A){
			if(gs->KeyFlag_A == 0){
				gs->FadeStart = 1;
				gs->KeyFlag_A = 1;
				PlaySoundMem(gs->SoundHandle[SOUND_KEYH],DX_PLAYTYPE_BACK);
			}
		}else gs->KeyFlag_A = 0;
		// BGMサンプル演奏
		if(gs->SelectVar < 2){
			SetVolume(gs,VOL_BGM,gs->Op.BgmVolume * VOL_PITCH);
			SetVolume(gs,VOL_SE,gs->Op.SeVolume * VOL_PITCH);
			if(CheckSoundMem(gs->SoundHandle[SOUND_BGM]) == 0){
				PlaySoundMem(gs->SoundHandle[SOUND_BGM],DX_PLAYTYPE_LOOP);
			}
		}else StopSoundMem(gs->SoundHandle[SOUND_BGM]);
	}
}

void OptionInit(Option *op){
	if(LoadIniFile(op,OPTION_FILE) == DXLIB_FALSE){
		op->BgmVolume = 5;
		op->SeVolume = 5;
		op->HardDrop = 1;
		op->Hold = 1;
		op->Ghost = 1;
		op->NextNum = 3;
		op->KeyRight = PAD_INPUT_A;
		op->KeyLeft = PAD_INPUT_B;
		op->KeyHold = PAD_INPUT_C;
		SaveIniFile(op,OPTION_FILE);
	}
}

int LoadIniFile(Option *op,char *str){
	FILE *fp;
	char buf[STRING_BUFFER];
	if(fopen_s(&fp,str,"r") != 0) return DXLIB_FALSE;
	while(fgets(buf,STRING_BUFFER,fp) != NULL){
		if(buf[0] != INI_COMMENT){
			const int param = GetIniParam(buf);
			switch(GetIniState(buf)){
				case OPTION_BGMV:
					op->BgmVolume = param;
					break;
				case OPTION_SEV:
					op->SeVolume = param;
					break;
				case OPTION_HDROP:
					op->HardDrop = param;
					break;
				case OPTION_HOLD:
					op->Hold = param;
					break;
				case OPTION_GHOST:
					op->Ghost = param;
					break;
				case OPTION_NEXT:
					op->NextNum = param;
					break;
				case OPTION_KEYL:
					op->KeyLeft = param;
					break;
				case OPTION_KEYR:
					op->KeyRight = param;
					break;
				case OPTION_KEYH:
					op->KeyHold = param;
					break;
			}
		}
	}
	fclose(fp);
	return DXLIB_TRUE;
}

int SaveIniFile(Option *op,char *str){
	FILE *fp;
	char buf[STRING_BUFFER];
	if(fopen_s(&fp,str,"w") != 0) return DXLIB_FALSE;
	sprintf_s(buf,STRING_BUFFER,"%s\n",INI_DEFAULT);
	fputs(buf,fp);
	for(int i=0;i<OPTION_MAX;i++){
		char *sp = NULL;
		int param = 0;
		switch(i){
			case OPTION_BGMV:
				sp = INI_STR_BGMV;
				param = op->BgmVolume;
				break;
			case OPTION_SEV:
				sp = INI_STR_SEV;
				param = op->SeVolume;
				break;
			case OPTION_HDROP:
				sp = INI_STR_HDROP;
				param = op->HardDrop;
				break;
			case OPTION_HOLD:
				sp = INI_STR_HOLD;
				param = op->Hold;
				break;
			case OPTION_GHOST:
				sp = INI_STR_GHOST;
				param = op->Ghost;
				break;
			case OPTION_NEXT:
				sp = INI_STR_NEXT;
				param = op->NextNum;
				break;
			case OPTION_KEYL:
				sp = INI_STR_KEYL;
				param = op->KeyLeft;
				break;
			case OPTION_KEYR:
				sp = INI_STR_KEYR;
				param = op->KeyRight;
				break;
			case OPTION_KEYH:
				sp = INI_STR_KEYH;
				param = op->KeyHold;
				break;
		}
		sprintf_s(buf,STRING_BUFFER,"%s%d\n",sp,param);
		fputs(buf,fp);
	}
	fclose(fp);
	return DXLIB_TRUE;
}

int GetIniParam(char *buf){
	char tmpstr[STRING_BUFFER];
	int len = 0,tmplen = 0;
	while(buf[len] != INI_TOKEN){
		if(buf[len] == NULL) return DXLIB_FALSE;
		len++;
	}
	len++;
	while(buf[len] != NULL){
		tmpstr[tmplen] = buf[len];
		len++;
		tmplen++;
	}
	tmpstr[tmplen] = NULL;
	return atoi(tmpstr);
}

int GetIniState(char *buf){
	int i;
	for(i=0;i<OPTION_MAX;i++){
		char *sp = NULL;
		int len = 0,maxlen;
		int count = 0;
		switch(i){
			case OPTION_BGMV:
				sp = INI_STR_BGMV;
				break;
			case OPTION_SEV:
				sp = INI_STR_SEV;
				break;
			case OPTION_HDROP:
				sp = INI_STR_HDROP;
				break;
			case OPTION_HOLD:
				sp = INI_STR_HOLD;
				break;
			case OPTION_GHOST:
				sp = INI_STR_GHOST;
				break;
			case OPTION_NEXT:
				sp = INI_STR_NEXT;
				break;
			case OPTION_KEYL:
				sp = INI_STR_KEYL;
				break;
			case OPTION_KEYR:
				sp = INI_STR_KEYR;
				break;
			case OPTION_KEYH:
				sp = INI_STR_KEYH;
				break;
		}
		maxlen = strlen(sp);
		while(sp[len] != NULL){
			if(buf[len] == sp[len]) count++;
			len++;
		}
		if(count == maxlen) break;
	}
	return i;
}

void ChangeOptionValue(Option *op,int index,int inc){
	int tmpkey;
	if((inc < -1)||(inc > 1)) return;
	switch(index){
		case OPTION_BGMV:
			op->BgmVolume += inc;
			if(op->BgmVolume < 0) op->BgmVolume = 0;
			else if(op->BgmVolume > OPT_BGM_NUM) op->BgmVolume = OPT_BGM_NUM;
			break;
		case OPTION_SEV:
			op->SeVolume += inc;
			if(op->SeVolume < 0) op->SeVolume = 0;
			else if(op->SeVolume > OPT_SE_NUM) op->SeVolume = OPT_BGM_NUM;
			break;
		case OPTION_HDROP:
			op->HardDrop += inc;
			if(op->HardDrop < 0) op->HardDrop = 0;
			else if(op->HardDrop > OPT_DEF_NUM) op->HardDrop = OPT_DEF_NUM;
			break;
		case OPTION_HOLD:
			op->Hold += inc;
			if(op->Hold < 0) op->Hold = 0;
			else if(op->Hold > OPT_DEF_NUM) op->Hold = OPT_DEF_NUM;
			break;
		case OPTION_GHOST:
			op->Ghost += inc;
			if(op->Ghost < 0) op->Ghost = 0;
			else if(op->Ghost > OPT_DEF_NUM) op->Ghost = OPT_DEF_NUM;
			break;
		case OPTION_NEXT:
			op->NextNum += inc;
			if(op->NextNum < 0) op->NextNum = 0;
			else if(op->NextNum > OPT_NEXT_NUM) op->NextNum = OPT_NEXT_NUM;
			break;
		case OPTION_KEYL:
			tmpkey = op->KeyLeft;
			if(inc < 0){
				if(op->KeyLeft == PAD_INPUT_B) op->KeyLeft = PAD_INPUT_A;
				else if(op->KeyLeft == PAD_INPUT_C) op->KeyLeft = PAD_INPUT_B;
			}else{
				if(op->KeyLeft == PAD_INPUT_A) op->KeyLeft = PAD_INPUT_B;
				else if(op->KeyLeft == PAD_INPUT_B) op->KeyLeft = PAD_INPUT_C;
			}
			if(op->KeyLeft == op->KeyRight) swap(&tmpkey,&op->KeyRight);
			if(op->KeyLeft == op->KeyHold) swap(&tmpkey,&op->KeyHold);
			break;
		case OPTION_KEYR:
			tmpkey = op->KeyRight;
			if(inc < 0){
				if(op->KeyRight == PAD_INPUT_B) op->KeyRight = PAD_INPUT_A;
				else if(op->KeyRight == PAD_INPUT_C) op->KeyRight = PAD_INPUT_B;
			}else{
				if(op->KeyRight == PAD_INPUT_A) op->KeyRight = PAD_INPUT_B;
				else if(op->KeyRight == PAD_INPUT_B) op->KeyRight = PAD_INPUT_C;
			}
			if(op->KeyRight == op->KeyLeft) swap(&tmpkey,&op->KeyLeft);
			if(op->KeyRight == op->KeyHold) swap(&tmpkey,&op->KeyHold);
			break;
		case OPTION_KEYH:
			tmpkey = op->KeyHold;
			if(inc < 0){
				if(op->KeyHold == PAD_INPUT_B) op->KeyHold = PAD_INPUT_A;
				else if(op->KeyHold == PAD_INPUT_C) op->KeyHold = PAD_INPUT_B;
			}else{
				if(op->KeyHold == PAD_INPUT_A) op->KeyHold = PAD_INPUT_B;
				else if(op->KeyHold == PAD_INPUT_B) op->KeyHold = PAD_INPUT_C;
			}
			if(op->KeyHold == op->KeyRight) swap(&tmpkey,&op->KeyRight);
			if(op->KeyHold == op->KeyLeft) swap(&tmpkey,&op->KeyLeft);
			break;
		default:
			return;
	}
}

void swap(int *a,int *b){
	int c;
	c = *a;
	*a = *b;
	*b = c;
}
