echo "Setting VV_ENGINE path to ~/.bashrc"
sed -i "/export VV_ENINGE_PATH=.*/d" ~/.bashrc
sudo -S echo "export VV_ENINGE_PATH="$(pwd)>>~/.bashrc
sudo -S echo 'export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${VV_ENINGE_PATH}/fmuZIP/Linux/Model2Id1/binaries/linux64'>>~/.bashrc
sudo -S echo 'export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${VV_ENINGE_PATH}/fmuZIP/Linux/LogicalModel/binaries/linux64'>>~/.bashrc
echo "-------------Finsihed--------------"