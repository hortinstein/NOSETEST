[![Gitpod Ready-to-Code](https://img.shields.io/badge/Gitpod-Ready--to--Code-blue?logo=gitpod)](https://gitpod.io/#https://github.com/hortinstein/NOSETEST) 

# NOSETEST

```
docker build -t nosetest .
sudo docker run -it --mount src=$(pwd),target=/workspace,type=bind nosetest /bin/ash



cp /monocypher-3.0.0/src/monocypher.h /workspace/NOSETEST/include/
cp /monocypher-3.0.0/src/monocypher.c /workspace/NOSETEST/src/

```

