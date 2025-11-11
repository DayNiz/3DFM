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
#include <math.h>
#include "raylib.h"

#include "file_color.h"
#include "util.hpp"

#define SCREEN_WIDTH (600)
#define SCREEN_HEIGHT (450)

#define WINDOW_TITLE "3DFM"

namespace fs = std::filesystem;


/*
BeginDrawing();
DrawText("LOADING...", 0, 0, 18, GREEN);
EndDrawing();
loadDir(path, file_array, skip_dotfiles);
num_files = file_array.size();
nfiles_sqrt = (int)ceil(sqrt((double)num_files));
*/

int main(int argc, char **argv)
{
	const Vector3 cube_size = { 1.0f, 1.0f, 1.0f };
	int num_files;
	int nfiles_sqrt;
	bool skip_dotfiles = true;
	fs::path path;
	std::vector<fileT> file_array;

	switch (argc) {
	case 1:
		path = fs::current_path();
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

	BeginDrawing();
	DrawText("LOADING...", 0, 0, 18, GREEN);
	EndDrawing();
	loadDir(path, file_array, skip_dotfiles);

	num_files = file_array.size();
	nfiles_sqrt = (int)ceil(sqrt((double)num_files));

	// Define the camera
	Camera3D camera = {};
	camera.position = {	-2.7f, 0.8f, -2.7f };
	camera.target = { 0.0f, 0.0f, 0.0f };
	camera.up = { 0.0f, 1.0f, 0.0f };
	camera.fovy = 45.0f;
	camera.projection = CAMERA_PERSPECTIVE;


	while (!WindowShouldClose()) {
		if (IsKeyPressed(KEY_H)) {
			skip_dotfiles = !skip_dotfiles;
			BeginDrawing();
			DrawText("LOADING...", 0, 0, 18, GREEN);
			EndDrawing();
			loadDir(path, file_array, skip_dotfiles);
			num_files = file_array.size();
			nfiles_sqrt = (int)ceil(sqrt((double)num_files));
		}
		if (IsKeyPressed(KEY_P)) {
			BeginDrawing();
			DrawText("LOADING...", 0, 0, 18, GREEN);
			EndDrawing();
			path = path.parent_path();
			loadDir(path, file_array, skip_dotfiles);
			num_files = file_array.size();
			nfiles_sqrt = (int)ceil(sqrt((double)num_files));
		}

		BeginDrawing();
		{
			ClearBackground(WHITE);

			DrawFPS(SCREEN_WIDTH-10, SCREEN_HEIGHT-10);
			fs::path abs_path = fs::absolute(path);
			DrawText((const char*)abs_path.c_str(), SCREEN_WIDTH/2, 0, 12, C_TEXT);

			if (IsCursorHidden()) {
				UpdateCamera(&camera, CAMERA_FIRST_PERSON);
			}
			// Toggle camera control
			if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
				if (IsCursorHidden()) {
					EnableCursor();
				} else {
					DisableCursor();
				}
			}

			BeginMode3D(camera);
			{
				for (int i = 0; i  < file_array.size(); i++) {
					int x = std::floor(i/nfiles_sqrt);
					int y = i%nfiles_sqrt;
					file_array[i].position = (Vector3) {
						x * 2.0f,
						0.5f,
						y * 2.0f
					};
					Vector3 cube_pos = file_array[i].position;
					switch (file_array[i].type) {
					case fs::file_type::symlink:
						// Symlink file
						DrawCubeV(cube_pos, cube_size, file_array[i].color);
						break;
					case fs::file_type::regular :
						// Regular file
						DrawCubeV(cube_pos, cube_size, file_array[i].color);
						break;
					case fs::file_type::directory:
						// Directory file
						DrawCubeV(cube_pos, cube_size, file_array[i].color);
						break;
					case fs::file_type::block :
						// Block file
						DrawCubeV(cube_pos, cube_size, file_array[i].color);
						break;
					case fs::file_type::character:
						// Character file
						DrawCubeV(cube_pos, cube_size, file_array[i].color);
						break;
					case fs::file_type::fifo :
						// FIFO file
						DrawCubeV(cube_pos, cube_size, file_array[i].color);
						BeginMode3D(camera);
						break;
					case fs::file_type::socket :
						// Socket file
						DrawCubeV(cube_pos, cube_size, file_array[i].color);
						break;
					case fs::file_type::unknown:
					default:
						// Any other files
						DrawCubeV(cube_pos, cube_size, file_array[i].color);
						break;
					}

				}
				DrawPlane((Vector3) {
					nfiles_sqrt-0.5f, 0.0f, nfiles_sqrt-0.5f
				},
				(Vector2) {
					nfiles_sqrt*2.0f+1.0f, nfiles_sqrt*2.0f+1.0f
				},
				C_FLOOR);
			}
			EndMode3D();

			float closest_distance = 0.0;
			int closest_i = -1;
			for (int i = 0; i  < file_array.size(); i++) {
				float distance_pressed = detectMouse(file_array[i], camera);
				if (distance_pressed > closest_distance) {
					closest_distance = distance_pressed;
					closest_i = i;
				}
			}

			if (closest_distance != 0) {
				int x = std::floor(closest_i/nfiles_sqrt);
				int y = closest_i%nfiles_sqrt;
				//FIXME
				Vector2 name_scr_pos = (Vector2) {
					x*1.0f, y*1.0f
				};
				std::cout << "clicking on " << file_array[closest_i].name << std::endl;
				std::cout << (fs::exists(file_array[closest_i].name)) << std::endl;
				if (file_array[closest_i].type != fs::file_type::unknown
				        && file_array[closest_i].type == fs::file_type::directory
				        && fs::exists(file_array[closest_i].path)
				        && IsCursorHidden() ) {
					path = file_array[closest_i].path;
					std::cout << "loading " << path << std::endl;
					DrawText("LOADING...", 0, 0, 18, GREEN);
					loadDir(path, file_array, skip_dotfiles);
					num_files = file_array.size();
					nfiles_sqrt = (int)ceil(sqrt((double)num_files));
				}
			}
		}
		EndDrawing();
	}
	CloseWindow();

	return 0;
}
