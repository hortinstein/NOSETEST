[![Gitpod Ready-to-Code](https://img.shields.io/badge/Gitpod-Ready--to--Code-blue?logo=gitpod)](https://gitpod.io/#https://github.com/hortinstein/NOSETEST) 

# NOSETEST

```
docker build -t nosetest .
sudo docker run -it --mount src=$(pwd),target=/workspace,type=bind nosetest /bin/ash
```