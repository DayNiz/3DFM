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

#include <cmath>
#include <sys/types.h>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include "raylib.h"

#define SCREEN_WIDTH (600)
#define SCREEN_HEIGHT (450)

#define WINDOW_TITLE "3DFM"

namespace fs = std::filesystem;

typedef struct {
	// Maybe creating a type is useless but fow now I'll keep it.
	fs::file_type type;
	std::string name;
} fileT;

int main(int argc, char **argv)
{
	// Should we skip hidden (.*) files ?
	bool skip_hidden_files = true;
	fs::path path;
	std::vector<fileT> file_array;
	int num_files;
	int nfiles_sqrt;

	switch (argc) {
	case 1:
		path = ".";
		break;
	case 2:
		path = argv[1];
		break;
	default:
		std::cerr << "Usage: ./3DFM [path_to_folder]" << std::endl;
		exit(1);
	}
	if (!fs::exists(path)) {
		std::cerr << "Directory doesn't exist." << std::endl;
		exit(1);
	}
	if (!fs::is_directory(path)) {
		std::cerr << "This is not a directory." << std::endl;
		exit(1);
	}

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
	SetTargetFPS(60);
	DisableCursor();

	for (fs::directory_entry const &file : fs::directory_iterator{path}) {
		std::string name = file.path().filename().string();
		if (skip_hidden_files and name[0] == '.' and name != "..") {
			std::cout<<"Skipping "<<file.path().filename().string()<<std::endl;
			continue;
		}
		fileT current_file;
		// We record the file name and type
		std::cout << "Name: " << name << std::endl;
		current_file.name = name;
		/* Checking file type */
		if (file.is_regular_file()) {
			std::cout << "type: file" << std::endl;
			current_file.type = fs::file_type::regular;
		} else if (file.is_directory()) {
			std::cout << "type: dir" << std::endl;
			current_file.type = fs::file_type::directory;
		} else if (file.is_symlink()) {
			std::cout << "type: sym" << std::endl;
			current_file.name = fs::read_symlink(file);
			current_file.type = fs::file_type::symlink;
		} else if (file.is_block_file()) {
			std::cout << "type: blk" << std::endl;
			current_file.type = fs::file_type::block;
		} else if (file.is_character_file()) {
			std::cout << "type: chr" << std::endl;
			current_file.type = fs::file_type::character;
		} else if (file.is_fifo()) {
			std::cout << "type: fifo" << std::endl;
			current_file.type = fs::file_type::fifo;
		} else if (file.is_socket()) {
			std::cout << "type: socket" << std::endl;
			current_file.type = fs::file_type::socket;
		} else {
			std::cout << "type: other" << std::endl;
			current_file.type = fs::file_type::unknown;
		}
		file_array.push_back(current_file);
	}

	num_files = file_array.size();
	nfiles_sqrt = (int)ceil(sqrt((double)num_files));

	// Define the camera to look into our 3d world
	Camera3D camera = {};
	camera.position = (Vector3) {
		-2.7f, 3.3f, -2.7f
	    };
	camera.target = (Vector3) {
		0.0f, 0.0f, 0.0f
	};
	camera.up = (Vector3) {
		0.0f, 1.0f, 0.0f
	};
	camera.fovy = 45.0f;
	camera.projection = CAMERA_PERSPECTIVE;

	Vector3 cube_pos;
	Vector3 cube_size;
	Vector2 name_scr_pos;

	while (!WindowShouldClose()) {
		BeginDrawing();
		{
			ClearBackground(WHITE);

			DrawFPS(SCREEN_WIDTH-10, SCREEN_HEIGHT-10);
			fs::path abs_path = fs::absolute(path);
			DrawText(abs_path.c_str(), 0, 0, 12, BLACK);
			BeginMode3D(camera);
			{
				UpdateCamera(&camera, CAMERA_THIRD_PERSON);
				for (int i = 0; i  < file_array.size(); i++) {
					int x = std::floor(i/nfiles_sqrt);
					int y = i%nfiles_sqrt;
					cube_pos = (Vector3) {
						x * 2.0f,
						0.0f,
						y * 2.0f
					};
					cube_size = (Vector3) {
						1.0f, 1.0f, 1.0f
					};

					name_scr_pos = GetWorldToScreen(cube_pos, camera);
					switch (file_array[i].type) {
					case fs::file_type::regular :
						// Regular file
						DrawCubeV(cube_pos, cube_size, GREEN);
						EndMode3D();
						DrawText(file_array[i].name.c_str(), name_scr_pos.x,
						         name_scr_pos.y, 10, BLACK);
						BeginMode3D(camera);
						break;
					case fs::file_type::directory:
						// Directory file
						DrawCubeV(cube_pos, cube_size, RED);
						EndMode3D();
						DrawText(file_array[i].name.c_str(), name_scr_pos.x,
						         name_scr_pos.y, 10, BLACK);
						BeginMode3D(camera);
						break;
					case fs::file_type::symlink:
						// Symlink file
						DrawCubeV(cube_pos, cube_size, BLUE);
						EndMode3D();
						DrawText(file_array[i].name.c_str(), name_scr_pos.x,
						         name_scr_pos.y, 10, BLACK);
						BeginMode3D(camera);
						break;
					case fs::file_type::block :
						// Block file
						DrawCubeV(cube_pos, cube_size, PURPLE);
						EndMode3D();
						DrawText(file_array[i].name.c_str(), name_scr_pos.x,
						         name_scr_pos.y, 10, BLACK);
						BeginMode3D(camera);
						break;
					case fs::file_type::character:
						// Character file
						DrawCubeV(cube_pos, cube_size, BROWN);
						EndMode3D();
						DrawText(file_array[i].name.c_str(), name_scr_pos.x,
						         name_scr_pos.y, 10, BLACK);
						BeginMode3D(camera);
						break;
					case fs::file_type::fifo :
						// FIFO file
						DrawCubeV(cube_pos, cube_size, ORANGE);
						EndMode3D();
						DrawText(file_array[i].name.c_str(), name_scr_pos.x,
						         name_scr_pos.y, 10, BLACK);
						BeginMode3D(camera);
						break;
					case fs::file_type::socket :
						// Socket file
						DrawCubeWiresV(cube_pos, cube_size, PINK);
						EndMode3D();
						DrawText(file_array[i].name.c_str(), name_scr_pos.x,
						         name_scr_pos.y, 10, BLACK);
						BeginMode3D(camera);
						break;
					default:
						// Any other files
						DrawCube(cube_pos, 1.0f, 1.0f, 1.0f, GRAY);
						EndMode3D();
						DrawText("unknown", name_scr_pos.x,
						         name_scr_pos.y, 10, BLACK);
						BeginMode3D(camera);
						break;
					}
				}
				DrawGrid(20, 1.0f);
			}
			EndMode3D();
		}
		EndDrawing();
	}
	CloseWindow();

	return 0;
}
