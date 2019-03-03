#include "User_Control.h"

uint16_t Emer_flag=FALSE; //emergency case occure
uint16_t Nbr_ID=0; //number of ID
uint8_t OneTouch=FALSE; //one touch to open door
uint16_t password[31]={};

uint8_t Door_status=OPEN;//preveous door's status (open or close)

char TempBuff[50];

volatile uint16_t buff_pos=0;
volatile uint16_t buff[100]={};
volatile uint8_t receive_flag=FALSE;

uint8_t ScanedID[5]={0x00,0x00,0x00,0x00,0x00};

volatile uint8_t Enter_pass_remove_alert=FALSE;

volatile uint8_t AddMember_flag=FALSE;

volatile uint8_t RemoveMember_flag=FALSE;

volatile uint8_t ChangePassword_flag=FALSE;

volatile uint8_t OneTouchMode_flag=FALSE;

volatile uint8_t OpenDoorUSART_flag=FALSE;

//===================convert data=================

uint16_t* ConvertCharToUint16(char *s) {
	uint8_t i;
	static uint16_t temp[31];
	ResetArr(temp,31);
	for(i=0; s[i]; i++) {
		temp[i]=s[i];
	}
	return temp;
}

char* ConvertUint16ToChar(uint16_t* s) {
	uint8_t i;
	static char temp[31];
	ResetArr((uint16_t*)temp,31);
	for(i=0; s[i]; i++) {
		temp[i]=s[i];
	}
	return temp;
}

char* ConvertUint8ToChar(uint8_t* s) {
	uint8_t i;
	static char temp[31];
	ResetArr((uint16_t*)temp,31);
	for(i=0; s[i]; i++) {
		temp[i]=s[i];
	}
	return temp;
}

uint8_t* ConvertCharToUint8(char *s) {
	uint8_t i;
	static uint8_t temp[31];
	ResetArr((uint16_t*)temp,31);
	for(i=0; s[i]; i++) {
		temp[i]=s[i];
	}
	return temp;
}

uint8_t* ConvertUint16ToUint8(uint16_t *s) {
	uint8_t i;
	static uint8_t temp[31];
	ResetArr((uint16_t*)temp,31);
	for(i=0; s[i]; i++) {
		temp[i]=s[i];
	}
	return temp;
}

/*=================Read and Write page 0===============*/

void Write_EmerFlag(uint16_t flag) {
	//position 0 in page 0
	User_FLASH_Write(flag,0,PAGE0);
}
void Updata_EmerFlag(void) {
	//position 0 in page 0
	Emer_flag=User_FLASH_Read(0,PAGE0);
}

void Write_NbrID(uint16_t Nbr) {
	//position 1 in page 0
	User_FLASH_Write(Nbr,1,PAGE0);
}
void Updata_NbrID(void) {
	//position 1 in page 0
	Nbr_ID=User_FLASH_Read(1,PAGE0);
}

void Write_OneTouchMode(uint16_t status) {
	//position 2 in page 0
	User_FLASH_Write(status,2,PAGE0);
}
void Updata_OneTouchMode(void) {
	//position 2 in page 0
	OneTouch=User_FLASH_Read(2,PAGE0);
}

void Write_Password(uint16_t* pass) {
	//stated at position 3 in page 0
	uint8_t i;
	for(i=0; pass[i]; i++) {
		User_FLASH_Write(pass[i],3+i,PAGE0);
	}
	for(i=i; i<31; i++) {
		User_FLASH_Write(0x00,3+i,PAGE0);
	}
}
void Updata_Password(void) {
	//stated at position 3 in page 0
	uint8_t i;
	for(i=0; i<31; i++) {
		password[i]=User_FLASH_Read(3+i,PAGE0);
	}
}

void Write_Page0(uint16_t E_flag, uint16_t Nbr, uint16_t One_status, uint16_t* pass) {
	User_FLASH_Erase(PAGE0);
	
	Write_EmerFlag(E_flag);
	Write_NbrID(Nbr);
	Write_OneTouchMode(One_status);
	Write_Password(pass);
}

void Updata_Data_From_PAGE0() {
	Updata_EmerFlag();
	Updata_NbrID();
	Updata_OneTouchMode();
	Updata_Password();
}

void Display_PAGE0(void) {
	sprintf(TempBuff,"\nEmer_flag:%d\n",Emer_flag);
	User_USART2_SendSchar (TempBuff);
	sprintf(TempBuff,"Nbr_ID:%d\n",Nbr_ID);
	User_USART2_SendSchar(TempBuff);
	sprintf(TempBuff,"One touch:%d\n",OneTouch);
	User_USART2_SendSchar(TempBuff);
	sprintf(TempBuff,"password:%s\n",ConvertUint16ToChar(password));
	User_USART2_SendSchar(TempBuff);
}

/*=================Read and Write page 1===============*/
void Add_ID (uint8_t* InID) {
	uint8_t i, j;
	if(Nbr_ID>=50) {
		User_USART2_SendSchar("\nDa du so luong ID khong the add them\n");
		return;
	}
	
	//case Nbr_ID=0
	if(Nbr_ID==0) {
		User_FLASH_Erase(PAGE1);
		for(i=0; i<5; i++) {
			User_FLASH_Write(InID[i],i, PAGE1);
		}
		return;
	}
	
	//backup data from page 1 to page 4 first
	User_FLASH_Erase(PAGE4);
	/*
	-	i: index of number of ID
	- j: index of IDi
	*/
	for(i=0; i<Nbr_ID; i++) {
		for(j=0; j<5; j++) {
			User_FLASH_Write(User_FLASH_Read(i*5+j, PAGE1), i*5+j, PAGE4); //copy data from page 1 to page 4
		}
	}
	//start write old data (from page 4) and new data to page 1
	User_FLASH_Erase(PAGE1);
	//old data
	for(i=0; i<Nbr_ID; i++) {
		for(j=0; j<5; j++) {
			User_FLASH_Write(User_FLASH_Read(i*5+j, PAGE4), i*5+j, PAGE1);
		}
	}
	//new data
	for(i=0; i<5; i++) {
		User_FLASH_Write(InID[i],Nbr_ID*5 + i, PAGE1);
	}
	/*-------------update nbr_id to flash (page0)---------------*/
//	Nbr_ID++;
//	Write_Page0(Emer_flag,Nbr_ID,OneTouch,password);
}
int8_t Search_ID (uint8_t* InID) {
	uint8_t i,j;
	for(i=0; i<Nbr_ID; i++) {
		for(j=0; j<5; j++) {
			if(InID[j]!=User_FLASH_Read(i*5+j,PAGE1)) {
				break;
			}
		}
		if(j==5) { // phat hien ID dung
			return i;
		}
	}
	return ID_NOT_FOUND;
}

uint8_t* Pick_ID (uint8_t index){
	static uint8_t TempID[5];
	uint8_t i;
	if(index >= Nbr_ID) {
		User_USART2_SendSchar("\nChi so khong hop le\n");
		return NULL;
	}
	for(i=0; i<5; i++) {
		TempID[i]=User_FLASH_Read(index*5+i,PAGE1);
	}
	return TempID;
}

void Remove_ID (uint8_t index) {
	
	uint8_t i, j;
	if(index>=Nbr_ID) return;
	
	//in case: there's only one ID and pos need to remove is 0
	if(Nbr_ID==1 && index==0) {
		User_FLASH_Erase(PAGE1);
		return;
	}
	
	//backup data from page 1 to page 4 first
	User_FLASH_Erase(PAGE4);
	/*
	-	i: index of number of ID
	- j: index of IDi
	*/
	for(i=0; i<Nbr_ID; i++) {
		for(j=0; j<5; j++) {
			User_FLASH_Write(User_FLASH_Read(i*5+j, PAGE1), i*5+j, PAGE4); //copy data from page 1 to page 4
		}
	}
	
	//start write old data from page 4 to page 1 BUT ignore ID index
	User_FLASH_Erase(PAGE1);
	//old data
	for(i=0; i<Nbr_ID-1; i++) {
		//if ID[index] found
		if(i==index) {
			for(i=i+1; i<Nbr_ID; i++) {
				for(j=0; j<5; j++) {
					User_FLASH_Write(User_FLASH_Read(i*5+j, PAGE4), (i-1)*5+j, PAGE1);
				}
			}
			break;
		}
		//if not found
		for(j=0; j<5; j++) {
			User_FLASH_Write(User_FLASH_Read(i*5+j, PAGE4), i*5+j, PAGE1);
		}
	}
	
	/*-------------update nbr_id to flash (page0)--------------*/
//	Nbr_ID--;
//	Write_Page0(Emer_flag,Nbr_ID,OneTouch,password);
}

/*=================Read and Write page 2===============*/
void Add_Name (uint8_t* InName) {
	uint8_t i, j;
	if(Nbr_ID>=50) {
		User_USART2_SendSchar("\nDa du so luong Name khong the add them\n");
		return;
	}
	//case Nbr_ID=0
	if(Nbr_ID==0) {
		User_FLASH_Erase(PAGE2);
		for(i=0; i<10; i++) {
			User_FLASH_Write(InName[i],i, PAGE2);
		}
		return;
	}
	//backup data from page 2 to page 4 first
	User_FLASH_Erase(PAGE4);
	/*
	-	i: index of number of Name
	- j: index of Namei
	*/
	for(i=0; i<Nbr_ID; i++) {
		for(j=0; j<10; j++) {
			User_FLASH_Write(User_FLASH_Read(i*10+j, PAGE2), i*10+j, PAGE4); //copy data from page 2 to page 4
		}
	}
	//start write old data (from page 4) and new data to page 2
	User_FLASH_Erase(PAGE2);
	//old data
	for(i=0; i<Nbr_ID; i++) {
		for(j=0; j<10; j++) {
			User_FLASH_Write(User_FLASH_Read(i*10+j, PAGE4), i*10+j, PAGE2);
		}
	}
	//new data
	for(i=0; i<10; i++) {
		User_FLASH_Write(InName[i],Nbr_ID*10 + i, PAGE2);
	}
	/*-------------update nbr_id to flash (page0)---------------*/
//	Nbr_ID++;
//	Write_Page0(Emer_flag,Nbr_ID,OneTouch,password);
}
int8_t Search_Name (uint8_t* InName) {
	uint8_t i,j;
	for(i=0; i<Nbr_ID; i++) {
		for(j=0; j<10; j++) {
			if(InName[j]!=User_FLASH_Read(i*10+j,PAGE2)) {
				break;
			}
		}
		if(j==10) { // phat hien Name dung
			return i;
		}
	}
	return -1;
}

uint8_t* Pick_Name (uint8_t index){
	static uint8_t TempName[10];
	uint8_t i;
	if(index >= Nbr_ID) {
		User_USART2_SendSchar("\nChi so khong hop le\n");
		return NULL;
	}
	for(i=0; i<10; i++) {
		TempName[i]=User_FLASH_Read(index*10+i,PAGE2);
	}
	return TempName;
}

void Remove_Name (uint8_t index) {
	
	uint8_t i, j;
	if(index>=Nbr_ID) return;
	//in case: there's only one ID and pos need to remove is 0
	if(Nbr_ID==1 && index==0) {
		User_FLASH_Erase(PAGE2);
		return;
	}
	//backup data from page 2 to page 4 first
	User_FLASH_Erase(PAGE4);
	/*
	-	i: index of number of Name
	- j: index of Namei
	*/
	for(i=0; i<Nbr_ID; i++) {
		for(j=0; j<10; j++) {
			User_FLASH_Write(User_FLASH_Read(i*10+j, PAGE2), i*10+j, PAGE4); //copy data from page 2 to page 4
		}
	}
	
	//start write old data from page 4 to page 2 BUT ignore Name index
	User_FLASH_Erase(PAGE2);
	//old data
	for(i=0; i<Nbr_ID-1; i++) {
		//if Name[index] found
		if(i==index) {
			for(i=i+1; i<Nbr_ID; i++) {
				for(j=0; j<10; j++) {
					User_FLASH_Write(User_FLASH_Read(i*10+j, PAGE4), (i-1)*10+j, PAGE2);
				}
			}
			break;
		}
		//if not found
		for(j=0; j<10; j++) {
			User_FLASH_Write(User_FLASH_Read(i*10+j, PAGE4), i*10+j, PAGE2);
		}
	}
	
	/*-------------update nbr_id to flash (page0)---------------*/
//	Nbr_ID--;
//	Write_Page0(Emer_flag,Nbr_ID,OneTouch,password);
}

/*=================Read and Write page 3===============*/

void Add_MSSV (uint32_t InMSSV) {
	uint8_t i, j;
	if(Nbr_ID>=50) {
		User_USART2_SendSchar("\nDa du so luong MSSV khong the add them\n");
		return;
	}
	
	//case Nbr_ID=0
	if(Nbr_ID==0) {
		User_FLASH_Erase(PAGE3);
		User_FLASH_Write((uint16_t)(InMSSV>>16),0,PAGE3); //msb first
		User_FLASH_Write((uint16_t)(InMSSV&0x0000FFFF),1,PAGE3); //lsb after
		return;
	}
	
	//backup data from page 3 to page 4 first
	User_FLASH_Erase(PAGE4);
	/*
	-	i: index of number of MSSV
	- j: index of MSSVi
	*/
	for(i=0; i<Nbr_ID; i++) {
		for(j=0; j<2; j++) {
			User_FLASH_Write(User_FLASH_Read(i*2+j, PAGE3), i*2+j, PAGE4); //copy data from page 3 to page 4
		}
	}
	//start write old data (from page 4) and new data to page 3
	User_FLASH_Erase(PAGE3);
	//old data
	for(i=0; i<Nbr_ID; i++) {
		for(j=0; j<2; j++) {
			User_FLASH_Write(User_FLASH_Read(i*2+j, PAGE4), i*2+j, PAGE3);
		}
	}
	//new data
	User_FLASH_Write((uint16_t)(InMSSV>>16),Nbr_ID*2,PAGE3); //msb first
	User_FLASH_Write((uint16_t)(InMSSV&0x0000FFFF),Nbr_ID*2+1,PAGE3); //lsb after
	//Updata new Nbr_ID
}

int8_t Search_MSSV (uint32_t InMSSV) {
	uint8_t i;
	for(i=0; i<Nbr_ID; i++) {
		//                         MSB                                                    LSB
		if( ((((uint32_t)User_FLASH_Read(Nbr_ID*2,PAGE3))<<16) | ((uint32_t)User_FLASH_Read(Nbr_ID*2+1,PAGE3))) == InMSSV) {
			return i;
		}
	}
	return -1;
}

uint32_t Pick_MSSV (uint8_t index){
	if(index >= Nbr_ID) {
		User_USART2_SendSchar("\nChi so khong hop le\n");
		return NULL;
	}
	return ((((uint32_t)User_FLASH_Read(index*2,PAGE3))<<16) | ((uint32_t)User_FLASH_Read(index*2+1,PAGE3)));
}

void Remove_MSSV (uint8_t index) {
	
	uint8_t i, j;
	if(index>=Nbr_ID) return;
	if(Nbr_ID==1 && index==0) {
		User_FLASH_Erase(PAGE3);
		return;
	}
	//backup data from page 3 to page 4 first
	User_FLASH_Erase(PAGE4);
	/*
	-	i: index of number of MSSV
	- j: index of MSSVi
	*/
	for(i=0; i<Nbr_ID; i++) {
		for(j=0; j<2; j++) {
			User_FLASH_Write(User_FLASH_Read(i*2+j, PAGE3), i*2+j, PAGE4); //copy data from page 3 to page 4
		}
	}
	
	//start write old data from page 4 to page 3 BUT ignore MSSV index
	User_FLASH_Erase(PAGE3);
	//old data
	for(i=0; i<Nbr_ID-1; i++) {
		//if MSSV[index] found
		if(i==index) {
			for(i=i+1; i<Nbr_ID; i++) {
				for(j=0; j<2; j++) {
					User_FLASH_Write(User_FLASH_Read(i*2+j, PAGE4), (i-1)*2+j, PAGE3);
				}
			}
			break;
		}
		//if not found
		for(j=0; j<2; j++) {
			User_FLASH_Write(User_FLASH_Read(i*2+j, PAGE4), i*2+j, PAGE3);
		}
	}
	
	/*-------------update nbr_id to flash (page0)---------------*/
}

void Add_NewMem(uint8_t* InID,uint8_t* InName,uint32_t InMSSV) {
	if(Nbr_ID>=50) {
		User_USART2_SendSchar("\nDa du so luong luong, khong the add them\n");
		return;
	}
	Add_ID(InID);
	Add_Name(InName);
	Add_MSSV(InMSSV);
	//update new Nbr_ID
	Nbr_ID++;
	Write_Page0(Emer_flag,Nbr_ID,OneTouch,password);
}

void Remove_Mem(uint8_t index) {
	if(index>=Nbr_ID) {
		User_USART2_SendSchar("\nChi so khong hop le!\n");
		return;
	}
	Remove_ID(index);
	Remove_Name(index);
	Remove_MSSV(index);
	Nbr_ID--;
	Write_Page0(Emer_flag,Nbr_ID,OneTouch,password);
}

void Display_ID(uint8_t index) {
	if(index>=Nbr_ID) {
		User_USART2_SendSchar ("\nchi so khong hop le!\n");
		return;
	}
	sprintf(TempBuff,"[%d].",index);
	User_USART2_SendSchar(TempBuff);
	uint8_t* temp=NULL;
	User_USART2_SendSchar("\nMa the:");
	temp=Pick_ID(index);
	sprintf(TempBuff,"[%02X,%02X,%02X,%02X,%02X]\n",temp[0],temp[1],temp[2],temp[3],temp[4]);
	User_USART2_SendSchar(TempBuff);
	sprintf(TempBuff,"Ten: %s\n",ConvertUint8ToChar(Pick_Name(index)));
	User_USART2_SendSchar(TempBuff);
	sprintf(TempBuff,"MSSV: %d\n",Pick_MSSV(index));
	User_USART2_SendSchar(TempBuff);
}

void CloseDoor(void) {
	Turn_lock(CLOSE);
	Turn_led_close(ON);
	Turn_led_open(OFF);
	DelayMs(500);
	Turn_buzz(ON);
	DelayMs(500);
	Turn_buzz(OFF);
	User_USART2_SendSchar("\nClosed door\n");
	DelayMs(1000);
}
void OpenDoor(void){
	Turn_lock(OPEN);
	Door_status=OPEN;
	Turn_buzz(ON);
	DelayMs(300);
	Turn_buzz(OFF);
	DelayMs(300);
	Turn_buzz(ON);
	DelayMs(300);
	Turn_buzz(OFF);
	Turn_led_close(OFF);
	Turn_led_open(ON);
	User_USART2_SendSchar("\nOpened door\n");
	DelayMs(4000);
	CloseDoor();
}

void Turn_lock(uint8_t status) {
	if(status==CLOSE) {
		SC_bit(GPIOB,LOCK_Pin,SET);
	}
	else {
		SC_bit(GPIOB,LOCK_Pin,CLEAR);
	}
}

void Turn_led_close(uint8_t status){
	if(status==ON) {
		SC_bit(GPIOA,LED_Close_Pin,CLEAR);
	}
	else {
		SC_bit(GPIOA,LED_Close_Pin,SET);
	}
}


void Turn_led_open(uint8_t status){
	if(status==ON) {
		SC_bit(GPIOA,LED_Open_Pin,CLEAR);
	}
	else {
		SC_bit(GPIOA,LED_Open_Pin,SET);
	}
}


void Turn_buzz(uint8_t status){
	if(status==OFF) {
		SC_bit(GPIOA,BUZZ_Pin,CLEAR);
	}
	else {
		SC_bit(GPIOA,BUZZ_Pin,SET);
	}
}

void ResetArr(uint16_t* arr, uint8_t leng) {
	int i;
	for(i=0;i<30; i++) {
		arr[i]=0x0000;
	}
}

uint8_t ComparePassword(uint16_t* InputPass) {
	int i;
	for (i=0; i<31; i++) {
		if(InputPass[i]!=password[i]) {
			return FALSE;
		}
	}
	return TRUE;
}

uint8_t RequirePassword() {
	User_USART2_SendSchar("\nVui long nhap mat khau va ket thuc bang dau '.'\n");
	if (ComparePassword(User_USART2_ReceiveString(TempBuff,'.',30))) {
		User_USART2_SendSchar("\nMat khau dung\n");
		ResetArr((uint16_t*)TempBuff,50);
		return TRUE;
	}
	else {
		User_USART2_SendSchar("\nMat khau sai\n");
		ResetArr((uint16_t*)TempBuff,50);
		return FALSE;
	}
}

uint8_t CheckAlert (void) {
	if (Emer_flag) return Emer_flag;
	if((Door_status==CLOSE) && (CheckDoorStatus()==OPEN)) { //if door is close but switch indicate open
		uint8_t i=0;
		for(i=0; i<5; i++) {
			DelayMs(100);
			if (!((Door_status==CLOSE) && (CheckDoorStatus()==OPEN))) { //noisy cause
				return Emer_flag;
			}
		}
		Emer_flag=TRUE;
		Write_Page0(Emer_flag,Nbr_ID,OneTouch,password);
	}
	return Emer_flag;
}

uint8_t CheckDoorStatus(void) {
	if(!Read_status(GPIOB,SWITCH_Pin)) {
		return CLOSE;
	}
	else
		return OPEN;
}

uint8_t CheckScanedID(uint8_t* IDCheck) {
	if(Search_ID(IDCheck)==ID_NOT_FOUND)
		return FALSE;
	else 
		return TRUE;
}

void DisplayMenu(void) {
	User_USART2_SendSchar("\nm: Hien thi menu\n");
	User_USART2_SendSchar("d: Mo cua\n");
	User_USART2_SendSchar("a: Them thanh vien\n");
	User_USART2_SendSchar("r: Loai bo thanh vien\n");
	User_USART2_SendSchar("p: Doi password\n");
	User_USART2_SendSchar("o: Che do mot cham\n");
	User_USART2_SendSchar("x: Tat bao dong\n");
}

void Add_Mem_Procedure(void) {
	uint8_t i;
	uint8_t NameR[10];
	char MSSVR[12]; //will be turned into uint32_t
	uint16_t* ReceiveTemp=NULL;
	User_USART2_SendSchar("\nAdd memeber\n");
	if(RequirePassword()){
		IWDG_ReloadCounter();
		User_USART2_SendSchar("\nVui long dua the lai gan may quet\n");
		while(TM_MFRC522_Check(ScanedID) == MI_ERR);
		User_USART2_SendSchar("\nDa nhan duoc the\n");
		if(CheckScanedID(ScanedID)) {//ID ton tai
			User_USART2_SendSchar("\nID da ton tai\n");
			return;
		}
		//====================receive & assign for Name=============
		IWDG_ReloadCounter();
		User_USART2_SendSchar("\nNhap ten(ket thuc bang dau '.'): ");
		ReceiveTemp=User_USART2_ReceiveString2('.',9);
		for(i=0; i<10; i++) {
			NameR[i]=ReceiveTemp[i];
		}
		//====================receive & assign for MSSV=============
		IWDG_ReloadCounter();
		User_USART2_SendSchar("\nNhap MSSV(ket thuc bang dau '.'): ");
		ReceiveTemp=User_USART2_ReceiveString2('.',10);
		for(i=0; i<10; i++) {
			MSSVR[i]=ReceiveTemp[i];
		}
		Add_NewMem(ScanedID,NameR,atoi(MSSVR));
		User_USART2_SendSchar("\nThem thanh vien thanh cong\n");
	}
}

void Remove_Mem_Procedure(void) {
	if(Nbr_ID<=0) {
		User_USART2_SendSchar("\nSo luong thanh vien hien la 0\n");
		return;
	}
	uint8_t i;
	User_USART2_SendSchar("\nRemove memeber\n");
	if(RequirePassword()){
		IWDG_ReloadCounter();
		for(i=0; i<Nbr_ID; i++) {
			Display_ID(i);
		}
		User_USART2_SendSchar("\nChon thanh vien can loai bo (ket thuc bang dau '.'): ");
		do {
			User_USART2_ReceiveString(TempBuff,'.',10);
		} while(atoi(TempBuff)<0 || atoi(TempBuff)>=Nbr_ID);
		Remove_Mem(atoi(TempBuff));
		User_USART2_SendSchar("\nDa loai bo thanh vien\n");
	}
}

uint8_t CompareUintChar(uint16_t* s1, char* s2, uint8_t leng) {
	uint8_t i;
	for (i=0 ; i<leng ; i++) {
		if(s1[i]!=s2[i])
			return FALSE;
	}
	return TRUE;
}

void ChangePass(void) {
	User_USART2_SendSchar("\nChange password\nnhap lai mat khau cu: ");
	if(RequirePassword()) { //require old pass
		IWDG_ReloadCounter();
		ResetArr((uint16_t*)TempBuff,31);
		User_USART2_SendSchar("\nNhap mat khau moi (ket thuc bang dau '.'): ");
		User_USART2_ReceiveString(TempBuff,'.',30);
		User_USART2_SendChar('\n');
		User_USART2_SendSchar("\nNhap lai mat khau moi (ket thuc bang dau '.'): ");
		IWDG_ReloadCounter();
		if(CompareUintChar(User_USART2_ReceiveString2('.',30),TempBuff,30)) {
			User_USART2_SendSchar("\nMat khau trung khop, mat khau moi la:\n");
			User_USART2_SendSchar(TempBuff); //display new password
			Write_Page0(Emer_flag,Nbr_ID,OneTouch,ConvertCharToUint16(TempBuff)); //write password to flash
			Updata_Data_From_PAGE0(); //updata new password from flash
			return;
		}
		else {
			User_USART2_SendSchar("\nMat khau xac nhan khong dung:\n");
			return;
		}
	}
	else {
		User_USART2_SendSchar("Sai mat khau, truy cap bi tu choi\n");
		return;
	}
}

void TurnOneTouchMode(void) {
	User_USART2_SendSchar("\nBat che do mot cham:\n");
	if(RequirePassword()) {
		IWDG_ReloadCounter();
		User_USART2_SendSchar("\nON: de bat\nOFF: de tat\n");
		ResetArr((uint16_t*)TempBuff,31);
		User_USART2_ReceiveString(TempBuff,'.',4);
		IWDG_ReloadCounter();
		if(strcmp(TempBuff,"ON")==0) {
			OneTouch=ON;
			Write_Page0(Emer_flag,Nbr_ID,OneTouch,password);
			User_USART2_SendSchar("\nTurn ON\n");
			return;
		}
		else if(strcmp(TempBuff,"OFF")==0) {
			OneTouch=OFF;
			Write_Page0(Emer_flag,Nbr_ID,OneTouch,password);
			User_USART2_SendSchar("\nTurn OFF\n");
			return;
		}
		else {
			User_USART2_SendSchar("\nNhap khong dung\n");
			return;
		}
	}
	else {
		User_USART2_SendSchar("\nSai mat khau\n");
	}
}

//for interrupt receive usart2==============
void USART2_IRQHandler(void){
	//get data when ever receive interrupt excuse and receive_flag for receive string
	if((USART_GetITStatus(User_USART2, USART_IT_RXNE) != RESET) && receive_flag) {
		buff[buff_pos++]=User_USART2_ReceiveChar();
	}
	
	if(USART_GetITStatus(User_USART2, USART_IT_RXNE) != RESET && !receive_flag) {
		if (!Enter_pass_remove_alert && !Emer_flag) {
			switch(User_USART2_ReceiveChar()) {
				case 'm': //display option menu
					DisplayMenu();
					break;
				
				case 'a': //add new member
					AddMember_flag=TRUE;
					break;
				
				case 'r': //remove mem
					RemoveMember_flag=TRUE;
					break;
				
				case 'p': //change new pass word
					ChangePassword_flag=TRUE;
					break;
				
				case 'o': //change one touch mode
					OneTouchMode_flag=TRUE;
					break;
				case 'd': //opendoor from usart
					OpenDoorUSART_flag=TRUE;
					break;
			}
		} else {
			if (User_USART2_ReceiveChar()=='x') {
				Enter_pass_remove_alert=TRUE;
			}
		}
	}
}
