#$t0 saves memory address or stack address
#$t1 save multiplication resolt

Initialize:
limm $sp, $zero, $zero, 2048 	
limm $t0, $zero, $zero, 3	
sub $sp, $sp, $t0, 0		# adjust stack for 3 items
sw $s2, $sp, $zero, 2		# save $s2
sw $s1, $sp, $zero, 1		# save $s1
sw $s0, $sp, $zero, 0		# save $s0
# initialize variables
limm $s0, $zero, $zero, 1	 #i =1
limm $s2, $zero, $zero 10	 #$s2 = 10

fori:
limm $s1, $zero, $zero, 1	#j=1
ble $zero $s0, $s2, forj	#if i<=10 jump to forj
lw $s2, $sp, $zero, 2		# save $s2
lw $s1, $sp, $zero, 1		# save $s1
lw $s0, $sp, $zero, 0		# save $s0
limm $t0, $zero, $zero, 3	
add $sp, $sp, $t0, 0			
halt

forj:
limm $t0, $zero, $zero, -1	#$t0=-1
add $t1, $s1, $t0		#$t1 = j-1
add $t0, $s0, $t0		#$t0=i-1
add $t2, $t1, $zero		#$t2 = $t1 = j-1
limm $t3, $zero, $zero, 3	#$t3= 8 t3 is the number of shifts for multiplicate
sll $t1, $t1, $t3		#$t1 = (j-1)*10  mul by shift left 3 add add 2(j-1)  
add $t1, $t1, $t2		#$t1 = (j-1)*10
add $t1, $t1, $t2 		#$t1 = (j-1)*10
limm $t3, $zero, $zero, 1024	#$t3= 1024
add $t0, $t0, $t1
add $t0, $t0, $t3		#$t0 = address for memory = [1024+(j-1)*10+i-1]
add $t1, $s0, $zero		#$t1 = i
limm $t3, $zero, $zero, 1 	#$t3= 1

mul:
beq $zero, $t2, $zero, saveToMem	#check if we finished claculating i*j 
add $t1, $s0, $t1			#t1 += i
sub $t2, $t2, $t3			# j = j-1
beq $zero, $zero, $zero, mul 		#end of loop that clac i*j 

saveToMem:
sw $t1, $t0, $zero, 0
add $s1, $s1, $t3		#j++
ble $zero, $s1, $s2, forj  	#if j<=10 jump to forj
add $s0, $s0, $t3		#i++
beq $zero, $zero, $zero, fori