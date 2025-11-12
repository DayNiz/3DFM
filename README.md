# 3DFM *( 3 Dimention File Manager )*
Explore your filesytem in a 3D world ! 

## Usage
`./3DFM [path]`

3DFM will try to load the path if provided, otherwise it will load the current working directory.

3DFM is developped on Linux (Debian 13), but is also known to work on macOS 26, and compile on Windows (but haven't been tested yet).

You can find releases on the Release tab, or find the latest version on the Actions tab (but the latter require a GitHub Account)

## Navigation
 - To naviguate, use WASD (or the equivalent on your keaboard, for instance ZQSD on a AZERTY keyboard), the arrow keys and your mouse.
 - To enter a directory, put it around the center and click.
 - To go to the parent, press P (for Parent).
 - To show hidden files (those starting by a ., press H (for Hidden). However, the parent dir (..) is always shown, but apparently doesn't works (I'm trying to figure out why), and the current dir (.) is always hidden.
 - To unlock/lock your mouse pointer, do a right-click.


## TODO (in no order)
 - [x] basic file navigation
 - [ ] Navigate to .. by clicking the parent cube
 - [ ] Use real 3D models instead of basic cubes
 - [ ] File and folders operations: creating, deleting, opening, renaming
 - [ ] And more...

---
## Compiling
1. Clone this repo: `git clone https://github.com/DayNiz/3DFM && cd 3DFM`
2. Install [CMake](https://cmake.org/download)
3. Setup build directory: `cmake -B build`
4. Compile: `cmake --build build` 

## License
Copyright (C) 2025	Dayvid Nizier

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, version 3 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
