#include "reg52.h"			 //此文件中定义了单片机的一些特殊功能寄存器

typedef unsigned int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;
 //数码管控制端口
sbit LSA=P2^2;       
sbit LSB=P2^1;
sbit LSC=P2^3;
sbit LSD=P2^4;

sbit k2=P3^2;//按键控制
sbit k3=P3^3;



sbit led1=P1^0;//南北 绿 
sbit led2=P1^1;//南北 黄 
sbit led3=P1^2; //南北 红


sbit led4=P1^3;//东西  绿 
sbit led5=P1^4;//东西 黄
sbit led6=P1^5; //东西 红



u8 code smgduan[17]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
					0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};//显示0~F的值

u8 sum2,sum1=0;
u16 js;//计数变量
u8 aa,bb;     //中间变量
u8 table=0;   //切换标志位
u8 nbshi,dxshi,moshi; //时间中间标记
u8 nbshi1,dxshi1,moshi1; //时间中间标记1
u8 huangshi=3;//黄灯时间
u8 nbzlshi=25;    //南北各灯时间 直行    绿灯				
u8 nbzhshi=30;  //南北各灯时间 直行   红灯
		

					
u8 dxzlshi=25;   //东西各灯时间	直行  绿灯			
u8 dxzhshi=30;  //东西各灯时间	直行  红灯


u8 dxhshi,nbhshi;  //南北 东西红灯时间	中间变量

u8 INT1_1,INT1_0;	//中断标志								
/*******************************************************************************
* 函 数 名         : delay
* 函数功能		   : 延时函数，i=1时，大约延时10us
*******************************************************************************/
void delay(u16 i)
{
	while(i--);	
}



/*******************************************************************************
* 函 数 名         : Timer0Init
* 函数功能		   : 定时器0初始化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void Timer0Init()
{
	TMOD|=0X01;//选择为定时器0模式，工作方式1，仅用TR0打开启动。

	TH0=0XFC;	//给定时器赋初值，定时1ms
	TL0=0X18;	
	ET0=1;//打开定时器0中断允许
	EA=1;//打开总中断
	TR0=1;//打开定时器			
}

void hlhdeng(u8 deng)
{
  switch(deng)
			{
				case 0x00:         //东西直行 绿灯
					if(sum1==0)
          led1=1; // 绿     // 南北 灯         
					led2=1; //黄
					led3=0; //红	
	        if(sum1==0)
          led4=0; // 绿       //东西 灯   
					led5=1; // 黄 
					led6=1; //红
	    
	      
						
				break;
				case 0x01:            //东西直行 黄灯
						if(sum1==0)
					 led1=1; // 绿     // 南北 灯         
					led2=1; //黄
					led3=0; //红	
	        if(sum1==0)
          led4=1; // 绿       //东西 灯   
					led5=0; // 黄 
					led6=1; //红
	    
	      
				break;
				case 0x02:            // 南北 直行 绿
					if(sum1==0)
					led1=0; // 绿     // 南北 灯         
					led2=1; //黄
					led3=1; //红	
	        if(sum1==0)
          led4=1; // 绿       //东西 灯   
					led5=1; // 黄 
					led6=0; //红
				break;
					case 0x03:            // 南北 直行 黄
						if(sum1==0)
					led1=1; // 绿     // 南北 灯         
					led2=0; //黄
					led3=1; //红	
	         if(sum1==0)
          led4=1; // 绿       //东西 灯   
					led5=1; // 黄 
					led6=0; //红
				break;
			
			}
}
/*******************************************************************************
* 函 数 名         : DigDisplay
* 函数功能		   : 数码管动态扫描函数，循环扫描8个数码管显示
*******************************************************************************/

void smg(u8 nbshi,u8 dxshi)
{
	 LSA=1; LSB=1;LSC=0; LSD=1;  
	       
		aa=nbshi/10;
		P0=smgduan[aa];
		delay(50);
	 LSA=1; LSB=1;LSC=1; LSD=0; 
	            
		bb=nbshi%10;
		P0=smgduan[bb];
		delay(50);
		
	   LSA=0; LSB=1;LSC=1; LSD=1; 
		aa=dxshi/10;
		P0=smgduan[aa];
		delay(50);
	
	    LSA=1; LSB=0;LSC=1; LSD=1;               
		bb=dxshi%10;
		P0=smgduan[bb];
		delay(50);

}

void nbzh(u8 nbshi,u8 dxshi,u8 moshi)      //南北直行 黄
{
	hlhdeng(moshi);   //4种

  smg(nbshi,dxshi); //南北 东西 

}
/*******************************************************************************
* 函 数 名         : Int1Init()
* 函数功能		   : 设置外部中断1
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void Int0Init()
{
	//设置INT0
	IT0=1;//跳变沿出发方式（下降沿）
	EX0=1;//打开INT0的中断允许。	
	EA=1;//打开总中断	
}
/*******************************************************************************
* 函 数 名         : Int1Init()
* 函数功能		   : 设置外部中断1
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void Int1Init1()
{
	//设置INT1
	IT1=1;//跳变沿出发方式（下降沿）
	EX1=1;//打开INT1的中断允许。	
	EA=1;//打开总中断	
}

/*******************************************************************************
* 函 数 名       : main
* 函数功能		 : 主函数
* 输    入       : 无
* 输    出    	 : 无
*******************************************************************************/
void main()
{	
	Timer0Init();//定时器中断初始化
	 Int0Init(); // 外部中断0初始化
   Int1Init1();// 外部中断1初始化
	nbshi=nbzhshi;  //南北红灯时间
	dxshi=dxzlshi;  //东西 绿灯  
	while(1)
	{
			if(INT1_0)//中断0
		{
		  nbzh(nbshi1,dxshi1,moshi1);	
		}else
		if(INT1_1)//中断1
		{
		 nbzh(nbshi1,dxshi1,moshi1);	
		}else//正常模式
		{
	  	nbzh(nbshi,dxshi,moshi);	
		}
   	 if(sum1)
			{
				js++;
				if(js>=200)
				{
					js=0;
					if(moshi==0)	//东西直行 绿灯
			    led4=!led4;
				  if(moshi==2)	//南北 绿
					 led1=!led1; 
				   sum2++;
				if(sum2>=6){sum1=0;}
			 }
			}
	}					
}


/*******************************************************************************
* 函 数 名         : void Timer0() interrupt 1
* 函数功能		   : 定时器0中断函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void Timer0() interrupt 1
{
	static u16 i,j;

	TH0=0XFC;	//给定时器赋初值，定时1ms
	TL0=0X18;

	if(INT1_0)//中断0
		{
		  	i++;
			if(i>=1000)//5S
			{
				i=0;
			  if(dxshi1)
				{
				  nbshi1--;
	        dxshi1--;
				}else
				{
				  INT1_0=0;
				}
			}
		}else
		if(INT1_1)//中断1
		{
		  	i++;
			if(i>=1000)//10S
			{
			  i=0;
			  if(dxshi1)
				{
				  nbshi1--;
	        dxshi1--;
				}else
				{
				  INT1_1=0;
				}
			}
		}else//正常模式
		{
				i++;
			if(i==1000)                       //1s进入一次
			{
				 i=0;	    
								if(moshi==0)	//东西直行 绿灯
								{
									if(j==0) //第一次进入
									{
										j=1;
										nbshi=nbzhshi;  //南北红灯时间
										dxshi=dxzlshi;  //东西 绿灯  
									}else
									{
										if(dxshi)
										{
											nbshi--;
											dxshi--;
										}else
										{
											if(sum2==0)
											sum1=1;
											if(sum1==0)
											{
												moshi++;
												sum2=0;
													nbshi--;
												dxshi=huangshi;//东西黄灯时间  
											}
										}
									}
								}
								else
								 if(moshi==1)	//东西 黄
								{
								
									if(dxshi>1)
									{
										nbshi--;
										dxshi--;
									}else
									{
										moshi++;
										dxshi--;
										nbshi=nbzlshi;  //南北 绿灯时间
										dxshi=dxzhshi;  //东西红灯 
									}
								}else
								 if(moshi==2)	//南北 绿
								{
								 
									 if(nbshi)
									{
										nbshi--;
										 dxshi--;
									}else
									{
										
										if(sum2==0)
											sum1=1;
											if(sum1==0)
											{
													moshi++;
												dxshi--;
												sum2=0;
												nbshi=huangshi;  
											}
									}
								}else
								 if(moshi==3)	//南北 黄
								{
									 if(nbshi>1)
									{
										 nbshi--;
										 dxshi--;
									}else
									{
										moshi=0;
										dxshi--;
											j=0;
										nbshi=nbzhshi;  //南北红灯时间
										dxshi=dxzlshi;  //东西 绿灯  
									}
								}
								
					}	
	      }	
}
/*******************************************************************************
* 函 数 名         : Int0()	interrupt 0
* 函数功能		   : 外部中断0的中断函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/

void Int0()	interrupt 0		//外部中断0的中断函数
{
	delay(1000);	 //延时消抖
	if(k2==0)
	{
	 INT1_0=1;
		nbshi1=5;  //南北红灯时间
    dxshi1=5;  //东西 绿灯  
		moshi1=0;
	}
}
/*******************************************************************************
* 函 数 名         : Int1()	interrupt 2
* 函数功能		   : 外部中断0的中断函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/

void Int1()	interrupt 2		//外部中断1的中断函数
{
	delay(1000);	 //延时消抖
	if(k3==0)
	{
		INT1_1=1;
			nbshi1=5;  //南北红灯时间
    dxshi1=5;  //东西 绿灯 
	 moshi1=2;		
	}
}
