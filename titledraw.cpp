// titledraw.cpp

#include "tetris.h"

void GameTitleDraw(GameStatus *gs){
	DrawGraph(0,0,gs->TitleBgHandle,FALSE);
	DrawTransBox(276,296,400,380,0,G_BOX_ALPHA);
	DrawTileChar(gs->CharHandle,300,300,G_START_START,G_START_WIDTH,1);
	if(gs->ReplayStart == NULL) SetDrawBlendMode(DX_BLENDMODE_ALPHA,FADEOUT_START);
	DrawTileChar(gs->CharHandle,300,320,G_REPLAY_START,G_REPLAY_WIDTH,1);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
	DrawTileChar(gs->CharHandle,300,340,G_RANKING_START,G_RANKING_WIDTH,1);
	DrawTileChar(gs->CharHandle,300,360,G_OPTION_START,G_OPTION_WIDTH,1);
	DrawGraph(TMENU_CUR_X,gs->Cursol_Pos,gs->CharHandle[G_CURSOL_CODE],TRUE);
	if(gs->FadeStart == 1){
		DrawTransBox(0,0,640,480,0,gs->FadeTimer + FADEOUT_START);
	}
}

void GameOptionDraw(GameStatus *gs){
	const int bx = RANK_TBOX_X;
	const int by = RANK_TBOX_X;
	const int bx1 = 640 - RANK_TBOX_X;
	const int by1 = 480 - RANK_TBOX_X;
	DrawScrollBG(gs);
	DrawTransBox(bx,by,bx1,by1,0,G_BOX_ALPHA);
	DrawGraph(64,32,gs->OptionCharHandle,TRUE);
	for(int i=0;i<OPTION_MAX;i++){
		Option *op = &gs->Op;
		int x,y,x1,y1;
		switch(i){
			case OPTION_BGMV:
				x = op->BgmVolume;
				break;
			case OPTION_SEV:
				x = op->SeVolume;
				break;
			case OPTION_HDROP:
				x = op->HardDrop;
				break;
			case OPTION_HOLD:
				x = op->Hold;
				break;
			case OPTION_GHOST:
				x = op->Ghost;
				break;
			case OPTION_NEXT:
				x = op->NextNum;
				break;
			case OPTION_KEYL:
				if(op->KeyLeft == PAD_INPUT_A) x = 0;
				else if(op->KeyLeft == PAD_INPUT_B) x = 1;
				else x = 2;
				break;
			case OPTION_KEYR:
				if(op->KeyRight == PAD_INPUT_A) x = 0;
				else if(op->KeyRight == PAD_INPUT_B) x = 1;
				else x = 2;
				break;
			case OPTION_KEYH:
				if(op->KeyHold == PAD_INPUT_A) x = 0;
				else if(op->KeyHold == PAD_INPUT_B) x = 1;
				else x = 2;
				break;
		}
		if(i < 6){
			x1 = CHAR_SIZE * 3;
		}else{
			x1 = CHAR_SIZE * 5;
		}
		x *= x1;
		x += 160 + 64;
		x1 = x + x1;
		y = i * 16;
		if(i < 6) y += 80 + 32;
		else y += 96 + 32;
		y1 = y + CHAR_SIZE;
		if(gs->SelectVar == i) DrawBox(x,y,x1,y1,GetColor(255,0,0),FALSE);
		else DrawBox(x,y,x1,y1,gs->Color,FALSE);
	}
	if(gs->FadeStart == 1){
		DrawTransBox(0,0,640,480,0,gs->FadeTimer + FADEOUT_START);
	}
}