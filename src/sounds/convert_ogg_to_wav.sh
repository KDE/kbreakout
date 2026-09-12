# from https://stackoverflow.com/questions/62263919/how-to-batch-convert-ogg-files-to-wav-using-a-command-line-utility
for i in *.ogg; do
  ffmpeg -acodec libvorbis -i "$i" -acodec pcm_s16le "${i%ogg}wav"
done
