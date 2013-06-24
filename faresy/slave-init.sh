# main slave logic, this is launched from slave-go.sh

mkdir -p incoming

# keep the webcam running at all times
while :
do
  if [ -c /dev/video0 ]
  then
    nice webcam webcam.conf
  fi
  sleep 3
done &

# upload changes to master and display latest image as test
# use separate script so changes can be picked up without rebooting
while :
do
  ./slave-loop.sh
	sleep 1
done
