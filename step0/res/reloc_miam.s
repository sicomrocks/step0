# allons au ru
.set noreorder

.text
	ADDI $t1, $zero,8
	Lw $t0, lunchtime
boucle: 
	BEQ $t0, $t1, byebye
	NOP
	addi $t1 , $t1, 1
	J boucle
		NOP
byebye: 

.bss
tableau: .space 16

.data
debut_cours: .word8
lunchtime: .word 12
adresse_lunchtime: .word lunchtim
