FROM alpine:3.12
RUN apk update && apk add --no-cache cmake make g++
COPY src/ ./src
COPY CMakeLists.txt .
RUN mkdir build && cd build && cmake .. && make
ENTRYPOINT ["/build/main"]
