extends Node

@onready var player = get_node("root/main/Player")

# Called when the node enters the scene tree for the first time.
func _ready():
	var platforms = get_tree().get_nodes_in_group("lvl1_platforms")
	for platform in platforms:
		platform.connect("player_entered_platform", on_player_entered_platform)
		platform.connect("player_exited_platform", on_player_exited_platform)
		
	var chests = get_tree().get_nodes_in_group("lvl1_chests")
	for chest in chests:
		chest.connect("player_entered_area", _on_chest_player_entered_area)
		chest.connect("player_exited_area", _on_chest_player_exited_area)

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass

func on_player_entered_platform() -> void:
	player.player_set_on_platform(true)
	$switch_circle.show_interaction()
	
func on_player_exited_platform() -> void:
	player.player_set_on_platform(false)
	$switch_circle.hide_interaction()

func _on_chest_player_entered_area():
	$open_chest.show_interaction()

func _on_chest_player_exited_area():
	$open_chest.hide_interaction()
