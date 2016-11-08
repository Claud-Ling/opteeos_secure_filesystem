# opteeos_secure_filesystem

A Fuse-based filesystem that is encrypted using [Optee OS](https://github.com/OP-TEE/optee_os) running in Arm TrustZone. Development in progress.

## Build Instructions

Use build.sh, which will set up the cross-compile variables to compile for Arm64 on the raspberrypi 3. However, Fuse needs to be downloaded and cross-compiled first.

To cross-compile Fuse, first download the libfuse submodule:

`git submodule init`
`git submodule update`

To build libfuse:

`./build.sh fuse`

To build host application:

`./build.sh host`

To build trusted application:

`./build.sh ta`

TODO:
* Add downloading and building Optee into build script/Makefile
* Use make rather than build script; create scripts to call for downloading and building different modules
* Go beyond the testing code after testing is done
