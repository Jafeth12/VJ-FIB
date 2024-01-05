extends CanvasLayer

@onready var animation_player = $AnimationPlayer

signal transition_ended()

func _ready() -> void:
	animation_player.connect("animation_finished", _on_animation_finished)

func _on_animation_finished(animation):
	emit_signal("transition_ended")

func change_scene(path_to_target_scene: String, speed: float = 1.0) -> void:
	fade_out(speed)
	await animation_player.animation_finished
	get_tree().change_scene_to_file(path_to_target_scene)
	fade_in(speed)

func change_scene_fade_in(path_to_target_scene: String, speed: float = 1.0) -> void:
	get_tree().change_scene_to_file(path_to_target_scene)
	fade_in(speed)

func fade_in(speed: float = 1.0) -> void:
	animation_player.play("dissolve", -1, -speed, true)

func fade_out(speed: float = 1.0) -> void:
	animation_player.play("dissolve", -1, speed, false)

func change_scene_with_middle_scene(target_scene: String, middle_scene: String, middle_scene_time: float = 5.0, speed: float = 1.0) -> void:
	# Setup timer
	var timer = Timer.new()
	timer.one_shot = true
	add_child(timer)
	# Change to the middle scene
	change_scene(middle_scene, speed)
	# await for the timer. Free it when it's done
	timer.start(middle_scene_time)
	await timer.timeout
	timer.queue_free()
	# Change to the final scene
	change_scene(target_scene, speed)
