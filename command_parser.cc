#include "command_parser.h"

bool find_pipe(char *s, int start, int end)
{
	if (start == end)
		return false;
	else{
		char *p;
		for (p = s+start; p != s+end; p++){
			if (*p == '|'){
				return true;
			}
		}	
		return false;
	}
}

bool handle_bg_symbol(char *s)
{
	size_t len = strlen(s);
	for (int i = len-1; i >= 0; i--){
		if (s[i] != ' ' && s[i] != '&')
			return false;
		else if (s[i] == ' ')
			continue;
		else{
			s[i] = '\0'; //throw the & symbol since it has been detected.
			return true;
		}
	}
	return false;
}


void parse_command(Job *job)
{
	char line[80], line_copy[80];
	fgets(line, sizeof(line), stdin);
	size_t len = strlen(line);
	line[len-1] = '\0'; // throw the \n at the end which is captured by fgets
	job->command_line = line;
	job->is_bg = handle_bg_symbol(line);
	strncpy(line_copy, line, sizeof(line_copy));

	std::string sep_string = "\t |\n"; 	
	const char *sep = sep_string.c_str(); //separators: space, \t, or pipe
	char *token, *brk_t, *last_token;
	last_token = line_copy;
	
	bool command_flag = true; // the first token should be the command name
	
	Command new_command;
	for (token = strtok_r(line_copy, sep, &brk_t);
		 token;
		 token = strtok_r(NULL, sep, &brk_t))
	{
		bool is_pipe = find_pipe(line, last_token - line_copy, 
									token - line_copy);
		if (is_pipe){
			job->commands.push_back(new_command); // add the last command into list
			new_command = Command(token); // create a new command
		}
		else{
			if (command_flag){
				command_flag = false;
				new_command = Command(token); // create a new command
			}
			else{
				std::string new_parameter = token;
				new_command.add_parameter(new_parameter); // add new para into current command
			}
		}
		last_token = token;
	}
	job->commands.push_back(new_command);
}



