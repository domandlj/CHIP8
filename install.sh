#!/bin/sh

if [[ $OSTYPE == 'darwin'* ]]; 
then
  echo 'installing dependencies for macOS...'
  brew install SDL2

else
  echo 'installing dependencies form Linux...'
  sudo apt-get install libsdl2-2.0

fi
