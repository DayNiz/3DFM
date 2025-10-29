/* 3DFM - 3D File Manager
 * - Explore your filesystem in a 3D world -
 * Copyright (C) 2025	Dayvid Nizier
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//TODO: Replace dirent by tinydir
//TODO: replace fileListe[] by stb_ds
//#include <dirent.h>

#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <math.h>
#include "raylib.h"


#include "tinydir.h"
#include "stb_ds.h"

#define SCREEN_WIDTH (800)
#define SCREEN_HEIGHT (450)

#define WINDOW_TITLE "3DFM"

typedef struct {
	int type;
	// 255 char is usually the max for a filename
	char name[255];
} fileT;

int main(int argc, char** argv)
{
	char* path;
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
	tinydir_file file;

	tinydir_open_sorted(&dir, path);
	
	/*unsigned int numFiles = 0;
	while (file != NULL)
	{
		if (file->d_name[0] == '.')
		{
			// We skip hidden files.
			file = readdir(dirp);
			continue;
		}
		switch (file->d_type)
		{
			case DT_REG:
				printf("name: %s\n", file->d_name);
				numFiles++;
				break;
			case DT_DIR:
				printf("name: %s\n", file->d_name);
				numFiles++;
				break;
			case DT_LNK:
				printf("name: %s\n", file->d_name);
				numFiles++;
				break;
			default:
				break;
		}
		printf("\n");
		file = readdir(dirp);
	}

	printf("count: %i\n", numFiles);
	*/
	printf("count: %i\n", dir.n_files);

	//DEBUG
	tinydir_close(&dir);
	////////

	/*
	/\* We run once again to set the array of files *\/
	fileT fileArray[numFiles];
	/\* We close and reopen the dir *\/
	closedir(dirp);
	dirp = opendir(path);
	if (dirp == NULL)
	{
		switch (errno)
		{
			case EACCES:
				fprintf(stderr, "ERR: Access denied\n\n");
				return 2;
			case ENOENT:
				fprintf(stderr, "ERR: No such file or directory\n\n");
				return 2;
			case ENOTDIR:
				fprintf(stderr, "ERR: Not a directory\n\n");
				return 2;
			default:
				fprintf(stderr, "ERR opening dir");
				return 2;
		}
	}
	file = readdir(dirp);

	int count = 0;
	int all = 0;
	while (file != NULL)
	{
		if (file->d_name[0] == '.')
		{
			file = readdir(dirp);
			continue;
		}
		strcpy(fileArray[count].name, file->d_name);
		fileArray[count].type = file->d_type;

		file = readdir(dirp);
		count++;
	}
	closedir(dirp);
	if (count == 0)
	{
		printf("Dir is empty, quitting for now...");
		return 3;
	}


	int numFilesSquared = (int)ceil( sqrt( (double)numFiles) );

	// Define the camera to look into our 3d world
	Camera3D camera = { 0 };
	camera.position = (Vector3){ 0.0f, 3.5f, -5.0f };	// Camera position
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
							case DT_REG:
								DrawCube( cubePos, 1.0f, 1.0f, 1.0f, GREEN);
								EndMode3D();
								DrawText(fileArray[i].name, nameScrPos.x, nameScrPos.y, 10, WHITE);
								BeginMode3D(camera);
								break;
							case DT_DIR:
								DrawCube( cubePos, 1.0f, 1.0f, 1.0f, RED);
								EndMode3D();
								DrawText(fileArray[i].name, nameScrPos.x, nameScrPos.y, 10, WHITE);
								BeginMode3D(camera);
								break;
							case DT_LNK:
								DrawCube( cubePos, 1.0f, 1.0f, 1.0f, BLUE);
								EndMode3D();
								DrawText(fileArray[i].name, nameScrPos.x, nameScrPos.y, 10, WHITE);
								BeginMode3D(camera);
								break;
							default:
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
	*/

	return 0;
}

