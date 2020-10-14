addiu $t0, $zero, 16
addiu $t1, $zero, 21
slt   $a0, $t0, $t1
addiu $v0 ,$zero ,1
syscall

addi $v0 ,$zero ,10  #program ends
syscall
