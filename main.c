/* 
This code use created for  find and reanme ths file in my PC
feel free to use and modify.

Richard Chang
 richard4350@gmail.com

*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h> 
#include <unistd.h>
#include <string.h>
char *getcwd(char *buf, size_t size);

#define FIND_RESULT_TMP_FILE "/tmp/jf_result.txt"




char *file_type = NULL;
char *destination_folder = NULL;
int term_flag = 0;

int find_and_gen_list(char *key_word, char *path)
{
	char cmd[256] = {0};
	sprintf(cmd, "find . -name \"*.%s\" > %s",
		  key_word, FIND_RESULT_TMP_FILE);
	system(cmd);
//	printf(" find cmd --> %s", cmd);
}


char* gen_new_file_name(char *org_path)
{
	char *f_name = NULL;
	char *new_name = NULL;
	char *p_head;
	int count = 0;
	int len = (int)strlen(org_path);

	f_name = (char *)malloc(sizeof(char) * len);	
	
	if(f_name == NULL)
	{
		perror("Malloc:");
		return NULL;
	}
	new_name = f_name;
	
	memset(f_name, 0 , len);

	p_head = (char *)strstr(org_path, "./");

	
	count += strlen("./");
	p_head+=count;

	while((count < len) && (term_flag == 0))
	{

		if(*p_head == '/')
			*f_name = '-';
		else if(*p_head == '\n')
			break;
		else
			*f_name = *p_head;

		f_name++;
		p_head++;
		count++;
	}

	return new_name;
}


void signal_term(int sig)
{
	term_flag = 1;
}


int do_file_rename_func(char *dest_path, char *file_list)
{

	FILE * fp;
 	char * line = NULL;
	size_t len = 0;
	ssize_t read;

	char *new_file_name = NULL;
	char oeg_file_name[128] = {0};

	char cmd_buf[256] = {0};

	int ret = 0;


    	fp = fopen(file_list, "r");
    	if (fp == NULL)
	{
		perror("open");
		return -1;
	}

    	while ((read = getline(&line, &len, fp)) != -1) 
	{
//        	printf("Retrieved line of length %zu :\n", read);
//        	printf(" line  -> %s len[%d]", line, strlen(line));
	
		memset(oeg_file_name, 0, sizeof(oeg_file_name));
		strncpy(oeg_file_name, line, strlen(line) -1);
		
	
		new_file_name = gen_new_file_name(line);

		if(new_file_name == NULL)
			continue;	

		memset(cmd_buf, 0, sizeof(cmd_buf));
//		printf(" org[%s] -----> new file [%s] \n", line, new_file_name);
		sprintf(cmd_buf, "cp %s %s/%s", oeg_file_name, destination_folder, new_file_name);
		printf(" cmd_buf[%s] \n", cmd_buf);
		system(cmd_buf);	
		ret ++;

		if(new_file_name != NULL)
		{
			free(new_file_name);
			new_file_name = NULL;
		}
		if(term_flag)
		{
			printf(" Received signal, terminated !! \n");
			break;
		}
    	}

	return ret;
}

void dump_out_reslut(int ret, char *folder, char *type)
{
	printf("\n");
	printf("\n");
	printf("\n");

	printf(" ******************************************************* \n");
	printf(" *						       * \n");
	printf(" * File type : %s \n", type);
	printf(" * Total : %d files copy into %s                        \n", ret ,folder);
	printf(" *						       * \n");
	printf(" ******************************************************* \n");
}

int main(int argc, char *argv[])
{

	char user_file_folder[128] = {0};
	char cmd_buf[256] = {0};

	

	signal(SIGINT, signal_term);
	
    	if(argc > 1)
    	{
    		int tmp = 1;
    		while(tmp < argc)
    		{
    			if(strcmp(argv[tmp], "-t") == 0)
    			{
		    		file_type = argv[tmp+1];
    			}
    			else  if(strcmp(argv[tmp], "-d") == 0)
    			{
		    		destination_folder = argv[tmp+1];
    			}
    			else  if(strcmp(argv[tmp], "-h") == 0)
    			{
				printf(" -t : opration file type ex: jpg, txt \n");
				printf(" -d : destniation,  The completed file in path, ex: /home/ok \n");
				return 0;
    			}
    			tmp++;
    		}
    	}

	if(file_type == NULL)
	{
		printf(" No file type input \n");
		file_type = "jpg";
	}


	if(destination_folder == NULL)
	{
		char cwd[1024] = {0};
   		if (getcwd(cwd, sizeof(cwd)) != NULL)
       		{
			sprintf(user_file_folder, "%s/jf_re/", cwd);
			destination_folder = user_file_folder;
		}			
		else{
			destination_folder = "tmp/jf_re/";
		}

	}

	sprintf(cmd_buf, "rm -r %s", destination_folder);
	printf("%s", cmd_buf);
	system(cmd_buf);
		
	sprintf(cmd_buf, "mkdir %s", destination_folder);
	printf("%s", cmd_buf);
	system(cmd_buf);

	printf(" \n");
	printf(" \n");

	printf(" --------------------------------------- \n");
	printf(" File type : %s \n", file_type);
	printf(" Destination folder : %s \n", destination_folder);
	printf(" --------------------------------------- \n");


	
	find_and_gen_list(file_type, "./");

	int ret = do_file_rename_func(destination_folder, FIND_RESULT_TMP_FILE);

	dump_out_reslut(ret, destination_folder, file_type);

	return 0;
}
