#ifndef __PINOUT_H_
#define __PINOUT_H_

// moteur A
#define DIR_M1  D37
#define STEP_M1 D38

// moteur B
#define DIR_M2  D2
#define STEP_M2 D3

// moteur C
#define DIR_M3  D4
#define STEP_M3 D5 

// moteur D
#define DIR_M4  D7
#define STEP_M4 D6

//Servo_bras 1
#define SERVO_B1  PA_0
//Servo_bras 2
#define SERVO_B2  PD_12
//Servo_bras 3
#define SERVO_B3  PD_13

//Servo_bras_1_prise_1
#define SERVO_B1_P1  PC_8
//Servo_bras_1_prise_2
#define SERVO_B1_P2  PB_11

//Servo_bras_2_prise_1
#define SERVO_B2_P1  PC_9
//Servo_bras_2_prise_2
#define SERVO_B2_P2  PB_10

//Servo_bras_3_prise_1
#define SERVO_B3_P1  PE_5
//Servo_bras_3_prise_2
#define SERVO_B3_P2  PE_6

//Turbine 1
#define TURB1  PF_8
//Turbine 2
#define TURB2  PF_7
//Turbine 3
#define TURB3  PF_9

//Ldc Screen
#define LCD_RS PC_10
#define LCD_EN PC_11
#define LCD_D4 PC_12
#define LCD_D5 PD_2
#define LCD_D6 PG_2
#define LCD_D7 PG_3

//Button init
#define BT_INIT PE_2
#define BT_INIT_GND PE_4

// Bluetooth HC-06
#define UART_TX D1  
#define UART_RX D0

//LIDAR
#define LIDAR_TX PD_5  
#define LIDAR_RX PD_6

//Tirette
#define BT_TIRETTE PE_3



#endif // __PINOUT_H_
