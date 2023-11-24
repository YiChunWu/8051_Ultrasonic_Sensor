#include<string.h>
#define RS P1_0
#define RW P1_1
#define E  P1_2
/*      延遲副程式    */
void delay_10us(unsigned int time) // 約延遲10us 
  {while(time>0) time--;}
/**** 寫入指令 副程式 ****/
void write_c(char lcd_data)
  { RS=0;RW=0;E=1;
    P0=lcd_data;
    delay_10us(100);
    E=0;delay_10us(100);
  }
/**** 寫入資料 副程式 ****/
void write_d(char lcd_data)
  { RS=1;RW=0;E=1;
    P0=lcd_data;
    delay_10us(100);
    E=0;delay_10us(100);
  }
/**** LCM初始化 副程式 ****/
void init(void)
  { delay_10us(2000);//延遲20ms
    write_c(0x30);	 //設定功能 
    write_c(0x30);	 //設定功能 
		write_c(0x38);	 //設定功能 
    write_c(0x08);  // 顯示器 OFF 
    write_c(0x01);  // 清除顯示器 
    write_c(0x06);  // 輸入模式設定 
    write_c(0x0c);  // 顯示器 ON
  }
/**** 顯示位置 副程式 ****/
void disp_p(char row,word)
  { char change_p;
    row--;word--;
    change_p=0x80+(row*0x40+word);
    write_c(change_p);
  } 
/**** 顯示字串 副程式 ****/
void disp_s(char *ptr)
  { 
		char len=strlen(ptr);
		char i;
		for(i=0;i<len;i++){
			write_d(ptr[i]);
		}
// 		while(*ptr!=0)
//       { write_d(*ptr);
//         ptr++;
//       }
  }