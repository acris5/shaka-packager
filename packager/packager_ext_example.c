#include <packager_ext.h>

int main() {
	void* packager = Packager_New();
	char json_string_params[] = " {\
      \"streams\": [ \
          {\
            \"in\": \"udp://239.0.0.1:1234?buffer_size=1200000&reuse=1\",\
            \"stream\":\"video\",\
            \"init_segment\":\"/shaka-results/bitrate_3/video_init.mp4\",\
            \"segment_template\":\"/shaka-results/bitrate_3/video_$Number$.m4s\",\
            \"playlist_name\":\"/shaka-results/bitrate_3/video.m3u8\"\
          },\
          {\
            \"in\":\"pipe0\",\
            \"stream\":\"text\",\
            \"format\":\"ttml+mp4\",\
            \"skip_encryption\":1,\
            \"init_segment\":\"/shaka-results/bitrate_10/text_init.mp4\",\
            \"segment_template\":\"/shaka-results/bitrate_10/text_$Number$.mp4\",\
            \"lang\":\"ru\",\
            \"playlist_name\":\"/shaka-results/text0.m3u8\"\
          },\
          {\
            \"in\":\"pipe0\",\
            \"stream\":\"scte35\"\
          }\
      ],\
      \"hls_master_playlist_output\":\"/shaka-results/demo_master1.m3u8\",\
      \"mpd_output\" :\"/shaka-results/manifest1.mpd\" ,\
      \"hls_playlist_type\" :\"LIVE\",\
      \"segment_duration\": 2 ,\
      \"min_buffer_time\": 4 ,\
      \"suggested_presentation_delay\": 10 ,\
      \"time_shift_buffer_depth\": 12 ,\
      \"allow_approximate_segment_timeline\": true,\
      \"preserved_segments_outside_live_window\": 20\
      }";
   Packager_Initialize(packager, json_string_params);
   Packager_Run(packager);
   return 0;
}