// gameupdate.cpp

#include "tetris.h"

void GameMainUpdate(GameStatus *gs){
	if(gs->AnimeTimer == 0){
		UserControl(gs);
		MoveTetriDrop(gs);
	}else{
		gs->AnimeTimer--;
		if(gs->EraseLineNum > 0){
			if(gs->AnimeTimer == L_ERASE_TIME){
				EraseBlockLine(gs->Stage,gs->EraseLines);
			}else if(gs->AnimeTimer == L_DROP_TIME){
				DropBlockLine(gs->Stage,gs->Bl_Y - 1);
				PlaySoundMem(gs->SoundHandle[SOUND_DROP],DX_PLAYTYPE_BACK);
			}else if(gs->AnimeTimer == L_FLASH_TIME){
				PlaySoundMem(gs->SoundHandle[SOUND_FLSH],DX_PLAYTYPE_BACK);
			}else if(gs->AnimeTimer == 0){
				UpdateGameParam(gs);
			}
		}
		if(gs->AnimeTimer <= 0){ 
			ResetTetriBlocks(gs);
			gs->AnimeTimer = 0;
		}
	}
	if(gs->ReplayMode == REPLAYMODE_PLAY){
		if(gs->FadeStart == 1){
			if(gs->FadeTimer > FADEOUT_END - FADEOUT_START){
				gs->GameMode = GAMEMODE_TITLE;
				StopSoundMem(gs->SoundHandle[SOUND_BGM]);
				InitTitle(gs);
			}
			gs->FadeTimer += FADE_SPEED;
		}else{
			if(GetJoypadInputState(DX_INPUT_KEY_PAD1) == PAD_INPUT_A){
				gs->FadeStart = 1;
				PlaySoundMem(gs->SoundHandle[SOUND_KEYH],DX_PLAYTYPE_BACK);
			}
		}
	}
	gs->GameTime++;
}

void GameInitFirst(GameStatus *gs){
	int ret;
	gs->BgHandle = LoadGraph(BG_FNAME);
	gs->TitleBgHandle = LoadGraph(TITLEBG_FNAME);
	gs->OptionCharHandle = LoadGraph(OPTIONCH_FNAME);
	ret = LoadCharacters(gs->CharHandle,CHAR_FNAME,CHARSET_SIZE_X,CHARSET_SIZE_Y,CHAR_SIZE);
	ret = LoadCharacters(gs->CharHandle1,CHAR1_FNAME,CHARSET_SIZE_X,CHARSET1_SIZE_Y,CHAR_SIZE);
	ret = LoadCharacters(gs->FontHandle,FONT_FNAME,FONTSET_SIZE_X,FONTSET_SIZE_Y,FONT_SIZE);
	LoadSoundFiles(gs->SoundHandle);
	SetLoopPosSoundMem(BGM_LOOP_POINT,gs->SoundHandle[SOUND_BGM]);
	gs->CurrentReplay = gs->ReplayStart = NULL;
	for(int i=0;i<SPEED_MAX;i++){
		gs->SpeedTable[i] = GetStrParam(SPEED_TABLE,i*3,2);
	}
	DestroyBGHandle(gs->BgChipHandle);
	if(LoadRankingFile(gs->PlayerRanking,RANKING_FILE) == DXLIB_FALSE){
		RankingInit(gs);
		SaveRankingFile(gs->PlayerRanking,RANKING_FILE);
	}
	LoadReplayFile(gs,REPLAY_FILE);
	OptionInit(&gs->Op);
	InitTitle(gs);
}

void InitStatus(GameStatus *gs){
	int s_index = STAGE_SIZE_Y * STAGE_SIZE_X;
	// ステージ初期化
	for(int i=0;i<s_index;i++) gs->Stage[i] = BLOCK_OFF;
	for(int i=0;i<STAGE_SIZE_Y;i++){
		int index = i * STAGE_SIZE_X;
		gs->Stage[index] = gs->Stage[index + 1] = BLOCK_WALL;
		gs->Stage[index + 12] = gs->Stage[index + 13] = BLOCK_WALL;
	}
	for(int i=0;i<(STAGE_SIZE_X*2);i++){
		int bot_index = s_index - (STAGE_SIZE_X * 2) + i;
		gs->Stage[bot_index] = BLOCK_WALL;
	}
	// 各パラメータ初期化
	gs->Speed = 0;
	gs->GameTime = 0;
	gs->Level = 1;
	gs->Score = 0;
	gs->GetLines = 0;
	gs->StackHeight = 0;
	gs->Hold = 0;
	gs->FadeStart = 0;
	gs->FadeTimer = 0;
	gs->GovCounter = 0;
	gs->HiScore = gs->PlayerRanking[0].Score;
	gs->Rank = 0;
	gs->KeyFlag_A = 1;
	gs->KeyFlag_B = 1;
	gs->KeyFlag_C = 0;
	gs->KeyFlag_L = 1;
	gs->KeyFlag_R = 1;
	gs->KeyFlag_UP = 1;
	gs->KeyRepeat_L = KEY_REPEAT_TIME;
	gs->KeyRepeat_R = KEY_REPEAT_TIME;
	// リプレイ用初期化処理
	if(gs->ReplayMode == REPLAYMODE_REC){
		DATEDATA dd;
		GetDateTime(&dd);
		gs->RandSeed = dd.Sec;
		ResetReplay(gs);
	}else{
		gs->CurrentReplay = gs->ReplayStart;
		gs->RecordIndex = 0;
	}
	SRand(gs->RandSeed);
	for(int i=0;i<NEXT_NUM;i++) gs->Next[i] = GetRand(TETRI_NUM - 1) + 1;
	ResetTetriBlocks(gs);
	SetVolume(gs,VOL_BGM,gs->Op.BgmVolume * VOL_PITCH);
	SetVolume(gs,VOL_SE,gs->Op.SeVolume * VOL_PITCH);
	PlaySoundMem(gs->SoundHandle[SOUND_BGM],DX_PLAYTYPE_LOOP);
}

void ResetTetriBlocks(GameStatus *gs){
	gs->Bl_Y = START_POS_Y;
	gs->Bl_X = START_POS_X;
	gs->PieceCenter = SetPattern(gs->Piece,RotateNext(gs));
	gs->RotateStatus = 0;
	gs->KeyFlag_UP = 1;
	gs->KeyFlag_DOWN = 1;
	gs->KeyFlag_C = 0;
	gs->EraseLineNum = 0;
	gs->AnimeTimer = 0;
	gs->WaitTime = gs->SpeedTable[gs->Speed];
	gs->HardDrop = 0;
	gs->GroundFlag = 0;
	if(GetCollision(gs->Piece,gs->Stage,gs->Bl_X,gs->Bl_Y) == DXLIB_TRUE){
		GameOverInit(gs);
	}
	PlaySoundMem(gs->SoundHandle[SOUND_BLIN],DX_PLAYTYPE_BACK);

}

int SetPattern(int *piece,int type){
	int returnValue = DXLIB_FALSE;
	for(int i=0;i<TETRI_SIZE;i++){
		for(int j=0;j<TETRI_SIZE;j++){
			piece[i*TETRI_SIZE+j] = BLOCK_OFF;
		}
	}
	switch(type){
	case PIECE_TYPE_1:
		piece[PIECE_1_1] = piece[PIECE_1_2] = piece[PIECE_1_3] = piece[PIECE_1_4] = PIECE_TYPE_1;
		returnValue = PIECE_CENTER_1;
		break;
	case PIECE_TYPE_2:
		piece[PIECE_2_1] = piece[PIECE_2_2] = piece[PIECE_2_3] = piece[PIECE_2_4] = PIECE_TYPE_2;
		returnValue = PIECE_CENTER_2;
		break;
	case PIECE_TYPE_3:
		piece[PIECE_3_1] = piece[PIECE_3_2] = piece[PIECE_3_3] = piece[PIECE_3_4] = PIECE_TYPE_3;
		returnValue = PIECE_CENTER_3;
		break;
	case PIECE_TYPE_4:
		piece[PIECE_4_1] = piece[PIECE_4_2] = piece[PIECE_4_3] = piece[PIECE_4_4] = PIECE_TYPE_4;
		returnValue = PIECE_CENTER_4;
		break;
	case PIECE_TYPE_5:
		piece[PIECE_5_1] = piece[PIECE_5_2] = piece[PIECE_5_3] = piece[PIECE_5_4] = PIECE_TYPE_5;
		returnValue = PIECE_CENTER_5;
		break;
	case PIECE_TYPE_6:
		piece[PIECE_6_1] = piece[PIECE_6_2] = piece[PIECE_6_3] = piece[PIECE_6_4] = PIECE_TYPE_6;
		returnValue = PIECE_CENTER_6;
		break;
	case PIECE_TYPE_7:
		piece[PIECE_7_1] = piece[PIECE_7_2] = piece[PIECE_7_3] = piece[PIECE_7_4] = PIECE_TYPE_7;
		returnValue = PIECE_CENTER_7;
		break;
	}
	return returnValue;
}

int LoadCharacters(int *gHandle,const char *str,int width,int height,int size){
	const int allNum = width * height;
	int retVal;
	retVal = LoadDivGraph(str,allNum,width,height,size,size,gHandle);
	return retVal;
}

int GetDigitNum(int num,int digit){
	char buf[STRING_BUFFER];
	int digit_max;
	int returnValue;
	sprintf_s(buf,STRING_BUFFER,"%d",num);
	digit_max = (int)strlen(buf);
	if(digit >= digit_max) return DXLIB_FALSE;
	returnValue = (int)(buf[(digit_max - digit) - 1] - '0');
	return returnValue;
}

void MoveTetriDrop(GameStatus *gs){
	const int waitDrop = gs->SpeedTable[gs->Speed];
	if(gs->GroundFlag == 0){
		if(GetCollision(gs->Piece,gs->Stage,gs->Bl_X,gs->Bl_Y + 1) == DXLIB_TRUE){
			gs->WaitTime = gs->SpeedTable[0];
			gs->GroundFlag = 1;
			PlaySoundMem(gs->SoundHandle[SOUND_BHIT],DX_PLAYTYPE_BACK);
		}
	}else{
		if(GetCollision(gs->Piece,gs->Stage,gs->Bl_X,gs->Bl_Y + 1) == DXLIB_FALSE){
			gs->WaitTime = waitDrop;
			gs->GroundFlag = 0;
		}
	}
	if(gs->HardDrop == 1){
		gs->Bl_Y = GetGhostPos(gs->Stage,gs->Piece,gs->Bl_X,gs->Bl_Y);
		gs->WaitTime = 0;
		PlaySoundMem(gs->SoundHandle[SOUND_HDRP],DX_PLAYTYPE_BACK);
	}
	gs->WaitTime--;
	if(gs->WaitTime < 0){
		gs->Bl_Y++;
		gs->WaitTime = waitDrop;
		if(GetCollision(gs->Piece,gs->Stage,gs->Bl_X,gs->Bl_Y) == DXLIB_TRUE){
			StackBlock(gs,gs->Bl_X,gs->Bl_Y-1);
			gs->EraseLineNum = ScanBlockLine(gs->Stage,gs->EraseLines,gs->Bl_Y-1);
			if(gs->EraseLineNum == 0) gs->AnimeTimer = BLOCK_RECAST;
			else gs->AnimeTimer = ANIME_TIME;
			if(gs->StackHeight >= PINCH_HEIGHT){
				SetFrequencySoundMem(PINCH_FREQ,gs->SoundHandle[SOUND_BGM]);
			}
		}
	}
}

int TetriRotate(GameStatus *gs,int rotate){
	int tmpPiece[TETRI_SIZE * TETRI_SIZE];
	int stat = gs->RotateStatus;
	const int pieceNum = TETRI_SIZE * TETRI_SIZE;
	const int ptype = gs->Next[0];
	const int rOffsetX[] = { 1, 0,-1, 0, 0};
	const int rOffsetY[] = { 0, 1, 0,-1, 0};
	int ox,oy,ov,oIndex;
	int offset_X = 0;
	int offset_Y = 0;
	if((ptype == PIECE_TYPE_1)||(ptype == PIECE_TYPE_3)||(ptype == PIECE_TYPE_4)){
		if(stat == 1) rotate = ROTATE_LEFT;
		else if(stat == 0) rotate = ROTATE_RIGHT;
	}
	for(int i=0;i<TETRI_SIZE;i++){
		for(int j=0;j<TETRI_SIZE;j++){
			int index;
			switch(rotate){
			case ROTATE_RIGHT:
				index = (12 + i) - (j * TETRI_SIZE);
				break;
			case ROTATE_LEFT:
				index = (3 - i) + (j * TETRI_SIZE);
				break;
			}
			tmpPiece[i*TETRI_SIZE + j] = gs->Piece[index];
		}
	}
	if(rotate == ROTATE_RIGHT){
		stat++;
		ov = 1;
		if(stat > 3) stat = 0;
	}else{
		stat--;
		ov = -1;
		if(stat < 0) stat = 3;
	}
	if(gs->PieceCenter == 0){
		oIndex = 4;
	}else if(gs->PieceCenter == 1){
		oIndex = stat;
	}else {
		oIndex = stat - 1;
		if(oIndex < 0) oIndex = 3;
	}
	if(ov == -1){
		oIndex = 3 - oIndex;
		if(oIndex < 0) oIndex = 4;
	}
	ox = gs->Bl_X + (rOffsetX[oIndex] * ov);
	oy = gs->Bl_Y + (rOffsetY[oIndex]);
	offset_Y = PieceOffsetY(tmpPiece,gs->Stage,ox,oy);
	offset_X = PieceOffsetX(tmpPiece,gs->Stage,ox,oy + offset_Y);
	if(offset_X == ROTATE_NOT) return DXLIB_FALSE;
	//if(GetCollision(tmpPiece,gs->Stage,ox,oy) == DXLIB_TRUE) return DXLIB_FALSE;
	for(int i=0;i<pieceNum;i++) gs->Piece[i] = tmpPiece[i];
	gs->RotateStatus = stat;
	gs->Bl_X = ox + offset_X;
	gs->Bl_Y = oy + offset_Y;
	PlaySoundMem(gs->SoundHandle[SOUND_ROTE],DX_PLAYTYPE_BACK);
	return DXLIB_TRUE;
}

int GetCollision(int *piece,int *stage,int x,int y){
	int colFlag = DXLIB_FALSE;
	for(int i=0;i<TETRI_SIZE;i++){
		for(int j=0;j<TETRI_SIZE;j++){
			int p_index = i * TETRI_SIZE + j;
			int s_index = (i + y) * STAGE_SIZE_X + (j + x);
			if(piece[p_index] > 0){
				if(stage[s_index] > 0) colFlag = DXLIB_TRUE;
			}
		}
	}
	return colFlag;
}

void StackBlock(GameStatus *gs,int x,int y){
	const int stageTop = STAGE_SIZE_X * STAGE_CLIP;
	const int stageBot = STAGE_SIZE_Y - STAGE_CLIP;
	int pieceHeight = 0;
	int stage_mask[] = {1,1,1,1,1,0,0,0,0,1,1,1,1,1};
	for(int i=0;i<TETRI_SIZE;i++){
		for(int j=0;j<TETRI_SIZE;j++){
			int p_index = i * TETRI_SIZE + j;
			int mask_index = j + x;
			int s_index = (i + y) * STAGE_SIZE_X + mask_index;
			if(gs->Piece[p_index] > BLOCK_OFF){
				if((s_index > stageTop)||(stage_mask[mask_index] == 0)){
					gs->Stage[s_index] = gs->Piece[p_index];
					if(pieceHeight == 0) pieceHeight = stageBot - (y + i);
				}
			}
		}
	}
	if(gs->StackHeight < pieceHeight) gs->StackHeight = pieceHeight;
}

int UserControl(GameStatus *gs){
	int vx = 0;
	int stick;
	if(gs->ReplayMode == REPLAYMODE_REC){
		stick = GetJoypadInputState(DX_INPUT_KEY_PAD1);
		RecordReplay(gs,stick);
	}else{
		stick = PlaybackReplay(gs);
		if(stick == DXLIB_FALSE){
			gs->GameMode = GAMEMODE_TITLE;
			return 0;
		}
	}
	// 左移動
	if(stick & PAD_INPUT_LEFT){
		if(gs->KeyFlag_L == 0){
			vx = -1;
			gs->KeyFlag_L = 1;
		}else{
			gs->KeyRepeat_L--;
			if(gs->KeyRepeat_L <= 0){
				gs->KeyFlag_L = 0;
				gs->KeyRepeat_L = 0;
			}
		}
	}else{
		gs->KeyFlag_L = 0;
		gs->KeyRepeat_L = KEY_REPEAT_TIME;
	}
	// 右移動
	if(stick & PAD_INPUT_RIGHT){
		if(gs->KeyFlag_R == 0){
			vx = 1;
			gs->KeyFlag_R = 1;
		}else{
			gs->KeyRepeat_R--;
			if(gs->KeyRepeat_R <= 0){
				gs->KeyFlag_R = 0;
				gs->KeyRepeat_R = 0;
			}
		}
	}else{
		gs->KeyFlag_R = 0;
		gs->KeyRepeat_R = KEY_REPEAT_TIME;
	}
	// 下入力(高速落下)
	if(stick & PAD_INPUT_DOWN){
		if(gs->KeyFlag_DOWN == 0){
			gs->WaitTime = 0;
			vx = 0;
		}
	}else{
		gs->KeyFlag_DOWN = 0;
	}
	// 上入力(ハードドロップ)
	if(gs->Op.HardDrop == 1){
		if(stick & PAD_INPUT_UP){
			if(gs->KeyFlag_UP == 0){
				gs->HardDrop = 1;
				gs->KeyFlag_UP = 1;
				vx = 0;
			}
		}else{
			gs->KeyFlag_UP = 0;
		}
	}
	// 座標計算
	if(GetCollision(gs->Piece,gs->Stage,gs->Bl_X+vx,gs->Bl_Y) == DXLIB_TRUE) vx = 0;
	gs->Bl_X += vx;
	// ボタン1（右回転）
	if(stick & gs->Op.KeyRight){
		if(gs->KeyFlag_A == 0){
			TetriRotate(gs,ROTATE_RIGHT);
			gs->KeyFlag_A = 1;
		}
	}else{
		gs->KeyFlag_A = 0;
	}
	// ボタン2（左回転）
	if(stick & gs->Op.KeyLeft){
		if(gs->KeyFlag_B == 0){
			TetriRotate(gs,ROTATE_LEFT);
			gs->KeyFlag_B = 1;
		}
	}else{
		gs->KeyFlag_B = 0;
	}
	// ボタン3（HOLD機能）
	if(gs->Op.Hold == 1){
		if(stick & gs->Op.KeyHold){
			if(gs->KeyFlag_C == 0){
				if(GetHoldBlock(gs) != DXLIB_FALSE) gs->KeyFlag_C = 1;
			}
		}
	}
	return stick;
}

int RotateNext(GameStatus *gs){
	for(int i=1;i<NEXT_NUM;i++) gs->Next[i-1] = gs->Next[i];
	gs->Next[NEXT_NUM-1] = GetRandomPiece(gs->Next);
	return gs->Next[0];
}

int ScanBlockLine(int *stage,int *eraseLines,int row){
	int lines = 0;
	for(int i=0;i<STAGE_SIZE_Y;i++) eraseLines[i] = BLOCK_OFF;
	for(int i=0;i<TETRI_SIZE;i++){
		int count = 0;
		if(CheckBlockLine(stage,row + i) == C_LINE_FULL){
			eraseLines[i + row] = BLOCK_ENABLE;
			lines++;
		}
	}
	return lines;
}

void EraseBlockLine(int *stage,int *eraseLines){
	const int stageBot = STAGE_SIZE_Y - STAGE_CLIP;
	const int stageTop = STAGE_CLIP;
	for(int i=stageTop;i<stageBot;i++){
		if(eraseLines[i] == BLOCK_ENABLE){
			for(int j=STAGE_CLIP;j<(STAGE_SIZE_X-STAGE_CLIP);j++){
				int index = i * STAGE_SIZE_X + j;
				stage[index] = BLOCK_OFF;
			}
		}
	}
}

void DropBlockLine(int *stage,int row){
	int scanY = row + TETRI_SIZE - 1;
	int scanLimit = TETRI_SIZE;
	const int stageLeft = STAGE_CLIP;
	const int stageRight = STAGE_SIZE_X - STAGE_CLIP;
	const int stageTop = STAGE_CLIP;
	const int stageBot = STAGE_SIZE_Y - STAGE_CLIP;
	const int scanEnd = row;
	if(scanY >= stageBot) scanY = stageBot - 1;
	while((scanY >= scanEnd)&&(scanLimit >= 0)){
		if(CheckBlockLine(stage,scanY) == C_LINE_EMPTY){
			for(int i=scanY;i>stageTop;i--){
				CopyBlockLine(stage,i - 1,i);
			}
			for(int i=stageLeft;i<stageRight;i++){
				const int index = STAGE_CLIP * STAGE_SIZE_X + i;
				stage[index] = BLOCK_OFF;
			}
		}
		if(CheckBlockLine(stage,scanY) != C_LINE_EMPTY){
			scanY--;
		}
		scanLimit--;
	}
}

int UpdateGameParam(GameStatus *gs){
	gs->Score += gs->EraseLineNum * gs->EraseLineNum * 10;
	if(gs->Score > gs->HiScore) gs->HiScore = gs->Score;
	gs->GetLines += gs->EraseLineNum;
	gs->StackHeight -= gs->EraseLineNum;
	if(gs->GetLines >= (gs->Level * LEVEL_UP_LINES)){
		gs->Level++;
		gs->Speed++;
		if(gs->Speed >= SPEED_MAX) gs->Speed = 5;
		PlaySoundMem(gs->SoundHandle[SOUND_LVUP],DX_PLAYTYPE_BACK);
	}
	if(gs->StackHeight < PINCH_HEIGHT){
		SetFrequencySoundMem(NORMAL_FREQ,gs->SoundHandle[SOUND_BGM]);
	}

	return gs->EraseLineNum;
}
void CopyBlockLine(int *stage,int orgLine,int copyLine){
	const int stageLeft = STAGE_CLIP;
	const int stageRight = STAGE_SIZE_X - STAGE_CLIP;
	const int orgDes = orgLine * STAGE_SIZE_X;
	const int copyDes = copyLine * STAGE_SIZE_X;
	for(int i=stageLeft;i<stageRight;i++){
		int s_index = orgDes + i;
		int c_index = copyDes + i;
		stage[c_index] = stage[s_index];
	}
}

int CheckBlockLine(int *stage,int checkLine){
	const int stageLeft = STAGE_CLIP;
	const int stageRight = STAGE_SIZE_X - STAGE_CLIP;
	const int checkDes = checkLine * STAGE_SIZE_X;
	int count = 0;
	for(int i=stageLeft;i<stageRight;i++){
		int index = checkDes + i;
		if((stage[index] > 0)&&(stage[index] < 9)) count++;
	}
	return count;
}

int GetGhostPos(int *stage,int *piece,int x,int y){
	while(GetCollision(piece,stage,x,y) == DXLIB_FALSE) y++;
	return --y;
}

int GetHoldBlock(GameStatus *gs){
	int tmpHold;
	int offset_X = 0;
	int tmpPiece[TETRI_SIZE * TETRI_SIZE];
	if(gs->Hold == 0){
		gs->Hold = gs->Next[0];
		gs->AnimeTimer = BLOCK_RECAST;
	}else{
		tmpHold = gs->Hold;
		SetPattern(tmpPiece,tmpHold);
		offset_X = PieceOffsetX(tmpPiece,gs->Stage,gs->Bl_X,gs->Bl_Y);
		if(offset_X == ROTATE_NOT) return DXLIB_FALSE;
		gs->Bl_X += offset_X;
		gs->Hold = gs->Next[0];
		gs->Next[0] = tmpHold;
		SetPattern(gs->Piece,gs->Next[0]);
	}
	PlaySoundMem(gs->SoundHandle[SOUND_HOLD],DX_PLAYTYPE_BACK);
	return gs->Hold;
}

int PieceOffsetX(int *piece,int *stage,int x,int y){
	int offset = 0;
	if(GetCollision(piece,stage,x,y) == DXLIB_FALSE) return 0;
	for(int i=-2;i<=2;i++){
		int of_x = x + i;
		int returnValue;
		if(of_x < 0) of_x = 0;
		else if(of_x > STAGE_SIZE_X - TETRI_SIZE) of_x = STAGE_SIZE_X - TETRI_SIZE;
		returnValue = GetCollision(piece,stage,of_x,y);
		if(returnValue == DXLIB_FALSE){
			if(i == 0) return 0;
			else if(offset == 0) offset = i;
		}
	}
	if(offset == 0) return ROTATE_NOT;
	return offset;
}

int PieceOffsetY(int *piece,int *stage,int x,int y){
	int offset = 0;
	if(GetCollision(piece,stage,x,y) == DXLIB_FALSE) return 0;
	for(int i=1;i<=2;i++){
		int of_y = y - i;
		int returnValue;
		if(of_y > STAGE_SIZE_Y - TETRI_SIZE) of_y = STAGE_SIZE_Y - TETRI_SIZE;
		returnValue = GetCollision(piece,stage,x,of_y);
		if(returnValue == DXLIB_FALSE){
			if(offset == 0) offset = -i;
		}
	}
	return offset;
}

int LoadSoundFiles(int *sHandle){
	sHandle[SOUND_BGM] = LoadSoundMem(S_FN_BGM);
	sHandle[SOUND_BLIN] = LoadSoundMem(S_FN_BLIN);
	sHandle[SOUND_BHIT] = LoadSoundMem(S_FN_BHIT);
	sHandle[SOUND_DROP] = LoadSoundMem(S_FN_DROP);
	sHandle[SOUND_HOLD] = LoadSoundMem(S_FN_HOLD);
	sHandle[SOUND_ROTE] = LoadSoundMem(S_FN_ROTE);
	sHandle[SOUND_FLSH] = LoadSoundMem(S_FN_FLSH);
	sHandle[SOUND_KEYH] = LoadSoundMem(S_FN_KEYH);
	sHandle[SOUND_HDRP] = LoadSoundMem(S_FN_HDRP);
	sHandle[SOUND_LVUP] = LoadSoundMem(S_FN_LVUP);
	sHandle[SOUND_GOVR] = LoadSoundMem(S_FN_GOVR);
	for(int i=0;i<SOUND_NUM;i++){
		if(sHandle[i] == DXLIB_FALSE) return DXLIB_FALSE;
	}
	return DXLIB_TRUE;
}

int GetRandomPiece(int *next){
	int rnd,repeat= 1;
	while(repeat){
		rnd = GetRand(TETRI_NUM - 1) + 1;
		repeat = 0;
		for(int i=1;i<NEXT_NUM;i++){
			if(rnd == next[i]) repeat = 1;
		}
	}
	return rnd;
}

void GameOverUpdate(GameStatus *gs){
	int stick = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	if(gs->GovCounter > GOVERDEMO_END){
		if((stick > 0)&&(stick != PAD_INPUT_M)){
			if(gs->KeyFlag_A == 0){
				gs->Rank = CheckRanking(gs->PlayerRanking,gs->Score,gs->GetLines);
				if((gs->Rank != DXLIB_FALSE)&&(gs->ReplayMode == REPLAYMODE_REC)){
					StartRankingInput(gs);
				}else InitTitle(gs);
				gs->KeyFlag_A = 1;
				PlaySoundMem(gs->SoundHandle[SOUND_KEYH],DX_PLAYTYPE_BACK);
			}
		}else gs->KeyFlag_A = 0;
		gs->FadeTimer++;
		if(gs->FadeTimer > GOVFADE_END) gs->FadeTimer = GOVFADE_END;
	}else if(gs->GovCounter == GOVERDEMO_END){
		int tetriMax = TETRI_SIZE * TETRI_SIZE;
		for(int i=0;i<tetriMax;i++){
			if(gs->Piece[i] > 0) gs->Piece[i] = BLOCK_WALL;
		}
	}else{
		int gy = STAGE_SIZE_Y - STAGE_CLIP - gs->GovCounter - 1;
		int stg_left = STAGE_SIZE_X - STAGE_CLIP;
		for(int i=STAGE_CLIP;i<stg_left;i++){
			int index = gy * STAGE_SIZE_X + i;
			if(gs->Stage[index] > 0) gs->Stage[index] = BLOCK_WALL;
		}
	}
	if((gs->GameTime % GOVERDEMO_TIME) == 0) gs->GovCounter++;
	gs->GameTime++;
}

void SetBlockMask(int *mask,int x,int y){
	int stage_mask[] = {1,1,1,1,1,0,0,0,0,1,1,1,1,1};
	if(y == 0){
		for(int i=0;i<TETRI_SIZE;i++){
			int index = i + x;
			mask[i] = MASK_ON;
			mask[i + TETRI_SIZE] = stage_mask[index];
		}
	}else if(y == 1){
		for(int i=0;i<TETRI_SIZE;i++){
			int index = i + x;
			mask[i] = stage_mask[index];
			mask[i + TETRI_SIZE] = MASK_OFF;
		}
	}
}

void RecordReplay(GameStatus *gs,int stick){
	Replay *rp;
	if(stick != 0){
		if(gs->ReplayStart == NULL){
			rp = new Replay;
			rp->Next = NULL;
			rp->Stick = stick;
			rp->GameTime = gs->GameTime;
			gs->ReplayStart = gs->CurrentReplay = rp;
		}else{
			rp = gs->CurrentReplay;
			rp->Next = new Replay;
			rp->Next->Next = NULL;
			rp->Next->GameTime = gs->GameTime;
			rp->Next->Stick = stick;
			gs->CurrentReplay = rp->Next;
		}
		gs->RecordIndex++;
	}
}

int PlaybackReplay(GameStatus *gs){
	Replay *rp = gs->CurrentReplay;
	if(rp == NULL) goto END_OF_RECORD;
	if(rp->GameTime == gs->GameTime){
		gs->CurrentReplay = rp->Next;
		gs->RecordIndex++;
		return rp->Stick;
	}
END_OF_RECORD:
	return 0;
}

void ResetReplay(GameStatus *gs){
	Replay *rp = gs->ReplayStart;
	gs->RecordIndex = 0;
	if(rp == NULL) return;
	while(rp->Next != NULL){
		Replay *rt = rp->Next;
		delete rp;
		rp = rt;
	}
	delete rp;
	gs->ReplayStart = gs->CurrentReplay = NULL;
}

void GameOverInit(GameStatus *gs){
	StopSoundMem(gs->SoundHandle[SOUND_BGM]);
	SetFrequencySoundMem(NORMAL_FREQ,gs->SoundHandle[SOUND_BGM]);
	PlaySoundMem(gs->SoundHandle[SOUND_GOVR],DX_PLAYTYPE_BACK);
	gs->GameMode = GAMEMODE_GOV;
	gs->FadeStart = 0;
	gs->FadeTimer = 0;
}

int LoadRankingFile(Ranking *rp,char *str){
	FILE *fp;
	char buf[STRING_BUFFER];
	if(fopen_s(&fp,str,"r") != 0) return DXLIB_FALSE;
	for(int i=0;i<RANK_MAX;i++){
		fgets(buf,STRING_BUFFER,fp);
		rp[i].Date.Year = GetStrParam(buf,3,4);
		rp[i].Date.Mon = GetStrParam(buf,8,2);
		rp[i].Date.Day = GetStrParam(buf,11,2);
		rp[i].Date.Hour = GetStrParam(buf,14,2);
		rp[i].Date.Min = GetStrParam(buf,17,2);
		rp[i].Score = GetStrParam(buf,20,7);
		rp[i].Lines = GetStrParam(buf,28,4);
		for(int j=0;j<RANKING_NAME;j++){
			int index = 33 + j * 3;
			rp[i].Name[j] = GetStrParam(buf,index,2);
		}
	}
	fclose(fp);
	return DXLIB_TRUE;
}

int SaveRankingFile(Ranking *rp,char *str){
	FILE *fp;
	char buf[STRING_BUFFER];
	if(fopen_s(&fp,str,"w") != 0) return DXLIB_FALSE;
	for(int i=0;i<RANK_MAX;i++){
		const int rank = i + 1;
		const int year = rp[i].Date.Year;
		const int month = rp[i].Date.Mon;
		const int day = rp[i].Date.Day;
		const int hour = rp[i].Date.Hour;
		const int minit = rp[i].Date.Min;
		const int score = rp[i].Score;
		const int line = rp[i].Lines;
		int *name = rp[i].Name;
		sprintf_s(buf,STRING_BUFFER
				,"%02d,%04d,%02d,%02d,%02d,%02d,%07d,%04d,%02d,%02d,%02d,%02d,%02d\n"
			    ,rank,year,month,day,hour,minit,score,line
				,name[0],name[1],name[2],name[3],name[4]
		);
		fputs(buf,fp);
	}
	fclose(fp);
	return DXLIB_TRUE;
}

int GetStrParam(char *str,int start,int digit){
	char buf[STRING_BUFFER];
	const int len = strlen(str);
	int endpos = start + digit;
	if(start > len) return DXLIB_FALSE;
	if(endpos > len){
		digit = len - start;
		endpos = len;
	}
	for(int i=start;i<endpos;i++){
		const int index = i - start;
		buf[index] = str[i];
		buf[index + 1] = NULL;
	}
	return atoi(buf);
}

int LoadReplayFile(GameStatus *gs,char *str){
	FILE *fp;
	int count = 0;
	int seed;
	if(fopen_s(&fp,str,"rb") != 0) return DXLIB_FALSE;
	ResetReplay(gs);
	fread(&seed,sizeof(int),1,fp);
	gs->RandSeed = seed;
	while(feof(fp) == 0){
		int time;
		int stick;
		fread(&time,sizeof(int),1,fp);
		fread(&stick,sizeof(int),1,fp);
		gs->GameTime = time;
		RecordReplay(gs,stick);
		count++;
	}
	fclose(fp);
	return DXLIB_TRUE;
}

int SaveReplayFile(GameStatus *gs,char *str){
	FILE *fp;
	Replay *rp = gs->ReplayStart;
	int time,stick;
	const int seed = gs->RandSeed;
	if(fopen_s(&fp,str,"wb") != 0) return DXLIB_FALSE;
	fwrite(&seed,sizeof(int),1,fp);
	while(rp->Next !=NULL){
		time = rp->GameTime;
		stick = rp->Stick;
		fwrite(&time,sizeof(int),1,fp);
		fwrite(&stick,sizeof(int),1,fp);
		rp = rp->Next;
	}
	time = rp->GameTime;
	stick = rp->Stick;
	fwrite(&time,sizeof(int),1,fp);
	fwrite(&stick,sizeof(int),1,fp);
	fclose(fp);
	return DXLIB_TRUE;
}

void SetVolume(GameStatus *gs,int mode,int volume){
	if(mode == 0){
		ChangeVolumeSoundMem(volume,gs->SoundHandle[SOUND_BGM]);
	}else{
		for(int i=SOUND_BLIN;i<SOUND_NUM;i++){
			ChangeVolumeSoundMem(volume,gs->SoundHandle[i]);
		}
	}
}