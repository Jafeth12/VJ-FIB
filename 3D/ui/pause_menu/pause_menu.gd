extends Control


# Called when the node enters the scene tree for the first time.
func _ready():
	hide()
	process_mode = Node.PROCESS_MODE_WHEN_PAUSED # Solo corre cuando el tree está pausado
	$voptions/resume.connect("button_down", _on_resume_button_down)
	$voptions/exit.connect("button_down", _on_exit_button_down)

func _input(event):
	if event.is_action_pressed("pause"):
		_on_resume_button_down()

func _on_resume_button_down() -> void:
	hide()
	get_tree().paused = false

func _on_exit_button_down() -> void:
	hide()
	get_tree().paused = false
	MainLogic.finish_timer_died()
	SceneTransitions.change_scene("res://ui/menu/menu.tscn")
