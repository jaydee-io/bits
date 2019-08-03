#!/usr/bin/env bash

NAME_GTEST="googletest"
REPO_GTEST="https://github.com/google/$NAME_GTEST.git"
TAG_GTEST="release-1.8.1"

function install_GTest()
{
    # Go to a tmp dir
    rm -fr tmp-$NAME_GTEST
    mkdir tmp-$NAME_GTEST
    cd tmp-$NAME_GTEST

    # Clone
    git clone -b $TAG_GTEST --depth=1 $REPO_GTEST

    # Build
    mkdir build
    cd build
    cmake ../$NAME_GTEST -DCMAKE_BUILD_TYPE=Release
    cmake --build . -j

    # Install
    sudo make install
    # This is better, but doesn't work on Travis CI
    # sudo cmake --install .
}

install_GTest