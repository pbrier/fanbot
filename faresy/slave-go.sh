# put this line in crontab (crontab -e):
#   @reboot fanbot/faresy/slave-go.sh

cd ~/fanbot/faresy
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

# briefly show our own IP address on the display
IP=$(ip addr list eth0 | sed -n -e '/inet /s/\/.*//' -e 's/inet // p')
sudo slave-lcd/lcd -m "$IP"
sleep 3

# upload changes to master and display latest image as test
# use separate script so changes can be picked up without rebooting
while :
do
  ./slave-loop.sh
	sleep 1
done
