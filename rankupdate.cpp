// rankupdate.cpp

#include "tetris.h"

void GameRankingUpdate(GameStatus *gs){
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
			InitTitle(gs);
		}
		gs->FadeTimer += FADE_SPEED;
	}else{
		// 決定キー入力
		if(stick == PAD_INPUT_A){
			if(gs->KeyFlag_A == 0){
				gs->FadeStart = 1;
				gs->KeyFlag_A = 1;
				PlaySoundMem(gs->SoundHandle[SOUND_KEYH],DX_PLAYTYPE_BACK);
			}
		}else gs->KeyFlag_A = 0;
	}
}

void RankingStart(GameStatus *gs){
	gs->BG_X = 0;
	gs->BG_Y = 0;
	gs->GameMode = GAMEMODE_RANK;
	gs->BgChipHandle[0] = MakeBGChip(gs->CharHandle,0);
	gs->BgChipHandle[1] = MakeBGChip(gs->CharHandle,1);
	gs->Color = GetColor(255,255,255);
	gs->FadeTimer = 0;
	gs->FadeStart = 0;
	gs->KeyFlag_A = 1;
}

int CheckRanking(Ranking *rp,int score,int lines){
	int i;
	for(i=0;i<RANK_MAX;i++){
		if(rp[i].Score < score) return i;
		else if(rp[i].Score == score){
			if(rp[i].Lines > lines) return i;
		}
	}
	return DXLIB_FALSE;
}

void StartRankingInput(GameStatus *gs){
	Ranking *rp = gs->PlayerRanking;
	gs->SelectVar = CHAR_A;
	gs->Cursol_Pos = 0;
	gs->Cursol_Pos2 = 0;
	gs->FadeStart = 0;
	gs->KeyFlag_A = 1;
	gs->KeyFlag_B = 1;
	gs->KeyFlag_L = 0;
	gs->KeyFlag_R = 0;
	gs->GameMode = GAMEMODE_RANKIN;
	for(int i=RANK_MAX - 2;i>=gs->Rank;i--){
		rp[i + 1].Date.Year = rp[i].Date.Year;
		rp[i + 1].Date.Mon = rp[i].Date.Mon;
		rp[i + 1].Date.Day = rp[i].Date.Day;
		rp[i + 1].Date.Hour = rp[i].Date.Hour;
		rp[i + 1].Date.Min = rp[i].Date.Min;
		rp[i + 1].Score = rp[i].Score;
		rp[i + 1].Lines = rp[i].Lines;
		for(int j=0;j<RANKING_NAME;j++){
			rp[i + 1].Name[j] = rp[i].Name[j];
		}
	}
	for(int i=0;i<RANKING_NAME;i++){
		rp[gs->Rank].Name[i] = CHAR_SPACE;
	}
}

void GameInputRankingUpdate(GameStatus *gs){
	int *np = gs->PlayerRanking[gs->Rank].Name;
	int stick = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	// 加速キー
	if(stick & PAD_INPUT_B){
		if(gs->KeyFlag_B == 0){
			gs->KeyFlag_B = 12;
		}
	}else gs->KeyFlag_B = 0;
	// 左移動
	if(gs->KeyFlag_L == 0){
		if(stick & PAD_INPUT_LEFT){
			gs->KeyFlag_L = 1;
		}
	}else gs->Cursol_Pos += 4 + gs->KeyFlag_B;
	// 右移動
	if(gs->KeyFlag_R == 0){
		if(stick & PAD_INPUT_RIGHT){
			gs->KeyFlag_R = 1;
		}
	}else gs->Cursol_Pos -= 4 + gs->KeyFlag_B;
	// パラメータ更新処理
	if(gs->Cursol_Pos > R_INP_SIZE1){
		gs->SelectVar--;
		gs->Cursol_Pos = 0;
		gs->KeyFlag_L = 0;
		if(gs->SelectVar < CHAR_0) gs->SelectVar = CHAR_END;
		PlaySoundMem(gs->SoundHandle[SOUND_KEYH],DX_PLAYTYPE_BACK);
	}else if(gs->Cursol_Pos < -R_INP_SIZE1){
		gs->SelectVar++;
		gs->Cursol_Pos = 0;
		gs->KeyFlag_R = 0;
		if(gs->SelectVar > CHAR_END) gs->SelectVar = CHAR_0;
		PlaySoundMem(gs->SoundHandle[SOUND_KEYH],DX_PLAYTYPE_BACK);
	}
	// フェード処理
	gs->FadeTimer++;
	if(gs->FadeTimer > GOVFADE_END) gs->FadeTimer = GOVFADE_END;
	// 決定キー
	if(stick == PAD_INPUT_A){
		if(gs->KeyFlag_A == 0){
			switch(gs->SelectVar){
				case CHAR_BACK:
					np[gs->Cursol_Pos2] = CHAR_SPACE;
					gs->Cursol_Pos2--;
					if(gs->Cursol_Pos2 < 0) gs->Cursol_Pos2 = 0;
					break;
				case CHAR_END:
					if(gs->Cursol_Pos2 > 0){ 
						UpdateRanking(gs);
						RankingStart(gs);
					}else gs->SelectVar = CHAR_0;
					break;
				default:
					np[gs->Cursol_Pos2] = gs->SelectVar;
					gs->Cursol_Pos2++;
					if(gs->Cursol_Pos2 >= RANKING_NAME){
						gs->Cursol_Pos2 = RANKING_NAME - 1;
						gs->SelectVar = CHAR_END;
					}
					break;
			}
			gs->KeyFlag_A = 1;
			PlaySoundMem(gs->SoundHandle[SOUND_KEYH],DX_PLAYTYPE_BACK);
		}
	}else gs->KeyFlag_A = 0;
}

void UpdateRanking(GameStatus *gs){
	Ranking *rp = &gs->PlayerRanking[gs->Rank];
	int *np = rp->Name;
	GetDateTime(&rp->Date);
	rp->Score = gs->Score;
	rp->Lines = gs->GetLines;
	SaveRankingFile(gs->PlayerRanking,RANKING_FILE);
}

void RankingInit(GameStatus *gs){
	CharGraph cg = CHAR_UBAR;
	Ranking *rp = gs->PlayerRanking;
	for(int i=0;i<RANK_MAX;i++){
		rp[i].Score = HISCORE - i * 200;
		rp[i].Date.Year = DEFAULT_YEAR;
		rp[i].Date.Mon = DEFAULT_MONTH;
		rp[i].Date.Day = DEFAULT_DAY;
		rp[i].Date.Hour = DEFAULT_HOUR;
		rp[i].Date.Min = DEFAULT_MINIT;
		rp[i].Lines = rp[i].Score / 10;
		for(int j=0;j<RANKING_NAME;j++) rp[i].Name[j] = cg;
	}
}

