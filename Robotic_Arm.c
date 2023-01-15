#include "MKL25Z4.h"                     // Device header
#include <stdio.h>
#include "math.h" // for mathematical operations 


#define MASK(x) (1UL << (x)) // Macro for masking
#define servo_mod 3268 //Period value
static int angle1; 	//temporary global angle value for servo 1
static int angle2;	//temporary global angle value for servo 2
static int angle3;	//temporary global angle value for servo 3
static int angle4;	//temporary global angle value for servo 4
/* Declarations of functions */
void TPM_PWM_init(void);
void ADC0_init(void);
int ADC_getresults(int channel);
int Ontime_cal(float duty, int mod);
void Servo_control(int servo_no, int angle);
void TPM_CnV_change(int servo_no, int duty);
void Delay(volatile unsigned int time_del);


int main(void){
	int adcres1,adcres2,adcres3,adcres4;
	TPM_PWM_init();
	ADC0_init();
	while(1){
	adcres1=ADC_getresults(0);
	angle1=round(adcres1/22.755555);
	Servo_control(1,angle1);
	Delay(3000);
	adcres2=ADC_getresults(3);
	angle2=round(adcres2/22.755555);
	Servo_control(2,angle2);
	Delay(3000);
	adcres3=ADC_getresults(8);
	angle3=round(adcres3/22.755555);
	Servo_control(3,angle3);
	Delay(3000);
	adcres4=ADC_getresults(9);
	angle4=round(adcres4/22.755555);
	Servo_control(4,angle4);
	Delay(60000);
}
}
void TPM_PWM_init(void){

	SIM->SCGC5 |= 0x1000;                             // Enable clock to Port D

  PORTD->PCR[0] = 0x0400;                           // PTD-0 to TPM0-CH0

	PORTD->PCR[1] = 0x0400;                           // PTD-1 to TPM0-CH1

	PORTD->PCR[2] = 0x0400;                           // PTD-2 to TPM0-CH2
	PORTD->PCR[3] = 0x0400;                           // PTD-3 to TPM0-CH3 
                      // Configuring PTD-5 Mux (TPM0)
	SIM->SCGC6 |= 0x01000000;                         // Enable clock to TPM0
	SIM->SOPT2 |= 0x01000000;                         // Clock source selection
	TPM0->SC = 0;                                     // Disable timer

	TPM0->CONTROLS[0].CnSC = 0x20 | 0x08;             // Center-aligned, non inverted TPM0-CH0
	TPM0->CONTROLS[1].CnSC = 0x20 | 0x08;             // Center-aligned, non inverted TPM0-CH1
	TPM0->CONTROLS[2].CnSC = 0x20 | 0x08;							// Center-aligned, non inverted TPM0-CH2
	TPM0->CONTROLS[3].CnSC = 0x20 | 0x08; 						// Center-aligned, non inverted TPM0-CH3
	
	Servo_control(1, 0);
	Servo_control(2, 0);
	Servo_control(3, 0);
	Servo_control(4, 0);
		
      
}

int ADC_getresults(int channel){
	int result=0;
	ADC0->SC1[0] = channel; /* start conversion on channel 0 */
	while(!(ADC0->SC1[0] & 0x80)) { } /* wait for conversion complete */
	result = ADC0->R[0]; /* read conversion result and clear COCO flag */
	return result;
}


void TPM_CnV_change(int servo_no, int cnv){
	TPM0->MOD = 0x0CCC; 
	if(servo_no==1){TPM0->CONTROLS[0].CnV = cnv;}      // Writes the CnV values to the channel 0
	else{
	if(servo_no==2){TPM0->CONTROLS[1].CnV = cnv;}			 // Writes the CnV values to the channel 1
	else{
	if(servo_no==3){TPM0->CONTROLS[2].CnV = cnv;}			 // Writes the CnV values to the channel 2
	else{	
	if(servo_no==4){TPM0->CONTROLS[3].CnV = cnv;}			 // Writes the CnV values to the channel 3
}
}
}
	TPM0->SC = 0xF;   //TPM0 initalize
}    
	
int Ontime_cal(float duty, int mod){
	int cnv;
	cnv = (duty*(mod))/100.0;                         // Calculate the CnV given the duty cycle and MOD
	return cnv;
}

void Servo_control(int servo_no, int data){
	float duty = 0.0;
	int cnv = 0, angle;
	angle=data;
	duty = (10*angle / 180 ) + 2.5 ;                     		// angle to duty cycle
	cnv = Ontime_cal(duty, servo_mod);             					// send duty to calculation
	TPM_CnV_change(servo_no,cnv);                           // calculated result send to channel's counter value
}

void ADC0_init(void) // ADC initilaze
{
SIM->SCGC5 |= 0x2000; /* clock to PORTE */
SIM->SCGC5 |= 0x400;
PORTE->PCR[20] = 0;
PORTE->PCR[22] = 0;
PORTB->PCR[1] = 0;
PORTB->PCR[0] = 0;	/* PTE20 analog input */
SIM->SCGC6 |= 0x8000000; /* clock to ADC0 */
ADC0->SC2 &= ~0x40; /* software trigger */
/* clock div by 4, long sample time, single ended 12 bit, bus clock */
ADC0->CFG1 = 0x40 | 0x10 | 0x04 | 0x00;
}


void Delay(volatile unsigned int time_del) { // System Delay
  while (time_del--) 
		{
  }
}