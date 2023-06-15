#include "register_types.h"

#include "./port_audio.h"
#include "./port_audio_callback_data.h"
#include "./port_audio_stream.h"
#include "./port_audio_stream_parameter.h"

#include "./port_audio_test_node.h"

#include "core/object/class_db.h"
#include "core/config/engine.h"

static PortAudio *port_audio = nullptr;

void initialize_portaudio_module(ModuleInitializationLevel p_level) {
	// Singleton
	if (p_level == MODULE_INITIALIZATION_LEVEL_SERVERS)
	{
		port_audio = memnew(PortAudio);
		GDREGISTER_CLASS(PortAudio);
		Engine::get_singleton()->add_singleton(Engine::Singleton("PortAudio", PortAudio::get_singleton()));

		// Port Audio
		GDREGISTER_CLASS(PortAudioStream);
		GDREGISTER_CLASS(PortAudioStreamParameter);
		GDREGISTER_CLASS(PortAudioCallbackData);

		// Nodes
		GDREGISTER_CLASS(PortAudioTestNode);
	}
}

void uninitialize_portaudio_module(ModuleInitializationLevel p_level)
{
	if (p_level == MODULE_INITIALIZATION_LEVEL_SERVERS)
	{
		memdelete(port_audio);
	}
}
