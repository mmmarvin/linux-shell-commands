/**********
 *
 *     This program is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 * 
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 * 
 *     You should have received a copy of the GNU General Public License
 *     along with this program.  If not, see <https://www.gnu.org/licenses/>. 
 *
 **********/
#include "del.h"

#include <ctype.h>
#include <dirent.h>
#include <fnmatch.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "path.h"
#include "path_def.h"
#include "str.h"

void del(run_param_t* param)
{
  if(param->permanent) {
    mani_files(param, &del_file);
  } else {
    mani_files(param, &mov_file);
  }
}

void del_file(const path_t* path, run_param_t* param)
{
  path_t orig_filename = get_filename(path);

  if(is_dir(path)) {
    if(!param->quiet) {
      printf("%s is a directory. Continue?[Y/N]: ", get_path_str(&orig_filename));

      char a = getchar();
      while(tolower(a) != 'y' && tolower(a) != 'n') {
        a = getchar();
      }

      if(tolower(a) == 'n') {
        return;
      }
    }
  }

//  printf("Deleting %s\n", get_path_str(path));
}

void mov_file(const path_t* path, run_param_t* param)
{
  path_t orig_filename = get_filename(path);
  path_t filename = combine_path(&trash_files_dir, &orig_filename);

  // check if the file already exists
  int i = 2;
  while(!access(&filename.value[0], F_OK)) {
    path_t file_extension = get_file_extension(&orig_filename);
    path_t new_filename = remove_extension(&orig_filename);

    sprintf(&new_filename.value[0], "%s%d", get_path_str(&new_filename), i++);
    new_filename = combine_path_string(&new_filename, ".");
    new_filename = combine_path_string(&new_filename, &file_extension.value[0]);

    filename = combine_path(&trash_files_dir, &new_filename);
  }

  if(is_dir(path)) {
    if(!param->quiet) {
      printf("%s is a directory. Continue?[Y/N]: ", get_path_str(&orig_filename));

      char a = getchar();
      while(tolower(a) != 'y' && tolower(a) != 'n') {
        a = getchar();
      }

      if(tolower(a) == 'n') {
        return;
      }
    }
  }

  rename(get_path_str(path), get_path_str(&filename));
  generate_outfile(&filename, path);

//  printf("Moving %s to %s\n", get_path_str(path), get_path_str(&filename));
}

void generate_outfile(const path_t* file_path, const path_t* path)
{
  static const char* TRASH_INFO_STR = "[Trash Info]";
  static const char* PATH_STR = "Path=";
  static const char* DELETION_DATA_STR = "DeletionDate=";
  static const char* NEW_LINE = "\n";

  path_t filename = get_filename(file_path);
  filename = combine_path_string(&filename, ".trashinfo");
  filename = combine_path(&trash_info_dir, &filename);

  path_t complete_file_path;
  if(path->value[0] == '/') {
    complete_file_path = copy_path(path);
  } else {
    complete_file_path = combine_path(&cwd, path);
  }

  FILE* f = fopen(get_path_str(&filename), "w");
  if(f) {
    fwrite(TRASH_INFO_STR, sizeof(char), strlen(TRASH_INFO_STR), f);
    fwrite(NEW_LINE, sizeof(char), strlen(NEW_LINE), f);

    fwrite(PATH_STR, sizeof(char), strlen(PATH_STR), f);
    fwrite(&complete_file_path.value[0],
           sizeof(char),
           strlen(&complete_file_path.value[0]),
           f);
    fwrite(NEW_LINE, sizeof(char), strlen(NEW_LINE), f);

    fwrite(DELETION_DATA_STR, sizeof(char), strlen(DELETION_DATA_STR), f);

    char date_buff[255];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(&date_buff[0],
            "%d-%02d-%02dT%02d:%02d:%02d",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    fwrite(&date_buff[0], sizeof(char), strlen(&date_buff[0]), f);
    fwrite(NEW_LINE, sizeof(char), strlen(NEW_LINE), f);
  }

//  printf("Generating outputfile %s\n", get_path_str(&filename));
}

void mani_files(run_param_t* param, void (*func)(const path_t*, run_param_t*))
{
  for(size_t i = 0; i < param->num_files; ++i) {
    if(contains(get_path_str(&param->files[i]), '*')) {
      printf("Unknown file \"%s\"\n", get_path_str(&param->files[i]));
//      path_t root = get_root_path(&param->files[i]);
//      DIR* d = opendir(get_path_str(&root));
//      struct dirent* e;

//      if(d) {
//        while((e = readdir(d)) != NULL) {
//          if(!fnmatch(get_path_str(&param->files[i]), e->d_name, FNM_PATHNAME)) {
//            path_t real_filename;
//            strcpy(&real_filename.value[0], e->d_name);

//            printf("Match %s\n", get_path_str(&real_filename));

//            path_t complete_path = combine_path(&root, &real_filename);
//            func(&complete_path, param);
//            param->files_deleted[i] = true;
//          }
//        }
//      }
    } else {
//      printf("File: \"%s\"\n", get_path_str(&param->files[i]));
      func(&param->files[i], param);
      param->files_deleted[i] = true;
    }
  }
}
