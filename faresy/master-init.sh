# main master logic, this is launched from master-go.sh

PATH=/usr/local/bin:$PATH
INDIR=master-node/client/static/uploads
OUTDIR=$HOME/Dropbox/Faresy/Registrations

# keep looking for new print jobs
while :
do
  for FBID in $(rsync -av $INDIR/*.{jpg,txt} $OUTDIR/ | sed -n -e '/\.txt$/s///p')
  do
    echo $FBID `date` >>master-log.txt
    ./master-pdf.sh $FBID
  done
  sleep 10
done &

# keep the web server running, nodemon will auto-restart on all file changes
cd master-node
while :
do
  nodemon --exitcrash app.js
	sleep 10
done #>../master-log.txt 2>&1
