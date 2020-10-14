addi $a0, $zero, -3
bgtz $a0, jump
addi $a0, $a0, 10
jump:
addi $v0, $zero, 1
syscall
addi $v0, $zero, 10
syscall
