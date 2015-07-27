#version 330

uniform vec2 screen_size;
uniform sampler1D pos_wave_form;
uniform sampler1D neg_wave_form;
uniform vec2 min_max_value;
out vec4 outColor;

void main()
{
	float neg_audio_sample = texture1D(neg_wave_form, trunc(gl_FragCoord.x) / screen_size.x).r;
	float neg_sample_ratio = neg_audio_sample / min_max_value.x;

	float pos_audio_sample = texture1D(pos_wave_form, trunc(gl_FragCoord.x) / screen_size.x).r;
	float pos_sample_ratio = pos_audio_sample / min_max_value.y;
	
	int last_pos_pixel_to_color = int(trunc(pos_sample_ratio * (screen_size.y / 2)));
	int last_neg_pixel_to_color = int(trunc(neg_sample_ratio * (screen_size.y / 2)));
	last_pos_pixel_to_color = int(screen_size.y / 2) + last_pos_pixel_to_color;
	last_neg_pixel_to_color = int(screen_size.y / 2) - last_neg_pixel_to_color;
	
	if(all(bvec2(
		int(trunc(gl_FragCoord.y)) <= last_pos_pixel_to_color,
		int(trunc(gl_FragCoord.y)) >= last_neg_pixel_to_color
	))) {
		outColor = vec4(1.0, 0.0, trunc(gl_FragCoord.x) / screen_size.x, 1.0);
	} else {
		outColor = vec4(0,0.2,0,1);
	}
}