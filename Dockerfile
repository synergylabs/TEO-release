FROM ocaml/opam:ubuntu-20.04-ocaml-4.06

ARG DEBIAN_FRONTEND=noninteractive

COPY . /app
WORKDIR /app

# Install ProVerif
RUN bin/setup-proverif.sh
