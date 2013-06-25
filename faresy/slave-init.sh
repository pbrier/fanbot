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

# perform periodic outbound rsyncs
IP=`cat master-ip.txt`
while :
do
  RSYNC_PASSWORD=tralala rsync -au incoming/ jcw@$IP::uploads/
  sleep 10
done &

# upload changes to master and display latest image as test
# use separate script so changes can be picked up without rebooting
while :
do
  ./slave-loop.sh
	sleep 1
done
