// gamedraw.cpp

#include "tetris.h"

void GameMainDraw(GameStatus *gs){
	// 背景描画
	DrawGraph(0,0,gs->BgHandle,FALSE);
	DrawStage(gs->CharHandle,gs->Stage,STAGE_POS_X,STAGE_POS_Y);
	// ブロック描画
	if(gs->AnimeTimer == 0){
		if(gs->Op.Ghost == 1) DrawGhostTetriBlocks(gs);
		DrawPlayerTetriBlocks(gs,gs->Bl_X,gs->Bl_Y);
	}else{
		if(gs->EraseLineNum > 0) DrawFlashLine(gs);
	}
	// ステージのフレーム
	DrawStageFlame(gs->CharHandle,STAGE_POS_X,STAGE_POS_Y);
	// インターフェース部描画
	if(gs->Op.NextNum > 0) DrawNextTetriBlocks(gs,NEXT_POS_X,NEXT_POS_Y);
	if(gs->Op.Hold == 1) DrawHoldTetriBlock(gs,HOLD_POS_X,HOLD_POS_Y);
	DrawHighScore(gs,HSCORE_POS_X,HSCORE_POS_Y);
	DrawScore(gs,SCORE_POS_X,SCORE_POS_Y);
	DrawLevel(gs,LEVEL_POS_X,LEVEL_POS_Y);
	DrawLines(gs,LINES_POS_X,LINES_POS_Y);
	DrawExtendTileChar(gs->CharHandle,0,20,220,78,G_TITLE_START,G_TITLE_WIDTH,G_TITLE_HEIGHT);
	// ゲームオーバー表示
	if(gs->GameMode == GAMEMODE_GOV) DrawGameOver(gs);
	// ランキング入力表示
	if(gs->GameMode == GAMEMODE_RANKIN) DrawRankingInput(gs);
	// フェード処理
	if((gs->FadeStart == 1)&&(gs->GameMode == GAMEMODE_MAIN)){
		DrawTransBox(0,0,640,480,0,gs->FadeTimer + FADEOUT_START);
	}
}

void DrawNumber(int *gHandle,int x,int y,int num,int digit_max){
	const int dx = x + (digit_max - 1) * FONT_SIZE;
	for(int i=0;i<digit_max;i++){
		int nx = dx - i * FONT_SIZE;
		int d_num = GetDigitNum(num,i);
		if(d_num == DXLIB_FALSE) DrawGraph(nx,y,gHandle[0],TRUE);
		else DrawGraph(nx,y,gHandle[d_num],TRUE);
	}
}

void DrawStage(int *gHandle,int *stage,int x,int y){
	int fx = STAGE_SIZE_X - STAGE_CLIP - 1;
	int fy = STAGE_SIZE_Y - STAGE_CLIP - 1;
	int sx = x - CHAR_SIZE;
	int sy = y - CHAR_SIZE;
	for(int i=STAGE_CLIP;i<=fy;i++){
	//for(int i=0;i<STAGE_SIZE_Y;i++){
		for(int j=STAGE_CLIP;j<=fx;j++){
		//for(int j=0;j<STAGE_SIZE_X;j++){
			const int block_code = stage[i*STAGE_SIZE_X+j];
			if(block_code == BLOCK_OFF){
				SetDrawBlendMode(DX_BLENDMODE_ALPHA,i * 10 + 35);
			}else{
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
			}
			DrawGraph(sx+j*CHAR_SIZE,sy+i*CHAR_SIZE,gHandle[block_code],FALSE);	
		}
	}
	for(int i=STAGE_ENTER_L+1;i<STAGE_ENTER_R;i++){
		const int block_code = stage[STAGE_SIZE_X+i];
		if(block_code == BLOCK_OFF){
			SetDrawBlendMode(DX_BLENDMODE_ALPHA,STAGETOP_ALPHA);
		}else{
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
		}
		DrawGraph(sx+i*CHAR_SIZE,sy+CHAR_SIZE,gHandle[block_code],FALSE);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
}

void DrawStageFlame(int *gHandle,int x,int y){
	const int fx = STAGE_SIZE_X - STAGE_CLIP - 1;
	const int fy = STAGE_SIZE_Y - STAGE_CLIP - 1;
	const int sx = x - CHAR_SIZE;
	const int sy = y - CHAR_SIZE;
	DrawGraph(x,y,gHandle[LINE_LT],TRUE);
	for(int i=STAGE_CLIP;i<STAGE_ENTER_L;i++) DrawGraph(sx+i*CHAR_SIZE,y,gHandle[LINE_TOP],TRUE);
	DrawGraph(sx+STAGE_ENTER_L*CHAR_SIZE,y,gHandle[LINE_ENT_L],TRUE);
	DrawGraph(sx+STAGE_ENTER_R*CHAR_SIZE,y,gHandle[LINE_ENT_R],TRUE);
	for(int i=STAGE_ENTER_R+1;i<=fx;i++) DrawGraph(sx+i*CHAR_SIZE,y,gHandle[LINE_TOP],TRUE);
	DrawGraph(x+fx*CHAR_SIZE,y,gHandle[LINE_RT],TRUE);
	for(int i=STAGE_CLIP;i<=fy;i++){
		DrawGraph(x,sy+i*CHAR_SIZE,gHandle[LINE_LM],TRUE);
		DrawGraph(x+fx*CHAR_SIZE,sy+i*CHAR_SIZE,gHandle[LINE_RM],TRUE);
	}
	DrawGraph(x,y+fy*CHAR_SIZE,gHandle[LINE_LB],TRUE);
	for(int i=1;i<11;i++){
		DrawGraph(x+i*CHAR_SIZE,y+fy*CHAR_SIZE,gHandle[LINE_BOT],TRUE);
	}
	DrawGraph(x+fx*CHAR_SIZE,y+fy*CHAR_SIZE,gHandle[LINE_RB],TRUE);
}

void DrawPlayerTetriBlocks(GameStatus *gs,int x,int y){
	const int tx = STAGE_POS_X + (x - 1) * CHAR_SIZE;
	const int ty = STAGE_POS_Y + (y - 1) * CHAR_SIZE;
	int mask[] = {0,0,0,0,0,0,0,0};
	SetBlockMask(mask,x,y);
	DrawTetriBlocks(gs->CharHandle,gs->Piece,tx,ty,mask);
}

void DrawTetriBlocks(int *gHandle,int *piece,int x,int y,int *mask){
	const int m_max = TETRI_SIZE * STAGE_CLIP;
	for(int i=0;i<TETRI_SIZE;i++){
		for(int j=0;j<TETRI_SIZE;j++){
			const int m_index = i * TETRI_SIZE + j;
			const int ps = piece[m_index];
			const int bx = x + j * CHAR_SIZE;
			const int by = y + i * CHAR_SIZE;
			int d_flag = 0;
			if(m_index < m_max){
				if((mask[m_index] == MASK_OFF)){
					d_flag = 1;
				}
			}else d_flag = 1;
			if((ps != BLOCK_OFF)&&(d_flag == 1)){
				DrawGraph(bx,by,gHandle[ps],FALSE);
			}
		}
	}
}

void DrawNextTetriBlocks(GameStatus *gs,int x,int y){
	int tmpTetri[TETRI_SIZE * TETRI_SIZE];
	int mask[] = {0,0,0,0,0,0,0,0};
	const int nextMax = gs->Op.NextNum + 1;
	const int px = x + 4;
	const int py = y + 4;
	const int bx1 = x;
	const int by1 = y + CHAR_SIZE;
	const int bx2 = x + (CHAR_SIZE * TETRI_SIZE) * nextMax + 8 + 32;
	const int by2 = y + CHAR_SIZE * (TETRI_SIZE - 1) + 8;
	DrawTransBox(bx1,by1,bx2,by2,0,G_BOX_ALPHA);
	DrawTileChar(gs->CharHandle,px,py+24,G_NEXT_START,G_NEXT_WIDTH,1);
	for(int i=1;i<nextMax;i++){
		const int nx = px + (i - 1) * (CHAR_SIZE * TETRI_SIZE + CHAR_SIZE) + (CHAR_SIZE * 3);
		SetPattern(tmpTetri,gs->Next[i]);
		DrawTetriBlocks(gs->CharHandle,tmpTetri,nx,py,mask);
	}
}

void DrawFlashLine(GameStatus *gs){
	const int animeCode = L_FLASH_TIME - gs->AnimeTimer + G_FLASH_START;
	const int stageBot = STAGE_SIZE_Y - STAGE_CLIP;
	const int stageTop = STAGE_CLIP;
	const int stageLeft = STAGE_CLIP;
	const int stageRight = STAGE_SIZE_X - STAGE_CLIP;
	const int sx = STAGE_POS_X - CHAR_SIZE;
	const int sy = STAGE_POS_Y - CHAR_SIZE;
	if((animeCode < G_FLASH_END)&&(animeCode >= G_FLASH_START)){
		for(int i=stageTop;i<stageBot;i++){
			if(gs->EraseLines[i] == BLOCK_ENABLE){
				for(int j=stageLeft;j<stageRight;j++){
					const int index = i * STAGE_SIZE_X + j;
					const int x = j * CHAR_SIZE + sx;
					const int y = i * CHAR_SIZE + sy;
					DrawGraph(x,y,gs->CharHandle[animeCode],TRUE);
				}
			}
		}
	}
}

void DrawGhostTetriBlocks(GameStatus *gs){
	const int x = gs->Bl_X;
	const int y = GetGhostPos(gs->Stage,gs->Piece,x,gs->Bl_Y);
	const int sx = (x * CHAR_SIZE) + STAGE_POS_X - CHAR_SIZE;
	const int sy = (y * CHAR_SIZE) + STAGE_POS_Y - CHAR_SIZE;
	int mask[] = {0,0,0,0,0,0,0,0};
	if(gs->Bl_Y >= y) return;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,GHOST_ALPHA);
	DrawTetriBlocks(gs->CharHandle,gs->Piece,sx,sy,mask);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
}

void DrawHoldTetriBlock(GameStatus *gs,int x,int y){
	int tmpTetri[TETRI_SIZE * TETRI_SIZE];
	int mask[] = {0,0,0,0,0,0,0,0};
	const int px = x + G_BOX_MARGIN;
	const int ty = y + G_BOX_MARGIN;
	const int tx = px + CHAR_SIZE / 2;
	const int py = ty + CHAR_SIZE + G_BOX_MARGIN;
	const int bx1 = x;
	const int by1 = y;
	const int bx2 = x + CHAR_SIZE * TETRI_SIZE + G_BOX_MARGIN * 2;
	const int by2 = y + CHAR_SIZE * (TETRI_SIZE + 1) + G_BOX_MARGIN * 3;
	DrawTransBox(bx1,by1,bx2,by2,0,G_BOX_ALPHA);
	DrawTileChar(gs->CharHandle,tx,ty,G_HOLD_START,G_HOLD_WIDTH,1);
	if(gs->Hold > 0){
		int ox = 0;
		const int pc = SetPattern(tmpTetri,gs->Hold);
		if(pc == 1) ox = CHAR_SIZE / 2;
		else if(pc == 2) ox = -(CHAR_SIZE / 2);
		DrawTetriBlocks(gs->CharHandle,tmpTetri,px + ox,py,mask);
	}
}

void DrawTileChar(int *gHandle,int x,int y,int start,int width,int height){
	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
			const int tx = x + j * CHAR_SIZE;
			const int ty = y + i * CHAR_SIZE;
			const int c_code = start + (i * CHARSET_SIZE_X) + j;
			DrawGraph(tx,ty,gHandle[c_code],TRUE);
		}
	}
}

void DrawExtendTileChar(int *gHandle,int x,int y,int x1,int y1,
											int start,int width,int height){
	const double cx = (double)(x1 - x) / width;
	const double cy = (double)(y1 - y) / height;
	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
			const int tx = x + j * (int)cx;
			const int ty = y + i * (int)cy;
			const int tx1 = (int)(tx + cx);
			const int ty1 = (int)(ty + cy);
			const int c_code = start + (i * CHARSET_SIZE_X) + j;
			DrawExtendGraph(tx,ty,tx1,ty1,gHandle[c_code],TRUE);
		}
	}
}

void DrawTransBox(int x,int y,int x1,int y1,int color,int blendPalam){
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,blendPalam);
	DrawBox(x,y,x1,y1,color,TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
}

void DrawHighScore(GameStatus *gs,int x,int y){
	const int px = x + G_BOX_MARGIN;
	const int py = y + G_BOX_MARGIN;
	const int nx = px + G_HSCORE_WIDTH * CHAR_SIZE + G_BOX_MARGIN * 2;
	const int bx = nx + G_HSCORE_DIGIT * CHAR_SIZE + G_BOX_MARGIN;
	const int by = y + CHAR_SIZE + G_BOX_MARGIN * 2;
	DrawTransBox(x,y,bx,by,0,G_BOX_ALPHA);
	DrawTileChar(gs->CharHandle,px,py,G_HSCORE_START,G_HSCORE_WIDTH,1);
	DrawNumber(gs->FontHandle,nx,py,gs->HiScore,G_HSCORE_DIGIT);
}

void DrawScore(GameStatus *gs,const int x,const int y){
	const int px = x + G_BOX_MARGIN;
	const int py = y + G_BOX_MARGIN;
	const int nx = px + G_SCORE_WIDTH * CHAR_SIZE + G_BOX_MARGIN * 2;
	const int bx = nx + G_SCORE_DIGIT * CHAR_SIZE + G_BOX_MARGIN;
	const int by = y + CHAR_SIZE + G_BOX_MARGIN * 2;
	DrawTransBox(x,y,bx,by,0,G_BOX_ALPHA);
	DrawTileChar(gs->CharHandle,px,py,G_SCORE_START,G_SCORE_WIDTH,1);
	DrawNumber(gs->FontHandle,nx,py,gs->Score,G_SCORE_DIGIT);
}

void DrawLevel(GameStatus *gs,const int x,const int y){
	const int px = x + G_BOX_MARGIN;
	const int py = y + G_BOX_MARGIN;
	const int nx = px + G_LEVEL_WIDTH * CHAR_SIZE + G_BOX_MARGIN * 2;
	const int bx = nx + G_LEVEL_DIGIT * CHAR_SIZE + G_BOX_MARGIN;
	const int by = y + CHAR_SIZE + G_BOX_MARGIN * 2;
	DrawTransBox(x,y,bx,by,0,G_BOX_ALPHA);
	DrawTileChar(gs->CharHandle,px,py,G_LEVEL_START,G_LEVEL_WIDTH,1);
	DrawNumber(gs->FontHandle,nx,py,gs->Level,G_LEVEL_DIGIT);
}

void DrawLines(GameStatus *gs,const int x,const int y){
	const int px = x + G_BOX_MARGIN;
	const int py = y + G_BOX_MARGIN;
	const int nx = px + G_LINES_WIDTH * CHAR_SIZE + G_BOX_MARGIN * 2;
	const int bx = nx + G_LINES_DIGIT * CHAR_SIZE + G_BOX_MARGIN;
	const int by = y + CHAR_SIZE + G_BOX_MARGIN * 2;
	DrawTransBox(x,y,bx,by,0,G_BOX_ALPHA);
	DrawTileChar(gs->CharHandle,px,py,G_LINES_START,G_LINES_WIDTH,1);
	DrawNumber(gs->FontHandle,nx,py,gs->GetLines,G_LINES_DIGIT);
}

void DrawGameOver(GameStatus *gs){
	const int tx = GOVER_POS_X;
	const int ty = GOVER_POS_Y;
	const int tx1 = tx + CHAR_SIZE;
	const int ty1 = ty + CHAR_SIZE * 3;
	if(gs->GovCounter > GOVERDEMO_END){
		DrawTransBox(0,0,640,480,0,gs->FadeTimer);
		DrawTileChar(gs->CharHandle,tx,ty,G_GOVER_START,G_GOVER_WIDTH,G_GOVER_HEIGHT);
		DrawTileChar(gs->CharHandle,tx1,ty1,G_H_A_K_START,G_H_A_K_WIDTH,1);
	}
}
