// rankdraw.cpp

#include "tetris.h"

void GameRankingDraw(GameStatus *gs){
	const int rcy = RANK_CAPTION_Y;
	const int rcy1 = rcy + CHAR_SIZE;
	const int bx = RANK_TBOX_X;
	const int by = RANK_TBOX_X;
	const int bx1 = 640 - RANK_TBOX_X;
	const int by1 = 480 - RANK_TBOX_X;
	int rlx = RANK_LIST_POS_X;
	int rly = RANK_LIST_POS_Y;
	DrawScrollBG(gs);
	DrawTransBox(bx,by,bx1,by1,0,G_BOX_ALPHA);
	DrawTileChar(gs->CharHandle,RANK_TITLE_X,RANK_TITLE_Y,G_RANKING_START,G_RANKING_WIDTH,1);
	SetDrawBlendMode(DX_BLENDMODE_ADD,255);
	DrawExtendGraph(bx,rcy,bx1,rcy1,gs->CharHandle[G_GRADATION],TRUE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,100);
	DrawLine(R_LINE_DATE_X,rcy,R_LINE_DATE_X,R_LINE_BOT,gs->Color);
	DrawLine(R_LINE_SCORE_X,rcy,R_LINE_SCORE_X,R_LINE_BOT,gs->Color);
	DrawLine(R_LINE_LINE_X,rcy,R_LINE_LINE_X,R_LINE_BOT,gs->Color);
	DrawLine(R_LINE_NAME_X,rcy,R_LINE_NAME_X,R_LINE_BOT,gs->Color);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
	DrawTileChar(gs->CharHandle,R_CAP_RANK_X,rcy,G_RANK_RANK_S,G_RANK_RANK_W,1);
	DrawTileChar(gs->CharHandle,R_CAP_DATE_X,rcy,G_RANK_DATE_S,G_RANK_DATE_W,1);
	DrawTileChar(gs->CharHandle,R_CAP_SCORE_X,rcy,G_RANK_SCORE_S,G_RANK_SCORE_W,1);
	DrawTileChar(gs->CharHandle,R_CAP_LINE_X,rcy,G_RANK_LINE_S,G_RANK_LINE_W,1);
	DrawTileChar(gs->CharHandle,R_CAP_NAME_X,rcy,G_RANK_NAME_S,G_RANK_NAME_W,1);
	for(int i=0;i<RANK_MAX;i++){
		DrawRankingLine(gs,rlx,rly + i * 32,i);
	}
	if(gs->FadeStart == 1){
		DrawTransBox(0,0,640,480,0,gs->FadeTimer + FADEOUT_START);
	}
}

void DrawScrollBG(GameStatus *gs){
	const int cpWidth = BG_CHIP_WIDTH * CHAR_SIZE;
	const int cpHeight = BG_CHIP_HEIGHT * CHAR_SIZE;
	for(int i=0;i<BG_SIZE_Y;i++){
		int logo = i % 2;
		for(int j=0;j<BG_SIZE_X;j++){
			int cy = gs->BG_Y + i * cpHeight;
			int cx = gs->BG_X + j * cpWidth;
			DrawGraph(cx,cy,gs->BgChipHandle[logo],FALSE);
			if(logo == 1) logo = 0;
			else logo = 1;
		}
	}
}

int MakeBGChip(int *gHandle,int logo){
	int makeHandle;
	const int mx = BG_CHIP_WIDTH * CHAR_SIZE;
	const int my = BG_CHIP_HEIGHT * CHAR_SIZE;
	const int ly = 3 * CHAR_SIZE;
	for(int i=0;i<BG_CHIP_WIDTH;i++){
		int ch_index = i+1;
		for(int j=0;j<BG_CHIP_HEIGHT;j++){
			int cx = j * CHAR_SIZE;
			int cy = i * CHAR_SIZE;
			ch_index++;
			if(ch_index > 8) ch_index = 1;
			DrawGraph(cx,cy,gHandle[ch_index],FALSE);
		}
	}
	if(logo == 1){
		DrawTileChar(gHandle,0,ly,G_TITLE_START,G_TITLE_WIDTH,G_TITLE_HEIGHT);
	}
	makeHandle = MakeGraph(mx,my);
	GetDrawScreenGraph(0,0,mx,my,makeHandle);
	return makeHandle;
}

void DrawNameString(int *gHandle,int x,int y,int *str){
	for(int i=0;i<RANKING_NAME;i++){
		int sx = x + i * FONT_SIZE;
		DrawGraph(sx,y,gHandle[str[i]],TRUE);
	}
}

void DrawRankingLine(GameStatus *gs,int x,int y,int rank){
	int dx = x + R_DATE_POS_X;
	CharGraph cg = CHAR_SLASH;
	Ranking *rp = &gs->PlayerRanking[rank];
	for(int i = 0;i<5;i++){
		switch(i){
			case 0:
				DrawNumber(gs->FontHandle,dx,y,rp->Date.Year,2);
				break;
			case 1:
				DrawNumber(gs->FontHandle,dx,y,rp->Date.Mon,2);
				break;
			case 2:
				DrawNumber(gs->FontHandle,dx,y,rp->Date.Day,2);
				break;
			case 3:
				DrawNumber(gs->FontHandle,dx,y,rp->Date.Hour,2);
				break;
			case 4:
				DrawNumber(gs->FontHandle,dx,y,rp->Date.Min,2);
				break;
		}
		dx += CHAR_SIZE * 2;
		if(i == 3) cg = CHAR_COLON;
		else if(i == 2) cg = CHAR_MINUS;
		if(i < 4) DrawGraph(dx,y,gs->FontHandle[cg],TRUE);
		dx += CHAR_SIZE;
	}
	DrawNumber(gs->FontHandle,x,y,rank + 1,2);
	DrawNumber(gs->FontHandle,x + R_SCORE_POS_X,y,rp->Score,G_SCORE_DIGIT);
	DrawNumber(gs->FontHandle,x + R_LINE_POS_X,y,rp->Lines,G_LINES_DIGIT);
	DrawNameString(gs->FontHandle,x + R_NAME_POS_X,y,rp->Name);
}

void DrawRankingInput(GameStatus *gs){
	Ranking *rp = &gs->PlayerRanking[gs->Rank];
	const int cx1 = R_INP_POS_LX + R_INP_POS_CX;
	const int cx2 = cx1 + R_INP_SIZE2 - CHAR_SIZE;
	const int cy1 = R_INP_POS_CY;
	const int cy2 = cy1 + R_INP_SIZE2 - CHAR_SIZE;
	DrawTransBox(0,0,640,480,0,gs->FadeTimer);
	DrawTileChar(gs->CharHandle1,R_INP_CAP1_X,R_INP_CAP_Y,G_UGET1_START,G_UGET1_WIDTH,G_UGET1_HEIGHT);
	DrawTileChar(gs->CharHandle1,R_INP_CAP2_X,R_INP_CAP_Y,G_UGET2_START,G_UGET2_WIDTH,G_UGET2_HEIGHT);
	Draw2xNumber(gs->FontHandle,R_INP_CNUM_X,R_INP_CNUM_Y,gs->Rank + 1,R_INP_CNUM_DIG);
	DrawInputName(gs->FontHandle,R_INP_POS_LX,R_INP_POS_CY,gs->Cursol_Pos,gs->SelectVar);
	switch(gs->Rank){
		case 0:
			DrawTileChar(gs->CharHandle1,R_INP_COPT_X,R_INP_COPT_Y,G_1ST_START,G_NOPT_WIDTH,1);
			break;
		case 1:
			DrawTileChar(gs->CharHandle1,R_INP_COPT_X,R_INP_COPT_Y,G_2ND_START,G_NOPT_WIDTH,1);
			break;
		case 2:
			DrawTileChar(gs->CharHandle1,R_INP_COPT_X,R_INP_COPT_Y,G_3RD_START,G_NOPT_WIDTH,1);
			break;
		default:
			DrawTileChar(gs->CharHandle1,R_INP_COPT_X,R_INP_COPT_Y,G_4TH_START,G_NOPT_WIDTH,1);
			break;
	}
	DrawScore(gs,R_INP_SCORE_X,R_INP_SCORE_Y);
	DrawLines(gs,R_INP_LINES_X,R_INP_LINES_Y);
	DrawGraph(cx1,cy1,gs->CharHandle[G_CORNER_LU],TRUE);
	DrawGraph(cx2,cy1,gs->CharHandle[G_CORNER_RU],TRUE);
	DrawGraph(cx1,cy2,gs->CharHandle[G_CORNER_LD],TRUE);
	DrawGraph(cx2,cy2,gs->CharHandle[G_CORNER_RD],TRUE);
	DrawRankingName(gs->FontHandle,R_INPSTR_X,R_INPSTR_Y,gs->Cursol_Pos2,gs->SelectVar,rp->Name);
}

void DrawInputName(int *gHandle,int x,int y,int count,int code){
	const int cSize1 = R_INP_SIZE1;
	const int cSize2 = R_INP_SIZE2;
	const int cx1 = x + R_INP_POS_CX + count;
	const int cx2 = cx1 + cSize2 + count;
	const int cy2 = y + cSize2;
	int index1 = code - R_INPUT_CENTER;
	int index2 = code + 7;
	if(index1 < CHAR_0) index1 += CHAR_END + 1;
	if(index2 > CHAR_END) index2 -= CHAR_END + 1;
	for(int i=0;i<R_INPUT_CENTER;i++){
		const int lx1 = x + count + i * cSize1;
		const int lx2 = lx1 + cSize1;
		const int rx1 = x + R_INP_POS_AX + count - i * cSize1;
		const int rx2 = rx1 + cSize1;
		const int lry1 = y + CHAR_SIZE;
		const int lry2 = y + CHAR_SIZE + cSize1;
		const int ext = i * 2;
		const int alpha = 100 + ext * 10;
		if(index2 < CHAR_0) index2 += CHAR_END + 1;
		if(index1 > CHAR_END) index1 = CHAR_0;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,alpha);
		DrawExtendGraph(lx1 - ext,lry1 - ext,lx2 + ext,lry2 + ext,gHandle[index1],TRUE);
		DrawExtendGraph(rx1 - ext,lry1 - ext,rx2 + ext,lry2 + ext,gHandle[index2],TRUE);
		index1++;
		index2--;
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
	DrawExtendGraph(cx1,y,cx2,cy2,gHandle[code],TRUE);
}

void DrawRankingName(int *gHandle,int x,int y,int pos,int chr,int *name){
	const int bx = x - 4;
	const int by = y - 4;
	const int bx1 = x + FONT_SIZE * RANKING_NAME + 4;
	const int by1 = y + FONT_SIZE + 4;
	const int px = x + pos * FONT_SIZE;
	DrawTransBox(bx,by,bx1,by1,GetColor(0,0,128),G_BOX_ALPHA);
	for(int i=0;i<RANKING_NAME;i++){
		const int sx = x + i * FONT_SIZE;
		DrawGraph(sx,y,gHandle[name[i]],TRUE);
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,G_BOX_ALPHA);
	DrawBox(px,y,px + CHAR_SIZE,y + CHAR_SIZE,GetColor(150,0,0),TRUE);
	DrawGraph(px,y,gHandle[chr],TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
}

void Draw2xNumber(int *gHandle,int x,int y,int num,int digit_max){
	const int dx = x + (digit_max - 1) * FONT_SIZE;
	const int csz = FONT_SIZE * 2;
	for(int i=0;i<digit_max;i++){
		const int nx = dx - i * csz;
		const int d_num = GetDigitNum(num,i);
		const int nx1 = nx + csz;
		const int y1 = y + csz;
		if(d_num == DXLIB_FALSE) DrawExtendGraph(nx,y,nx1,y1,gHandle[0],TRUE);
		else DrawExtendGraph(nx,y,nx1,y1,gHandle[d_num],TRUE);
	}
}

void DestroyBGHandle(int *gHandle){
	for(int i=0;i<BG_CHIP_NUM;i++){
		if(gHandle[i] != 0){
			DeleteGraph(gHandle[i]);
			gHandle[i] = 0;
		}
	}
}


