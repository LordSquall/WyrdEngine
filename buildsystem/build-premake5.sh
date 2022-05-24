###### Premake5 ######
mkdir dependencies/premake-core
git clone https://github.com/premake/premake-core.git dependencies/premake-core
cd dependencies/premake-core
make -f Bootstrap.mak linux