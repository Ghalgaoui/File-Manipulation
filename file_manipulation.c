#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


typedef enum {
	false, 
	true
} bool;

void readValue(char *variable)
{
	FILE *original = fopen("Reverve.txt", "r");
	char *buffer = malloc(15);
	char *value = malloc(10);
	char *line = NULL;
    size_t len = 0;
    ssize_t read;
    
	while ((read = getline(&line, &len, original)) != -1) {
		sscanf(line, "%s = %s", buffer, value);
		if (! strncmp(buffer, variable, strlen(variable))) {
			fprintf(stdout, "The value of %s equal %s\n", variable, value);
		}
	}
	fclose(original);
}

void writeValue(const char *variable, const char *value)
{
	FILE *original = fopen("Reverve.txt", "a+");
	char *line = NULL;
    size_t len = 0;
    ssize_t read;
	char *buffer = malloc(strlen(variable) + strlen(value) + 3);
	char *temp = malloc(20);
	strncpy(buffer, variable, strlen(variable));
	strncat(buffer, " = ", strlen(" = "));
	strncat(buffer, value, strlen(value));
	strncat(buffer, "\n", strlen("\n"));
	bool exist = false;
	int ret;
	
	while ((read = getline(&line, &len, original)) != -1) {
		sscanf(line, "%s = ", temp);
		if (! strncmp(variable, temp, strlen(temp))) {
			int pid = fork();
			if (pid == 0) {
				line[strlen(line) - 1] = '\0';
				buffer[strlen(buffer) - 1] = '\0';
				execl("/usr/bin/change", "/usr/bin/change", 
					line, buffer, 
					"Reverve.txt",
					NULL);  
			}
			wait(&ret);
			if (ret == 0)
				exist = true;
		}
	}
	if (exist == false)
		fwrite(buffer, 1, strlen(buffer), original);
	free(temp);
	free(buffer);
	fclose(original);
}

int main(int argc, char **argv)
{
	if (argc != 3) {
		fprintf(stderr, "Please Use: <Program <variable> <Value>\n");
		fprintf(stderr, "OR\n");
		fprintf(stderr, "Please Use: <Program <variable> GET\n");
		return 1;
	}
	if (strncmp(argv[2], "GET", strlen("GET")) == 0) {
		readValue(argv[1]);
	} else {
		writeValue(argv[1], argv[2]);
	}
	return 0;  
}
