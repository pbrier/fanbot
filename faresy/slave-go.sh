# put this line in crontab (crontab -e):
#   @reboot fanbot/faresy/slave-go.sh

#PATH=/usr/local/bin:$PATH
cd ~/fanbot/faresy
IP=`cat master-ip.txt`

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
while :
do
	RSYNC_PASSWORD=tralala rsync -a incoming/ jcw@$IP::uploads/
	for i in 1 2 3 4 5
	do
    convert /tmp/snap.jpg -geometry 240x320 /tmp/snap.rgb
    sudo nice --20 slave-lcd/lcd -b /tmp/snap.rgb
	  sleep 2
	done
	cp -a /tmp/snap.jpg incoming/0000.jpg
done
