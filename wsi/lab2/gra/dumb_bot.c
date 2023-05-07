/****************************
Maciej Gębala (CC BY-NC 4.0)
Client ver. 0.1
2023-03-30
****************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <limits.h>
#include "./mersen/mtwister.h"
#include "board.h"


#define UPPER_MASK		0x80000000
#define LOWER_MASK		0x7fffffff
#define TEMPERING_MASK_B	0x9d2c5680
#define TEMPERING_MASK_C	0xefc60000

inline static void m_seedRand(MTRand* rand, unsigned long seed) {
  /* set initial seeds to mt[STATE_VECTOR_LENGTH] using the generator
   * from Line 25 of Table 1 in: Donald Knuth, "The Art of Computer
   * Programming," Vol. 2 (2nd Ed.) pp.102.
   */
  rand->mt[0] = seed & 0xffffffff;
  for(rand->index=1; rand->index<STATE_VECTOR_LENGTH; rand->index++) {
    rand->mt[rand->index] = (6069 * rand->mt[rand->index-1]) & 0xffffffff;
  }
}

/**
* Creates a new random number generator from a given seed.
*/
MTRand seedRand(unsigned long seed) {
  MTRand rand;
  m_seedRand(&rand, seed);
  return rand;
}

/**
 * Generates a pseudo-randomly generated long.
 */
unsigned long genRandLong(MTRand* rand) {

  unsigned long y;
  static unsigned long mag[2] = {0x0, 0x9908b0df}; /* mag[x] = x * 0x9908b0df for x = 0,1 */
  if(rand->index >= STATE_VECTOR_LENGTH || rand->index < 0) {
    /* generate STATE_VECTOR_LENGTH words at a time */
    int kk;
    if(rand->index >= STATE_VECTOR_LENGTH+1 || rand->index < 0) {
      m_seedRand(rand, 4357);
    }
    for(kk=0; kk<STATE_VECTOR_LENGTH-STATE_VECTOR_M; kk++) {
      y = (rand->mt[kk] & UPPER_MASK) | (rand->mt[kk+1] & LOWER_MASK);
      rand->mt[kk] = rand->mt[kk+STATE_VECTOR_M] ^ (y >> 1) ^ mag[y & 0x1];
    }
    for(; kk<STATE_VECTOR_LENGTH-1; kk++) {
      y = (rand->mt[kk] & UPPER_MASK) | (rand->mt[kk+1] & LOWER_MASK);
      rand->mt[kk] = rand->mt[kk+(STATE_VECTOR_M-STATE_VECTOR_LENGTH)] ^ (y >> 1) ^ mag[y & 0x1];
    }
    y = (rand->mt[STATE_VECTOR_LENGTH-1] & UPPER_MASK) | (rand->mt[0] & LOWER_MASK);
    rand->mt[STATE_VECTOR_LENGTH-1] = rand->mt[STATE_VECTOR_M-1] ^ (y >> 1) ^ mag[y & 0x1];
    rand->index = 0;
  }
  y = rand->mt[rand->index++];
  y ^= (y >> 11);
  y ^= (y << 7) & TEMPERING_MASK_B;
  y ^= (y << 15) & TEMPERING_MASK_C;
  y ^= (y >> 18);
  return y;
}

/**
 * Generates a pseudo-randomly generated double in the range [0..1].
 */
double genRand(MTRand* rand) {
  return((double)genRandLong(rand) / (unsigned long)0xffffffff);
}

int masks[28];
int heury[9][3]; //zliczam formacje
int heury_punkty[9][3]; //poprawiam na te istotne
int wagi[9] = {10,1,10,20,100,-1000000,20000000, 10, 1000};
int maxdepth = 0;
int final_move = 0;

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
        heury_punkty[8][1] = 1;
    }
    if(heury_punkty[4][2] > 1){
        heury_punkty[8][2] = 1;
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

int maska(int tab[4]){

    int code = 0;
    for (int i = 0; i < 4; i++){
        code = (code << 2) | (tab[i] & 0x0F);
    }
    return code;
}

int get_suma_heur(int gracz){
    int suma1 = 0;
    int suma2 = 0;
    int suma = 0;


    for(int i = 5; i < 7; i++){
        suma1 += heury_punkty[i][1];
    }

    for(int i = 5; i < 7; i++){
        suma2 += heury_punkty[i][2];
    }
    if(gracz == 1){
        suma = suma1 - suma2;
    }
    else{
        suma = suma2 - suma1;
    }
    return suma;
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

int evaluate(int gracz){
    setMasks();
    setHeury();
    setHeuryPunkty();
    readBoard();
    licz_heury();
    licz_heury_punkty();
    return get_suma_heur(gracz);

}
int min(int a, int b){
    if(a < b){
        return a;
    }
    else{
        return b;
    }
}
int max(int a, int b){
    if(a > b){
        return a;
    }
    else{
        return b;
    }
}

void minimax(int board[5][5], int depth, bool isMax, int gracz )
{
    MTRand r = seedRand(1337);
    double f;
    int score;
    int curr;
    //printf("current depth: %d\n", depth);

    // // If Maximizer has won the game return his/her
    // // evaluated score
    // if (score == 10)
    //     return score;
  
    // // If Minimizer has won the game return his/her
    // // evaluated score
    // if (score == -10)
    //     return score;
  
    // If there are no more moves and no winner then
    // it is a tie
    // if (isMovesLeft(board)==false)
    //     return 0;
  
    // If this maximizer's move
    if(depth == 0){
        return;
    }
    if (isMax)
    {
        int best = INT_MIN;
  
        // Traverse all cells
        for (int i = 0; i<5; i++)
        {
            for (int j = 0; j<5; j++)
            {
                // Check if cell is empty
                if (board[i][j]== 0)
                {
                    // Make the move
                    board[i][j] = gracz;
                    //score = evaluate(gracz);
                    // Call minimax recursively and choose
                    // the maximum value
                    score = evaluate(gracz); //wynik planszy
                    best = max( best, score );
                    //printf("current score: %d and depth: %d and maxdepth: %d and best is: %d\n", score, depth, maxdepth, best);

                    if(depth == maxdepth && score > best){
                        //printf("dup\n");
                        final_move = 10*(i+1) + j+1;
                    }
                    if(depth == maxdepth && score == best){
                        f = genRand(&r);
                        if(f > 0.51){
                            final_move = 10*(i+1) + j+1;
                        }
                    }

                    minimax(board, depth-1, !isMax, gracz);


                    // Undo the move
                    board[i][j] = 0;
                }
            }
        }
        //return best;
    }
  
    // If this minimizer's move
    else
    {
        int best = INT_MAX;
  
        // Traverse all cells
        for (int i = 0; i<3; i++)
        {
            for (int j = 0; j<3; j++)
            {
                // Check if cell is empty
                if (board[i][j]== 0)
                {
                    // Make the move
                    board[i][j] = 3 - gracz;
  
                    // Call minimax recursively and choose
                    // the minimum value
                    score = evaluate(gracz);
                    best = min(best, score);
                    
                    minimax(board, depth-1, !isMax, gracz);
  
                    // Undo the move
                    board[i][j] = 0;
                }
            }
        }
        //return best;
    }
    //return 0;
}


int main(int argc, char *argv[])
{
  int socket_desc;
  struct sockaddr_in server_addr;
  char server_message[16], client_message[16];

  bool end_game;
  int player, msg, move;

  if( argc!=5 ) {
    printf("Wrong number of arguments\n");
    return -1;
  }

  // Create socket
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  if( socket_desc<0 ) {
    printf("Unable to create socket\n");
    return -1;
  }
  printf("Socket created successfully\n");

  // Set port and IP the same as server-side
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(atoi(argv[2]));
  server_addr.sin_addr.s_addr = inet_addr(argv[1]);
  maxdepth = atoi(argv[4]); 
  // Send connection request to server
  if( connect(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr))<0 ) {
    printf("Unable to connect\n");
    return -1;
  }
  printf("Connected with server successfully\n");

  // Receive the server message
  memset(server_message, '\0', sizeof(server_message));
  if( recv(socket_desc, server_message, sizeof(server_message), 0)<0 ) {
    printf("Error while receiving server's message\n");
    return -1;
  }
  printf("Server message: %s\n",server_message);

  memset(client_message, '\0', sizeof(client_message));
  strcpy(client_message, argv[3]);
  // Send the message to server
  if( send(socket_desc, client_message, strlen(client_message), 0)<0 ) {
    printf("Unable to send message\n");
    return -1;
  }

  setBoard(); 
  end_game = false;
  player = atoi(argv[3]);

  while( !end_game ) {
    memset(server_message, '\0', sizeof(server_message));
    if( recv(socket_desc, server_message, sizeof(server_message), 0)<0 ) {
      printf("Error while receiving server's message\n");
      return -1;
    }
    printf("Server message: %s\n", server_message);
    msg = atoi(server_message);
    move = msg%100;
    msg = msg/100;
    if( move!=0 ) {
      setMove(move, 3-player);
      printBoard();
    }
    if( (msg==0) || (msg==6) ) {
      printf("Your move: ");
    //   if( fgets(client_message, sizeof(client_message), stdin)==NULL ) {
    //     printf("Error while reading move\n");
    //     return -1;
    //   };
      minimax(board, maxdepth, true, player);
      move = final_move;
      setMove(move, player);
      printBoard();
      memset(client_message, '\0', sizeof(client_message));
      sprintf(client_message, "%d", move);
      if( send(socket_desc, client_message, strlen(client_message), 0)<0 ) {
        printf("Unable to send message\n");
        return -1;
      }
      printf("Client message: %s\n", client_message);
     }
     else {
       end_game = true;
       switch( msg ) {
         case 1 : printf("You won.\n"); break;
         case 2 : printf("You lost.\n"); break;
         case 3 : printf("Draw.\n"); break;
         case 4 : printf("You won. Opponent error.\n"); break;
         case 5 : printf("You lost. Your error.\n"); break;
       }
     }
   }

  // Close socket
  close(socket_desc);

  return 0;
}
