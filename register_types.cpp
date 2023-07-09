#include "register_types.h"

#include "core/object/class_db.h"
#include "core/config/engine.h"

// if acting as a library the libgodot audio driver is used
#ifdef PORT_AUDIO
 #ifndef LIBRARY_ENABLED
  #include "./port_audio.h"
  #include "./port_audio_callback_data.h"
  #include "./port_audio_stream.h"
  #include "./port_audio_stream_parameter.h"

  #include "./port_audio_test_node.h"
  static PortAudio *port_audio = nullptr;
 #endif
#endif

void initialize_portaudio_module(ModuleInitializationLevel p_level) {
	// Singleton
	if (p_level == MODULE_INITIALIZATION_LEVEL_SERVERS)
	{
#ifdef PORT_AUDIO
 #ifndef LIBRARY_ENABLED
		port_audio = memnew(PortAudio);
		GDREGISTER_CLASS(PortAudio);
		Engine::get_singleton()->add_singleton(Engine::Singleton("PortAudio", PortAudio::get_singleton()));

		// Port Audio
		GDREGISTER_CLASS(PortAudioStream);
		GDREGISTER_CLASS(PortAudioStreamParameter);
		GDREGISTER_CLASS(PortAudioCallbackData);

		// Nodes
		GDREGISTER_CLASS(PortAudioTestNode);
 #endif
#endif
	}
}

void uninitialize_portaudio_module(ModuleInitializationLevel p_level)
{
	if (p_level == MODULE_INITIALIZATION_LEVEL_SERVERS)
	{
#ifdef PORT_AUDIO
 #ifndef LIBRARY_ENABLED
		memdelete(port_audio);
 #endif
#endif
	}
}
