cd glfw
git pull
cd ..
cd Simple-OpenGL-Image-Library
git pull
cd ..
cd glm
git pull
cd ..
cd imgui
git pull
cd ..
cd spdlog
git pull
cd ..
cd efsw
git pull
cd ..
cd jsonxx
git pull
cd ..
cd mono
git pull
cd ..

sudo touch /etc/profile.d/osirislibs.sh
sudo chmod 777 /etc/profile.d/osirislibs.sh
sudo echo "export OSIRIS_EXTERN_REPOS="$PWD"/" > /etc/profile.d/osirislibs.sh
source /etc/profile.d/osirislibs.sh