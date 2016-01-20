typedef struct
{
	int show_time;
	int show_second;
	int show_page_slide;
	int auto_save;
	int auto_save_time;
	int auto_save_exit;
	char Font_path[50];	
} CONFIG;
CONFIG config;

void load_config();

void setup_config();

extern int dirlist(const char* path,const char* pattern, char** result,const char * filter);

extern BOOL file_browse (const char * path,const char * filter,char * filename);


