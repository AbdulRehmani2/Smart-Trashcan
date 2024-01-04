.include "m328pdef.inc"
.include "delay.inc"
.include "servo.inc"
.include "UART.inc"
.cseg
.org 0x00

SBI DDRB, PB1
SBI DDRB, PB5
SBI DDRB, PB4
CBI DDRB, PB3
CBI PORTB, PB5
CBI PORTB, PB4
SBI PORTB, PB3

LDI r16, 0b10100010
STS TCCR1A, r16

LDI r16, 0b00011010
STS TCCR1B, r16

LDI r16, 0
STSw TCNT1H, r16, r16

LDI r16, LOW(40000)
LDI r17, HIGH(40000)
STSw ICR1H,r17,r16

Serial_begin

loop:
	call RemoteControl
	SBIS PINB, PB3
 	rjmp L1
 	CBI PORTB, 5

rjmp loop

L1:
		SBI PINB, 5
		CALL OpenLid
		delay 2000
		delay 2000
		CALL CloseLid
		rjmp loop


OpenLid:
	Serial_writeChar '1'
	Serial_writeNewLine
	LDI  r16, LOW(900)
	LDI r17, HIGH(900)
	STSw OCR1AH,r17,r16
	SBI PORTB, 4
	delay 500
	CBI PORTB, 4

	LDI r16, LOW(4900)
	LDI r17, HIGH(4900)
	STSw OCR1AH,r17,r16
	delay 500

ret

CloseLid:
	Serial_writeChar '0'
	Serial_writeNewLine

	LDI r16, LOW(900)
	LDI r17, HIGH(900)
	STSw OCR1AH,r17,r16
	SBI PORTB, 4
	delay 500
	CBI PORTB, 4

ret

RemoteControl:
	Serial_read
	cpi r20, '1'
	breq RemoteOpen
	cpi r20, '0'
	breq RemoteClose

	EndFunction:
		ret

RemoteOpen:
	call OpenLid
	rjmp EndFunction

RemoteClose:
	call CloseLid
	rjmp EndFunction