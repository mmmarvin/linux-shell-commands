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
#ifndef PATH_DEF_H
#define PATH_DEF_H

#include "path.h"

extern path_t trash_dir;
extern path_t trash_files_dir;
extern path_t trash_info_dir;
extern path_t cwd;

void init_trash_dirs();
void init_dirs();

path_t get_trash_dir();

#endif // PATH_DEF_H
