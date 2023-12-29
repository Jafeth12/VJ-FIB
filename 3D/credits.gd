extends Control

func _ready():
	print("pa ti mi cola")

func _on_back_button_up():
	get_tree().change_scene_to_file("res://menu.tscn")
