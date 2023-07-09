#ifndef PORT_AUDIO_H
#define PORT_AUDIO_H

#ifdef PORT_AUDIO
#include "port_audio_stream.h"

#include "core/io/stream_peer.h"
#include "core/object/object.h"
#include "drivers/portaudio/virtual_audio_driver.h"
#include "port_audio_callback_data.h"
#include "servers/audio_server.h"

#include <map>

class PortAudioVirtualDriver;

class PortAudio : public Object {
	GDCLASS(PortAudio, Object);

	friend class PortAudioVirtualDriver;
	friend class EditorNode;
	Ref<PortAudioStream> main_stream;

public:
	const PackedFloat64Array standard_sample_rates = {
		8000.0,
		9600.0,
		11025.0,
		12000.0,
		16000.0,
		22050.0,
		24000.0,
		32000.0,
		44100.0,
		48000.0,
		88200.0,
		96000.0,
		192000.0
	};
	const PackedInt32Array standard_samples_per_block = {
		32, 48, 64, 96, 128, 196, 256, 384, 512, 768, 1024, 1536, 2048
	};
	const PackedInt32Array stream_resolution = { 4, 4, 3, 2, 1, 1 };
	enum PortAudioError {
		// Custom Error
		UNDEFINED = -1,
		NOT_PORT_AUDIO_NODE = -2,
		INVALID_FUNC_REF = -3,
		STREAM_NOT_FOUND = -4,
		STREAM_USER_DATA_NOT_FOUND = -5,
		// PortAudio Library Error
		NO_ERROR = 0,
		NOT_INITIALIZED = -10000,
		UNANTICIPATED_HOST_ERROR,
		INVALID_CHANNEL_COUNT,
		INVALID_SAMPLE_RATE,
		INVALID_DEVICE,
		INVALID_FLAG,
		SAMPLE_FORMAT_NOT_SUPPORTED,
		BAD_IO_DEVICE_COMBINATION,
		INSUFFICIENT_MEMORY,
		BUFFER_TO_BIG,
		BUFFER_TOO_SMALL,
		NULL_CALLBACK,
		BAD_STREAM_PTR,
		TIMED_OUT,
		INTERNAL_ERROR,
		DEVICE_UNAVAILABLE,
		INCOMPATIBLE_HOST_API_SPECIFIC_STREAM_INFO,
		STREAM_IS_STOPPED,
		STREAM_IS_NOT_STOPPED,
		INPUT_OVERFLOWED,
		OUTPUT_UNDERFLOWED,
		HOST_API_NOT_FOUND,
		INVALID_HOST_API,
		CAN_NOT_READ_FROM_A_CALLBACK_STREAM,
		CAN_NOT_WRITE_TO_A_CALLBACK_STREAM,
		CAN_NOT_READ_FROM_AN_OUT_PUTONLY_STREAM,
		CAN_NOT_WRITE_TO_AN_INPUT_ONLY_STREAM,
		INCOMPATIBLE_STREAM_HOST_API,
		BAD_BUFFER_PTR
	};

	enum PortAudioCallbackResult {
		CONTINUE = 0,
		COMPLETE = 1,
		ABORT = 2,
	};

	enum StandardSampleRates {
		SAMPLERATE_8000_0,
		SAMPLERATE_9600_0,
		SAMPLERATE_11025_0,
		SAMPLERATE_12000_0,
		SAMPLERATE_16000_0,
		SAMPLERATE_22050_0,
		SAMPLERATE_24000_0,
		SAMPLERATE_32000_0,
		SAMPLERATE_44100_0,
		SAMPLERATE_48000_0,
		SAMPLERATE_88200_0,
		SAMPLERATE_96000_0,
		SAMPLERATE_192000_0,
	};

	enum SamplesPerBlock {
		SAMPLES_PER_BLOCK_32,
		SAMPLES_PER_BLOCK_48,
		SAMPLES_PER_BLOCK_64,
		SAMPLES_PER_BLOCK_96,
		SAMPLES_PER_BLOCK_128,
		SAMPLES_PER_BLOCK_192,
		SAMPLES_PER_BLOCK_256,
		SAMPLES_PER_BLOCK_384,
		SAMPLES_PER_BLOCK_512,
		SAMPLES_PER_BLOCK_768,
		SAMPLES_PER_BLOCK_1024,
		SAMPLES_PER_BLOCK_1536,
		SAMPLES_PER_BLOCK_2048,
	};

private:
	static PortAudio *singleton;
	bool initialized = false;
	bool ready = false;
	int bytes_per_sample = 4;
	std::map<Ref<PortAudioStream>, void *> data_map;
	short current_driver_idx = 0;
	//HashMap<short, short> current_devices_idx;
	PackedStringArray host_api_names;
	Vector<PackedStringArray> host_devices_names;
	Vector<PackedInt32Array> host_devices_name_idx;
	int samples_per_block = 512;

	short current_device_index = 0;

	PackedInt32Array ring_buffer_in;
	PackedInt32Array ring_buffer_out;
	// internal switch if stream
	// should be forwarded to
	// builtin-driver or PortAudio
	//bool native_streaming = false;

	void editor_node_register_signal(Object *p_ref);
	void on_project_settings_changed_editor();

	PortAudio::PortAudioCallbackResult internal_server_stream(const Ref<PortAudioCallbackData> &p_data);

	void init_main_stream();
	PortAudio::PortAudioError start_main_stream();
	PortAudio::PortAudioError stop_main_stream();
	PortAudio::PortAudioError finish_main_stream();

protected:
	static void _bind_methods();
	void _notification(int p_what);
	void on_project_settings_changed(String p_setting);

public:
	bool input_started = false;
	SafeFlag ring_buffer_out_ready;
	SafeFlag ring_buffer_in_ready;
	SafeFlag exit_main_stream;
	SafeFlag pause_main_stream;
	static PortAudio *get_singleton();

	int get_main_stream_input_buffer(int32_t *r_buffer, int buffer_size);
	//const PackedInt32Array& get_main_stream_input_buffer();
	void push_main_stream_buffer(int32_t *p_buffer, int buffer_size);
	Ref<PortAudioStream> get_main_stream();

	String get_device_by_index_for_host(int p_current_host_api, int p_current_device_idx) const;
	void set_main_stream_mix_rate(double p_mix_rate);
	void change_audio_driver(const int p_stored_current_driver_idx, const int p_device_idx);
	int get_version();
	int get_current_driver_idx() const;
	//int get_main_stream_sample_size() const;
	String get_version_text();
	Dictionary get_version_info();
	String get_error_text(PortAudio::PortAudioError p_error);

	PortAudio::PortAudioError initialize();
	PortAudio::PortAudioError terminate();

	int get_host_api_count();
	int get_default_host_api();
	Dictionary get_host_api_info(int p_host_api);
	int host_api_type_id_to_host_api_index(int p_host_api_type_id);
	int host_api_device_index_to_device_index(int p_host_api, int p_host_api_device_index);
	Dictionary get_last_host_error_info();
	int get_device_count();
	int get_default_input_device();
	int get_default_output_device();

	int get_samples_per_block();
	int get_samples_per_block(const int p_device_index);
	PackedInt32Array get_available_buffer_sizes();
	PackedInt32Array get_available_buffer_sizes(const int p_device_index);
	void show_asio_control_panel();

	Dictionary get_device_info(int p_device_index);
	PortAudio::PortAudioError open_audio_server_stream(Ref<PortAudioStream> p_stream, PortAudioStreamParameter::PortAudioSampleFormat p_sample_format, Variant p_user_data);
	PortAudio::PortAudioError is_format_supported(Ref<PortAudioStreamParameter> p_input_stream_parameter, Ref<PortAudioStreamParameter> p_output_stream_parameter, double p_sample_rate);
	PortAudio::PortAudioError open_stream(Ref<PortAudioStream> p_stream, Callable p_audio_callback, Variant p_user_data);
	PortAudio::PortAudioError open_default_stream(Ref<PortAudioStream> p_stream, PortAudioStreamParameter::PortAudioSampleFormat p_sample_format, Callable p_audio_callback, Variant p_user_data);
	PortAudio::PortAudioError close_stream(Ref<PortAudioStream> p_stream);
	PortAudio::PortAudioError set_stream_finished_callback(Ref<PortAudioStream> p_stream, Callable p_stream_finished_callback);
	PortAudio::PortAudioError start_stream(Ref<PortAudioStream> p_stream);
	PortAudio::PortAudioError stop_stream(Ref<PortAudioStream> p_stream);
	PortAudio::PortAudioError abort_stream(Ref<PortAudioStream> p_stream);
	PortAudio::PortAudioError is_stream_stopped(Ref<PortAudioStream> p_stream);
	PortAudio::PortAudioError is_stream_active(Ref<PortAudioStream> p_stream);
	Dictionary get_stream_info(Ref<PortAudioStream> p_stream);
	double get_stream_time(Ref<PortAudioStream> p_stream);
	double get_stream_cpu_load(Ref<PortAudioStream> p_stream);
	PortAudio::PortAudioError read_stream(Ref<PortAudioStream> p_stream, PackedByteArray p_buffer, uint64_t p_frames);
	PortAudio::PortAudioError write_stream(Ref<PortAudioStream> p_stream, PackedByteArray p_buffer, uint64_t p_frames);
	int64_t get_stream_read_available(Ref<PortAudioStream> p_stream);
	int64_t get_stream_write_available(Ref<PortAudioStream> p_stream);
	PortAudio::PortAudioError get_sample_size(PortAudioStreamParameter::PortAudioSampleFormat p_sample_format);
	void sleep(unsigned int p_ms);

	PortAudio::PortAudioError util_device_index_to_host_api_index(int p_device_index);
	PortAudio::PortAudioError util_enable_exclusive_mode(Ref<PortAudioStreamParameter> p_stream_parameter);
	void util_insert_buffer(Ref<StreamPeerBuffer> p_source, int p_source_offset, Ref<StreamPeerBuffer> p_destination, int p_destination_offset, int p_length);
	void util_write_buffer(Ref<StreamPeerBuffer> p_source, Ref<StreamPeerBuffer> p_destination, int p_length);

	PortAudio();
	~PortAudio();
};

VARIANT_ENUM_CAST(PortAudio::PortAudioError);
VARIANT_ENUM_CAST(PortAudio::PortAudioCallbackResult);

#endif // PORT_AUIDO
#endif
