section .note.GNU-stack noalloc noexec nowrite progbits

section .data
    three dq 3.0
    half dq 0.5
    thirty dq 30.0
    one dq 1.0

section .text
global f1, f2, f3, f11, f22, f33

; f1(x) = 3/((x-1)^2 + 1)
f1:

    push ebp 
    mov ebp, esp
    
    ; Loading x
    fld qword[ebp + 8]
    
    ; Calculate (x-1)^2
    fld1
    fsubp st1, st0
    fld st0
    fmulp st1, st0
    
    ; Add 1
    fld1
    faddp st1, st0
    
    ; Load 3 and divide
    fld qword[three]
    fdivrp st1, st0
    
    pop ebp
    ret

; f2(x) = sqrt(x + 0.5)
f2:

    push ebp
    mov ebp, esp

    ; Loading x
    fld qword[ebp + 8]

    ; Loading 0.5
    fld qword[half]

    ; We add x and 0.5 and take the root of their sum
    faddp st1, st0
    fsqrt 

    pop ebp
    ret

; f3(x) = e^(-x)
f3:
    push ebp
    mov ebp, esp

    ; Load x and change the sign
    fld qword[ebp + 8]
    fchs

    ; Load log2(e), multiply by -x and duplicate the value
    fldl2e
    fmulp st1, st0
    fld st0
    
    ; We round to get the whole part (int) and subtract to get the fractional part (frac)
    frndint
    fsub st1, st0
    
    ; We swap st0 and st1, calculate (2^(frac) - 1) and add 1
    fxch st1
    f2xm1
    fld1
    faddp st1, st0

    ; We scale for 2^(frac) * 2^(int) = 2^(frac + int) = 2^(-x*log2(e)) = e^(-x)
    ; and remove the integer part from the stack
    fscale
    fstp st1

    pop ebp
    ret

; f11(x) = (30 - x)/3
f11:
    push ebp
    mov ebp, esp
    
    ; Loading 30.0
    fld qword[thirty]
    
    ; Subtract x
    fsub qword[ebp + 8]
    
    ; Divide by 3
    fdiv qword[three]
    
    pop ebp
    ret


; f22(x) = 3x + 1
f22:
    push ebp
    mov ebp, esp
    
    ; Loading x
    fld qword[ebp + 8]
    
    ; Multiply by 3
    fmul qword[three]
    
    ; Add 1
    fadd qword[one]
    
    pop ebp
    ret

; f33(x) = x^3
f33:
    push ebp
    mov ebp, esp
    
    ; Loading x
    fld qword[ebp + 8]
    
    ; Duplicate x
    fld st0
    
    ; Multiply x * x
    fmulp st1, st0
    
    ; Multiply x^2 * x
    fmul qword[ebp + 8]
    
    pop ebp
    ret