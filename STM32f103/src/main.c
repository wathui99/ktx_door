#include <main.h>



int main (void) {
	/*------------watchdog----------*/
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_256); //xung nhip IWDG la 40KHz, chon prescaler la 256
	IWDG_SetReload(3125); //thoi gian de IWDG dem nguoc va reset lai la 20s => chon gia tri 3125, 4=3125/(40KHz/256)
	IWDG_ReloadCounter(); //reset lai thoi gian.
	IWDG_Enable();
	/*--------------------------------*/
	DelayInit();
	TM_MFRC522_Init();
	User_TIM_Init();
	User_USART2_Init(9600);
	User_GPIO_Init();
	Turn_buzz(OFF);
	Turn_led_close(OFF);
	Turn_led_open(OFF);
	Updata_Data_From_PAGE0();
	uint8_t i=0;
	for (i=0; i<10; i++) {
		if(CheckDoorStatus()==CLOSE) {
			User_TIM_Handle(SERVO_ANGLE_CLOSE);
			Door_status=CLOSE;
			Turn_led_close(ON);
			break;
		}
		DelayMs(200);
	}
//	else {
//		if(Emer_flag==FALSE) {
//			User_TIM_Handle(SERVO_ANGLE_OPEN);
//			Turn_led_open(ON);
//			Door_status=OPEN;
//		}
//	}
	/*---------------------loop while---------------------*/
	while (1) {
		//---------------------ALERT-----------------------
//		if(CheckAlert()) {//if alert occure
//			Alert();
//		}
		//else {
		TM_MFRC522_Init();
			if (TM_MFRC522_Check(ScanedID) == MI_OK){
				IWDG_ReloadCounter(); //reset lai thoi gian.
				if(CheckScanedID(ScanedID)) {//right ID
					if(Door_status!=OPEN) {
						User_USART2_SendSchar("\nID dung, cho mo cua\n");
						OpenDoor();
					}
				}
				else {
					Turn_buzz(ON);
					DelayMs(1000);
					Turn_buzz(OFF);
					User_USART2_SendSchar("\nID sai, Nhap lai!");
				}
			}
		//}
		//-----------------------add new member-----------------------
		if(AddMember_flag) {
			IWDG_ReloadCounter(); //reset lai thoi gian.
			Add_Mem_Procedure();
			AddMember_flag=FALSE;
		}
		if(RemoveMember_flag) {
			IWDG_ReloadCounter(); //reset lai thoi gian.
			Remove_Mem_Procedure();
			RemoveMember_flag=FALSE;
		}
		//------------------------change password----------------------
		if(ChangePassword_flag) {
			IWDG_ReloadCounter(); //reset lai thoi gian.
			ChangePass();
			ChangePassword_flag=FALSE;
		}
		//----------------------change one touch mode------------------------
		if(OneTouchMode_flag) {
			IWDG_ReloadCounter(); //reset lai thoi gian.
			TurnOneTouchMode();
			OneTouchMode_flag=FALSE;
		}
		//----------------------Open door use button------------------------
		if(!Read_status(GPIO_BT,BT1_Pin) && Door_status!=OPEN) {
			IWDG_ReloadCounter(); //reset lai thoi gian.
			if(OneTouch){
				OpenDoor();
			}
		}
		//----------------------Open door use USART------------------------
		if(OpenDoorUSART_flag && Door_status!=OPEN) {
			IWDG_ReloadCounter(); //reset lai thoi gian.
			if(OneTouch){
				OpenDoor();
			}
			else {
				if(RequirePassword()) {
					OpenDoor();
				}
			}
			OpenDoorUSART_flag=FALSE;
		}
		//------------------------close door automatically-----------------------
		if(CheckDoorStatus()==CLOSE && Door_status!=CLOSE) {//able close door
			IWDG_ReloadCounter(); //reset lai thoi gian.
			//delay 500ms in case someone open again
			DelayMs(500);
			if(CheckDoorStatus()==CLOSE){
				CloseDoor();
			}
		}
		IWDG_ReloadCounter(); //reset lai thoi gian.
		//--------------------end loop-------------------
	}
}
