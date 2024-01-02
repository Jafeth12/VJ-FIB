extends Node

@onready var hud = $CanvasLayer/HUD
@onready var animation_player = $CanvasLayer/AnimationPlayer

func _ready():
	animation_player.play("fade_in")
	await animation_player.animation_finished
	pass

func _process(_delta):
	pass

# -----------------------------

func _on_player_died():
	# $Player.player_reset()
	# $level1.reset()
	pass # Replace with function body.
