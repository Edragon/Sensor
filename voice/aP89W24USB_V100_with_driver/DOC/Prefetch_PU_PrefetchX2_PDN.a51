;==========================================================================
; This program is developed for AP89341
; To demostrate the CPU serial command mode
; Command sequency: Prefetch > PUP2 > Prefetch x 2 > PDN2
;==========================================================================

;==========================================================================
; User defined Constant
;==========================================================================

;==========================================================================
; definition of Macro
;==========================================================================

;==========================================================================
; definition of AP89341  constant
;==========================================================================
  Snd_Cmd_PUP1     EQU   0C5h
  Snd_Cmd_PUP2     EQU   08Dh
  Snd_Cmd_PDN1     EQU   0E1h
  Snd_Cmd_PDN2     EQU   0A9h
  Snd_Cmd_PLAY     EQU   055h
  Snd_Cmd_STATUS   EQU   0E3h
  Snd_Cmd_PAUSE    EQU   039h
  Snd_Cmd_RESUME   EQU   01Dh
  Snd_Cmd_PREFETCH EQU   071h
  
  Snd_Status_BUSYB EQU   010h
  Snd_Status_8KHz  EQU   011h
  Snd_Status_4KHz  EQU   012h
  Snd_Status_2KHz  EQU   013h
  Snd_Status_1KHz  EQU   014h
  Snd_Status_16Hz  EQU   015h
  Snd_Status_1MHz  EQU   016h
  Snd_Status_EMPTY EQU   017h

  
;==========================================================================
; definition of I/O Ports
;==========================================================================
  Sound            EQU   P2

  _Snd_DAT         EQU   P2.0
  _Snd_CLK         EQU   P2.1
  _Snd_CS          EQU   P2.2
  _Snd_M1          EQU   P2.3
  _Snd_Busy        EQU   P2.5
  _Snd_Out2        EQU   P2.6
  _Snd_Full        EQU   P2.7

  
  Snd_DAT          EQU   001h
  Snd_CLK          EQU   002h
  Snd_CS           EQU   004h
  Snd_M1           EQU   008h
  Snd_Busy         EQU   020h 
  Snd_Out2         EQU   040h 
  Snd_Full         EQU   080h 
  
  Snd_Initial      EQU   (Snd_M1 + Snd_Full + Snd_Out2 + Snd_Busy)
  
;==========================================================================
; definition of Variable
;==========================================================================
; Byte Operation Area [30:7f]
  Dly0             EQU   030h
  Dly1             EQU   031h
  Dly2             EQU   032h
  
  BitCnt           EQU   038h
  Cmd              EQU   039h

  SCONF            EQU   0bfh
  WDTC             EQU   09fh

;==========================================================================
; Vector Table
;==========================================================================

                   ORG   000h           ; Reset
                   ljmp  Initial

                   ORG   003h           ; INT0

                   ORG   00bh           ; Timer0

                   ORG   013h           ; INT1 (may replace with other ISR)
               ;   jmp   0F000h         ; ISP_Entry (Live update only)

                   ORG   01bh           ; Timer1

                   ORG   023h           ; Serial Port

                   ORG   02bh           ; Timer2

                   ORG   030h

;==========================================================================
Initial:           nop
                   orl   SCONF,#01h             ; Inhibit ALE Pin
                   nop		  

                   mov   Sound,#Snd_Initial     ; Initial Sound Port
                   clr   _Snd_CS
                   clr   _Snd_CLK
                   clr   _Snd_DAT
                   setb  _Snd_M1
                   setb  _Snd_Full
                   setb  _Snd_Out2
                   setb  _Snd_Busy
                   call  LongDelay
;------------------                   
EnableSound:       nop
                   setb  _Snd_CS
                   mov   Cmd,#Snd_Cmd_STATUS
                   call  Sound_Cmd
                   mov   Cmd,#Snd_Status_8KHz
                   call  Sound_Cmd
                   clr   _Snd_CS

		   nop
                   nop                   
 
;------------------                   
           
Sec00:             nop
                   setb  _Snd_CS                ; PREFETCH Section 00
                   mov   Cmd,#Snd_Cmd_PREFETCH
                   call  Sound_Cmd
                   mov   Cmd,#00h
                   call  Sound_Cmd
                   clr   _Snd_CS
Wait00:   	   mov   C,_Snd_Full            ; can be ignore on stable system
                   jnc   Sec00                  ; can be ignore on stable system

;------------------
                   setb  _Snd_CS
                   mov   Cmd,#Snd_Cmd_PUP2
                   call  Sound_Cmd
                   clr   _Snd_CS   
;-=-=-=-=-=-=-=-=-                         
Wait01:            mov   C,_Snd_Full
                   jc    Wait01
                   mov   C,_Snd_Full
                   jc    Wait01
Sec01:             nop
                   setb  _Snd_CS                ; PREFETCH Section 01
                   mov   Cmd,#Snd_Cmd_PREFETCH
                   call  Sound_Cmd
                   mov   Cmd,#01h
                   call  Sound_Cmd
                   clr   _Snd_CS
Check01:  	   mov   C,_Snd_Full            ; can be ignore on stable system
                   jnc   Sec01                  ; can be ignore on stable system
;-=-=-=-=-=-=-=-=-                         
Wait02:            mov   C,_Snd_Full 
                   jc    Wait02
                   mov   C,_Snd_Full
                   jc    Wait02
Sec02:             nop
                   setb  _Snd_CS                ; PREFETCH Section 02
                   mov   Cmd,#Snd_Cmd_PREFETCH
                   call  Sound_Cmd
                   mov   Cmd,#02h
                   call  Sound_Cmd
                   clr   _Snd_CS
Check02:  	   mov   C,_Snd_Full            ; can be ignore on stable system
                   jnc   Sec02                  ; can be ignore on stable system
;-=-=-=-=-=-=-=-=-                         
WaitLastStart:     mov   C,_Snd_Full 
                   jc    WaitLastStart
                   mov   C,_Snd_Full
                   jc    WaitLastStart
WaitBusy:          mov   C,_Snd_Busy
                   jc    WaitBusy
                   mov   C,_Snd_Busy
                   jc    WaitBusy                        
;------------------                             ; un-richable code
DisableSound:      setb  _Snd_CS
                   mov   Cmd,#Snd_Cmd_PDN2
                   call  Sound_Cmd
                   clr   _Snd_CS

TheEnd:            jmp   TheEnd                                     
;--------------------------------------------------------------------------
Sound_Cmd:         mov   BitCnt,#08h
                   mov   A,Cmd
Sound_Cmd_bit:     RRC   A
                   jc    Sound_Cmd_bit1
Sound_Cmd_bit0:    clr   _Snd_DAT 
                   jmp   Sound_Cmd_ck
Sound_Cmd_bit1:    setb  _Snd_DAT
                   jmp   Sound_Cmd_ck
Sound_Cmd_ck:      setb  _Snd_CLK
                   nop
                   nop           
                   clr   _Snd_CLK                           
                   djnz  BitCnt,Sound_Cmd_bit
                   clr   _Snd_DAT
                   ret
;--------------------------------------------------------------------------
LargeDelay:        mov   Dly2,#04h
                   jmp   LongDelay2
;------------------                   
LongDelay:         mov   Dly2,#01h
                   jmp   LongDelay2
;------------------                   
LongDelay2:        mov   Dly1,#00h
LongDelay1:        mov   Dly0,#00h
LongDelay0:        nop
                   nop
                   nop
                   djnz  Dly0,LongDelay0
                   djnz  Dly1,LongDelay1
                   djnz  Dly2,LongDelay2
                   ret
;==========================================================================
EndingAddress:
END
