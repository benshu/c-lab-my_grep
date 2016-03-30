#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

int main(int argc, char *argv[])
{
	FILE *input_file;
	char *str_to_find=NULL, *line=NULL;
	size_t line_cap =0;
	int line_length=0;
	str_to_find = argv[1];
	if (argc==3) {
		input_file = fopen(argv[2],"r");
	} else
		input_file =stdin;
	while((line_length = getline(&line, &line_cap, input_file)) > 0){
		if(strstr(line,str_to_find))
			printf("%s",line);
		free(line);
		line = NULL;
	}
	fclose(input_file);
	return 0;
}