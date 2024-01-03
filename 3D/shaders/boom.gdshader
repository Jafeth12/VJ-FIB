shader_type canvas_item;

uniform bool is_pistol = true;

void vertex() {
	// Called for every vertex the material is visible on.
}

void fragment() {
	// Called for every pixel the material is visible on.
	vec3 pistol_color = vec3(0, 1, 1);
	vec3 rifle_color = vec3(0, 0, 1);
	if (is_pistol) {
		COLOR.rgb = COLOR.rgb * pistol_color;
	} else {
		COLOR.rgb = COLOR.rgb * rifle_color;
	}
}

void light() {
	// Called for every pixel for every light affecting the CanvasItem.
}
