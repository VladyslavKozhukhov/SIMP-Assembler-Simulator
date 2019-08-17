
limm $sp, $zero, $zero, 2048 	
limm $t0, $zero, $zero, 3	# $t0 = 3
sub $sp, $sp, $t0, 0		# 3 items
sw $s0,$sp,$zero, 2 #open stack and save s1 to stack
sw $s1,$sp,$zero, 1
sw $s2,$sp,$zero, 0


#start Bubble Sort

add $t1,$zero,$zero #c=0
add $t2,$zero,$zero #d=0
add $t3,$zero,$zero #t=0
limm $s1,$zero,$zero,15 #n=15
for1:
bgt $zero,$t1,$s1 L0 #if(c>15 == c<16)
sub $s2,$s1,$t1	#15-c
limm $a1,$zero,$zero, 1 #a1=1
sub $s2,$s2,$a1	#15-c-1
for2:
bgt $zero,$t2,$s2 L1 # if(d>14-c == d<15-c)
limm $t0,$zero,$zero 1024 #list[0]
add $a0,$t0,$t2	#1024+d
limm $a1,$zero,$zero, 1 #a1=1
add $a1,$a0,$a1	#1024+d+1
lw $v0,$a0,$zero #list(d)
lw $s0,$a1,$zero #list(d+1)
bgt $zero,$v0,$s0 L3 #if(list(d)<list(d+1)
add $t3,$zero,$v0 #t=list(d)
sw $s0,$a0,$zero 0 #list(d)=list(d+1)
sw $t3,$a1,$zero 0 #list(d+1)=t
L3:
limm $a1,$zero,$zero, 1 #a1=1
add $t2,$a1,$t2 #d++
jal $zero,$zero,$zero, for2 #back to for2
L1: #end for2
limm $a1,$zero,$zero, 1 #a1=1
add $t1,$a1,$t1 #c++
add $t2,$zero,$zero #d=0
jal $zero,$zero,$zero, for1 #back to for1
L0: #end for1
lw $s2,$sp,$zero 0
lw $s1,$sp,$zero 1
lw $s0,$sp,$zero 2
add $sp, $sp, $t0, 3
halt


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

