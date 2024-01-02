extends Control

@onready var animation_player = $CanvasLayer/AnimationPlayer
@onready var music_controller = $"/root/MusicController"

var fade_in_complete = false
var fade_out_complete = false

func _ready():
	music_controller.play_menu_music()
	animation_player.play("fade_in")

func _process(_delta) -> void:
	pass

func _on_play_button_up() -> void:
	animation_player.play("fade_out")
	await animation_player.animation_finished
	music_controller.play_level_music()
	get_tree().change_scene_to_file("res://main.tscn")


func _on_quit_button_up() -> void:
	animation_player.play("fade_out")
	await animation_player.animation_finished
	get_tree().quit()

func _on_credits_button_up():
	get_tree().change_scene_to_file("res://ui/menu/views/credits/credits.tscn")

func _on_controls_button_up():
	get_tree().change_scene_to_file("res://ui/menu/views/controls/controls.tscn")
