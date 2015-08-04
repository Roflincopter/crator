#version 330

uniform vec2 screen_size;
uniform sampler1D pos_wave_form;
uniform sampler1D neg_wave_form;
uniform sampler1D energy;
out vec4 outColor;

void main()
{
	float x = trunc(gl_FragCoord.x);
	float y = trunc(gl_FragCoord.y);
	
	float x_ratio = x / screen_size.x;
	
	float neg_audio_sample = texture1D(neg_wave_form, x_ratio).r;
	float pos_audio_sample = texture1D(pos_wave_form, x_ratio).r;
	
	int last_pos_pixel_to_color = int(pos_audio_sample * (screen_size.y / 2));
	int last_neg_pixel_to_color = int(neg_audio_sample * (screen_size.y / 2));
	last_pos_pixel_to_color = int(screen_size.y / 2) + int(last_pos_pixel_to_color / 1.2);
	last_neg_pixel_to_color = int(screen_size.y / 2) - int(last_neg_pixel_to_color / 1.2);
	
	if(all(bvec2(
		int(y) <= last_pos_pixel_to_color,
		int(y) >= last_neg_pixel_to_color
	))) {
		vec3 color = mix(vec3(0.0, 1.0, 0.0), vec3(1.0, 0.0, 0.0), texture1D(energy, x_ratio).r);
		outColor = vec4(color, 1.0);
	} else {
		outColor = vec4(0,0,0,1);
	}
}