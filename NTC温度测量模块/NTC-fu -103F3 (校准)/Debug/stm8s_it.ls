   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Parser V4.8.32 - 23 Mar 2010
   3                     ; Generator V4.3.4 - 23 Mar 2010
  43                     ; 66 INTERRUPT_HANDLER_TRAP(TRAP_IRQHandler)
  43                     ; 67 {
  44                     	switch	.text
  45  0000               f_TRAP_IRQHandler:
  49                     ; 71 }
  52  0000 80            	iret
  74                     ; 78 INTERRUPT_HANDLER(TLI_IRQHandler, 0)
  74                     ; 79 
  74                     ; 80 {
  75                     	switch	.text
  76  0001               f_TLI_IRQHandler:
  80                     ; 84 }
  83  0001 80            	iret
 105                     ; 91 INTERRUPT_HANDLER(AWU_IRQHandler, 1)
 105                     ; 92 {
 106                     	switch	.text
 107  0002               f_AWU_IRQHandler:
 111                     ; 96 }
 114  0002 80            	iret
 136                     ; 103 INTERRUPT_HANDLER(CLK_IRQHandler, 2)
 136                     ; 104 {
 137                     	switch	.text
 138  0003               f_CLK_IRQHandler:
 142                     ; 108 }
 145  0003 80            	iret
 168                     ; 115 INTERRUPT_HANDLER(EXTI_PORTA_IRQHandler, 3)
 168                     ; 116 {
 169                     	switch	.text
 170  0004               f_EXTI_PORTA_IRQHandler:
 174                     ; 120 }
 177  0004 80            	iret
 200                     ; 127 INTERRUPT_HANDLER(EXTI_PORTB_IRQHandler, 4)
 200                     ; 128 {
 201                     	switch	.text
 202  0005               f_EXTI_PORTB_IRQHandler:
 206                     ; 132 }
 209  0005 80            	iret
 232                     ; 139 INTERRUPT_HANDLER(EXTI_PORTC_IRQHandler, 5)
 232                     ; 140 {
 233                     	switch	.text
 234  0006               f_EXTI_PORTC_IRQHandler:
 238                     ; 144 }
 241  0006 80            	iret
 264                     ; 151 INTERRUPT_HANDLER(EXTI_PORTD_IRQHandler, 6)
 264                     ; 152 {
 265                     	switch	.text
 266  0007               f_EXTI_PORTD_IRQHandler:
 270                     ; 156 }
 273  0007 80            	iret
 296                     ; 163 INTERRUPT_HANDLER(EXTI_PORTE_IRQHandler, 7)
 296                     ; 164 {
 297                     	switch	.text
 298  0008               f_EXTI_PORTE_IRQHandler:
 302                     ; 168 }
 305  0008 80            	iret
 327                     ; 215 INTERRUPT_HANDLER(SPI_IRQHandler, 10)
 327                     ; 216 {
 328                     	switch	.text
 329  0009               f_SPI_IRQHandler:
 333                     ; 220 }
 336  0009 80            	iret
 359                     ; 227 INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_BRK_IRQHandler, 11)
 359                     ; 228 {
 360                     	switch	.text
 361  000a               f_TIM1_UPD_OVF_TRG_BRK_IRQHandler:
 365                     ; 232 }
 368  000a 80            	iret
 391                     ; 239 INTERRUPT_HANDLER(TIM1_CAP_COM_IRQHandler, 12)
 391                     ; 240 {
 392                     	switch	.text
 393  000b               f_TIM1_CAP_COM_IRQHandler:
 397                     ; 245 }
 400  000b 80            	iret
 423                     ; 278  INTERRUPT_HANDLER(TIM2_UPD_OVF_BRK_IRQHandler, 13)
 423                     ; 279  {
 424                     	switch	.text
 425  000c               f_TIM2_UPD_OVF_BRK_IRQHandler:
 429                     ; 283  }
 432  000c 80            	iret
 455                     ; 290  INTERRUPT_HANDLER(TIM2_CAP_COM_IRQHandler, 14)
 455                     ; 291  {
 456                     	switch	.text
 457  000d               f_TIM2_CAP_COM_IRQHandler:
 461                     ; 295  }
 464  000d 80            	iret
 487                     ; 332  INTERRUPT_HANDLER(UART1_TX_IRQHandler, 17)
 487                     ; 333  {
 488                     	switch	.text
 489  000e               f_UART1_TX_IRQHandler:
 493                     ; 337  }
 496  000e 80            	iret
 519                     ; 344  INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
 519                     ; 345  {
 520                     	switch	.text
 521  000f               f_UART1_RX_IRQHandler:
 525                     ; 349  }
 528  000f 80            	iret
 550                     ; 383 INTERRUPT_HANDLER(I2C_IRQHandler, 19)
 550                     ; 384 {
 551                     	switch	.text
 552  0010               f_I2C_IRQHandler:
 556                     ; 388 }
 559  0010 80            	iret
 581                     ; 474  INTERRUPT_HANDLER(ADC1_IRQHandler, 22)
 581                     ; 475  {
 582                     	switch	.text
 583  0011               f_ADC1_IRQHandler:
 587                     ; 479  }
 590  0011 80            	iret
 613                     ; 500  INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, 23)
 613                     ; 501  {
 614                     	switch	.text
 615  0012               f_TIM4_UPD_OVF_IRQHandler:
 619                     ; 505  }
 622  0012 80            	iret
 645                     ; 513 INTERRUPT_HANDLER(EEPROM_EEC_IRQHandler, 24)
 645                     ; 514 {
 646                     	switch	.text
 647  0013               f_EEPROM_EEC_IRQHandler:
 651                     ; 518 }
 654  0013 80            	iret
 666                     	xdef	f_EEPROM_EEC_IRQHandler
 667                     	xdef	f_TIM4_UPD_OVF_IRQHandler
 668                     	xdef	f_ADC1_IRQHandler
 669                     	xdef	f_I2C_IRQHandler
 670                     	xdef	f_UART1_RX_IRQHandler
 671                     	xdef	f_UART1_TX_IRQHandler
 672                     	xdef	f_TIM2_CAP_COM_IRQHandler
 673                     	xdef	f_TIM2_UPD_OVF_BRK_IRQHandler
 674                     	xdef	f_TIM1_UPD_OVF_TRG_BRK_IRQHandler
 675                     	xdef	f_TIM1_CAP_COM_IRQHandler
 676                     	xdef	f_SPI_IRQHandler
 677                     	xdef	f_EXTI_PORTE_IRQHandler
 678                     	xdef	f_EXTI_PORTD_IRQHandler
 679                     	xdef	f_EXTI_PORTC_IRQHandler
 680                     	xdef	f_EXTI_PORTB_IRQHandler
 681                     	xdef	f_EXTI_PORTA_IRQHandler
 682                     	xdef	f_CLK_IRQHandler
 683                     	xdef	f_AWU_IRQHandler
 684                     	xdef	f_TLI_IRQHandler
 685                     	xdef	f_TRAP_IRQHandler
 704                     	end
