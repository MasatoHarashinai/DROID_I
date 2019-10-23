#include "DxLib.h"	// Dxライブラリのインクルード

/*****************
	定数定義
*****************/
#define MapSizeX 9		// マップの横のマス数
#define MapSizeY 9		// マップの縦のマス数
#define BlockSize 64	// 1ブロック当たりの幅
#define SideStart 608	// ステータスバーの始まりのＸ座標
#define BulletSpeed 15	// ショット攻撃の移動速度
#define GodTime 500		// 無敵時間定数
#define PHI_F	DX_PI	// pi

/*********************
	プロトタイプ宣言
**********************/
void CreateAI();		// AIプレイヤーの描画関数
void DrawPlayer();		// プレイヤーの描画関数
void Collision();		// 衝突判定の関数
void DrawMap();			// マップの生成関数
void Move();			// プレイヤーの移動関数
void KeyInput();		// 入力検出の関数
void Shot();			// ショットの関数
void State();			// プレイヤーのステータス
void Initialize();		// 初期化関数
void LiveOn3();			// ラウンドスタートの関数
void DrawStar();		// ラウンド取得数の表示

/********************
	グローバル変数
*********************/
// マップ (1:箱を置く 0:床)//
char Map[MapSizeY][MapSizeX] =
{
	1,0,1,0,1,0,1,0,1,
	0,0,0,0,0,0,0,0,0,
	1,0,1,0,1,0,1,0,1,
	0,0,0,0,0,0,0,0,0,
	1,0,1,0,1,0,1,0,1,
	0,0,0,0,0,0,0,0,0,
	1,0,1,0,1,0,1,0,1,
	0,0,0,0,0,0,0,0,0,
	1,0,1,0,1,0,1,0,1,
};

// ステータス変数 //
int PlayerX[2] = {64,480};		// プレイヤーのX座標
int PlayerY[2] = {0,544};		// プレイヤーのY座標
int PlayerW = 32;				// プレイヤー画像の幅
int PlayerH = 32;				// プレイヤー画像の高さ
int PlayerMuki[2] = {2,0};		// プレイヤーの向き
int PlayerLife[2] = {5,5};		// プレイヤーのライフ
int PlayerFlag[2] = {1,1};		// プレイヤーの生存フラグ

/*	// 現在はAI未実装
int AIX[4] = {0,0,0,0};			// AIのX座標
int AIY[4] = {0,0,0,0};			// AIのY座標
int AIW = 32;					// AIの幅
int AIH = 32;					// AIの高さ
int AIMuki[4] = {0,0,0,0};		// AIの向き
int AIFlag[4] = {0,0,0,0};		// AIの生存フラグ
*/

int MoveFlag[2] = {0,0};		// プレイヤーの移動フラグ
int MoveCnt[2] = {0,0};			// プレイヤー移動距離のカウンタ
int ShotX[2] = {0,0};			// 自機弾のX座標
int ShotY[2] = {0,0};			// 自機弾のY座標
int ShotW = 16;					// 自機弾の画像の幅
int ShotH = 16;					// 自機弾の画像の高さ
int ShotMuki[2] = {2,0};		// 自機弾の向き
int ShotFlag[2] = {0,0};		// 自機弾の存在フラグ
int LastDamaged[2] = {0,0};		// 最後に被弾した時間
int God[2] = {0,0};				// 無敵時間のフラグ

// ハンドル(ファイル識別番号) //
int PlayerHandle[2];	// プレイヤーの画像

/*
int AIHandle;
*/

int BackHandle;			// 背景の画像
int BlockHandle;		// ブロックの画像
int ShotHandle;			// 自機弾の画像
int SideHandle;			// ステータスバー
int LabelHandle_PLAYER;	// "PLAYER"の画像
int LabelHandle_PLAYER2;// "PLAYER2"の画像
int LabelHandle_HP;		// "HP"の画像
int HPHandle;			// ハートの画像
int StarHandle;			// 星の画像
int Player1WinHandle;	// "PLAYER1WIN"の画像
int Player2WinHandle;	// "PLAYER2WIN"の画像
int StartHandle;		// スタート画面の画像
int SelectHandle;		// 選択画面の画像
int BGMHandle;			// BGMの音楽
int ShotSoundHandle;	// ショットの効果音
int WinnerHandle;		// 勝利のジングル

//Etc //
int LargeFont;							// 大き目のフォントの識別子
int NormalFont;							// 普通のフォントの識別子
int Color_Red = GetColor(255,0,0);		// 赤色の識別子
int Color_Blue = GetColor(0,0,255);		// 青色の識別子
int Color_Black	= GetColor(0,0,0);		// 黒色の識別子
int a = 0;								//
int i = 0;								//	汎用カウンタ
int j = 0;								//
int Scene = 0;							// シーン定数
int EndFlag = 0;						// ゲーム終了フラグ
int Select = 0;							// シーンの選択定数
int GameCount = 0;						// 総ラウンド数
int CountWin[2] = {0,0};				// Playerの勝利カウント
int PlayerWin[2] = {0,0};				// Playerが勝ったかどうか

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow )

{
	// 初期設定 //
	SetWindowSizeExtendRate(1.6,1.2);	// ゲーム画面の拡大
	ChangeWindowMode(TRUE);				// ウィンドウモードでの起動
	SetUseDirectInputFlag(TRUE);		// DirectInputの許可
	SetDrawScreen(DX_SCREEN_BACK);		// 描画する対象を裏画面に設定
	SetGraphMode(800,576,32,60);		// 解像度とフレームレートの設定
	DxLib_Init();						// Dxライブラリの初期化	

	// 画像の読み込み //
	BlockHandle = LoadGraph("resource/image/b_block.png");	
	PlayerHandle[0] = LoadGraph("resource/image/player1.png");
	PlayerHandle[1] = LoadGraph("resource/image/player2.png");
	BackHandle = LoadGraph("resource/image/floor.png");
	ShotHandle = LoadGraph("resource/image/shot.png");
	SideHandle = LoadGraph("resource/image/side.png");
	LabelHandle_PLAYER = LoadGraph("resource/image/label_PLAYER.png");
	LabelHandle_PLAYER2 = LoadGraph("resource/image/label_PLAYER2.png");
	LabelHandle_HP = LoadGraph("resource/image/label_HP.png");
	HPHandle = LoadGraph("resource/image/HP.png");
	StarHandle = LoadGraph("resource/image/star.png");
	Player1WinHandle = LoadGraph("resource/image/player1win.png");
	Player2WinHandle = LoadGraph("resource/image/player2win.png");
	StartHandle = LoadGraph("resource/image/start.png");
	SelectHandle = LoadGraph("resource/image/Select.png");

	//　サウンドの読み込み //
	BGMHandle = LoadSoundMem("resource/sound/bgm.wav");
	ShotSoundHandle = LoadSoundMem("resource/sound/shot.mp3");
	WinnerHandle = LoadSoundMem("resource/sound/winner.mp3");

	// Fontの作成 //
	LargeFont = CreateFontToHandle(NULL,65,9);
	NormalFont = CreateFontToHandle(NULL,20,9);


	while( (ProcessMessage() == 0) && (CheckHitKey(KEY_INPUT_ESCAPE) == 0) && (EndFlag == 0))
	{
		// シーンの分岐//
		switch(Scene)
		{
			// スタート画面 //
			case 0: Initialize();	// ゲームの状態を初期化する
					StopSoundMem(WinnerHandle);	// ゲームオーバー後から戻った場合にサウンドを停止させる
					while(CheckHitKey(KEY_INPUT_SPACE) == 0)
					{
						if(CheckHitKey(KEY_INPUT_ESCAPE) == 1) EndFlag = 1;
						ClearDrawScreen();
						DrawGraph(0,0,StartHandle,TRUE);
						ScreenFlip();
					}
					Scene = 1;
					break;
			// ゲーム画面 //
			case 1: Initialize();
				    LiveOn3();
					PlaySoundMem(BGMHandle,DX_PLAYTYPE_LOOP,TRUE);
					while(Scene == 1 && (CheckHitKey(KEY_INPUT_ESCAPE) == 0))
					{
						// 残像処理 //
						ClearDrawScreen();

						// 背景の描画 //
						DrawGraph(0,0,BackHandle,TRUE);
						DrawGraph(578,0,SideHandle,TRUE);
					
						// 衝突判定 //
						Collision();

						// ショット攻撃 //
						Shot();

						// プレイヤーの移動 //
						Move();

						// ステータス //
						State();

						// プレイヤーの配置 //
						DrawPlayer();
		
						// 勝利カウントの表示 //
						DrawStar();

						// Map生成 //
						DrawMap();

						// キーの検出 //
						KeyInput();

						// 裏画面の情報を反映 //
						ScreenFlip();

						// 片方が倒れたらラウンド終了 //
						if((PlayerFlag[0] == 0) || (PlayerFlag[1] == 0)) 
						{
							Scene = 2;
							if(PlayerFlag[1] == 0) CountWin[0] ++;	//　プレイヤー１に勝利数をカウントさせる
							if(PlayerFlag[0] == 0) CountWin[1] ++;	// プレイヤー２に勝利数をカウントさせる
							GameCount++;	// 総ラウンド数をカウントする
							StopSoundMem(BGMHandle);	// 一旦ＢＧＭを停止させる
						}
					}
					break;

			// ラウンド終了 //
			case 2: PlaySoundMem(WinnerHandle,DX_PLAYTYPE_BACK,TRUE);
					while(1)
					{
						// 終了した時点でラウンド数を反映させる //
						DrawStar();
						
						// 勝った方の勝者ラベルを表示 //
						if((PlayerFlag[0] == 1) && (PlayerFlag[1] == 0)) DrawGraph(0,0,Player1WinHandle,TRUE);
						if((PlayerFlag[1] == 1) && (PlayerFlag[0] == 0)) DrawGraph(0,0,Player2WinHandle,TRUE);
						ScreenFlip();

						// ゲームセットかどうかを判別する //
						if(GameCount == 2)
						{
							if(CountWin[0] == 2) PlayerWin[0] = 1;	// プレイヤー１が２連勝した場合
							if(CountWin[1] == 2) PlayerWin[1] = 1;	// プレイヤー２が２連勝した場合
						}
						
						if(GameCount == 3)
						{
							if(CountWin[0] == 2 && CountWin[1] == 1) PlayerWin[0] = 1;	// 2:1でプレイヤー１が勝った場合
							if(CountWin[0] == 1 && CountWin[1] == 2) PlayerWin[1] = 1;	// 1:2でプレイヤー２が勝った場合
						}

						
						if(CheckSoundMem(WinnerHandle) == 0 && (PlayerWin[0] == 1 || PlayerWin[1] == 1)) // 試合が終了した場合は終了画面へ
						{
							Scene = 3;
							break;
						}
						else if(CheckSoundMem(WinnerHandle) == 0 && (PlayerWin[0] == 0 && PlayerWin[1] == 0)) // 試合が終わっていない場合は再度ラウンドを開始する
						{
							Scene = 1;
							break;
						}
					}
					break;

			// 試合終了 //
			case 3: while(1)
					{
							DrawGraph(0,70,SelectHandle,TRUE);
							ScreenFlip();

							if(CheckHitKey(KEY_INPUT_R))	// 再戦するため各カウンタをリセットする
							{
								CountWin[0] = 0;
								CountWin[1] = 0;
								PlayerWin[0] = 0;
								PlayerWin[1] = 0;
								GameCount = 0;
								Scene = 0;
								break;
							}
							if(CheckHitKey(KEY_INPUT_ESCAPE))	// ゲームを終了させる
							{
								EndFlag = 1;
								break;
							}
					}
			default:break;
		}

	}

	DxLib_End(); // Dxライブラリを終了させる
	return 0;

}

// ゲーム開始までのカウントダウン
void LiveOn3()
{
	ClearDrawScreen();
	DrawGraph(0,0,BackHandle,TRUE);
	DrawGraph(578,0,SideHandle,TRUE);
	DrawMap();
	DrawPlayer();
	State();
	DrawStar();
	DrawStringToHandle(200,230,"READY?",GetColor(255,0,0),LargeFont);
	ScreenFlip();
	WaitTimer(1800);

	ClearDrawScreen();
	DrawGraph(0,0,BackHandle,TRUE);
	DrawGraph(578,0,SideHandle,TRUE);
	DrawMap();
	DrawPlayer();
	State();
	DrawStar();
	DrawStringToHandle(240,230,"3",GetColor(255,0,0),LargeFont);
	ScreenFlip();
	WaitTimer(1000);

	ClearDrawScreen();
	DrawGraph(0,0,BackHandle,TRUE);
	DrawGraph(578,0,SideHandle,TRUE);
	DrawMap();
	State();
	DrawStar();
	DrawPlayer();
	DrawStringToHandle(240,230,"2",GetColor(255,0,0),LargeFont);
	ScreenFlip();
	WaitTimer(1000);

	ClearDrawScreen();
	DrawGraph(0,0,BackHandle,TRUE);
	DrawGraph(578,0,SideHandle,TRUE);
	DrawMap();
	DrawPlayer();
	State();
	DrawStar();
	DrawStringToHandle(240,230,"1",GetColor(255,0,0),LargeFont);
	ScreenFlip();
	WaitTimer(1000);
}

// ステージの描画
void DrawMap()
{
	for(i = 0 ; i < MapSizeY ; i++)
	{
		for(j = 0 ; j < MapSizeX ; j++)
		{
			if(Map[i][j] == 0) continue;
			DrawGraph(j*BlockSize,i*BlockSize,BlockHandle,TRUE);
		}
	}
}


// プレイヤーの描画
void DrawPlayer()
{
	for(a = 0 ; a < 2; a++)
	{
		if(PlayerFlag[a] == 1)
		{
				//　プレイヤーの向きに合わせて画像を回転させる(０：上　１：右　２：下　３：左)
				switch(PlayerMuki[a])
				{
					case 0: DrawRotaGraph(PlayerX[a]+PlayerW/2,PlayerY[a]+PlayerH/2,1,PHI_F,PlayerHandle[a],TRUE,TRUE);
							break;
					case 1: DrawRotaGraph(PlayerX[a]+PlayerW/2,PlayerY[a]+PlayerH/2,1,-(PHI_F / 2),PlayerHandle[a],TRUE,TRUE);
							break;
					case 2: DrawRotaGraph(PlayerX[a]+PlayerW/2,PlayerY[a]+PlayerH/2,1,PHI_F*2,PlayerHandle[a],TRUE,TRUE);
							break;
					case 3: DrawRotaGraph(PlayerX[a]+PlayerW/2,PlayerY[a]+PlayerH/2,1,(PHI_F / 2),PlayerHandle[a],TRUE,TRUE);
							break;
				}			
		}
	}
}

// ゲームの初期化
void Initialize()
{
PlayerX[0] = 64;
PlayerX[1] = 480;
PlayerY[0] = 0;
PlayerY[1] = 544;
PlayerW = 32;
PlayerH = 32;
PlayerMuki[0] = 2;
PlayerMuki[1] = 0;
PlayerLife[0] = 5;
PlayerLife[1] = 5;
PlayerFlag[0] = 1;
PlayerFlag[1] = 1;
a = 0;
i = 0;
j = 0;
MoveFlag[0] = 0;
MoveFlag[1] = 0;
MoveCnt[0] = 0;
MoveCnt[1] = 0;
ShotX[0] = 0;
ShotX[1] = 0;
ShotY[0] = 0;
ShotY[1] = 0;
ShotW = 16;
ShotH = 16;
ShotMuki[0] = 2;
ShotMuki[1] = 0;
ShotFlag[0] = 0;
ShotFlag[1] = 0;
LastDamaged[0] = 0;
LastDamaged[1] = 0;
God[0] = 0;
God[1] = 0;
}

void KeyInput()
{
	/*************************
	   プレイヤー１のキー検出 
	**************************/
	// 移動 //
	if(CheckHitKey(KEY_INPUT_W) == 1 && (MoveFlag[0] == 0)) 
	{
		PlayerMuki[0] = 0;
		MoveFlag[0] = 1;
	}

	if(CheckHitKey(KEY_INPUT_D) == 1 && (MoveFlag[0] == 0))
	{
		PlayerMuki[0] = 1;
		MoveFlag[0] = 1;
	}

	if(CheckHitKey(KEY_INPUT_S) == 1 && (MoveFlag[0] == 0))
	{
		PlayerMuki[0] = 2;
		MoveFlag[0] = 1;
	}

	if(CheckHitKey(KEY_INPUT_A) == 1 && (MoveFlag[0] == 0))
	{
		PlayerMuki[0] = 3;
		MoveFlag[0] = 1;
	}

	// 方向転換 //
	if(CheckHitKey(KEY_INPUT_1) == 1 && (MoveFlag[0] == 0)) PlayerMuki[0] = 0;
	if(CheckHitKey(KEY_INPUT_2) == 1 && (MoveFlag[0] == 0)) PlayerMuki[0] = 1;
	if(CheckHitKey(KEY_INPUT_3) == 1 && (MoveFlag[0] == 0)) PlayerMuki[0] = 2;
	if(CheckHitKey(KEY_INPUT_4) == 1 && (MoveFlag[0] == 0)) PlayerMuki[0] = 3;

	// ショット攻撃 //
	if(CheckHitKey(KEY_INPUT_Z) == 1 && (ShotFlag[0] == 0))
	{
		ShotFlag[0] = 1;
		ShotMuki[0] = PlayerMuki[0];
		PlaySoundMem(ShotSoundHandle,DX_PLAYTYPE_BACK,TRUE);
	}
	
	/************************
	  プレイヤー２のキー検出 
	*************************/
	// 移動 //
	if(CheckHitKey(KEY_INPUT_UP) == 1 && (MoveFlag[1] == 0)) 
	{
		PlayerMuki[1] = 0;
		MoveFlag[1] = 1;
	}

	if(CheckHitKey(KEY_INPUT_RIGHT) == 1 && (MoveFlag[1] == 0))
	{
		PlayerMuki[1] = 1;
		MoveFlag[1] = 1;
	}

	if(CheckHitKey(KEY_INPUT_DOWN) == 1 && (MoveFlag[1] == 0))
	{
		PlayerMuki[1] = 2;
		MoveFlag[1] = 1;
	}

	if(CheckHitKey(KEY_INPUT_LEFT) == 1 && (MoveFlag[1] == 0))
	{
		PlayerMuki[1] = 3;
		MoveFlag[1] = 1;
	}

	// 方向転換 //
	if(CheckHitKey(KEY_INPUT_5) == 1 && (MoveFlag[1] == 0)) PlayerMuki[1] = 0;
	if(CheckHitKey(KEY_INPUT_6) == 1 && (MoveFlag[1] == 0)) PlayerMuki[1] = 1;
	if(CheckHitKey(KEY_INPUT_7) == 1 && (MoveFlag[1] == 0)) PlayerMuki[1] = 2;
	if(CheckHitKey(KEY_INPUT_8) == 1 && (MoveFlag[1] == 0)) PlayerMuki[1] = 3;

	// ショット攻撃 //
	if(CheckHitKey(KEY_INPUT_L) == 1 && (ShotFlag[1] == 0))
	{
		ShotFlag[1] = 1;
		ShotMuki[1] = PlayerMuki[1];
		PlaySoundMem(ShotSoundHandle,DX_PLAYTYPE_BACK,TRUE);
	}
}

// 星（勝利数カウント）の描画
void DrawStar()
{
	for(i = 0 ; i < CountWin[0] ; i++)
	{
			DrawGraph(SideStart+32*i,32*10-15,StarHandle,TRUE);
	}
	for(i = 0 ; i < CountWin[1] ; i++)
	{

			DrawGraph(SideStart+(32*i),32*12-15,StarHandle,TRUE);
	}
}

// ステータスバーを描画する
void State()
{
	DrawGraph(SideStart,32*1,LabelHandle_PLAYER,TRUE);
	DrawGraph(SideStart,32*2,LabelHandle_HP,TRUE);
	DrawGraph(SideStart,32*5,LabelHandle_PLAYER2,TRUE);
	DrawGraph(SideStart,32*6,LabelHandle_HP,TRUE);
	DrawStringToHandle(SideStart,32*9,"Player1",GetColor(255,255,255),NormalFont);
	DrawStringToHandle(SideStart,32*11,"Player2",GetColor(255,255,255),NormalFont);
	for(a = 0 ; a < 2 ; a++)
	{
		for(i = 0 ; i < PlayerLife[a] ; i++)
		{
			if(a == 0) DrawGraph(SideStart+(32*i),32*3,HPHandle,TRUE);
			if(a == 1) DrawGraph(SideStart+(32*i),32*7,HPHandle,TRUE);
		}
	}
}

// プレイヤー１マス分移動させる（4 * 8 = 32Px　ずつ移動）
void Move()
{
	for(a = 0; a < 2 ; a++)
	{
		if(MoveFlag[a] == 1)
		{
			switch(PlayerMuki[a])	// プレイヤーの向きに合わせて移動方向を変更
			{
				case 0:	PlayerY[a] -= 4;
						break;
				case 1:	PlayerX[a] += 4;
						break;
				case 2: PlayerY[a] += 4;
						break;
				case 3: PlayerX[a] -= 4;
						break;
			}
			MoveCnt[a]++;	//　移動量を一つ増やす
			if(MoveCnt[a] == 8)	// 32pix進んだら停止させる
			{
				MoveFlag[a] = 0;
				MoveCnt[a] = 0;
			}
		}
	}
}

// 各画像同士の衝突判定
void Collision()
{
	for(a = 0 ; a < 2 ; a++)
	{
		/*****************
			外壁と自機
		*****************/

		if(PlayerX[a] < 0) 
		{
			MoveCnt[a] = 0;
			PlayerX[a] = 0;
			if(MoveFlag[a] == 1) MoveFlag[a] = 0;
		}
		if(PlayerX[a] > 544)
		{
			MoveCnt[a] = 0;
			PlayerX[a] = 544;
			if(MoveFlag[a] == 1) MoveFlag[a] = 0;
		}
		if(PlayerY[a] < 0)
		{
			MoveCnt[a] = 0;
			PlayerY[a] = 0;
			if(MoveFlag[a] == 1) MoveFlag[a] = 0;
		}
		if(PlayerY[a] > 544)
		{
			MoveCnt[a] = 0;
			PlayerY[a] = 544;
			if(MoveFlag[a] == 1) MoveFlag[a] = 0;
		}

		/******************
		  ブロックと自機
		******************/
		for(i = 0 ; i < MapSizeY ; i++)
		{
			for(j = 0 ; j <  MapSizeX; j++)
			{ 
				if(Map[i][j] == 0) continue;

				if( ((PlayerX[a]+PlayerW > BlockSize*j) && (PlayerX[a]+PlayerW < BlockSize*j+BlockSize)) || ((BlockSize*j+BlockSize > PlayerX[a]) && (BlockSize*j < PlayerX[a])) )
				{
					if( ((PlayerY[a]+PlayerH > BlockSize*i) && (PlayerY[a]+PlayerH < BlockSize*i+BlockSize)) || ((BlockSize*i+BlockSize > PlayerY[a]) && (BlockSize*i < PlayerY[a])) )
					{
						if(MoveFlag[a] == 1) // マス移動中に衝突した場合の処理
						{
							MoveCnt[a] = 0;
							MoveFlag[a] = 0;
						}
						
						switch(PlayerMuki[a]) //　ブロックにはみ出てしまうための位置調整 (０：上　１：右　２：下　３：左)
						{
							case 0:	PlayerY[a] += 4;
									break;
							case 1:	PlayerX[a] -= 4;
									break;
							case 2: PlayerY[a] -= 4;
									break;
							case 3: PlayerX[a] += 4;
									break;
						}
					}
				}
			}
		}

		/************************
		　ショット　と　プレイヤー
		*************************/
		// 1Pのショットから2P //
		if(PlayerFlag[0] == 1)
		{
			if( ((ShotX[0]+ShotW > PlayerX[1]) && (ShotX[0]+ShotW < PlayerX[1]+PlayerW)) || ((PlayerX[1]+PlayerW > ShotX[0]) && (PlayerX[1] < ShotX[0])) )
			{
				if( ((ShotY[0]+ShotH > PlayerY[1]) && (ShotY[0]+ShotH < PlayerY[1]+PlayerH)) || ((PlayerY[1]+PlayerH > ShotY[0]) && (PlayerY[1] < ShotY[0])) )
				{
					if(ShotFlag[0] == 1)	// ショットを発射している場合のみ判定する（プレイヤーの画像領域内にショットが格納されている場合にダメージ判定を無くすため） 
					{
						if(GetNowCount() - LastDamaged[1] < GodTime) // 無敵時間が終了しているかどうか
						{
							God[1] = 1;
						}
						else
						{
							God[1] = 0;
						}

						if(God[1] == 0)	// 無敵時間中に衝突したかどうかを判定
						{
							LastDamaged[1] = GetNowCount(); // 無敵時間計算用にダメージを食らった瞬間の時刻を保存
							if(PlayerLife[1] > 0) //　生きていればプレイヤーのライフを減らす
							{
								PlayerLife[1] --;
							}

							if(PlayerLife[1] <= 0) // 死んでいればプレイヤーの生存フラグを０に変更する
							{
								PlayerFlag[1] = 0;
							}
						}
					ShotFlag[0] = 0;
					ShotX[0] = PlayerX[0]+(PlayerW-ShotW)/2;
					ShotY[0] = PlayerY[0]+(PlayerH-ShotH)/2;
				}
			}
		}
		// 2Pのショットから1P //
		if(PlayerFlag[1] == 1)
		{
			if( ((ShotX[1]+ShotW > PlayerX[0]) && (ShotX[1]+ShotW < PlayerX[0]+PlayerW)) || ((PlayerX[0]+PlayerW > ShotX[1]) && (PlayerX[0] < ShotX[1])) )
			{
				if( ((ShotY[1]+ShotH > PlayerY[0]) && (ShotY[1]+ShotH < PlayerY[0]+PlayerH)) || ((PlayerY[0]+PlayerH > ShotY[1]) && (PlayerY[0] < ShotY[1])) )
				{
					if(ShotFlag[1] == 1)	// ショットを発射している場合のみ判定する（プレイヤーの画像領域内にショットが格納されている場合にダメージ判定を無くすため）
					{
						if(GetNowCount() - LastDamaged[0] < GodTime)	// 無敵時間が終了しているかどうか
						{
							God[0] = 1;
						}
						else
						{
							God[0] = 0;
						}

						if(God[0] == 0)		// 無敵時間中に衝突したかどうかを判定
						{
							LastDamaged[0] = GetNowCount();	// 無敵時間計算用にダメージを食らった瞬間の時刻を保存
							if(PlayerLife[0] > 0)	//　生きていればプレイヤーのライフを減らす
							{
								PlayerLife[0] --;
							}
	
							if(PlayerLife[0] <= 0)	// 死んでいればプレイヤーの生存フラグを０に変更する
							{
								PlayerFlag[0] = 0;
							}
						}
					
						ShotFlag[1] = 0;
						ShotX[1] = PlayerX[1]+(PlayerW-ShotW)/2;
						ShotY[1] = PlayerY[1]+(PlayerH-ShotH)/2;
					}
					}
				}
			}
		}

		/************************
			ショット と　外壁
		***********************/
		if(ShotX[a] < 0)
		{
			ShotX[a] = PlayerX[a]+(PlayerW-ShotW)/2;
			ShotY[a] = PlayerY[a]+(PlayerH-ShotH)/2;
			ShotFlag[a] = 0;
		}

		if(ShotX[a] > 560)
		{
			ShotX[a] = PlayerX[a]+(PlayerW-ShotW)/2;
			ShotY[a] = PlayerY[a]+(PlayerH-ShotH)/2;
			ShotFlag[a] = 0;
		}

		if(ShotY[a] < 0)
		{
			ShotX[a] = PlayerX[a]+(PlayerW-ShotW)/2;
			ShotY[a] = PlayerY[a]+(PlayerH-ShotH)/2;
			ShotFlag[a] = 0;
		}

		if(ShotY[a] > 560)
		{
			ShotX[a] = PlayerX[a]+(PlayerW-ShotW)/2;
			ShotY[a] = PlayerY[a]+(PlayerH-ShotH)/2;
			ShotFlag[a] = 0;
		}

		/**********************
		   ショットとブロック
		**********************/
		for(i = 0 ; i < MapSizeY ; i++)
		{
			for(j = 0 ; j < MapSizeX ; j++)
			{
				if( ((ShotX[a]+ShotW > BlockSize*j) && (ShotX[a]+PlayerW < BlockSize*j+BlockSize)) || ((BlockSize*j+BlockSize > ShotX[a]) && (BlockSize*j < ShotX[a])) )
				{
					if( ((ShotY[a]+ShotH > BlockSize*i) && (ShotY[a]+ShotH < BlockSize*i+BlockSize)) || ((BlockSize*i+BlockSize > ShotY[a]) && (BlockSize*i < ShotY[a])) )
					{
						if(Map[i][j] == 0) continue;
						ShotFlag[a] = 0;
						ShotX[a] = PlayerX[a]+(PlayerW-ShotW)/2;
						ShotY[a] = PlayerY[a]+(PlayerH-ShotH)/2;
						
					}
				}
			}
		}


	}
}


// プレイヤーのショット攻撃
void Shot()
{
	for(a = 0 ; a < 2 ; a++)
	{
		if(ShotFlag[a] == 1)	// ショットがまだ発射されている場合
		{
			// 向きによってショットの進行方向を変更させる(０：上　１：右　２：下　３：左)
			switch(ShotMuki[a]) 
			{
				case 0:	ShotY[a] -= BulletSpeed;
						break;
				case 1:	ShotX[a] += BulletSpeed;
						break;
				case 2: ShotY[a] += BulletSpeed;
						break;
				case 3: ShotX[a] -= BulletSpeed;
						break;
			}

			// プレイヤーの向きに合わせてショットの画像を回転させる(０：上　１：右　２：下　３：左)
			switch(ShotMuki[a])
			{
				case 0: DrawRotaGraph(ShotX[a]+ShotW/2,ShotY[a]+ShotH/2,1,PHI_F,ShotHandle,TRUE,TRUE);
						break;
				case 1: DrawRotaGraph(ShotX[a]+ShotW/2,ShotY[a]+ShotH/2,1,-(PHI_F / 2),ShotHandle,TRUE,TRUE);
						break;
				case 2: DrawRotaGraph(ShotX[a]+ShotW/2,ShotY[a]+ShotH/2,1,PHI_F*2,ShotHandle,TRUE,TRUE);
						break;
				case 3: DrawRotaGraph(ShotX[a]+ShotW/2,ShotY[a]+ShotH/2,1,(PHI_F / 2),ShotHandle,TRUE,TRUE);
						break;
			}
		}
		else	// ショットが発射されていない場合
		{
			ShotX[a] = PlayerX[a]+(PlayerW-ShotW)/2;
			ShotY[a] = PlayerY[a]+(PlayerH-ShotH)/2;
		}
	}
}