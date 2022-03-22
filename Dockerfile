FROM ocaml/opam:ubuntu-20.04-ocaml-4.06

ARG DEBIAN_FRONTEND=noninteractive
ARG uid=1000

RUN sudo apt update -qq && sudo apt install -qq -y python3-pip

COPY . /app
RUN sudo chown -R ${uid}:${uid} /app 

WORKDIR /app

# Install ProVerif
RUN bin/setup-proverif.sh
RUN bin/setup-protocol-compiler.sh