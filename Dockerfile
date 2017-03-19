FROM ubuntu:latest

MAINTAINER Antoine Karcher <antoine.karcher.id@gmail.com>

RUN apt-get update 

RUN apt-get install -y build-essential cmake git

RUN mkdir -p /Projects

RUN cd /Projects && \
    git clone https://github.com/tonykero/Moe.git
