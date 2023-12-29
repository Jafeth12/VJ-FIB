extends Control

func _ready():
	$"/root/MusicController".play_menu_music()

func _process(_delta) -> void:
	pass

func _on_play_button_up() -> void:
	$"/root/MusicController".play_level_music()
	get_tree().change_scene_to_file("res://main.tscn")

func _on_quit_button_up() -> void:
	get_tree().quit()

func _on_credits_button_up():
	get_tree().change_scene_to_file("res://credits.tscn")

func _on_controls_button_up():
	get_tree().change_scene_to_file("res://controls.tscn")
