Meteor
======
[![Build Status](https://travis-ci.org/kiddos/meteor.svg?branch=master)](https://travis-ci.org/kiddos/meteor)

A game written in Alleogro library

##build

grab the dependencies

```shell
sudo apt-get install build-essential git cmake cmake-curses-gui xorg-dev libgl1-mesa-dev libglu-dev
sudo apt-get install libpng-dev libcurl4-openssl-dev libfreetype6-dev libjpeg-dev libvorbis-dev libopenal-dev \
       libphysfs-dev libgtk2.0-dev libasound-dev libpulse-dev libflac-dev libdumb1-dev
```

You will need allegro5 c library first

```shell
git clone https://github.com/liballeg/allegro5.git
cd allegro5
mkdir build
cd build
cmake ..
make
sudo make install
```


building ...

```shell
mkdir build
cd build
cmake ..
make
sudo make install
```

###Play
type in shell
```shell
meteor
```
