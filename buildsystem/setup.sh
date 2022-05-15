# ensure all required header file and libs available
sudo apt install cmake
sudo apt install uuid-dev

# create toolchain dir to store any build tools
mkdir toolchain 

# create directory and clone premake
# premake5 is not available to install as a binary so we need to build it
git clone https://github.com/premake/premake-core.git ./toolchain/premake-core
cd toolchain/premake-core
make -f Bootstrap.mak linux
