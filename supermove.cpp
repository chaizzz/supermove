#include <iostream>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <cstring>
#include <stdlib.h>

void super(const char* path)
{
	struct stat st;

	if(0 == stat(path, &st))
	{
		if(!S_ISDIR(st.st_mode))
		{
			unlink(path);
			exit(1);
		}
	}

	struct dirent* pde = NULL;
	DIR* dirp = opendir(path);
	char old_cwd[101];
	
	if(dirp == NULL)
	{
		perror("opendir");
		return;
	}

	getcwd(old_cwd, sizeof(old_cwd));
	chdir(path);

	while(pde = readdir(dirp))
	{
		printf("%s\n", pde->d_name);
		if(0 == stat(pde->d_name, &st))
		{
			if(S_ISDIR(st.st_mode))
			{
				//文件夹
				if(strcmp(pde->d_name, ".") != 0 && strcmp(pde->d_name, "..") != 0)
					super(pde->d_name);
				remove(pde->d_name);
			}

			else
			{
				//普通文件
				unlink(pde->d_name);
			}
		}
	}

	chdir(old_cwd);

	closedir(dirp);

	remove(path);
}



int main(int argc, char** argv)
{
	if(argc != 2)
	{
		std::cout<< "错误" <<std::endl;
		exit(1);
	}

	super(argv[1]);	

	return 0;
}
