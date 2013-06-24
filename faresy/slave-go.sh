# put this line in crontab (crontab -e):
#   @reboot fanbot/faresy/slave-go.sh

cd ~/fanbot/faresy

# briefly show our own IP address on the display
IP=$(ip addr list eth0 | sed -n -e '/inet /s/\/.*//' -e 's/inet // p')
sudo slave-lcd/lcd -m "$IP"
sleep 3

git pull # always get the latest changes before doing any further setup

# build the apps, in case a source change just came in
(cd slave-lcd && make)
(cd slave-usbot && make)

sh slave-init.sh
