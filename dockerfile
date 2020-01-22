#!/usr/bin/env -S docker build --compress -t pvtmert/simple-gc -f

FROM debian:testing as build

RUN apt update
RUN apt install -y \
	build-essential

WORKDIR /data
COPY ./ ./
RUN make -j1 clean all || true

FROM debian:testing as runtime
COPY --from=build /data/*.exe /opt
CMD find /opt -perm -111 -type f -exec {} \;
