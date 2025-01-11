add:
push a
push b
push a
push b
add
ret
add:
push a
push b
push a
push b
add
ret
main:
push 5
set x
push 10
set y
push x
push y
call add
set result
push result
push 10
gt
jz label0
push result
push 10
add
set result
jmp label1
label0:
label1:
label2:
push result
push 0
gt
jz label3
push result
push 1
sub
set result
jmp label2
label3:
