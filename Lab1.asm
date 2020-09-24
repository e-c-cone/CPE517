.data
string1: .asciiz "I love you" #first string to compare
string2: .asciiz "I love you chicken nugget" #second string to compare

.text
main:
la $t0, string1
la $t5, string2 
li $a0, 48 #set output to false by default -- will change if match is found

lb $t2, ($t0) #load byte pointed to by address of string1 into $t2
beq $t2, 0, end #edge case -- end program if string1 is empty


firstmatch:
j strchr #run strchr
increment: #goes back to increment string1 if no match is found
add $t0, $t0, 1 #move forward in char array
lb $t2, ($t0) #load byte pointed to by address of string1 into $t2
bne $t2, 0, firstmatch #end of loop
j end #jump to end if terminating character found

strchr:
la $t1, ($t5) #sets pointer at beginning of string2 each time strchr runs
lb $t3, ($t1) #load byte pointed to by address of string2 into $t2
beq $t3, 0, end #edge case -- end program if string2 is empty
loop:
beq $t2, $t3, match #if match is found, jump to match section
add $t1, $t1, 1 #move forward in char array
lb $t3, ($t1) #load byte pointed to by address of string2 into $t2
bne $t3, 0, loop #if string hasn't ended, continue loop
j increment #jump back to firstmatch if string2 has been fully iterated

match:
la $a0, ($t3) #set $a0 (which will be the output) to found character
end:
li $v0, 11 #set to print char in syscall
syscall #print character
li $v0, 10 #set to terminate on syscall
syscall #terminate
