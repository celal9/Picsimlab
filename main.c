/*
 * File: Main.c
 */


#include "Pragmas.h"
#include "ADC.h"
#include "LCD.h"
#include <string.h>
#include <stdio.h>
#include <xc.h>
int selected_player=0;
int check=0;
unsigned int rbval = 0;
unsigned int fresbee_moving = 0;
unsigned int refreshspeed=0;
unsigned int loopcountertmr1=0;
unsigned int rb4_pressed=0;
unsigned int frisbee_steps_counter=0;
unsigned int valid_mode=1;
unsigned int rb1_pressed=0;
unsigned int team1_score=0;
unsigned int team2_score=0;
unsigned int disp0 = 0;
unsigned int disp1 = 0;


unsigned int number_of_steps_ret;
unsigned short frisbee_steps[15][2];  
unsigned int players[5][3]={ // player variable 5 of it are player1,player2,player3.player4 and for frisbee for example firstplayer_x=3 y=2 and 2, index for the 
    // which character index (LCDAddSpecialCharacter)
    {
        3,2,1
    },
    {
        3,3,0
    },
    {
        14,2,3
    },
    {
        14,3,3
    },
    {
        9,2,6
    }
    
};
byte zero[8] ={
                  0b00000,
                  0b00000,
                  0b00000,
                  0b00000,
                  0b00000,
                  0b00000,
                  0b00000,
                  0b00000
                };
byte teamA_player[3][8] = {
                {
                  0b10001,
                  0b10101,
                  0b01010,
                  0b00100,
                  0b00100,
                  0b00100,
                  0b01010,
                  0b01010
                },
                {
                  0b10001,
                  0b10101,
                  0b01010,
                  0b00100,
                  0b00100,
                  0b00100,
                  0b01010,
                  0b11111
                },{
                  0b11111,
                  0b10101,
                  0b01010,
                  0b00100,
                  0b00100,
                  0b00100,
                  0b01010,
                  0b11111
                }
};              // teamA_player NOT holding the frisbee, NOT indicated by the cursor

byte teamB_player[3][8] = {{
                  0b10001,
                  0b10101,
                  0b01010,
                  0b00100,
                  0b01110,
                  0b11111,
                  0b01010,
                  0b01010
                },{
                  0b10001,
                  0b10101,
                  0b01010,
                  0b00100,
                  0b01110,
                  0b11111,
                  0b01010,
                  0b11111
                },{
                  0b11111,
                  0b10101,
                  0b01010,
                  0b00100,
                  0b01110,
                  0b11111,
                  0b01010,
                  0b11111
                }};              // teamB_player NOT holding the frisbee, NOT indicated by the cursor
         
byte frisbee[2][8] = {{
                  0b01110,
                  0b11111,
                  0b11111,
                  0b11111,
                  0b01110,
                  0b00000,
                  0b00000,
                  0b00000
                },{
                  0b01110,
                  0b10001,
                  0b10001,
                  0b10001,
                  0b01110,
                  0b00000,
                  0b00000,
                  0b00000
                }};              // the frisbee itself

// Display function arranges team1score, `-` and team2score and display  accordingly with lata and latd 
void display(){
    switch(team1_score){
			case 0: 
				disp0 = 63  ;// b'00111111'
				break ;
			case 1: 
				disp0 = 6   ;// b'00000110'
				break ;
			case 2: 
				disp0 = 91  ;// b'01011011'
				break ;
			case 3: 
				disp0 = 79  ;// b'01001111'
				break ;
			case 4: 
				disp0 = 102 ;// b'01100110'
				break ;
			case 5: 
				disp0 = 109 ;// b'01101101'
				break ;
			case 6: 
				disp0 = 125 ;// b'01111101'
				break ;
			case 7: 
				disp0 = 7   ;// b'00000111'
				break ;
			case 8: 
				disp0 = 127 ;// b'01111111'
				break ;
			case 9: 
				disp0 = 111 ;// b'01101111'
				break ;
			default:
				/* if execution reaches here then there is a bug */
				break ; 
      }       
    switch(team2_score){
			case 0: 
				disp1 = 63  ;// b'00111111'
				break ;
			case 1: 
				disp1 = 6   ;// b'00000110'
				break ;
			case 2: 
				disp1 = 91  ;// b'01011011'
				break ;
			case 3: 
				disp1 = 79  ;// b'01001111'
				break ;
			case 4: 
				disp1 = 102 ;// b'01100110'
				break ;
			case 5: 
				disp1 = 109 ;// b'01101101'
				break ;
			case 6: 
				disp1 = 125 ;// b'01111101'
				break ;
			case 7: 
				disp1 = 7   ;// b'00000111'
				break ;
			case 8: 
				disp1 = 127 ;// b'01111111'
				break ;
			case 9: 
				disp1 = 111 ;// b'01101111'
				break ;
			default:
				/* if execution reaches here then there is a bug */
				break ; 
      }          
                
   

        
        LATA=8;
		LATD = disp0 ;
        __delay_us(100);
        LATA=16;
		LATD = 64 ;
        __delay_us(100);
        LATA=32;
		LATD = disp1 ;
        __delay_us(100);
        

	
    
    
        
}
// compute_frisbee_target_and_route it arrange random target for frisbee and arrange the route
unsigned short compute_frisbee_target_and_route(unsigned short current_fisbee_x_position, unsigned short current_fisbee_y_position) {
    
   // YOU CAN DO ANY NECESSARY CHANGES IN THIS FUNCTION //
    
    unsigned short x_step_size, y_step_size;    // hold the number of cells to walk in x and y dimensions, respectively
    unsigned short number_of_steps;             // hold the maximum number of steps to walk
    unsigned short target_x, target_y;         // hold the target <x,y> coordinates of the frisbee
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    // compute target <x,y> position for the frisbee
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    
    while(1) {  // loop until finding a valid position
        
        target_x = TMR0L%16 ; // find a random integer in [0, 15] + 1
        target_y = TMR0H%4 ;  // find a random integer in [0, 3] + 1
        
        // how many cells are there in x-dimension (horizontal) between the target and current positions of the frisbee
        if (target_x < current_fisbee_x_position)
            x_step_size = current_fisbee_x_position - target_x;
        else
            x_step_size = target_x - current_fisbee_x_position;
        
        // how many cells are there in y-dimension (vertical) between the target and current positions of the frisbee
        if (target_y < current_fisbee_y_position)
            y_step_size = current_fisbee_y_position - target_y;
        else
            y_step_size = target_y - current_fisbee_y_position;
        
        // a close target cell is not preferred much, so change the target if it is very close
        if (x_step_size <= 2 && y_step_size <= 2)
            continue;
        
        // total number of steps can be as many as the maximum of {x_step_size, y_step_size}
        if (x_step_size > y_step_size)
            number_of_steps = x_step_size;
        else
            number_of_steps = y_step_size;
        
        break;
    }
    
    
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    // compute a route for the frisbee to reach the target step-by-step
    // note that each step corresponds to a 1-cell-movement in x and y directions simultaneously
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    
    // steps in x direction
    unsigned short x = current_fisbee_x_position;
    if (target_x < current_fisbee_x_position) {
        for (unsigned short i = 0; i < x_step_size; i++) {
            x = x - 1;                  // move in -x direction
            frisbee_steps[i][0] = x;    // the x-position at i^th step
        }  
    }
    else {
        for (unsigned short i = 0; i < x_step_size; i++) {
            x = x + 1;                  // move in +x direction
            frisbee_steps[i][0] = x;    // the x-position at i^th step
        } 
    }
    for (unsigned short i = x_step_size; i < number_of_steps; i++)
        frisbee_steps[i][0] = x;        // fill the rest of the steps as "no movement" in x direction 
    
    // steps in y direction
    unsigned short y = current_fisbee_y_position;
    if (target_y < current_fisbee_y_position) {
        for (unsigned short i = 0; i < y_step_size; i++) {
            y = y - 1;                  // move in -y direction
            frisbee_steps[i][1] = y;    // the y-position at i^th step
        }  
    }
    else {
        for (unsigned short i = 0; i < y_step_size; i++) {
            y = y + 1;                  // move in +y direction
            frisbee_steps[i][1] = y;    // the y-position at i^th step
        } 
    }
    for (unsigned short i = y_step_size; i < number_of_steps; i++)
        frisbee_steps[i][1] = y;        // fill the rest of the steps as "no movement" in y direction
    
    return number_of_steps;
}
unsigned short random_generator() {
    unsigned short randomy = TMR0H%4;
    if(selected_player!=0){
        if(randomy==0){
            if(players[0][0]<16 && (players[0][0]+1!=players[1][0] || players[0][1]!=players[1][1]) && (players[0][0]+1!=players[2][0] || players[0][1]!=players[2][1] )&& (players[0][0]+1!=players[3][0] || players[0][1]!=players[3][1] )){
                players[0][0]++;
            }
            else{
                    randomy+=TMR0H%3+1;
                }
        }
        if(randomy==1){
            if(players[0][0]>1 && (players[0][0]-1!=players[1][0] || players[0][1]!=players[1][1]) && (players[0][0]-1!=players[2][0] || players[0][1]!=players[2][1] )&& (players[0][0]-1!=players[3][0] || players[0][1]!=players[3][1] )){
                players[0][0]--;
            }
            else{
                    randomy+=TMR0H%2+1;
                }
        }
        
        if(randomy==2){
            if(players[0][1]<16 && (players[0][0]!=players[1][0] || players[0][1]+1!=players[1][1]) && (players[0][0]!=players[2][0] || players[0][1]+1!=players[2][1] )&& (players[0][0]!=players[3][0] || players[0][1]+1!=players[3][1] )){
                players[0][1]++;
            }
            else{
                    randomy+=1;
                }
        }
        if(randomy==3){
            if(players[0][1]>1 && (players[0][0]!=players[1][0] || players[0][1]-1!=players[1][1]) && (players[0][0]!=players[2][0] || players[0][1]-1!=players[2][1] )&& (players[0][0]!=players[3][0] || players[0][1]-1!=players[3][1] )){
                players[0][1]--;
            }
            
        }
        
        
    }

    randomy = TMR0L%4;
    
    if(selected_player!=1){
        
        if(randomy==0){
            if(players[1][0]<16 && (players[1][0]+1!=players[0][0] || players[1][1]!=players[0][1]) && (players[1][0]+1!=players[2][0] || players[1][1]!=players[2][1] )&& (players[1][0]+1!=players[3][0] || players[1][1]!=players[3][1] )){
                players[1][0]++;
            }  
            else{
                    randomy+=TMR0L%3+1;
                }
        }
        if (randomy==1){
            if(players[1][0]>1 && (players[1][0]-1!=players[0][0] || players[1][1]!=players[0][1]) && (players[1][0]-1!=players[2][0] || players[1][1]!=players[2][1] )&& (players[1][0]-1!=players[3][0] || players[1][1]!=players[3][1] )){
                players[1][0]--;
            }
            else{
                    randomy+=TMR0L%2+1;
                }
        }

        if(randomy==2){
            if(players[1][1]<16 && (players[1][1]+1!=players[0][1] || players[1][0]!=players[0][0]) && (players[1][0]!=players[2][0] || players[1][1]+1 !=players[2][1] )&& (players[1][0]!=players[3][0] || players[1][1]+1 !=players[3][1] )){
                players[1][1]++;
            }
            else{
                    randomy+=1;
                }
        }
        if (randomy==3){
            if(players[1][1]>1 && (players[1][1]-1!=players[0][1] || players[1][0]!=players[0][0]) && (players[1][0]!=players[2][0] || players[1][1]-1 !=players[2][1] )&& (players[1][0]!=players[3][0] || players[1][1]-1 !=players[3][1] ) ){
                players[1][1]--;
            }
        }
        
        
    }
    randomy = (TMR0H/TMR0L)%4;
    if(selected_player!=2){

            if(randomy==0){
                if(players[2][0]<16 && (players[2][0]+1!=players[0][0] || players[2][1]!=players[0][1]) && (players[2][0]+1!=players[1][0] || players[2][1]!=players[1][1] )&& (players[2][0]+1!=players[3][0] || players[2][1]!=players[3][1] )){
                    players[2][0]++;
                }
                else{
                    randomy+=(TMR0H/TMR0L)%3+1;
                }
                
            }
            if (randomy==1){
                if(players[2][0]>1 && (players[2][0]-1!=players[0][0] || players[2][1]!=players[0][1]) && (players[2][0]-1!=players[1][0] || players[2][1]!=players[1][1] )&& (players[2][0]-1!=players[3][0] || players[2][1]!=players[3][1] ) ){
                    players[2][0]--;
                }
                else{
                    randomy+=(TMR0H/TMR0L)%1+1;
                }
            }

            if(randomy==2){
                if(players[2][1]<16 && (players[2][0]!=players[0][0] || players[2][1]+1!=players[0][1]) && (players[2][0]!=players[1][0] || players[2][1]+1!=players[1][1] )&& (players[2][0]!=players[3][0] || players[2][1]+1!=players[3][1] ) ){
                    players[2][1]++;
                }
                else{
                    randomy+=1;
                }
            }
            if(randomy==3){
                if(players[2][1]>1 && (players[2][0]!=players[0][0] || players[2][1]-1!=players[0][1]) && (players[2][0]!=players[1][0] || players[2][1]-1!=players[1][1] )&& (players[2][0]!=players[3][0] || players[2][1]-1!=players[3][1] ) ){
                    players[2][1]--;
                }
            }
        
        
    }
    
    randomy = (TMR0L/TMR0H)%4;
    if(selected_player!=3){

            if(randomy==0){
                if(players[3][0]<16 && (players[3][0]+1!=players[0][0] || players[3][1]!=players[0][1]) && (players[3][0]+1!=players[1][0] || players[3][1]!=players[1][1] )&& (players[3][0]+1!=players[2][0] || players[3][1]!=players[2][1] ) ){
                    players[3][0]++;
                }
                else{
                    randomy+=(TMR0L/TMR0H)%3+1;
                }
                
            }
            if (randomy==1){
                if(players[3][0]>1 && (players[3][0]-1!=players[0][0] || players[3][1]!=players[0][1]) && (players[3][0]-1!=players[1][0] || players[3][1]!=players[1][1] )&& (players[3][0]-1!=players[2][0] || players[3][1]!=players[2][1] ) ){
                    players[3][0]--;
                }
                else{
                    randomy+=(TMR0L/TMR0H)%2+1;
                }
            }
        
            if(randomy==2){
                if(players[3][1]<16 && (players[3][0]!=players[0][0] || players[3][1]+1!=players[0][1]) && (players[3][0]!=players[1][0] || players[3][1]+1!=players[1][1] )&& (players[3][0]!=players[2][0] || players[3][1]+1!=players[2][1] ) ){
                    players[3][1]++;
                }
                else{
                    randomy+=1;
                }
            }
            if (randomy==3){
                if(players[3][1]>1 && (players[3][0]!=players[0][0] || players[3][1]-1!=players[0][1]) && (players[3][0]!=players[1][0] || players[3][1]-1!=players[1][1] )&& (players[3][0]!=players[2][0] || players[3][1]-1!=players[2][1] ) ){
                    players[3][1]--;
                }
                
            }
        }
        display();
}
void __interrupt() isr(void)
{
    
    char values[10] = {0};
    unsigned short convertion = 0;
    
    if(TMR1IF == 1) {
		/* For Timer1 interrupt */
		    TMR1 = 15535;
            if(fresbee_moving){
                loopcountertmr1++;
        if(loopcountertmr1==refreshspeed){
            
            if(frisbee_steps_counter<number_of_steps_ret)
                {
                    players[4][0]=1+frisbee_steps[frisbee_steps_counter][0];
                    players[4][1]=5-frisbee_steps[frisbee_steps_counter][1];
                    frisbee_steps_counter++;
                
                }
            else{
                fresbee_moving=0;
            }
            
            random_generator();
            
            if(players[4][0]!=frisbee_steps[number_of_steps_ret-1][0] +1|| players[4][1]!=5-frisbee_steps[number_of_steps_ret-1][1] ){
                if(frisbee_steps_counter==number_of_steps_ret-1 && !check && players[selected_player][0]==frisbee_steps[number_of_steps_ret-1][0]+1 && players[selected_player][1]==5-frisbee_steps[number_of_steps_ret-1][1]){
                    if(selected_player/2==0){
                        team1_score++;
                    }
                    else{
                        team2_score++;
                    }
                    check++;
                    
                        
            
                }
                
                
            }
            loopcountertmr1=0;
        }
        if(players[4][0]!=frisbee_steps[number_of_steps_ret-1][0] +1|| players[4][1]!=5-frisbee_steps[number_of_steps_ret-1][1] ){
                
                LCDGoto(frisbee_steps[number_of_steps_ret-1][0]+1 , 5-frisbee_steps[number_of_steps_ret-1][1]);
                LCDDat(players[4][2]+1);  
                
            }
               
            __delay_us(20000);
            LCDCmd(0x1);    
                
                
            }
        display();
		TMR1IF = 0 ;
        
                
	}
    if(INTCONbits.INT0IF)
    {
        //LCDStr("INT");
        if(PORTBbits.RB0==0){
            if(valid_mode){
                number_of_steps_ret= compute_frisbee_target_and_route(players[4][0],5-players[4][1]);
                frisbee_steps_counter=0;
                fresbee_moving=1;
                check=0;
            }            
            
            
        }

        
                
        
        display();
        INTCONbits.INT0IF = 0;
        
    }
    
    if(INTCON3bits.INT1IF)
    {
        
        if(PORTBbits.RB1==0){
            if(!rb1_pressed){
                
                if(selected_player == 3){
                    players[selected_player][2]--;
                    players[0][2]++ ;
                    selected_player = 0  ;            
                }
            else if(selected_player != 3){
                players[selected_player][2]--;
                players[selected_player+1][2]++ ;
                selected_player++;  
            }
               rb1_pressed=1; 
        }
            else{
                rb1_pressed=0;
                
            }
            
        __delay_us(20000);
       
        display();
        }
        
  
        
        
        INTCON3bits.INT1IF= 0;
    }
    
    
    if(INTCONbits.RBIF == 1) {
        rbval = PORTB;
		/* For ADC interrupt */
        
        if(PORTBbits.RB4==0){
          
            
            if(players[selected_player][1]>1 && (players[selected_player][1]-1 != players[0][1] || players[selected_player][0] != players[0][0]) && (players[selected_player][1]-1 != players[1][1] || players[selected_player][0] != players[1][0]) && (players[selected_player][1]-1 != players[2][1] || players[selected_player][0]!= players[2][0]) && (players[selected_player][1]-1 != players[3][1] || players[selected_player][0]!= players[3][0])     ){
                while (PORTBbits.RB4=0){
                    rbval = PORTB;
                }
                
                players[selected_player][1]-=1;
                __delay_us(20000);
                LCDCmd(0x1);
                
            } 
        }
        else if(PORTBbits.RB5==0){
            
            if(players[selected_player][0]<16  && (players[selected_player][1] != players[0][1] || players[selected_player][0]+1 != players[0][0]) && (players[selected_player][1] != players[1][1] || players[selected_player][0]+1 != players[1][0]) && (players[selected_player][1]!= players[2][1] || players[selected_player][0]+1 != players[2][0]) && (players[selected_player][1] != players[3][1] || players[selected_player][0]+1 != players[3][0]) ){
                while (PORTBbits.RB5!=0){
                    rbval = PORTB;
                }
                if(players[selected_player][1] != players[0][1] || players[selected_player][0] != players[0][0]){
                    
                }
                players[selected_player][0]+=1;
                __delay_us(10000);
                LCDCmd(0x1);
                
            }
        }
        else if(PORTBbits.RB6==0 && (players[selected_player][1]+1 != players[0][1] || players[selected_player][0] != players[0][0]) && (players[selected_player][1]+1 != players[1][1] || players[selected_player][0] != players[1][0]) && (players[selected_player][1]+1 != players[2][1] || players[selected_player][0]!= players[2][0]) && (players[selected_player][1]+1 != players[3][1] || players[selected_player][0]!= players[3][0])){
             
            if(players[selected_player][1]<16){
                while (PORTBbits.RB6!=0){
                    rbval = PORTB;
                }
                
                players[selected_player][1]+=1;
                __delay_us(10000);
                LCDCmd(0x1);
                
            };  
        }
        else if(PORTBbits.RB7==0){
             
            if(players[selected_player][0]>1 && (players[selected_player][1] != players[0][1] || players[selected_player][0]-1 != players[0][0]) && (players[selected_player][1] != players[1][1] || players[selected_player][0]-1 != players[1][0]) && (players[selected_player][1]!= players[2][1] || players[selected_player][0]-1 != players[2][0]) && (players[selected_player][1] != players[3][1] || players[selected_player][0]-1 != players[3][0])){
                while (PORTBbits.RB7!=0){
                    rbval = PORTB;
                }
                
                players[selected_player][0]-=1;
                __delay_us(20000);
                LCDCmd(0x1);
                
            } 
        }
        display();
        INTCONbits.RBIF = 0 ;
		
	}
    if (INTCONbits.T0IF){
		/* For ADC interrupt */
      
        INTCONbits.T0IF = 0; // Acknowledge interrupt
        // Preload the timer with the proper value
        TMR0H = (55536 >> 1) & 0xff;
        TMR0L = 55536 & 0xff;
        // Start A/D comversion right away
        ADCON0bits.GO = 1;
		
	}
    if (PIR1bits.ADIF) {
        
        PIR1bits.ADIF = 0; // Acknowledge interrupt
        // Read result of A/D conversion and add an offset to round to the nearest 
        
             convertion = readADCChannel(0);
             if (convertion>=0 && convertion<=255){
                 refreshspeed=1;
             }
             else if (convertion>=256 && convertion<=511){
                 refreshspeed=2;
             }
             else if (convertion>=512 && convertion<=767){
                 refreshspeed=3;
             }
             else if (convertion>=768 && convertion<=1023){
                 refreshspeed=4;
             }
            LATA=0; 
        LCDGoto(players[0][0], players[0][1]);
        LCDDat(players[0][2]);  

        LCDGoto(players[1][0], players[1][1]);
        LCDDat(players[1][2]);

        LCDGoto(players[2][0], players[2][1]);
        LCDDat(players[2][2]);

        LCDGoto(players[3][0], players[3][1]);
        LCDDat(players[3][2]);

        LCDGoto(players[4][0], players[4][1]);
        LCDDat(6);
 display();
       
                
        // Toggle between channels AN0 and AN1 right away so the acquisition
        // capacitor can catch up until the next TMR0 ISR.
        ADCON0 ^= 0x04;
        
    }
}


void main(void) { 
    TRISA=0;
    
    TRISD=0;
    initADC();
    
    InitLCD();
    // all character for later usage
    LCDAddSpecialCharacter(0, teamA_player[0]);
    LCDAddSpecialCharacter(1, teamA_player[1]);
    LCDAddSpecialCharacter(2, teamA_player[2]);
    LCDAddSpecialCharacter(3, teamB_player[0]);
    LCDAddSpecialCharacter(4, teamB_player[1]);
    LCDAddSpecialCharacter(5, teamB_player[2]);
    LCDAddSpecialCharacter(6, frisbee[0]);
    LCDAddSpecialCharacter(7, frisbee[1]);

    
    
    char values[10] = {0};
    
    unsigned short convertion = 0;
   
    TRISBbits.RB0 = 1;
    TRISBbits.RB1 = 1;

    TRISBbits.RB2 = 0;
    TRISBbits.RB3 = 0;
    
    TRISBbits.RB4 = 1;
    TRISBbits.RB5 = 1;
    TRISBbits.RB6 = 1;
    TRISBbits.RB7 = 1;
 
    INTCONbits.INT0IE = 1; //Enable INT0 pin interrupt
    INTCONbits.INT0IF = 0;  
    
    INTCONbits.RBIE = 1; //Enable INT0 pin interrupt
    INTCONbits.RBIF = 0;  
    
    
    INTCON3bits.INT1IE=1; ////Enable INT1 pin interrupt
    INTCON3bits.INT1IF=0;
    
    
    PIR1bits.ADIF=1;
    
    T0CON = 0x08;
    TMR0H = (55536  >> 8) & 0xff;
    TMR0L = 55536  & 0xff;  
    INTCONbits.TMR0IE = 1;
    T0CONbits.TMR0ON = 1;
    
    
    T1CON = 248 ; 
	TMR1 = 15535;
    PIE1bits.TMR1IE = 1;
	T1CONbits.TMR1ON = 1 ; 
    
    
    PIE1bits.ADIE = 1;
    
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;  
    display();
    while(1)
    {
         display();
        
    }
    
    
    
    return;
}
