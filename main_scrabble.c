/* ======================================================================================================
    PROGRAM   : main_scrabble.c
    DESKRIPSI : File ekstensi .c untuk projek tugas besar mata kuliah Dasar-Dasar Pemrograman (DDP)
                kelas 1B semester 1
    AUTHOR    : By Kelompok B5, anggota :
                1) Rachmat Purwa Saputra
                2) Muhammad Ferdy Fauzan
                3) Ghessa Theniana
    DATE      : 10 Januari 2021
    COMPILER  : Code::Blocks 20.03 dan Dev C++
    JUMLAH MODUL DARI LUAR : 3 (tiga) buah

    Catatan : Ini bukanlah versi final, masih terdapat kekurangan disana-sini...
======================================================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>    //untuk keperluan tipe data boolean
#include <string.h>     //untuk keperluan tipe data array of char dan operasi sahihnya
#include <sys/time.h>   //untuk keperluan jam permainan selesai
#include <time.h>       //untuk keperluan modul delay, dan sintaks srand
#include <windows.h>    //untuk keperluan mode, color, cls, pause
#include <ctype.h>      //untuk keperluan toupper
#include <conio.h>      //untuk keperluan dan aktivasi getche() supaya tidak dapat warning


//TIGA MACAM TIPE DATA BENTUKAN
typedef struct Player{
    char plyName[2][8];         //index 0 u/ player1, dan index 1 u/ player2
    int plyScore[2];            //index 0 u/ player1, dan index 1 u/ player2
    int numGiliran;
    char playerWord[7];
    char hurufDipegang[2][7];   //index 0 u/ player1, dan index 1 u/ player2
    char writDir;
} Player;

typedef struct Papan{
    int loc_X;
    int loc_Y;
} Papan;

typedef struct AvailableLetter{
    char gudangHuruf[101];
    int letterValue[28];
    char commonLetter[28];
} AvailableLetter;

/*** DEKLARASI PROTOTIPE MODUL-MODUL YANG DIGUNAKAN DI DALAM PROGRAM ***/

//MODUL OUTSIDE THE GAME USED HERE
void gotoxy(int x, int y);
void delay(int ms);

//MODUL INSIDE THE GAME USED HERE
//BIG CHUNCK OF THE GAME SECTION LISTED FIRST
int homeMenu();

void playGame();
void showAbout();
void showHelp();

void beforeEnteringGameStage(struct Player *dtPlayer, struct AvailableLetter *dtHuruf);
void duringPlayGame(struct Player *dtPlayer, struct AvailableLetter *dtHuruf, struct Papan *dtPapan);
void afterPlayGameFinished(struct Player *dtPlayer);
void gotQuitTime(char *wkt);

void writeHistory(struct Player *dtPlayer, char wkt[]);
void showHistory();

//SMALLER CHUNCK OF THE GAME SECTION LISTED SECONDLY HERE
//PARTS WHICH IS CALLED WITHIN beforeEnteringGameStage PROCEDURE
void printInputNameBox();
void inputPlayerName(char (*nameArr)[8]);
void fillWordBasket(struct AvailableLetter *dtHuruf);
void showLoadingBar();
void printPapan();

void fillSpecialField(struct Papan *dtPapan);

//PARTS WHICH IS CALLED WITHIN duringPlayGame PROCEDURE AND ITS BRANCHES PROBABLY CONTAINED TOO
void initializeOutputForEachRole(struct Player *dtPlayer,struct AvailableLetter *dtHuruf);
void printScoreBox();
void printHurufPeganganBox();
void printRandomizeHuruf(char *gudangHuruf, char (*hurufDipegang)[7], int giliran);
void printMenuBox();

int  chosenMenu(int pilihan);
int  roleExchange(int numGiliran);

int  inputWordAsPerGiliran(struct Player *dtPlayer, struct AvailableLetter *dtHuruf, struct Papan *dtPapan);
void printCurrentGiliranText(char (*nameArr)[8], int giliran);
void printInputKataChosen();

int  inputPosisiDiPapan(struct Papan *dtPapan);
void convertXYforBoard(int X, int Y, struct Papan *dtPapan);
void chooseWritDir(struct Player *dtPlayer);
int  checkMatchWithHurufPemain(struct Player *dtPlayer, struct Papan *dtPapan);
void sucessToInput(int value);
void plotToBoard(char *kata, struct Papan *dtPapan, char arah);

int  inputWordByPlayer(struct Player *dtPlayer, struct Papan *dtPapan);
bool cekKamus(char *kataDiinput);
int  scoringWord(char *plyWord, struct AvailableLetter *dtHuruf);
void outputWordAsPerGiliran(char *playerWord, char (*plyName)[8], int giliran);
void printScorePlayer(int *skor, int giliran);

//PARTS WHICH IS CALLED WITHIN shuffleLah PROCEDURE AND ITS BRANCHES PROBABLY CONTAINED TOO
void shuffleLah(struct Player *dtPlayer);
void shuffleHurufPegangan(char (*hurufmuKini)[7], int giliran);
void printAfterShuffled(char (*hurufmuKini)[7],int giliran);

//PARTS WHICH IS CALLED WITHIN afterPlayGameFinished PROCEDURE AND ITS BRANCHES PROBABLY CONTAINED TOO
void whoIsWinner(struct Player *dtPlayer);

/***** MODUL UTAMA *****/
int main(){
    //YANG BELUM BERHASIL DIBUAT DALAM PROGRAM INI DIANTARANYA: LEADERBOARD, CEK KESAHIHAN POSISI, SKORING UNTUK KOTAK SPECIAL
   	homeMenu();

    return 0;
}

/*** DEFINISI MODUL-MODUL PADA PROGRAM SCRABBLE INI ***/
void gotoxy(int x, int y){
    /*Procedure gotoxy untuk memidahkan kursor ke letak x,y terkait di layar konsol
      Parameter input nilai x dan y
      Sumber referensi : https://www.geeksforgeeks.org/how-to-use-gotoxy-in-codeblocks/
    */
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),c);
}

void delay(int ms){
    /*Digunakan untuk keperluan loading bar saja
      Menahan agar tampilan output ke layar terhenti sesaat selama ms milisekon
      Sumber referensi : https://www.geeksforgeeks.org/time-delay-c/
    */
    clock_t timeDelay = ms + clock();
    while(timeDelay > clock());
}

void printInputNameBox(){
    /* Tujuan : untuk menampilkan kerangka kotak / tabel input nama pemain saat akan memasuki permainan*/

	system("mode 90, 12");
    printf("\n\n\t\t");
    char ch1, ch2, ch3, ch4, ch5, ch6, ch7, ch8, ch9, ch10, ch11;   //tabel menggunakan kode ASCII EXTENDED (unsigned char)
    ch1 = 185; //left-ext-middle-right-up-above
    ch2 = 186; //two-vertical
    ch3 = 187; //right-corner-upper
    ch4 = 188; //right-corner-below
    ch5 = 200; //left-corner-below
    ch6 = 201; //left-corner-upper
    ch7 = 204; //right-ext-middle-left-up-above
    ch8 = 205; //two-horizontal
    ch9 = 206; //intersection
    ch10 = 202; //up-ext-middle-left-right
    ch11 = 203; //dow-ext-midlle-left-right

    for(int j = 1; j < 60; j++){    //print untuk baris pertama tabel
        if (j == 1)
            printf("%c",ch6);
        else if (j == 59)
            printf("%c",ch3);
        else
            printf("%c",ch8);
    }
    printf("\n\t\t%c  \t\t     SCRABBLE GAMERS    \t\t  %c\n\t\t",ch2,ch2);

    for(int j = 1; j < 60; j++){
        if(j == 1)
            printf("%c",ch7);
        else if (j == 30)
            printf("%c",ch11);
        else if (j == 59)
            printf("%c",ch1);
        else
            printf("%c",ch8);
    }

    printf("\n\t\t%c  Masukkan nama Player 1:   %c",ch2,ch2);

    gotoxy(74,5);
    printf("%c\n",ch2);

    printf("\t\t");

    for(int j = 1; j < 60; j++){
        if(j == 1)
            printf("%c",ch7);
        else if (j == 30)
            printf("%c",ch9);
        else if (j == 59)
            printf("%c",ch1);
        else
            printf("%c",ch8);
    }
    printf("\n");

    printf("\t\t%c  Masukkan nama Player 2:   %c",ch2,ch2);
    gotoxy(74,7);
    printf("%c\n",ch2);

    printf("\t\t");
    for(int j = 1; j < 60; j++){
        if(j == 1)
            printf("%c",ch5);
        else if(j == 30)
            printf("%c",ch10);
        else if(j == 59)
            printf("%c\n",ch4);
        else
            printf("%c",ch8);
    }
}

int homeMenu(){
    /* Tujuan : menampilkan tampilan home menu, yang terdapat 5 menu di dalamnya*/

	system("mode 90, 20");
	int pilihanMenu;
	char waktuAkhirBermain[64];
	do{
		system("cls");
		system("COLOR 0B");
		gotoxy(35,5);
		printf("SCRABBLE GAMERS");
		gotoxy(33,6);
		printf("===================");
		gotoxy(35,7);
		printf("[1]Play Game");
		gotoxy(35,8);
		printf("[2]Help");
		gotoxy(35,9);
		printf("[3]About");
		gotoxy(35,10);
		printf("[4]History");
		gotoxy(35,11);
		printf("[0]Exit Game");
		gotoxy(35,13);
		printf("Choose Menu : ");
		scanf("%d", &pilihanMenu);

		if(pilihanMenu < 0 || pilihanMenu > 4){
            printf("Salah input brodii...!!!\n\n");
            system("pause");
        }
            switch(pilihanMenu){
                case 1:
                    playGame();
                    break;
                case 2:
                    showHelp();
                    break;
                case 3:
                    showAbout();
                    break;
                case 4:
                    showHistory(waktuAkhirBermain);
                    break;
            }

        }while(pilihanMenu!=0);
    return 0;
}

void playGame(){
    /* Procedure yang mengatur jalannya fitur Play game setelah pergi dari home Menu*/

    system("mode 160, 40");
	//Definisi untuk variabel string yang menampung teks waktu
	char waktuAkhirBermain[64];

    //Definisi struct dan ukuran alokasi memorinya
    struct Player *dtPlayer = (Player*)malloc(sizeof(Player));
    struct AvailableLetter *dtHuruf = (AvailableLetter*)malloc(sizeof(AvailableLetter));
    struct Papan *dtPapan = (Papan*)malloc(sizeof(Papan));

    beforeEnteringGameStage(dtPlayer,dtHuruf);      //modul yang mengurus hingga bagian saat loading selesai dimuat

    //WITHIN THE PROCESS OF ENTERING WORD INTO THE BOARD
    duringPlayGame(dtPlayer,dtHuruf,dtPapan);   //modul yang mengurus daur hidup permainan
    afterPlayGameFinished(dtPlayer);

    gotQuitTime(waktuAkhirBermain);
    writeHistory(dtPlayer,waktuAkhirBermain);      //untuk menulis ke file histori permainan
}

void showHelp(){
    /* Tujuan : untuk menampilkan file help dari secondary storage ke program ketika dipanggil menu Help*/
 	system("mode 200, 80");
 	system("cls");
 		FILE*fhp;
 		char help[100];

 		fhp = fopen("help1.txt","r");
 		while(!feof(fhp)){
 			fgets(help,100,fhp);
 			printf("%s",help);
		}
		fclose(fhp);
		system("pause");
		main();

 }

void showAbout(){
    /* Tujuan : untuk menampilkan file about berisi credits (author, release date, dan penjelasan singkat aplikasi)
       dari secondary storage ke program ketika dipanggil menu About*/
	system("mode 70, 22");
	system("cls");
		FILE*fa;
		char about[100];

		fa = fopen("about.txt","r");
		while(!feof(fa)){
			fgets(about,100,fa);
			printf("%s",about);
		}
		fclose(fa);
		//getche();
		system("pause");
		main();
		getch();
}

void inputPlayerName(char (*nameArr)[8]){
    /*Menerima input nama player sebelum masuk ke stage permainan*/
    printInputNameBox();

    gotoxy(49,5);
    scanf(" %7[^\n]%*c", nameArr[0]);
    gotoxy(49,7);
    scanf(" %7[^\n]%*c", nameArr[1]);
}

void showLoadingBar(){
	system("mode 102, 15");
    system("cls");
    system("COLOR 0B");
    int i, j, Y_LOC = 6, progressLoad = 0;
    char lcu, rcu, lcb, rcb, vert, horz, thickBar, percentageSign;
    lcu = 201; rcu = 187; lcb = 200; rcb = 188; vert = 186; horz = 205; thickBar = 219; percentageSign = 37;

    //DUA PERULANAGN INI UNTUK MENGATUR TAMPILAN "FRAME" ATAU KOTAK PROSES LOADING BAR SAJA
    for(i = 1; i < 4; i++){
        for(j = 1; j < 55; j++){
            if(i == 1 && j == 1){
                gotoxy(20, Y_LOC);
                printf("%c", lcu);
            }
            if(i == 1 && j == 54){
                gotoxy(73, Y_LOC);
                printf("%c\n", rcu);
            }
            if(i == 3 && j == 1){
                gotoxy(20, Y_LOC + 2);
                printf("%c", lcb);
            }
            if(i == 3 && j == 54){
                gotoxy(73, Y_LOC + 2);
                printf("%c", rcb);
            }
            if(i == 1 || i == 3){
                if(j > 1 && j < 54)
                    printf("%c", horz);
            }
            if(j == 1 || j == 54){
                if(i == 2 && j == 54){
                    gotoxy(73, Y_LOC + 1);
                    printf("%c\n", vert);
                }
                if(i == 2 && j == 1){
                    gotoxy(20, Y_LOC + 1);
                    printf("%c", vert);
                }
            }
        }
    }

    gotoxy(41, 10);
    printf("Now loading ....");

    //PERULANGAN YANG MENGATUR JALANNYA BAR LOADING BAR
    for(i = 0; i < 50; i++){
        gotoxy(22 + i, Y_LOC + 1);
        printf("%c", thickBar);
        gotoxy(77, Y_LOC + 1);
        progressLoad += 2;
        printf("%d%c", progressLoad, percentageSign);
        delay(50);
    }

    gotoxy(20, 10);
    printf("PROCESS COMPLETE! NOW PROCEED TO THE GAME BY CLICKING ENTER ONCE ");
    getche();
}

void printPapan(){
    /*Untuk menampilkan tampilan papan scrabble ukuran 15 x 15*/
	system("mode 160, 40");
    system("cls");
    int baris = 1, kolom, n = 61;
    char ch1, ch2, ch3, ch4, ch5, ch6, ch7, ch8, ch9, ch10, ch11;
    ch1 = 185; //left-ext-middle-right-up-above
    ch2 = 186; //two-vertical
    ch3 = 187; //right-corner-upper
    ch4 = 188; //right-corner-below
    ch5 = 200; //left-corner-below
    ch6 = 201; //left-corner-upper
    ch7 = 204; //right-ext-middle-left-up-above
    ch8 = 205; //two-horizontal
    ch9 = 206; //intersection
    ch10 = 202; //up-ext-middle-left-right
    ch11 = 203; //dow-ext-midlle-left-right

    //print nomor sumbu-X
    int xForAxis = 12,yForAxis = 3, axisNum = 1;
    for(xForAxis = 12; xForAxis <= 68; xForAxis += 4){
        gotoxy(xForAxis,yForAxis);
        printf("%d", axisNum);
        axisNum++;
    }

    //print nomor sumbu-Y
    xForAxis = 7,yForAxis = 5, axisNum = 1;
    for(yForAxis = 5; yForAxis <= 33; yForAxis += 2){
        gotoxy(xForAxis,yForAxis);
        printf("%d", axisNum);
        axisNum++;
    }

    //print papan huruf 15 x 15
    int jj;
    while(baris <= n){
        kolom = 1;
        gotoxy(10,4 + (baris - 1));
        while(kolom <= n){
            jj = 0;
            if(baris == 1){
                if(kolom == 1){
                    printf("%c", ch6);
                }
                if(kolom >= 2 && kolom <= 30){
                    if(kolom % 2 != 0){
                        printf("%c", ch11);
                    } else {
                        while(jj <= 2){
                            printf("%c", ch8);
                            jj++;
                        }
                    }
                }
                if(kolom == 31){
                    printf("%c\n", ch3);
                }
            }

            if(baris >= 2 && baris <= 30){
                if((baris - 1) % 2 != 0){
                    if(kolom == 1)
                        printf("%c", ch2);
                    else if(kolom >= 2 && kolom <= 60)
                        if((kolom - 1) % 4 != 0)
                            printf(" ");
                        else
                            printf("%c", ch2);
                    else if(kolom == 61)
                        printf("%c\n", ch2);
                }
                else if(((baris - 1) % 2 == 0)){
                    if(kolom == 1)
                        printf("%c", ch7);
                    if(kolom >= 2 && kolom <= 60){
                        if((kolom - 1) % 4 == 0)
                            printf("%c", ch9);
                        else
                            printf("%c", ch8);
                    }
                    if(kolom == 61)
                        printf("%c\n", ch1);
                }
            }

            if(baris == 31){
                if(kolom == 1){
                    printf("%c", ch5);
                }
                if(kolom >= 2 && kolom <= 30){
                    if(kolom % 2 != 0){
                        printf("%c", ch10);
                    } else {
                        while(jj <= 2){
                            printf("%c", ch8);
                            jj++;
                        }
                    }
                }
                if(kolom == 31){
                    printf("%c\n", ch4);
                }
            }
            kolom++;
        }
        baris++;
    }
}

void printScoreBox(){
    /*Untuk menampilkan tampilan kotak/tabel tempat skor kedua pemain berada*/

    char ch1, ch2, ch3, ch4, ch5, ch6, ch7, ch8, ch9, ch10, ch11;
    ch1 = 185; //left-ext-middle-right-up-above
    ch2 = 186; //two-vertical
    ch3 = 187; //right-corner-upper
    ch4 = 188; //right-corner-below
    ch5 = 200; //left-corner-below
    ch6 = 201; //left-corner-upper
    ch7 = 204; //right-ext-middle-left-up-above
    ch8 = 205; //two-horizontal
    ch9 = 206; //intersection
    ch10 = 202; //up-ext-middle-left-right
    ch11 = 203; //dow-ext-midlle-left-right

    gotoxy(80,4);
    printf("   S  C  O  R  E   ", ch2, ch2);
    gotoxy(80,5);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", ch6, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch11, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch3);
    gotoxy(80,6);
    printf("%c   P1   %c   P2   %c", ch2, ch2, ch2);
    gotoxy(80,7);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", ch7, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch9, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch1);
    gotoxy(80,8);
    printf("%c", ch2);
    gotoxy(89,8);
    printf("%c", ch2);
    gotoxy(98,8);
    printf("%c", ch2);


    gotoxy(80,9);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", ch5, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch10, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch4);
}

void printMenuBox(){
    /*Untuk menampilkan tampilan tabel/kotak yang berisi 5 pilihan menu saat permainan berlangusng*/
    char ch1, ch2, ch3, ch4, ch5, ch6, ch7, ch8, ch9, ch10, ch11;
    ch1 = 185; //left-ext-middle-right-up-above
    ch2 = 186; //two-vertical
    ch3 = 187; //right-corner-upper
    ch4 = 188; //right-corner-below
    ch5 = 200; //left-corner-below
    ch6 = 201; //left-corner-upper
    ch7 = 204; //right-ext-middle-left-up-above
    ch8 = 205; //two-horizontal

    //TO CLEAN UP PREVIOUS TEXT WRITTEN HERE, SO IT KIND OF FLUSHED
    gotoxy(80,11);
    printf("                                      ");
    gotoxy(80,12);
    printf("                                                         ");
    gotoxy(80,13);
    printf("                                      ");
    gotoxy(80,14);
    printf("                                                         ");
    gotoxy(80,15);
    printf("                                                         ");
    gotoxy(80,16);
    printf("                                                         ");
    gotoxy(80,17);
    printf("                                       ");
    gotoxy(80,18);
    printf("                                                         ");
    gotoxy(80,19);
    printf("                                       ");
    gotoxy(80,21);
    printf("                                                         ");
    gotoxy(80,20);
    printf("                                            ");


    gotoxy(80,11);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", ch6, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch3);
    gotoxy(80,12);
    printf("%c  PILIHAN MENU:  %c", ch2, ch2);
    gotoxy(80,13);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", ch7, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch1);
    gotoxy(80,14);
    printf("%c 1. Input kata   %c", ch2, ch2);
    gotoxy(80,15);
    printf("%c 2. Pass giliran %c", ch2, ch2);
    gotoxy(80,16);
    printf("%c 3. Shuffle      %c", ch2, ch2);
    gotoxy(80,17);
    printf("%c 4. Swap huruf   %c", ch2, ch2);
    gotoxy(80,18);
    printf("%c 5. Surrender    %c", ch2, ch2);
    gotoxy(80,19);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", ch7, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch1);
    gotoxy(80,20);
    printf("%c Pilih menu: [ ] %c", ch2, ch2);
    gotoxy(80,21);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", ch5, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch4);
}

int chosenMenu(int pilihan){
    /*Untuk menerima input menu mana yang dipilih (1 / 2 / 3 / 4 / 5) dari dalam kotak menu box
      saat permainan berlangusng
    */
    gotoxy(94,20);
    printf("[");
    scanf("%d", &pilihan);

    return pilihan;
}

void printHurufPeganganBox(){
    /*Untuk menampilkan tampilan kotak/tabel dimana huruf setiap pemain mendapat giliran main berada*/
    char ch1, ch2, ch3, ch4, ch5, ch6, ch7, ch8, ch9, ch10, ch11;
    ch1 = 185; //left-ext-middle-right-up-above
    ch2 = 186; //two-vertical
    ch3 = 187; //right-corner-upper
    ch4 = 188; //right-corner-below
    ch5 = 200; //left-corner-below
    ch6 = 201; //left-corner-upper
    ch7 = 204; //right-ext-middle-left-up-above
    ch8 = 205; //two-horizontal

    gotoxy(80,25);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", ch6, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch3);
    gotoxy(80,26);
    printf("%c  HURUF PEMAIN:  %c", ch2, ch2);
    gotoxy(80,27);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", ch7, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch1);
    gotoxy(80,28);
    printf("%c                 %c", ch2, ch2);
    gotoxy(80,29);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", ch5, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch8, ch4);
}

void printRandomizeHuruf(char *gudangHuruf, char (*hurufDipegang)[7], int giliran){
    /*Untuk menampilkan huruf-huruf tiap pemain (huuf pegangan) yang dirandom dari gudangHuruf*/
    int idx_rand;

    gotoxy(83,28);
    srand(time(NULL));
    for(int i = 0; i < 7; i++){
        idx_rand = rand() % 100;
        printf("%c ", gudangHuruf[idx_rand]);
        hurufDipegang[giliran - 1][i] = gudangHuruf[idx_rand];
    }
}

void printInputKataChosen(){
    /*Untuk menampilkan tampilan kerangka teks instruksi saat pemain memilih menu input kata*/
    char ch2 = 205;

    gotoxy(80,11);
    printf("                                      ");
    gotoxy(80,12);
    printf(" Tulis posisi awal kata (format: 6 11) ");
    gotoxy(80,13);
    printf("                                      ");
    gotoxy(80,14);
    printf(" Vertical atau horizontal (format [v/h] ");
    gotoxy(80,15);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2);
    gotoxy(80,16);
    printf(" Apa kata yang akan Anda input ? ---> ");
    gotoxy(80,17);
    printf("                                      ");
    gotoxy(80,18);
    printf(" Huruf keberapa saja yang dipilih? -->      ");
    gotoxy(80,19);
    printf(" (harap pisahkan dengan spasi)        ");
    gotoxy(80,20);
    printf("                                      ");
    gotoxy(80,21);
    printf("                                      ");
}

void sucessToInput(int value){
    /*Untuk menampilkan pesan saat seorang pemain berhasil memasukkan kata ke papan dan  memperoleh skor*/
    char ch2 = 205;

    gotoxy(80,17);
    printf("                                       ");
    gotoxy(80,18);
    printf(" Kata yang diinput tersebut bernilai %d                           ", value);
    gotoxy(80,19);
    printf("                                       ");
    gotoxy(80,21);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2, ch2);
    gotoxy(80,20);
    printf(" Selamat, player ini skornya bertambah!!\n");
    getche();
}

void printCurrentGiliranText(char (*nameArr)[8], int giliran){
    /*Untuk menampilkan nama pemain terkini yang sedang mendapat giliran bermain scrabble*/
    gotoxy(50,1);
    printf("        ");
    gotoxy(22,1);
    printf("SEKARANG ADALAH GILIRAN MAIN %s\n", nameArr[giliran-1]);
}

bool cekKamus(char *kataDiinput){
    /*Untuk melakukan proses cek kata yang diinput pemain, apakah terdapat di dalam kamus*/
    char ch[21];
    int getWord = 0;

    FILE *fp;
    fp = fopen("PlayerDict.txt","r");
    if (fp== NULL)
    {
        system("cls");
        printf("There's no such file! Alas, cannot be opened!\n\n");
        system("pause");
        exit(EXIT_FAILURE); //program terhenti karena file tidak ada
    }

    while(!feof(fp) && getWord == 0)
    {
        fscanf(fp,"%s",ch);
        if( strcmp(ch,kataDiinput) == 0 )
            getWord++;
    }

    fclose(fp);

    if(getWord > 0)
        return true;
    else
        return false;
}

int roleExchange(int numGiliran){
    /*Untuk mengganti nomor giliran pemain agar setiap selesai 1 giliran pemain, pemain lain bisa bermain*/
    if(numGiliran == 1)
        return 2;
    else
        return 1;
}

int scoringWord(char *plyWord, struct AvailableLetter *dtHuruf){
    //Menghitung skor dari kata yang dimasukkan pemain ke papan scrabble
    int akumulasiNilaiPerKata = 0;

    for(int i = 0; i < strlen(plyWord); i++){
        for(int j = 0; j < 28; j++){
            if(plyWord[i] == dtHuruf->commonLetter[j]){
                akumulasiNilaiPerKata += dtHuruf->letterValue[j];
            }
        }
    }

    sucessToInput(akumulasiNilaiPerKata);

    return akumulasiNilaiPerKata;
}

void printScorePlayer(int *skor, int giliran){
    //Menampilkan skor pemain di kotak skor pemain
    if(giliran == 1){
        gotoxy(84,8);
        printf("%d", skor[giliran - 1]);
    }
    if(giliran == 2){
        gotoxy(93,8);
        printf("%d", skor[giliran - 1]);
    }
}

void chooseWritDir(struct Player *dtPlayer){
    //Meminta masukan 'v' atau 'h' dari pemain untuk menentukan direksi atau arah penulisan kata di papan
    fflush(stdin); fflush(stdout);
    scanf(" %c", &dtPlayer->writDir);
}

void plotToBoard(char *kata, struct Papan *dtPapan, char arah){
    //Memindahkan kata yang diinput ke papan permainan Scrabble sesuai dengan arah direksi penulisan yang dipilih pemain
    if(arah == 'v' || arah == 'V'){
        int idx = 0;
        do{
            gotoxy(dtPapan->loc_X,dtPapan->loc_Y);
            putchar(kata[idx]);
            idx++; dtPapan->loc_Y += 2; //bergeser ke bawah
        } while(idx < strlen(kata));
    }
    else if(arah == 'h' || arah == 'H'){
        int idx = 0;
        do{
            gotoxy(dtPapan->loc_X,dtPapan->loc_Y);
            putchar(kata[idx]);
            idx++; dtPapan->loc_X += 4;  //bergeser ke kanan
        } while(idx < strlen(kata));
    }
}

int checkMatchWithHurufPemain(struct Player *dtPlayer, struct Papan *dtPapan){
    //Melakukan proses pengecekan, apakah semua huruf yang dimasukkan pemain sama dengan huruf-huruf yang dialamatkan oleh nomor indeks letak huruf
    // di kotak huruf pemain. Indeks letak huruf 1 ada di paling kiri kotak huruf pemain
    int idx, eqFound;
    do{
        gotoxy(107,4);
        printf("                                              ");

        int idxHurufDipilih[strlen(dtPlayer->playerWord)];
        idx = 0;
        memset(idxHurufDipilih,0,strlen(dtPlayer->playerWord));

        char temp = '\0';
        gotoxy(119,18);
        printf("                ");
        gotoxy(120,18);
        printf("\b");
        do{
            scanf("%d%c", &idxHurufDipilih[idx], &temp);
            if(idxHurufDipilih[idx] == 0)
                break;  //menghentikan perulangan
            idx++;
        } while (temp != '\n');

        int j = 0;
        eqFound= 0;
        if(idxHurufDipilih[0] != 0){
            for(int k = 0; k < strlen(dtPlayer->playerWord); k++){
                if(dtPlayer->playerWord[k] == (*(dtPlayer->hurufDipegang + (dtPlayer->numGiliran - 1)))[idxHurufDipilih[j] - 1]){
                    eqFound++;
                }
                j++;
            }

            if(eqFound == strlen(dtPlayer->playerWord)){
                plotToBoard(dtPlayer->playerWord,dtPapan,dtPlayer->writDir); //jika benar, maka akan di plot ke papan
                return 1;
            }
            else{
                gotoxy(107,4);
                printf("BAD INPUT (DOESN'T MATCH WITH LETTER INDEX)!"); //jika tidak, maka kata tidak akan diplot ke papan, dan proses di modul ini harus diulangi pemain
                getche();
            }
        } else {
            return 0;
        }
    } while (eqFound != idx);
}

int inputWordByPlayer(struct Player *dtPlayer, struct Papan *dtPapan){
    //menginput 4 macam masukan : letak di papan, arah penulisan kata, kata apa yang akan diinput, dan indeks letak huruf pada kotak huruf pemain
    bool statusKata;
    int matching;

    do{
        gotoxy(119,14); printf("  ");
        gotoxy(118,16); printf("            ");
        gotoxy(42,36); printf("        ");
        gotoxy(118,12); printf("     ");
        int pilihPosisi = inputPosisiDiPapan(dtPapan);
        if(pilihPosisi == 1){
            gotoxy(119,14); printf(" ");
            chooseWritDir(dtPlayer);
            gotoxy(118,16); printf(" "); //this operation sets the cursor at such position

            fflush(stdin);
            scanf("%7[^\n]%*c", dtPlayer->playerWord);  //nanti panggil modul get x,y position, dari struct Papan


            //Change letter case into uppercase for all characters
            if((statusKata = cekKamus(dtPlayer->playerWord)) == true){
                for(int i =0; i < strlen(dtPlayer->playerWord); i++)
                    dtPlayer->playerWord[i] = toupper(dtPlayer->playerWord[i]);

                matching = checkMatchWithHurufPemain(dtPlayer,dtPapan);
            }
            else {
                gotoxy(107,4);
                printf("KATA SALAH!!");
            }
        }
        else if(pilihPosisi == 0){
            matching = 0;
            statusKata = true;
        }

    } while(statusKata == false);

    if(matching == 1){
        //proses hitung skor dengan melihat keberadaannya di papan, apakah menginnjak field DW/DL/TL/TW atau tidak
        //belum terpikirkan bagaimana, mengingat modul skor memanggil parameter yang berbeda dengan yang diperlukan
        // di proses ini

        return 1;
    }
    else if (matching == 0)
        return 0;
}

void outputWordAsPerGiliran(char *playerWord, char (*plyName)[8], int giliran){
    gotoxy(3,36);
    printf("Inilah kata masukkan player %s: ", plyName[giliran-1]);
    gotoxy(42,36);
    printf("%s\n", playerWord);
}

int inputPosisiDiPapan(struct Papan *dtPapan){
    //meminta masukan berupa dua angka , representasi kolom dan baris dari pemain
    //dua angka representasi baris dan kolom dipisahkan oleh spasi saat diinputkan
    gotoxy(118,12); printf(" ");
    fflush(stdin);

    scanf("%d %d", &dtPapan->loc_X, &dtPapan->loc_Y);
    if(dtPapan->loc_X != 0 && dtPapan->loc_Y != 0){
        convertXYforBoard(dtPapan->loc_X, dtPapan->loc_Y, dtPapan);
        return 1;
    }
    else
        return 0;  //jika pemain memasukkan 0 0, maka pemain batal memasukkan kata ke papan
}

int inputWordAsPerGiliran(struct Player *dtPlayer, struct AvailableLetter *dtHuruf, struct Papan *dtPapan){
    //mengatur proses input kata setiap giliran main pemain
    fflush(stdin);
    printInputKataChosen();

    int berhasilInput = inputWordByPlayer(dtPlayer,dtPapan);
    if(berhasilInput == 1){
        dtPlayer->plyScore[(dtPlayer->numGiliran) - 1] += scoringWord(dtPlayer->playerWord,dtHuruf);

        outputWordAsPerGiliran(dtPlayer->playerWord,dtPlayer->plyName,dtPlayer->numGiliran);
        printScorePlayer(dtPlayer->plyScore,dtPlayer->numGiliran);

        dtPlayer->numGiliran = roleExchange(dtPlayer->numGiliran);

        return 1;
    }
    else{
        return 0;  //maknanya, pemain membatalkan proses memasukkan kata ke papan
    }
}

void convertXYforBoard(int X, int Y, struct Papan *dtPapan){
    //karena nilai X dan Y yang diinput pengguna saat proses input kata
    //tidak merujuk pada nilai koordinat X dan Y pada layar konsol,
    //modul ini mengconvert nilai X dan Y masukkan pengguna agar bisa dipakai
    //oleh modul gotoxy
    X = 4 * (X + 2);
    Y = (2 * Y) + 3;

    dtPapan->loc_X = X;
    dtPapan->loc_Y = Y;
}

void shuffleHurufPegangan(char (*hurufmuKini)[7], int giliran){
    //menshuffle huruf pemain yang ada di tampilan kotak huruf pemain
    int a,b,c,d,e,f,temp;

    srand(time(NULL));
    for(int i = 0; i < 14; i++){
        a = rand() % 7; b = rand() % 7; c = rand() % 7; d = rand() % 7; e = rand() % 7; f = rand() % 7;

        temp = hurufmuKini[giliran-1][a];
        hurufmuKini[giliran-1][a] = hurufmuKini[giliran-1][b];
        hurufmuKini[giliran-1][b] = hurufmuKini[giliran-1][c];
        hurufmuKini[giliran-1][c] = hurufmuKini[giliran-1][d];
        hurufmuKini[giliran-1][d] = hurufmuKini[giliran-1][e];
        hurufmuKini[giliran-1][e] = hurufmuKini[giliran-1][f];
        hurufmuKini[giliran-1][f] = temp;
    }
}

void printAfterShuffled(char (*hurufmuKini)[7],int giliran){
    //menampilkan huruf yang sudah di shuffled di kotak huruf pemain
    fflush(stdin); fflush(stdout);
    gotoxy(82,28);
    for(int i = 0; i < 7; i++){
        printf("%c ", hurufmuKini[giliran-1][i]);
    }
}

void shuffleLah(struct Player *dtPlayer){
    //mengatur proses shuffle jika menu ini dipilih pemain
    fflush(stdin);
    shuffleHurufPegangan(dtPlayer->hurufDipegang,dtPlayer->numGiliran);
    printAfterShuffled(dtPlayer->hurufDipegang,dtPlayer->numGiliran);
}

void initializeOutputForEachRole(struct Player *dtPlayer,struct AvailableLetter *dtHuruf){
    //mengatur tampilan konsol untuk setiap giliran main pemain berganti
    printScoreBox();
    printHurufPeganganBox();
    printRandomizeHuruf(dtHuruf->gudangHuruf,dtPlayer->hurufDipegang,dtPlayer->numGiliran);
    printMenuBox();
    printCurrentGiliranText(dtPlayer->plyName,dtPlayer->numGiliran);
}

void fillWordBasket(struct AvailableLetter *dtHuruf){
    //mengisi array gudang huruf
    memcpy(dtHuruf->gudangHuruf, "AAAAAAAAABBCCDDDDEEEEEEEEEEEEFFGGGHHIIIIIIIIIJKLLLLMMNNNNNNOOOOOOOOPPQRRRRRRSSSSTTTTTTUUUUVVWWXYYZ##", 101);
}

void beforeEnteringGameStage(struct Player *dtPlayer, struct AvailableLetter *dtHuruf){
    //proses konfigurasi game sebelum benar-benar masuk kedalam arena permainan
    inputPlayerName(dtPlayer->plyName);
    dtPlayer->plyScore[0] = 0;
    dtPlayer->plyScore[1] = 0;

    struct Papan *dtPapan = (Papan*)malloc(sizeof(Papan));

    fillWordBasket(dtHuruf);
    dtHuruf->letterValue[0] = 1;
    dtHuruf->letterValue[1] = 3;
    dtHuruf->letterValue[2] = 3;
    dtHuruf->letterValue[3] = 2;
    dtHuruf->letterValue[4] = 1;
    dtHuruf->letterValue[5] = 4;
    dtHuruf->letterValue[6] = 2;
    dtHuruf->letterValue[7] = 4;
    dtHuruf->letterValue[8] = 1;
    dtHuruf->letterValue[9] = 8;
    dtHuruf->letterValue[10] = 5;
    dtHuruf->letterValue[11] = 1;
    dtHuruf->letterValue[12] = 3;
    dtHuruf->letterValue[13] = 1;
    dtHuruf->letterValue[14] = 1;
    dtHuruf->letterValue[15] = 3;
    dtHuruf->letterValue[16] = 10;
    dtHuruf->letterValue[17] = 1;
    dtHuruf->letterValue[18] = 1;
    dtHuruf->letterValue[19] = 1;
    dtHuruf->letterValue[20] = 1;
    dtHuruf->letterValue[21] = 4;
    dtHuruf->letterValue[22] = 4;
    dtHuruf->letterValue[23] = 8;
    dtHuruf->letterValue[24] = 4;
    dtHuruf->letterValue[25] = 10;
    dtHuruf->letterValue[26] = 0;

    memcpy(dtHuruf->commonLetter, "ABCDEFGHIJKLMNOPQRSTUVWXYZ#", 28);
    showLoadingBar();

    //MULAI BAGIAN MENAMPILKAN OUTPUT DI DALAM STAGE PERMAINAN SCRABBLE, MUNCUL SEKALI SAJA
    printPapan();
    fillSpecialField(dtPapan);
}

void duringPlayGame(struct Player *dtPlayer, struct AvailableLetter *dtHuruf, struct Papan *dtPapan){
    //mengatur jalannya permainan di dalam stage permainan, selama belum ada pemain yang memilih menu surrender
    int menuChosen, struggling = 0;
    bool surrender = false;

    dtPlayer->numGiliran = 1;       //sekali diinisiasi nilai 1 sebagai awalan saja

    do{
        fflush(stdin);
        gotoxy(58,1); printf("                ");
        menuChosen = 0; //nilai default pilihan menu untuk setiap giliran pemain

        initializeOutputForEachRole(dtPlayer,dtHuruf);  //menampilkan kerangka kotak-kotak dan teks output
        menuChosen = chosenMenu(menuChosen);

        if(menuChosen == 1){
            struggling = inputWordAsPerGiliran(dtPlayer,dtHuruf,dtPapan);  //masuk ke proses input kata
        }
        else if(menuChosen == 2){
            dtPlayer->numGiliran = roleExchange(dtPlayer->numGiliran);  //giliran diberikan pada lawannya
        }
        else if(menuChosen == 3){
            char button;
            do{
                fflush(stdin);
                shuffleLah(dtPlayer);  //proses shuffle huruf dijalankan sekali
                button = getche();     //untuk setiap kali pemain menekan tombol ESCAPE KEY sekali
            } while ((int)button == 27);  //pemain menekan tombol enter, maka shuffle terhenti
            struggling = inputWordAsPerGiliran(dtPlayer,dtHuruf,dtPapan);
        }
        else if (menuChosen == 5) {
            surrender = true;    //untuk yang surrender
            struggling = 0;
        }

    } while (surrender == false && ((struggling == 1) || (struggling == 0)));
}

void whoIsWinner(struct Player *dtPlayer){
    //tampilan setelah ada pemain yang memilih surrender
    //sebagai informasi siapa yang memenangkan permainan scrabble
    printf("\n\t");
    if(dtPlayer->plyScore[0] > dtPlayer->plyScore[1])
        printf("PLAYER %s WON THIS GAME!!!\n", dtPlayer->plyName[0]);
    if(dtPlayer->plyScore[0] < dtPlayer->plyScore[1])
        printf("PLAYER %s WON THIS GAME!!!\n", dtPlayer->plyName[1]);
    if(dtPlayer->plyScore[0] == dtPlayer->plyScore[1])
        printf("DRAW-DRAW-DRAW!!!\n");
    printf("\n");
    system("pause");
}

void afterPlayGameFinished(struct Player *dtPlayer){
    //setelah sesi permainan scrabble berakhir, akan keluar dari tampilan arena permainan scrabble
    system("cls");
    whoIsWinner(dtPlayer);
}

void gotQuitTime(char *wkt){
    //Tujuan : mendapatkan waktu permainan berakhir saat pemain memilih menu surrender
    //Sumber : https://linuxhint.com/gettimeofday_c_language/
    printf("\n\n");

    struct timeval tv;
    time_t t;
    struct tm *info;
    char buffer[64];

    gettimeofday(&tv, NULL);
    t = tv.tv_sec;

    info = localtime(&t);
    strftime(buffer, sizeof buffer, "%A, %d %b %Y, %H:%M:%S\n", info); //format : Hari, tanggal bulan tahun, Jam, menit, detik
    strcpy(wkt, buffer);

    //sputs(wkt);
}

void showHistory(){
    //tujuan : untuk menampilkan isi file history dari secondary storage ketika dipanggil di menu History
	int i;
	char ch;
	system("cls");
	system("mode 100,35");

	FILE *fh;
	fh=fopen ("history.csv","r");   //file csv dengan delimiter koma

	gotoxy(0,3);
	do
    {
        ch = fgetc(fh);
        if(ch != ','){      //agar koma tidak tampil di program saat modul ini dijalankan
            putchar(ch);
        }
        else
            printf("\t");   //bila karakter koma ditemui, maka gantilah dengan escape character tabulasi
    } while (ch != EOF);

	fclose(fh);
	getche();
}

void writeHistory(struct Player *dtPlayer, char wkt[]){
    //Tujuan : menuliskan data nama pemain, skor terakhir yang diperoleh, dan waktu berakhir permainan dalam satu kali sesi permainan scrabble
    //File terletak di secondary storage yang sejajar dengan letak file ini berada (masih dalam satu folder yang sama)
	int i,loc_Y;

	FILE *fh;
	fh=fopen ("history.csv","a");

	if(fh == NULL){
        //Berarti tidak dapat membuka file, sehingga proses pengisian histori permainan dilewat
        printf("\nUnable to open such files in this directory...");
        printf("Please check whether file exists in same directory of this program or not...\n");
        exit(EXIT_FAILURE);
	}

    fprintf(fh,"Nama Player,Skor,Waktu Bermain Selesai\n");  //header tabel setiap data yang dituliskan ke file
	for (i=0; i<2; i++){
        int skor = dtPlayer->plyScore[i];
        char nama[8];
        memset(nama,' ',8);
        strcpy(nama,dtPlayer->plyName[i]);
		fprintf(fh," %s,%d,%s\n", nama, skor,wkt);
		fflush(stdout);
	}

	fclose(fh);

	printf("\nSuccessfully recorded data to the file!\n");  //pesan jika proses penulisan ke file berhasil
	getche();
}

void fillSpecialField(struct Papan *dtPapan){
    //Tujuan : memberikan representasi visual di papan permainan scrabble, agar mengetahui di petak mana saja ada penilaian khusus
    //bila terdapat kata atau huruf yang menginjak petak dengan representasi khusus di bawah
    // # berarti skor kata dikali 3
    // @ berarti skor huruf dikali 2
    // + berarti skor huruf dikali 3
    // = berarti skor kata dikali 2
    char simbolDiPapan[15][15] = {"#  @   #   @  #",
                                  " +   =   =   + ",
                                  "  +   @ @   +  ",
                                  "   +   @   +   ",
                                  "    +     +    ",
                                  "  =  =   =  =  ",
                                  "   @  @ @  @   ",
                                  "#   @  $  @   #",
                                  "   @  @ @  @   ",
                                  "  =  =   =  =  ",
                                  "    +     +    ",
                                  "   +   @   +   ",
                                  "  +   @ @   +  ",
                                  " +   =   =   + ",
                                  "#  @   #   @  #",
                                 };

    //PROSES MENAMPILKAN ARRAY simbolDiPapan ke papan permainan scrabble agar tampil
    int X = 12, Y = 5;
    for(int i = 0; i < 15; i++){
        X = 12;
        for(int j = 0; j < 15; j++){
            if(simbolDiPapan[i][j] != ' '){
                gotoxy(X,Y);
                printf("%c", simbolDiPapan[i][j]);
            }
            X += 4;
        }
        Y += 2;
    }

}

