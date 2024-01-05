extends Node

enum LEVEL { LEVEL1 = 1, LEVEL2 = 2, END = 3 }

class PlayerState:
	var health: int
	var ammo_pistol: int
	var ammo_rifle: int
	var has_rifle: bool

@onready var current_level: LEVEL = LEVEL.LEVEL1
@onready var player_info: PlayerState = PlayerState.new()
@onready var game_timer: Timer = null
const TOTAL_GAME_TIME: float = 120.0
var tmp_time_left: float = 0.0

var scores: Array = []

func _ready():
	add_score(10)
	add_score(100)
	add_score(500)
	add_score(1)
	add_score(10)
	add_score(100)
	add_score(500)
	add_score(1)
	add_score(500)
	add_score(1)
	add_score(500)
	add_score(1)
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

func add_score(time: float) -> void:
	scores.push_back(time)
	scores.sort()
	scores = scores.slice(0, 12)
	
func get_scores() -> Array:
	return scores

# ====== TIMER ======
# Comienza la cuenta atrás del timer
func init_timer() -> void:
	game_timer = Timer.new()
	game_timer.one_shot = true
	add_child(game_timer)
	game_timer.start(TOTAL_GAME_TIME)

# Detiene el timer so este está comenzado
func pause_timer() -> void:
	if game_timer == null:
		return
	if game_timer.is_stopped():
		return
	tmp_time_left = game_timer.time_left
	game_timer.stop()
	print("PAUSE. tl: ", tmp_time_left)

# Continua el timer si este está pausado. Te lo crea si no existe
func resume_timer() -> void:
	if game_timer == null:
		init_timer()
		return
	if !game_timer.is_stopped():
		return
	game_timer.start(tmp_time_left)
	print("RESUME. tl: ", tmp_time_left)

# Detiene el timer y retorna los segundos restantes (resultado)
func finish_timer_won() -> float:
	if game_timer == null:
		return -1
	if !game_timer.is_stopped():
		tmp_time_left = game_timer.time_left
		game_timer.stop()
	var score = TOTAL_GAME_TIME - tmp_time_left
	add_score(score)
	game_timer.queue_free()
	game_timer = null
	return score

# Detiene el timer y retorna los segundos restantes (resultado)
func finish_timer_died() -> void:
	if game_timer == null:
		return
	if !game_timer.is_stopped():
		game_timer.stop()
	game_timer.queue_free()
	game_timer = null
