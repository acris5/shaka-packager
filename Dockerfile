FROM alpine:3.14 as builder

# Install utilities, libraries, and dev tools.
RUN apk update && apk add --no-cache \
        bash curl \
        bsd-compat-headers linux-headers \
        build-base cmake git ninja python3

RUN apk update && apk add nlohmann-json nano go

# Build shaka-packager from the current directory, rather than what has been
# merged.
WORKDIR shaka-packager
COPY . /shaka-packager/
RUN rm -rf build
RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=1 -G Ninja
RUN cmake --build build/ --config Debug --parallel
RUN cp /shaka-packager/build/packager/libpackager.so \
       /shaka-packager/build/packager/libpackager_ext.so /usr/lib/
RUN cd packager; go build packager_ext.go ; chmod +x packager_ext

# Copy only result binaries to our final image.
FROM alpine:3.14
RUN apk add --no-cache libstdc++ python3 nlohmann-json nano go
COPY --from=builder /shaka-packager/build/packager/packager \
                    /shaka-packager/build/packager/mpd_generator \
                    /shaka-packager/build/packager/pssh-box.py \
                    /shaka-packager/packager/packager_ext \
                    /usr/bin/

COPY --from=builder /shaka-packager/build/packager/libpackager.so \
                    /shaka-packager/build/packager/libpackager_ext.so \
                    /usr/lib/

# Copy pyproto directory, which is needed by pssh-box.py script. This line
# cannot be combined with the line above as Docker's copy command skips the
# directory itself. See https://github.com/moby/moby/issues/15858 for details.
COPY --from=builder /shaka-packager/build/packager/pssh-box-protos \
                    /usr/bin/pssh-box-protos
