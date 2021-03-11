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
#include <stdio.h>
#include <stdlib.h>
#include "arg_parser.h"
#include "del.h"
#include "path.h"
#include "path_def.h"

void print_usage(const char* program_name);
void print_description();

int main(int argc, char** argv)
{
  init_dirs();
  init_trash_dirs();

  run_param_t param;
  init_run_param(&param);

  if(argc == 1) {
    print_usage(argv[0]);
    print_description();
  } else {
    bool error = false;
    size_t current_file_index = 0;
    for(int i = 1; i < argc; ++i) {
      if(argv[i][0] == '-') {
        if(i == 1) {
          if(!parse_flags(argv[i], &param)) {
            error = true;
            break;
          }
        } else {
          printf("Invalid usage!\n");
          print_usage(argv[0]);
          print_description();
          error = true;
          break;
        }
      } else {
        if(!param.num_files) {
          param.num_files = argc - i;
          param.files = malloc(sizeof(path_t) * param.num_files);
          param.files_deleted = malloc(sizeof(char*) * param.num_files);
          for(size_t j = 0; j < param.num_files; ++j) {
            param.files_deleted[j] = false;
          }

          if(parse_files(argv[i], &param, current_file_index)) {
            ++current_file_index;
          } else {
            error = true;
            break;
          }
        } else {
          if(parse_files(argv[i], &param, current_file_index)) {
            ++current_file_index;
          } else {
            error = true;
            break;
          }
        }
      }
    }

    if(!error) {
      del(&param);
    }
  }

  destroy_run_param(&param);
  return EXIT_SUCCESS;
}

void print_usage(const char* program_name)
{
  printf("Usage: %s -[flags] [files]...\n", program_name);
}

void print_description()
{
  printf("Flags:\n"
         "\tp\tPermanently delete the file\n"
         "\tq\tQuiet mode. Automatically moves / deletes folders\n");
}
