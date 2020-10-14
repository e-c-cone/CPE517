
addiu $t0, $zero, 16
slti  $a0, $t0, 17
addiu $v0 ,$zero ,1
syscall

addi $v0 ,$zero ,10  #program ends
syscall
