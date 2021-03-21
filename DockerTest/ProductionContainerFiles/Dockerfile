FROM ubuntu AS build

RUN apt-get update && apt-get -y install build-essential

RUN apt-get update && apt-get -y install cmake protobuf-compiler

WORKDIR /src

COPY CMakeLists.txt main.cpp ./

RUN cmake . && make

FROM ubuntu

COPY --from=build /src/CxxCode ./

CMD ["./CxxCode"]
