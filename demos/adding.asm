;
; Adding registers:
;

 add ax, bx
 add bx, cx

;
; Adding value at displaced address to register:
;

add ax, [bx + 0x0F0F] ; Word offset.
add bl, [di + 0x0F] ; Byte offset.

;
; Adding value at address (not displaced) to register:
;

add ax, [bx]

 ;
 ; Add immediate values to registers:
 ;

 add ax, 0xAB
 add dx, 0xCD

hlt