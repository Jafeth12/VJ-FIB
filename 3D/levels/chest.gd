extends StaticBody3D

signal player_entered_area(chest: Node)
signal player_exited_area(chest: Node)

@onready var player = get_node("/root/main/Player")
var has_been_opened: bool = false
var player_in_range: bool = false

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	pass

func can_open() -> bool:
	return !has_been_opened && player_in_range

func open():
	if !can_open():
		return

	var give_ammo = (player.player_has_max_health() || randi_range(0, 1) == 1)

	if give_ammo:
		var ammo_amount = randi_range(10, 30)
		player.player_give_ammo(ammo_amount)
	else:
		var health_amount = randi_range(30, 90)
		player.player_give_health(health_amount)

	has_been_opened = true

# ------------------------------

func _on_body_entered(body):
	if body is Player:
		player_in_range = true
		emit_signal("player_entered_area", self)

func _on_body_exited(body):
	if body is Player:
		player_in_range = false
		emit_signal("player_exited_area", self)
