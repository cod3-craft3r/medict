#!/bin/bash

IMAGE_NAME="medict-devenv"

# Check if the Docker image exists
if [[ "$(docker images -q $IMAGE_NAME 2> /dev/null)" == "" ]]; then
  echo
  echo -e "\e[35;1;3mImage \e[4m'$IMAGE_NAME'\e[24m is not present locally; building it now...\e[0m"
  echo
  docker build -t $IMAGE_NAME .
  echo
  echo -e "\e[32;1;3mImage \e[4m'$IMAGE_NAME'\e[24m has been built successfully.\e[0m"
  echo
  echo "starting a new container with this new image..."
  echo
  docker run --rm -it \
             -w /work \
             $IMAGE_NAME \
             /bin/bash
else
  echo -e "\e[32;1;3mImage \e[4m'$IMAGE_NAME'\e[24m is present locally.\e[0m"
  echo
fi

docker run --rm -it \
           -w /work \
           $IMAGE_NAME \
           /bin/bash
