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
#include <sys/time.h>
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


/**
 * @brief funkcje heurystyczne
 * 0) przeplot
 * 1) 1z4
 * 2) 
 * 
 */
int masks[28];
int heury[8][3]; //zliczam formacje
int heury_punkty[8][3]; //poprawiam na te istotne
//wagi[znak][etap][funkcje]
int wagi[552][3][3][10];
// = {{{10,1,20,10,100,-300000,2000000, 20, 1000, 1000}, {10,1,20,10,100,-300000,2000000, 20, 1000, 1000}, {10,1,20,10,100,-300000,2000000, 20, 1000, 1000}},
// {{10,1,20,10,100,-300000,2000000, 20, 1000, 1000}, {10,1,20,10,100,-300000,2000000, 20, 1000, 1000}, {10,1,20,10,100,-300000,2000000, 20, 1000, 1000}}, //krzyż
//   {{10,1,20,10,100,-300000,2000000, 20, 1000, 1000}, {10,1,20,10,100,-300000,2000000, 20, 1000, 1000}, {10,1,20,10,100,-300000,2000000, 20, 1000, 1000}}}; //kółko
double winratio[552];
int maxdepth = 0;
int final_move = 0;
int count_safe;
int samoboj;
int dziura;
int opponent;
int moveno;



void ultBlock(){
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){

            if(board[i][j] == 0){

                board[i][j] = 1;
                if(winCheck(1)){
                    board[i][j] = 2;
                    if(loseCheck(2)){
                        heury_punkty[7][1]++;
                        //printf("this block is on pos: %d", 10*(i+1)+1+1);
                    }
                }
                board[i][j] = 2;
                if(winCheck(2)){
                    board[i][j] = 1;
                    if(loseCheck(1)){
                        heury_punkty[7][2]++;
                        //printf("this block is on pos: %d", 10*(i+1)+1+1);
                    }
                }  
                board[i][j] = 0;
            }
        }
    }
}

int countSafeMoves(int player){

    count_safe = 0;
    samoboj = 0;
    opponent = 3 - player; // określenie numeru przeciwnika

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
        ////    /////////////////////////////////
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
                    count_safe++;
                }
            }

        }
    }        
    return count_safe;
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
  for(int i=0; i<8; i++)
    for(int j=0; j<3; j++)
      heury[i][j]=0;
}

void setHeuryPunkty()
{
  for(int i=0; i<8; i++)
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

void licz_heury_punkty(int moveNo, int bot)
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
        heury_punkty[6][1] = 1;
    }
    if(heury_punkty[4][2] > 1){
        heury_punkty[6][2] = 1;
    }

    heury_punkty[5][1] = countSafeMoves(1);
    heury_punkty[5][2] = countSafeMoves(2);

    if(moveNo > 0 && moveNo < 6){
    for(int i=0; i<8; i++)
    {
        for(int j=1; j<3; j++)
        {
            heury_punkty[i][j] += heury[i][j];
            heury_punkty[i][j] = heury_punkty[i][j] * wagi[bot][j][0][i]; //bot, znak,etap,funkcje
        }
    }
    }
    if(moveNo >= 6 && moveNo <= 15){

    for(int i=0; i<8; i++)
    {
        for(int j=1; j<3; j++)
        {
            heury_punkty[i][j] += heury[i][j];
            heury_punkty[i][j] = heury_punkty[i][j] * wagi[bot][j][1][i]; //bot, znak,etap,funkcje
        }
    } 
    }
    if(moveNo > 15){

    for(int i=0; i<8; i++)
    {
        for(int j=1; j<3; j++)
        {
            heury_punkty[i][j] += heury[i][j];
            heury_punkty[i][j] = heury_punkty[i][j] * wagi[bot][j][2][i]; //bot, znak,etap,funkcje
        }
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
    for(int i = 0; i < 10; i++){
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


    for(int i = 0; i < 8; i++){
        suma1 += heury_punkty[i][1];
    }

    for(int i = 0; i < 8; i++){
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

int tab[4];
int temp;
int counter;
void readBoard(){
    counter = 0;
    
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

int evaluate(int gracz, int moveNo, int bot){
    setMasks();
    setHeury();
    setHeuryPunkty();
    readBoard();
    //ultBlock();
    licz_heury();
    licz_heury_punkty(moveNo, bot);
    // if(winCheck(gracz)){
    //     return INT_MAX;
    // }
    // if(winCheck(3-gracz)){
    //     return INT_MIN;
    // }
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

int count_draw;
bool draw(){
    count_draw = 0;
    for(int i = 0; i < 5; i++){
        for(int j = 0; j< 5; j++){
            if (board[i][j] != 0){
                count_draw++;
            }
        }
    }
    if(count_draw == 25){
        return true;
    }
    else{
        return false;
    }
}

int score;
int best;
int kontratak;
int minimax(int depth, bool isMax, int player, int alpha, int beta, int moveNo, int bot)
{
    
    if(winCheck(player) ||  (loseCheck(3-player) && !winCheck(3-player))){
        //printf("ucinam symulację, wygrałem. Głębokość: %d. Wynik: %d\n", depth, evaluate(player));
        //printBoard();
        return INT_MAX;
    }
    if((loseCheck(player) && !winCheck(player)) || winCheck(3-player)){
        //printf("ucinam symulację, przegrałem. Głębokość: %d. Wynik: %d\n", depth, evaluate(player));
        //printBoard();
        return INT_MIN;
    }
    if(draw()){
        //printf("ucinam symulację, remis\n");
        return 0;
    } 

    if(depth == 0){
        return evaluate(player, moveNo, bot);
    }   
    
    // If this maximizer's move
    if (isMax)
    {       

        best = INT_MIN;
  
        // Traverse all cells
        for (int i = 0; i<5; i++)
        {
            for (int j = 0; j<5; j++)
            {
                // Check if cell is empty
                if (board[i][j] == 0)
                {
                    // Make the move
                    board[i][j] = player;
                    
                    best = max(best, minimax(depth-1, false, player, alpha, beta, moveNo+1, bot) );
     
                    board[i][j] = 0;
                    alpha = max(alpha, best);
                    if(beta <= alpha){
                        i = 10;
                        j = 10;
                    }
                }
            }
        }
        return best;
    }
  
    // If this minimizer's move
    else
    {
        best = INT_MAX;
  
        // Traverse all cells
        for (int i = 0; i<5; i++)
        {
            for (int j = 0; j<5; j++)
            {
                // Check if cell is empty
                if (board[i][j] == 0)
                {
                    // Make the move
                    board[i][j] = 3 - player;

                    if(winCheck(3-player) && depth == maxdepth-1){
                        kontratak = 10*(i+1) + j + 1;
                    }
                    else{
                        kontratak = -1;
                    }
             

                    best = min(best, minimax(depth-1, true, player, alpha, beta, moveNo, bot) );
             
                    board[i][j] = 0;
                    beta = min(beta, best);
                    if(beta <= alpha){
                        i = 10;
                        j = 10;
                    }
                }
            }
        }
        return best;
    }
}
  
int bestVal;
int bestMove;
int moveVal;
float f;
bool wincheck;
bool kontra;
int counterstrike;
int findBestMove(int player, int depth, MTRand* r, int moveNo, int bot)
{
    bestVal = INT_MIN;
    bestMove = 0;
    wincheck = false;
    kontra = false;
    for (int i = 0; i<5; i++)
    {
        for (int j = 0; j<5; j++)
        {
            // Check if cell is empty
            if (board[i][j]==0)
            {
                // Make the move
                board[i][j] = player;

                // compute evaluation function for this
                // move.
                //kontratak = 0;
                moveVal = minimax(depth-1, false, player, INT_MIN, INT_MAX, moveNo, bot);
                if(winCheck(player)){
                    wincheck = true;
                    board[i][j] = 0;
                    return 10*(i+1) + j + 1;
                }
                if(kontratak > 0 && board[(kontratak/10)-1][(kontratak%10)-1] == 0){
                    //board[i][j] = 0;
                    kontra = true;
                    counterstrike = kontratak;
                    //bestMove = 10*(i+1) + j + 1;
                    //printf("kontra kurwo: pos = %d\n ", kontratak);
                    //return kontratak;
                }
                // if(moveVal == INT_MAX){
                //     printf("move pos: %d", (10*(i+1) + j + 1));
                // }
                // Undo the move
                board[i][j] = 0;

                if (moveVal > bestVal)
                {
                    bestMove = 10*(i+1) + j + 1;
                    bestVal = moveVal;
                }
                if(moveVal == bestVal){
                    //printf("będę losował\n");
                    f = genRand(r);
                    if(f > 0.49){
                        bestVal = moveVal;
                        bestMove = 10*(i+1) + j + 1;
                    }
                }
            }
        }
    }

    //printf("The value of the best Move is : %d\n\n",bestVal);
    if(kontra && !wincheck){
        //printf("wykonuję kontratak: %d\n", counterstrike);
        return counterstrike;
    }
    else{
        return bestMove;
    }
}

int firstMoveX(int player, int depth, MTRand* r, int bot)
{
    bestVal = INT_MIN;
    bestMove = 0;
    
        for (int i = 0; i<3; i++)
        {
            for (int j = 0; j<=i; j++)
            {
                // Check if cell is empty
                if (board[i][j]==0)
                {
                    // Make the move
                    board[i][j] = player;
    
                    // compute evaluation function for this
                    // move.
                    moveVal = minimax(depth-1, false, player, INT_MIN, INT_MAX, 1, bot);
                    board[i][j] = 0;
    
                    if (moveVal > bestVal)
                    {
                        bestMove = 10*(i+1) + j + 1;
                        bestVal = moveVal;
                    }
                    if(moveVal == bestVal){
                        //printf("będę losował\n");
                        f = genRand(r);
                        if(f > 0.49){
                            bestVal = moveVal;
                            bestMove = 10*(i+1) + j + 1;
                        }
                    }
                }
            }
        }
  
    //printf("The value of the best Move is : %d\n\n",bestVal);
  
    return bestMove;
}
int xpos;
int firstMoveO(int depth){

    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            if(board[i][j] == 1){
                xpos = 10*(i+1)+j+1;
                j = 10;
                i = 10;
            }
        }
    }

    if(depth%2 == 0){
        switch(xpos){
            case 11:
                return 14;
            case 15:
                return 12;
            case 51:
                return 54;
            case 55:
                return 52;
            case 21:
                return 14;
            case 25:
                return 12;
            case 41:
                return 54;
            case 45:
                return 52;
            case 12:
                return 41;
            case 52:
                return 21; 
            case 14: 
                return 45;
            case 54: 
                return 25;      
            case 22: 
                return 14;
            case 24: 
                return 12;
            case 42: 
                return 54;
            case 44: 
                return 52;    
            case 31: 
                return 13;
            case 35: 
                return 13;
            case 32: 
            case 34: 
                return 13;
            case 23: 
            case 43: 
                return 31;
            case 33:
                return 21;
            case 13: 
            case 53: 
                return 35;        
        }
    }
    else{
        switch ((xpos))
        {
        case 11:
        case 21: 
            return 14;
        case 15: 
        case 25: 
            return 12;
        case 41: 
        case 51: 
            return 54;
        case 45: 
        case 55: 
            return 52;
        case 12: 
            return 41;
        case 14: 
            return 45;
        case 52: 
            return 21;
        case 54: 
            return 25;     
        case 22: 
            return 55;
        case 24: 
            return 51;
        case 42: 
            return 15;
        case 44: 
            return 11;
        case 31: 
            return 12;
        case 35: 
            return 14;
        case 13: 
            return 21;
        case 53:
            return 45;      
        case 32: 
        case 34: 
            return 13;
        case 23: 
        case 43:
            return 31;       
        case 33: 
            return 12;                           

        }
    }
    return 33;
}


bool end_game = false;
int move;
bool correct;

void playGame(int maxdepth, int bot1, int bot2, MTRand* r){

    // pierwsza gra, bot1 to krzyżyk
    end_game = false;
    wincheck = false;
    kontra = false;
    kontratak = -69;
    moveno = 0;
    setBoard();
    move = firstMoveX(1, maxdepth, r, bot1); //
    if(correct){
        setMove(move, 1);
    }
    else{
        end_game = true;
        winratio[bot2]++;
    }
    if(!end_game){
        move = firstMoveO(maxdepth);
        if(correct){
            setMove(move, 2);
        }
        else{
            end_game = true;
            winratio[bot1]++;
        } 
    }
   
    while(!end_game){

        move = findBestMove(1, maxdepth, r, moveno, bot1);
        if(correct){
            setMove(move, 1);
        }
        else{
            end_game = true;
            winratio[bot2]++;
        }
        if(winCheck(1) || loseCheck(2)){
            end_game = true;
            winratio[bot1]++;
        }
        if(loseCheck(1) || winCheck(2)){
            end_game = true;
            winratio[bot2]++;    
        }

        move = findBestMove(2, maxdepth, r, moveno, bot2);
        if(correct){
            setMove(move, 2);
        }
        else{
            end_game = true;
            winratio[bot1]++;
        }
        if(loseCheck(2) || winCheck(1)){
            end_game = true;
            winratio[bot1]++;
        }
        if(winCheck(2) || loseCheck(1)){
            end_game = true;
            winratio[bot2]++;    
        }      

    }

    // druga gra - bot2 jako krzyżyk

    end_game = false;
    wincheck = false;
    kontra = false;
    kontratak = -69;
    moveno = 0;
    setBoard();
    move = firstMoveX(1, maxdepth, r, bot2); //
    if(correct){
        setMove(move, 1);
    }
    else{
        end_game = true;
        winratio[bot1]++;
    }
    if(!end_game){
        move = firstMoveO(maxdepth);
        if(correct){
            setMove(move, 2);
        }
        else{
            end_game = true;
            winratio[bot2]++;
        }
    }

    while(!end_game){

        move = findBestMove(1, maxdepth, r, moveno, bot2);
        if(correct){
            setMove(move, 1);
        }
        else{
            end_game = true;
            winratio[bot1]++;
        }
        if(winCheck(1) || loseCheck(2)){
            end_game = true;
            winratio[bot2]++;
        }
        if(loseCheck(1) || winCheck(2)){
            end_game = true;
            winratio[bot1]++;    
        }

        move = findBestMove(2, maxdepth, r, moveno, bot1);
        if(correct){
            setMove(move, 2);
        }
        else{
            end_game = true;
            winratio[bot2]++;
        }
        if(loseCheck(2) || winCheck(1)){
            end_game = true;
            winratio[bot2]++;
        }
        if(winCheck(2) || loseCheck(1)){
            end_game = true;
            winratio[bot1]++;    
        }        
    }

}

int main(int argc, char *argv[])
{
    MTRand r = seedRand(1337);
    struct timeval begin, finite;
    maxdepth = atoi(argv[1]);


    if( argc!=2 ) {
        printf("Wrong number of arguments\n");
        return -1;
    }


    for(int i = 0; i < 552; i++){ // bot
        for(int j = 1; j < 3; j++){ //znak
            for(int k = 0; k < 3; k++){ //etap
                for(int l = 0; l < 8; l++){ //funkcje
                    wagi[i][j][k][l] = (genRandLong(&r) % 1000) -1;
                }

            }
        }
    }

    gettimeofday(&begin, 0);




    gettimeofday(&finite, 0);
    long seconds = finite.tv_sec - begin.tv_sec;
    long microseconds = finite.tv_usec - begin.tv_usec;
    double elapsed = seconds + microseconds*1e-6;
    
  
    
    printf("Time measured: %.3f seconds.\n", elapsed);

    return 0;

}
