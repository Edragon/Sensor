   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Parser V4.8.32 - 23 Mar 2010
   3                     ; Generator V4.3.4 - 23 Mar 2010
  43                     ; 52 void TIM5_DeInit(void)
  43                     ; 53 {
  45                     	switch	.text
  46  0000               _TIM5_DeInit:
  50                     ; 54   TIM5->CR1 = (uint8_t)TIM5_CR1_RESET_VALUE;
  52  0000 725f5300      	clr	21248
  53                     ; 55   TIM5->CR2 = TIM5_CR2_RESET_VALUE;
  55  0004 725f5301      	clr	21249
  56                     ; 56   TIM5->SMCR = TIM5_SMCR_RESET_VALUE;
  58  0008 725f5302      	clr	21250
  59                     ; 57   TIM5->IER = (uint8_t)TIM5_IER_RESET_VALUE;
  61  000c 725f5303      	clr	21251
  62                     ; 58   TIM5->SR2 = (uint8_t)TIM5_SR2_RESET_VALUE;
  64  0010 725f5305      	clr	21253
  65                     ; 61   TIM5->CCER1 = (uint8_t)TIM5_CCER1_RESET_VALUE;
  67  0014 725f530a      	clr	21258
  68                     ; 62   TIM5->CCER2 = (uint8_t)TIM5_CCER2_RESET_VALUE;
  70  0018 725f530b      	clr	21259
  71                     ; 65   TIM5->CCER1 = (uint8_t)TIM5_CCER1_RESET_VALUE;
  73  001c 725f530a      	clr	21258
  74                     ; 66   TIM5->CCER2 = (uint8_t)TIM5_CCER2_RESET_VALUE;
  76  0020 725f530b      	clr	21259
  77                     ; 67   TIM5->CCMR1 = (uint8_t)TIM5_CCMR1_RESET_VALUE;
  79  0024 725f5307      	clr	21255
  80                     ; 68   TIM5->CCMR2 = (uint8_t)TIM5_CCMR2_RESET_VALUE;
  82  0028 725f5308      	clr	21256
  83                     ; 69   TIM5->CCMR3 = (uint8_t)TIM5_CCMR3_RESET_VALUE;
  85  002c 725f5309      	clr	21257
  86                     ; 70   TIM5->CNTRH = (uint8_t)TIM5_CNTRH_RESET_VALUE;
  88  0030 725f530c      	clr	21260
  89                     ; 71   TIM5->CNTRL = (uint8_t)TIM5_CNTRL_RESET_VALUE;
  91  0034 725f530d      	clr	21261
  92                     ; 72   TIM5->PSCR	= (uint8_t)TIM5_PSCR_RESET_VALUE;
  94  0038 725f530e      	clr	21262
  95                     ; 73   TIM5->ARRH 	= (uint8_t)TIM5_ARRH_RESET_VALUE;
  97  003c 35ff530f      	mov	21263,#255
  98                     ; 74   TIM5->ARRL 	= (uint8_t)TIM5_ARRL_RESET_VALUE;
 100  0040 35ff5310      	mov	21264,#255
 101                     ; 75   TIM5->CCR1H = (uint8_t)TIM5_CCR1H_RESET_VALUE;
 103  0044 725f5311      	clr	21265
 104                     ; 76   TIM5->CCR1L = (uint8_t)TIM5_CCR1L_RESET_VALUE;
 106  0048 725f5312      	clr	21266
 107                     ; 77   TIM5->CCR2H = (uint8_t)TIM5_CCR2H_RESET_VALUE;
 109  004c 725f5313      	clr	21267
 110                     ; 78   TIM5->CCR2L = (uint8_t)TIM5_CCR2L_RESET_VALUE;
 112  0050 725f5314      	clr	21268
 113                     ; 79   TIM5->CCR3H = (uint8_t)TIM5_CCR3H_RESET_VALUE;
 115  0054 725f5315      	clr	21269
 116                     ; 80   TIM5->CCR3L = (uint8_t)TIM5_CCR3L_RESET_VALUE;
 118  0058 725f5316      	clr	21270
 119                     ; 81   TIM5->SR1 = (uint8_t)TIM5_SR1_RESET_VALUE;
 121  005c 725f5304      	clr	21252
 122                     ; 82 }
 125  0060 81            	ret
 293                     ; 90 void TIM5_TimeBaseInit( TIM5_Prescaler_TypeDef TIM5_Prescaler,
 293                     ; 91                         uint16_t TIM5_Period)
 293                     ; 92 {
 294                     	switch	.text
 295  0061               _TIM5_TimeBaseInit:
 297  0061 88            	push	a
 298       00000000      OFST:	set	0
 301                     ; 94   TIM5->PSCR = (uint8_t)(TIM5_Prescaler);
 303  0062 c7530e        	ld	21262,a
 304                     ; 96   TIM5->ARRH = (uint8_t)(TIM5_Period >> 8) ;
 306  0065 7b04          	ld	a,(OFST+4,sp)
 307  0067 c7530f        	ld	21263,a
 308                     ; 97   TIM5->ARRL = (uint8_t)(TIM5_Period);
 310  006a 7b05          	ld	a,(OFST+5,sp)
 311  006c c75310        	ld	21264,a
 312                     ; 98 }
 315  006f 84            	pop	a
 316  0070 81            	ret
 473                     ; 108 void TIM5_OC1Init(TIM5_OCMode_TypeDef TIM5_OCMode,
 473                     ; 109                   TIM5_OutputState_TypeDef TIM5_OutputState,
 473                     ; 110                   uint16_t TIM5_Pulse,
 473                     ; 111                   TIM5_OCPolarity_TypeDef TIM5_OCPolarity)
 473                     ; 112 {
 474                     	switch	.text
 475  0071               _TIM5_OC1Init:
 477  0071 89            	pushw	x
 478  0072 88            	push	a
 479       00000001      OFST:	set	1
 482                     ; 114   assert_param(IS_TIM5_OC_MODE_OK(TIM5_OCMode));
 484                     ; 115   assert_param(IS_TIM5_OUTPUT_STATE_OK(TIM5_OutputState));
 486                     ; 116   assert_param(IS_TIM5_OC_POLARITY_OK(TIM5_OCPolarity));
 488                     ; 119   TIM5->CCER1 &= (uint8_t)(~( TIM5_CCER1_CC1E | TIM5_CCER1_CC1P));
 490  0073 c6530a        	ld	a,21258
 491  0076 a4fc          	and	a,#252
 492  0078 c7530a        	ld	21258,a
 493                     ; 121   TIM5->CCER1 |= (uint8_t)((uint8_t)(TIM5_OutputState & TIM5_CCER1_CC1E )| 
 493                     ; 122                            (uint8_t)(TIM5_OCPolarity & TIM5_CCER1_CC1P));
 495  007b 7b08          	ld	a,(OFST+7,sp)
 496  007d a402          	and	a,#2
 497  007f 6b01          	ld	(OFST+0,sp),a
 498  0081 9f            	ld	a,xl
 499  0082 a401          	and	a,#1
 500  0084 1a01          	or	a,(OFST+0,sp)
 501  0086 ca530a        	or	a,21258
 502  0089 c7530a        	ld	21258,a
 503                     ; 125   TIM5->CCMR1 = (uint8_t)((uint8_t)(TIM5->CCMR1 & (uint8_t)(~TIM5_CCMR_OCM)) | 
 503                     ; 126                           (uint8_t)TIM5_OCMode);
 505  008c c65307        	ld	a,21255
 506  008f a48f          	and	a,#143
 507  0091 1a02          	or	a,(OFST+1,sp)
 508  0093 c75307        	ld	21255,a
 509                     ; 129   TIM5->CCR1H = (uint8_t)(TIM5_Pulse >> 8);
 511  0096 7b06          	ld	a,(OFST+5,sp)
 512  0098 c75311        	ld	21265,a
 513                     ; 130   TIM5->CCR1L = (uint8_t)(TIM5_Pulse);
 515  009b 7b07          	ld	a,(OFST+6,sp)
 516  009d c75312        	ld	21266,a
 517                     ; 131 }
 520  00a0 5b03          	addw	sp,#3
 521  00a2 81            	ret
 585                     ; 141 void TIM5_OC2Init(TIM5_OCMode_TypeDef TIM5_OCMode,
 585                     ; 142                   TIM5_OutputState_TypeDef TIM5_OutputState,
 585                     ; 143                   uint16_t TIM5_Pulse,
 585                     ; 144                   TIM5_OCPolarity_TypeDef TIM5_OCPolarity)
 585                     ; 145 {
 586                     	switch	.text
 587  00a3               _TIM5_OC2Init:
 589  00a3 89            	pushw	x
 590  00a4 88            	push	a
 591       00000001      OFST:	set	1
 594                     ; 147   assert_param(IS_TIM5_OC_MODE_OK(TIM5_OCMode));
 596                     ; 148   assert_param(IS_TIM5_OUTPUT_STATE_OK(TIM5_OutputState));
 598                     ; 149   assert_param(IS_TIM5_OC_POLARITY_OK(TIM5_OCPolarity));
 600                     ; 152   TIM5->CCER1 &= (uint8_t)(~( TIM5_CCER1_CC2E |  TIM5_CCER1_CC2P ));
 602  00a5 c6530a        	ld	a,21258
 603  00a8 a4cf          	and	a,#207
 604  00aa c7530a        	ld	21258,a
 605                     ; 154   TIM5->CCER1 |= (uint8_t)((uint8_t)(TIM5_OutputState  & TIM5_CCER1_CC2E )| \
 605                     ; 155     (uint8_t)(TIM5_OCPolarity & TIM5_CCER1_CC2P));
 607  00ad 7b08          	ld	a,(OFST+7,sp)
 608  00af a420          	and	a,#32
 609  00b1 6b01          	ld	(OFST+0,sp),a
 610  00b3 9f            	ld	a,xl
 611  00b4 a410          	and	a,#16
 612  00b6 1a01          	or	a,(OFST+0,sp)
 613  00b8 ca530a        	or	a,21258
 614  00bb c7530a        	ld	21258,a
 615                     ; 159   TIM5->CCMR2 = (uint8_t)((uint8_t)(TIM5->CCMR2 & (uint8_t)(~TIM5_CCMR_OCM)) |
 615                     ; 160                           (uint8_t)TIM5_OCMode);
 617  00be c65308        	ld	a,21256
 618  00c1 a48f          	and	a,#143
 619  00c3 1a02          	or	a,(OFST+1,sp)
 620  00c5 c75308        	ld	21256,a
 621                     ; 163   TIM5->CCR2H = (uint8_t)(TIM5_Pulse >> 8);
 623  00c8 7b06          	ld	a,(OFST+5,sp)
 624  00ca c75313        	ld	21267,a
 625                     ; 164   TIM5->CCR2L = (uint8_t)(TIM5_Pulse);
 627  00cd 7b07          	ld	a,(OFST+6,sp)
 628  00cf c75314        	ld	21268,a
 629                     ; 165 }
 632  00d2 5b03          	addw	sp,#3
 633  00d4 81            	ret
 697                     ; 175 void TIM5_OC3Init(TIM5_OCMode_TypeDef TIM5_OCMode,
 697                     ; 176                   TIM5_OutputState_TypeDef TIM5_OutputState,
 697                     ; 177                   uint16_t TIM5_Pulse,
 697                     ; 178                   TIM5_OCPolarity_TypeDef TIM5_OCPolarity)
 697                     ; 179 {
 698                     	switch	.text
 699  00d5               _TIM5_OC3Init:
 701  00d5 89            	pushw	x
 702  00d6 88            	push	a
 703       00000001      OFST:	set	1
 706                     ; 181   assert_param(IS_TIM5_OC_MODE_OK(TIM5_OCMode));
 708                     ; 182   assert_param(IS_TIM5_OUTPUT_STATE_OK(TIM5_OutputState));
 710                     ; 183   assert_param(IS_TIM5_OC_POLARITY_OK(TIM5_OCPolarity));
 712                     ; 185   TIM5->CCER2 &= (uint8_t)(~( TIM5_CCER2_CC3E  | TIM5_CCER2_CC3P));
 714  00d7 c6530b        	ld	a,21259
 715  00da a4fc          	and	a,#252
 716  00dc c7530b        	ld	21259,a
 717                     ; 187   TIM5->CCER2 |= (uint8_t)((uint8_t)(TIM5_OutputState  & TIM5_CCER2_CC3E   )|
 717                     ; 188                            (uint8_t)(TIM5_OCPolarity   & TIM5_CCER2_CC3P   ));
 719  00df 7b08          	ld	a,(OFST+7,sp)
 720  00e1 a402          	and	a,#2
 721  00e3 6b01          	ld	(OFST+0,sp),a
 722  00e5 9f            	ld	a,xl
 723  00e6 a401          	and	a,#1
 724  00e8 1a01          	or	a,(OFST+0,sp)
 725  00ea ca530b        	or	a,21259
 726  00ed c7530b        	ld	21259,a
 727                     ; 191   TIM5->CCMR3 = (uint8_t)((uint8_t)(TIM5->CCMR3 & (uint8_t)(~TIM5_CCMR_OCM)) | (uint8_t)TIM5_OCMode);
 729  00f0 c65309        	ld	a,21257
 730  00f3 a48f          	and	a,#143
 731  00f5 1a02          	or	a,(OFST+1,sp)
 732  00f7 c75309        	ld	21257,a
 733                     ; 194   TIM5->CCR3H = (uint8_t)(TIM5_Pulse >> 8);
 735  00fa 7b06          	ld	a,(OFST+5,sp)
 736  00fc c75315        	ld	21269,a
 737                     ; 195   TIM5->CCR3L = (uint8_t)(TIM5_Pulse);
 739  00ff 7b07          	ld	a,(OFST+6,sp)
 740  0101 c75316        	ld	21270,a
 741                     ; 196 }
 744  0104 5b03          	addw	sp,#3
 745  0106 81            	ret
 938                     ; 207 void TIM5_ICInit(TIM5_Channel_TypeDef TIM5_Channel,
 938                     ; 208                  TIM5_ICPolarity_TypeDef TIM5_ICPolarity,
 938                     ; 209                  TIM5_ICSelection_TypeDef TIM5_ICSelection,
 938                     ; 210                  TIM5_ICPSC_TypeDef TIM5_ICPrescaler,
 938                     ; 211                  uint8_t TIM5_ICFilter)
 938                     ; 212 {
 939                     	switch	.text
 940  0107               _TIM5_ICInit:
 942  0107 89            	pushw	x
 943       00000000      OFST:	set	0
 946                     ; 214   assert_param(IS_TIM5_CHANNEL_OK(TIM5_Channel));
 948                     ; 215   assert_param(IS_TIM5_IC_POLARITY_OK(TIM5_ICPolarity));
 950                     ; 216   assert_param(IS_TIM5_IC_SELECTION_OK(TIM5_ICSelection));
 952                     ; 217   assert_param(IS_TIM5_IC_PRESCALER_OK(TIM5_ICPrescaler));
 954                     ; 218   assert_param(IS_TIM5_IC_FILTER_OK(TIM5_ICFilter));
 956                     ; 220   if (TIM5_Channel == TIM5_CHANNEL_1)
 958  0108 9e            	ld	a,xh
 959  0109 4d            	tnz	a
 960  010a 2614          	jrne	L104
 961                     ; 223     TI1_Config((uint8_t)TIM5_ICPolarity,
 961                     ; 224                (uint8_t)TIM5_ICSelection,
 961                     ; 225                (uint8_t)TIM5_ICFilter);
 963  010c 7b07          	ld	a,(OFST+7,sp)
 964  010e 88            	push	a
 965  010f 7b06          	ld	a,(OFST+6,sp)
 966  0111 97            	ld	xl,a
 967  0112 7b03          	ld	a,(OFST+3,sp)
 968  0114 95            	ld	xh,a
 969  0115 cd0432        	call	L3_TI1_Config
 971  0118 84            	pop	a
 972                     ; 228     TIM5_SetIC1Prescaler(TIM5_ICPrescaler);
 974  0119 7b06          	ld	a,(OFST+6,sp)
 975  011b cd0348        	call	_TIM5_SetIC1Prescaler
 978  011e 202c          	jra	L304
 979  0120               L104:
 980                     ; 230   else if (TIM5_Channel == TIM5_CHANNEL_2)
 982  0120 7b01          	ld	a,(OFST+1,sp)
 983  0122 a101          	cp	a,#1
 984  0124 2614          	jrne	L504
 985                     ; 233     TI2_Config((uint8_t)TIM5_ICPolarity,
 985                     ; 234                (uint8_t)TIM5_ICSelection,
 985                     ; 235                (uint8_t)TIM5_ICFilter);
 987  0126 7b07          	ld	a,(OFST+7,sp)
 988  0128 88            	push	a
 989  0129 7b06          	ld	a,(OFST+6,sp)
 990  012b 97            	ld	xl,a
 991  012c 7b03          	ld	a,(OFST+3,sp)
 992  012e 95            	ld	xh,a
 993  012f cd0462        	call	L5_TI2_Config
 995  0132 84            	pop	a
 996                     ; 238     TIM5_SetIC2Prescaler(TIM5_ICPrescaler);
 998  0133 7b06          	ld	a,(OFST+6,sp)
 999  0135 cd0355        	call	_TIM5_SetIC2Prescaler
1002  0138 2012          	jra	L304
1003  013a               L504:
1004                     ; 243     TI3_Config((uint8_t)TIM5_ICPolarity,
1004                     ; 244                (uint8_t)TIM5_ICSelection,
1004                     ; 245                (uint8_t)TIM5_ICFilter);
1006  013a 7b07          	ld	a,(OFST+7,sp)
1007  013c 88            	push	a
1008  013d 7b06          	ld	a,(OFST+6,sp)
1009  013f 97            	ld	xl,a
1010  0140 7b03          	ld	a,(OFST+3,sp)
1011  0142 95            	ld	xh,a
1012  0143 cd0492        	call	L7_TI3_Config
1014  0146 84            	pop	a
1015                     ; 248     TIM5_SetIC3Prescaler(TIM5_ICPrescaler);
1017  0147 7b06          	ld	a,(OFST+6,sp)
1018  0149 cd0362        	call	_TIM5_SetIC3Prescaler
1020  014c               L304:
1021                     ; 250 }
1024  014c 85            	popw	x
1025  014d 81            	ret
1121                     ; 261 void TIM5_PWMIConfig(TIM5_Channel_TypeDef TIM5_Channel,
1121                     ; 262                      TIM5_ICPolarity_TypeDef TIM5_ICPolarity,
1121                     ; 263                      TIM5_ICSelection_TypeDef TIM5_ICSelection,
1121                     ; 264                      TIM5_ICPSC_TypeDef TIM5_ICPrescaler,
1121                     ; 265                      uint8_t TIM5_ICFilter)
1121                     ; 266 {
1122                     	switch	.text
1123  014e               _TIM5_PWMIConfig:
1125  014e 89            	pushw	x
1126  014f 89            	pushw	x
1127       00000002      OFST:	set	2
1130                     ; 267   uint8_t icpolarity = (uint8_t)TIM5_ICPOLARITY_RISING;
1132                     ; 268   uint8_t icselection = (uint8_t)TIM5_ICSELECTION_DIRECTTI;
1134                     ; 271   assert_param(IS_TIM5_PWMI_CHANNEL_OK(TIM5_Channel));
1136                     ; 272   assert_param(IS_TIM5_IC_POLARITY_OK(TIM5_ICPolarity));
1138                     ; 273   assert_param(IS_TIM5_IC_SELECTION_OK(TIM5_ICSelection));
1140                     ; 274   assert_param(IS_TIM5_IC_PRESCALER_OK(TIM5_ICPrescaler));
1142                     ; 277   if (TIM5_ICPolarity != TIM5_ICPOLARITY_FALLING)
1144  0150 9f            	ld	a,xl
1145  0151 a144          	cp	a,#68
1146  0153 2706          	jreq	L754
1147                     ; 279     icpolarity = (uint8_t)TIM5_ICPOLARITY_FALLING;
1149  0155 a644          	ld	a,#68
1150  0157 6b01          	ld	(OFST-1,sp),a
1152  0159 2002          	jra	L164
1153  015b               L754:
1154                     ; 283     icpolarity = (uint8_t)TIM5_ICPOLARITY_RISING;
1156  015b 0f01          	clr	(OFST-1,sp)
1157  015d               L164:
1158                     ; 287   if (TIM5_ICSelection == TIM5_ICSELECTION_DIRECTTI)
1160  015d 7b07          	ld	a,(OFST+5,sp)
1161  015f a101          	cp	a,#1
1162  0161 2606          	jrne	L364
1163                     ; 289     icselection = (uint8_t)TIM5_ICSELECTION_INDIRECTTI;
1165  0163 a602          	ld	a,#2
1166  0165 6b02          	ld	(OFST+0,sp),a
1168  0167 2004          	jra	L564
1169  0169               L364:
1170                     ; 293     icselection = (uint8_t)TIM5_ICSELECTION_DIRECTTI;
1172  0169 a601          	ld	a,#1
1173  016b 6b02          	ld	(OFST+0,sp),a
1174  016d               L564:
1175                     ; 296   if (TIM5_Channel == TIM5_CHANNEL_1)
1177  016d 0d03          	tnz	(OFST+1,sp)
1178  016f 2626          	jrne	L764
1179                     ; 299     TI1_Config((uint8_t)TIM5_ICPolarity, (uint8_t)TIM5_ICSelection,
1179                     ; 300                (uint8_t)TIM5_ICFilter);
1181  0171 7b09          	ld	a,(OFST+7,sp)
1182  0173 88            	push	a
1183  0174 7b08          	ld	a,(OFST+6,sp)
1184  0176 97            	ld	xl,a
1185  0177 7b05          	ld	a,(OFST+3,sp)
1186  0179 95            	ld	xh,a
1187  017a cd0432        	call	L3_TI1_Config
1189  017d 84            	pop	a
1190                     ; 303     TIM5_SetIC1Prescaler(TIM5_ICPrescaler);
1192  017e 7b08          	ld	a,(OFST+6,sp)
1193  0180 cd0348        	call	_TIM5_SetIC1Prescaler
1195                     ; 306     TI2_Config((uint8_t)icpolarity, (uint8_t)icselection, (uint8_t)TIM5_ICFilter);
1197  0183 7b09          	ld	a,(OFST+7,sp)
1198  0185 88            	push	a
1199  0186 7b03          	ld	a,(OFST+1,sp)
1200  0188 97            	ld	xl,a
1201  0189 7b02          	ld	a,(OFST+0,sp)
1202  018b 95            	ld	xh,a
1203  018c cd0462        	call	L5_TI2_Config
1205  018f 84            	pop	a
1206                     ; 309     TIM5_SetIC2Prescaler(TIM5_ICPrescaler);
1208  0190 7b08          	ld	a,(OFST+6,sp)
1209  0192 cd0355        	call	_TIM5_SetIC2Prescaler
1212  0195 2024          	jra	L174
1213  0197               L764:
1214                     ; 314     TI2_Config((uint8_t)TIM5_ICPolarity, (uint8_t)TIM5_ICSelection,
1214                     ; 315                (uint8_t)TIM5_ICFilter);
1216  0197 7b09          	ld	a,(OFST+7,sp)
1217  0199 88            	push	a
1218  019a 7b08          	ld	a,(OFST+6,sp)
1219  019c 97            	ld	xl,a
1220  019d 7b05          	ld	a,(OFST+3,sp)
1221  019f 95            	ld	xh,a
1222  01a0 cd0462        	call	L5_TI2_Config
1224  01a3 84            	pop	a
1225                     ; 318     TIM5_SetIC2Prescaler(TIM5_ICPrescaler);
1227  01a4 7b08          	ld	a,(OFST+6,sp)
1228  01a6 cd0355        	call	_TIM5_SetIC2Prescaler
1230                     ; 321     TI1_Config((uint8_t)icpolarity, (uint8_t)icselection, (uint8_t)TIM5_ICFilter);
1232  01a9 7b09          	ld	a,(OFST+7,sp)
1233  01ab 88            	push	a
1234  01ac 7b03          	ld	a,(OFST+1,sp)
1235  01ae 97            	ld	xl,a
1236  01af 7b02          	ld	a,(OFST+0,sp)
1237  01b1 95            	ld	xh,a
1238  01b2 cd0432        	call	L3_TI1_Config
1240  01b5 84            	pop	a
1241                     ; 324     TIM5_SetIC1Prescaler(TIM5_ICPrescaler);
1243  01b6 7b08          	ld	a,(OFST+6,sp)
1244  01b8 cd0348        	call	_TIM5_SetIC1Prescaler
1246  01bb               L174:
1247                     ; 326 }
1250  01bb 5b04          	addw	sp,#4
1251  01bd 81            	ret
1306                     ; 334 void TIM5_Cmd(FunctionalState NewState)
1306                     ; 335 {
1307                     	switch	.text
1308  01be               _TIM5_Cmd:
1312                     ; 337   assert_param(IS_FUNCTIONALSTATE_OK(NewState));
1314                     ; 340   if (NewState != DISABLE)
1316  01be 4d            	tnz	a
1317  01bf 2706          	jreq	L125
1318                     ; 342     TIM5->CR1 |= TIM5_CR1_CEN ;
1320  01c1 72105300      	bset	21248,#0
1322  01c5 2004          	jra	L325
1323  01c7               L125:
1324                     ; 346     TIM5->CR1 &= (uint8_t)(~TIM5_CR1_CEN) ;
1326  01c7 72115300      	bres	21248,#0
1327  01cb               L325:
1328                     ; 348 }
1331  01cb 81            	ret
1417                     ; 363 void TIM5_ITConfig(TIM5_IT_TypeDef TIM5_IT, FunctionalState NewState)
1417                     ; 364 {
1418                     	switch	.text
1419  01cc               _TIM5_ITConfig:
1421  01cc 89            	pushw	x
1422       00000000      OFST:	set	0
1425                     ; 366   assert_param(IS_TIM5_IT_OK(TIM5_IT));
1427                     ; 367   assert_param(IS_FUNCTIONALSTATE_OK(NewState));
1429                     ; 369   if (NewState != DISABLE)
1431  01cd 9f            	ld	a,xl
1432  01ce 4d            	tnz	a
1433  01cf 2709          	jreq	L565
1434                     ; 372     TIM5->IER |= (uint8_t)TIM5_IT;
1436  01d1 9e            	ld	a,xh
1437  01d2 ca5303        	or	a,21251
1438  01d5 c75303        	ld	21251,a
1440  01d8 2009          	jra	L765
1441  01da               L565:
1442                     ; 377     TIM5->IER &= (uint8_t)(~TIM5_IT);
1444  01da 7b01          	ld	a,(OFST+1,sp)
1445  01dc 43            	cpl	a
1446  01dd c45303        	and	a,21251
1447  01e0 c75303        	ld	21251,a
1448  01e3               L765:
1449                     ; 379 }
1452  01e3 85            	popw	x
1453  01e4 81            	ret
1489                     ; 387 void TIM5_UpdateDisableConfig(FunctionalState NewState)
1489                     ; 388 {
1490                     	switch	.text
1491  01e5               _TIM5_UpdateDisableConfig:
1495                     ; 390   assert_param(IS_FUNCTIONALSTATE_OK(NewState));
1497                     ; 393   if (NewState != DISABLE)
1499  01e5 4d            	tnz	a
1500  01e6 2706          	jreq	L706
1501                     ; 395     TIM5->CR1 |= TIM5_CR1_UDIS ;
1503  01e8 72125300      	bset	21248,#1
1505  01ec 2004          	jra	L116
1506  01ee               L706:
1507                     ; 399     TIM5->CR1 &= (uint8_t)(~TIM5_CR1_UDIS) ;
1509  01ee 72135300      	bres	21248,#1
1510  01f2               L116:
1511                     ; 401 }
1514  01f2 81            	ret
1572                     ; 411 void TIM5_UpdateRequestConfig(TIM5_UpdateSource_TypeDef TIM5_UpdateSource)
1572                     ; 412 {
1573                     	switch	.text
1574  01f3               _TIM5_UpdateRequestConfig:
1578                     ; 414   assert_param(IS_TIM5_UPDATE_SOURCE_OK(TIM5_UpdateSource));
1580                     ; 417   if (TIM5_UpdateSource != TIM5_UPDATESOURCE_GLOBAL)
1582  01f3 4d            	tnz	a
1583  01f4 2706          	jreq	L146
1584                     ; 419     TIM5->CR1 |= TIM5_CR1_URS ;
1586  01f6 72145300      	bset	21248,#2
1588  01fa 2004          	jra	L346
1589  01fc               L146:
1590                     ; 423     TIM5->CR1 &= (uint8_t)(~TIM5_CR1_URS) ;
1592  01fc 72155300      	bres	21248,#2
1593  0200               L346:
1594                     ; 425 }
1597  0200 81            	ret
1654                     ; 435 void TIM5_SelectOnePulseMode(TIM5_OPMode_TypeDef TIM5_OPMode)
1654                     ; 436 {
1655                     	switch	.text
1656  0201               _TIM5_SelectOnePulseMode:
1660                     ; 438   assert_param(IS_TIM5_OPM_MODE_OK(TIM5_OPMode));
1662                     ; 441   if (TIM5_OPMode != TIM5_OPMODE_REPETITIVE)
1664  0201 4d            	tnz	a
1665  0202 2706          	jreq	L376
1666                     ; 443     TIM5->CR1 |= TIM5_CR1_OPM ;
1668  0204 72165300      	bset	21248,#3
1670  0208 2004          	jra	L576
1671  020a               L376:
1672                     ; 447     TIM5->CR1 &= (uint8_t)(~TIM5_CR1_OPM) ;
1674  020a 72175300      	bres	21248,#3
1675  020e               L576:
1676                     ; 449 }
1679  020e 81            	ret
1747                     ; 479 void TIM5_PrescalerConfig(TIM5_Prescaler_TypeDef Prescaler,
1747                     ; 480                           TIM5_PSCReloadMode_TypeDef TIM5_PSCReloadMode)
1747                     ; 481 {
1748                     	switch	.text
1749  020f               _TIM5_PrescalerConfig:
1753                     ; 483   assert_param(IS_TIM5_PRESCALER_RELOAD_OK(TIM5_PSCReloadMode));
1755                     ; 484   assert_param(IS_TIM5_PRESCALER_OK(Prescaler));
1757                     ; 487   TIM5->PSCR = (uint8_t)Prescaler;
1759  020f 9e            	ld	a,xh
1760  0210 c7530e        	ld	21262,a
1761                     ; 490   TIM5->EGR = (uint8_t)TIM5_PSCReloadMode ;
1763  0213 9f            	ld	a,xl
1764  0214 c75306        	ld	21254,a
1765                     ; 491 }
1768  0217 81            	ret
1826                     ; 502 void TIM5_ForcedOC1Config(TIM5_ForcedAction_TypeDef TIM5_ForcedAction)
1826                     ; 503 {
1827                     	switch	.text
1828  0218               _TIM5_ForcedOC1Config:
1830  0218 88            	push	a
1831       00000000      OFST:	set	0
1834                     ; 505   assert_param(IS_TIM5_FORCED_ACTION_OK(TIM5_ForcedAction));
1836                     ; 508   TIM5->CCMR1  =  (uint8_t)((uint8_t)(TIM5->CCMR1 & (uint8_t)(~TIM5_CCMR_OCM))
1836                     ; 509                             | (uint8_t)TIM5_ForcedAction);
1838  0219 c65307        	ld	a,21255
1839  021c a48f          	and	a,#143
1840  021e 1a01          	or	a,(OFST+1,sp)
1841  0220 c75307        	ld	21255,a
1842                     ; 510 }
1845  0223 84            	pop	a
1846  0224 81            	ret
1882                     ; 521 void TIM5_ForcedOC2Config(TIM5_ForcedAction_TypeDef TIM5_ForcedAction)
1882                     ; 522 {
1883                     	switch	.text
1884  0225               _TIM5_ForcedOC2Config:
1886  0225 88            	push	a
1887       00000000      OFST:	set	0
1890                     ; 524   assert_param(IS_TIM5_FORCED_ACTION_OK(TIM5_ForcedAction));
1892                     ; 527   TIM5->CCMR2  =  (uint8_t)((uint8_t)(TIM5->CCMR2 & (uint8_t)(~TIM5_CCMR_OCM))
1892                     ; 528                             | (uint8_t)TIM5_ForcedAction);
1894  0226 c65308        	ld	a,21256
1895  0229 a48f          	and	a,#143
1896  022b 1a01          	or	a,(OFST+1,sp)
1897  022d c75308        	ld	21256,a
1898                     ; 529 }
1901  0230 84            	pop	a
1902  0231 81            	ret
1938                     ; 540 void TIM5_ForcedOC3Config(TIM5_ForcedAction_TypeDef TIM5_ForcedAction)
1938                     ; 541 {
1939                     	switch	.text
1940  0232               _TIM5_ForcedOC3Config:
1942  0232 88            	push	a
1943       00000000      OFST:	set	0
1946                     ; 543   assert_param(IS_TIM5_FORCED_ACTION_OK(TIM5_ForcedAction));
1948                     ; 546   TIM5->CCMR3  =  (uint8_t)((uint8_t)(TIM5->CCMR3 & (uint8_t)(~TIM5_CCMR_OCM))  
1948                     ; 547                             | (uint8_t)TIM5_ForcedAction);
1950  0233 c65309        	ld	a,21257
1951  0236 a48f          	and	a,#143
1952  0238 1a01          	or	a,(OFST+1,sp)
1953  023a c75309        	ld	21257,a
1954                     ; 548 }
1957  023d 84            	pop	a
1958  023e 81            	ret
1994                     ; 556 void TIM5_ARRPreloadConfig(FunctionalState NewState)
1994                     ; 557 {
1995                     	switch	.text
1996  023f               _TIM5_ARRPreloadConfig:
2000                     ; 559   assert_param(IS_FUNCTIONALSTATE_OK(NewState));
2002                     ; 562   if (NewState != DISABLE)
2004  023f 4d            	tnz	a
2005  0240 2706          	jreq	L1301
2006                     ; 564     TIM5->CR1 |= TIM5_CR1_ARPE ;
2008  0242 721e5300      	bset	21248,#7
2010  0246 2004          	jra	L3301
2011  0248               L1301:
2012                     ; 568     TIM5->CR1 &= (uint8_t)(~TIM5_CR1_ARPE) ;
2014  0248 721f5300      	bres	21248,#7
2015  024c               L3301:
2016                     ; 570 }
2019  024c 81            	ret
2055                     ; 578 void TIM5_OC1PreloadConfig(FunctionalState NewState)
2055                     ; 579 {
2056                     	switch	.text
2057  024d               _TIM5_OC1PreloadConfig:
2061                     ; 581   assert_param(IS_FUNCTIONALSTATE_OK(NewState));
2063                     ; 584   if (NewState != DISABLE)
2065  024d 4d            	tnz	a
2066  024e 2706          	jreq	L3501
2067                     ; 586     TIM5->CCMR1 |= TIM5_CCMR_OCxPE ;
2069  0250 72165307      	bset	21255,#3
2071  0254 2004          	jra	L5501
2072  0256               L3501:
2073                     ; 590     TIM5->CCMR1 &= (uint8_t)(~TIM5_CCMR_OCxPE) ;
2075  0256 72175307      	bres	21255,#3
2076  025a               L5501:
2077                     ; 592 }
2080  025a 81            	ret
2116                     ; 600 void TIM5_OC2PreloadConfig(FunctionalState NewState)
2116                     ; 601 {
2117                     	switch	.text
2118  025b               _TIM5_OC2PreloadConfig:
2122                     ; 603   assert_param(IS_FUNCTIONALSTATE_OK(NewState));
2124                     ; 606   if (NewState != DISABLE)
2126  025b 4d            	tnz	a
2127  025c 2706          	jreq	L5701
2128                     ; 608     TIM5->CCMR2 |= TIM5_CCMR_OCxPE ;
2130  025e 72165308      	bset	21256,#3
2132  0262 2004          	jra	L7701
2133  0264               L5701:
2134                     ; 612     TIM5->CCMR2 &= (uint8_t)(~TIM5_CCMR_OCxPE) ;
2136  0264 72175308      	bres	21256,#3
2137  0268               L7701:
2138                     ; 614 }
2141  0268 81            	ret
2177                     ; 622 void TIM5_OC3PreloadConfig(FunctionalState NewState)
2177                     ; 623 {
2178                     	switch	.text
2179  0269               _TIM5_OC3PreloadConfig:
2183                     ; 625   assert_param(IS_FUNCTIONALSTATE_OK(NewState));
2185                     ; 628   if (NewState != DISABLE)
2187  0269 4d            	tnz	a
2188  026a 2706          	jreq	L7111
2189                     ; 630     TIM5->CCMR3 |= TIM5_CCMR_OCxPE ;
2191  026c 72165309      	bset	21257,#3
2193  0270 2004          	jra	L1211
2194  0272               L7111:
2195                     ; 634     TIM5->CCMR3 &= (uint8_t)(~TIM5_CCMR_OCxPE) ;
2197  0272 72175309      	bres	21257,#3
2198  0276               L1211:
2199                     ; 636 }
2202  0276 81            	ret
2283                     ; 648 void TIM5_GenerateEvent(TIM5_EventSource_TypeDef TIM5_EventSource)
2283                     ; 649 {
2284                     	switch	.text
2285  0277               _TIM5_GenerateEvent:
2289                     ; 651   assert_param(IS_TIM5_EVENT_SOURCE_OK(TIM5_EventSource));
2291                     ; 654   TIM5->EGR = (uint8_t)TIM5_EventSource;
2293  0277 c75306        	ld	21254,a
2294                     ; 655 }
2297  027a 81            	ret
2333                     ; 665 void TIM5_OC1PolarityConfig(TIM5_OCPolarity_TypeDef TIM5_OCPolarity)
2333                     ; 666 {
2334                     	switch	.text
2335  027b               _TIM5_OC1PolarityConfig:
2339                     ; 668     assert_param(IS_TIM5_OC_POLARITY_OK(TIM5_OCPolarity));
2341                     ; 671     if (TIM5_OCPolarity != TIM5_OCPOLARITY_HIGH)
2343  027b 4d            	tnz	a
2344  027c 2706          	jreq	L5711
2345                     ; 673         TIM5->CCER1 |= TIM5_CCER1_CC1P ;
2347  027e 7212530a      	bset	21258,#1
2349  0282 2004          	jra	L7711
2350  0284               L5711:
2351                     ; 677         TIM5->CCER1 &= (uint8_t)(~TIM5_CCER1_CC1P) ;
2353  0284 7213530a      	bres	21258,#1
2354  0288               L7711:
2355                     ; 679 }
2358  0288 81            	ret
2394                     ; 690 void TIM5_OC2PolarityConfig(TIM5_OCPolarity_TypeDef TIM5_OCPolarity)
2394                     ; 691 {
2395                     	switch	.text
2396  0289               _TIM5_OC2PolarityConfig:
2400                     ; 693   assert_param(IS_TIM5_OC_POLARITY_OK(TIM5_OCPolarity));
2402                     ; 696   if (TIM5_OCPolarity != TIM5_OCPOLARITY_HIGH)
2404  0289 4d            	tnz	a
2405  028a 2706          	jreq	L7121
2406                     ; 698     TIM5->CCER1 |= TIM5_CCER1_CC2P ;
2408  028c 721a530a      	bset	21258,#5
2410  0290 2004          	jra	L1221
2411  0292               L7121:
2412                     ; 702     TIM5->CCER1 &= (uint8_t)(~TIM5_CCER1_CC2P) ;
2414  0292 721b530a      	bres	21258,#5
2415  0296               L1221:
2416                     ; 704 }
2419  0296 81            	ret
2455                     ; 714 void TIM5_OC3PolarityConfig(TIM5_OCPolarity_TypeDef TIM5_OCPolarity)
2455                     ; 715 {
2456                     	switch	.text
2457  0297               _TIM5_OC3PolarityConfig:
2461                     ; 717   assert_param(IS_TIM5_OC_POLARITY_OK(TIM5_OCPolarity));
2463                     ; 720   if (TIM5_OCPolarity != TIM5_OCPOLARITY_HIGH)
2465  0297 4d            	tnz	a
2466  0298 2706          	jreq	L1421
2467                     ; 722     TIM5->CCER2 |= TIM5_CCER2_CC3P ;
2469  029a 7212530b      	bset	21259,#1
2471  029e 2004          	jra	L3421
2472  02a0               L1421:
2473                     ; 726     TIM5->CCER2 &= (uint8_t)(~TIM5_CCER2_CC3P) ;
2475  02a0 7213530b      	bres	21259,#1
2476  02a4               L3421:
2477                     ; 728 }
2480  02a4 81            	ret
2525                     ; 741 void TIM5_CCxCmd(TIM5_Channel_TypeDef TIM5_Channel, FunctionalState NewState)
2525                     ; 742 {
2526                     	switch	.text
2527  02a5               _TIM5_CCxCmd:
2529  02a5 89            	pushw	x
2530       00000000      OFST:	set	0
2533                     ; 744   assert_param(IS_TIM5_CHANNEL_OK(TIM5_Channel));
2535                     ; 745   assert_param(IS_FUNCTIONALSTATE_OK(NewState));
2537                     ; 747   if (TIM5_Channel == TIM5_CHANNEL_1)
2539  02a6 9e            	ld	a,xh
2540  02a7 4d            	tnz	a
2541  02a8 2610          	jrne	L7621
2542                     ; 750     if (NewState != DISABLE)
2544  02aa 9f            	ld	a,xl
2545  02ab 4d            	tnz	a
2546  02ac 2706          	jreq	L1721
2547                     ; 752       TIM5->CCER1 |= TIM5_CCER1_CC1E ;
2549  02ae 7210530a      	bset	21258,#0
2551  02b2 202a          	jra	L5721
2552  02b4               L1721:
2553                     ; 756       TIM5->CCER1 &= (uint8_t)(~TIM5_CCER1_CC1E) ;
2555  02b4 7211530a      	bres	21258,#0
2556  02b8 2024          	jra	L5721
2557  02ba               L7621:
2558                     ; 760   else if (TIM5_Channel == TIM5_CHANNEL_2)
2560  02ba 7b01          	ld	a,(OFST+1,sp)
2561  02bc a101          	cp	a,#1
2562  02be 2610          	jrne	L7721
2563                     ; 763     if (NewState != DISABLE)
2565  02c0 0d02          	tnz	(OFST+2,sp)
2566  02c2 2706          	jreq	L1031
2567                     ; 765       TIM5->CCER1 |= TIM5_CCER1_CC2E;
2569  02c4 7218530a      	bset	21258,#4
2571  02c8 2014          	jra	L5721
2572  02ca               L1031:
2573                     ; 769       TIM5->CCER1 &= (uint8_t)(~TIM5_CCER1_CC2E) ;
2575  02ca 7219530a      	bres	21258,#4
2576  02ce 200e          	jra	L5721
2577  02d0               L7721:
2578                     ; 775     if (NewState != DISABLE)
2580  02d0 0d02          	tnz	(OFST+2,sp)
2581  02d2 2706          	jreq	L7031
2582                     ; 777       TIM5->CCER2 |= TIM5_CCER2_CC3E;
2584  02d4 7210530b      	bset	21259,#0
2586  02d8 2004          	jra	L5721
2587  02da               L7031:
2588                     ; 781       TIM5->CCER2 &= (uint8_t)(~TIM5_CCER2_CC3E) ;
2590  02da 7211530b      	bres	21259,#0
2591  02de               L5721:
2592                     ; 784 }
2595  02de 85            	popw	x
2596  02df 81            	ret
2641                     ; 806 void TIM5_SelectOCxM(TIM5_Channel_TypeDef TIM5_Channel, TIM5_OCMode_TypeDef TIM5_OCMode)
2641                     ; 807 {
2642                     	switch	.text
2643  02e0               _TIM5_SelectOCxM:
2645  02e0 89            	pushw	x
2646       00000000      OFST:	set	0
2649                     ; 809   assert_param(IS_TIM5_CHANNEL_OK(TIM5_Channel));
2651                     ; 810   assert_param(IS_TIM5_OCM_OK(TIM5_OCMode));
2653                     ; 812   if (TIM5_Channel == TIM5_CHANNEL_1)
2655  02e1 9e            	ld	a,xh
2656  02e2 4d            	tnz	a
2657  02e3 2610          	jrne	L5331
2658                     ; 815     TIM5->CCER1 &= (uint8_t)(~TIM5_CCER1_CC1E);
2660  02e5 7211530a      	bres	21258,#0
2661                     ; 818     TIM5->CCMR1 = (uint8_t)((uint8_t)(TIM5->CCMR1 & (uint8_t)(~TIM5_CCMR_OCM)) 
2661                     ; 819                             | (uint8_t)TIM5_OCMode);
2663  02e9 c65307        	ld	a,21255
2664  02ec a48f          	and	a,#143
2665  02ee 1a02          	or	a,(OFST+2,sp)
2666  02f0 c75307        	ld	21255,a
2668  02f3 2024          	jra	L7331
2669  02f5               L5331:
2670                     ; 821   else if (TIM5_Channel == TIM5_CHANNEL_2)
2672  02f5 7b01          	ld	a,(OFST+1,sp)
2673  02f7 a101          	cp	a,#1
2674  02f9 2610          	jrne	L1431
2675                     ; 824     TIM5->CCER1 &= (uint8_t)(~TIM5_CCER1_CC2E);
2677  02fb 7219530a      	bres	21258,#4
2678                     ; 827     TIM5->CCMR2 = (uint8_t)((uint8_t)(TIM5->CCMR2 & (uint8_t)(~TIM5_CCMR_OCM))
2678                     ; 828                             | (uint8_t)TIM5_OCMode);
2680  02ff c65308        	ld	a,21256
2681  0302 a48f          	and	a,#143
2682  0304 1a02          	or	a,(OFST+2,sp)
2683  0306 c75308        	ld	21256,a
2685  0309 200e          	jra	L7331
2686  030b               L1431:
2687                     ; 833     TIM5->CCER2 &= (uint8_t)(~TIM5_CCER2_CC3E);
2689  030b 7211530b      	bres	21259,#0
2690                     ; 836     TIM5->CCMR3 = (uint8_t)((uint8_t)(TIM5->CCMR3 & (uint8_t)(~TIM5_CCMR_OCM))
2690                     ; 837                             | (uint8_t)TIM5_OCMode);
2692  030f c65309        	ld	a,21257
2693  0312 a48f          	and	a,#143
2694  0314 1a02          	or	a,(OFST+2,sp)
2695  0316 c75309        	ld	21257,a
2696  0319               L7331:
2697                     ; 839 }
2700  0319 85            	popw	x
2701  031a 81            	ret
2735                     ; 847 void TIM5_SetCounter(uint16_t Counter)
2735                     ; 848 {
2736                     	switch	.text
2737  031b               _TIM5_SetCounter:
2741                     ; 850   TIM5->CNTRH = (uint8_t)(Counter >> 8);
2743  031b 9e            	ld	a,xh
2744  031c c7530c        	ld	21260,a
2745                     ; 851   TIM5->CNTRL = (uint8_t)(Counter);
2747  031f 9f            	ld	a,xl
2748  0320 c7530d        	ld	21261,a
2749                     ; 852 }
2752  0323 81            	ret
2786                     ; 860 void TIM5_SetAutoreload(uint16_t Autoreload)
2786                     ; 861 {
2787                     	switch	.text
2788  0324               _TIM5_SetAutoreload:
2792                     ; 863   TIM5->ARRH = (uint8_t)(Autoreload >> 8);
2794  0324 9e            	ld	a,xh
2795  0325 c7530f        	ld	21263,a
2796                     ; 864   TIM5->ARRL = (uint8_t)(Autoreload);
2798  0328 9f            	ld	a,xl
2799  0329 c75310        	ld	21264,a
2800                     ; 865 }
2803  032c 81            	ret
2837                     ; 873 void TIM5_SetCompare1(uint16_t Compare1)
2837                     ; 874 {
2838                     	switch	.text
2839  032d               _TIM5_SetCompare1:
2843                     ; 876   TIM5->CCR1H = (uint8_t)(Compare1 >> 8);
2845  032d 9e            	ld	a,xh
2846  032e c75311        	ld	21265,a
2847                     ; 877   TIM5->CCR1L = (uint8_t)(Compare1);
2849  0331 9f            	ld	a,xl
2850  0332 c75312        	ld	21266,a
2851                     ; 878 }
2854  0335 81            	ret
2888                     ; 886 void TIM5_SetCompare2(uint16_t Compare2)
2888                     ; 887 {
2889                     	switch	.text
2890  0336               _TIM5_SetCompare2:
2894                     ; 889   TIM5->CCR2H = (uint8_t)(Compare2 >> 8);
2896  0336 9e            	ld	a,xh
2897  0337 c75313        	ld	21267,a
2898                     ; 890   TIM5->CCR2L = (uint8_t)(Compare2);
2900  033a 9f            	ld	a,xl
2901  033b c75314        	ld	21268,a
2902                     ; 891 }
2905  033e 81            	ret
2939                     ; 899 void TIM5_SetCompare3(uint16_t Compare3)
2939                     ; 900 {
2940                     	switch	.text
2941  033f               _TIM5_SetCompare3:
2945                     ; 902   TIM5->CCR3H = (uint8_t)(Compare3 >> 8);
2947  033f 9e            	ld	a,xh
2948  0340 c75315        	ld	21269,a
2949                     ; 903   TIM5->CCR3L = (uint8_t)(Compare3);
2951  0343 9f            	ld	a,xl
2952  0344 c75316        	ld	21270,a
2953                     ; 904 }
2956  0347 81            	ret
2992                     ; 916 void TIM5_SetIC1Prescaler(TIM5_ICPSC_TypeDef TIM5_IC1Prescaler)
2992                     ; 917 {
2993                     	switch	.text
2994  0348               _TIM5_SetIC1Prescaler:
2996  0348 88            	push	a
2997       00000000      OFST:	set	0
3000                     ; 919   assert_param(IS_TIM5_IC_PRESCALER_OK(TIM5_IC1Prescaler));
3002                     ; 922   TIM5->CCMR1 = (uint8_t)((uint8_t)(TIM5->CCMR1 & (uint8_t)(~TIM5_CCMR_ICxPSC))|
3002                     ; 923                           (uint8_t)TIM5_IC1Prescaler);
3004  0349 c65307        	ld	a,21255
3005  034c a4f3          	and	a,#243
3006  034e 1a01          	or	a,(OFST+1,sp)
3007  0350 c75307        	ld	21255,a
3008                     ; 924 }
3011  0353 84            	pop	a
3012  0354 81            	ret
3048                     ; 936 void TIM5_SetIC2Prescaler(TIM5_ICPSC_TypeDef TIM5_IC2Prescaler)
3048                     ; 937 {
3049                     	switch	.text
3050  0355               _TIM5_SetIC2Prescaler:
3052  0355 88            	push	a
3053       00000000      OFST:	set	0
3056                     ; 939   assert_param(IS_TIM5_IC_PRESCALER_OK(TIM5_IC2Prescaler));
3058                     ; 942   TIM5->CCMR2 = (uint8_t)((uint8_t)(TIM5->CCMR2 & (uint8_t)(~TIM5_CCMR_ICxPSC))
3058                     ; 943                           | (uint8_t)TIM5_IC2Prescaler);
3060  0356 c65308        	ld	a,21256
3061  0359 a4f3          	and	a,#243
3062  035b 1a01          	or	a,(OFST+1,sp)
3063  035d c75308        	ld	21256,a
3064                     ; 944 }
3067  0360 84            	pop	a
3068  0361 81            	ret
3104                     ; 956 void TIM5_SetIC3Prescaler(TIM5_ICPSC_TypeDef TIM5_IC3Prescaler)
3104                     ; 957 {
3105                     	switch	.text
3106  0362               _TIM5_SetIC3Prescaler:
3108  0362 88            	push	a
3109       00000000      OFST:	set	0
3112                     ; 959   assert_param(IS_TIM5_IC_PRESCALER_OK(TIM5_IC3Prescaler));
3114                     ; 961   TIM5->CCMR3 = (uint8_t)((uint8_t)(TIM5->CCMR3 & (uint8_t)(~TIM5_CCMR_ICxPSC)) |
3114                     ; 962                           (uint8_t)TIM5_IC3Prescaler);
3116  0363 c65309        	ld	a,21257
3117  0366 a4f3          	and	a,#243
3118  0368 1a01          	or	a,(OFST+1,sp)
3119  036a c75309        	ld	21257,a
3120                     ; 963 }
3123  036d 84            	pop	a
3124  036e 81            	ret
3158                     ; 970 uint16_t TIM5_GetCapture1(void)
3158                     ; 971 {
3159                     	switch	.text
3160  036f               _TIM5_GetCapture1:
3162  036f 89            	pushw	x
3163       00000002      OFST:	set	2
3166                     ; 972   uint16_t temp = 0; 
3168                     ; 974   temp = ((uint16_t)TIM5->CCR1H << 8); 
3170  0370 c65311        	ld	a,21265
3171  0373 5f            	clrw	x
3172  0374 97            	ld	xl,a
3173  0375 4f            	clr	a
3174  0376 02            	rlwa	x,a
3175  0377 1f01          	ldw	(OFST-1,sp),x
3176                     ; 977   return (uint16_t)(temp | (uint16_t)(TIM5->CCR1L));
3178  0379 c65312        	ld	a,21266
3179  037c 5f            	clrw	x
3180  037d 97            	ld	xl,a
3181  037e 01            	rrwa	x,a
3182  037f 1a02          	or	a,(OFST+0,sp)
3183  0381 01            	rrwa	x,a
3184  0382 1a01          	or	a,(OFST-1,sp)
3185  0384 01            	rrwa	x,a
3188  0385 5b02          	addw	sp,#2
3189  0387 81            	ret
3223                     ; 985 uint16_t TIM5_GetCapture2(void)
3223                     ; 986 {
3224                     	switch	.text
3225  0388               _TIM5_GetCapture2:
3227  0388 89            	pushw	x
3228       00000002      OFST:	set	2
3231                     ; 987   uint16_t temp = 0; 
3233                     ; 989   temp = ((uint16_t)TIM5->CCR2H << 8);  
3235  0389 c65313        	ld	a,21267
3236  038c 5f            	clrw	x
3237  038d 97            	ld	xl,a
3238  038e 4f            	clr	a
3239  038f 02            	rlwa	x,a
3240  0390 1f01          	ldw	(OFST-1,sp),x
3241                     ; 992   return (uint16_t)(temp | (uint16_t)(TIM5->CCR2L));
3243  0392 c65314        	ld	a,21268
3244  0395 5f            	clrw	x
3245  0396 97            	ld	xl,a
3246  0397 01            	rrwa	x,a
3247  0398 1a02          	or	a,(OFST+0,sp)
3248  039a 01            	rrwa	x,a
3249  039b 1a01          	or	a,(OFST-1,sp)
3250  039d 01            	rrwa	x,a
3253  039e 5b02          	addw	sp,#2
3254  03a0 81            	ret
3288                     ; 1000 uint16_t TIM5_GetCapture3(void)
3288                     ; 1001 {
3289                     	switch	.text
3290  03a1               _TIM5_GetCapture3:
3292  03a1 89            	pushw	x
3293       00000002      OFST:	set	2
3296                     ; 1002   uint16_t temp = 0; 
3298                     ; 1004   temp = ((uint16_t)TIM5->CCR3H << 8);
3300  03a2 c65315        	ld	a,21269
3301  03a5 5f            	clrw	x
3302  03a6 97            	ld	xl,a
3303  03a7 4f            	clr	a
3304  03a8 02            	rlwa	x,a
3305  03a9 1f01          	ldw	(OFST-1,sp),x
3306                     ; 1006   return (uint16_t)(temp | (uint16_t)(TIM5->CCR3L));
3308  03ab c65316        	ld	a,21270
3309  03ae 5f            	clrw	x
3310  03af 97            	ld	xl,a
3311  03b0 01            	rrwa	x,a
3312  03b1 1a02          	or	a,(OFST+0,sp)
3313  03b3 01            	rrwa	x,a
3314  03b4 1a01          	or	a,(OFST-1,sp)
3315  03b6 01            	rrwa	x,a
3318  03b7 5b02          	addw	sp,#2
3319  03b9 81            	ret
3353                     ; 1014 uint16_t TIM5_GetCounter(void)
3353                     ; 1015 {
3354                     	switch	.text
3355  03ba               _TIM5_GetCounter:
3357  03ba 89            	pushw	x
3358       00000002      OFST:	set	2
3361                     ; 1016   uint16_t tmpcntr = 0;
3363                     ; 1018   tmpcntr = ((uint16_t)TIM5->CNTRH << 8); 
3365  03bb c6530c        	ld	a,21260
3366  03be 5f            	clrw	x
3367  03bf 97            	ld	xl,a
3368  03c0 4f            	clr	a
3369  03c1 02            	rlwa	x,a
3370  03c2 1f01          	ldw	(OFST-1,sp),x
3371                     ; 1020   return (uint16_t)(tmpcntr | (uint16_t)(TIM5->CNTRL));
3373  03c4 c6530d        	ld	a,21261
3374  03c7 5f            	clrw	x
3375  03c8 97            	ld	xl,a
3376  03c9 01            	rrwa	x,a
3377  03ca 1a02          	or	a,(OFST+0,sp)
3378  03cc 01            	rrwa	x,a
3379  03cd 1a01          	or	a,(OFST-1,sp)
3380  03cf 01            	rrwa	x,a
3383  03d0 5b02          	addw	sp,#2
3384  03d2 81            	ret
3408                     ; 1028 TIM5_Prescaler_TypeDef TIM5_GetPrescaler(void)
3408                     ; 1029 {
3409                     	switch	.text
3410  03d3               _TIM5_GetPrescaler:
3414                     ; 1031   return (TIM5_Prescaler_TypeDef)(TIM5->PSCR);
3416  03d3 c6530e        	ld	a,21262
3419  03d6 81            	ret
3565                     ; 1047 FlagStatus TIM5_GetFlagStatus(TIM5_FLAG_TypeDef TIM5_FLAG)
3565                     ; 1048 {
3566                     	switch	.text
3567  03d7               _TIM5_GetFlagStatus:
3569  03d7 89            	pushw	x
3570  03d8 89            	pushw	x
3571       00000002      OFST:	set	2
3574                     ; 1049   FlagStatus bitstatus = RESET;
3576                     ; 1053   assert_param(IS_TIM5_GET_FLAG_OK(TIM5_FLAG));
3578                     ; 1055   tim5_flag_l= (uint8_t)(TIM5->SR1 & (uint8_t)TIM5_FLAG);
3580  03d9 9f            	ld	a,xl
3581  03da c45304        	and	a,21252
3582  03dd 6b01          	ld	(OFST-1,sp),a
3583                     ; 1056   tim5_flag_h= (uint8_t)((uint16_t)TIM5_FLAG >> 8);
3585  03df 7b03          	ld	a,(OFST+1,sp)
3586  03e1 6b02          	ld	(OFST+0,sp),a
3587                     ; 1058   if (((tim5_flag_l)|(uint8_t)(TIM5->SR2 & tim5_flag_h)) != RESET )
3589  03e3 c65305        	ld	a,21253
3590  03e6 1402          	and	a,(OFST+0,sp)
3591  03e8 1a01          	or	a,(OFST-1,sp)
3592  03ea 2706          	jreq	L3171
3593                     ; 1060     bitstatus = SET;
3595  03ec a601          	ld	a,#1
3596  03ee 6b02          	ld	(OFST+0,sp),a
3598  03f0 2002          	jra	L5171
3599  03f2               L3171:
3600                     ; 1064     bitstatus = RESET;
3602  03f2 0f02          	clr	(OFST+0,sp)
3603  03f4               L5171:
3604                     ; 1066   return (FlagStatus)bitstatus;
3606  03f4 7b02          	ld	a,(OFST+0,sp)
3609  03f6 5b04          	addw	sp,#4
3610  03f8 81            	ret
3645                     ; 1082 void TIM5_ClearFlag(TIM5_FLAG_TypeDef TIM5_FLAG)
3645                     ; 1083 {
3646                     	switch	.text
3647  03f9               _TIM5_ClearFlag:
3649  03f9 89            	pushw	x
3650       00000000      OFST:	set	0
3653                     ; 1085   assert_param(IS_TIM5_CLEAR_FLAG_OK(TIM5_FLAG));
3655                     ; 1088   TIM5->SR1 = (uint8_t)(~((uint8_t)(TIM5_FLAG)));
3657  03fa 9f            	ld	a,xl
3658  03fb 43            	cpl	a
3659  03fc c75304        	ld	21252,a
3660                     ; 1089   TIM5->SR2 &= (uint8_t)(~((uint8_t)((uint16_t)TIM5_FLAG >> 8)));
3662  03ff 7b01          	ld	a,(OFST+1,sp)
3663  0401 43            	cpl	a
3664  0402 c45305        	and	a,21253
3665  0405 c75305        	ld	21253,a
3666                     ; 1090 }
3669  0408 85            	popw	x
3670  0409 81            	ret
3734                     ; 1103 ITStatus TIM5_GetITStatus(TIM5_IT_TypeDef TIM5_IT)
3734                     ; 1104 {
3735                     	switch	.text
3736  040a               _TIM5_GetITStatus:
3738  040a 88            	push	a
3739  040b 89            	pushw	x
3740       00000002      OFST:	set	2
3743                     ; 1105   ITStatus bitstatus = RESET;
3745                     ; 1106   uint8_t TIM5_itStatus = 0, TIM5_itEnable = 0;
3749                     ; 1109   assert_param(IS_TIM5_GET_IT_OK(TIM5_IT));
3751                     ; 1111   TIM5_itStatus = (uint8_t)(TIM5->SR1 & TIM5_IT);
3753  040c c45304        	and	a,21252
3754  040f 6b01          	ld	(OFST-1,sp),a
3755                     ; 1113   TIM5_itEnable = (uint8_t)(TIM5->IER & TIM5_IT);
3757  0411 c65303        	ld	a,21251
3758  0414 1403          	and	a,(OFST+1,sp)
3759  0416 6b02          	ld	(OFST+0,sp),a
3760                     ; 1115   if ((TIM5_itStatus != (uint8_t)RESET ) && (TIM5_itEnable != (uint8_t)RESET ))
3762  0418 0d01          	tnz	(OFST-1,sp)
3763  041a 270a          	jreq	L7671
3765  041c 0d02          	tnz	(OFST+0,sp)
3766  041e 2706          	jreq	L7671
3767                     ; 1117     bitstatus = SET;
3769  0420 a601          	ld	a,#1
3770  0422 6b02          	ld	(OFST+0,sp),a
3772  0424 2002          	jra	L1771
3773  0426               L7671:
3774                     ; 1121     bitstatus = RESET;
3776  0426 0f02          	clr	(OFST+0,sp)
3777  0428               L1771:
3778                     ; 1123   return (ITStatus)(bitstatus);
3780  0428 7b02          	ld	a,(OFST+0,sp)
3783  042a 5b03          	addw	sp,#3
3784  042c 81            	ret
3820                     ; 1136 void TIM5_ClearITPendingBit(TIM5_IT_TypeDef TIM5_IT)
3820                     ; 1137 {
3821                     	switch	.text
3822  042d               _TIM5_ClearITPendingBit:
3826                     ; 1139   assert_param(IS_TIM5_IT_OK(TIM5_IT));
3828                     ; 1142   TIM5->SR1 = (uint8_t)(~TIM5_IT);
3830  042d 43            	cpl	a
3831  042e c75304        	ld	21252,a
3832                     ; 1143 }
3835  0431 81            	ret
3887                     ; 1161 static void TI1_Config(uint8_t TIM5_ICPolarity,
3887                     ; 1162                        uint8_t TIM5_ICSelection,
3887                     ; 1163                        uint8_t TIM5_ICFilter)
3887                     ; 1164 {
3888                     	switch	.text
3889  0432               L3_TI1_Config:
3891  0432 89            	pushw	x
3892  0433 88            	push	a
3893       00000001      OFST:	set	1
3896                     ; 1166   TIM5->CCER1 &= (uint8_t)(~TIM5_CCER1_CC1E);
3898  0434 7211530a      	bres	21258,#0
3899                     ; 1169   TIM5->CCMR1  = (uint8_t)((uint8_t)(TIM5->CCMR1 & (uint8_t)(~( TIM5_CCMR_CCxS | TIM5_CCMR_ICxF )))
3899                     ; 1170                            | (uint8_t)(( (TIM5_ICSelection)) | ((uint8_t)( TIM5_ICFilter << 4))));
3901  0438 7b06          	ld	a,(OFST+5,sp)
3902  043a 97            	ld	xl,a
3903  043b a610          	ld	a,#16
3904  043d 42            	mul	x,a
3905  043e 9f            	ld	a,xl
3906  043f 1a03          	or	a,(OFST+2,sp)
3907  0441 6b01          	ld	(OFST+0,sp),a
3908  0443 c65307        	ld	a,21255
3909  0446 a40c          	and	a,#12
3910  0448 1a01          	or	a,(OFST+0,sp)
3911  044a c75307        	ld	21255,a
3912                     ; 1173   if (TIM5_ICPolarity != TIM5_ICPOLARITY_RISING)
3914  044d 0d02          	tnz	(OFST+1,sp)
3915  044f 2706          	jreq	L7302
3916                     ; 1175     TIM5->CCER1 |= TIM5_CCER1_CC1P ;
3918  0451 7212530a      	bset	21258,#1
3920  0455 2004          	jra	L1402
3921  0457               L7302:
3922                     ; 1179     TIM5->CCER1 &= (uint8_t)(~TIM5_CCER1_CC1P) ;
3924  0457 7213530a      	bres	21258,#1
3925  045b               L1402:
3926                     ; 1182   TIM5->CCER1 |=  TIM5_CCER1_CC1E;
3928  045b 7210530a      	bset	21258,#0
3929                     ; 1183 }
3932  045f 5b03          	addw	sp,#3
3933  0461 81            	ret
3985                     ; 1201 static void TI2_Config(uint8_t TIM5_ICPolarity,
3985                     ; 1202                        uint8_t TIM5_ICSelection,
3985                     ; 1203                        uint8_t TIM5_ICFilter)
3985                     ; 1204 {
3986                     	switch	.text
3987  0462               L5_TI2_Config:
3989  0462 89            	pushw	x
3990  0463 88            	push	a
3991       00000001      OFST:	set	1
3994                     ; 1206   TIM5->CCER1 &=  (uint8_t)(~TIM5_CCER1_CC2E);
3996  0464 7219530a      	bres	21258,#4
3997                     ; 1209   TIM5->CCMR2  = (uint8_t)((uint8_t)(TIM5->CCMR2 & (uint8_t)(~( TIM5_CCMR_CCxS | TIM5_CCMR_ICxF)))
3997                     ; 1210                            | (uint8_t)(( (TIM5_ICSelection)) | ((uint8_t)( TIM5_ICFilter << 4))));
3999  0468 7b06          	ld	a,(OFST+5,sp)
4000  046a 97            	ld	xl,a
4001  046b a610          	ld	a,#16
4002  046d 42            	mul	x,a
4003  046e 9f            	ld	a,xl
4004  046f 1a03          	or	a,(OFST+2,sp)
4005  0471 6b01          	ld	(OFST+0,sp),a
4006  0473 c65308        	ld	a,21256
4007  0476 a40c          	and	a,#12
4008  0478 1a01          	or	a,(OFST+0,sp)
4009  047a c75308        	ld	21256,a
4010                     ; 1214   if (TIM5_ICPolarity != TIM5_ICPOLARITY_RISING)
4012  047d 0d02          	tnz	(OFST+1,sp)
4013  047f 2706          	jreq	L1702
4014                     ; 1216     TIM5->CCER1 |= TIM5_CCER1_CC2P ;
4016  0481 721a530a      	bset	21258,#5
4018  0485 2004          	jra	L3702
4019  0487               L1702:
4020                     ; 1220     TIM5->CCER1 &= (uint8_t)(~TIM5_CCER1_CC2P) ;
4022  0487 721b530a      	bres	21258,#5
4023  048b               L3702:
4024                     ; 1224   TIM5->CCER1 |=  TIM5_CCER1_CC2E;
4026  048b 7218530a      	bset	21258,#4
4027                     ; 1225 }
4030  048f 5b03          	addw	sp,#3
4031  0491 81            	ret
4083                     ; 1241 static void TI3_Config(uint8_t TIM5_ICPolarity, uint8_t TIM5_ICSelection,
4083                     ; 1242                        uint8_t TIM5_ICFilter)
4083                     ; 1243 {
4084                     	switch	.text
4085  0492               L7_TI3_Config:
4087  0492 89            	pushw	x
4088  0493 88            	push	a
4089       00000001      OFST:	set	1
4092                     ; 1245   TIM5->CCER2 &=  (uint8_t)(~TIM5_CCER2_CC3E);
4094  0494 7211530b      	bres	21259,#0
4095                     ; 1248   TIM5->CCMR3 = (uint8_t)((uint8_t)(TIM5->CCMR3 & (uint8_t)(~( TIM5_CCMR_CCxS | TIM5_CCMR_ICxF))) 
4095                     ; 1249                           | (uint8_t)(( (TIM5_ICSelection)) | ((uint8_t)( TIM5_ICFilter << 4))));
4097  0498 7b06          	ld	a,(OFST+5,sp)
4098  049a 97            	ld	xl,a
4099  049b a610          	ld	a,#16
4100  049d 42            	mul	x,a
4101  049e 9f            	ld	a,xl
4102  049f 1a03          	or	a,(OFST+2,sp)
4103  04a1 6b01          	ld	(OFST+0,sp),a
4104  04a3 c65309        	ld	a,21257
4105  04a6 a40c          	and	a,#12
4106  04a8 1a01          	or	a,(OFST+0,sp)
4107  04aa c75309        	ld	21257,a
4108                     ; 1253   if (TIM5_ICPolarity != TIM5_ICPOLARITY_RISING)
4110  04ad 0d02          	tnz	(OFST+1,sp)
4111  04af 2706          	jreq	L3212
4112                     ; 1255     TIM5->CCER2 |= TIM5_CCER2_CC3P ;
4114  04b1 7212530b      	bset	21259,#1
4116  04b5 2004          	jra	L5212
4117  04b7               L3212:
4118                     ; 1259     TIM5->CCER2 &= (uint8_t)(~TIM5_CCER2_CC3P) ;
4120  04b7 7213530b      	bres	21259,#1
4121  04bb               L5212:
4122                     ; 1262   TIM5->CCER2 |=  TIM5_CCER2_CC3E;
4124  04bb 7210530b      	bset	21259,#0
4125                     ; 1263 }
4128  04bf 5b03          	addw	sp,#3
4129  04c1 81            	ret
4153                     ; 1271 void TIM5_InternalClockConfig(void)
4153                     ; 1272 {
4154                     	switch	.text
4155  04c2               _TIM5_InternalClockConfig:
4159                     ; 1274   TIM5->SMCR &=  (uint8_t)(~TIM5_SMCR_SMS);
4161  04c2 c65302        	ld	a,21250
4162  04c5 a4f8          	and	a,#248
4163  04c7 c75302        	ld	21250,a
4164                     ; 1275 }
4167  04ca 81            	ret
4266                     ; 1283 void TIM5_SelectOutputTrigger(TIM5_TRGOSource_TypeDef TIM5_TRGOSource)
4266                     ; 1284 {
4267                     	switch	.text
4268  04cb               _TIM5_SelectOutputTrigger:
4270  04cb 88            	push	a
4271  04cc 88            	push	a
4272       00000001      OFST:	set	1
4275                     ; 1285   uint8_t tmpcr2 = 0;
4277                     ; 1288   assert_param(IS_TIM5_TRGO_SOURCE_OK(TIM5_TRGOSource));
4279                     ; 1290   tmpcr2 = TIM5->CR2;
4281  04cd c65301        	ld	a,21249
4282  04d0 6b01          	ld	(OFST+0,sp),a
4283                     ; 1293   tmpcr2 &= (uint8_t)(~TIM5_CR2_MMS);
4285  04d2 7b01          	ld	a,(OFST+0,sp)
4286  04d4 a48f          	and	a,#143
4287  04d6 6b01          	ld	(OFST+0,sp),a
4288                     ; 1296   tmpcr2 |=  (uint8_t)TIM5_TRGOSource;
4290  04d8 7b01          	ld	a,(OFST+0,sp)
4291  04da 1a02          	or	a,(OFST+1,sp)
4292  04dc 6b01          	ld	(OFST+0,sp),a
4293                     ; 1298   TIM5->CR2 = tmpcr2;
4295  04de 7b01          	ld	a,(OFST+0,sp)
4296  04e0 c75301        	ld	21249,a
4297                     ; 1299 }
4300  04e3 85            	popw	x
4301  04e4 81            	ret
4384                     ; 1307 void TIM5_SelectSlaveMode(TIM5_SlaveMode_TypeDef TIM5_SlaveMode)
4384                     ; 1308 {
4385                     	switch	.text
4386  04e5               _TIM5_SelectSlaveMode:
4388  04e5 88            	push	a
4389  04e6 88            	push	a
4390       00000001      OFST:	set	1
4393                     ; 1309   uint8_t tmpsmcr = 0;
4395                     ; 1312   assert_param(IS_TIM5_SLAVE_MODE_OK(TIM5_SlaveMode));
4397                     ; 1314   tmpsmcr = TIM5->SMCR;
4399  04e7 c65302        	ld	a,21250
4400  04ea 6b01          	ld	(OFST+0,sp),a
4401                     ; 1317   tmpsmcr &= (uint8_t)(~TIM5_SMCR_SMS);
4403  04ec 7b01          	ld	a,(OFST+0,sp)
4404  04ee a4f8          	and	a,#248
4405  04f0 6b01          	ld	(OFST+0,sp),a
4406                     ; 1320   tmpsmcr |= (uint8_t)TIM5_SlaveMode;
4408  04f2 7b01          	ld	a,(OFST+0,sp)
4409  04f4 1a02          	or	a,(OFST+1,sp)
4410  04f6 6b01          	ld	(OFST+0,sp),a
4411                     ; 1322   TIM5->SMCR = tmpsmcr;
4413  04f8 7b01          	ld	a,(OFST+0,sp)
4414  04fa c75302        	ld	21250,a
4415                     ; 1323 }
4418  04fd 85            	popw	x
4419  04fe 81            	ret
4485                     ; 1331 void TIM5_SelectInputTrigger(TIM5_TS_TypeDef TIM5_InputTriggerSource)
4485                     ; 1332 {
4486                     	switch	.text
4487  04ff               _TIM5_SelectInputTrigger:
4489  04ff 88            	push	a
4490  0500 88            	push	a
4491       00000001      OFST:	set	1
4494                     ; 1333   uint8_t tmpsmcr = 0;
4496                     ; 1336   assert_param(IS_TIM5_TRIGGER_SELECTION_OK(TIM5_InputTriggerSource));
4498                     ; 1338   tmpsmcr = TIM5->SMCR;
4500  0501 c65302        	ld	a,21250
4501  0504 6b01          	ld	(OFST+0,sp),a
4502                     ; 1341   tmpsmcr &= (uint8_t)(~TIM5_SMCR_TS);
4504  0506 7b01          	ld	a,(OFST+0,sp)
4505  0508 a48f          	and	a,#143
4506  050a 6b01          	ld	(OFST+0,sp),a
4507                     ; 1342   tmpsmcr |= (uint8_t)TIM5_InputTriggerSource;
4509  050c 7b01          	ld	a,(OFST+0,sp)
4510  050e 1a02          	or	a,(OFST+1,sp)
4511  0510 6b01          	ld	(OFST+0,sp),a
4512                     ; 1344   TIM5->SMCR = (uint8_t)tmpsmcr;
4514  0512 7b01          	ld	a,(OFST+0,sp)
4515  0514 c75302        	ld	21250,a
4516                     ; 1345 }
4519  0517 85            	popw	x
4520  0518 81            	ret
4633                     ; 1357 void TIM5_EncoderInterfaceConfig(TIM5_EncoderMode_TypeDef TIM5_EncoderMode,
4633                     ; 1358                                  TIM5_ICPolarity_TypeDef TIM5_IC1Polarity,
4633                     ; 1359                                  TIM5_ICPolarity_TypeDef TIM5_IC2Polarity)
4633                     ; 1360 {
4634                     	switch	.text
4635  0519               _TIM5_EncoderInterfaceConfig:
4637  0519 89            	pushw	x
4638  051a 5203          	subw	sp,#3
4639       00000003      OFST:	set	3
4642                     ; 1361   uint8_t tmpsmcr = 0;
4644                     ; 1362   uint8_t tmpccmr1 = 0;
4646                     ; 1363   uint8_t tmpccmr2 = 0;
4648                     ; 1366   assert_param(IS_TIM5_ENCODER_MODE_OK(TIM5_EncoderMode));
4650                     ; 1367   assert_param(IS_TIM5_IC_POLARITY_OK(TIM5_IC1Polarity));
4652                     ; 1368   assert_param(IS_TIM5_IC_POLARITY_OK(TIM5_IC2Polarity));
4654                     ; 1370   tmpsmcr = TIM5->SMCR;
4656  051c c65302        	ld	a,21250
4657  051f 6b01          	ld	(OFST-2,sp),a
4658                     ; 1371   tmpccmr1 = TIM5->CCMR1;
4660  0521 c65307        	ld	a,21255
4661  0524 6b02          	ld	(OFST-1,sp),a
4662                     ; 1372   tmpccmr2 = TIM5->CCMR2;
4664  0526 c65308        	ld	a,21256
4665  0529 6b03          	ld	(OFST+0,sp),a
4666                     ; 1375   tmpsmcr &= (uint8_t)(TIM5_SMCR_MSM | TIM5_SMCR_TS)  ;
4668  052b 7b01          	ld	a,(OFST-2,sp)
4669  052d a4f0          	and	a,#240
4670  052f 6b01          	ld	(OFST-2,sp),a
4671                     ; 1376   tmpsmcr |= (uint8_t)TIM5_EncoderMode;
4673  0531 9e            	ld	a,xh
4674  0532 1a01          	or	a,(OFST-2,sp)
4675  0534 6b01          	ld	(OFST-2,sp),a
4676                     ; 1379   tmpccmr1 &= (uint8_t)(~TIM5_CCMR_CCxS);
4678  0536 7b02          	ld	a,(OFST-1,sp)
4679  0538 a4fc          	and	a,#252
4680  053a 6b02          	ld	(OFST-1,sp),a
4681                     ; 1380   tmpccmr2 &= (uint8_t)(~TIM5_CCMR_CCxS);
4683  053c 7b03          	ld	a,(OFST+0,sp)
4684  053e a4fc          	and	a,#252
4685  0540 6b03          	ld	(OFST+0,sp),a
4686                     ; 1381   tmpccmr1 |= TIM5_CCMR_TIxDirect_Set;
4688  0542 7b02          	ld	a,(OFST-1,sp)
4689  0544 aa01          	or	a,#1
4690  0546 6b02          	ld	(OFST-1,sp),a
4691                     ; 1382   tmpccmr2 |= TIM5_CCMR_TIxDirect_Set;
4693  0548 7b03          	ld	a,(OFST+0,sp)
4694  054a aa01          	or	a,#1
4695  054c 6b03          	ld	(OFST+0,sp),a
4696                     ; 1385   if (TIM5_IC1Polarity == TIM5_ICPOLARITY_FALLING)
4698  054e 9f            	ld	a,xl
4699  054f a144          	cp	a,#68
4700  0551 2606          	jrne	L5432
4701                     ; 1387     TIM5->CCER1 |= TIM5_CCER1_CC1P ;
4703  0553 7212530a      	bset	21258,#1
4705  0557 2004          	jra	L7432
4706  0559               L5432:
4707                     ; 1391     TIM5->CCER1 &= (uint8_t)(~TIM5_CCER1_CC1P) ;
4709  0559 7213530a      	bres	21258,#1
4710  055d               L7432:
4711                     ; 1394   if (TIM5_IC2Polarity == TIM5_ICPOLARITY_FALLING)
4713  055d 7b08          	ld	a,(OFST+5,sp)
4714  055f a144          	cp	a,#68
4715  0561 2606          	jrne	L1532
4716                     ; 1396     TIM5->CCER1 |= TIM5_CCER1_CC2P ;
4718  0563 721a530a      	bset	21258,#5
4720  0567 2004          	jra	L3532
4721  0569               L1532:
4722                     ; 1400     TIM5->CCER1 &= (uint8_t)(~TIM5_CCER1_CC2P) ;
4724  0569 721b530a      	bres	21258,#5
4725  056d               L3532:
4726                     ; 1403   TIM5->SMCR = tmpsmcr;
4728  056d 7b01          	ld	a,(OFST-2,sp)
4729  056f c75302        	ld	21250,a
4730                     ; 1404   TIM5->CCMR1 = tmpccmr1;
4732  0572 7b02          	ld	a,(OFST-1,sp)
4733  0574 c75307        	ld	21255,a
4734                     ; 1405   TIM5->CCMR2 = tmpccmr2;
4736  0577 7b03          	ld	a,(OFST+0,sp)
4737  0579 c75308        	ld	21256,a
4738                     ; 1406 }
4741  057c 5b05          	addw	sp,#5
4742  057e 81            	ret
4755                     	xdef	_TIM5_EncoderInterfaceConfig
4756                     	xdef	_TIM5_SelectSlaveMode
4757                     	xdef	_TIM5_SelectInputTrigger
4758                     	xdef	_TIM5_ClearITPendingBit
4759                     	xdef	_TIM5_GetITStatus
4760                     	xdef	_TIM5_ClearFlag
4761                     	xdef	_TIM5_GetFlagStatus
4762                     	xdef	_TIM5_GetPrescaler
4763                     	xdef	_TIM5_GetCounter
4764                     	xdef	_TIM5_GetCapture3
4765                     	xdef	_TIM5_GetCapture2
4766                     	xdef	_TIM5_GetCapture1
4767                     	xdef	_TIM5_SetIC3Prescaler
4768                     	xdef	_TIM5_SetIC2Prescaler
4769                     	xdef	_TIM5_SetIC1Prescaler
4770                     	xdef	_TIM5_SetCompare3
4771                     	xdef	_TIM5_SetCompare2
4772                     	xdef	_TIM5_SetCompare1
4773                     	xdef	_TIM5_SetAutoreload
4774                     	xdef	_TIM5_SetCounter
4775                     	xdef	_TIM5_SelectOCxM
4776                     	xdef	_TIM5_CCxCmd
4777                     	xdef	_TIM5_OC3PolarityConfig
4778                     	xdef	_TIM5_OC2PolarityConfig
4779                     	xdef	_TIM5_OC1PolarityConfig
4780                     	xdef	_TIM5_GenerateEvent
4781                     	xdef	_TIM5_OC3PreloadConfig
4782                     	xdef	_TIM5_OC2PreloadConfig
4783                     	xdef	_TIM5_OC1PreloadConfig
4784                     	xdef	_TIM5_ARRPreloadConfig
4785                     	xdef	_TIM5_ForcedOC3Config
4786                     	xdef	_TIM5_ForcedOC2Config
4787                     	xdef	_TIM5_ForcedOC1Config
4788                     	xdef	_TIM5_SelectOutputTrigger
4789                     	xdef	_TIM5_PrescalerConfig
4790                     	xdef	_TIM5_SelectOnePulseMode
4791                     	xdef	_TIM5_UpdateRequestConfig
4792                     	xdef	_TIM5_UpdateDisableConfig
4793                     	xdef	_TIM5_InternalClockConfig
4794                     	xdef	_TIM5_ITConfig
4795                     	xdef	_TIM5_Cmd
4796                     	xdef	_TIM5_PWMIConfig
4797                     	xdef	_TIM5_ICInit
4798                     	xdef	_TIM5_OC3Init
4799                     	xdef	_TIM5_OC2Init
4800                     	xdef	_TIM5_OC1Init
4801                     	xdef	_TIM5_TimeBaseInit
4802                     	xdef	_TIM5_DeInit
4821                     	end
