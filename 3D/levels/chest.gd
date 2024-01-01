extends StaticBody3D

signal player_entered_area
signal player_exited_area

var can_be_opened: bool = false

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	pass

func can_open() -> bool:
	return can_be_opened

func open():
	print("Chest opened")

# ------------------------------

func _on_body_entered(body):
	if body is Player:
		emit_signal("player_entered_area")
		can_be_opened = true

func _on_body_exited(body):
	if body is Player:
		emit_signal("player_exited_area")
		can_be_opened = false
