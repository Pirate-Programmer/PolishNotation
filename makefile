app:
	gcc -o ./target/polishNotation ./application/stack_application.c ./application/stackfunction.c -I./application

test:
	.\target\polishNotation.exe -f "K + L - M * N + (O^P) * W/U/V * T + Q" -p


run:
	target\polishNotation.exe -f "1+2+3+4+5-5*5/36 ^7" -a
	target\polishNotation.exe -f "(1+2 - 3) -1" -a
	target\polishNotation.exe -f "((1+2))" -a
	target\polishNotation.exe -f "((1 + 2)- (3* 5 -1) + 3)" -a
	