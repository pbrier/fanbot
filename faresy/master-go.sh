# put this line in crontab (crontab -e):
#   @reboot fanbot/faresy/master-go.sh

PATH=/usr/local/bin:$PATH
cd ~/w/fanbot/faresy/master-node

while :
do
  nodemon --exitcrash app.js
	sleep 10
done >../master-log.txt 2>&1
