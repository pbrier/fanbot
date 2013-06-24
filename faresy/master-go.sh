# put this line in crontab (crontab -e):
#   @reboot fanbot/faresy/master-go.sh

cd ~/w/fanbot/faresy

git pull # always get the latest changes before doing any further setup

sh master-init.sh
