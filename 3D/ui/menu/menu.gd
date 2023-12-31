extends Control

func _ready():
	MusicController.play_menu_music()
	SceneTransitions.fade_in()
	Input.set_mouse_mode(Input.MOUSE_MODE_VISIBLE)

func _on_play_button_up() -> void:
	MainLogic.reset_all()
	SceneTransitions.change_scene_with_middle_scene("res://main.tscn", "res://ui/menu/views/planets/water_planet.tscn")

func _on_settings_button_down():
	SceneTransitions.change_scene("res://ui/menu/views/settings/settings.tscn")

func _on_controls_button_up():
	SceneTransitions.change_scene("res://ui/menu/views/controls/controls.tscn")

func _on_leaderboard_button_up():
	SceneTransitions.change_scene("res://ui/menu/views/leaderboard/leaderboard.tscn")

func _on_credits_button_up():
	SceneTransitions.change_scene("res://ui/menu/views/credits/credits.tscn")

func _on_quit_button_up() -> void:
	SceneTransitions.fade_out()
	await SceneTransitions.animation_player.animation_finished
	get_tree().quit()
