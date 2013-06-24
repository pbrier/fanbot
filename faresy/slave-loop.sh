# called from slave-go.sh

sudo slave-lcd/lcd -b images/intro.rgb

IP=`cat master-ip.txt`
RSYNC_PASSWORD=tralala rsync -a incoming/ jcw@$IP::uploads/

slave-usbot/usbot >/tmp/info.txt

FBID=""
. /tmp/info.txt
if [ "$FBID" != "" ]
then
  echo $FBID "$NAME" `date` >>slave-log.txt
  while slave-usbot/usbot
	do
		if [ -r /tmp/snap.jpg ]
		then
		  mv /tmp/snap.jpg /tmp/last.jpg
			convert /tmp/last.jpg -geometry 240x320 /tmp/last.rgb
			sudo slave-lcd/lcd -b /tmp/last.rgb
		fi
		sleep 0.1
	done
	mv /tmp/info.txt incoming/$FBID.txt
	mv /tmp/last.jpg incoming/$FBID.jpg
	sudo slave-lcd/lcd -b images/done.rgb
	while :
	do
		slave-usbot/usbot || exit
	done
fi
