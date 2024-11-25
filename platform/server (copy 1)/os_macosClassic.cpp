/*************************************************************************/
/*  OS_MacOSClassic.cpp                                                        */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                    http://www.godotengine.org                         */
/*************************************************************************/
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/
#include "servers/visual/visual_server_raster.h"
#include "servers/visual/rasterizer_dummy.h"
#include "os_macosClassic.h"
#include <stdio.h>
#include <stdlib.h>
#include "print_string.h"
#include "servers/physics/physics_server_sw.h"

#include "main/main.h"

#include <unistd.h>

int OS_MacOSClassic::get_video_driver_count() const {

	return 1;
}
const char * OS_MacOSClassic::get_video_driver_name(int p_driver) const {

	return "Dummy";
}
OS::VideoMode OS_MacOSClassic::get_default_video_mode() const {

	return OS::VideoMode(800,600,false);
}

void OS_MacOSClassic::initialize(const VideoMode& p_desired,int p_video_driver,int p_audio_driver) {

	args=OS::get_singleton()->get_cmdline_args();
	current_videomode=p_desired;
	main_loop=NULL;

	
	rasterizer = memnew( RasterizerDummy );

	visual_server = memnew( VisualServerRaster(rasterizer) );

	AudioDriverManagerSW::get_driver(p_audio_driver)->set_singleton();

	if (AudioDriverManagerSW::get_driver(p_audio_driver)->init()!=OK) {

		ERR_PRINT("Initializing audio failed.");
	}

	sample_manager = memnew( SampleManagerMallocSW );
	audio_server = memnew( AudioServerSW(sample_manager) );
	audio_server->init();
	spatial_sound_server = memnew( SpatialSoundServerSW );
	spatial_sound_server->init();
	spatial_sound_2d_server = memnew( SpatialSound2DServerSW );
	spatial_sound_2d_server->init();

	
	ERR_FAIL_COND(!visual_server);

	visual_server->init();
	//
	physics_server = memnew( PhysicsServerSW );
	physics_server->init();
	physics_2d_server = memnew( Physics2DServerSW );
	physics_2d_server->init();

	input = memnew( InputDefault );

	_ensure_data_dir();

		
}
void OS_MacOSClassic::finalize() {

	if(main_loop)
		memdelete(main_loop);
	main_loop=NULL;

	spatial_sound_server->finish();
	memdelete(spatial_sound_server);
	spatial_sound_2d_server->finish();
	memdelete(spatial_sound_2d_server);

	//if (debugger_connection_console) {
//		memdelete(debugger_connection_console);
//}

	audio_server->finish();
	memdelete(audio_server);
	memdelete(sample_manager);

	visual_server->finish();
	memdelete(visual_server);
	memdelete(rasterizer);
	
	physics_server->finish();
	memdelete(physics_server);

	physics_2d_server->finish();
	memdelete(physics_2d_server);

	memdelete(input);

	args.clear();
}

void OS_MacOSClassic::set_mouse_show(bool p_show) {


}
void OS_MacOSClassic::set_mouse_grab(bool p_grab) {

	grab=p_grab;
}
bool OS_MacOSClassic::is_mouse_grab_enabled() const {

	return grab;
}

int OS_MacOSClassic::get_mouse_button_state() const {

	return 0;
}

Point2 OS_MacOSClassic::get_mouse_pos() const {

	return Point2();
}

void OS_MacOSClassic::set_window_title(const String& p_title) {


}

void OS_MacOSClassic::set_video_mode(const VideoMode& p_video_mode,int p_screen) {


}
OS::VideoMode OS_MacOSClassic::get_video_mode(int p_screen) const {

	return current_videomode;
}
void OS_MacOSClassic::get_fullscreen_mode_list(List<VideoMode> *p_list,int p_screen) const {


}


MainLoop *OS_MacOSClassic::get_main_loop() const {

	return main_loop;
}

void OS_MacOSClassic::delete_main_loop() {

	if (main_loop)
		memdelete(main_loop);
	main_loop=NULL;
}

void OS_MacOSClassic::set_main_loop( MainLoop * p_main_loop ) {

	main_loop=p_main_loop;
	input->set_main_loop(p_main_loop);
}

bool OS_MacOSClassic::can_draw() const {

	return false; //can never draw
};


String OS_MacOSClassic::get_name() {

	return "Server";
}



void OS_MacOSClassic::move_window_to_foreground() {

}

void OS_MacOSClassic::set_cursor_shape(CursorShape p_shape) {


}

void OS_MacOSClassic::run() {

	force_quit = false;
	
	if (!main_loop)
		return;
		
	main_loop->init();
		
	while (!force_quit) {
	
		if (Main::iteration()==true)
			break;
	};
	
	main_loop->finish();
}

OS_MacOSClassic::OS_MacOSClassic() {

	AudioDriverManagerSW::add_driver(&driver_dummy);
	//adriver here
	grab=false;

};
