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
#include "path_def.h"

#include <sys/stat.h>
#include <pwd.h>

path_t trash_dir;
path_t trash_files_dir;
path_t trash_info_dir;
path_t cwd;

void init_trash_dirs()
{
  path_t files_dir = create_path("files");
  path_t info_dir = create_path("info");

  trash_dir = get_trash_dir();

  trash_files_dir = get_trash_dir();
  trash_files_dir = combine_path(&trash_files_dir, &files_dir);

  trash_info_dir = get_trash_dir();
  trash_info_dir = combine_path(&trash_info_dir, &info_dir);

  struct stat st;

  if(stat(&trash_dir.value[0], &st) == -1) {
    mkdir(get_path_str(&trash_dir), 0700);
//    printf("Creating folder: %s\n", get_path_str(&trash_dir));
  }

  if(stat(&trash_files_dir.value[0], &st) == -1) {
    mkdir(get_path_str(&trash_files_dir), 0700);
//    printf("Creating folder: %s\n", get_path_str(&trash_files_dir));
  }

  if(stat(&trash_info_dir.value[0], &st) == -1) {
    mkdir(get_path_str(&trash_info_dir), 0700);
//    printf("Creating folder: %s\n", get_path_str(&trash_info_dir));
  }
}

void init_dirs()
{
  getcwd(cwd.value, sizeof(cwd.value));
}

path_t get_trash_dir()
{
  path_t home_dir = create_path(getpwuid(getuid())->pw_dir);
  path_t trash_dir = create_path(".local/share/Trash");

  return combine_path(&home_dir, &trash_dir);
}
