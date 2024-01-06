extends StaticBody3D

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	look_at(Vector3(0, get_position().y, 0), Vector3.UP)
