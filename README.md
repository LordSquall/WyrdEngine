# Wyrd Engine

## Build Instructions:
### Prerequisites:
- Mono (x64)
  - https://www.mono-project.com/download/stable/
- Visual Studio 2019 (x64)
  - https://visualstudio.microsoft.com/downloads/

### Optional installs
- RenderDoc 
  - https://renderdoc.org/
- Visual Studio Code 
  - https://code.visualstudio.com/
- GIMP
  - https://www.gimp.org/
- Inkscape
  - https://inkscape.org/
- Blender
  - https://www.blender.org/

### Steps
1. Install the Prerequisites listed above
2. Create a directory to hold all external git libraries
3. Copy `<repo>\toolchain\clone_all.bat` and `<repo>\toolchain\pull_all.bat` into your new directory
4. run `clone_all.bat`. This will clone all the dependant repos into the folder and create a new environment variable called `WYRD_EXTERN_REPOS` as a reference point.
5. Run `<repo>\build.bat` to create the solution
