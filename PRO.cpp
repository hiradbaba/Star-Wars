#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<conio.h>
#include<time.h>
#include<string.h>
#include "myMusic.h"


// defining space and arrow keys-----------------------------------------------------
#define UP	72
#define DOWN	80
#define LEFT	75
#define RIGHT	77
#define SPACE	32
//defining colors--------------------------------------------------------------------
#define RED		FOREGROUND_RED 	 | FOREGROUND_INTENSITY
#define GREEN 	FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define BLUE 	FOREGROUND_BLUE	 | FOREGROUND_INTENSITY
#define YELLOW	FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY
#define CYAN	FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
#define MAGNETA	FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY
#define WHITE 	FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY
//---------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
/*
 PROGRAM PARTS:
 -MENU            -reading highscore and position file(find_highscore,readP)
 -enemy type 1: killed - income_generate() - count() -hit
 -enemy type 2: killed - sp_random() - count()-hit
 -bullet type A: shot 
 -bullet type B: spshot
 -bullet type RPG: nRPG - Rshot
*/
//----------------------------------------------------------------------------------------
// SCORE: BASED ON SHOOTING ENEMIES --- LEVEL:BASED ON SCORE------------------------
int score;
int level;
int killed;

char Nname[20];
char Cname[20];
char Hname[20];
//----------------------------------------------------------------------------------

// Point structure which includes the coordination ---------------------------------
struct point{
	int x;
	int y;
};
//----------------------------------------------------------------------------------

//Income structure which includes the NUMBER of enemies coming and Their positions--
struct income{
	int n;
	point enemy[1000];
};
//----------------------------------------------------------------------------------
struct bull{
	int n;
	point bullet[10000];
};
//GLOBALS--------------------------------------------------------------------------
point me;
income en;
income spen;
int nRPG;


//GOXY function changes the cursors postion to print in different areas--------------
void goxy(int x,int y){
	COORD coord={x,y};
   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);

}
//COLOR function changes the color---------------------------------------------------
void color(int a){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole,a);

}

//-----------------------------------------------------------------------------------

 //this function geneterates the special enemy which dies with special weapon---------------
int sp_random(){
	int i=0;
 	time_t t;
	srand((unsigned) time(&t));
	int conflict = 1;
	while(conflict == 1 ){
        conflict = 0;
        for(i=0;i<spen.n;i++){
            spen.enemy[i].x=rand()%40;
            spen.enemy[i].y=-1*(rand()%(spen.n*2));
            if(spen.enemy[i].x == 0){
                spen.enemy[i].x = 1;
            }
            if(spen.enemy[i].y == 0){
                spen.enemy[i].y = 1;
            }
        }

        for(i=0;i<spen.n;i++){
            for(int j=0; j<spen.n; j++){
                if( i != j){
                    if((abs(spen.enemy[j].x-spen.enemy[i].x)<3) && (abs(spen.enemy[j].y-spen.enemy[i].y)<3) ){
                        conflict = 1;
                    }
                }
            }
        }
        for(i=0;i<spen.n;i++){
            for(int j=0; j<en.n; j++){
                    if((abs(en.enemy[j].x-spen.enemy[i].x)<6 && abs(spen.enemy[i].y-en.enemy[j].y)<4) ){
                        conflict = 1;
                    }
                }
            }
        
        
	}


   // income temp = spen;

	int minx=spen.enemy[0].y;
	int minp=0;
	for(i=1; i<spen.n; i++){
		if(spen.enemy[i].y<minx){
			minx=spen.enemy[i].y;
			minp=i;

		}
	}
	return minp;
}
 //---------------------------------------------------------------------
 /*Income_Generate: This function generates enemies' posittions randomly based on the max size of x and the number of enemies
 after generating it will find the last enemy who comes from the top of the screen and returns it position in the array-----*/
int income_generate(){
	time_t t;
	srand((unsigned) time(&t));
	int i=0;
	int conflict = 1;
	while(conflict == 1 ){
        conflict = 0;
        for(i=0;i<en.n;i++){
            en.enemy[i].x=rand()%40;
            en.enemy[i].y=-1*(rand()%(en.n*2));
            if(en.enemy[i].x == 0){
                en.enemy[i].x = 1;
            }
            if(en.enemy[i].y == 0){
                en.enemy[i].y = 1;
            }
        }

        for(i=0;i<en.n;i++){
            for(int j=0; j<en.n; j++){
                if( i != j){
                    if((abs(en.enemy[j].x-en.enemy[i].x)<3) && (abs(en.enemy[j].y-en.enemy[i].y)<3) ){
                        conflict = 1;
                    }
                }
            }
        }
	}


    //income temp = en;

	int minx=en.enemy[0].y;
	int minp=0;
	for(i=1; i<en.n; i++){
		if(en.enemy[i].y<minx){
			minx=en.enemy[i].y;
			minp=i;

		}
	}
	return minp;
}
//-------------------------------------------------------------
//this fuctions reads the highscore file-----------------------
int find_score(FILE* p){
	char y[4];
	fscanf(p,"%s %s",Hname,y);
	int i;
	i=atoi(y);
	return i;
}
//this function reads the saved the data used for the game------
void readP(){
	FILE*pr=fopen("position.txt","r");
	char posx[4],posy[4],posen[4],posscore[4],posNrpg[4],pospen[4];
	fscanf(pr,"%s %s %s %s %s %s %s",posx,posy,posen,posscore,posNrpg,pospen,Cname);
	score=atoi(posscore);
	me.y=atoi(posy);
	me.x=atoi(posx);
	en.n=atoi(posen);
	nRPG=atoi(posNrpg);
	spen.n=atoi(pospen);
	fclose(pr);
}
//this function counts the number of enemies in the screen----------
int count(){
	int i,counter=0;
	for(i=0;i<en.n;i++){
		if((en.enemy[i].y<=30 && en.enemy[i].y>0) || (spen.enemy[i].y<=30 && spen.enemy[i].y>0))
			counter++;
			
	}
	return counter;
}
//------------------------------------------------------------------
// this function prints the MENU of the game------------------------
void MENU(int i){
	goxy(20,12);
	color(YELLOW);
	printf("STAR WARS");
	goxy(20,15);
	color(CYAN);
	printf("New Game");
	goxy(20,16);
	printf("Continue");
	goxy(20,17);
	printf("High Score");
	goxy(20,18);
	printf("EXIT");
	goxy(18,i);
	printf("%c",1);

}
//--------------------------------------------------------------------------------------------
int mode=0;


//MAIN function-------------------------------------------------------------------------------
int main(){
	//Menu-----------------------------------------------------------------------------------
	while(1){

		if(mode==0){

				int i=15;
				int a,ci;
				MENU(i);
			while(1){

				ci=i;
				if(_kbhit()){
					a=_getch();
					if(a==UP && i>15){
						Beep(300, 200);
						i--;
					}
					else if(a==DOWN && i<18){
						Beep(300, 200);
						i++;
					}
					else if(a==13){
						Beep(500,200);
						if(i==15){
							mode=1;
							system("cls");
							break;
						}
						else if(i==16){
							mode=2;
							system("cls");
							break;
						}
						else if(i==18){
							exit(0);
						}
						else if(i==17){
							mode=3;
							system("cls");
							break;
						}
					
					MENU(i);
					}

				}
				if(ci!=i){
					goxy(18,ci);
					printf(" ");
					goxy(18,i);
					printf("%c",1);
				}
			}

		}
	//--------------------------------------------------------------------------------------
	//defining & initializing variables------------------------------------------------------
	if(mode==1 || mode==2){
		//data-----------------------------
		int min,mins;
		int time=100;
		point copy;
		bull B;
		bull SP;
		point RPG;
		B.n=0;
		SP.n=0;
		int spshot=0;
		int Rshot=0;
		spen.n=2;
		int hitx=0;
		
		
		
		//----------------------------------
		if(mode==1){
            goxy(15,10);
            color(13);
            printf("Please The Pilot's Name: ");
            scanf("%s",Nname);
            system("cls");
            //----------------------------------
            //starting data---------------------
            me.x=20;
            me.y=20;
            en.n=6;
            score=0;
            level=0;
            nRPG=3;
		}
		if(mode==2){
			readP();
			level=score/10;
		}

		FILE* fp=fopen("highscore.txt","r");
		int highscore=find_score(fp);

		fclose(fp);
		//-----------------------------------
		min=income_generate();
		mins=sp_random();
	// a: the character which we receive from keyboard  shot:a flag which is 1 if we have shoot----
		int a;
		int shot=0,hit=0;
		while(1){
		// copy would be the previous position of me or in other words our ship---------------
			copy=me;
			killed = 0;
			hitx=0;
			if(_kbhit()){
				a=_getch();
				//The keys we use in the game----------------------------------------------------
				if(a==UP && me.y>0){
					me.y--;
				}
				else if(a==DOWN && me.y<30){
					me.y++;
				}
				else if(a==LEFT && me.x>0)
					me.x--;
				else if(a==RIGHT && me.x<40)
					me.x++;
				else if(a==SPACE){
					shot=1;
					B.n+=1;
					B.bullet[B.n-1].x=copy.x+2;
					B.bullet[B.n-1].y=copy.y;
					Beep(250, 20);
				}
				else if(a=='c'){
					spshot=1;
					SP.n+=1;
					SP.bullet[SP.n-1].x=copy.x+2;
					SP.bullet[SP.n-1].y=copy.y;
					Beep(350, 20);


				}
				else if(a=='e'){
					goxy(15,15);
					color(YELLOW);
					printf("ARE YOU SURE YOU WANT TO EXIT? (Y/N)");
					a=getch();
					//Saving and ending the game--------------------
					if(a=='y'){
						FILE* x=fopen("position.txt","w");
						if(mode==1)
							fprintf(x,"%d %d %d %d %d %d %s",me.x,me.y,en.n,score,nRPG,spen.n,Nname);
						else if (mode==2)
							fprintf(x,"%d %d %d %d %d %d %s",me.x,me.y,en.n,score,nRPG,spen.n,Cname);
						fclose(x);
						system("cls");
						mode=0;
						break;
					}
					else{
						goxy(15,15);
						printf("                                    ");
						continue;
					}
				}
				//RPG button--------------------------------------
				else if(a=='x' && Rshot==0){
					Beep(2000,50);
					RPG.x=me.x+2;
					RPG.y=me.y;
					Rshot=1;
				}
				goxy(copy.x,copy.y);
    	        printf("     ");
				continue;
			}

		//printing our ship, bullets, enemy ships------------------------
		//ship,space,score and level--------------
			goxy(copy.x,copy.y);
			printf("     ");
			goxy(me.x,me.y);
			color(CYAN);
			printf("[%c%c%c]",205,234,205);
			goxy(20,35);
			color(YELLOW);
			printf("SCORE:%d",score);
			goxy(20,37);
			printf("LEVEl:%d",level);
			goxy(10,35);
			printf("RPG:%d",nRPG);
			//-----------------------------------------
			//normal bullet-----------------------------------
			if(shot){
				for(int k=0;k<B.n;k++){
					if(B.bullet[k].y>0){
					goxy(B.bullet[k].x,B.bullet[k].y-1);
					color(WHITE);
					printf("|");

				if(B.bullet[k].y!=copy.y){
					goxy(B.bullet[k].x,B.bullet[k].y);
					printf(" ");
				}

				B.bullet[k].y--;
				}
				if(B.bullet[k].y==0){

					goxy(B.bullet[k].x,0);
					printf(" ");
					B.bullet[k].x=NULL;
					B.bullet[k].y=NULL;
					}
				}
			}
			//special bullet-------------------------------------
			if(spshot){
				for(int k=0;k<SP.n;k++){
					if(SP.bullet[k].y>0){
					goxy(SP.bullet[k].x,SP.bullet[k].y-1);
					color(RED);
					printf("%c",186);
					
				if(SP.bullet[k].y!=copy.y){
					goxy(SP.bullet[k].x,SP.bullet[k].y);
					printf(" ");
				}
				SP.bullet[k].y--;
				}
				if(SP.bullet[k].y==0){

					goxy(SP.bullet[k].x,0);
					printf(" ");
					SP.bullet[k].x=NULL;
					SP.bullet[k].y=NULL;
					}
				}
			}
			//RPG SHOT which kills all the enemies--------------------
			if(Rshot && nRPG){

				if(RPG.y>0){
					goxy(RPG.x,RPG.y-1);
					color(YELLOW);
					printf("%c",254);
					}
					if(RPG.y!=copy.y){
					goxy(RPG.x,RPG.y);
					printf(" ");
				}
				RPG.y--;
				if(RPG.y<=10){
					for(int k=0;k<en.n;k++){
						goxy(en.enemy[k].x,en.enemy[k].y-1);
						printf("   ");
						goxy(en.enemy[k].x,en.enemy[k].y);
						printf("   ");
						goxy(RPG.x,RPG.y);
						printf(" ");
						if(level>=2){
						goxy(spen.enemy[k].x,spen.enemy[k].y-1);
						printf("   ");
						goxy(spen.enemy[k].x,spen.enemy[k].y);
						printf("   ");
						}
					}
					hitx=1;
					for(int k=0;k<en.n;k++){
						en.enemy[k].x=NULL;
						en.enemy[k].y=NULL;
						spen.enemy[k].x=NULL;
						spen.enemy[k].y=NULL;
					}
						nRPG--;
						Rshot=0;
					}
				}

			//-------------------------------------------
			//enemy--------------------------------------
			for(int j=0;j<en.n;j++){
				if(en.enemy[j].y>0 && en.enemy[j].y<31 && en.enemy[j].x!=NULL){

                    goxy(en.enemy[j].x,en.enemy[j].y);
                    color(GREEN);
                    printf("%c%c%c",195,15,180);
                    goxy(en.enemy[j].x,en.enemy[j].y-1);
                    printf("   ");

                    if(en.enemy[j].y==30){
                        goxy(en.enemy[j].x,en.enemy[j].y);
                        printf("   ");
                    }
                }
                    //Enemy hits me--------------------------------
                if((en.enemy[j].x<=me.x+4 && en.enemy[j].x>=me.x-2) && en.enemy[j].y==me.y){
                    hit=1;
                    break;
                }
            //enemy shot-------------------------------------------
            for(int k=0;k<B.n;k++){
            	if(B.bullet[k].x!=0 && B.bullet[k].y!=0){
                if(en.enemy[j].x-B.bullet[k].x<=0 && en.enemy[j].x-B.bullet[k].x>=-2 && ( B.bullet[k].y==en.enemy[j].y || B.bullet[k].y==en.enemy[j].y+1)){
                	goxy(en.enemy[j].x,en.enemy[j].y);
                    printf("   ");
                    en.enemy[j].x=NULL;
                    en.enemy[j].y=NULL;
                    goxy(B.bullet[k].x,B.bullet[k].y);
                    printf(" ");
                   // goxy(B.bullet[k].x,B.bullet[k].y-1);
                   // printf("   ");
                    B.bullet[k].x=NULL;
                    B.bullet[k].y=NULL;
                    //score++;
                    killed = 1;
                	}
                	}
            	}
                en.enemy[j].y++;
			}
			//special enemies--------------------------------------------------------
			if(level>=2){
				
				for(int j=0;j<en.n;j++){
				if(spen.enemy[j].y>0 && spen.enemy[j].y<31 && spen.enemy[j].x!=NULL){

                    goxy(spen.enemy[j].x,spen.enemy[j].y);
                    color(13);
                    printf("%c%c%c",204,207,185);
                    goxy(spen.enemy[j].x,spen.enemy[j].y-1);
                    printf("   ");

                    if(spen.enemy[j].y==30){
                        goxy(spen.enemy[j].x,spen.enemy[j].y);
                        printf("   ");
                    }
                }
                    //special enemy hits me--------------------------------
                if((spen.enemy[j].x<=me.x+4 && spen.enemy[j].x>=me.x-2) && spen.enemy[j].y==me.y){
                    hit=1;
                    break;
                }
            //special enemy shot-------------------------------------------
            for(int k=0;k<SP.n;k++){
            	if(SP.bullet[k].x!=0 && SP.bullet[k].y!=0){
                if(abs(SP.bullet[k].x-spen.enemy[j].x)<=2 && ( SP.bullet[k].y==spen.enemy[j].y || SP.bullet[k].y==spen.enemy[j].y+1)){
                	goxy(spen.enemy[j].x,spen.enemy[j].y);
                    printf("   ");
                    spen.enemy[j].x=NULL;
                    spen.enemy[j].y=NULL;
                    goxy(SP.bullet[k].x,SP.bullet[k].y);
                    printf(" ");
                   // goxy(SP.bullet[k].x,SP.bullet[k].y-1);
                   // printf(" ");
                    SP.bullet[k].x=NULL;
                    SP.bullet[k].y=NULL;
                    //score++;
                    killed = 1;

                	}
                }
            	}
                spen.enemy[j].y++;
			}
			}

		//when the last enemy reaches to the bottom we have to generate positions again
			if(en.enemy[min].y==32){

				min=income_generate();

			}
			if(spen.enemy[mins].y==32){
				mins=sp_random();
			}
			if (killed == 1){
                score++;
                int plevel=level;
                level=score/10;
                if(level-plevel!=0){
                    en.n+=2;
                    if(level%3==0)
                        time-=20;
                	if(level%2==0){
                	spen.n+=2;
					}
       		 	}
            }
            if(hitx==1){
            	score+=count();
            	int plevel=level;
                level=score/10;
                if(level-plevel!=0){
                    en.n+=2;
                    if(level%3==0)
                        time-=20;
                	if(level%2==0){
                	spen.n+=2;
					}
			}
			}
            //the end of the story------------------------------------------
			if(hit!=0){
				if(score>highscore){
					fp=fopen("highscore.txt","w");
					if(mode==1)
						fprintf(fp,"%s %d",Nname,score);
					else if(mode==2)
						fprintf(fp,"%s %d",Cname,score);
					fclose(fp);
                }

				system("cls");
				goxy(15,10);
				color(RED);
				Beep(1000,500);
				printf("GAME OVER!\n");
				goxy(10,12);
				if(mode==1)
					printf("Captain %s ... You Did Well",Nname);
				else if(mode==2)
					printf("Captain %s ... You Did Well",Cname);
				goxy(10,14);
				printf("Out There But Unfortunately");
				goxy(10,16);
				printf("The DARK SIDE Took Over The Galaxy");
				goxy(10,18);
				printf("Come Back And Rule The Galaxy");
				goxy(10,20);
				printf("Before Darth Vader Finds Luke Skywalker");
				goxy(10,22);
				printf("And Kills Every Last One Of JEDIs");
				music();
				system("cls");
					mode=0;
					break;

			}
			Sleep(time);
	}
	}
	//HIGH SCORE PREVIEW---------------------------------------------------------------
	if(mode==3){
		char x[20];
		FILE* f=fopen("highscore.txt","r");
		fgets(x,20,f);
		goxy(15,10);
		color(CYAN);
		printf("And The Top Player Is:");
		goxy(15,13);
		color(YELLOW);
		printf("%s",x);
		fclose(f);

		if(_kbhit()){
			system("cls");
			mode=0;
		}
	}
}
	return 0;
}
