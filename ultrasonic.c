//
// mode=0: UART
// mode=1: 脈衝
#include<regx52.h> 
#include<LCD.h> 
#define cnt 90 
#define cnt1 50000
sbit TX=P3^1;
sbit RX=P3^2;
struct raw{
		unsigned char first : 8; //高位元組 
		unsigned char last : 8;  //低位元組     
} ;
union myuint{
    unsigned int i;
    struct raw r;
} ;

code char string1[]={ "Distance:" };
code char string3[]={ "Mode: UART" };
code char string4[]={ "Mode: PulseWidth" };
char uart[]={0,1,0,1,0,1,0,1,0,1};
char string2[]={ "00000mm" }; 
union myuint distance;
unsigned char pos,j,r;
char num1,mode;


void main(void)
{
	init();		// LCD初始化 		
	disp_p(1,1); 
	disp_s(string1);
	disp_p(2,1); 
	disp_s(string3);
	
	num1=0;
	mode=0;   // 預設為UART模式
	pos=0;
	distance.i=0;
	TX=1;
	
	TMOD=0x11;// Timer0、Timer1都為mode1(16bit timer)
	PX1=1;    // 設定外部中斷1為最優先
	TR1=1;    // 啟動Timer1
	TR0=0;    // 關閉Timer0
	IT0=0;    // 設定為低態觸發
	ET0=1;	  // 開啟Timer 0 中斷 
	ET1=1;    // 開啟Timer 1 中斷
	EX1=1;    // 開啟外部中斷1
	EA=1;     // 開啟所有的中斷
  
	while(1) ;   
}

// 利用外部中斷0計算感測器回傳的值
void ext0 (void) interrupt 0
{		
	if(mode==0){
		delay_10us(6);     // 延遲60微秒
		for(j=0;j<8;j++){  // 高位元組 
			delay_10us(4);   // 延遲40微秒
			r=(RX==0)?0:1;   // 判斷腳位電壓
			distance.r.first|= r<<j; // LSB to MSB
		}
		delay_10us(28);    // 延遲280微秒
		for(j=0;j<8;j++){  // 低位元組
			delay_10us(4);
			r=(RX==0)?0:1;
			distance.r.last|= r<<j;  // LSB to MSB
		}
		string2[0]=distance.i/10000+'0';
		distance.i%=10000;
		string2[1]=distance.i/1000+'0';
		distance.i%=1000;
		string2[2]=distance.i/100+'0';
		distance.i%=100;
		string2[3]=distance.i/10+'0';
		string2[4]=distance.i%10+'0';
		disp_p(1,10); 
		disp_s(string2);  // 顯示障礙物距離(mm)
		distance.i=0;
		pos=0;
		EX0=0;
	}
	else{
		distance.r.first=TH0;
		distance.r.last=TL0;
		TL0=TH0=0;
		distance.i=distance.i*0.17; //實際距離(mm)=timer時間(us)*0.000001(s/us)*音速340(m/s)*1000(mm/m)/2=timer時間*0.17
		string2[0]=distance.i/10000+'0';
		distance.i%=10000;
		string2[1]=distance.i/1000+'0';
		distance.i%=1000;
		string2[2]=distance.i/100+'0';
		distance.i%=100;
		string2[3]=distance.i/10+'0';
		string2[4]=distance.i%10+'0';
		disp_p(1,10); 
		disp_s(string2); // 顯示障礙物距離(mm)
		EX0=0;           // 關閉外部中斷0
	}
}

// 利用外部中斷1切換模式
void ext1 (void) interrupt 2
{		
	delay_10us(20000); // 延遲0.2秒防按鈕開關彈跳
	if(mode==0){
		mode=1;    // 切換為脈衝模式
		TX=0;
		EX0=0;
		TMOD=0x19; // Timer0、Timer1都為mode1(16bit timer),Timer0為外部開啟,Timer1為內部開啟
		TL0=TH0=0; // 將計時暫存器歸0
		
		TR0=1;     // 啟動Timer0
		write_c(0x01);   // 清除LCD顯示內容		
		disp_p(1,1); 
		disp_s(string1);
		disp_p(2,1); 
		disp_s(string4); // 顯示當前模式
	}
	else{
		mode=0;    // 切換為UART模式
		TR0=0;     // 關閉Timer0
		TMOD=0x11;
		write_c(0x01);	 // 清除LCD顯示內容		
		disp_p(1,1); 
		disp_s(string1);
		disp_p(2,1); 
		disp_s(string3); // 顯示當前模式
	}
}	


void timer0 (void) interrupt 1
{ 
	// 在UART模式下利用Timer0以9600鮑率傳送0x55
	if(mode==0){
		TH0=(65536-cnt)/256;
		TL0=(65536-cnt)%256;
		if(pos<9){ 
			TX=uart[pos];
			pos++;	
		}
		else{  // 重送完畢
			TX=1;
			TR0=0;	 // 關閉Timer0
			EX0=1;	 // 開啟外部中斷0	
		}	
	}
}

// 利用Timer1啟動Timer0傳送訊號給感測器
void timer1 (void) interrupt 3
{ TH1=(65536-cnt1)/256;
	TL1=(65536-cnt1)%256; 
	num1++;			    
	if(num1>=10){ // 每0.5秒執行一次
		if(mode==0){
			num1=0;		
			pos=0;
			TR0=1;	  // 啟動Timer0
			TX=1;     // 設定TX為高電位準備傳送UART訊號
		}
		else{
			num1=0;
			TX=1;
			delay_10us(2); // 傳送20微秒高電位
			TX=0;
			EX0=1;    // 開啟外部中斷0
			TR0=1;	  // 啟動Timer0
		}
	}
}
