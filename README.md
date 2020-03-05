# NOSETEST

```
docker build -t nosetest .
sudo docker run -it --mount src=$(pwd),target=/workspace,type=bind nosetest /bin/ash
```