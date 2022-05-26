sudo apt remove --assume-yes cmake xorg-dev libglu1-mesa-dev uuid-dev libglm-dev libglfw3 libglfw3-dev libsoil-dev mono-complete

cd dependencies/uuid/cmakedir
sudo xargs rm < install_manifest.txt
cd ../../..

cd dependencies/uuid/cmakedir
sudo xargs rm < install_manifest.txt
cd ../../..

sudo rm -rf dependencies
sudo rm -rf toolchain