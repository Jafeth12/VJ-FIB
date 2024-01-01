extends Node

@onready var player = get_node("/root/main/Player")
@onready var platforms = get_tree().get_nodes_in_group("lvl1_platforms")
@onready var chests = get_tree().get_nodes_in_group("lvl1_chests")

var active_chest = null

# Called when the node enters the scene tree for the first time.
func _ready():
	randomize()

	for platform in platforms:
		platform.connect("player_entered_platform", on_player_entered_platform)
		platform.connect("player_exited_platform", on_player_exited_platform)
		
	for chest in chests:
		chest.connect("player_entered_area", _on_chest_player_entered_area)
		chest.connect("player_exited_area", _on_chest_player_exited_area)

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	handle_input()
	if active_chest != null:
		if active_chest.can_open():
			$open_chest.show_interaction()
		else:
			$open_chest.hide_interaction()
	else:
		$open_chest.hide_interaction()
	
func handle_input() -> void:
	if Input.is_action_just_pressed("interact"):
		if active_chest != null:
			if active_chest.can_open():
				active_chest.open()

func on_player_entered_platform() -> void:
	player.player_set_on_platform(true)
	$switch_circle.show_interaction()
	
func on_player_exited_platform() -> void:
	player.player_set_on_platform(false)
	$switch_circle.hide_interaction()

# --- Chests ---

func _on_chest_player_entered_area(chest):
	if chest.can_open():
		active_chest = chest

func _on_chest_player_exited_area(_chest):
	active_chest = null
