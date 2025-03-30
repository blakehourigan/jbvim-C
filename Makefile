default: 
	gcc -o 	gcc -fsanitize=address -g -o main gap_buffer/gap_buffer.c main.c cursor/cursor.c tui/tui.c -lm
