;
; Pushing/poping of general-purpose registers, stack registers, index registers:
;

push ax ; 0x50
push bx ; 0x53
push cx ; 0x51
push dx ; 0x52
push sp ; 0x54
push bp ; 0x55
push si ; 0x56
push di ; 0x57

pop ax ; 0x58
pop bx ; 0x5B
pop cx ; 0x59
pop dx ; 0x5A
pop sp ; 0x5C
pop bp ; 0x5D
pop si ; 0x5E
pop di ; 0x5F

;
; Pushing/poping of segment registers:
;

push es ; 0x06
push ss ; 0x16
push cs ; 0x0E
push ds ; 0x1F

pop es ; 0x07
pop ss ; 0x17
pop ds ; 0x1E
pop ds ; 0x1F

hlt
