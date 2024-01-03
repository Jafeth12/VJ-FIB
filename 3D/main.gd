extends Node

@onready var hud = $CanvasLayer/HUD
var timer = Timer.new()

func _ready():
	MusicController.play_level_music()
	pass

func _process(_delta):
	pass

# -----------------------------

func _on_player_died():
	SceneTransitions.fade_out()
	$Player.player_revive()
	$Player.player_reset_position()
	SceneTransitions.fade_in()
