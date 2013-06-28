# generate a PDF from specified ID and send it to the printer

INDIR=master-node/client/static/uploads
OUTDIR=$HOME/Dropbox/Faresy/Passports

# this sets FBID and NAME
source $INDIR/$1.txt 2>/dev/null

if [ -z "$FBID" ]
then
  echo "Usage: $0 <8-char-fanbot-id>"
  exit 1
fi

msg1='Gratis ophalen op maandag 1 juli tussen 11:00 en 14:00
Indoor Sportcentrum, Theo Koomenlaan 1 in Eindhoven
Breng dit paspoort mee!'
msg2='Koop een envelop bij de workshop (€10,-).
Schrijf je adres erop en stop dit paspoort erin.
Op 1 juli worden de FanBots verzameld en verstuurd.'

convert \
  -size 750x1000 canvas:none \
  $INDIR/$FBID.jpg \
  -resize 75% \
  -bordercolor white -border 20x50 \
  -gravity NorthWest \
  \( \
    -pointsize 24 \
    -draw "text 20,10 'FanBot naam: $NAME'" \
    -draw "text 400,50 'Paspoort'" \
    -draw "text 400,80 '$FBID'" \
  \) -layers flatten \
  -pointsize 15 \
  -weight Bold -draw "text 20,550 'FanBot thuis?'" -weight normal \
  -draw "text 20,565 '$msg1'" \
  -weight Bold -draw "text 20,625 'FanBot via de Post?'" -weight normal \
  -draw "text 20,640 '$msg2'" \
  -draw "text 400,655 'Vragen, opmerkingen?'" \
  -draw "text 400,670 'marieke@kekbot.org'" \
  -draw "image SrcOver 20,740,560,85 images/logobar2.jpg" \
  -page A4 \
  $OUTDIR/$FBID.pdf

osascript - <<EOF
tell application "Samsung ML-2525W Series"
print "$OUTDIR/$FBID.pdf" without print dialog
end tell
EOF
