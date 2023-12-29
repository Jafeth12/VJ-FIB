extends StaticBody3D

signal player_entered_platform()
signal player_exited_platform()

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass


func _on_area_body_entered(body):
	if body is Player:
		emit_signal("player_entered_platform")


func _on_area_body_exited(body):
	if body is Player:
		emit_signal("player_exited_platform")
