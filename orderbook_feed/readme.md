## Architecture

![Screenshot from 2023-05-25 21-54-51](https://github.com/Naseefabu/HFTBOT/assets/104965020/f93d8f82-0fdb-4996-8c65-48b04db553c1)



# Build Instructions

### Install dependencies

```
# install dependent packages
sudo apt-get update
sudo apt-get install libboost-all-dev


# install cmake
sudo apt install cmake

# install boost 1.79
wget https://www.boost.org/users/history/version_1_79_0.html
tar --bzip2 -xf boost_1_79_0.tar.bz2
cd boost_1_79_0/
./boostrap.sh
./b2
sudo ./b2 install
```

steps to build and run the feedhandler :
```
git clone --recurse-submodules https://github.com/Naseefabu/HFTRepo.git
cd orderbook_feed
rm -rf build
mkdir build
cd build
cmake ..
make
./coinbase

```