#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(void) {
	double time_py;
	double time_c;
	double time_asm;

	clock_t start_py, start_c, start_s, start_cc, start_cs, start_ls, end_py, end_c, end_s, end_cc, end_cs, end_ls;

	FILE *py_file = fopen("test.py", "w");
	char *py = "print(\"Hello, world!\")\n";
	fprintf(py_file, py);
	fclose(py_file);

	FILE *c_file = fopen("test.c", "w");
	char *c = "#include <stdio.h>\n\nint main(void) {\n\tprintf(\"Hello, world!\\n\");\n}\n";
	fprintf(c_file, c);
	fclose(c_file);

	FILE *s_file = fopen("test.asm", "w");
	char *s = "section .data\n\tmsg    db 'Hello, world!',10\n\tmsglen equ $-msg\n\nsection .text\n\tglobal _start\n\n"
			  "_start:\n\tmov eax, 4\n\tmov ebx, 1\n\tmov ecx, msg\n\tmov edx, msglen\n\tint 0x80\n\nmov eax, 1\n\txor ebx, ebx\n\tint 0x80\n";
	fprintf(s_file, s);
	fclose(s_file);

	start_py = clock();
	system("python3 test.py");
	end_py = clock(); // test.py

	start_cc = clock();
	system("cc -o test_c test.c");
	end_cc = clock(); // test.c

	start_c = clock();
	system("./test_c");
	end_c = clock();

	start_cs = clock();
	system("nasm -f elf64 -o test.o test.asm");
	end_cs = clock(); // test.asm

	start_ls = clock();
	system("ld -s -o test_asm test.o");
	end_ls = clock();

	start_s = clock();
	system("./test_asm");
	end_s = clock();

	double dur_py = ((double) end_py - start_py) / CLOCKS_PER_SEC;
	double dur_cc  = ((double) end_cc - start_cc) / CLOCKS_PER_SEC;
	double dur_cs  = ((double) end_cs - start_cs) / CLOCKS_PER_SEC;
	double dur_c = ((double) end_c - start_c) / CLOCKS_PER_SEC;
	double dur_s = ((double) end_s - start_s) / CLOCKS_PER_SEC;
	double dur_ls = ((double) end_ls - start_ls) / CLOCKS_PER_SEC;

	printf("\n");
	printf("The python program needed %f seconds for execution (python3 test.py).\n", dur_py);
	printf("The C program needed %f seconds for compilation (cc -o test_c test.c) and %f seconds for execution (./test_c).\n", dur_cc, dur_c);
	printf("The NASM program needed %f seconds for compilation (nasm -f elf64 -o test.o test.asm), %f seconds for linking (ld -s -o test_asm test.o) and %f seconds for execution (./test_asm).\n", dur_cs, dur_ls, dur_s);

	system("rm -rf test.asm test.py test_c test.o test_asm test.c");

	printf("\n");
	printf("All files removed. Program exited successfully with code 0.\n");

	return 0;
}

