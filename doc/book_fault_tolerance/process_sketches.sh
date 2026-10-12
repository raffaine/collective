#!/bin/bash
RAW_DIR="images/chapters/raw"
OUT_DIR="images/chapters"

for img in "$RAW_DIR"/*.jpg; do
  [ -e "$img" ] || continue
  filename=$(basename "$img")
  echo "Fading edges for $filename..."
  
  magick "$img" \
    \( +clone -fill black -colorize 100 \
       -fill white -draw "roundrectangle 60,60 %[fx:w-60],%[fx:h-60] 150,150" \
       -blur 0x80 \) \
    -alpha off -compose copy_opacity -composite \
    -background white -compose over -flatten \
    "$OUT_DIR/$filename"
done
echo "All sketches processed!"
