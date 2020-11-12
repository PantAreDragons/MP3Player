;; Boilerplate code for lookup procedure

        .global  lookup

        .thumb                          ; generate Thumb-2 code
        .text                           ; switch to code (ROM) section

;; Place declarations and references for variables here
        .align  4                       ; force word alignment
hundred: 	.word 		100
fail:       .word		0xff
;; Place code for lookup here
        .align  4                       ; force word alignment
lookup:  .asmfunc
        push    {LR}                    ; save return address
;;R0 = ADDER, R1 = SIZE, R2 = ROW, R3 = col
;;essentially our if statement
		ldr R4, hundred
		mov R5, R0 ;;save adder value mem location
;;		cmp R0, R4	KLH ???
;;		bls else
        mov R0, #0xff   ;; KLH assume one test below will fail
		cmp R1, R4
		bgt else
;;		cmp R2, R4
;;		bgt else
;;		cmp R3, R4
;;		bgt else
        cmp    R1, R2   ;; KLH size <= row #?
        ble    else     ;; outside of array
        cmp    R1, R3   ;; size <= column #?
        ble    else     ;; outside of array

;;if any of the above fail, go to else:
;;stuff inside if statement: ROW*size+col
		mul R2, R1  ;;ROW * SIZE
		add R2, R3	;;ANSWER ^ + COL


;;		ldrb    R0, [R5, #(R2)]   ;;Get base address of ADDR offset with R2, store in R0
        ldrb R0, [R5, R2]  ;; KLH

		b end ;;jump to end, goes past "else" function





else:
		ldr R0, fail ;;ff return value setup

end:
        pop     {PC}                    ; return to caller

        .endasmfunc
        .end

