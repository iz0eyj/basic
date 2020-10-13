#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include <readline/readline.h>
//#include <readline/history.h>
#include <string.h>
#include <signal.h>

/* ----------- */

#include "esp_system.h"
#include "esp_log.h"
#include "esp_console.h"
#include "esp_vfs_dev.h"
#include "driver/uart.h"
#include "linenoise/linenoise.h"
#include "argtable3/argtable3.h"
#include "cmd_decl.h"
#include "esp_vfs_fat.h"
#include "nvs.h"
#include "nvs_flash.h"


#include <stdbool.h>

#include "parser.h"

extern bool RUNNING;
extern bool __STOPPED;

static void sigint_handler(int signum)
{
  signal(SIGINT, sigint_handler);
    if (!RUNNING)return;
    RUNNING = false;
    __STOPPED = true;
    printf("STOP\n");
    fflush(stdout);
}


static char *
readline_gets ()
{
char* line_read = NULL;
 size_t line_len;

  __getline(&line_read, &line_len, stdin);
  //char * line_read = readline ("");

 // if (line_read && *line_read) {
 //    add_history (line_read);
 // }

  return line_read;
}

int out(int ch)
{
  putchar(ch);
  return 1;
}

int in(void)
{
  return getchar();
}

void repl(void)
{
  puts(" _               _      ");
  puts("| |__   __ _ ___(_) ___ ");
  puts("| '_ \\ / _` / __| |/ __|");
  puts("| |_) | (_| \\__ \\ | (__ ");
  puts("|_.__/ \\__,_|___/_|\\___|");
  puts("(c) 2015-2016 Johan Van den Brande");

  // using_history();
 
  char *input;
  while ((input = readline_gets()) != NULL )
  {
    if (strcmp(input, "QUIT") == 0) {
      free(input);
      break;
    }
    
    basic_eval(input);
    
    if (evaluate_last_error()) {
      printf("ERROR: %s\n", evaluate_last_error());
      clear_last_error();
    }

    free(input);
  }

  //clear_history();
}

void run(char *file_name){
  FILE* file = fopen(file_name, "r");

  if (file == NULL) {
    fprintf(stderr, "Can't open %s\n", file_name);
    return;  
  }  

  char line[tokenizer_string_length];
  while (fgets(line, sizeof(line), file)) {
    if(line[strlen(line)-1]!='\n')
    {
      printf("ERROR: NO EOL\n");
      exit(1);      
    }
    basic_eval(line);
  }
  fclose(file);

  basic_run();
}

void app_main(void)
{
  signal(SIGINT, sigint_handler);
printf ("PIPPO!!!");
  basic_init(1024*8, 2048);
  basic_register_io(out, in);

  basic_destroy();

  return;
}
