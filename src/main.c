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

//TODO: replace fileListe[] by stb_ds

#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <math.h>
#include "raylib.h"


#include "tinydir.h"
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define SCREEN_WIDTH (800)
#define SCREEN_HEIGHT (450)

#define WINDOW_TITLE "3DFM"

typedef struct {
	// With tinydir, I can only know if it's a file, a dir,
	// or something else, so for now:
	// 0: reg file, 1: dir, 2; unknown
	int type;
	// 255 char is usually the max for a filename
	char name[255];
} fileT;

int main(int argc, char** argv)
{
	char* path;
	int numFiles;

	switch (argc)
	{
		case 1:
			path = ".";
			break;
		case 2:
			path = argv[1];
			break;
		default:
			fprintf(stderr, "Usage: ./3DFM [path_to_folder]\n");
			return 1;
	}

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
	SetTargetFPS(60);

	tinydir_dir dir;

	tinydir_open_sorted(&dir, path);
	numFiles = dir.n_files;
	
	printf("count: %i\n", numFiles);

	/* We run once again to set the array of files */
	fileT fileArray[numFiles];

	for (int i = 0; i < numFiles; i++)
	{
		tinydir_file file;
		tinydir_readfile_n(&dir, &file, i);

		// We record the file name and type
		printf("Name: %s\n", file.name);
		strcpy(fileArray[i].name, file.name);
		if (file.is_reg)
		{
			fileArray[i].type = 0;
		}
		else if (file.is_dir)
		{
			fileArray[i].type = 1;
		}
		else
		{
			fileArray[i].type = 2;
		}
	}
	tinydir_close(&dir);

	int numFilesSquared = (int)ceil( sqrt( (double)numFiles) );

	// Define the camera to look into our 3d world
	Camera3D camera = { 0 };
	camera.position = (Vector3){ 0.0f, -2.0f, -5.5f };	// Camera position
	camera.target = (Vector3){ 0.0f, 1.0f, 0.0f };		// Camera looking at point
	camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };			// Camera up vector (rotation towards target)
	camera.fovy = 45.0f;								// Camera field-of-view Y
	camera.projection = CAMERA_PERSPECTIVE;				// Camera mode type
	Vector3 cubePos;
	Vector2 nameScrPos;

	while (!WindowShouldClose())
	{
		BeginDrawing();
		{
			ClearBackground(BLACK);

			DrawFPS(0,0);
			BeginMode3D(camera);
			int i = 0;
			{
				UpdateCamera(&camera, CAMERA_THIRD_PERSON);
				for (int x = -numFilesSquared/2 ; x<=numFilesSquared/2; x++)
				{
					for (int y = -numFilesSquared/2 ; y<=numFilesSquared/2 ; y++)
					{
						if (++i > numFiles)
						{
							break;
						}
						cubePos = (Vector3){ x*2.0f , 0.0f , y*2.0f };
						nameScrPos = GetWorldToScreen(cubePos, camera);
						switch (fileArray[i].type)
						{
							case 0:
								// Regular file
								DrawCube( cubePos, 1.0f, 1.0f, 1.0f, GREEN);
								EndMode3D();
								DrawText(fileArray[i].name, nameScrPos.x, nameScrPos.y, 10, WHITE);
								BeginMode3D(camera);
								break;
							case 1:
								// Directory file
								DrawCube( cubePos, 1.0f, 1.0f, 1.0f, RED);
								EndMode3D();
								DrawText(fileArray[i].name, nameScrPos.x, nameScrPos.y, 10, WHITE);
								BeginMode3D(camera);
								break;
							default:
								// Any other files
								DrawCube( cubePos, 1.0f, 1.0f, 1.0f, PURPLE);
								EndMode3D();
								DrawText("unknown", nameScrPos.x, nameScrPos.y, 10, WHITE);
								BeginMode3D(camera);
								break;
						}
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

