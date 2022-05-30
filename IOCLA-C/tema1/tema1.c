#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_SIZE 300

struct Dir;
struct File;

typedef struct Dir{
	char *name;
	struct Dir* parent;
	struct File* head_children_files;
	struct Dir* head_children_dirs;
	struct Dir* next;
} Dir;

typedef struct File {
	char *name;
	struct Dir* parent;
	struct File* next;
} File;

void touch (Dir* parent, char* name) 
{
	//Checking if the file name already exists
	int name_exists = 0;
	File *f = parent->head_children_files;
	while(f != NULL)
	{
		if(!strcmp(f->name, name)) name_exists = 1;
		f = f->next;
	}

	//If it doesn't create and add it, else print "File already exists"
	if(!name_exists)
	{
		//Creating a new file
		File *file = (File*)malloc(sizeof(File));
		file->name = (char*)malloc(sizeof(char)*(sizeof(name)+1));
		strcpy(file->name, name);
		file->parent = parent;
		file->next = NULL;


		//Inserting the file in the file list
		if(parent->head_children_files == NULL) parent->head_children_files = file;
		else
		{
			//Getting end of file list
			f = parent->head_children_files;
			while(f->next != NULL) f = f->next;

			//Inserting the file
			f->next = file;
		}
	}
	else fprintf(stdout, "File already exists\n");
}

void mkdir (Dir* parent, char* name) 
{
	//Checking if the dir name already exists
	int name_exists = 0;
	Dir *d = parent->head_children_dirs;
	while(d != NULL)
	{
		if(!strcmp(d->name, name)) name_exists = 1;
		d = d->next;
	}

	//If it doesn't create and add it, else print "Directory already exists"
	if(!name_exists)
	{
		//Creating a new dir
		Dir *dir = (Dir*)malloc(sizeof(Dir));
		dir->name = (char*)malloc(sizeof(char)*(sizeof(name)+1));
		strcpy(dir->name, name);
		dir->parent = parent;
		dir->head_children_files = NULL;
		dir->head_children_dirs = NULL;
		dir->next = NULL;
		
		//Inserting the dir in the dir list
		if(parent->head_children_dirs == NULL) parent->head_children_dirs = dir;
		else
		{
			//Getting end of dir list
			d = parent->head_children_dirs;
			while(d->next != NULL) d = d->next;

			//Inserting the dir
			d->next = dir;
		}
	}
	else fprintf(stdout, "Directory already exists\n");
}

void ls (Dir* parent) 
{
	//Printing the dir list
	Dir *d = parent->head_children_dirs;
	while(d != NULL)
	{
		fprintf(stdout, "%s\n", d->name);
		d = d->next;
	}

	//Printing the file list
	File *f = parent->head_children_files;
	while(f != NULL)
	{
		fprintf(stdout, "%s\n", f->name);
		f = f->next;
	}
}

void rm (Dir* parent, char* name) 
{
	File *f = parent->head_children_files;

	//If the file list is empty print "Could not find the file"
	if(f == NULL)
	{
		fprintf(stdout, "Could not find the file\n");
		return;
	}

	//Checking if the file is the head of the file list
	if(!strcmp(f->name, name)) 
	{
		//Making the file list point to NULL
		parent->head_children_files = f->next;

		//Freeing the file
		free(f->name);
		free(f);
	}

	//The file is not the head of the file list
	else
	{
		//Getting our file and the file directly above our file in the file list
		File *fprev;
		while(f != NULL && strcmp(f->name, name))
		{
			fprev = f;
			f = f->next;
		}

		//If we couldn't find the file print "Could not find the file"
		if(f == NULL)
		{
			fprintf(stdout, "Could not find the file\n");
			return;
		}

		//Making the file directly above our file in the file list point to the next file
		fprev->next = f->next;

		//Freeing the file
		free(f->name);
		free(f);
	}
}

void rmdir (Dir* parent, char* name) 
{
	Dir *d = parent->head_children_dirs;

	//If the dir list is empty print "Could not find the dir"
	if(d == NULL)
	{
		fprintf(stdout, "Could not find the dir\n");
		return;
	}

	//Checking if the dir is the head of the file list
	if(!strcmp(d->name, name))
	{
		//Freeing the contents of the dir
		while(d->head_children_files != NULL) 
			rm(d, d->head_children_files->name);
		while(d->head_children_dirs != NULL)
			rmdir(d, d->head_children_dirs->name);

		//Making the dir list point to NULL
		parent->head_children_dirs = d->next;

		//Freeing the dir
		free(d->name);
		free(d);
	}

	//The dir is not the head of the dir list
	else
	{
		//Getting our dir and the dir directly above our dir in the dir list
		Dir *dprev;
		while(d != NULL && strcmp(d->name, name))
		{
			dprev = d;
			d = d->next;
		}

		//If we couldn't find the dir print "Could not find the dir"
		if(d == NULL)
		{
			fprintf(stdout, "Could not find the dir\n");
			return;
		}

		//Freeing the contents of the dir
		while(d->head_children_files != NULL) 
			rm(d, d->head_children_files->name);
		while(d->head_children_dirs != NULL)
			rmdir(d, d->head_children_dirs->name);
		
		//Making the dir directly above our dir in the dir list point to the next dir
		dprev->next = d->next;
		
		//Freeing the dir
		free(d->name);
		free(d);
	}
}

void cd(Dir** target, char *name) 
{
	//If we want to change cwd to the parent dir
	if(!strcmp(name, ".."))

		//Changing cwd to parent dir
		if((*target)->parent != NULL) *target = (*target)->parent;
		else return;

	//Else change cwd to specified dir
	else
	{
		//Finding the specified dir 
		Dir *d = (*target)->head_children_dirs;
		while(d != NULL && strcmp(d->name, name)) d = d->next;

		//If we couldn't find it print "No directories found!" and return
		if(d == NULL)
		{
			fprintf(stdout, "No directories found!\n");
			return;
		}

		//Changing cwd
		*target = d;
	}
}

char *pwd (Dir* target) 
{
	//If we found the root dir return "/home"
	if(target->parent == NULL)
	{
		char *path = (char*)malloc(sizeof(char)*MAX_INPUT_LINE_SIZE);
		strcpy(path, "/");
		return strcat(path, target->name);
	}

	//Else append to the end of the path up until current dir "/the name of the dir"
	else return strcat(strcat(pwd(target->parent), "/"), target->name);
}

void stop (Dir* target) 
{
	//Freeing all dirs and files in target dir (home)
	while(target->head_children_files != NULL) 
		rm(target, target->head_children_files->name);
	while(target->head_children_dirs != NULL) 
		rmdir(target, target->head_children_dirs->name);

	//Freeing target dir (home)
	free(target->name);
	free(target);

	//Stop main with exit code 0
	_Exit(0);
}

void tree (Dir* target, int level)
{
	//Level was used to determine the number of tabs before a dir/file name

	//Printing the dir list by recursively calling tree function with level + 1
	Dir *d = target->head_children_dirs;
	while(d != NULL)
	{
		for(int i=0; i<level; i++) fprintf(stdout, "    ");
		fprintf(stdout, "%s\n", d->name);
		tree(d, level+1);
		d = d->next;
	}

	//Printing the file list
	File *f = target->head_children_files;
	while(f != NULL)
	{
		for(int i=0; i<level; i++) fprintf(stdout, "    ");
		fprintf(stdout, "%s\n", f->name);
		f = f->next;
	}
}

void mv(Dir* parent, char *oldname, char *newname) 
{
	//Finding the dir
	Dir *d = parent->head_children_dirs;
	while(d != NULL && strcmp(d->name, oldname)) d = d->next;

	//Finding the file
	File *f = parent->head_children_files;
	while(f != NULL && strcmp(f->name, oldname)) f = f->next;

	//If we found a dir and a file or neither print "File/Director not found" and return
	if(((f != NULL) && (d != NULL)) || (f == NULL) && (d == NULL)) 
	{
		printf("File/Director not found\n");
		return;
	}

	File *f_new;
	Dir *d_new;

	//If a file was found
	if(f != NULL)
	{
		//Search for a file that already has the new name of our file
		f_new = parent->head_children_files;
		while(f_new != NULL && strcmp(f_new->name, newname)) f_new = f_new->next;

		//If we found one print "File/Director already exists" and return
		if(f_new != NULL)
		{
			printf("File/Director already exists\n");
			return;
		}

		//Search for a dir that already has the new name of our file
		d_new = parent->head_children_dirs;
		while(d_new != NULL && strcmp(d_new->name, newname)) d_new = d_new->next;

		//If we found one print "File/Director already exists" and return
		if(d_new != NULL)
		{
			printf("File/Director already exists\n");
			return;
		}	

		//Change the name of our file into the new name
		strcpy(f->name, newname);

		File *file = f;

		//If the file is the head of the file list
		if(file == parent->head_children_files)
		{
			//If the file list contains only our file return
			if(file->next == NULL) return;

			//Move our file to the end of the file list
			parent->head_children_files = file->next;
			f = parent->head_children_files;
			while(f->next != NULL) f = f->next;
			f->next = file;
			file->next = NULL; 
		}

		//Else
		else
		{
			//Get the file directly above our file in the file list
			f = parent->head_children_files;
			while(f->next != file) f = f->next;

			//Remove our file from the file list
			f->next = file->next;

			//Add our file to the end of the file list
			f = parent->head_children_files;
			while(f->next != NULL) f = f->next;
			f->next = file;
			file->next = NULL;
		}
	}

	//If a dir was found
	else if(d != NULL)
	{
		//Search for a file that already has the new name of our dir
		f_new = parent->head_children_files;
		while(f_new != NULL && strcmp(f_new->name, newname)) f_new = f_new->next;

		//If we found one print "File/Director already exists" and return
		if(f_new != NULL)
		{
			printf("File/Director already exists\n");
			return;
		}

		//Search for a dir that already has the new name of our dir
		d_new = parent->head_children_dirs;
		while(d_new != NULL && strcmp(d_new->name, newname)) d_new = d_new->next;

		//If we found one print "File/Director already exists" and return
		if(d_new != NULL)
		{
			printf("File/Director already exists\n");
			return;
		}	

		//Change the name of our dir into the new name
		strcpy(d->name, newname);

		Dir *dir = d;

		//If the dir is the head of the dir list
		if(d == parent->head_children_dirs)
		{
			//If the dir list contains only our dir return
			if(dir->next == NULL) return;

			//Move our dir to the end of the dir list
			parent->head_children_dirs = dir->next;
			d = parent->head_children_dirs;
			while(d->next != NULL) d = d->next;
			d->next = dir;
			dir->next = NULL;
		}

		//Else
		else
		{
			//Get the dir directly above our dir in the dir list
			d = parent->head_children_dirs;
			while(d->next != dir) d = d->next;

			//Remove our dir from the dir list
			d->next = dir->next;

			//Add our dir to the end of the dir list
			d = parent->head_children_dirs;
			while(d->next != NULL) d = d->next;
			d->next = dir;
			dir->next = NULL;
		}
	}
}

int main () {
	int true = 1;

	//initializing variables for input processing
	char *input = (char*)malloc(sizeof(char)*MAX_INPUT_LINE_SIZE);
	char *function = (char*)malloc(sizeof(char)*6);
	char *arg1 = (char*)malloc(sizeof(char)*(MAX_INPUT_LINE_SIZE-6)/2);
	char *arg2 = (char*)malloc(sizeof(char)*(MAX_INPUT_LINE_SIZE-6)/2);
	char *split_str;

	//Making a home directory and initializing cwd(current working directory)
	Dir *home = (Dir*)malloc(sizeof(Dir));
	home->name = (char*)malloc(sizeof(char)*5);
	strcpy(home->name, "home");
	home->parent = NULL;
	home->head_children_files = NULL;
	home->head_children_dirs = NULL;
	home->next = NULL;
	Dir *cwd = home;

	do
	{
		/*
		Summary:
			Reads from stdin a string and breaks it down into command and in
			case it needs into a name.
		*/

		//Getting input
		fgets(input, MAX_INPUT_LINE_SIZE, stdin);

		//Input processing
		split_str = strtok(input, " \n");
		strcpy(function, split_str);
		split_str = strtok(NULL, " \n");
		if(split_str != NULL)
		{
			strcpy(arg1, split_str);
			split_str = strtok(NULL, " \n");
			if(split_str != NULL) 
			{
				strcpy(arg2, split_str);
			}
		}

		//Choosing the function
		if(!strcmp(function, "touch"))
		{
			touch(cwd, arg1);
		} else if(!strcmp(function, "mkdir")) 
		{
			mkdir(cwd, arg1);
		} else if(!strcmp(function, "ls"))
		{
			ls(cwd);
		} else if(!strcmp(function, "rm"))
		{
			rm(cwd, arg1);
		} else if(!strcmp(function, "rmdir"))
		{
			rmdir(cwd, arg1);
		} else if(!strcmp(function, "cd"))
		{
			cd(&cwd, arg1);
		} else if(!strcmp(function, "tree"))
		{
			tree(cwd, 0);
		} else if(!strcmp(function, "pwd"))
		{
			char *path = pwd(cwd);
			fprintf(stdout, "%s\n", path);
			free(path);
		} else if(!strcmp(function, "mv"))
		{
			mv(cwd, arg1, arg2);
		} else if(!strcmp(function, "stop"))
		{
				free(input);
				free(function);
				free(arg1);
				free(arg2);
				stop(home);
		}
	} while (true);

	return 0;
}
