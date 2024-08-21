app:
	gcc -o ./target/polishNotation ./application/stack_application.c ./application/stackfunction.c -I./application

run:
	target\polishNotation.exe -f "1+2+3+4+5-5*5/36 ^7" -a
	target\polishNotation.exe -f "(1+2 - 3) -1" -a
	target\polishNotation.exe -f "((1+2))" -a
	target\polishNotation.exe -f "((1 + 2)- (3* 5 -1) + 3)" -a

test:
	target\polishNotation.exe -f "(1111  *2/3333 + 6 -2)" -a
	