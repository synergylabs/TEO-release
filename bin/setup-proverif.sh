#!/usr/bin/env bash 

sudo apt update
sudo apt install -y pkg-config libexpat1-dev libgtk2.0-dev

opam install -y proverif
