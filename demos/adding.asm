;
; Adding registers:
;

 add ax, bx
 add cx, dx

;
; Adding value at displaced address to register:
;

add ax, [bx + 0x0ABC] ; Word offset on word instruction.
add al, [bx + 0x0ABC] ; Word offset on byte instruction.
add bl, [di + 0x0F] ; Byte offset on byte instruction.
add bx, [di + 0x0F] ; Byte offset on word instruction.

;
; Adding value at address (not displaced) to register:
;

add ax, [bp]
add ax, [bx]
add ax, [si]
add ax, [di]

;
; Adding register value to (not displacement) address:
;

add [bp], ax
add [bx], ax
add [si], ax
add [di], ax

;
; Add immediate values to registers:
;

add ax, 0xAB
add dx, 0xCD

;
;
;

add word [bx], 0xAA

hlt