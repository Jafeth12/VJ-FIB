shader_type canvas_item;

uniform bool disabled = false;

void vertex() {
	// Called for every vertex the material is visible on.
}

void fragment() {
	// Called for every pixel the material is visible on.
	if (disabled) {
		COLOR.rgb = COLOR.rgb * vec3(0.4, 0.4, 0.4);
	}
}

void light() {
	// Called for every pixel for every light affecting the CanvasItem.
}
