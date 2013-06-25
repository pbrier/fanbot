# called from slave-go.sh

sudo slave-lcd/lcd -b images/intro.rgb

# wait a while for a FanBot to be plugged in
slave-usbot/usbot >/tmp/info.txt

FBID=""
. /tmp/info.txt
if [ "$FBID" != "" ]
then
  # only reached when readout succeeded
  echo $FBID "$NAME" `date` >>slave-log.txt
  # show latest image on LCD as long as the FanBot remains plugged in
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
  # done, prepare to send out a new registration
  mv -f /tmp/info.txt incoming/$FBID.txt
  mv -f /tmp/last.jpg incoming/$FBID.jpg
  sudo slave-lcd/lcd -b images/done.rgb
  sleep 1
fi
