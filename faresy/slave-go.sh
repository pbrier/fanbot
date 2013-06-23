# put this line in crontab (crontab -e):
#   @reboot fanbot/faresy/slave-go.sh

cd ~/fanbot/faresy

# briefly show our own IP address on the display
IP=$(ip addr list eth0 | sed -n -e '/inet /s/\/.*//' -e 's/inet // p')
sudo slave-lcd/lcd -m "$IP"

git pull # always get the latest changes before doing any further setup

sh slave-init.sh
