   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Parser V4.8.32 - 23 Mar 2010
   3                     ; Generator V4.3.4 - 23 Mar 2010
  79                     ; 4 main()
  79                     ; 5 {
  81                     	switch	.text
  82  0000               _main:
  84  0000 5203          	subw	sp,#3
  85       00000003      OFST:	set	3
  88                     ; 9 	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);//配置时钟，使用内部16M高速时钟，不分频
  90  0002 4f            	clr	a
  91  0003 cd0000        	call	_CLK_HSIPrescalerConfig
  93                     ; 11 	SEG_GPIO_Init();
  95  0006 cd02d0        	call	_SEG_GPIO_Init
  97                     ; 12 	MY_ADC1_Init();
  99  0009 cd024c        	call	_MY_ADC1_Init
 101                     ; 14 	Display_Open();//开机显示888
 103  000c cd0675        	call	_Display_Open
 105  000f               L73:
 106                     ; 18 		ADC_vol=ADC1_GetValue_Average();		//获取ADC转换结果
 108  000f cd0291        	call	_ADC1_GetValue_Average
 110  0012 1f01          	ldw	(OFST-2,sp),x
 111                     ; 20 		TempVol_X10=GetTemperatureValue_X10(ADC_vol);//
 113  0014 1e01          	ldw	x,(OFST-2,sp)
 114  0016 ad37          	call	_GetTemperatureValue_X10
 116  0018 1f01          	ldw	(OFST-2,sp),x
 117                     ; 22 		for(count=0;count<50;count++)//避免刷新过快，进行延时
 119  001a 0f03          	clr	(OFST+0,sp)
 120  001c               L34:
 121                     ; 24 			Display_Temperature(TempVol_X10);//显示温度值
 123  001c 1e01          	ldw	x,(OFST-2,sp)
 124  001e cd06a0        	call	_Display_Temperature
 126                     ; 22 		for(count=0;count<50;count++)//避免刷新过快，进行延时
 128  0021 0c03          	inc	(OFST+0,sp)
 131  0023 7b03          	ld	a,(OFST+0,sp)
 132  0025 a132          	cp	a,#50
 133  0027 25f3          	jrult	L34
 134                     ; 26 		count=0;//重新计数
 137  0029 20e4          	jra	L73
 189                     ; 30 void delay_ms(uint nms)//ms延时函数
 189                     ; 31 {												
 190                     	switch	.text
 191  002b               _delay_ms:
 193  002b 5204          	subw	sp,#4
 194       00000004      OFST:	set	4
 197                     ; 33 	for(x=nms;x>0;x--)
 199  002d 1f01          	ldw	(OFST-3,sp),x
 201  002f 2017          	jra	L301
 202  0031               L77:
 203                     ; 34 		for(y=1500;y>0;y--);
 205  0031 ae05dc        	ldw	x,#1500
 206  0034 1f03          	ldw	(OFST-1,sp),x
 207  0036               L701:
 211  0036 1e03          	ldw	x,(OFST-1,sp)
 212  0038 1d0001        	subw	x,#1
 213  003b 1f03          	ldw	(OFST-1,sp),x
 216  003d 1e03          	ldw	x,(OFST-1,sp)
 217  003f 26f5          	jrne	L701
 218                     ; 33 	for(x=nms;x>0;x--)
 220  0041 1e01          	ldw	x,(OFST-3,sp)
 221  0043 1d0001        	subw	x,#1
 222  0046 1f01          	ldw	(OFST-3,sp),x
 223  0048               L301:
 226  0048 1e01          	ldw	x,(OFST-3,sp)
 227  004a 26e5          	jrne	L77
 228                     ; 35 }
 231  004c 5b04          	addw	sp,#4
 232  004e 81            	ret
 285                     ; 37 int GetTemperatureValue_X10(uint advol)//将AD值转换成相应温度值 的10倍
 285                     ; 38 {																			//10倍便于数据处理	
 286                     	switch	.text
 287  004f               _GetTemperatureValue_X10:
 289  004f 89            	pushw	x
 290  0050 5203          	subw	sp,#3
 291       00000003      OFST:	set	3
 294                     ; 42 	if((advol>=AD_MIN)&&(advol<=AD_MAX))//判断是否超出检测范围
 296  0052 a30027        	cpw	x,#39
 297  0055 2403          	jruge	L41
 298  0057 cc0246        	jp	L341
 299  005a               L41:
 301  005a a3035b        	cpw	x,#859
 302  005d 2503          	jrult	L61
 303  005f cc0246        	jp	L341
 304  0062               L61:
 305                     ; 44 		if(advol>778&&advol<=858)       //-10~0，由于非线性，分11段处理，每段跨10度, 
 307  0062 a3030b        	cpw	x,#779
 308  0065 2520          	jrult	L541
 310  0067 a3035b        	cpw	x,#859
 311  006a 241b          	jruge	L541
 312                     ; 45 			T_vol=(-100)+8+(int)((858-advol)*1.25);//乘10处理
 314  006c ae035a        	ldw	x,#858
 315  006f 72f004        	subw	x,(OFST+1,sp)
 316  0072 cd0000        	call	c_uitof
 318  0075 ae0030        	ldw	x,#L351
 319  0078 cd0000        	call	c_fmul
 321  007b cd0000        	call	c_ftoi
 323  007e 1cffa4        	addw	x,#65444
 324  0081 1f02          	ldw	(OFST-1,sp),x
 326  0083 ac420242      	jpf	L751
 327  0087               L541:
 328                     ; 46 		else if(advol>678&&advol<=778) //0~10
 330  0087 1e04          	ldw	x,(OFST+1,sp)
 331  0089 a302a7        	cpw	x,#679
 332  008c 2513          	jrult	L161
 334  008e 1e04          	ldw	x,(OFST+1,sp)
 335  0090 a3030b        	cpw	x,#779
 336  0093 240c          	jruge	L161
 337                     ; 47 			T_vol=0+8+778-advol;
 339  0095 ae0312        	ldw	x,#786
 340  0098 72f004        	subw	x,(OFST+1,sp)
 341  009b 1f02          	ldw	(OFST-1,sp),x
 343  009d ac420242      	jpf	L751
 344  00a1               L161:
 345                     ; 48 		else if(advol>457&&advol<=678) //10~20度,20~30度因为相似，合并
 347  00a1 1e04          	ldw	x,(OFST+1,sp)
 348  00a3 a301ca        	cpw	x,#458
 349  00a6 2522          	jrult	L561
 351  00a8 1e04          	ldw	x,(OFST+1,sp)
 352  00aa a302a7        	cpw	x,#679
 353  00ad 241b          	jruge	L561
 354                     ; 49 			T_vol=100+4+(int)((678-advol)*0.91);
 356  00af ae02a6        	ldw	x,#678
 357  00b2 72f004        	subw	x,(OFST+1,sp)
 358  00b5 cd0000        	call	c_uitof
 360  00b8 ae002c        	ldw	x,#L371
 361  00bb cd0000        	call	c_fmul
 363  00be cd0000        	call	c_ftoi
 365  00c1 1c0068        	addw	x,#104
 366  00c4 1f02          	ldw	(OFST-1,sp),x
 368  00c6 ac420242      	jpf	L751
 369  00ca               L561:
 370                     ; 51 		else if(advol>356&&advol<=457)// 30~40
 372  00ca 1e04          	ldw	x,(OFST+1,sp)
 373  00cc a30165        	cpw	x,#357
 374  00cf 2513          	jrult	L102
 376  00d1 1e04          	ldw	x,(OFST+1,sp)
 377  00d3 a301ca        	cpw	x,#458
 378  00d6 240c          	jruge	L102
 379                     ; 52 			T_vol=300+457-advol;
 381  00d8 ae02f5        	ldw	x,#757
 382  00db 72f004        	subw	x,(OFST+1,sp)
 383  00de 1f02          	ldw	(OFST-1,sp),x
 385  00e0 ac420242      	jpf	L751
 386  00e4               L102:
 387                     ; 53 		else if(advol>270&&advol<=356) //40~50
 389  00e4 1e04          	ldw	x,(OFST+1,sp)
 390  00e6 a3010f        	cpw	x,#271
 391  00e9 2522          	jrult	L502
 393  00eb 1e04          	ldw	x,(OFST+1,sp)
 394  00ed a30165        	cpw	x,#357
 395  00f0 241b          	jruge	L502
 396                     ; 54 			T_vol=400+(int)((356-advol)*1.16);//pass
 398  00f2 ae0164        	ldw	x,#356
 399  00f5 72f004        	subw	x,(OFST+1,sp)
 400  00f8 cd0000        	call	c_uitof
 402  00fb ae0028        	ldw	x,#L312
 403  00fe cd0000        	call	c_fmul
 405  0101 cd0000        	call	c_ftoi
 407  0104 1c0190        	addw	x,#400
 408  0107 1f02          	ldw	(OFST-1,sp),x
 410  0109 ac420242      	jpf	L751
 411  010d               L502:
 412                     ; 56 		else if(advol>203&&advol<=270) //50~60
 414  010d 1e04          	ldw	x,(OFST+1,sp)
 415  010f a300cc        	cpw	x,#204
 416  0112 2522          	jrult	L122
 418  0114 1e04          	ldw	x,(OFST+1,sp)
 419  0116 a3010f        	cpw	x,#271
 420  0119 241b          	jruge	L122
 421                     ; 57 			T_vol=500-5+(int)((270-advol)*1.59);
 423  011b ae010e        	ldw	x,#270
 424  011e 72f004        	subw	x,(OFST+1,sp)
 425  0121 cd0000        	call	c_uitof
 427  0124 ae0024        	ldw	x,#L722
 428  0127 cd0000        	call	c_fmul
 430  012a cd0000        	call	c_ftoi
 432  012d 1c01ef        	addw	x,#495
 433  0130 1f02          	ldw	(OFST-1,sp),x
 435  0132 ac420242      	jpf	L751
 436  0136               L122:
 437                     ; 58 		else if(advol>151&&advol<=203) //60~70
 439  0136 1e04          	ldw	x,(OFST+1,sp)
 440  0138 a30098        	cpw	x,#152
 441  013b 2522          	jrult	L532
 443  013d 1e04          	ldw	x,(OFST+1,sp)
 444  013f a300cc        	cpw	x,#204
 445  0142 241b          	jruge	L532
 446                     ; 59 			T_vol=600-10+(int)((203-advol)*1.92);
 448  0144 ae00cb        	ldw	x,#203
 449  0147 72f004        	subw	x,(OFST+1,sp)
 450  014a cd0000        	call	c_uitof
 452  014d ae0020        	ldw	x,#L342
 453  0150 cd0000        	call	c_fmul
 455  0153 cd0000        	call	c_ftoi
 457  0156 1c024e        	addw	x,#590
 458  0159 1f02          	ldw	(OFST-1,sp),x
 460  015b ac420242      	jpf	L751
 461  015f               L532:
 462                     ; 60 		else if(advol>113&&advol<=151) //70~80
 464  015f 1e04          	ldw	x,(OFST+1,sp)
 465  0161 a30072        	cpw	x,#114
 466  0164 2522          	jrult	L152
 468  0166 1e04          	ldw	x,(OFST+1,sp)
 469  0168 a30098        	cpw	x,#152
 470  016b 241b          	jruge	L152
 471                     ; 61 			T_vol=700-15+(int)((151-advol)*2.63);
 473  016d ae0097        	ldw	x,#151
 474  0170 72f004        	subw	x,(OFST+1,sp)
 475  0173 cd0000        	call	c_uitof
 477  0176 ae001c        	ldw	x,#L752
 478  0179 cd0000        	call	c_fmul
 480  017c cd0000        	call	c_ftoi
 482  017f 1c02ad        	addw	x,#685
 483  0182 1f02          	ldw	(OFST-1,sp),x
 485  0184 ac420242      	jpf	L751
 486  0188               L152:
 487                     ; 62 		else if(advol>85&&advol<=113) //80~90
 489  0188 1e04          	ldw	x,(OFST+1,sp)
 490  018a a30056        	cpw	x,#86
 491  018d 2522          	jrult	L562
 493  018f 1e04          	ldw	x,(OFST+1,sp)
 494  0191 a30072        	cpw	x,#114
 495  0194 241b          	jruge	L562
 496                     ; 63 			T_vol=800-20+(int)((113-advol)*3.57);	
 498  0196 ae0071        	ldw	x,#113
 499  0199 72f004        	subw	x,(OFST+1,sp)
 500  019c cd0000        	call	c_uitof
 502  019f ae0018        	ldw	x,#L372
 503  01a2 cd0000        	call	c_fmul
 505  01a5 cd0000        	call	c_ftoi
 507  01a8 1c030c        	addw	x,#780
 508  01ab 1f02          	ldw	(OFST-1,sp),x
 510  01ad ac420242      	jpf	L751
 511  01b1               L562:
 512                     ; 64 		else if(advol>65&&advol<=85) //90~100
 514  01b1 1e04          	ldw	x,(OFST+1,sp)
 515  01b3 a30042        	cpw	x,#66
 516  01b6 251b          	jrult	L103
 518  01b8 1e04          	ldw	x,(OFST+1,sp)
 519  01ba a30056        	cpw	x,#86
 520  01bd 2414          	jruge	L103
 521                     ; 65 			T_vol=900-20+(85-advol)*5;
 523  01bf ae0055        	ldw	x,#85
 524  01c2 72f004        	subw	x,(OFST+1,sp)
 525  01c5 90ae0005      	ldw	y,#5
 526  01c9 cd0000        	call	c_imul
 528  01cc 1c0370        	addw	x,#880
 529  01cf 1f02          	ldw	(OFST-1,sp),x
 531  01d1 206f          	jra	L751
 532  01d3               L103:
 533                     ; 66 		else if(advol>=49&&advol<=65) //pass  大于100度，处理 方法有一点不同
 535  01d3 1e04          	ldw	x,(OFST+1,sp)
 536  01d5 a30031        	cpw	x,#49
 537  01d8 2548          	jrult	L503
 539  01da 1e04          	ldw	x,(OFST+1,sp)
 540  01dc a30042        	cpw	x,#66
 541  01df 2441          	jruge	L503
 542                     ; 68 				T_vol=1000-20+(int)((65-advol)*6.5);
 544  01e1 ae0041        	ldw	x,#65
 545  01e4 72f004        	subw	x,(OFST+1,sp)
 546  01e7 cd0000        	call	c_uitof
 548  01ea ae0014        	ldw	x,#L313
 549  01ed cd0000        	call	c_fmul
 551  01f0 cd0000        	call	c_ftoi
 553  01f3 1c03d4        	addw	x,#980
 554  01f6 1f02          	ldw	(OFST-1,sp),x
 555                     ; 70 				remainder=((int)((65-advol)*6.5))%10;
 557  01f8 ae0041        	ldw	x,#65
 558  01fb 72f004        	subw	x,(OFST+1,sp)
 559  01fe cd0000        	call	c_uitof
 561  0201 ae0014        	ldw	x,#L313
 562  0204 cd0000        	call	c_fmul
 564  0207 cd0000        	call	c_ftoi
 566  020a a60a          	ld	a,#10
 567  020c cd0000        	call	c_smodx
 569  020f 01            	rrwa	x,a
 570  0210 6b01          	ld	(OFST-2,sp),a
 571  0212 02            	rlwa	x,a
 572                     ; 71 				if(remainder>=5) T_vol=T_vol+10; //四舍五入处理
 574  0213 7b01          	ld	a,(OFST-2,sp)
 575  0215 a105          	cp	a,#5
 576  0217 2529          	jrult	L751
 579  0219 1e02          	ldw	x,(OFST-1,sp)
 580  021b 1c000a        	addw	x,#10
 581  021e 1f02          	ldw	(OFST-1,sp),x
 582  0220 2020          	jra	L751
 583  0222               L503:
 584                     ; 73 		else if(advol>=39&&advol<49) //110~120,注意这里的等号
 586  0222 1e04          	ldw	x,(OFST+1,sp)
 587  0224 a30027        	cpw	x,#39
 588  0227 2519          	jrult	L751
 590  0229 1e04          	ldw	x,(OFST+1,sp)
 591  022b a30031        	cpw	x,#49
 592  022e 2412          	jruge	L751
 593                     ; 74 			T_vol=1100-20+(49-advol)*10;  //pass
 595  0230 ae0031        	ldw	x,#49
 596  0233 72f004        	subw	x,(OFST+1,sp)
 597  0236 90ae000a      	ldw	y,#10
 598  023a cd0000        	call	c_imul
 600  023d 1c0438        	addw	x,#1080
 601  0240 1f02          	ldw	(OFST-1,sp),x
 602  0242               L751:
 603                     ; 76 		return T_vol;		//-100 ~ 1200
 605  0242 1e02          	ldw	x,(OFST-1,sp)
 607  0244 2003          	jra	L21
 608  0246               L341:
 609                     ; 79 		return T_OVERFLOW;//超出检测范围时，返回一个检测范围外的温度值，用于数码管显示
 611  0246 ae05dc        	ldw	x,#1500
 613  0249               L21:
 615  0249 5b05          	addw	sp,#5
 616  024b 81            	ret
 643                     ; 84 void MY_ADC1_Init(void)
 643                     ; 85 {
 644                     	switch	.text
 645  024c               _MY_ADC1_Init:
 649                     ; 86 	GPIO_Init(GPIOD,GPIO_PIN_6,GPIO_MODE_IN_FL_NO_IT);//初始化ADC1通道引脚
 651  024c 4b00          	push	#0
 652  024e 4b40          	push	#64
 653  0250 ae500f        	ldw	x,#20495
 654  0253 cd0000        	call	_GPIO_Init
 656  0256 85            	popw	x
 657                     ; 87   ADC1_DeInit();		//ADC1复位
 659  0257 cd0000        	call	_ADC1_DeInit
 661                     ; 88   ADC1_Init(ADC1_CONVERSIONMODE_SINGLE,ADC1_CHANNEL_6, ADC1_PRESSEL_FCPU_D18, //单次转换模式，18分频
 661                     ; 89     ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL3, DISABLE);
 663  025a 4b00          	push	#0
 664  025c 4b03          	push	#3
 665  025e 4b08          	push	#8
 666  0260 4b00          	push	#0
 667  0262 4b00          	push	#0
 668  0264 4b70          	push	#112
 669  0266 ae0006        	ldw	x,#6
 670  0269 4f            	clr	a
 671  026a 95            	ld	xh,a
 672  026b cd0000        	call	_ADC1_Init
 674  026e 5b06          	addw	sp,#6
 675                     ; 90   ADC1_Cmd(ENABLE);//使能ADC1
 677  0270 a601          	ld	a,#1
 678  0272 cd0000        	call	_ADC1_Cmd
 680                     ; 91 }
 683  0275 81            	ret
 721                     ; 93 uint ADC1_GetValue(void)//获取ADC1单次转换的结果
 721                     ; 94 {
 722                     	switch	.text
 723  0276               _ADC1_GetValue:
 725  0276 89            	pushw	x
 726       00000002      OFST:	set	2
 729                     ; 97   ADC1_StartConversion();//启动ADC1转换
 731  0277 cd0000        	call	_ADC1_StartConversion
 734  027a               L753:
 735                     ; 98   while(!(ADC1_GetFlagStatus(ADC1_FLAG_EOC)));//等待ADC1转换结束
 737  027a a680          	ld	a,#128
 738  027c cd0000        	call	_ADC1_GetFlagStatus
 740  027f 4d            	tnz	a
 741  0280 27f8          	jreq	L753
 742                     ; 99 	adc1_vol=ADC1_GetConversionValue(); //得到转换值
 744  0282 cd0000        	call	_ADC1_GetConversionValue
 746  0285 1f01          	ldw	(OFST-1,sp),x
 747                     ; 100   ADC1_ClearFlag(ADC1_FLAG_EOC);//清除转换结束标志位
 749  0287 a680          	ld	a,#128
 750  0289 cd0000        	call	_ADC1_ClearFlag
 752                     ; 102   return adc1_vol;//返回转换结果
 754  028c 1e01          	ldw	x,(OFST-1,sp)
 757  028e 5b02          	addw	sp,#2
 758  0290 81            	ret
 812                     ; 105 uint ADC1_GetValue_Average(void)//连续10次得到ADC1的转换值，再去平均值
 812                     ; 106 {																//提高转换准确度
 813                     	switch	.text
 814  0291               _ADC1_GetValue_Average:
 816  0291 5204          	subw	sp,#4
 817       00000004      OFST:	set	4
 820                     ; 107   uint adc1_vol_average,sum=0,i=0;
 822  0293 5f            	clrw	x
 823  0294 1f01          	ldw	(OFST-3,sp),x
 826                     ; 109   for(i=0;i<10;i++)
 828  0296 5f            	clrw	x
 829  0297 1f03          	ldw	(OFST-1,sp),x
 830  0299               L114:
 831                     ; 111     sum=sum+ADC1_GetValue();//对连续10次转换结果累加
 833  0299 addb          	call	_ADC1_GetValue
 835  029b 72fb01        	addw	x,(OFST-3,sp)
 836  029e 1f01          	ldw	(OFST-3,sp),x
 837                     ; 109   for(i=0;i<10;i++)
 839  02a0 1e03          	ldw	x,(OFST-1,sp)
 840  02a2 1c0001        	addw	x,#1
 841  02a5 1f03          	ldw	(OFST-1,sp),x
 844  02a7 1e03          	ldw	x,(OFST-1,sp)
 845  02a9 a3000a        	cpw	x,#10
 846  02ac 25eb          	jrult	L114
 847                     ; 113   adc1_vol_average=sum/10; //求平均值
 849  02ae 1e01          	ldw	x,(OFST-3,sp)
 850  02b0 90ae000a      	ldw	y,#10
 851  02b4 65            	divw	x,y
 852  02b5 1f03          	ldw	(OFST-1,sp),x
 853                     ; 114 	if((sum%10)>=5) adc1_vol_average++;//四舍五入处理
 855  02b7 1e01          	ldw	x,(OFST-3,sp)
 856  02b9 90ae000a      	ldw	y,#10
 857  02bd 65            	divw	x,y
 858  02be 51            	exgw	x,y
 859  02bf a30005        	cpw	x,#5
 860  02c2 2507          	jrult	L714
 863  02c4 1e03          	ldw	x,(OFST-1,sp)
 864  02c6 1c0001        	addw	x,#1
 865  02c9 1f03          	ldw	(OFST-1,sp),x
 866  02cb               L714:
 867                     ; 116   return adc1_vol_average;//返回平均值
 869  02cb 1e03          	ldw	x,(OFST-1,sp)
 872  02cd 5b04          	addw	sp,#4
 873  02cf 81            	ret
 897                     ; 121 void SEG_GPIO_Init(void)//对数码管显示的GPIO进行初始化配置
 897                     ; 122 {												
 898                     	switch	.text
 899  02d0               _SEG_GPIO_Init:
 903                     ; 123 	GPIO_Init(GPIOB,GPIO_PIN_4|GPIO_PIN_5,GPIO_MODE_OUT_PP_LOW_FAST);
 905  02d0 4be0          	push	#224
 906  02d2 4b30          	push	#48
 907  02d4 ae5005        	ldw	x,#20485
 908  02d7 cd0000        	call	_GPIO_Init
 910  02da 85            	popw	x
 911                     ; 124 	GPIO_Init(GPIOC,GPIO_PIN_HNIB|GPIO_PIN_3,GPIO_MODE_OUT_PP_LOW_FAST);
 913  02db 4be0          	push	#224
 914  02dd 4bf8          	push	#248
 915  02df ae500a        	ldw	x,#20490
 916  02e2 cd0000        	call	_GPIO_Init
 918  02e5 85            	popw	x
 919                     ; 125 	GPIO_Init(GPIOD,GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2,GPIO_MODE_OUT_PP_LOW_FAST);
 921  02e6 4be0          	push	#224
 922  02e8 4b3c          	push	#60
 923  02ea ae500f        	ldw	x,#20495
 924  02ed cd0000        	call	_GPIO_Init
 926  02f0 85            	popw	x
 927                     ; 126 }
 930  02f1 81            	ret
 975                     .const:	section	.text
 976  0000               L43:
 977  0000 030a          	dc.w	L134
 978  0002 034d          	dc.w	L334
 979  0004 0390          	dc.w	L534
 980  0006 03d3          	dc.w	L734
 981  0008 0416          	dc.w	L144
 982  000a 0459          	dc.w	L344
 983  000c 049c          	dc.w	L544
 984  000e 04df          	dc.w	L744
 985  0010 0522          	dc.w	L154
 986  0012 0564          	dc.w	L354
 987                     ; 128 void Send_NumberCode(uchar num,uchar dp_flag)
 987                     ; 129 {
 988                     	switch	.text
 989  02f2               _Send_NumberCode:
 991  02f2 89            	pushw	x
 992       00000000      OFST:	set	0
 995                     ; 130 	switch(num)
 997  02f3 9e            	ld	a,xh
 999                     ; 238 		default:break;
1000  02f4 a10a          	cp	a,#10
1001  02f6 2407          	jruge	L23
1002  02f8 5f            	clrw	x
1003  02f9 97            	ld	xl,a
1004  02fa 58            	sllw	x
1005  02fb de0000        	ldw	x,(L43,x)
1006  02fe fc            	jp	(x)
1007  02ff               L23:
1008  02ff a02d          	sub	a,#45
1009  0301 2603          	jrne	L63
1010  0303 cc05a5        	jp	L554
1011  0306               L63:
1012  0306 aced05ed      	jpf	L505
1013  030a               L134:
1014                     ; 132 		case 0: 
1014                     ; 133 			SEG_A_LOW;    
1016  030a 4b40          	push	#64
1017  030c ae500a        	ldw	x,#20490
1018  030f cd0000        	call	_GPIO_WriteLow
1020  0312 84            	pop	a
1021                     ; 134       SEG_B_LOW;    
1023  0313 4b20          	push	#32
1024  0315 ae500f        	ldw	x,#20495
1025  0318 cd0000        	call	_GPIO_WriteLow
1027  031b 84            	pop	a
1028                     ; 135       SEG_C_LOW;    
1030  031c 4b10          	push	#16
1031  031e ae5005        	ldw	x,#20485
1032  0321 cd0000        	call	_GPIO_WriteLow
1034  0324 84            	pop	a
1035                     ; 136       SEG_D_LOW;   
1037  0325 4b80          	push	#128
1038  0327 ae500a        	ldw	x,#20490
1039  032a cd0000        	call	_GPIO_WriteLow
1041  032d 84            	pop	a
1042                     ; 137       SEG_E_LOW;   
1044  032e 4b08          	push	#8
1045  0330 ae500f        	ldw	x,#20495
1046  0333 cd0000        	call	_GPIO_WriteLow
1048  0336 84            	pop	a
1049                     ; 138       SEG_F_LOW;    
1051  0337 4b10          	push	#16
1052  0339 ae500a        	ldw	x,#20490
1053  033c cd0000        	call	_GPIO_WriteLow
1055  033f 84            	pop	a
1056                     ; 139       SEG_G_HIGH;   
1058  0340 4b20          	push	#32
1059  0342 ae5005        	ldw	x,#20485
1060  0345 cd0000        	call	_GPIO_WriteHigh
1062  0348 84            	pop	a
1063                     ; 140 			break;
1065  0349 aced05ed      	jpf	L505
1066  034d               L334:
1067                     ; 142 		case 1:
1067                     ; 143 			SEG_A_HIGH;    
1069  034d 4b40          	push	#64
1070  034f ae500a        	ldw	x,#20490
1071  0352 cd0000        	call	_GPIO_WriteHigh
1073  0355 84            	pop	a
1074                     ; 144       SEG_B_LOW;    
1076  0356 4b20          	push	#32
1077  0358 ae500f        	ldw	x,#20495
1078  035b cd0000        	call	_GPIO_WriteLow
1080  035e 84            	pop	a
1081                     ; 145       SEG_C_LOW;    
1083  035f 4b10          	push	#16
1084  0361 ae5005        	ldw	x,#20485
1085  0364 cd0000        	call	_GPIO_WriteLow
1087  0367 84            	pop	a
1088                     ; 146       SEG_D_HIGH;    
1090  0368 4b80          	push	#128
1091  036a ae500a        	ldw	x,#20490
1092  036d cd0000        	call	_GPIO_WriteHigh
1094  0370 84            	pop	a
1095                     ; 147       SEG_E_HIGH;   
1097  0371 4b08          	push	#8
1098  0373 ae500f        	ldw	x,#20495
1099  0376 cd0000        	call	_GPIO_WriteHigh
1101  0379 84            	pop	a
1102                     ; 148       SEG_F_HIGH;    
1104  037a 4b10          	push	#16
1105  037c ae500a        	ldw	x,#20490
1106  037f cd0000        	call	_GPIO_WriteHigh
1108  0382 84            	pop	a
1109                     ; 149       SEG_G_HIGH;   
1111  0383 4b20          	push	#32
1112  0385 ae5005        	ldw	x,#20485
1113  0388 cd0000        	call	_GPIO_WriteHigh
1115  038b 84            	pop	a
1116                     ; 150 			break;
1118  038c aced05ed      	jpf	L505
1119  0390               L534:
1120                     ; 151 		case 2: 
1120                     ; 152 			SEG_A_LOW;    
1122  0390 4b40          	push	#64
1123  0392 ae500a        	ldw	x,#20490
1124  0395 cd0000        	call	_GPIO_WriteLow
1126  0398 84            	pop	a
1127                     ; 153       SEG_B_LOW;    
1129  0399 4b20          	push	#32
1130  039b ae500f        	ldw	x,#20495
1131  039e cd0000        	call	_GPIO_WriteLow
1133  03a1 84            	pop	a
1134                     ; 154       SEG_C_HIGH;    
1136  03a2 4b10          	push	#16
1137  03a4 ae5005        	ldw	x,#20485
1138  03a7 cd0000        	call	_GPIO_WriteHigh
1140  03aa 84            	pop	a
1141                     ; 155       SEG_D_LOW;   
1143  03ab 4b80          	push	#128
1144  03ad ae500a        	ldw	x,#20490
1145  03b0 cd0000        	call	_GPIO_WriteLow
1147  03b3 84            	pop	a
1148                     ; 156       SEG_E_LOW;   
1150  03b4 4b08          	push	#8
1151  03b6 ae500f        	ldw	x,#20495
1152  03b9 cd0000        	call	_GPIO_WriteLow
1154  03bc 84            	pop	a
1155                     ; 157       SEG_F_HIGH;    
1157  03bd 4b10          	push	#16
1158  03bf ae500a        	ldw	x,#20490
1159  03c2 cd0000        	call	_GPIO_WriteHigh
1161  03c5 84            	pop	a
1162                     ; 158       SEG_G_LOW;   
1164  03c6 4b20          	push	#32
1165  03c8 ae5005        	ldw	x,#20485
1166  03cb cd0000        	call	_GPIO_WriteLow
1168  03ce 84            	pop	a
1169                     ; 159 			break;
1171  03cf aced05ed      	jpf	L505
1172  03d3               L734:
1173                     ; 161 		case 3:
1173                     ; 162 			SEG_A_LOW;    
1175  03d3 4b40          	push	#64
1176  03d5 ae500a        	ldw	x,#20490
1177  03d8 cd0000        	call	_GPIO_WriteLow
1179  03db 84            	pop	a
1180                     ; 163       SEG_B_LOW;    
1182  03dc 4b20          	push	#32
1183  03de ae500f        	ldw	x,#20495
1184  03e1 cd0000        	call	_GPIO_WriteLow
1186  03e4 84            	pop	a
1187                     ; 164       SEG_C_LOW;    
1189  03e5 4b10          	push	#16
1190  03e7 ae5005        	ldw	x,#20485
1191  03ea cd0000        	call	_GPIO_WriteLow
1193  03ed 84            	pop	a
1194                     ; 165       SEG_D_LOW;    
1196  03ee 4b80          	push	#128
1197  03f0 ae500a        	ldw	x,#20490
1198  03f3 cd0000        	call	_GPIO_WriteLow
1200  03f6 84            	pop	a
1201                     ; 166       SEG_E_HIGH;   
1203  03f7 4b08          	push	#8
1204  03f9 ae500f        	ldw	x,#20495
1205  03fc cd0000        	call	_GPIO_WriteHigh
1207  03ff 84            	pop	a
1208                     ; 167       SEG_F_HIGH;    
1210  0400 4b10          	push	#16
1211  0402 ae500a        	ldw	x,#20490
1212  0405 cd0000        	call	_GPIO_WriteHigh
1214  0408 84            	pop	a
1215                     ; 168       SEG_G_LOW;   
1217  0409 4b20          	push	#32
1218  040b ae5005        	ldw	x,#20485
1219  040e cd0000        	call	_GPIO_WriteLow
1221  0411 84            	pop	a
1222                     ; 169 			break;
1224  0412 aced05ed      	jpf	L505
1225  0416               L144:
1226                     ; 170 		case 4: 
1226                     ; 171 			SEG_A_HIGH;    
1228  0416 4b40          	push	#64
1229  0418 ae500a        	ldw	x,#20490
1230  041b cd0000        	call	_GPIO_WriteHigh
1232  041e 84            	pop	a
1233                     ; 172       SEG_B_LOW;    
1235  041f 4b20          	push	#32
1236  0421 ae500f        	ldw	x,#20495
1237  0424 cd0000        	call	_GPIO_WriteLow
1239  0427 84            	pop	a
1240                     ; 173       SEG_C_LOW;    
1242  0428 4b10          	push	#16
1243  042a ae5005        	ldw	x,#20485
1244  042d cd0000        	call	_GPIO_WriteLow
1246  0430 84            	pop	a
1247                     ; 174       SEG_D_HIGH;   
1249  0431 4b80          	push	#128
1250  0433 ae500a        	ldw	x,#20490
1251  0436 cd0000        	call	_GPIO_WriteHigh
1253  0439 84            	pop	a
1254                     ; 175       SEG_E_HIGH;   
1256  043a 4b08          	push	#8
1257  043c ae500f        	ldw	x,#20495
1258  043f cd0000        	call	_GPIO_WriteHigh
1260  0442 84            	pop	a
1261                     ; 176       SEG_F_LOW;    
1263  0443 4b10          	push	#16
1264  0445 ae500a        	ldw	x,#20490
1265  0448 cd0000        	call	_GPIO_WriteLow
1267  044b 84            	pop	a
1268                     ; 177       SEG_G_LOW;   
1270  044c 4b20          	push	#32
1271  044e ae5005        	ldw	x,#20485
1272  0451 cd0000        	call	_GPIO_WriteLow
1274  0454 84            	pop	a
1275                     ; 178 			break;
1277  0455 aced05ed      	jpf	L505
1278  0459               L344:
1279                     ; 180 		case 5:
1279                     ; 181 			SEG_A_LOW;    
1281  0459 4b40          	push	#64
1282  045b ae500a        	ldw	x,#20490
1283  045e cd0000        	call	_GPIO_WriteLow
1285  0461 84            	pop	a
1286                     ; 182       SEG_B_HIGH;    
1288  0462 4b20          	push	#32
1289  0464 ae500f        	ldw	x,#20495
1290  0467 cd0000        	call	_GPIO_WriteHigh
1292  046a 84            	pop	a
1293                     ; 183       SEG_C_LOW;    
1295  046b 4b10          	push	#16
1296  046d ae5005        	ldw	x,#20485
1297  0470 cd0000        	call	_GPIO_WriteLow
1299  0473 84            	pop	a
1300                     ; 184       SEG_D_LOW;    
1302  0474 4b80          	push	#128
1303  0476 ae500a        	ldw	x,#20490
1304  0479 cd0000        	call	_GPIO_WriteLow
1306  047c 84            	pop	a
1307                     ; 185       SEG_E_HIGH;   
1309  047d 4b08          	push	#8
1310  047f ae500f        	ldw	x,#20495
1311  0482 cd0000        	call	_GPIO_WriteHigh
1313  0485 84            	pop	a
1314                     ; 186       SEG_F_LOW;    
1316  0486 4b10          	push	#16
1317  0488 ae500a        	ldw	x,#20490
1318  048b cd0000        	call	_GPIO_WriteLow
1320  048e 84            	pop	a
1321                     ; 187       SEG_G_LOW;   
1323  048f 4b20          	push	#32
1324  0491 ae5005        	ldw	x,#20485
1325  0494 cd0000        	call	_GPIO_WriteLow
1327  0497 84            	pop	a
1328                     ; 188 			break;
1330  0498 aced05ed      	jpf	L505
1331  049c               L544:
1332                     ; 189 		case 6: 
1332                     ; 190 			SEG_A_LOW;    
1334  049c 4b40          	push	#64
1335  049e ae500a        	ldw	x,#20490
1336  04a1 cd0000        	call	_GPIO_WriteLow
1338  04a4 84            	pop	a
1339                     ; 191       SEG_B_HIGH;    
1341  04a5 4b20          	push	#32
1342  04a7 ae500f        	ldw	x,#20495
1343  04aa cd0000        	call	_GPIO_WriteHigh
1345  04ad 84            	pop	a
1346                     ; 192       SEG_C_LOW;    
1348  04ae 4b10          	push	#16
1349  04b0 ae5005        	ldw	x,#20485
1350  04b3 cd0000        	call	_GPIO_WriteLow
1352  04b6 84            	pop	a
1353                     ; 193       SEG_D_LOW;   
1355  04b7 4b80          	push	#128
1356  04b9 ae500a        	ldw	x,#20490
1357  04bc cd0000        	call	_GPIO_WriteLow
1359  04bf 84            	pop	a
1360                     ; 194       SEG_E_LOW;   
1362  04c0 4b08          	push	#8
1363  04c2 ae500f        	ldw	x,#20495
1364  04c5 cd0000        	call	_GPIO_WriteLow
1366  04c8 84            	pop	a
1367                     ; 195       SEG_F_LOW;    
1369  04c9 4b10          	push	#16
1370  04cb ae500a        	ldw	x,#20490
1371  04ce cd0000        	call	_GPIO_WriteLow
1373  04d1 84            	pop	a
1374                     ; 196       SEG_G_LOW;   
1376  04d2 4b20          	push	#32
1377  04d4 ae5005        	ldw	x,#20485
1378  04d7 cd0000        	call	_GPIO_WriteLow
1380  04da 84            	pop	a
1381                     ; 197 			break;
1383  04db aced05ed      	jpf	L505
1384  04df               L744:
1385                     ; 199 		case 7:
1385                     ; 200 			SEG_A_LOW;    
1387  04df 4b40          	push	#64
1388  04e1 ae500a        	ldw	x,#20490
1389  04e4 cd0000        	call	_GPIO_WriteLow
1391  04e7 84            	pop	a
1392                     ; 201       SEG_B_LOW;    
1394  04e8 4b20          	push	#32
1395  04ea ae500f        	ldw	x,#20495
1396  04ed cd0000        	call	_GPIO_WriteLow
1398  04f0 84            	pop	a
1399                     ; 202       SEG_C_LOW;    
1401  04f1 4b10          	push	#16
1402  04f3 ae5005        	ldw	x,#20485
1403  04f6 cd0000        	call	_GPIO_WriteLow
1405  04f9 84            	pop	a
1406                     ; 203       SEG_D_HIGH;    
1408  04fa 4b80          	push	#128
1409  04fc ae500a        	ldw	x,#20490
1410  04ff cd0000        	call	_GPIO_WriteHigh
1412  0502 84            	pop	a
1413                     ; 204       SEG_E_HIGH;   
1415  0503 4b08          	push	#8
1416  0505 ae500f        	ldw	x,#20495
1417  0508 cd0000        	call	_GPIO_WriteHigh
1419  050b 84            	pop	a
1420                     ; 205       SEG_F_HIGH;    
1422  050c 4b10          	push	#16
1423  050e ae500a        	ldw	x,#20490
1424  0511 cd0000        	call	_GPIO_WriteHigh
1426  0514 84            	pop	a
1427                     ; 206       SEG_G_HIGH;   
1429  0515 4b20          	push	#32
1430  0517 ae5005        	ldw	x,#20485
1431  051a cd0000        	call	_GPIO_WriteHigh
1433  051d 84            	pop	a
1434                     ; 207 			break;
1436  051e aced05ed      	jpf	L505
1437  0522               L154:
1438                     ; 208 		case 8: 
1438                     ; 209 			SEG_A_LOW;    
1440  0522 4b40          	push	#64
1441  0524 ae500a        	ldw	x,#20490
1442  0527 cd0000        	call	_GPIO_WriteLow
1444  052a 84            	pop	a
1445                     ; 210       SEG_B_LOW;    
1447  052b 4b20          	push	#32
1448  052d ae500f        	ldw	x,#20495
1449  0530 cd0000        	call	_GPIO_WriteLow
1451  0533 84            	pop	a
1452                     ; 211       SEG_C_LOW;    
1454  0534 4b10          	push	#16
1455  0536 ae5005        	ldw	x,#20485
1456  0539 cd0000        	call	_GPIO_WriteLow
1458  053c 84            	pop	a
1459                     ; 212       SEG_D_LOW;   
1461  053d 4b80          	push	#128
1462  053f ae500a        	ldw	x,#20490
1463  0542 cd0000        	call	_GPIO_WriteLow
1465  0545 84            	pop	a
1466                     ; 213       SEG_E_LOW;   
1468  0546 4b08          	push	#8
1469  0548 ae500f        	ldw	x,#20495
1470  054b cd0000        	call	_GPIO_WriteLow
1472  054e 84            	pop	a
1473                     ; 214       SEG_F_LOW;    
1475  054f 4b10          	push	#16
1476  0551 ae500a        	ldw	x,#20490
1477  0554 cd0000        	call	_GPIO_WriteLow
1479  0557 84            	pop	a
1480                     ; 215       SEG_G_LOW;   
1482  0558 4b20          	push	#32
1483  055a ae5005        	ldw	x,#20485
1484  055d cd0000        	call	_GPIO_WriteLow
1486  0560 84            	pop	a
1487                     ; 216 			break;
1489  0561 cc05ed        	jra	L505
1490  0564               L354:
1491                     ; 218 		case 9:
1491                     ; 219 			SEG_A_LOW;    
1493  0564 4b40          	push	#64
1494  0566 ae500a        	ldw	x,#20490
1495  0569 cd0000        	call	_GPIO_WriteLow
1497  056c 84            	pop	a
1498                     ; 220       SEG_B_LOW;    
1500  056d 4b20          	push	#32
1501  056f ae500f        	ldw	x,#20495
1502  0572 cd0000        	call	_GPIO_WriteLow
1504  0575 84            	pop	a
1505                     ; 221       SEG_C_LOW;    
1507  0576 4b10          	push	#16
1508  0578 ae5005        	ldw	x,#20485
1509  057b cd0000        	call	_GPIO_WriteLow
1511  057e 84            	pop	a
1512                     ; 222       SEG_D_LOW;    
1514  057f 4b80          	push	#128
1515  0581 ae500a        	ldw	x,#20490
1516  0584 cd0000        	call	_GPIO_WriteLow
1518  0587 84            	pop	a
1519                     ; 223       SEG_E_HIGH;   
1521  0588 4b08          	push	#8
1522  058a ae500f        	ldw	x,#20495
1523  058d cd0000        	call	_GPIO_WriteHigh
1525  0590 84            	pop	a
1526                     ; 224       SEG_F_LOW;    
1528  0591 4b10          	push	#16
1529  0593 ae500a        	ldw	x,#20490
1530  0596 cd0000        	call	_GPIO_WriteLow
1532  0599 84            	pop	a
1533                     ; 225       SEG_G_LOW;   
1535  059a 4b20          	push	#32
1536  059c ae5005        	ldw	x,#20485
1537  059f cd0000        	call	_GPIO_WriteLow
1539  05a2 84            	pop	a
1540                     ; 226 			break;
1542  05a3 2048          	jra	L505
1543  05a5               L554:
1544                     ; 228 		case '-':     //显示负号
1544                     ; 229 			SEG_A_HIGH;    
1546  05a5 4b40          	push	#64
1547  05a7 ae500a        	ldw	x,#20490
1548  05aa cd0000        	call	_GPIO_WriteHigh
1550  05ad 84            	pop	a
1551                     ; 230 			SEG_B_HIGH;    
1553  05ae 4b20          	push	#32
1554  05b0 ae500f        	ldw	x,#20495
1555  05b3 cd0000        	call	_GPIO_WriteHigh
1557  05b6 84            	pop	a
1558                     ; 231 			SEG_C_HIGH;    
1560  05b7 4b10          	push	#16
1561  05b9 ae5005        	ldw	x,#20485
1562  05bc cd0000        	call	_GPIO_WriteHigh
1564  05bf 84            	pop	a
1565                     ; 232 			SEG_D_HIGH;    
1567  05c0 4b80          	push	#128
1568  05c2 ae500a        	ldw	x,#20490
1569  05c5 cd0000        	call	_GPIO_WriteHigh
1571  05c8 84            	pop	a
1572                     ; 233 			SEG_E_HIGH;   
1574  05c9 4b08          	push	#8
1575  05cb ae500f        	ldw	x,#20495
1576  05ce cd0000        	call	_GPIO_WriteHigh
1578  05d1 84            	pop	a
1579                     ; 234 			SEG_F_HIGH;    
1581  05d2 4b10          	push	#16
1582  05d4 ae500a        	ldw	x,#20490
1583  05d7 cd0000        	call	_GPIO_WriteHigh
1585  05da 84            	pop	a
1586                     ; 235 			SEG_G_LOW; 
1588  05db 4b20          	push	#32
1589  05dd ae5005        	ldw	x,#20485
1590  05e0 cd0000        	call	_GPIO_WriteLow
1592  05e3 84            	pop	a
1593                     ; 236 			SEG_DP_HIGH;	
1595  05e4 4b20          	push	#32
1596  05e6 ae500a        	ldw	x,#20490
1597  05e9 cd0000        	call	_GPIO_WriteHigh
1599  05ec 84            	pop	a
1600  05ed               L754:
1601                     ; 238 		default:break;
1603  05ed               L505:
1604                     ; 240 	if(dp_flag==DP_DISENABLE)	
1606  05ed 0d02          	tnz	(OFST+2,sp)
1607  05ef 260b          	jrne	L705
1608                     ; 241 			SEG_DP_HIGH;//不亮
1610  05f1 4b20          	push	#32
1611  05f3 ae500a        	ldw	x,#20490
1612  05f6 cd0000        	call	_GPIO_WriteHigh
1614  05f9 84            	pop	a
1616  05fa 2009          	jra	L115
1617  05fc               L705:
1618                     ; 243 			SEG_DP_LOW;//亮
1620  05fc 4b20          	push	#32
1621  05fe ae500a        	ldw	x,#20490
1622  0601 cd0000        	call	_GPIO_WriteLow
1624  0604 84            	pop	a
1625  0605               L115:
1626                     ; 244 }
1629  0605 85            	popw	x
1630  0606 81            	ret
1686                     ; 246 void Display_SingleNumber(uchar num,uchar wei,uchar dp_flag)
1686                     ; 247 {
1687                     	switch	.text
1688  0607               _Display_SingleNumber:
1690  0607 89            	pushw	x
1691       00000000      OFST:	set	0
1694                     ; 248 	Send_NumberCode(num,dp_flag);		
1696  0608 7b05          	ld	a,(OFST+5,sp)
1697  060a 97            	ld	xl,a
1698  060b 7b01          	ld	a,(OFST+1,sp)
1699  060d 95            	ld	xh,a
1700  060e cd02f2        	call	_Send_NumberCode
1702                     ; 250 	switch(wei)
1704  0611 7b02          	ld	a,(OFST+2,sp)
1706                     ; 268 		default:break;	
1707  0613 4a            	dec	a
1708  0614 2708          	jreq	L315
1709  0616 4a            	dec	a
1710  0617 2722          	jreq	L515
1711  0619 4a            	dec	a
1712  061a 273c          	jreq	L715
1713  061c 2055          	jra	L355
1714  061e               L315:
1715                     ; 252 		case SEG_W1:
1715                     ; 253 			SEG_W2_DISENABLE;
1717  061e 4b08          	push	#8
1718  0620 ae500a        	ldw	x,#20490
1719  0623 cd0000        	call	_GPIO_WriteLow
1721  0626 84            	pop	a
1722                     ; 254 			SEG_W3_DISENABLE;
1724  0627 4b10          	push	#16
1725  0629 ae500f        	ldw	x,#20495
1726  062c cd0000        	call	_GPIO_WriteLow
1728  062f 84            	pop	a
1729                     ; 255 			SEG_W1_ENABLE;
1731  0630 4b04          	push	#4
1732  0632 ae500f        	ldw	x,#20495
1733  0635 cd0000        	call	_GPIO_WriteHigh
1735  0638 84            	pop	a
1736                     ; 256 			break;
1738  0639 2038          	jra	L355
1739  063b               L515:
1740                     ; 257 		case SEG_W2:
1740                     ; 258 			SEG_W3_DISENABLE;
1742  063b 4b10          	push	#16
1743  063d ae500f        	ldw	x,#20495
1744  0640 cd0000        	call	_GPIO_WriteLow
1746  0643 84            	pop	a
1747                     ; 259 			SEG_W1_DISENABLE;
1749  0644 4b04          	push	#4
1750  0646 ae500f        	ldw	x,#20495
1751  0649 cd0000        	call	_GPIO_WriteLow
1753  064c 84            	pop	a
1754                     ; 260 			SEG_W2_ENABLE;
1756  064d 4b08          	push	#8
1757  064f ae500a        	ldw	x,#20490
1758  0652 cd0000        	call	_GPIO_WriteHigh
1760  0655 84            	pop	a
1761                     ; 261 			break;
1763  0656 201b          	jra	L355
1764  0658               L715:
1765                     ; 262 		case SEG_W3:
1765                     ; 263 			SEG_W1_DISENABLE;
1767  0658 4b04          	push	#4
1768  065a ae500f        	ldw	x,#20495
1769  065d cd0000        	call	_GPIO_WriteLow
1771  0660 84            	pop	a
1772                     ; 264 			SEG_W2_DISENABLE;
1774  0661 4b08          	push	#8
1775  0663 ae500a        	ldw	x,#20490
1776  0666 cd0000        	call	_GPIO_WriteLow
1778  0669 84            	pop	a
1779                     ; 265 			SEG_W3_ENABLE;
1781  066a 4b10          	push	#16
1782  066c ae500f        	ldw	x,#20495
1783  066f cd0000        	call	_GPIO_WriteHigh
1785  0672 84            	pop	a
1786                     ; 266 			break;
1788  0673               L125:
1789                     ; 268 		default:break;	
1791  0673               L355:
1792                     ; 270 }
1795  0673 85            	popw	x
1796  0674 81            	ret
1822                     ; 272 void Display_Open(void)//开机时显示8.8.8.
1822                     ; 273 {
1823                     	switch	.text
1824  0675               _Display_Open:
1828                     ; 274 	Send_NumberCode(8,DP_ENABLE);
1830  0675 ae0001        	ldw	x,#1
1831  0678 a608          	ld	a,#8
1832  067a 95            	ld	xh,a
1833  067b cd02f2        	call	_Send_NumberCode
1835                     ; 275 	SEG_W2_ENABLE;
1837  067e 4b08          	push	#8
1838  0680 ae500a        	ldw	x,#20490
1839  0683 cd0000        	call	_GPIO_WriteHigh
1841  0686 84            	pop	a
1842                     ; 276 	SEG_W3_ENABLE;
1844  0687 4b10          	push	#16
1845  0689 ae500f        	ldw	x,#20495
1846  068c cd0000        	call	_GPIO_WriteHigh
1848  068f 84            	pop	a
1849                     ; 277 	SEG_W1_ENABLE;//因为3个数码管显示相同，位选使能全部打开
1851  0690 4b04          	push	#4
1852  0692 ae500f        	ldw	x,#20495
1853  0695 cd0000        	call	_GPIO_WriteHigh
1855  0698 84            	pop	a
1856                     ; 278 	delay_ms(1500);
1858  0699 ae05dc        	ldw	x,#1500
1859  069c cd002b        	call	_delay_ms
1861                     ; 279 }
1864  069f 81            	ret
1930                     ; 281 void Display_Temperature(int temp)
1930                     ; 282 {
1931                     	switch	.text
1932  06a0               _Display_Temperature:
1934  06a0 89            	pushw	x
1935  06a1 5203          	subw	sp,#3
1936       00000003      OFST:	set	3
1939                     ; 285 	if(temp>-100&&temp<0)
1941  06a3 9c            	rvf
1942  06a4 a3ff9d        	cpw	x,#65437
1943  06a7 2f5b          	jrslt	L716
1945  06a9 9c            	rvf
1946  06aa a30000        	cpw	x,#0
1947  06ad 2e55          	jrsge	L716
1948                     ; 287 		temp=-temp;//得到其绝对值
1950  06af 1e04          	ldw	x,(OFST+1,sp)
1951  06b1 50            	negw	x
1952  06b2 1f04          	ldw	(OFST+1,sp),x
1953                     ; 288 		shi=temp/10;
1955  06b4 1e04          	ldw	x,(OFST+1,sp)
1956  06b6 a60a          	ld	a,#10
1957  06b8 cd0000        	call	c_sdivx
1959  06bb 01            	rrwa	x,a
1960  06bc 6b03          	ld	(OFST+0,sp),a
1961  06be 02            	rlwa	x,a
1962                     ; 289 		ge=temp%10;
1964  06bf 1e04          	ldw	x,(OFST+1,sp)
1965  06c1 a60a          	ld	a,#10
1966  06c3 cd0000        	call	c_smodx
1968  06c6 01            	rrwa	x,a
1969  06c7 6b02          	ld	(OFST-1,sp),a
1970  06c9 02            	rlwa	x,a
1971                     ; 291 		Display_SingleNumber('-',SEG_W1,DP_DISENABLE);
1973  06ca 4b00          	push	#0
1974  06cc ae0001        	ldw	x,#1
1975  06cf a62d          	ld	a,#45
1976  06d1 95            	ld	xh,a
1977  06d2 cd0607        	call	_Display_SingleNumber
1979  06d5 84            	pop	a
1980                     ; 292 		delay_ms(5);
1982  06d6 ae0005        	ldw	x,#5
1983  06d9 cd002b        	call	_delay_ms
1985                     ; 293 		Display_SingleNumber(shi,SEG_W2,DP_ENABLE);
1987  06dc 4b01          	push	#1
1988  06de ae0002        	ldw	x,#2
1989  06e1 7b04          	ld	a,(OFST+1,sp)
1990  06e3 95            	ld	xh,a
1991  06e4 cd0607        	call	_Display_SingleNumber
1993  06e7 84            	pop	a
1994                     ; 294 		delay_ms(5);
1996  06e8 ae0005        	ldw	x,#5
1997  06eb cd002b        	call	_delay_ms
1999                     ; 295 		Display_SingleNumber(ge,SEG_W3,DP_DISENABLE);
2001  06ee 4b00          	push	#0
2002  06f0 ae0003        	ldw	x,#3
2003  06f3 7b03          	ld	a,(OFST+0,sp)
2004  06f5 95            	ld	xh,a
2005  06f6 cd0607        	call	_Display_SingleNumber
2007  06f9 84            	pop	a
2008                     ; 296 		delay_ms(5);
2010  06fa ae0005        	ldw	x,#5
2011  06fd cd002b        	call	_delay_ms
2014  0700 ac100810      	jpf	L126
2015  0704               L716:
2016                     ; 299 	else if(temp>=0&&temp<1000)
2018  0704 9c            	rvf
2019  0705 1e04          	ldw	x,(OFST+1,sp)
2020  0707 2f68          	jrslt	L326
2022  0709 9c            	rvf
2023  070a 1e04          	ldw	x,(OFST+1,sp)
2024  070c a303e8        	cpw	x,#1000
2025  070f 2e60          	jrsge	L326
2026                     ; 301 		bai=temp/100;
2028  0711 1e04          	ldw	x,(OFST+1,sp)
2029  0713 a664          	ld	a,#100
2030  0715 cd0000        	call	c_sdivx
2032  0718 01            	rrwa	x,a
2033  0719 6b01          	ld	(OFST-2,sp),a
2034  071b 02            	rlwa	x,a
2035                     ; 302 		shi=temp/10%10;
2037  071c 1e04          	ldw	x,(OFST+1,sp)
2038  071e a60a          	ld	a,#10
2039  0720 cd0000        	call	c_sdivx
2041  0723 a60a          	ld	a,#10
2042  0725 cd0000        	call	c_smodx
2044  0728 01            	rrwa	x,a
2045  0729 6b03          	ld	(OFST+0,sp),a
2046  072b 02            	rlwa	x,a
2047                     ; 303 		ge=temp%10;
2049  072c 1e04          	ldw	x,(OFST+1,sp)
2050  072e a60a          	ld	a,#10
2051  0730 cd0000        	call	c_smodx
2053  0733 01            	rrwa	x,a
2054  0734 6b02          	ld	(OFST-1,sp),a
2055  0736 02            	rlwa	x,a
2056                     ; 305 		Display_SingleNumber(bai,SEG_W1,DP_DISENABLE);
2058  0737 4b00          	push	#0
2059  0739 ae0001        	ldw	x,#1
2060  073c 7b02          	ld	a,(OFST-1,sp)
2061  073e 95            	ld	xh,a
2062  073f cd0607        	call	_Display_SingleNumber
2064  0742 84            	pop	a
2065                     ; 306 		delay_ms(5);
2067  0743 ae0005        	ldw	x,#5
2068  0746 cd002b        	call	_delay_ms
2070                     ; 307 		Display_SingleNumber(shi,SEG_W2,DP_ENABLE);
2072  0749 4b01          	push	#1
2073  074b ae0002        	ldw	x,#2
2074  074e 7b04          	ld	a,(OFST+1,sp)
2075  0750 95            	ld	xh,a
2076  0751 cd0607        	call	_Display_SingleNumber
2078  0754 84            	pop	a
2079                     ; 308 		delay_ms(5);
2081  0755 ae0005        	ldw	x,#5
2082  0758 cd002b        	call	_delay_ms
2084                     ; 309 		Display_SingleNumber(ge,SEG_W3,DP_DISENABLE);
2086  075b 4b00          	push	#0
2087  075d ae0003        	ldw	x,#3
2088  0760 7b03          	ld	a,(OFST+0,sp)
2089  0762 95            	ld	xh,a
2090  0763 cd0607        	call	_Display_SingleNumber
2092  0766 84            	pop	a
2093                     ; 310 		delay_ms(5);
2095  0767 ae0005        	ldw	x,#5
2096  076a cd002b        	call	_delay_ms
2099  076d ac100810      	jpf	L126
2100  0771               L326:
2101                     ; 312 	else if(temp>=1000&&temp<=1200)
2103  0771 9c            	rvf
2104  0772 1e04          	ldw	x,(OFST+1,sp)
2105  0774 a303e8        	cpw	x,#1000
2106  0777 2f6f          	jrslt	L726
2108  0779 9c            	rvf
2109  077a 1e04          	ldw	x,(OFST+1,sp)
2110  077c a304b1        	cpw	x,#1201
2111  077f 2e67          	jrsge	L726
2112                     ; 314 		temp=temp/10;//截掉最低位
2114  0781 1e04          	ldw	x,(OFST+1,sp)
2115  0783 a60a          	ld	a,#10
2116  0785 cd0000        	call	c_sdivx
2118  0788 1f04          	ldw	(OFST+1,sp),x
2119                     ; 316 		bai=temp/100;
2121  078a 1e04          	ldw	x,(OFST+1,sp)
2122  078c a664          	ld	a,#100
2123  078e cd0000        	call	c_sdivx
2125  0791 01            	rrwa	x,a
2126  0792 6b01          	ld	(OFST-2,sp),a
2127  0794 02            	rlwa	x,a
2128                     ; 317 		shi=temp/10%10;
2130  0795 1e04          	ldw	x,(OFST+1,sp)
2131  0797 a60a          	ld	a,#10
2132  0799 cd0000        	call	c_sdivx
2134  079c a60a          	ld	a,#10
2135  079e cd0000        	call	c_smodx
2137  07a1 01            	rrwa	x,a
2138  07a2 6b03          	ld	(OFST+0,sp),a
2139  07a4 02            	rlwa	x,a
2140                     ; 318 		ge=temp%10;
2142  07a5 1e04          	ldw	x,(OFST+1,sp)
2143  07a7 a60a          	ld	a,#10
2144  07a9 cd0000        	call	c_smodx
2146  07ac 01            	rrwa	x,a
2147  07ad 6b02          	ld	(OFST-1,sp),a
2148  07af 02            	rlwa	x,a
2149                     ; 320 		Display_SingleNumber(bai,SEG_W1,DP_DISENABLE);
2151  07b0 4b00          	push	#0
2152  07b2 ae0001        	ldw	x,#1
2153  07b5 7b02          	ld	a,(OFST-1,sp)
2154  07b7 95            	ld	xh,a
2155  07b8 cd0607        	call	_Display_SingleNumber
2157  07bb 84            	pop	a
2158                     ; 321 		delay_ms(5);
2160  07bc ae0005        	ldw	x,#5
2161  07bf cd002b        	call	_delay_ms
2163                     ; 322 		Display_SingleNumber(shi,SEG_W2,DP_DISENABLE);
2165  07c2 4b00          	push	#0
2166  07c4 ae0002        	ldw	x,#2
2167  07c7 7b04          	ld	a,(OFST+1,sp)
2168  07c9 95            	ld	xh,a
2169  07ca cd0607        	call	_Display_SingleNumber
2171  07cd 84            	pop	a
2172                     ; 323 		delay_ms(5);
2174  07ce ae0005        	ldw	x,#5
2175  07d1 cd002b        	call	_delay_ms
2177                     ; 324 		Display_SingleNumber(ge,SEG_W3,DP_DISENABLE);
2179  07d4 4b00          	push	#0
2180  07d6 ae0003        	ldw	x,#3
2181  07d9 7b03          	ld	a,(OFST+0,sp)
2182  07db 95            	ld	xh,a
2183  07dc cd0607        	call	_Display_SingleNumber
2185  07df 84            	pop	a
2186                     ; 325 		delay_ms(5);
2188  07e0 ae0005        	ldw	x,#5
2189  07e3 cd002b        	call	_delay_ms
2192  07e6 2028          	jra	L126
2193  07e8               L726:
2194                     ; 329 		Send_NumberCode('-',DP_DISENABLE);
2196  07e8 5f            	clrw	x
2197  07e9 a62d          	ld	a,#45
2198  07eb 95            	ld	xh,a
2199  07ec cd02f2        	call	_Send_NumberCode
2201                     ; 330 		SEG_W2_ENABLE;
2203  07ef 4b08          	push	#8
2204  07f1 ae500a        	ldw	x,#20490
2205  07f4 cd0000        	call	_GPIO_WriteHigh
2207  07f7 84            	pop	a
2208                     ; 331 		SEG_W3_ENABLE;
2210  07f8 4b10          	push	#16
2211  07fa ae500f        	ldw	x,#20495
2212  07fd cd0000        	call	_GPIO_WriteHigh
2214  0800 84            	pop	a
2215                     ; 332 		SEG_W1_ENABLE;
2217  0801 4b04          	push	#4
2218  0803 ae500f        	ldw	x,#20495
2219  0806 cd0000        	call	_GPIO_WriteHigh
2221  0809 84            	pop	a
2222                     ; 333 		delay_ms(15);
2224  080a ae000f        	ldw	x,#15
2225  080d cd002b        	call	_delay_ms
2227  0810               L126:
2228                     ; 339 }
2231  0810 5b05          	addw	sp,#5
2232  0812 81            	ret
2245                     	xdef	_ADC1_GetValue
2246                     	xdef	_main
2247                     	xdef	_Display_SingleNumber
2248                     	xdef	_GetTemperatureValue_X10
2249                     	xdef	_Display_Temperature
2250                     	xdef	_Send_NumberCode
2251                     	xdef	_Display_Open
2252                     	xdef	_SEG_GPIO_Init
2253                     	xdef	_ADC1_GetValue_Average
2254                     	xdef	_MY_ADC1_Init
2255                     	xdef	_delay_ms
2256                     	xref	_GPIO_WriteLow
2257                     	xref	_GPIO_WriteHigh
2258                     	xref	_GPIO_Init
2259                     	xref	_CLK_HSIPrescalerConfig
2260                     	xref	_ADC1_ClearFlag
2261                     	xref	_ADC1_GetFlagStatus
2262                     	xref	_ADC1_GetConversionValue
2263                     	xref	_ADC1_StartConversion
2264                     	xref	_ADC1_Cmd
2265                     	xref	_ADC1_Init
2266                     	xref	_ADC1_DeInit
2267                     	switch	.const
2268  0014               L313:
2269  0014 40d00000      	dc.w	16592,0
2270  0018               L372:
2271  0018 40647ae1      	dc.w	16484,31457
2272  001c               L752:
2273  001c 402851eb      	dc.w	16424,20971
2274  0020               L342:
2275  0020 3ff5c28f      	dc.w	16373,-15729
2276  0024               L722:
2277  0024 3fcb851e      	dc.w	16331,-31458
2278  0028               L312:
2279  0028 3f947ae1      	dc.w	16276,31457
2280  002c               L371:
2281  002c 3f68f5c2      	dc.w	16232,-2622
2282  0030               L351:
2283  0030 3fa00000      	dc.w	16288,0
2284                     	xref.b	c_x
2304                     	xref	c_sdivx
2305                     	xref	c_smodx
2306                     	xref	c_imul
2307                     	xref	c_ftoi
2308                     	xref	c_fmul
2309                     	xref	c_uitof
2310                     	end
