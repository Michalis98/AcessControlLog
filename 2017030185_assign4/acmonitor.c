#define _GNU_SOURCE

#include <time.h>
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <openssl/md5.h>
struct entry {

	int uid; /* user id (positive integer) */
	int access_type; /* access type values [0-2] */
	int action_denied; /* is action denied values [0-1] */

	time_t date; /* file access date */
	time_t time; /* file access time */

	char *file; /* filename (string) */
	char *fingerprint; /* file fingerprint */

	

};


void
usage(void)
{
	printf(
	       "\n"
	       "usage:\n"
	       "\t./monitor \n"
		   "Options:\n"
		   "-m, Prints malicious users\n"
		   "-i <filename>, Prints table of users that modified "
		   "the file <filename> and the number of modifications\n"
		   "-h, Help message\n\n"
		   );

	exit(1);
}


void 
list_unauthorized_accesses(FILE *log)
{
	const char *line_array[7]; 
	char *line = NULL;
	size_t len=0;
	ssize_t read;
	int i = 0;
	int denied_counter = 0;
	FILE *fd = log;
	if (fd ==NULL)
		exit(EXIT_FAILURE);

	FILE *open_text = fopen("./access_control_logfile.txt","r");
	long lSize; 
	char *buffer;
	fseek(open_text,0L,SEEK_END);
	lSize = ftell(open_text);
	rewind(open_text);
	
	buffer = calloc(1,lSize+1);
	if (!buffer) fclose(open_text),fputs("memory allocation failed\n",stderr),exit(1);
	
	fread(buffer, lSize,1,open_text);
	

	while((read = getline(&line, &len,fd)) != -1){

			denied_counter=0;
			char *tb = "\t";
			char *token;
			token = strtok(line,tb);
			i =0;
			while(i<7){
				line_array[i] = token;
				token = strtok(NULL,tb);
				i++;
			}
			/*1st unsuccessfull access*/
			if((strcmp(line_array[5],"1")==0)){
				denied_counter++;
	
				char *line2 = NULL;
				size_t len2=0;
				ssize_t read2;
				const char *line_array2[7];
				/*if UID another 6 unsuccessfull accesses*/
				while((read2 = getline(&line2, &len2,fd)) != -1){
					char *tb2 = "\t";
					char *token2;
					token2 = strtok(line2,tb2);
					int k =0;
					while(k<7){
						line_array2[k] = token2;
						token2 = strtok(NULL,tb2);
						k++;
					}
					if((strcmp(line_array[0],line_array2[0])==0) && (strcmp(line_array2[5],"1")==0) && strcmp(line_array[1],line_array2[1])!=0){
						
						denied_counter++;
					}
					if(denied_counter == 7){
						printf("User %s tried unsuccesfully to open 7 different files\n",line_array[0]);
						
						break;
					}
				}
			}
		}
		



	return;

}


void
list_file_modifications(FILE *log, char *file_to_scan)
{

	const char *given_path = file_to_scan;	
	const char *line_array[7]; //pinakas me string ka8e mia apo tis 7 stiles tou log
	char *line = NULL;
	size_t len=0;
	char *line1 = NULL;
	size_t len1=0;
	ssize_t read;
	ssize_t read2;
	char **users = malloc (sizeof(char*)); 
	int changes[1000];
	int i = 0;
	int val;
	int flag=0;
	int denied_counter = 0;
	FILE *fd = log;
	FILE *fd2 = log;
	int j=0;
	char user[20];
	if (fd ==NULL)
		exit(EXIT_FAILURE);
	
	char line_hashes[MD5_DIGEST_LENGTH];
	const char *prev_hash =NULL;

	

		
		

	
	while((read = getline(&line, &len,fd)) != -1){
		char *tb = "\t";
		char *token;
		token = strtok(line,tb);
		i =0;
			while(i<7){
				line_array[i] = token;
				token = strtok(NULL,tb);
				i++;
			}
			if((strcmp(line_array[1], given_path)==0) && (strcmp(line_array[5],"0")==0)){
				if(prev_hash!=NULL){
					if((strcmp(prev_hash,line_array[6])!=0)){
				
						j++;
					}
				}
			prev_hash = line_array[6];
			char *line2 = NULL;
			size_t len2=0;
			ssize_t read2;
			const char *line_array2[7];
			

			while((read2 = getline(&line2, &len2,fd)) != -1 ) {
				int jj=1;
				
				char *tb2 = "\t";
				char *token2;

				token2 = strtok(line2,tb2);
				int k =0;
				while(k<7){
						line_array2[k] = token2;
						token2 = strtok(NULL,tb2);
						k++;
					}
				if((strcmp(line_array2[1], given_path)==0) && (strcmp(prev_hash,line_array2[6])!=0) && (strcmp(line_array2[5],"0")==0)){
				
					if(strcmp(line_array2[5],"0")==0){

										j++;

					}
					
					prev_hash =line_array2[6];
					break;
					}
					
				}
				strcpy(user,line_array2[0]);

			}
			
				

		}

	printf("The user: %s Modified %d times \n",user,j);
	return;

}


int 
main(int argc, char *argv[])
{

	int ch;
	FILE *log;

	if (argc < 2)
		usage();

	log = fopen("./access_control_logfile.txt", "r");
	if (log == NULL) {
		printf("Error opening log file \"%s\"\n", "./log");
		return 1;
	}

	while ((ch = getopt(argc, argv, "hi:m")) != -1) {
		switch (ch) {		
		case 'i':
			list_file_modifications(log, optarg);
			break;
		case 'm':
			list_unauthorized_accesses(log);
			break;
		default:
			usage();
		}

	}


	


	fclose(log);
	argc -= optind;
	argv += optind;	
	
	return 0;
}
