package main

// #cgo LDFLAGS: -L../out/Release/lib -lpackager_ext
// #include <stdlib.h>
// #include <string.h>
// #include "packager_ext.h"
import "C"
import "unsafe"
import "io/ioutil"
import "fmt"
import "os"

type (
	Char   C.char
)

type Foo struct {
	ptr unsafe.Pointer
}

func NewFoo() Foo {
	var foo Foo
	foo.ptr = C.Packager_New()
	return foo
}

func (foo Foo) Run() {
	C.Packager_Run(foo.ptr)
}

func (foo Foo) Init(s string) int {
	cstr := C.CString(s)
	defer C.free(unsafe.Pointer(cstr))
	return int(C.Packager_Initialize(foo.ptr, cstr))
}

func (foo Foo) Destroy() {
	C.Packager_Destroy(foo.ptr)
}



func main() {
	foo := NewFoo()
	json_params := `{
		"streams": [
			{
			  "in": "udp://239.0.0.1:1234?buffer_size=1200000&reuse=1",
			  "stream":"video",
			  "init_segment":"/shaka-results/bitrate_3/video_init.mp4",
			  "segment_template":"/shaka-results/bitrate_3/video_$Number$.m4s",
			  "playlist_name":"/shaka-results/bitrate_3/video.m3u8"
			},
			{
			  "in":"pipe0",
			  "stream":"text",
			  "format":"ttml+mp4",
			  "skip_encryption":1,
			  "init_segment":"/shaka-results/bitrate_10/text_init.mp4",
			  "segment_template":"/shaka-results/bitrate_10/text_$Number$.mp4",
			  "lang":"ru",
			  "playlist_name":"/shaka-results/text0.m3u8"
			}
		],
		"hls_master_playlist_output":"/shaka-results/demo_master1.m3u8",
		"mpd_output" :"/shaka-results/manifest1.mpd" ,
		"hls_playlist_type" :"LIVE",
		"segment_duration": 2 ,
		"min_buffer_time": 4 ,
		"suggested_presentation_delay": 10 ,
		"time_shift_buffer_depth": 12 ,
		"allow_approximate_segment_timeline": true,
		"preserved_segments_outside_live_window": 20
		}`
	if len(os.Args) > 1 {
		content, err := ioutil.ReadFile(os.Args[1])
		if err != nil {
			fmt.Println(err)
			return
		}
		json_params = string(content)
	}
	result := foo.Init(json_params)
	defer foo.Destroy()
	fmt.Println("[packager] initialize res", result)
	foo.Run()
	fmt.Println("[packager] finished", result)
}