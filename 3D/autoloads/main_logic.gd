extends Node

enum LEVEL { LEVEL1 = 1, LEVEL2 = 2, END = 3 }

class PlayerState:
	var health: int
	var ammo_pistol: int
	var ammo_rifle: int
	var has_rifle: bool

@onready var current_level: LEVEL = LEVEL.LEVEL1
@onready var player_info: PlayerState = PlayerState.new()

var scores: Array = []

func _ready():
	add_score(10)
	add_score(100)
	add_score(500)
	add_score(1)	
	reset_all()

func get_current_level() -> LEVEL:
	return current_level

func go_to_next_level() -> LEVEL:
	match current_level:
		LEVEL.LEVEL1:
			current_level = LEVEL.LEVEL2
		LEVEL.LEVEL2:
			current_level = LEVEL.END
	return current_level

func save_player_state(info: PlayerState) -> void:
	player_info = info

func get_player_state() -> PlayerState:
	return player_info

func reset_level() -> void:
	current_level = LEVEL.LEVEL1

func reset_player_state() -> void:
	player_info.health = Player.INIT_HEALTH
	player_info.ammo_pistol = Player.MAX_AMMO_PISTOL
	player_info.ammo_rifle = Player.MAX_AMMO_RIFLE
	player_info.has_rifle = Player.INIT_HAS_RIFLE

func reset_all() -> void:
	reset_level()
	reset_player_state()

func add_score(time: int) -> void:
	scores.push_back(time)
	scores.sort()
	
func get_scores() -> Array:
	return scores
