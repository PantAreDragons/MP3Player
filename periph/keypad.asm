;;
;; keypad.asm: Boilerplate code for keypad procedures
;;
;;  Created on:  July 21, 2019
;;      Author: khughes
;;
;;Kevin Acosta
;;Collaborations:
;;Cordero Forgach
;;Jason Andrew Loo
;;Rakan Saeed
;;using ports P and K

		.global	initKeypad, getKey ;; KLH

        .thumb                          ; generate Thumb-2 code

		.bss	rport,4,4		;; KLH
		.bss	cport,4,4		;; KLH
		.bss	lastrow,1,1		;; KLH
        .text                           ; switch to code (ROM) section


;; Place declarations and references for variables here.
;;PORTA, GPIO OFFSETS
;; GPIO_PORT_A:             .word	 0x40065000
GPIO_PORT_A:             .word	 0x40058000 ;; KLH
;;lastrow:					 .word   0
rportAddr:				.word	rport ;; KLH
cportAddr:				.word	cport ;; KLH
lastrowAddr:			.word	lastrow ;; KLH

;;following two lines might no longer be needed as it needs to be general

;;GPIO_PORT_P:             .word	 0x40065000  ;;declare port p
;;GPIO_PORT_K:             .word	 0x40061000 ;;port k rows
;;code declaring pins may not be needed if i do a static #number offset in asm
GPIO_PIN_0              .equ    (1 << 0)
GPIO_PIN_1              .equ    (1 << 1)
GPIO_PIN_2				.equ    (1 << 2)
GPIO_PIN_3              .equ    (1 << 3)
GPIO_PIN_4              .equ    (1 << 4)
GPIO_PIN_5              .equ    (1 << 5)
GPIO_PIN_6              .equ    (1 << 6)
GPIO_PIN_7              .equ    (1 << 7)
zeroxthree              .equ   0x03
;;these are to stay, declared offsets that are needed for later
GPIO_DIR				.equ	0x400
GPIO_PUR				.equ	0x510
GPIO_DEN				.equ	0x51c
GPIO_ODR				.equ	0x50c
GPIO_AFSEL				.equ    0x420
        .align  4                       ; force word alignment
;; Place definitions for peripheral registers here (base
;; addresses, register offsets, and field constants).  Follow the
;; example for the SYSCTL RCGCCPIO register from Lab 3.
SYSCTL:                 .word   0x400fe000
SYSCTL_RCGCGPIO         .equ    0x608
;;might not be needed due to the general solution

;;SYSCTL_RCGCGPIO_PORTP   .equ    (1<<14) ;;cols
;;SYSCTL_RCGCGPIO_PORTK   .equ    (1<<9) ;;rows

;; void initKeypad( const struct portinfo *col, const struct portinfo *row );

        .align  4                       ; force word alignment
initKeypad:   .asmfunc
	push  {LR, R4-R12}
	mov R6, R0
	mov R7, R1
	ldr	R4, cportAddr		;; KLH
	str	R0, [R4]			;; KLH
	ldr	R4, rportAddr		;; KLH
	str	R1, [R4]			;; KLH
	pop	{PC, R4-R12}

;;Declaring constsant portinfo vars cport pointer and rport..
;;i am making the assumption that port A's first 4 bits contain the 0-3 for rows
;;and that port A's last 4 bits contain the 0-3 of my columns
	;;Save base addresses for row and column ports for getKey()
	;;  rport = r; rows

	;;rport in assembly would just be the port A address with a certain offset(4)
	ldr R0, GPIO_PORT_A ;;gets value
	ldr	R1, [R6, #4]	;; KLH
	sub R1, R0          ;; KLH
	lsr R1, #12         ;; KLH
	mov R2, #1          ;; KLH
	lsl R2, R1          ;; KLH

	ldr	R1, [R7, #4]	;; KLH
	sub R1, R0          ;; KLH
	lsr R1, #12         ;; KLH
	mov R3, #1          ;; KLH
	lsl R3, R1          ;; KLH
	orr	R2, R3

	ldr	R0, SYSCTL      ;; KLH
	ldr	R1, [R0, #SYSCTL_RCGCGPIO]  ;; KLH
	orr R1, R2          ;; KLH
	str	R1, [R0, #SYSCTL_RCGCGPIO]  ;; KLH

	ldrb	R2, [R6]     ;; KLH  pins[0]
	ldrb	R3, [R6, #1] ;; KLH  pins[0]
    orr     R2, R3       ;; KLH
	ldrb	R3, [R6, #2] ;; KLH  pins[0]
    orr     R2, R3       ;; KLH
	ldrb	R3, [R6, #3] ;; KLH  pins[0]
    orr     R2, R3       ;; KLH

;;	ldr R0, [R0];; gets base address
	;; (cport->base) does this now have everything it needs??
	ldr	R0, [R6, #4 ] ;; KLH

	;;  cport->base[GPIO_DIR] &= ~pins;
	Ldr R1, [R0, #GPIO_DIR]
;;	bic R1, #(GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3) ;; And not
    bic R1, R2           ;; KLH
	str R1, [R0, #GPIO_DIR]

	;;  cport->base[GPIO_PUR] |= pins;
;;	ldr R1,  [R6]
	ldr R1,  [R0, #GPIO_PUR] ;;cport->base[GPIO_PUR]
;;	orr	R1,  #(GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3)
    orr R1, R2            ;; KLH
	str R1,  [R0, #GPIO_PUR]

	;;cport->base[GPIO_ODR] &= ~pins;
	ldr R1,  [R6]
	ldr R1,  [R0, #GPIO_ODR] ;;cport->base[GPIO_ODR]
	bic R1, #(GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3)
	str R1,  [R0, #GPIO_ODR]

	;;cport->base[GPIO_AFSEL] &= ~pins;
	ldr R1,  [R6]
	ldr R1,  [R0, #GPIO_AFSEL] ;;cport->base[GPIO_AFSEL]
	bic R1,  #(GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3)
	ldr R1,  [R0, #GPIO_AFSEL]

	;;cport->base[GPIO_DEN] |= pins;
	ldr R1,  [R6]
	ldr R1,  [R0, #GPIO_DEN] ;;cport->base[GPIO_DEN]
	orr R1, #(GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3)
	str R1,  [R0, #GPIO_DEN]

	;;COMPLETED CPORT, FOLLOWING CODE IS NOW BASED ON RPORT
	;;----------------------------------------------------------------------------

	ldr R0, GPIO_PORT_A ;;gets value
	ldr R0, [R0];; gets base address

	;;  rport->base[GPIO_DIR] |= pins;
	Ldr R1, [R0, #GPIO_DIR]
	orr R1, #(GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7)
	str R1, [R0, #GPIO_DIR]


	;;   rport->base[GPIO_PUR] &= ~pins;
	ldr R1,  [R7]
	ldr R1,  [R0, #GPIO_PUR] ;;cport->base[GPIO_PUR]
	bic	R1,  #(GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7)
	str R1,  [R0, #GPIO_PUR]

	;;rport->base[GPIO_ODR] |= pins;
	ldr R1,  [R7]
	ldr R1,  [R0, #GPIO_ODR] ;;cport->base[GPIO_ODR]
	orr R1,  #(GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7)
	str R1,  [R0, #GPIO_ODR]


	;;rport->base[GPIO_AFSEL] &= ~pins;
	ldr R1,  [R7]
	ldr R1,  [R0, #GPIO_AFSEL] ;;cport->base[GPIO_AFSEL]
	bic R1,  #(GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7)
	ldr R1,  [R0, #GPIO_AFSEL]

	;;cport->base[GPIO_DEN] |= pins;
	ldr R1,  [R7]
	ldr R1,  [R0, #GPIO_DEN] ;;cport->base[GPIO_DEN]
	orr R1,  #(GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7)
	str R1,  [R0, #GPIO_DEN]



	;;pins = rport->pins[0] | rport->pins[1] | rport->pins[2] | rport->pins[3];

	ldr R1,  [R7]
	ldr R1,  [R0, #GPIO_DIR] ;;cport->base[GPIO_DIR]
	bic R1,  #(GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7)
	ldr R1,  [R0, #GPIO_DIR]



  	;;set the initial row to scan

  	;;setrow( 0 );
	;;ldr R8, setrow ;;essentially setrow(0)

	;;uint8_t pins = rport->pins[0] | rport->pins[1] |  rport->pins[2] | rport->pins[3];
	;;this is our R2 = pins
	;; lastrow = row & 0x03;
	;;and R9, R8, zeroxthree ;;not sure if it has to be stored to a register first

	;; rport->base[pins] = pins ^ rport->pins[lastrow];
	ldr R0, [R0, #R2] ;;rport->base[pins]
	ldr R9, [R2, #R9] ;;rport->pins[lastrow];
	;;str r0, [GPIO_Port_A, #

	eor R0, R2, R9 ;;all stored into R0


	pop {PC, R4-R12}
        .endasmfunc

;; bool getKey( uint8_t *col, uint8_t *row );
        .align  4                       ; force word alignment
getKey: .asmfunc
	;;R2 = pins i really hope
	push  {LR,R4-R12}
	mov R6, R0
	mov R7, R1 ;;storing values in case of R0 and R1 rewrite

	;;pins = rport->pins[0] | rport->pins[1] | rport->pins[2] | rport->pins[3];
;;	ldrb R2, [R0, #GPIO_PIN_0] ;;cport->pins[0]
;;	ldrb R3, [R0, #GPIO_PIN_1] ;;cport->pins[1]
	ldr	 R0, cportAddr	;; KLH
	ldr	 R0, [R0]		;; KLH
	ldrb R2, [R0] ;;cport->pins[0] ;; KLH
	ldrb R3, [R0, #1] ;;cport->pins[1] ;; KLH
	orr R2, R3
;;	ldrb R3, [R0, #GPIO_PIN_2] ;;cport->pins[2]
	ldrb R3, [R0, #2] ;;cport->pins[1] ;; KLH
	orr R2, R3
;;	ldrb R3, [R0, #GPIO_PIN_3] ;;cport->pins[3]
	ldrb R3, [R0, #3] ;;cport->pins[1] ;; KLH
	orr R2, R3

	;;R2 = pins;
	;; if( cport->base[pins] == pins )
;;	ldrb R3, [R2, #GPIO_PIN_0] ;;cport->base[pins]
	ldr  R1, [R0, #4] 		;; KLH
	ldrb R3, [R1, R2, LSL #2] ;; KLH
	cmp R2, R3
	bne outif ;;if R3 != R2, jump past all this following code and go to outif

	;;setrow( lastrow + 1 );
;;	ldr R4, lastrow ;;loads last row in
	ldr	R4, lastrowAddr  ;; KLH
	ldrb R4, [R4]        ;; KLH
	add R4, #1 ;; lastrow + 1

	ldr R0, [R0, R2] ;;rport->base[pins]
	ldr R4, [R2, R4] ;;rport->pins[lastrow];

	eor R0, R2, R4 ;;all stored into R0

;;	ldr R0, #0 ;;return 0 (false)
    mov R0, #0 ;; KLH
	 b exit
outif: ;;outsife
	;;*row = lastrow;
	mov R1, R4


	;;for( uint8_t i = 0; i < 3; ++i ) {
;;	ldr R5, #0 ;; i= 0
    mov R5, #0 ;; KLH
forcheck: ;;checks to make sure I is not exceeding 3
		cmp R5, #3
		bge outfor ;;leaves for loop if i >= 3

		;;if( cport->base[cport->pins[i]] == 0 )
		ldrb R2, [R6, R5] ;;cport->pins[i]
		ldrb R3, [R2] ;;cport->base[cport->pins[i]]

		cmp R3, #0 ;;comapre
		;; if cport->base[cport->pins[i]] == 0
		bne outinif ;;leaves if, if != 0
		;;R6 = R0 = *col
		;;R5 = i
		;;mov R0, R6

		ldr R6, [R5, R6] ;;*col = i;

;;		ldr R0, #1 ;;return true
		mov  R0, #1 ;; KLH
		b exit

outinif:
		add R5, #1 ;;i++
		b forcheck ;;goes back in loop

outfor:
;;		ldr R6, [#3, R6] ;;*col = i;
;;		str R0, #1 ;;return true
		mov R3, #3 ;; KLH
		strb R6, [R3] ;; KLH
		mov	R0, #3 ;; KLH
		b exit
exit:
		pop {LR,R4-R12}
        .endasmfunc

        .data                           ; switch to data (RAM) section
        .align  4                       ; force word alignment

;; Place any variables stored in RAM here

        .end
