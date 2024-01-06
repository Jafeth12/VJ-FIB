extends StaticBody3D

signal player_entered_platform()
signal player_exited_platform()


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	look_at(Vector3(0, get_position().y, 0), Vector3.UP)


func _on_area_body_entered(body):
	if body is Player:
		emit_signal("player_entered_platform")


func _on_area_body_exited(body):
	if body is Player:
		emit_signal("player_exited_platform")
