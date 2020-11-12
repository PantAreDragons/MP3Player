;; Boilerplate code for switches procedures

        .thumb                          ; generate Thumb-2 code
        .text                           ; switch to code (ROM) section

;; Place declarations and references for variables here.
		.align 4 ;;added by user, not boilerplate


GPIO_PORTL:             .word	 0x40062000  ;;declare portL

GPIO_PIN_0              .equ    (1 << 0)
GPIO_PIN_1              .equ    (1 << 1)
GPIO_PIN_2				.equ    (1 << 2)
GPIO_PIN_3              .equ    (1 << 3)


GPIO_DIR				.equ	0x400
GPIO_PUR				.equ	0x510
GPIO_DEN				.equ	0x51c
GPIO_ODR				.equ	0x50c
;; Place definitions for peripheral registers here (base 
;; addresses, register offsets, and field constants).  Follow the 
;; example for the SYSCTL RCGCCPIO register.

        .align  4                       ; force word alignment
SYSCTL:                 .word   0x400fe000
SYSCTL_RCGCGPIO         .equ    0x608
SYSCTL_RCGCGPIO_PORTL   .equ    (1<<10)

        .align  4                       ; force word alignment
;; Place your code between these comments

		.global	switches ;;user created global declaration // KLH
switches: ; KLH
		.asmfunc
		push  {LR}             		;;save return address and other registers
	    ldr R0, SYSCTL					;;load sysctl base address
		ldr R1, [R0, #SYSCTL_RCGCGPIO]  ; read the RCGCCPIO register
		orr R1, #SYSCTL_RCGCGPIO_PORTL
		str R1, [R0, #SYSCTL_RCGCGPIO]

		ldr R0, GPIO_PORTL

;;GPIO_PORTL[GPIO_DIR] |= (GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 );
		ldr R1, [R0, #GPIO_DIR]
		orr R1, #(GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3)
		bic R1, #GPIO_PIN_0 ;; KLH
		str R1, [R0, #GPIO_DIR]
;;GPIO_PORTL[GPIO_ODR] &= ~GPIO_PIN_0;
		ldr R1, [R0, #GPIO_ODR]
		bic R1, #GPIO_PIN_0
		str R1, [R0, #GPIO_ODR]

;;GPIO_PORTL[GPIO_ODR] |= (GPIO_PIN_1 |  GPIO_PIN_2 | GPIO_PIN_3 );
		ldr R1, [R0, #GPIO_ODR]
		orr R1, #(GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3)
		str R1, [R0, #GPIO_ODR]
;;GPIO_PORTL[GPIO_DEN] |= (GPIO_PIN_1 | GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3 );
		ldr R1, [R0, #GPIO_DEN]
;;		orr R1, #(GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3)
		orr R1, #(GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3)  ;; KLH
		str R1, [R0, #GPIO_DEN]
;;GPIO_PORTL[GPIO_PUR] |= GPIO_PIN_0;
		ldr R1, [R0, #GPIO_PUR]
;;		orr R1, #(GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3)
		bic R1, #(GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3) ;; KLH
		orr R1, #(GPIO_PIN_0) ;; KLH
		str R1, [R0, #GPIO_PUR]
;; Place your code between these comments

		pop   {PC}					;;return
		.endasmfunc
        .data                           ; switch to data (RAM) section
        .align  4                       ; force word alignment

;; Place any variable store in RAM here
        .end
