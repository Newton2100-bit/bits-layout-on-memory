#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdbool.h>

#define INTSIZE (sizeof(int) * 8)
#define SHORTSIZE (sizeof(short) * 8)
#define VALUE(x) ((int)strtol((x), NULL, 10))

void logic(int, int);
void two_complement(bool *, int);
void convert(bool *, int );
void mark_as_negative(int *value, int size, int *mark);
void help_message(char *message);
void evaluate(int value, bool *array, int size);
void short_premessage(char *message);
void process_args(int argc, char  **argv);
void print(bool *, int *, int, int);
void short_func(char *message);
void int_func(char *);
void int_premessage();

char *blue = "\033[38;5;115m"; 
char *end = "\033[0m";
char *yellow = "\033[38;5;99m";
char *red = "\033[38;5;70m";



int main(int argc, char *argv[]){
	/* Converting decinals into bytes */
	if( argc == 1) 
		help_message(argv[0]);

	if(argc == 2)
		logic(strtol(argv[1], NULL, 10), sizeof(int) * 8);
	else
		process_args(argc, argv);

	return 0;
}

void process_args(int argc, char  **argv){
	int options;
	while((options = getopt(argc, argv, "s:i:")) != -1){
		switch(options){
			case 's':
				short_func(optarg);
				break;
			case 'i':
				int_func(optarg);
				break;
			default:
				printf("Nothing here\n");
				break;
		}
	}
}

void help_message(char *message){
	printf("Converting int and shorts to how thy look in memory\n");
	printf("%s -s <number> -i <number> ....\n", message);
	exit(1);
}

void short_func(char *message){
	short_premessage(message);
	logic(VALUE(optarg), SHORTSIZE);
}

void int_func(char *message){
	int_premessage(message);
	logic(VALUE(optarg), INTSIZE);
}

void int_premessage(){
	printf("%s[int] The arg is %s%s\n", blue, optarg, end);
	// int value = (int)(strtol(message, NULL, 10));
}



void short_premessage(char *message){
	int ivalue = strtol(message,NULL, 10);
	short svalue = (short)ivalue;
	
	if( ivalue != (int)svalue){
		printf("%s[short] The opt arg is %d => %hd%s\n",blue,ivalue, svalue, end);
		logic(ivalue,  INTSIZE);
	}else
		printf("%s[short] The opt arg is %hd%s\n",blue, svalue, end);
}



void logic(int value, int size){
	bool array[size] ;
	array[0] = 0;

	int mark = 0;
	mark_as_negative(&value, size, &mark);
	evaluate(value, array, size);

	if(mark){
		print(array, &value, size, mark? 0: 1);
		two_complement(array, size);
	}

	print(array, &value, size, mark? 1: 0);
}

void mark_as_negative(int *value, int size, int *mark){
	if((size / 8) == sizeof(short))
		*value = (int)((short)*value);
	if(*value < 0){
		*mark = 1;
		*value = -(*value);
	}
}



void evaluate(int value, bool *array, int size){
	for (int i = (size - 1); i > 0; i--)
	{
		array[i] = value % 2;
		if(i == 1) break;
		value = value / 2;
	}
}


void convert(bool *array, int size){
	for(int i = size; i > 0; i--){
		if(array[i] == 1)
			array[i] = 0;
		else
			array[i] = 1;
	}
}


void two_complement(bool *array, int size){
	if(array[size - 1] == 1)
		convert(array , (size - 2));
	else{
		for(int i = (size - 2); i > 0; i--){
			if( array[i] == 1){
				convert(array , (i - 1));
				break;
			}
		}
	}
}


void print(bool *array, int *value, int size, int is_negative){
	int sofar = 0;
	if(is_negative)
		array[0] = 1;

	printf("[%s %d %s]	\nbits [ ", red, (is_negative)? -(*value): *value, end);
	for (int i = 0; i < size; i++)
	{ 
		sofar++;
		printf("%s%i%s ",yellow, array[i], end);
		if(sofar % 8 == 0 && sofar != size)
			printf("%s# %s", red, end);
		if(sofar % 4 == 0 && sofar != size && sofar % 8 != 0) 
			printf(" ");
	}

	printf("]\n");
}
