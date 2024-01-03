extends Node

@onready var player = get_node("/root/main/Player")
@onready var platforms = get_tree().get_nodes_in_group("lvl1_platforms")
@onready var chests = get_tree().get_nodes_in_group("lvl1_chests")
@onready var enemies = get_tree().get_nodes_in_group("lvl1_enemies")

signal level_ended

var active_chest = null

var height1_enemies: int = 0
var height2_enemies: int = 0
var height3_enemies: int = 0

var is_player_ready_to_next_height: bool = false
var is_player_ready_to_next_level: bool = false

# Called when the node enters the scene tree for the first time.
func _ready():
	randomize()

	for platform in platforms:
		platform.connect("player_entered_platform", on_player_entered_platform)
		platform.connect("player_exited_platform", on_player_exited_platform)
		
	for chest in chests:
		chest.connect("player_entered_area", _on_chest_player_entered_area)
		chest.connect("player_exited_area", _on_chest_player_exited_area)

	for enemy in enemies:
		if enemy.is_in_group("lvl1_height1_enemies"):
			height1_enemies += 1
		elif enemy.is_in_group("lvl1_height2_enemies"):
			height2_enemies += 1
		elif enemy.is_in_group("lvl1_height3_enemies"):
			height3_enemies += 1

		enemy.connect("enemy_died", _on_enemy_died)

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
	if Input.is_action_just_pressed("change_level_height"):
		if is_player_ready_to_next_height:
			$next_height.hide_interaction()
		if is_player_ready_to_next_level:
			$next_level.hide_interaction()
			emit_signal("level_ended")

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

# --- Enemies ---

func _on_enemy_died(enemy):
	if enemy.is_in_group("lvl1_height1_enemies"):
		height1_enemies -= 1
		check_if_all_enemies_are_dead(height1_enemies)
	elif enemy.is_in_group("lvl1_height2_enemies"):
		height2_enemies -= 1
		check_if_all_enemies_are_dead(height2_enemies)
	elif enemy.is_in_group("lvl1_height3_enemies"):
		height3_enemies -= 1
		check_if_can_go_to_next_level()

func check_if_all_enemies_are_dead(height_enemies) -> void:
	if height_enemies == 0:
		$next_height.show_interaction()
		is_player_ready_to_next_height = true
		player.player_set_ready_to_next_height(true)

func check_if_can_go_to_next_level() -> void:
	if height3_enemies == 0:
		$next_level.show_interaction()
		is_player_ready_to_next_height = true
