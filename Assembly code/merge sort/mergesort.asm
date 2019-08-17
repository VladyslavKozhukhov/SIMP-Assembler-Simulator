init:

	limm $sp, $zero, $zero 2048
	
	limm $t0, $zero, $zero, 4	 #save ra to stack 
	sub $sp, $sp, $t0, 0
	sw $ra, $sp, $zero, 0
	sw $s0, $sp, $zero, 1 #save s0 and s1
	sw $s1, $sp, $zero, 2
	sw $gp, $sp, $zero, 3
	
	limm $s0, $zero, $zero, 1024		#s0 =start address of array a
	limm $s1, $zero, $zero 1044			#s1 =start address of array b
	add $a0, $s0, $zero, 0				#a0=1024
	jal	$zero, $zero, $zero, copyArray	#copy array a to new array b
	jal	$zero, $zero, $zero, mergesort		# Call the merge sort function
	
	jal $zero, $zero, $zero, arrangeArray #arange array from biggest to smallest

	lw $ra, $sp, $zero, 0 	#load back registers
	lw $s0, $sp, $zero, 1 
	lw $s1, $sp, $zero, 2 
	lw $gp, $sp, $zero, 3
	limm $t0, $zero, $zero 4 #return stack to original address
	add $sp, $sp, $t0, 0
	
	halt
	
arrangeArray:
		limm $t0, $zero, $zero, 1024
		lw $t1, $t0, $zero, 0
		limm $t3, $zero, $zero, 1039
		lw $t2, $t3, $zero, 0
		sw $t2, $t0, $zero, 0
		sw $t1, $t3, $zero, 0
		
		limm $t0, $zero, $zero, 1025
		lw $t1, $t0, $zero, 0
		limm $t3, $zero, $zero, 1038
		lw $t2, $t3, $zero, 0
		sw $t2, $t0, $zero, 0
		sw $t1, $t3, $zero, 0
		
		limm $t0, $zero, $zero, 1026
		lw $t1, $t0, $zero, 0
		limm $t3, $zero, $zero, 1037
		lw $t2, $t3, $zero, 0
		sw $t2, $t0, $zero, 0
		sw $t1, $t3, $zero, 0
		
		limm $t0, $zero, $zero, 1027
		lw $t1, $t0, $zero, 0
		limm $t3, $zero, $zero, 1036
		lw $t2, $t3, $zero, 0
		sw $t2, $t0, $zero, 0
		sw $t1, $t3, $zero, 0
		
		limm $t0, $zero, $zero, 1028
		lw $t1, $t0, $zero, 0
		limm $t3, $zero, $zero, 1035
		lw $t2, $t3, $zero, 0
		sw $t2, $t0, $zero, 0
		sw $t1, $t3, $zero, 0
		
		limm $t0, $zero, $zero, 1029
		lw $t1, $t0, $zero, 0
		limm $t3, $zero, $zero, 1034
		lw $t2, $t3, $zero, 0
		sw $t2, $t0, $zero, 0
		sw $t1, $t3, $zero, 0
		
		limm $t0, $zero, $zero, 1030
		lw $t1, $t0, $zero, 0
		limm $t3, $zero, $zero, 1033
		lw $t2, $t3, $zero, 0
		sw $t2, $t0, $zero, 0
		sw $t1, $t3, $zero, 0
		
		limm $t0, $zero, $zero, 1031
		lw $t1, $t0, $zero, 0
		limm $t3, $zero, $zero, 1032
		lw $t2, $t3, $zero, 0
		sw $t2, $t0, $zero, 0
		sw $t1, $t3, $zero, 0
		
		jr $ra
	
	
mergesort:
	
	#limm $t0, $zero, $zero 1 #save ra to stack 
	#sub $sp, $sp, $t0, 0
	#sw $ra, $zero, $zero, 0
	
	limm $a0, $zero, $zero 0	 #start index of array	
	limm $a1, $zero, $zero 16	#end index of array

recursiveMerge: 
	add $t0, $a0, $zero, 0 #save begin address
	add $t1, $a1, $zero, 0 #save end address
	
	sub $t2, $t1, $t0, 0 		#t2 = end index-start index 
	limm $t3, $zero, $zero 1	 #t3=1
	bgt $zero, $t2,  $t3, continueRecMerg	# if array len <= 1 stop recursion
	jr $ra
	
continueRecMerg:
	add $t2, $t0, $t1, 0		#t2 = imiddle
	sra $t2, $t2, $t3			#devide: t2 = (t0+t1)/2
	
	limm $t0, $zero, $zero, 4 #save ra,begin,middle,end to stack
	sub $sp, $sp, $t0, 0
	sw $ra, $sp, $zero, 0	#save to stack ra
	sw $a0, $sp, $zero, 1	#save to stack beginning
	sw $a1, $sp, $zero, 2	#save to stack end
	sw $t2, $sp, $zero, 3	#save to stack middle
	
	add $a1, $t2, $zero, 0	#end = middle (a0 - beginning remains the same)
	
	
	add $t3, $s0, $zero, 0	#switch arrays
	add $s0, $s1, $zero, 0
	add $s1, $t3, $zero, 0
	
	
	jal	$zero, $zero, $zero, recursiveMerge

	lw $a0, $sp, $zero, 3	#get from stack middle
	lw $a1, $sp, $zero, 2	#get from stack end
	jal	$zero, $zero, $zero, recursiveMerge
	
	add $t3, $s0, $zero, 0
	add $s0, $s1, $zero, 0
	add $s1, $t3, $zero, 0

	lw $a0 , $sp, $zero, 1	#get from stack start
	lw $a1, $sp, $zero, 2	#get from stack end
	lw $gp, $sp, $zero, 3	
	jal	$zero, $zero, $zero, merge
	

	lw $ra, $sp, $zero, 0	#load from stack ra
	lw $a0, $sp, $zero, 1	#load from stack beginning
	lw $a1, $sp, $zero, 2	#load from stack end
	limm $t0, $zero, $zero 4 #change sp pointer
	add $sp, $sp, $t0, 0
	jr $ra
	
#stopRec:
#	




#begin a0, end a1, middle gp, start pos of first array s0, start pos of second array s1
# start pos of first array s0, start pos of second array s1
# t2, t3 save content of arrays

merge:
	add $v0, $zero, $a0		#v0=begin = i
	add $t0, $zero, $gp		#t0=middle = j
	add $t1, $zero, $v0		#t1=v0=begin (index for loop) = k
mergeLoop:
	beq $zero, $t1, $a1, endForMerge 	#jump if index = end (if k=end)
	ble $zero, $gp, $v0, else			#check if middle<=i
	ble $zero, $a1, $t0, continueMerge  #check if j>=end if yes jumps to continue
	add $t2, $s1, $v0, 0	   	#t2 = &array2[i]
	add $t3, $s1, $t0, 0		#t3 = &array2[j]
	lw $t2, $t2, $zero, 0		#t2 = array2[i]
	lw $t3, $t3, $zero, 0		#t3 = array2[j]
	bgt $zero, $t2, $t3, else 	#checks if array2[i]>array2[j] jumps to else	
continueMerge:
	add $t3, $s0, $t1, 0	#t3 = &array1[k]
	add $t2, $s1, $v0, 0	   	#t2 = &array2[i]
	lw $t2, $t2, $zero, 0		#t2 = array2[i]
	sw $t2, $t3, $zero 0 	#array1[k] = array2[i]
	limm $t2, $zero, $zero, 1	#t2=1
	add $v0, $v0, $t2			#i++;
	beq $zero, $zero, $zero, skipElse 	#end of if
	#skip else
else:
	add $t2, $s0, $t1, 0	#t2 = &array1[k]
	add $t3, $s1, $t0, 0		#t3 = &array2[j]
	lw $t3, $t3, $zero, 0		#t3 = array2[j]
	sw $t3, $t2, $zero 0 	#array1[k] = array2[j]
	limm $t2, $zero, $zero, 1	#t2=1
	add $t0, $t0, $t2			#j++;
			
skipElse:
	
	add $t1, $t1, $t2	#k++
	beq $zero, $zero, $zero, mergeLoop
	
	
endForMerge:
	jr $ra
	
	

copyArray: #copy array in address 1024-1039 to address 1044-1059

	#v0 is pinter to first array
	#t0 is opinter to second array
	#t1 contains 1
	#t2 contains content to copy
	#t3 contains #iterations left
	
	add $v0, $zero, $a0, 0
	limm $t0, $zero, $zero, 1044
	limm $t1, $zero, $zero, 1 		#t1=1
	limm $t3, $zero, $zero, 16
	
copyLoop:
	beq $zero, $t3, $zero, exitCopy
	lw $t2, $v0, $zero, 0
	sw $t2, $t0, $zero, 0
	add $v0, $v0, $t1, 0
	add $t0, $t0 ,$t1, 0
	sub $t3, $t3, $t1, 0
	beq $zero, $zero, $zero, copyLoop
	
	
exitCopy: jr $ra


.word 1024 0x00000001
.word 1025 0x00000002
.word 1026 0x00000003
.word 1027 0x0000000F
.word 1028 0x00000001
.word 1029 0x00000002
.word 1030 0x00000003
.word 1031 0x0000000F
.word 1032 0x00000001
.word 1033 0x00000002
.word 1034 0x00000003
.word 1035 0x0000000F
.word 1036 0x00000001
.word 1037 0x00000002
.word 1038 0x00000003
.word 1039 0x0000000F

