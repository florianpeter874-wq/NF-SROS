; Multiboot fejléc - Ebből tudja a virtuális gép, hogy OS-t tölt be
section .multiboot
align 4
    dd 0x1BADB002           ; Mágikus szám
    dd 0x00                 ; Zászlók (Flags)
    dd - (0x1BADB002 + 0x00) ; Ellenőrző összeg (Checksum)

section .text
global _start

_start:
    mov esp, stack_top      ; Beállítjuk a C nyelvnek szükséges memóriaterületet
    call _kernel_main       ; Átugrunk a C függvényedre!

.hang:                      ; Védelmi háló, ha a C kód valaha leállna, bár ez lehetetlen a while(1) ciklus miatt
    cli                     ; Megszakítások kikapcsolása
    hlt                     ; Processzor leállítása
    jmp .hang               ; Végtelen ciklus

section .bss                ; bss align section
align 16
stack_bottom:
    resb 16384              ; 16 KB memóriát foglalunk a C változóknak
stack_top: