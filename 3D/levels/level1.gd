extends Node

var player = null

# Called when the node enters the scene tree for the first time.
func _ready():
	player = get_node("/root/main/Player")
	var platforms = get_tree().get_nodes_in_group("lvl1_platforms")
	for platform in platforms:
		platform.connect("player_entered_platform", on_player_entered_platform)
		platform.connect("player_exited_platform", on_player_exited_platform)

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass

func on_player_entered_platform() -> void:
	player.player_set_on_platform(true)
	$switch_circle.show_interaction()
	
func on_player_exited_platform() -> void:
	player.player_set_on_platform(false)
	$switch_circle.hide_interaction()
