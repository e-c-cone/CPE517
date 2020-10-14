.data



.text
main:
addi $t1 , $zero ,10   # $t1 = 0 +10 = 10
#add $a0 ,$t1, $zero
#bgtz $a0, print

j add_2


print:
add $a0, $t1, $zero
addi $v0 ,$zero ,1
syscall
end:
addi $v0 ,$zero ,10  #program ends
syscall


add_2:
addi $t1, $t1, 2
j print

