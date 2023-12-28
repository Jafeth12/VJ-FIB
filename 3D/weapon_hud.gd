extends Control

@export var weapon_tex: Texture2D

var ammo: int = 30

# Called when the node enters the scene tree for the first time.
func _ready():
	$weapon.texture = weapon_tex
	

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass
	

# --------------------------------

func set_ammo(value: int):
	if value < 0:
		value = 0

	ammo = value
	$ammo_label.text = str(ammo)

