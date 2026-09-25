 Instructions to create docker container:-
1. Copy all the required config files and binaries to DockerCreation folder.
2. Keep the Release binaries in OUTPUT folder.
3. Open createImage_GEN7.sh and edit line 70 to line 75 according to requirement for copying the above files inside docker container.
4. Now open terminal in the same directory and use following commandsin order :-
   To create base image - sudo sh create_base_image_GEN7.sh
   To create the image  - sudo sh createImage_GEN7.sh
5. After running the above commands copy the .tar and .simg file and paste it inside DockerRun folder.
6. keep the required logs in DockerRun/INPUT_LOGS/input_logs/
7. To run the docker container, open a terminal in DockerRun folder and use - sudo ./GEN_LOG.sh /BMW_SRR5_Input.json /output/OUTPUT_DIR /sil_engine_config.xml




Docker commands:-
To see all the images already present in PC - sudo docker images   
To delete an image - sudo docker rmi -f image_id
service docker start
service docker stop
Service docker status
docker info
docker images
docker ps -a
docker run -it -name <container_name> ubuntu /bin/bash
exit(exit from docker container)
docker pull <image_name>
docker search <image_name>
docker start <container_name>
docker attach <container_name>
docker stop <container_name>
docker rm <container_name>
