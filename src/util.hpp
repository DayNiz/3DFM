/* 3DFM - 3D File Manager
 * - Explore your filesystem in a 3D world -
 * Copyright (C) 2025	Dayvid Nizier
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once
#include <vector>
#include <string>
#include <filesystem>
#include "raylib.h"

namespace fs = std::filesystem;

typedef struct {
	fs::file_type type;
	std::string name;
	fs::path path;
	Vector3 position;
	Color color;
} fileT;

/**
 * @brief Load the path into the file_array
 *
 * @param path The path to open
 * @param file_array The fileT array to load into
 * @param skip_dotfiles Should we skip dot files ?
 */
void loadDir(
    fs::path path,
    std::vector<fileT>& file_array,
    bool skip_dotfiles
);

/**
 * @brief Detect if the file's cube is hovered by the mouse
 *
 * @param file The file to look at
 * @param camera The reference camera
 * @return float The distance between the camera and the file's cube
 */
float detectMouse(
    fileT file,
    Camera camera
);

/**
 * @brief Open given directory, and count the file
 * Internally, this function calls loadDir
 *
 * @param path The path to open
 * @param file_array The fileT array to load into
 * @param skip_dotfiles Should we skip dot files ?
 * @param num_files Where to write the number of files to
 * @param nfiles_sqrt Where to write the squared number of files to
 */
void open_dir(
    fs::path path,
    std::vector<fileT>& file_array,
    bool skip_dotfiles,
    int* num_files,
    int* nfiles_sqrt
);
