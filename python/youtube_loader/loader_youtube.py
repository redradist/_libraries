from pytube import YouTube

# not necessary, just for demo purposes.
from pprint import pprint

yt = YouTube("https://www.safaribooksonline.com/library/view/design-patterns-in/9781491935828/part05.html?autoStart=True")

# Once set, you can see all the codec and quality options YouTube has made
# available for the perticular video by printing videos.

pprint(yt.get_videos())

# [<Video: MPEG-4 Visual (.3gp) - 144p>,
#  <Video: MPEG-4 Visual (.3gp) - 240p>,
#  <Video: Sorenson H.263 (.flv) - 240p>,
#  <Video: H.264 (.flv) - 360p>,
#  <Video: H.264 (.flv) - 480p>,
#  <Video: H.264 (.mp4) - 360p>,
#  <Video: H.264 (.mp4) - 720p>,
#  <Video: VP8 (.webm) - 360p>,
#  <Video: VP8 (.webm) - 480p>]

# The filename is automatically generated based on the video title.  You
# can override this by manually setting the filename.

# view the auto generated filename:
try:
   print(yt.filename)
except:
   print("Error, decoding finename !!")
   print("Maybe your console does not support decoding in utf-8.")
# Pulp Fiction - Dancing Scene [HD]

# set the filename:
yt.set_filename("vdgsgsdgsdgsdgsdgd")

# You can also filter the criteria by filetype.
pprint(yt.filter('flv'))

# [<Video: Sorenson H.263 (.flv) - 240p>,
#  <Video: H.264 (.flv) - 360p>,
#  <Video: H.264 (.flv) - 480p>]

# Notice that the list is ordered by lowest resolution to highest. If you
# wanted the highest resolution available for a specific file type, you
# can simply do:
print(yt.filter('mp4')[-1])
# <Video: H.264 (.mp4) - 720p>

# You can also get all videos for a given resolution
pprint(yt.filter(resolution='480p'))

# [<Video: H.264 (.flv) - 480p>,
#  <Video: VP8 (.webm) - 480p>]

# To select a video by a specific resolution and filetype you can use the get
# method.

video = yt.get('mp4', '360p')

# NOTE: get() can only be used if and only if one object matches your criteria.
# for example:

pprint(yt.videos)

#[<Video: MPEG-4 Visual (.3gp) - 144p>,
# <Video: MPEG-4 Visual (.3gp) - 240p>,
# <Video: Sorenson H.263 (.flv) - 240p>,
# <Video: H.264 (.flv) - 360p>,
# <Video: H.264 (.flv) - 480p>,
# <Video: H.264 (.mp4) - 360p>,
# <Video: H.264 (.mp4) - 720p>,
# <Video: VP8 (.webm) - 360p>,
# <Video: VP8 (.webm) - 480p>]

# Since we have two H.264 (.mp4) available to us... now if we try to call get()
# on mp4...

video = yt.get('mp4')
# MultipleObjectsReturned: 2 videos met criteria.

# In this case, we'll need to specify both the codec (mp4) and resolution
# (either 360p or 720p).

# Okay, let's download it! (a destination directory is required)
video.download('./')