app:
	gcc -o ./target/polishNotation ./application/stack_application.c ./application/stackfunction.c -I./application

test:
	.\target\polishNotation.exe -f "(1+7^8*(1+4-3)+1)" -p
	target\polishNotation.exe -f "1+2+3+4+5-5*5/36 ^7" -a
	target\polishNotation.exe -f "(1+2 - 3) -1" -a
	