	AREA espresso_program, CODE, READONLY
	ENTRY
	EXPORT main
; fetch the addresses of the controllers status registers 		
main LDR r1, =0x20000000	; addr of temp sensor reg
	LDR r2, =0x20000100	; addr of press sensor reg
	LDR r3, =0x20000200	; addr of user-controlled power-switch reg
	LDR r4, =0x20010000	; addr of boiler control reg
	LDR r5, =0x20010100	; addr of pump control reg
	LDR r6, =0x20010200	; addr of 3-way valve control reg
;
;keep checking the power switch state and take action if ON
;
	B power_loop	; infinite loop
	
power_loop BL power_switch ; check power switch state
	MOV r11, r0	; load the power state
	CMPS r11, #0x01 ; r11 - 0x01 
	BNE power_loop  ; check if power is ON
	MOV r9, #0x00	; close the valve
	BL three_way_valve	; change valve state
polling	BL adjust_boiler	; manage boiler state and monitor temp
	BL adjust_pump	; monitor pressure and pump state
	BL power_switch	; check if power is OFF
	MOV r11, r0	; load the power state
	CMP r11, #0x00 ; r11 - 0x00 
	BNE polling	; keep monitoring temp and pressure
	MOV r9, #0x011	; vent the valve
	BL three_way_valve	; change valve state
	MOV r7, #0x00	; turn off boiler
	BL boiler	
	MOV r8, #0x00	; turn off pump
	BL pump	
	B power_loop	; loop_back

;
; parent function changing the state of the boiler 
; this is based on if the temp_sensor reading is above of below 75 degrees
;
adjust_boiler BL temp_sensor	; read the temp value
	CMP r0, #75		; r0 - 75
	BGT boiler_off
	MOV r7, #0x01	; turn on the boiler
	BL boiler		; load new boiler state into reg
	BX LR
boiler_off MOV r7, #0x00	; turn off the boiler
	MOV r9, #0x01	; open the valve
	BL three_way_valve	; change valve state
	BL boiler		; load new boiler state into reg
	BX LR

;
;parent function changing the state of the pump
;
adjust_pump BL press_sensor	; read the press value
	ASR r10, r0, #8		; Q format to int
	CMP r10, #9		; r10 - 9
	BGT pump_off
	MOV r8, #0x01	; turn on the pump
	BL pump
	BX LR
pump_off MOV r8, #0x00		; turn off the pump
	BL pump
	BX LR

;		
;	
; implement functions for each device 
;
; function for user-controlled power switch
power_switch LDRB r0, [r3]	; read the power state
	BX LR

; function for temp sensor value
temp_sensor LDR r0, [r1]	; read the current temp
	BX LR

; function for pressure sensor value
press_sensor LDR r0, [r2]	; read the current pressure
	BX LR

; function for boiler status
boiler LDRB r0, [r4]	; read the current boiler state
	CMP r7, r0	; r7 - r0 
	BNE new_boiler_state	; check if boiler state is thesame
	BX LR
new_boiler_state MOV r0, r7	
	STRB r0, [r4]	; change the boiler state
	BX LR
		
; function for pump state
pump LDRB r0, [r5]	; read the current pump state
	CMP r8, r0	; r8 - r0 
	BNE new_pump_state	; check if pump state is thesame
	BX LR
new_pump_state MOV r0, r8
	STRB r0, [r5]	; change the pump state
	BX LR

; function for 3-way valve
three_way_valve LDRB r0, [r6]	; read the valve position
	CMP r9, r0	; r9 - r0 
	BNE new_valve_state ; check if valve position is same
	BX LR
new_valve_state MOV r0, r9
	STRB r0, [r6]	; chane the valve position
	BX LR
;
	END