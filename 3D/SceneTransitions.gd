extends CanvasLayer

@onready var animation_player = $AnimationPlayer

func change_scene(path_to_target_scene: String, speed: float = 1.0) -> void:
	animation_player.play("dissolve", -1, speed, false)
	await animation_player.animation_finished
	get_tree().change_scene_to_file(path_to_target_scene)
	animation_player.play("dissolve", -1, -speed, true)

func fade_in(speed: float = 1.0) -> void:
	animation_player.play("dissolve", -1, -speed, true)

func fade_out(speed: float = 1.0) -> void:
	animation_player.play("dissolve", -1, speed, false)
