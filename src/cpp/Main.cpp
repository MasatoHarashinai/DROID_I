#include "DxLib.h"	// Dx���C�u�����̃C���N���[�h

/*****************
	�萔��`
*****************/
#define MapSizeX 9		// �}�b�v�̉��̃}�X��
#define MapSizeY 9		// �}�b�v�̏c�̃}�X��
#define BlockSize 64	// 1�u���b�N������̕�
#define SideStart 608	// �X�e�[�^�X�o�[�̎n�܂�̂w���W
#define BulletSpeed 15	// �V���b�g�U���̈ړ����x
#define GodTime 500		// ���G���Ԓ萔
#define PHI_F	DX_PI	// pi

/*********************
	�v���g�^�C�v�錾
**********************/
void CreateAI();		// AI�v���C���[�̕`��֐�
void DrawPlayer();		// �v���C���[�̕`��֐�
void Collision();		// �Փ˔���̊֐�
void DrawMap();			// �}�b�v�̐����֐�
void Move();			// �v���C���[�̈ړ��֐�
void KeyInput();		// ���͌��o�̊֐�
void Shot();			// �V���b�g�̊֐�
void State();			// �v���C���[�̃X�e�[�^�X
void Initialize();		// �������֐�
void LiveOn3();			// ���E���h�X�^�[�g�̊֐�
void DrawStar();		// ���E���h�擾���̕\��

/********************
	�O���[�o���ϐ�
*********************/
// �}�b�v (1:����u�� 0:��)//
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

// �X�e�[�^�X�ϐ� //
int PlayerX[2] = {64,480};		// �v���C���[��X���W
int PlayerY[2] = {0,544};		// �v���C���[��Y���W
int PlayerW = 32;				// �v���C���[�摜�̕�
int PlayerH = 32;				// �v���C���[�摜�̍���
int PlayerMuki[2] = {2,0};		// �v���C���[�̌���
int PlayerLife[2] = {5,5};		// �v���C���[�̃��C�t
int PlayerFlag[2] = {1,1};		// �v���C���[�̐����t���O

/*	// ���݂�AI������
int AIX[4] = {0,0,0,0};			// AI��X���W
int AIY[4] = {0,0,0,0};			// AI��Y���W
int AIW = 32;					// AI�̕�
int AIH = 32;					// AI�̍���
int AIMuki[4] = {0,0,0,0};		// AI�̌���
int AIFlag[4] = {0,0,0,0};		// AI�̐����t���O
*/

int MoveFlag[2] = {0,0};		// �v���C���[�̈ړ��t���O
int MoveCnt[2] = {0,0};			// �v���C���[�ړ������̃J�E���^
int ShotX[2] = {0,0};			// ���@�e��X���W
int ShotY[2] = {0,0};			// ���@�e��Y���W
int ShotW = 16;					// ���@�e�̉摜�̕�
int ShotH = 16;					// ���@�e�̉摜�̍���
int ShotMuki[2] = {2,0};		// ���@�e�̌���
int ShotFlag[2] = {0,0};		// ���@�e�̑��݃t���O
int LastDamaged[2] = {0,0};		// �Ō�ɔ�e��������
int God[2] = {0,0};				// ���G���Ԃ̃t���O

// �n���h��(�t�@�C�����ʔԍ�) //
int PlayerHandle[2];	// �v���C���[�̉摜

/*
int AIHandle;
*/

int BackHandle;			// �w�i�̉摜
int BlockHandle;		// �u���b�N�̉摜
int ShotHandle;			// ���@�e�̉摜
int SideHandle;			// �X�e�[�^�X�o�[
int LabelHandle_PLAYER;	// "PLAYER"�̉摜
int LabelHandle_PLAYER2;// "PLAYER2"�̉摜
int LabelHandle_HP;		// "HP"�̉摜
int HPHandle;			// �n�[�g�̉摜
int StarHandle;			// ���̉摜
int Player1WinHandle;	// "PLAYER1WIN"�̉摜
int Player2WinHandle;	// "PLAYER2WIN"�̉摜
int StartHandle;		// �X�^�[�g��ʂ̉摜
int SelectHandle;		// �I����ʂ̉摜
int BGMHandle;			// BGM�̉��y
int ShotSoundHandle;	// �V���b�g�̌��ʉ�
int WinnerHandle;		// �����̃W���O��

//Etc //
int LargeFont;							// �傫�ڂ̃t�H���g�̎��ʎq
int NormalFont;							// ���ʂ̃t�H���g�̎��ʎq
int Color_Red = GetColor(255,0,0);		// �ԐF�̎��ʎq
int Color_Blue = GetColor(0,0,255);		// �F�̎��ʎq
int Color_Black	= GetColor(0,0,0);		// ���F�̎��ʎq
int a = 0;								//
int i = 0;								//	�ėp�J�E���^
int j = 0;								//
int Scene = 0;							// �V�[���萔
int EndFlag = 0;						// �Q�[���I���t���O
int Select = 0;							// �V�[���̑I��萔
int GameCount = 0;						// �����E���h��
int CountWin[2] = {0,0};				// Player�̏����J�E���g
int PlayerWin[2] = {0,0};				// Player�����������ǂ���

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow )

{
	// �����ݒ� //
	SetWindowSizeExtendRate(1.6,1.2);	// �Q�[����ʂ̊g��
	ChangeWindowMode(TRUE);				// �E�B���h�E���[�h�ł̋N��
	SetUseDirectInputFlag(TRUE);		// DirectInput�̋���
	SetDrawScreen(DX_SCREEN_BACK);		// �`�悷��Ώۂ𗠉�ʂɐݒ�
	SetGraphMode(800,576,32,60);		// �𑜓x�ƃt���[�����[�g�̐ݒ�
	DxLib_Init();						// Dx���C�u�����̏�����	

	// �摜�̓ǂݍ��� //
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

	//�@�T�E���h�̓ǂݍ��� //
	BGMHandle = LoadSoundMem("resource/sound/bgm.wav");
	ShotSoundHandle = LoadSoundMem("resource/sound/shot.mp3");
	WinnerHandle = LoadSoundMem("resource/sound/winner.mp3");

	// Font�̍쐬 //
	LargeFont = CreateFontToHandle(NULL,65,9);
	NormalFont = CreateFontToHandle(NULL,20,9);


	while( (ProcessMessage() == 0) && (CheckHitKey(KEY_INPUT_ESCAPE) == 0) && (EndFlag == 0))
	{
		// �V�[���̕���//
		switch(Scene)
		{
			// �X�^�[�g��� //
			case 0: Initialize();	// �Q�[���̏�Ԃ�����������
					StopSoundMem(WinnerHandle);	// �Q�[���I�[�o�[�ォ��߂����ꍇ�ɃT�E���h���~������
					while(CheckHitKey(KEY_INPUT_SPACE) == 0)
					{
						if(CheckHitKey(KEY_INPUT_ESCAPE) == 1) EndFlag = 1;
						ClearDrawScreen();
						DrawGraph(0,0,StartHandle,TRUE);
						ScreenFlip();
					}
					Scene = 1;
					break;
			// �Q�[����� //
			case 1: Initialize();
				    LiveOn3();
					PlaySoundMem(BGMHandle,DX_PLAYTYPE_LOOP,TRUE);
					while(Scene == 1 && (CheckHitKey(KEY_INPUT_ESCAPE) == 0))
					{
						// �c������ //
						ClearDrawScreen();

						// �w�i�̕`�� //
						DrawGraph(0,0,BackHandle,TRUE);
						DrawGraph(578,0,SideHandle,TRUE);
					
						// �Փ˔��� //
						Collision();

						// �V���b�g�U�� //
						Shot();

						// �v���C���[�̈ړ� //
						Move();

						// �X�e�[�^�X //
						State();

						// �v���C���[�̔z�u //
						DrawPlayer();
		
						// �����J�E���g�̕\�� //
						DrawStar();

						// Map���� //
						DrawMap();

						// �L�[�̌��o //
						KeyInput();

						// ����ʂ̏��𔽉f //
						ScreenFlip();

						// �Е����|�ꂽ�烉�E���h�I�� //
						if((PlayerFlag[0] == 0) || (PlayerFlag[1] == 0)) 
						{
							Scene = 2;
							if(PlayerFlag[1] == 0) CountWin[0] ++;	//�@�v���C���[�P�ɏ��������J�E���g������
							if(PlayerFlag[0] == 0) CountWin[1] ++;	// �v���C���[�Q�ɏ��������J�E���g������
							GameCount++;	// �����E���h�����J�E���g����
							StopSoundMem(BGMHandle);	// ��U�a�f�l���~������
						}
					}
					break;

			// ���E���h�I�� //
			case 2: PlaySoundMem(WinnerHandle,DX_PLAYTYPE_BACK,TRUE);
					while(1)
					{
						// �I���������_�Ń��E���h���𔽉f������ //
						DrawStar();
						
						// ���������̏��҃��x����\�� //
						if((PlayerFlag[0] == 1) && (PlayerFlag[1] == 0)) DrawGraph(0,0,Player1WinHandle,TRUE);
						if((PlayerFlag[1] == 1) && (PlayerFlag[0] == 0)) DrawGraph(0,0,Player2WinHandle,TRUE);
						ScreenFlip();

						// �Q�[���Z�b�g���ǂ����𔻕ʂ��� //
						if(GameCount == 2)
						{
							if(CountWin[0] == 2) PlayerWin[0] = 1;	// �v���C���[�P���Q�A�������ꍇ
							if(CountWin[1] == 2) PlayerWin[1] = 1;	// �v���C���[�Q���Q�A�������ꍇ
						}
						
						if(GameCount == 3)
						{
							if(CountWin[0] == 2 && CountWin[1] == 1) PlayerWin[0] = 1;	// 2:1�Ńv���C���[�P���������ꍇ
							if(CountWin[0] == 1 && CountWin[1] == 2) PlayerWin[1] = 1;	// 1:2�Ńv���C���[�Q���������ꍇ
						}

						
						if(CheckSoundMem(WinnerHandle) == 0 && (PlayerWin[0] == 1 || PlayerWin[1] == 1)) // �������I�������ꍇ�͏I����ʂ�
						{
							Scene = 3;
							break;
						}
						else if(CheckSoundMem(WinnerHandle) == 0 && (PlayerWin[0] == 0 && PlayerWin[1] == 0)) // �������I����Ă��Ȃ��ꍇ�͍ēx���E���h���J�n����
						{
							Scene = 1;
							break;
						}
					}
					break;

			// �����I�� //
			case 3: while(1)
					{
							DrawGraph(0,70,SelectHandle,TRUE);
							ScreenFlip();

							if(CheckHitKey(KEY_INPUT_R))	// �Đ킷�邽�ߊe�J�E���^�����Z�b�g����
							{
								CountWin[0] = 0;
								CountWin[1] = 0;
								PlayerWin[0] = 0;
								PlayerWin[1] = 0;
								GameCount = 0;
								Scene = 0;
								break;
							}
							if(CheckHitKey(KEY_INPUT_ESCAPE))	// �Q�[�����I��������
							{
								EndFlag = 1;
								break;
							}
					}
			default:break;
		}

	}

	DxLib_End(); // Dx���C�u�������I��������
	return 0;

}

// �Q�[���J�n�܂ł̃J�E���g�_�E��
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

// �X�e�[�W�̕`��
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


// �v���C���[�̕`��
void DrawPlayer()
{
	for(a = 0 ; a < 2; a++)
	{
		if(PlayerFlag[a] == 1)
		{
				//�@�v���C���[�̌����ɍ��킹�ĉ摜����]������(�O�F��@�P�F�E�@�Q�F���@�R�F��)
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

// �Q�[���̏�����
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
	   �v���C���[�P�̃L�[���o 
	**************************/
	// �ړ� //
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

	// �����]�� //
	if(CheckHitKey(KEY_INPUT_1) == 1 && (MoveFlag[0] == 0)) PlayerMuki[0] = 0;
	if(CheckHitKey(KEY_INPUT_2) == 1 && (MoveFlag[0] == 0)) PlayerMuki[0] = 1;
	if(CheckHitKey(KEY_INPUT_3) == 1 && (MoveFlag[0] == 0)) PlayerMuki[0] = 2;
	if(CheckHitKey(KEY_INPUT_4) == 1 && (MoveFlag[0] == 0)) PlayerMuki[0] = 3;

	// �V���b�g�U�� //
	if(CheckHitKey(KEY_INPUT_Z) == 1 && (ShotFlag[0] == 0))
	{
		ShotFlag[0] = 1;
		ShotMuki[0] = PlayerMuki[0];
		PlaySoundMem(ShotSoundHandle,DX_PLAYTYPE_BACK,TRUE);
	}
	
	/************************
	  �v���C���[�Q�̃L�[���o 
	*************************/
	// �ړ� //
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

	// �����]�� //
	if(CheckHitKey(KEY_INPUT_5) == 1 && (MoveFlag[1] == 0)) PlayerMuki[1] = 0;
	if(CheckHitKey(KEY_INPUT_6) == 1 && (MoveFlag[1] == 0)) PlayerMuki[1] = 1;
	if(CheckHitKey(KEY_INPUT_7) == 1 && (MoveFlag[1] == 0)) PlayerMuki[1] = 2;
	if(CheckHitKey(KEY_INPUT_8) == 1 && (MoveFlag[1] == 0)) PlayerMuki[1] = 3;

	// �V���b�g�U�� //
	if(CheckHitKey(KEY_INPUT_L) == 1 && (ShotFlag[1] == 0))
	{
		ShotFlag[1] = 1;
		ShotMuki[1] = PlayerMuki[1];
		PlaySoundMem(ShotSoundHandle,DX_PLAYTYPE_BACK,TRUE);
	}
}

// ���i�������J�E���g�j�̕`��
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

// �X�e�[�^�X�o�[��`�悷��
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

// �v���C���[�P�}�X���ړ�������i4 * 8 = 32Px�@���ړ��j
void Move()
{
	for(a = 0; a < 2 ; a++)
	{
		if(MoveFlag[a] == 1)
		{
			switch(PlayerMuki[a])	// �v���C���[�̌����ɍ��킹�Ĉړ�������ύX
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
			MoveCnt[a]++;	//�@�ړ��ʂ�����₷
			if(MoveCnt[a] == 8)	// 32pix�i�񂾂��~������
			{
				MoveFlag[a] = 0;
				MoveCnt[a] = 0;
			}
		}
	}
}

// �e�摜���m�̏Փ˔���
void Collision()
{
	for(a = 0 ; a < 2 ; a++)
	{
		/*****************
			�O�ǂƎ��@
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
		  �u���b�N�Ǝ��@
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
						if(MoveFlag[a] == 1) // �}�X�ړ����ɏՓ˂����ꍇ�̏���
						{
							MoveCnt[a] = 0;
							MoveFlag[a] = 0;
						}
						
						switch(PlayerMuki[a]) //�@�u���b�N�ɂ͂ݏo�Ă��܂����߂̈ʒu���� (�O�F��@�P�F�E�@�Q�F���@�R�F��)
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
		�@�V���b�g�@�Ɓ@�v���C���[
		*************************/
		// 1P�̃V���b�g����2P //
		if(PlayerFlag[0] == 1)
		{
			if( ((ShotX[0]+ShotW > PlayerX[1]) && (ShotX[0]+ShotW < PlayerX[1]+PlayerW)) || ((PlayerX[1]+PlayerW > ShotX[0]) && (PlayerX[1] < ShotX[0])) )
			{
				if( ((ShotY[0]+ShotH > PlayerY[1]) && (ShotY[0]+ShotH < PlayerY[1]+PlayerH)) || ((PlayerY[1]+PlayerH > ShotY[0]) && (PlayerY[1] < ShotY[0])) )
				{
					if(ShotFlag[0] == 1)	// �V���b�g�𔭎˂��Ă���ꍇ�̂ݔ��肷��i�v���C���[�̉摜�̈���ɃV���b�g���i�[����Ă���ꍇ�Ƀ_���[�W����𖳂������߁j 
					{
						if(GetNowCount() - LastDamaged[1] < GodTime) // ���G���Ԃ��I�����Ă��邩�ǂ���
						{
							God[1] = 1;
						}
						else
						{
							God[1] = 0;
						}

						if(God[1] == 0)	// ���G���Ԓ��ɏՓ˂������ǂ����𔻒�
						{
							LastDamaged[1] = GetNowCount(); // ���G���Ԍv�Z�p�Ƀ_���[�W��H������u�Ԃ̎�����ۑ�
							if(PlayerLife[1] > 0) //�@�����Ă���΃v���C���[�̃��C�t�����炷
							{
								PlayerLife[1] --;
							}

							if(PlayerLife[1] <= 0) // ����ł���΃v���C���[�̐����t���O���O�ɕύX����
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
		// 2P�̃V���b�g����1P //
		if(PlayerFlag[1] == 1)
		{
			if( ((ShotX[1]+ShotW > PlayerX[0]) && (ShotX[1]+ShotW < PlayerX[0]+PlayerW)) || ((PlayerX[0]+PlayerW > ShotX[1]) && (PlayerX[0] < ShotX[1])) )
			{
				if( ((ShotY[1]+ShotH > PlayerY[0]) && (ShotY[1]+ShotH < PlayerY[0]+PlayerH)) || ((PlayerY[0]+PlayerH > ShotY[1]) && (PlayerY[0] < ShotY[1])) )
				{
					if(ShotFlag[1] == 1)	// �V���b�g�𔭎˂��Ă���ꍇ�̂ݔ��肷��i�v���C���[�̉摜�̈���ɃV���b�g���i�[����Ă���ꍇ�Ƀ_���[�W����𖳂������߁j
					{
						if(GetNowCount() - LastDamaged[0] < GodTime)	// ���G���Ԃ��I�����Ă��邩�ǂ���
						{
							God[0] = 1;
						}
						else
						{
							God[0] = 0;
						}

						if(God[0] == 0)		// ���G���Ԓ��ɏՓ˂������ǂ����𔻒�
						{
							LastDamaged[0] = GetNowCount();	// ���G���Ԍv�Z�p�Ƀ_���[�W��H������u�Ԃ̎�����ۑ�
							if(PlayerLife[0] > 0)	//�@�����Ă���΃v���C���[�̃��C�t�����炷
							{
								PlayerLife[0] --;
							}
	
							if(PlayerLife[0] <= 0)	// ����ł���΃v���C���[�̐����t���O���O�ɕύX����
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
			�V���b�g �Ɓ@�O��
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
		   �V���b�g�ƃu���b�N
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


// �v���C���[�̃V���b�g�U��
void Shot()
{
	for(a = 0 ; a < 2 ; a++)
	{
		if(ShotFlag[a] == 1)	// �V���b�g���܂����˂���Ă���ꍇ
		{
			// �����ɂ���ăV���b�g�̐i�s������ύX������(�O�F��@�P�F�E�@�Q�F���@�R�F��)
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

			// �v���C���[�̌����ɍ��킹�ăV���b�g�̉摜����]������(�O�F��@�P�F�E�@�Q�F���@�R�F��)
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
		else	// �V���b�g�����˂���Ă��Ȃ��ꍇ
		{
			ShotX[a] = PlayerX[a]+(PlayerW-ShotW)/2;
			ShotY[a] = PlayerY[a]+(PlayerH-ShotH)/2;
		}
	}
}