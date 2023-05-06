#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

int board[5][5];
int masks[28];

// int przeplatanki[3]; //h0 
// int układ_4_z_1_[3]; //h1
// int dwa_z_dziurą[3]; //h2
// int dwa_z_follow[3]; //h3
// int trz_z_dziurą[3]; //h4
// int samobójstwoo[3]; //h5
// int zwycięnstwoo[3]; //h6
// l. pól bez samobója; //h7
// bonus za więcej form //h8

int heury[9][3]; //zliczam formacje
int heury_punkty[9][3]; //poprawiam na te istotne
int wagi[9] = {10,1,10,20,100,-1000000,20000000, 10, 1000};

int count_legal_moves(int player) {
    int legal_moves = 0;

    // Sprawdzenie poziomych linii
    for (int i = 0; i < 5; i++) {
        int player_count = 0;
        int empty_count = 0;
        for (int j = 0; j < 5; j++) {
            if (board[i][j] == player) {
                player_count++;
            } else if (board[i][j] == 0) {
                empty_count++;
            } else {
                player_count = 0;
                empty_count = 0;
            }

            if (player_count == 3 && empty_count == 1) {
                legal_moves++;
                break;
            }
        }
    }

    // Sprawdzenie pionowych linii
    for (int j = 0; j < 5; j++) {
        int player_count = 0;
        int empty_count = 0;
        for (int i = 0; i < 5; i++) {
            if (board[i][j] == player) {
                player_count++;
            } else if (board[i][j] == 0) {
                empty_count++;
            } else {
                player_count = 0;
                empty_count = 0;
            }

            if (player_count == 3 && empty_count == 1) {
                legal_moves++;
                break;
            }
        }
    }

    // Sprawdzenie przekątnych
    int player_count = 0;
    int empty_count = 0;
    for (int i = 0; i < 5; i++) {
        int j = i;
        if (board[i][j] == player) {
            player_count++;
        } else if (board[i][j] == 0) {
            empty_count++;
        } else {
            player_count = 0;
            empty_count = 0;
        }

        if (player_count == 3 && empty_count == 1) {
            legal_moves++;
            break;
        }
    }

    player_count = 0;
    empty_count = 0;
    for (int i = 0; i < 5; i++) {
        int j = 4 - i;
        if (board[i][j] == player) {
            player_count++;
        } else if (board[i][j] == 0) {
            empty_count++;
        } else {
            player_count = 0;
            empty_count = 0;
        }

        if (player_count == 3 && empty_count == 1) {
            legal_moves++;
            break;
        }
    }

    return legal_moves;
}

int countSafeMoves(int player) {
    int count = 0;
    int samoboj = 0;
    int opponent = 3 - player; // określenie numeru przeciwnika

    for(int posi = 0; posi < 5; posi++){
        for(int posj = 0; posj < 5; posj++){
            samoboj = 0;
            if(board[posi][posj] == 0){

            

            if(posi % 5 == 0){
                if((board[posi+1][posj] == player) && (board[posi+2][posj] == player )){
                    samoboj++;
                }
            }
            if(posi % 5 == 1){
                if(((board[posi+1][posj] == player) && (board[posi+2][posj] == player )) || ((board[posi+1][posj] == player) && (board[posi-1][posj] == player ))){
                    samoboj++;
                }
            }
            if(posi % 5 == 2){
                if(((board[posi+1][posj] == player) && (board[posi+2][posj] == player )) || ((board[posi+1][posj] == player) && (board[posi-1][posj] == player )) || ((board[posi-2][posj] == player) && (board[posi-1][posj] == player ))){
                    samoboj++;
                }
            }
            if(posi % 5 == 3){
                if(((board[posi-1][posj] == player) && (board[posi-2][posj] == player )) || ((board[posi+1][posj] == player) && (board[posi-1][posj] == player ))){
                    samoboj++;
                }
            }
            if(posi % 5 == 4){
                if((board[posi-1][posj] == player) && (board[posi-2][posj] == player )){
                    samoboj++;
                }
            }
        /////////////////////////////////////
            if(posj % 5 == 0){
                if((board[posi][posj+1] == player) && (board[posi][posj+2] == player )){
                    samoboj++;
                }
            }
            if(posj % 5 == 1){
                if(((board[posi][posj+1] == player) && (board[posi][posj+2] == player )) || ((board[posi][posj+1] == player) && (board[posi][posj-1] == player ))){
                    samoboj++;
                }
            }
            if(posj % 5 == 2){
                if(((board[posi][posj+1] == player) && (board[posi][posj+2] == player )) || ((board[posi][posj+1] == player) && (board[posi][posj-1] == player )) || ((board[posi][posj-2] == player) && (board[posi][posj-1] == player ))){
                    samoboj++;
                }
            }
            if(posj % 5 == 3){
                if(((board[posi][posj-1] == player) && (board[posi][posj-2] == player )) || ((board[posi][posj+1] == player) && (board[posi][posj-1] == player ))){
                    samoboj++;
                }
            }
            if(posj % 5 == 4){
                if((board[posi][posj-1] == player) && (board[posi][posj-2] == player )){
                    samoboj++;
                }
            }
            if((posi == 1 && posj == 1)){
                if((board[posi-1][posj-1] == player) && (board[posi+1][posj+1] == player )){
                    samoboj++;
                }
                if((board[posi+1][posj-1] == player) && (board[posi-1][posj+1] == player )){
                    samoboj++;
                }
                if((board[posi+1][posj+1] == player) && (board[posi+2][posj+2] == player )){
                    samoboj++;
                }                
            }
            if((posi == 1 && posj == 2)){
                if((board[posi-1][posj-1] == player) && (board[posi+1][posj+1] == player )){
                    samoboj++;
                }
                if((board[posi+1][posj-1] == player) && (board[posi-1][posj+1] == player )){
                    samoboj++;
                }
                if((board[posi+1][posj+1] == player) && (board[posi+2][posj+2] == player )){
                    samoboj++;
                }
                if((board[posi+1][posj-1] == player) && (board[posi+2][posj-2] == player )){
                    samoboj++;
                }
            }
            if((posi == 1 && posj == 3)){
                if((board[posi-1][posj-1] == player) && (board[posi+1][posj+1] == player )){
                    samoboj++;
                }
                if((board[posi+1][posj-1] == player) && (board[posi-1][posj+1] == player )){
                    samoboj++;
                }
                if((board[posi+1][posj-1] == player) && (board[posi+2][posj-2] == player )){
                    samoboj++;
                }

            }
            if((posi == 2 && posj == 1)){
                if((board[posi-1][posj-1] == player) && (board[posi+1][posj+1] == player )){
                    samoboj++;
                }
                if((board[posi+1][posj-1] == player) && (board[posi-1][posj+1] == player )){
                    samoboj++;
                }
                if((board[posi+1][posj+1] == player) && (board[posi+2][posj+2] == player )){
                    samoboj++;
                }
                if((board[posi-1][posj+1] == player) && (board[posi-2][posj+2] == player )){
                    samoboj++;
                }
            }
            if((posi == 2 && posj == 2)){
                if((board[posi-1][posj-1] == player) && (board[posi+1][posj+1] == player )){
                    samoboj++; //ukos
                }
                if((board[posi+1][posj-1] == player) && (board[posi-1][posj+1] == player )){
                    samoboj++; //ukos
                }
                if((board[posi+1][posj+1] == player) && (board[posi+2][posj+2] == player )){
                    samoboj++; //prawy dół
                }
                if((board[posi-1][posj+1] == player) && (board[posi-2][posj+2] == player )){
                    samoboj++; //prawy góra
                }
                if((board[posi+1][posj-1] == player) && (board[posi+2][posj-2] == player )){
                    samoboj++; //lewy dół
                }
                if((board[posi-1][posj-1] == player) && (board[posi-2][posj-2] == player )){
                    samoboj++; //lewy góra
                }                
            }
            if((posi == 2 && posj == 3)){
                if((board[posi-1][posj-1] == player) && (board[posi+1][posj+1] == player )){
                    samoboj++; //ukos
                }
                if((board[posi+1][posj-1] == player) && (board[posi-1][posj+1] == player )){
                    samoboj++; //ukos
                }
                if((board[posi+1][posj-1] == player) && (board[posi+2][posj-2] == player )){
                    samoboj++; //lewy dół
                }
                if((board[posi-1][posj-1] == player) && (board[posi-2][posj-2] == player )){
                    samoboj++; //lewy góra
                }    
            }
            if((posi == 3 && posj == 1)){
                if((board[posi-1][posj-1] == player) && (board[posi+1][posj+1] == player )){
                    samoboj++; //ukos
                }
                if((board[posi+1][posj-1] == player) && (board[posi-1][posj+1] == player )){
                    samoboj++; //ukos
                }
                if((board[posi+1][posj+1] == player) && (board[posi+2][posj+2] == player )){
                    samoboj++; //prawy dół
                }
                if((board[posi-1][posj+1] == player) && (board[posi-2][posj+2] == player )){
                    samoboj++; //prawy góra
                }   
            }
            if((posi == 3 && posj == 2)){
                if((board[posi-1][posj-1] == player) && (board[posi+1][posj+1] == player )){
                    samoboj++; //ukos
                }
                if((board[posi+1][posj-1] == player) && (board[posi-1][posj+1] == player )){
                    samoboj++; //ukos
                }
                if((board[posi-1][posj+1] == player) && (board[posi-2][posj+2] == player )){
                    samoboj++; //prawy góra
                }
                if((board[posi-1][posj-1] == player) && (board[posi-2][posj-2] == player )){
                    samoboj++; //lewy góra
                }    
            }
            if((posi == 3 && posj == 3)){
                if((board[posi-1][posj-1] == player) && (board[posi+1][posj+1] == player )){
                    samoboj++; //ukos
                }
                if((board[posi+1][posj-1] == player) && (board[posi-1][posj+1] == player )){
                    samoboj++; //ukos
                }
                if((board[posi-1][posj-1] == player) && (board[posi-2][posj-2] == player )){
                    samoboj++; //lewy góra
                }    
            }

            if (samoboj == 0){
                count++;
            }
            }

        }
    }        


    return count;
}


void licz_heury(){

    for(int i = 0; i<28; i++){  

        switch(masks[i]){
            // przeplatanka
            case 66:
                heury[0][1]++;
                break;
            case 17:
                heury[0][1]++;
                break;  
            case 136:
                heury[0][2]++;
                break;
            case 34:
                heury[0][2]++;
                break;             
            // 4 z 1
            case 64:
                heury[1][1]++;
                break;
            case 16:
                heury[1][1]++;
                break;  
            case 4:
                heury[1][1]++;
                break;
            case 1:
                heury[1][1]++; 
                break;
            case 128:
                heury[1][2]++;
                break;
            case 32:
                heury[1][2]++;
                break;
            case 8:
                heury[1][2]++;
                break;
            case 2:
                heury[1][2]++; 
                break; 
            //   dwójka z dziurą
            case 65:
                heury[2][1]++; 
                break;
            case 130:
                heury[2][2]++;
                break;    
            // dwa z follow        
            case 80:
                heury[3][1]++;
                break;
            case 5:
                heury[3][1]++; 
                break;
            case 160:
                heury[3][2]++;
                break;
            case 10:
                heury[3][2]++;
                break; 
            //rozerwana trójka
            case 81:
                heury[4][1]++;
                break;
            case 69:
                heury[4][1]++; 
                break;
            case 162:
                heury[4][2]++;
                break;
            case 138:
                heury[4][2]++;
                break;                               
            //samobój
            case 149:
                heury[5][1]++;
                break;
            case 86:
                heury[5][1]++;
                break;  
            case 84:
                heury[5][1]++;
                break;
            case 21:
                heury[5][1]++; 
                break;
            case 168:
                heury[5][2]++;
                break;
            case 169:
                heury[5][2]++;
                break;
            case 42:
                heury[5][2]++;
                break;
            case 106:
                heury[5][2]++; 
                break; 
            //win
            case 85:
                heury[6][1]++;
                break;
            case 170:
                heury[6][2]++;
                break;        
            default:
                break;    
        }
    }    


}





void setMasks(){
    for(int i = 0; i < 28; i++){
        masks[i] = 0;
    }
}

void printMasks(){
    for(int i = 0; i < 28; i++){
        printf("maska nr %d: %d\n", i+1, masks[i]);
    }
}

void setBoard()
{
  for(int i=0; i<5; i++)
    for(int j=0; j<5; j++)
      board[i][j]=0;
}

void setHeury()
{
  for(int i=0; i<9; i++)
    for(int j=0; j<3; j++)
      heury[i][j]=0;
}

void setHeuryPunkty()
{
  for(int i=0; i<9; i++)
    for(int j=0; j<3; j++)
      heury_punkty[i][j]=0;
}

void por(int dupa, int numer){

    if(numer == 1){

        if(dupa == 5 || dupa == 80){
            heury_punkty[3][1]--;
        }
        if(dupa == 17 || dupa == 66){
            heury_punkty[0][1]--;
        }
        if(dupa == 1 || dupa == 4 || dupa == 16 || dupa == 64){
            heury_punkty[1][1]--;
        }
        if(dupa == 69 || dupa == 81){
            heury_punkty[4][1]--;
        }   
        if(dupa == 65){
            heury_punkty[2][1]--;
        }     
    }
    else{
        if(dupa == 10 || dupa == 160){
            heury_punkty[3][2]--;
        }
        if(dupa == 34 || dupa == 132){
            heury_punkty[0][2]--;
        }
        if(dupa == 2 || dupa == 8 || dupa == 32 || dupa == 128){
            heury_punkty[1][2]--;
        }
        if(dupa == 138 || dupa == 162){
            heury_punkty[4][2]--;
        }   
        if(dupa == 130){
            heury_punkty[2][2]--;
        }

    }
    
}

void licz_heury_punkty()
{
 
    for(int i = 0; i<5;i++){
        if( ( masks[2*i] == 65 && masks[(2*i)+1] == 5) || ( masks[2*i] == 80 && masks[(2*i)+1] == 65) ){
            heury_punkty[2][1]--;
        }
        if( ( masks[2*i] == 130 && masks[(2*i)+1] == 10) || ( masks[2*i] == 160 && masks[(2*i)+1] == 130) ){
            heury_punkty[2][2]--;
        }
    }
    if( ( masks[21] == 65 && masks[22] == 5) || ( masks[21] == 80 && masks[22] == 65) ){
        heury_punkty[2][1]--;
    }
    if( ( masks[21] == 130 && masks[22] == 10) || ( masks[21] == 160 && masks[22] == 130) ){
        heury_punkty[2][2]--;
    }
    if( ( masks[25] == 65 && masks[26] == 5) || ( masks[25] == 80 && masks[26] == 65) ){
        heury_punkty[2][1]--;
    }
    if( ( masks[25] == 130 && masks[26] == 10) || ( masks[25] == 160 && masks[26] == 130) ){
        heury_punkty[2][2]--;
    }
    if(masks[21] == 81 && masks[22] == 69){
        heury_punkty[4][1]--;
    }
    if(masks[25] == 81 && masks[26] == 69){
        heury_punkty[4][1]--;
    }///////////
    if(masks[6] == 81 && masks[27] == 81){
        heury_punkty[4][1]--;
    }
    if(masks[4] == 81 && (masks[25] == 81 || masks[26] == 69)){
        heury_punkty[4][1]--;
    }
    if(masks[2] == 81 && masks[24] == 69){
        heury_punkty[4][1]--;
    }
    ///////
    if(masks[21] == 162 && masks[22] == 138){
        heury_punkty[4][2]--;
    }
    if(masks[25] == 162 && masks[26] == 138){
        heury_punkty[4][2]--;
    }
    if(masks[0] == 69 || masks[2] == 69 || masks[4] == 69 || masks[6] == 69){
        por(masks[12], 1);
        por(masks[13], 1);

    }  
    if(masks[0] == 81 || masks[2] == 81 || masks[4] == 81 || masks[6] == 81){
        por(masks[14], 1);
        por(masks[15], 1);

    }  
    if(masks[1] == 81 || masks[3] == 81 || masks[5] == 81 || masks[7] == 81){
        por(masks[16], 1);
        por(masks[17], 1);

    }
    if(masks[1] == 69 || masks[3] == 69 || masks[5] == 69 || masks[7] == 69){
        por(masks[14], 1);
        por(masks[15], 1);

    }
    if(masks[8] == 69 && masks[13] == 80){
        heury_punkty[3][1]--;
    }
    if(masks[8] == 81 && masks[15] == 80){
        heury_punkty[3][1]--;
    }
    if(masks[9] == 81 && masks[17] == 80){
        heury_punkty[3][1]--;
    }
    if(masks[10] == 69 && masks[2] == 5){
        heury_punkty[3][1]--;
    }
    if(masks[10] == 81 && masks[4] == 5){
        heury_punkty[3][1]--;
    }
    if(masks[11] == 81 && masks[6] == 5){
        heury_punkty[3][1]--;
    }
    if(masks[18] == 69 && masks[3] == 80){
        heury_punkty[3][1]--;
    }
    if(masks[18] == 81 && masks[5] == 80){
        heury_punkty[3][1]--;
    }
    if(masks[19] == 81 && masks[7] == 80){
        heury_punkty[3][1]--;
    }
    

    if(masks[0] == 138 || masks[2] == 138 || masks[4] == 138 || masks[6] == 138){
        por(masks[12], 2);
    }  
    if(masks[0] == 162 || masks[2] == 162 || masks[4] == 162 || masks[6] == 162){
        por(masks[14], 2);
    }  
    if(masks[1] == 162 || masks[3] == 162 || masks[5] == 162 || masks[7] == 162){
        por(masks[16], 2);
    }

    if(masks[8] == 69*2 && masks[13] == 80*2){
        heury_punkty[3][2]--;
    }
    if(masks[8] == 81*2 && masks[15] == 80*2){
        heury_punkty[3][2]--;
    }
    if(masks[9] == 81*2 && masks[17] == 80*2){
        heury_punkty[3][2]--;
    }
    if(masks[10] == 69*2 && masks[2] == 5*2){
        heury_punkty[3][2]--;
    }
    if(masks[10] == 81*2 && masks[4] == 5*2){
        heury_punkty[3][2]--;
    }
    if(masks[11] == 81*2 && masks[6] == 5*2){
        heury_punkty[3][2]--;
    }
    if(masks[18] == 69*2 && masks[3] == 80*2){
        heury_punkty[3][2]--;
    }
    if(masks[18] == 81*2 && masks[5] == 80*2){
        heury_punkty[3][2]--;
    }
    if(masks[19] == 81*2 && masks[7] == 80*2){
        heury_punkty[3][2]--;
    }

    if(heury_punkty[4][1] > 1){
        heury_punkty[8][1]++;
    }
    if(heury_punkty[4][2] > 1){
        heury_punkty[8][2]++;
    }

    heury_punkty[7][1] = countSafeMoves(1);
    heury_punkty[7][2] = countSafeMoves(2);



    for(int i=0; i<9; i++)
    {
        for(int j=1; j<3; j++)
        {
            heury_punkty[i][j] += heury[i][j];
            heury_punkty[i][j] = heury_punkty[i][j] * wagi[i];
        }
    } 



}


void printHeury(){
    for(int i = 0; i < 7; i++){
        printf("Lizba figury nr: %d\n",i);
        for(int j = 1; j < 3; j++){
            printf("Dla gracza nr %d == %d\n",j,heury[i][j]);
        }
    }
}

void printHeuryPunkty(){
    for(int i = 0; i < 9; i++){
        printf("Lizba punktów z heury nr: %d\n",i);
        for(int j = 1; j < 3; j++){
            printf("Dla gracza nr %d == %d\n",j,heury_punkty[i][j]);
        }
    }
}

void printBoard()
{
  printf("  1 2 3 4 5\n");
  for(int i=0; i<5; i++) {
    printf("%d",i+1);
    for(int j=0; j<5; j++ )
      switch(board[i][j]) {
        case 0: printf(" -"); break;
        case 1: printf(" X"); break;
        case 2: printf(" O"); break;
      }
    printf("\n");
  }
  printf("\n");
}

bool setMove(int move, int player)
{
  int i,j;
  i = (move/10)-1;
  j = (move%10)-1;
  if( (i<0) || (i>4) || (j<0) || (j>4) ) return false; 
  if( board[i][j]!=0 ) return false;
  board[i][j] = player;
  return true;
}

int maska(int tab[4]){

    int code = 0;
    for (int i = 0; i < 4; i++){
        code = (code << 2) | (tab[i] & 0x0F);
    }
    return code;
}


void readBoard(){
    int counter = 0;
    int tab[4];
    int temp;

    for(int i = 0; i < 5; i++){ //poziome maski
        for(int j = 0; j < 4; j++){
            tab[j] = board[i][j];
        }
        masks[counter] = maska(tab);
        temp = masks[counter];
        counter++;
        for(int j = 0; j < 4; j++){
            tab[j] = board[i][j+1];
        }
        masks[counter] = maska(tab);

        counter++;
    }

    for(int i = 0; i < 5; i++){ //pionowe maski
        for(int j = 0; j < 4; j++){
            tab[j] = board[j][i];
        }
        masks[counter] = maska(tab);
        counter++;
        for(int j = 0; j < 4; j++){
            tab[j] = board[j+1][i];
        }
        masks[counter] = maska(tab);
        counter++;
    }

    for(int i = 0; i<4; i++){ //poniżej lewej przekątnej #23
        tab[i] = board[i+1][i];
    }
    masks[counter] = maska(tab);
    counter++;

    for(int i = 0; i<4; i++){ //lewa przekątna górna
        tab[i] = board[i][i];
    }
    masks[counter] = maska(tab);
    counter++;

    for(int i = 0; i<4; i++){ //lewa przekątna dolna #22
        tab[i] = board[i+1][i+1];
    }
    masks[counter] = maska(tab);
    counter++;

    for(int i = 0; i<4; i++){ //powyżej lewej przekątnej #24
        tab[i] = board[i][i+1];
    }
    masks[counter] = maska(tab);
    counter++;

    for(int i = 0; i<4; i++){ //powyżej prawej przekątnej #27
        tab[i] = board[i][4-i-1];
    }
    masks[counter] = maska(tab);
    counter++;

    for(int i = 0; i<4; i++){ //prawa przekątna górna #25
        tab[i] = board[i][4-i];
    }
    masks[counter] = maska(tab);
    counter++;

    for(int i = 0; i<4; i++){ //prawa przekątna dolna #26
        tab[i] = board[i+1][4-i-1];
    }
    masks[counter] = maska(tab);
    counter++;

    for(int i = 0; i<4; i++){ //poniżej prawej przekątnej #28
        tab[i] = board[i+1][4-i];
    }
    masks[counter] = maska(tab);
    counter++;

}



int main(int argc, char* argv[argc+1]){

    int numer = argv[1][0]; 
    printf("numer gracza: %c: \n", numer);

    char krzyżyk = 1;
    char kólko = 2;

    setBoard();
    setMasks();
    setHeury();
    setHeuryPunkty();
    printBoard();
    //printMasks();


    setMove(43, 1);
    setMove(42, 1);
    setMove(45, 1);
    setMove(34, 1);
    setMove(54, 1);
    setMove(24, 1);


    readBoard();
    printBoard();

    licz_heury();
    licz_heury_punkty();

    //printHeury();
    printHeuryPunkty();

    //printMasks();



    return 0;
}
