# put this line in crontab (crontab -e):
#   @reboot fanbot/faresy/slave-go.sh

cd ~/fanbot/faresy
git pull # always get the latest changes before doing any further setup
. slave-init.sh
