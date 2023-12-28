extends Control

@export var weapon_tex: Texture2D
@export var new_material: Material

# Called when the node enters the scene tree for the first time.
func _ready():
	$weapon.texture = weapon_tex
	$box.material = new_material
	$weapon.material = new_material
	$ammo.material = new_material
	$ammo_label.material = new_material

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	pass

# --------------------------------

func set_ammo(value: int) -> void:
	if value < 0:
		value = 0

	$ammo_label.text = str(value)

func enable() -> void:
	$box.material.set_shader_parameter("disabled", false)
	$weapon.material.set_shader_parameter("disabled", false)
	$ammo.material.set_shader_parameter("disabled", false)
	$ammo_label.material.set_shader_parameter("disabled", false)

func disable() -> void:
	$box.material.set_shader_parameter("disabled", true)
	$weapon.material.set_shader_parameter("disabled", true)
	$ammo.material.set_shader_parameter("disabled", true)
	$ammo_label.material.set_shader_parameter("disabled", true)

