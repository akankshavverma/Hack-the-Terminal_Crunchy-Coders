#include<iostream>
#include<conio.h>
#include<dos.h> 
#include <windows.h>
#include <time.h>


#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 26
#define WIN_WIDTH 70
#define MENU_WIDTH 20
#define GAP_SIZE 7
#define Enemy_DIF 45


using namespace std; 
 
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

int enemyY[3];
int enemyX[3];
int enemyFlag[3];
char bird[7][5] = { ' ',' ','8',' ',' ',
					'|',' ','8',' ','|',
					' ','8','8','8',' ', }; 
int birdPos = WIN_WIDTH/2;
int score = 0;
int bullets[20][4];
int bulletsLife[20];
int bIndex = 0;

void gotoxy(int x, int y){
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}
void setcursor(bool visible, DWORD size) {
	if(size == 0)
		size = 20;	
	
	CONSOLE_CURSOR_INFO lpCursor;	
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(console,&lpCursor);
}
void drawBorder(){ 
	
	for(int i=0; i<SCREEN_WIDTH; i++){ 
		gotoxy(i,SCREEN_HEIGHT); cout<<"=";
	}
	
	for(int i=0; i<SCREEN_HEIGHT; i++){
		gotoxy(80,i); cout<<"�";
	}
}
void genEnemy(int ind){
	enemyX[ind] = 3 + rand()%(WIN_WIDTH-10);  
}
void drawEnemy(int ind){
	if( enemyFlag[ind] == true ){
		gotoxy(enemyX[ind], enemyY[ind]);   cout<<"      q      ";  
		gotoxy(enemyX[ind], enemyY[ind]+1); cout<<"  o .-o-. o  "; 
		gotoxy(enemyX[ind], enemyY[ind]+2); cout<<"o-( o o o )-o"; 
		gotoxy(enemyX[ind], enemyY[ind]+3); cout<<"  o .-o-. o  ";
		gotoxy(enemyX[ind], enemyY[ind]+4); cout<<"      b      ";
		 
	} 
}
void eraseEnemy(int ind){
	if( enemyFlag[ind] == true ){
		gotoxy(enemyX[ind], enemyY[ind]); cout<<"             ";  
		gotoxy(enemyX[ind], enemyY[ind]+1); cout<<"             "; 
		gotoxy(enemyX[ind], enemyY[ind]+2); cout<<"             "; 
		gotoxy(enemyX[ind], enemyY[ind]+3); cout<<"             ";
		gotoxy(enemyX[ind], enemyY[ind]+4); cout<<"             ";
	} 
}
void resetEnemy(int ind){
	eraseEnemy(ind);
	enemyY[ind] = 4;
	genEnemy(ind);
}
void genBullet(){
	bullets[bIndex][0] = 22;
	bullets[bIndex][1] = birdPos;
	bullets[bIndex][2] = 22; 
	bullets[bIndex][3] = birdPos+4; 
	bIndex++;
	if( bIndex == 20)
		bIndex = 0;
}
void moveBullet(){
	for(int i=0; i<20; i++){
		if( bullets[i][0] > 2 )
			bullets[i][0]--;
		else
			bullets[i][0] = 0;
		
		if( bullets[i][2] > 2 )
			bullets[i][2]--;
		else
			bullets[i][2] = 0;
	} 
}
void drawBullets(){
	for(int i=0; i<20; i++){
		if( bullets[i][0] > 1){
			gotoxy(bullets[i][1],bullets[i][0]); cout<<"."; 
			gotoxy(bullets[i][3],bullets[i][2]); cout<<".";
		}
	}
}
void eraseBullets(){
	for(int i=0; i<20; i++){
		if( bullets[i][0] >= 1 ){
			gotoxy(bullets[i][1],bullets[i][0]); cout<<" ";
			gotoxy(bullets[i][3],bullets[i][2]); cout<<" ";
		}
	}
}
void eraseBullet(int i){ 
	gotoxy(bullets[i][1],bullets[i][0]); cout<<" ";
	gotoxy(bullets[i][3],bullets[i][2]); cout<<" "; 
}
void drawBird(){
	for(int i=0; i<3; i++){
		for(int j=0; j<5; j++){
			gotoxy(j+birdPos, i+22); cout<<bird[i][j];
		}
	}
}
void eraseBird(){
	for(int i=0; i<3; i++){
		for(int j=0; j<5; j++){
			gotoxy(j+birdPos, i+22); cout<<" ";
		}
	}
}
 
int collision(){
	if( enemyY[0]+4 >= 23 ){
		if( enemyX[0] + 4 - birdPos >= 0 && enemyX[0] + 4 - birdPos < 8  ){
			return 1;
		}
	}
	return 0;
}
int bulletHit(){
	for(int i=0; i<20; i++){
		for(int j=0; j<4; j+=2){
			if( bullets[i][j] != 0 ){
				if( bullets[i][j] >= enemyY[0] && bullets[i][j] <= enemyY[0]+4 ){
					if( bullets[i][j+1] >= enemyX[0] && bullets[i][j+1] <= enemyX[0]+4 ){
						eraseBullet(i);
						bullets[i][j] = 0;
						resetEnemy(0);
						return 1;
					}
				}
				if( bullets[i][j] >= enemyY[1] && bullets[i][j] <= enemyY[1]+4 ){
					if( bullets[i][j+1] >= enemyX[1] && bullets[i][j+1] <= enemyX[1]+4 ){
						eraseBullet(i);
						resetEnemy(1); 
						bullets[i][j] = 0;
						return 1;
					}
				}
			}
		}
	}
	return 0;
}
void gameover(){
	system("cls");
	cout<<endl;
	cout<<"\t\t=========================="<<endl;
	cout<<"\t\t-------- Game Over -------"<<endl;
	cout<<"\t\tCorona Won @because@ you were unable to save the world!!"<<endl;
	cout<<"\t\tBetter luck for White ,Black & yellow Fungus"<<endl;
	cout<<"\t\t=========================="<<endl<<endl;
	cout<<"\n\t\t {     }";
    cout<<"\n\t\t| () () |";
    cout<<"\n\t\t {  ^  }";
    cout<<"\n\t\t  |||||";
    cout<<"\n\t\t  ||||| ";
	cout<<"\n\n\t\tPress any key to go back to menu.";
	getch();
}
void updateScore(){
	gotoxy(WIN_WIDTH + 7, 5);cout<<"Score: "<<score<<endl;
}


void aboutdev(){
	
	system("cls");
	cout<<"About developer team";
	cout<<"\n================";
	cout<<"\n Source code originally made by TEAM - Crunchy Coders \n";
	cout<<"\nTEAM MEMBERS are -";
	cout<<"\n -Aditya Kumar (20MIP10008) ,\n -Akanksha Verma (20MIP10002) ,\n -Ishika Shrivastava (20MIP10035),\n -Ujjwal Srivastava (20MIP10020) ,\n -Deepti Shakya (20MIP10041),\n -Anannya Manojawas (20MIP10028),\n -Kallol Kanti Bera (20MIP10009)";
	cout<<"\n\nPress any key to go back to menu";
	getch();
}

class instructions{
	public:
		void display()
		{
			system("cls");
			gotoxy(10,8);cout<<"\n-press space bar to fire vaccine droplets";
		    gotoxy(10,9);cout<<"\n-If you and virus got [EXACT] head to head then game will over";
		    gotoxy(10,10);cout<<"\n-press W ,A,S,D on your keyboard to navigate player(vaccine)";
		    gotoxy(10,13);cout<<"\n-press any key on keyboard to visit main menu.";
		    getch();
		}
};

class pl {
	public:
		void play(){
	
	birdPos = -1 + WIN_WIDTH/2;
	score = 0;
	enemyFlag[0] = 1;  
	enemyFlag[1] = 1;
	enemyY[0] = enemyY[1] = 4;
	
	for(int i=0; i<20; i++){
		bullets[i][0] = bullets[i][1] = 0; 
	}
	
	system("cls"); 
	drawBorder();
	genEnemy(0);
	genEnemy(1);
	updateScore();
	
	gotoxy(WIN_WIDTH + 5, 2);cout<<"Go Corona Go!!";
	gotoxy(WIN_WIDTH + 6, 4);cout<<"----------";
	gotoxy(WIN_WIDTH + 6, 6);cout<<"----------";
	gotoxy(WIN_WIDTH + 2, 12);cout<<"Controls to play ";
	gotoxy(WIN_WIDTH + 7, 13);cout<<"-------- ";
	gotoxy(WIN_WIDTH + 1, 14);cout<<" A -Left movement";
	gotoxy(WIN_WIDTH + 1, 15);cout<<" D -Right movement";
	gotoxy(WIN_WIDTH + 1, 16);cout<<" Spacebar = Vaccine Splash";
	
	gotoxy(10, 5);cout<<"Press any key to start";
	getch();
	gotoxy(10, 5);cout<<"                      ";
	
	while(1){
		if(kbhit()){
			char ch = getch();
			if( ch=='a' || ch=='A' ){
				if( birdPos > 2 )
					birdPos -= 2;
			}
			if( ch=='d' || ch=='D' ){
				if( birdPos < WIN_WIDTH-7 )
					birdPos += 2;
			}
			if(ch==32){
				genBullet();
			} 
			if(ch==27){
				break;
			}
		}
		
		drawBird();
		drawEnemy(0);
		drawEnemy(1);
		drawBullets();
		if( collision() == 1  ){
			gameover();
			return;
		}
		if(  bulletHit() == 1 ){
			score++;
			updateScore(); 
		} 
		Sleep(200);
		eraseBird();
		eraseEnemy(0);
		eraseEnemy(1);
		eraseBullets();
		moveBullet();   
		
		if( enemyFlag[0] == 1 )
			enemyY[0] += 1;
		
		if( enemyFlag[1] == 1 )
			enemyY[1] += 1;
		 
		if( enemyY[0] > SCREEN_HEIGHT-5 ){ 
			resetEnemy(0); 
		}
		if( enemyY[1] > SCREEN_HEIGHT-5 ){ 
			resetEnemy(1); 
		}
	}
}
};


int main()
{
	setcursor(0,0); 
	srand( (unsigned)time(NULL));
	 
	do{
		system("cls");
		system("color 3");
		gotoxy(10,1); cout<<"\n  _____  _____      _____ ____  _____   ____  _   _              _____  ____     _   _ ";
		gotoxy(10,2); cout<<"\n | ____|| |__| |   / ____/ __| |  __ | / __ || | | |   /|       / ____|/ __ |   | | | |";
		gotoxy(10,3); cout<<"\n | |  __| |  | |  | |   | |  | | |__) | |  | |  || |  /  |     | |  __| |  | |  | | | |";
		gotoxy(10,4); cout<<"\n | | |_ | |  | |  | |   | |  | |  _  /| |  | | . ` | / || |    | | |_ | |  | |  | | | |";
		gotoxy(10,5); cout<<"\n | |__| | |__| |  | |___| |__| | | | || |__| | | | |/ ____ |   | |__| | |__| |  |_| |_|";
		gotoxy(10,6); cout<<"\n  |_____||____/   |_____|____/ |_|  |_||____/|_| |_/_/    |_|  |_____|| ____/   (_) (_)";
		gotoxy(10,11); cout<<" -------------------------- "; 
		gotoxy(10,12); cout<<" |     !GO CORONA GO!      | "; 
		gotoxy(10,13); cout<<" --------------------------";
		gotoxy(10,15); cout<<"1. Start Game";
		gotoxy(10,16); cout<<"2. About Developer Team";	 
		gotoxy(10,17); cout<<"3. Quit";
		gotoxy(10,18); cout<<"4. Instructions";
		gotoxy(10,20); cout<<"Select option: ";
		char op = getche();
		
		pl p;
		instructions i;
		if( op=='1') p.play();
		else if( op=='2') aboutdev();
		else if( op=='3') exit(0);
		else if( op=='4') i.display();
		
	}while(1);
	
	return 0;
}
