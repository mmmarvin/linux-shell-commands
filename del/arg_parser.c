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
#include "arg_parser.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void init_run_param(run_param_t* param)
{
  param->permanent = false;
  param->quiet = false;
  param->files = NULL;
  param->files_deleted = NULL;
  param->num_files = 0;
}

void destroy_run_param(run_param_t* param)
{
  if(param->files) {
    free(param->files);
    free(param->files_deleted);
  }
}

bool parse_flags(const char* flags, run_param_t* param)
{
  for(size_t i = 1; i < strlen(flags); ++i) {
    switch(flags[i]) {
    case 'p':
      param->permanent = true;
      break;
    case 'q':
      param->quiet = true;
      break;
    default:
      printf("Invalid flag %c", flags[i]);
      return false;
    }
  }

  return true;
}

bool parse_files(const char* file, run_param_t* param, size_t current_file_index)
{
  if(strnlen(file, FILENAME_MAX + 1) == FILENAME_MAX + 1) {
    char shrt[FILENAME_MAX];
    strncpy(&shrt[0], file, FILENAME_MAX);

    printf("Filename \"%s\" is too long!", &shrt[0]);
    return false;
  }

  param->files[current_file_index] = create_path(file);
  return true;
}
