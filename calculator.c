#include<stdbool.h>
/// Create definition for PORT B registers
#define GPIO_PORTB_DATA_R       (*((volatile unsigned long *)0x400053FC))
#define GPIO_PORTB_DIR_R        (*((volatile unsigned long *)0x40005400))
#define GPIO_PORTB_PUR_R        (*((volatile unsigned long *)0x40005510))
#define GPIO_PORTB_DEN_R        (*((volatile unsigned long *)0x4000551C))
#define GPIO_PORTB_CR_R         (*((volatile unsigned long *)0x40005524))
#define GPIO_PORTB_AMSEL_R      (*((volatile unsigned long *)0x40005528))
#define GPIO_PORTB_PCTL_R       (*((volatile unsigned long *)0x4000552C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))

//Create definitions for Port E registers
#define GPIO_PORTE_DATA_R       (*((volatile unsigned long *)0x400243FC))
#define GPIO_PORTE_DIR_R        (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_PUR_R        (*((volatile unsigned long *)0x40024510))
#define GPIO_PORTE_DEN_R        (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_CR_R         (*((volatile unsigned long *)0x40024524))
#define GPIO_PORTE_AMSEL_R      (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_AFSEL_R      (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_PCTL_R       (*((volatile unsigned long *)0x4002452C))

//Create definitions for Port F registers
#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))

//Create definitions for Port D registers  // Keypad ROW Pins as output
#define GPIO_PORTD_DATA_R       (*((volatile unsigned long *)0x400073FC))
#define GPIO_PORTD_DIR_R        (*((volatile unsigned long *)0x40007400))
#define GPIO_PORTD_AFSEL_R      (*((volatile unsigned long *)0x40007420))
#define GPIO_PORTD_PUR_R        (*((volatile unsigned long *)0x40007510))
#define GPIO_PORTD_DEN_R        (*((volatile unsigned long *)0x4000751C))
#define GPIO_PORTD_LOCK_R       (*((volatile unsigned long *)0x40007520))
#define GPIO_PORTD_CR_R         (*((volatile unsigned long *)0x40007524))
#define GPIO_PORTD_AMSEL_R      (*((volatile unsigned long *)0x40007528))
#define GPIO_PORTD_PCTL_R       (*((volatile unsigned long *)0x4000752C))

//Create definitions for Port A registers  // Keypad Column Pins as input
#define GPIO_PORTA_DATA_R       (*((volatile unsigned long *)0x400043FC))
#define GPIO_PORTA_DIR_R        (*((volatile unsigned long *)0x40004400))
#define GPIO_PORTA_AFSEL_R      (*((volatile unsigned long *)0x40004420))
#define GPIO_PORTA_PUR_R        (*((volatile unsigned long *)0x40004510))
#define GPIO_PORTA_DEN_R        (*((volatile unsigned long *)0x4000451C))
#define GPIO_PORTA_LOCK_R       (*((volatile unsigned long *)0x40004520))
#define GPIO_PORTA_CR_R         (*((volatile unsigned long *)0x40004524))
#define GPIO_PORTA_AMSEL_R      (*((volatile unsigned long *)0x40004528))
#define GPIO_PORTA_PCTL_R       (*((volatile unsigned long *)0x4000452C))

//Global variables
int numbers[16] = {0x40,0x79,0x24,0x30,0x19, // Each value turns on bits needed
0x12,0x02,0x78,0x00,0x10, 
0x08, 0x00, 0x46, 0x40, 0x06, 0x0E}; // to show numbers in display
int digit1, digit2, digit3, digit4; // Number to be displayed in each digit
unsigned long SW2; // On-board push button
unsigned long count=9999; // Counter for button presses

int before=1; // States if button was pressed in previous
// loop cycle to enable counter.

void PortAD_Init(); // PORTA: Keypad column, PORTD: keypad row
//   Function Prototypes
void PortBEF_Init(void); // PORT B: 7-SED a, b, c, d, e, f, g, LED selection
// PORT E: Select Digit 1, 2, 3, 4 (L-R)
// PORT F: SW2 to turn on
void Delay(void);
void Display(int number, int digit);

int cdelay = 100;
int vdelay = 0;
unsigned long swCont = 0;
//  Subroutines Section
unsigned char getKey(void);
//const unsigned char keymap[4][4];
unsigned char calculate(int a, int b, int oper);
void NumSplit(int counted);
	
void PortAD_Init(){
	SYSCTL_RCGC2_R |= 0x00000009; // clk for A and D
// Initialize PORTD as Row output
	GPIO_PORTD_LOCK_R = 0x4C4F434B;   // Unlock PortD
  GPIO_PORTD_CR_R = 0x0F;           // Allow changes to PD3-PD0    
  GPIO_PORTD_AMSEL_R = 0x00;        // Disable analog function
  GPIO_PORTD_PCTL_R = 0x00000000;   // GPIO clear bit PCTL  
  GPIO_PORTD_DIR_R = 0x0F;          // PD3-PD0 output  
  GPIO_PORTD_AFSEL_R = 0x00;        // No alternate function
  GPIO_PORTD_PUR_R = 0x00;          // No Enable pullup resistor      
  GPIO_PORTD_DEN_R = 0x0F;          // Enable digital pins PD3-PD0   
// Initialize PORTA as column input
GPIO_PORTA_LOCK_R = 0x4C4F434B;   // Unlock PortA
  GPIO_PORTA_CR_R = 0xF0;           // Allow changes to PA7-PA4    
  GPIO_PORTA_AMSEL_R = 0x00;        // Disable analog function
  GPIO_PORTA_PCTL_R = 0x00000000;   // GPIO clear bit PCTL  
  GPIO_PORTA_DIR_R = 0x00;          // PA7-PA4 input  
  GPIO_PORTA_AFSEL_R = 0x00;        // No alternate function
  GPIO_PORTA_PUR_R = 0xF0;          // Enable pullup resistors on PA7-PA4      
  GPIO_PORTA_DEN_R = 0xF0;          // Enable digital pins PA7-PA4

}

// Subroutine to initialize ports B, E and F
void PortBEF_Init(void){
  SYSCTL_RCGC2_R |= 0x00000002;     // Port B clock initialized
  GPIO_PORTB_CR_R = 0x7F;           // Allow changes to PB6-PB0      
  GPIO_PORTB_AMSEL_R = 0x00;        // Disable analog function
  GPIO_PORTB_PCTL_R = 0x00000000;   // GPIO clear bit PCTL  
  GPIO_PORTB_DIR_R = 0x7F;          // Set PB6-PB0 outputs  
  GPIO_PORTB_PUR_R = 0x00;          // Enable pullup resistors on PB4,PF0      
  GPIO_PORTB_DEN_R = 0x7F;          // 7) Enable digital pins PB6-PB0        

  SYSCTL_RCGC2_R |= 0x00000010;     // Port E clock initialized
  GPIO_PORTE_CR_R = 0x0F;           // Allow changes to PE4-0      
  GPIO_PORTE_AMSEL_R = 0x00;        // Disable analog function
  GPIO_PORTE_PCTL_R = 0x00000000;   // GPIO clear bit PCTL  
  GPIO_PORTE_DIR_R = 0x0F;          // PE3-PE0 output  
  GPIO_PORTE_PUR_R = 0x00;          // Disable pullup resistors      
  GPIO_PORTE_DEN_R = 0x0F;          // Enable digital pins PE3-PE0

  SYSCTL_RCGC2_R |= 0x00000020;     // Port F clock initialized
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // Unlock PortF
  GPIO_PORTF_CR_R = 0x1F;           // Allow changes to PB4-PB0    
  GPIO_PORTF_AMSEL_R = 0x00;        // Disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;   // GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R = 0x0E;          // PF4,PF0 input  
  GPIO_PORTF_AFSEL_R = 0x00;        // No alternate function
  GPIO_PORTF_PUR_R = 0x11;          // Enable pullup resistors on PF4,PF0      
  GPIO_PORTF_DEN_R = 0x1F;          // Enable digital pins PF4-PF0        
}


int keymap[4][4] = {
{ 1, 2, 3, 10},
{ 4, 5, 6, 11},
{ 7, 8, 9, 12},
{ 14, 0, 15, 13},
};
 
int row, column;

unsigned char getKey(void){
// first check if anykey has been pressed
	GPIO_PORTD_DATA_R = 0x00;
	column = GPIO_PORTA_DATA_R &0xF0;
//if(column == 0xF0){
//return 0;
//}
	while (1){ // find out which row has been activated
		row = 0;
		GPIO_PORTD_DATA_R = 0x0E;
		Delay();
		column = GPIO_PORTA_DATA_R &0xF0;
		if(column != 0xF0) 
			{break;}

		row = 1;
		GPIO_PORTD_DATA_R = 0x0D;
		Delay();
		column = GPIO_PORTA_DATA_R &0xF0;
		if(column != 0xF0) 
			{break;}

		row = 2;
		GPIO_PORTD_DATA_R = 0x0B;
		Delay();
		column = GPIO_PORTA_DATA_R &0xF0;
		if(column != 0xF0) 
			{break;}

		row = 3;
		GPIO_PORTD_DATA_R = 0x07;
		Delay();
		column = GPIO_PORTA_DATA_R &0xF0;
		if(column != 0xF0) 
			{break;}
	}

	if (column == 0xE0) return keymap[row][0]; /* key in column 0 */
	if (column == 0xD0) return keymap[row][1]; /* key in column 1 */
	if (column == 0xB0) return keymap[row][2]; /* key in column 2 */
	if (column == 0x70) return keymap[row][3]; /* key in column 3 */
	return 0; /* just to be safe */
}
unsigned char calculate(int a, int b1, int oper){
	int ty=0, res=0;
	ty = oper;
	if (ty == 10){
		res= a+b1;
	}
	else if (ty == 11){
		res= a-b1;
	}
	else if (ty == 12){
		res= a*b1;
	}
	else if (ty == 13){
		res= a/b1;
	}
	return res;
}

unsigned char key;
int main(void){
  PortBEF_Init();         // Initialize microcontroller ports
	PortAD_Init();
	int a;
	int oper;
	int b1;
	int c;
	
	bool a_t = false;	
	bool oper_t = false;
	bool b_t = false;
	int value, dsp;
	
	while(1){
		key = getKey();
		if(key != 0){
			value = (int)key;
			if (a_t == false && value <=9 && value >= 0){
				//Display(key, 1);
				a_t = true;
				a = value;
				dsp = a;
				//Delay();
			}
			if (oper_t == false && value <=13 && value >= 10){
				oper_t = true;
				
				//Display(key, 2);
				oper = value;
				dsp = oper;
				//Delay();
			}
			if (oper_t == true && a_t == true && b_t == false && value<=9 && value>=0){
				b_t = true;
				
				b1 = value;
				dsp = b1;
				//Display(key, 2);
				//Delay();
			}
			if (value == 15){
				c = calculate(a, b1, oper);
				dsp = c;
				//Display(c, 8);
				a_t = false;	
				oper_t = false;
				b_t = false;
				//Delay();
			}
			
			
			//Delay();
			int i=0;
			
		
			NumSplit(dsp);	
			Display(4,digit2);
			Delay();
			Display(8,digit1);
			Delay();
				
		}
		
		//else Delay();
		
  }
}

void Display(int digit, int number){
	GPIO_PORTB_DATA_R = 0x00; // Turns off LEDs
	GPIO_PORTE_DATA_R = digit; // Selects digit
	GPIO_PORTB_DATA_R = numbers[number]; // Turns on number in selected digit
	//Delay(); // Wait 0.1 ms
}

// Splits number in counter into 4 separate numbers for each digit
void NumSplit(int counted){
		digit1 = counted%10; //Copies value in counter, divides it by 10 and then keeps remainder
    counted /= 10; //Dividing value in counter by 10 shifts it by one decimal
    digit2 = counted%10;
    counted /= 10;
}


// Creates 0.1ms delay
void Delay(void){
	unsigned long volatile time;
	time = 727240*200/91000;  // 0.1 ms
  while(time){
	time--;
  }
}
