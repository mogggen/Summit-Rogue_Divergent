ffmpeg -i "$1" -vf "crop=iw:ih/2:0:0" "${1%.*}-0.png"
ffmpeg -i "$1" -vf "crop=iw:ih/2:0:ih/2" "${1%.*}-1.png"